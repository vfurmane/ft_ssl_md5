import hashlib
import pytest
import subprocess
from utils import make_tested_command

test_inputs = ["", "1", "hello, world!", "As the wind caresses the grass, I fell asleep.",
               "The sun dipped low, painting the sky in shades of pink.",
               "The dog ran across the field, chasing after a red ball.",
               "The moon rose high above the dark, quiet sleeping town.",
               "The bird flew high, soaring through the bright blue sky.",
               "The cat sat quietly by the window, watching the birds fly.",
               "The pie was sour, lightly crisp on the side, and this is how she liked her pies since then.",
               "He stood still for a moment, hoping that the night would last forever. That's how those condemned to death must hope.",
               "A quiet breeze swept through the trees as stars sparkled above, casting a serene glow over the calm and peaceful night.",
               "The golden sun dipped below the horizon, painting the evening sky with hues of pink, orange, and purple in soft strokes."]
with_leak_check = True


class TestSha256:
    @pytest.mark.parametrize("test_input", ["hello, world!"])
    def test_string_flag(self, cli_path, test_input):
        result = subprocess.run(make_tested_command([cli_path, "sha256", "-s", test_input], with_leak_check),
                                capture_output=True, text=True)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert hashlib.sha256(test_input.encode("utf-8")).hexdigest() in result.stdout, "hash not found in output"
