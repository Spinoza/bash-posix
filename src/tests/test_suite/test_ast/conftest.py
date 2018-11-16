import pytest
import subprocess
import yaml
def pytest_collect_file(parent, path):
    if path.ext == ".yml" and path.basename.startswith("test"):
        return YamlFile(path, parent)

class YamlFile(pytest.File):
    def collect(self):
        raw = yaml.safe_load(self.fspath.open())
        for name, spec in sorted(raw.items()):
            yield OutputDiffItem(name, self, spec)

class YamlItem(pytest.Item):
    def __init__(self, name, parent, spec):
        super(YamlItem, self).__init__(name, parent)
        print("before return")
        self.spec = spec
        self.command = str.encode(self.spec["command"])
        if "expected" in self.spec:
            self.expected = self.spec["expected"]

    def runtest(self):
        tmp = self.command.decode().split()
        args = []
        args.append("./ast_main")
        for string in tmp:
            args.append(string)
        process = subprocess.Popen(args,\
                stdout=subprocess.PIPE,\
                stderr=subprocess.PIPE,\
                stdin=subprocess.PIPE)

        process.communicate(input=self.command)
        dot_file = open('ast.dot',mode='r')
        all_of_it = dot_file.read()
        self.expected["stdout"] = self.expected["stdout"].decode()
        dot_file.close()

        r = process.returncode
        process.kill()
        if "stdout" in self.expected:
            if self.expected["stdout"] != all_of_it:
                raise YamlException("stdout", self.expected['stdout'],all_of_it, self.command, self.name)


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
class OutputDiffItem(YamlItem):
    def __init__(self, name, parent, spec):
        super().__init__(name,parent,spec)
        self.name = name
        for item in self.expected:
            if item == "rvalue":
                self.expected["rvalue"] = int(self.expected["rvalue"])
                continue
            self.expected[item] = str.encode(self.expected[item])

class YamlException(Exception):
    """ custom exception for error reporting. """
    def __init__(self,expected_type,expected_value,output_value, command,name):
        self.command = command
        self.name = name
        self.expected_type = expected_type
        self.expected_value = expected_value
        self.output_value = output_value
