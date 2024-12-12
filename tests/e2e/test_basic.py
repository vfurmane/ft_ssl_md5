import subprocess
from utils import make_tested_command


class TestBasic:
    def test_no_arg(self, cli_path):
        result = subprocess.run(make_tested_command([cli_path], with_leak_check=True), capture_output=True, text=True)
        assert result.returncode == 1

    def test_wrong_command(self, cli_path):
        result = subprocess.run(make_tested_command([cli_path, "ft"], with_leak_check=True), capture_output=True,
                                text=True)
        assert result.returncode == 1
