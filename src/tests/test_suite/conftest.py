import pytest
import subprocess


def pytest_collect_file(parent, path):
    if path.ext == ".yml" and path.basename.startswith("test"):
        return YamlFile(path, parent)

class YamlFile(pytest.File):
    def collect(self):
        import yaml  # we need a yaml parser, e.g. PyYAML

        raw = yaml.safe_load(self.fspath.open())
        for name, spec in sorted(raw.items()):
            yield YamlItem(name, self, spec)

class YamlItem(pytest.Item):
    def __init__(self, name, parent, spec):
        super(YamlItem, self).__init__(name, parent)
        self.spec = spec

    def runtest(self):
        process = subprocess.Popen(["bash"], stdout=subprocess.PIPE,\
                stderr=subprocess.PIPE,\
                stdin=subprocess.PIPE)

        out, err = process.communicate(input=self.command)
        process.kill()
        if "stdout" in self.expected:
            if self.expected["stdout"] != out:
                raise YamlException("stdout", self.expected["stdout"], out)
        elif out: #check if no out should be returned but my program returned one
            raise YamlException("stdout", b"(empty)", out)

        if "stderr" in self.expected:
            if self.expected["stderr"] != err:
                raise YamlException("stderr", self.expected["stderr"], err)
        elif err: #check if no out should be returned but my program returned one
            raise YamlException("stderr", b"(empty)", err)
        if self.expected['rvalue'] != process.returncode:
            raise YamlException("rvalue", self.expected["rvalue"], rvalue)

    def repr_failure(self, excinfo):
        """ called when self.runtest() raises an exception. """
        if isinstance(excinfo.value, YamlException):
            return "\n".join(
                [
                    "usecase execution failed",
                    "   spec failed: %r: %r" % excinfo.value.args[1:3],
                    "   no further details known at this point.",
                ]
            )

class OutputDiffItem(YamlItem):
    def __init__(self, name, parent, spec):
        super().__init__(name,parent,spec)
        for item in self.exptected:
            if item == "rvalue":
                self.expected["rvalue"] = int(self.expected["rvalue"])
                continue
            self.expected[item] = str.encode(self.expected[item])