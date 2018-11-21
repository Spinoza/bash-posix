import pytest
import subprocess
import yaml

def pytest_collect_file(parent, path):
    print(path)
    if path.ext == ".yml" and path.basename.startswith("test"):
        return YamlFile(path, parent)

class YamlFile(pytest.File):
    def collect(self):
        raw = yaml.safe_load(self.fspath.open())
        for name, spec in sorted(raw.items()):
            if spec['type'] == "lexer diff":
                yield LexerDiffItem(name, self, spec)
            if spec['type'] == "grammar diff":
                yield GrammarDiffItem(name,self,spec)
            if spec ['type'] == "bash diff":
                yield BashDiffItem(name, self, spec)
            if spec ['type'] == "ast print diff":
                yield FileDiffItem(name, self, spec)

class YamlItem(pytest.Item):
    def __init__(self, name, parent, spec):
        super(YamlItem, self).__init__(name, parent)
        self.spec = spec
        self.command = str.encode(self.spec["command"])
        if "expected" in self.spec:
            self.expected = self.spec["expected"]

    def runtest(self):
        tmp = self.command.decode().split()
        args = []
        if type(self) is LexerDiffItem:
            args.append("./lexer_main")

        if type(self) is GrammarDiffItem:
            args.append("./grammar_main")

        if type(self) is FileDiffItem:
            print("here")
            args.append("./ast_main")

        if type(self) is BashDiffItem:
            args.append("./42sh")

        for string in tmp:
            args.append(string)

        process = subprocess.Popen(args,\
                stdout=subprocess.PIPE,\
                stderr=subprocess.PIPE,\
                stdin=subprocess.PIPE)

        out, err = process.communicate(input=self.command)
        r = process.returncode
        process.kill()
        if type(self) is FileDiffItem:
            dot_file = open('ast.dot',mode='r')
            all_of_it = dot_file.read()
            self.expected["stdout"] = self.expected["stdout"].decode()
            dot_file.close()

            if "stdout" in self.expected:
                if self.expected["stdout"] != all_of_it:
                    raise YamlException("stdout", self.expected['stdout'], \
                            all_of_it, self.command, self.name)
            return

        if type(self) is GrammarDiffItem:
            if self.expected['rvalue'] != r:
                raise YamlException('return value', self.expected['rvalue'],\
                        r, self.command, self.name)
            return
        if "stdout" in self.expected:
            if self.expected["stdout"] != out:
                raise YamlException("stdout", self.expected['stdout'],out, self.command, self.name)
        elif out: #check if no out should be returned but my program returned one
                raise YamlException("stdout", b'(empty)',out, self.command, self.name)

        if "stderr" in self.expected:
            if self.expected["stderr"] != err:
                raise YamlException("stderr", self.expected['stderr'],out,self.command, self.name)
        if not "stderr" in self.expected:
            if err: #check if no err should be returned but my program returned err
                raise YamlException("stderr", b'(empty)', out, self.command, self.name)

        if self.expected['rvalue'] != r:
                raise YamlException('stderr', b'(empty)', out, self.command, self.name)

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

class BashDiffItem(YamlItem):
    def __init__(self, name, parent, spec):
        super().__init__(name,parent,spec)
        self.name = name
        tmp = self.command.decode().split()
        args = ["bash"]
        for string in tmp:
            args.append(string)
        bash = subprocess.Popen(args,\
                stdout=subprocess.PIPE,\
                stderr=subprocess.PIPE,\
                stdin=subprocess.PIPE)
        out, err = bash.communicate(input=self.command)
        if "stdout" in self.expected:
            self.expected["stdout"] = out
        if "stderr" in self.expected:
            self.expected["stdout"] = err
        if "rvalue" in self.expected:
            self.expected["rvalue"] = bash.returncode
        bash.kill()

class FileDiffItem(YamlItem):
    def __init__(self, name, parent, spec):
        super().__init__(name,parent,spec)
        self.name = name
        for item in self.expected:
            if item == "rvalue":
                self.expected["rvalue"] = int(self.expected["rvalue"])
                continue
            self.expected[item] = str.encode(self.expected[item])

class LexerDiffItem(YamlItem):
    def __init__(self, name, parent, spec):
        super().__init__(name,parent,spec)
        self.name = name
        for item in self.expected:
            if item == "rvalue":
                self.expected["rvalue"] = int(self.expected["rvalue"])
                continue
            self.expected[item] = str.encode(self.expected[item])

class GrammarDiffItem(YamlItem):
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
