import hashlib
import pytest
import subprocess
from utils import make_tested_command


class TestMd5:
    @pytest.mark.parametrize("test_input", ["hello, world!"])
    def test_algo(self, cli_path, test_input):
        result = subprocess.run(make_tested_command([cli_path, "-s", test_input], with_leak_check=True),
                                capture_output=True, text=True)
        assert hashlib.md5(test_input.encode("utf-8")).hexdigest() in result.stdout, "hash not found in output"
