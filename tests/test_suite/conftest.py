import pytest
import subprocess
import yaml
import os
def pytest_addoption(parser):
    parser.addoption("--valgrind", action="store", default="2")
    parser.addoption("--timeout", action="store", default="1")
    parser.addoption("--check", action="store", default="0")

def pytest_collect_file(parent, path, *args, **kwargs):
    if path.ext == ".yml" and path.basename.startswith("test"):
        return YamlFile(path, parent)

def pytest_collection_modifyitems(config, items):
    for item in items:
        item.add_option(config)

class YamlFile(pytest.File):
    def collect(self):
        raw = yaml.safe_load(self.fspath.open())
        for name, spec in sorted(raw.items()):
            if spec ['type'] == "bash diff":
                yield BashDiffItem(name, self, spec)
            if spec ['type'] == "output diff":
                yield OutputDiffItem(name, self, spec)

class YamlItem(pytest.Item):
    def __init__(self, name, parent, spec):
        super(YamlItem, self).__init__(name, parent)
        self.spec = spec
        self.command = str.encode(self.spec["command"])
        if "expected" in self.spec:
            self.expected = self.spec["expected"]

    def add_option(self, config):
        self.config = config

    def runtest(self):
        tmp = self.command.decode()
        args = []

        sanity = self.config.getoption("--valgrind")
        my_timeout = int(self.config.getoption("--timeout"))
        if "timeout" in self.expected:
            my_timeout = self.expected["timeout"]
        check = self.config.getoption("--check")
        if sanity == "1":
            args.append("valgrind")
            args.append("--error-exitcode=1")

        cwd = os.getcwd()
        if type(self) is BashDiffItem or type(self) is OutputDiffItem:
            args.append("./../../build/42sh")
            args.append("-c")
            args.append(tmp)

        process = subprocess.Popen(args,\
                stdout=subprocess.PIPE,\
                stderr=subprocess.PIPE,\
                stdin=subprocess.PIPE)
        try:
            out, err = process.communicate(input=self.command, timeout=my_timeout)
        except TimeoutExpired:
            process.kill()
            raise TimeoutException(3,self.command,self.name)
        err = err.decode()
        r = process.returncode
        process.kill()
        if sanity == '1':
            if "All heap blocks were freed -- no leaks are possible" not in err:
                raise ValgrindException(err,self.command,self.name)
            if "ERROR SUMMARY: 0 errors from 0 contexts" not in err:
                raise ValgrindException(err,self.command,self.name)

        if type(self) is OutputDiffItem:
            if "stdout" in self.expected:
                if self.expected["stdout"] != out:
                        raise YamlException("stdout", self.expected['stdout'],\
                                out, self.command, self.name)
            elif out: #check if no out should be returned but my program returned one
                raise YamlException("stdout", b'(empty)',out, self.command,\
                        self.name)

            if not "stderr" in self.expected:
                if err: #check if no err should be returned but my program returned err
                    raise YamlException("stderr", b'(empty)', out,\
                            self.command, self.name)

        if type(self) is BashDiffItem:
            if "stdout" in self.expected:
                if self.expected["stdout"] != out:
                        raise YamlException("stdout", self.expected['stdout'],\
                                out, self.command, self.name)
            elif out: #check if no out should be returned but my program returned one
                raise YamlException("stdout", "expected nothing",out, self.command,\
                        self.name)

            if not "stderr" in self.expected:
                if err: #check if no err should be returned but my program returned err
                    raise YamlException("stderr", "expected nothing", out,\
                            self.command, self.name)
            if self.expected['rvalue'] != r:
                raise YamlException('return value', self.expected['rvalue'], r \
                        , self.command, self.name)

    def repr_failure(self, excinfo):
        "called when runtest raises an exception"
        if isinstance(excinfo.value, YamlException):
            instance = excinfo.value
            return "\n".join(
                    [
                        "test : %s failed, called with [ %s ]\n"
                        "%s : expected : [ %s ]\n%s : got : [ %s ]"\
                                %(instance.name,\
                                instance.command,\
                                instance.expected_type,\
                                instance.expected_value,\
                                instance.expected_type,\
                                instance.output_value)
                    ]
            )
        if isinstance(excinfo.value, ValgrindException):
            instance = excinfo.value
            return "\n".join(
                    [
                        "test : %s failed sanity check, called with [ %s ]\n"
                        "valgrind returned : %s"\
                                %(instance.name,\
                                instance.command,\
                                instance.err)
                    ]
            )
        if isinstance(excinfo.value, TimeoutException):
            instance = excinfo.value
            return "\n".join(
                    [
                        "test : %s failed due to timeout, called with [ %s ]\n"
                        "timed out after %s : %s"\
                                %(instance.name,\
                                instance.command,\
                                instance.time)
                    ]
            )

    def reportinfo(self):
        return self.fspath,0,"usecase : %s" % self.name

class BashDiffItem(YamlItem):
    def __init__(self, name, parent, spec):
        super().__init__(name,parent,spec)
        my_timeout = int(self.config.getoption("--timeout"))
        self.name = name
        tmp = self.command.decode()
        args = ["bash", "--posix", "-c"]
        args.append(tmp)
        bash = subprocess.Popen(args,\
                stdout=subprocess.PIPE,\
                stderr=subprocess.PIPE,\
                stdin=subprocess.PIPE)
        out, err = bash.communicate(timeout= my_timeout)
        if "stdout" in self.expected:
            self.expected["stdout"] = out
        if "stderr" in self.expected:
            self.expected["stderr"] = err
        if "rvalue" in self.expected:
            self.expected["rvalue"] = bash.returncode
        bash.kill()

class OutputDiffItem(YamlItem):
    def __init__(self, name, parent, spec):
        super().__init__(name,parent,spec)
        self.name = name
        for item in self.expected:
            if item == "rvalue":
                self.expected["rvalue"] = int(self.expected["rvalue"])
                continue
            self.expected[item] = str.encode(self.expected[item])

class TimeoutException(Exception):
    """ custom exception for error reporting. """
    def __init__(self, time, command,name):
        self.command = command
        self.name = name
        self.time = time

class ValgrindException(Exception):
    """ custom exception for error reporting. """
    def __init__(self,err, command,name):
        self.command = command
        self.name = name
        self.err = err

class YamlException(Exception):
    """ custom exception for error reporting. """
    def __init__(self,expected_type,expected_value,output_value, command,name):
        self.command = command
        self.name = name
        self.expected_type = expected_type
        self.expected_value = expected_value
        self.output_value = output_value
