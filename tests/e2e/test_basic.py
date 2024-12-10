import subprocess


class TestBasic:
    def test_no_arg(self, cli_path):
        result = subprocess.run([cli_path], capture_output=True, text=True)
        assert result.returncode == 1
