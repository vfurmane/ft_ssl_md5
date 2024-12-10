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


class TestMd5:

    @pytest.mark.parametrize("test_input", test_inputs)
    def test_arguments_less_md5(self, cli_path, test_input):
        result = subprocess.run(make_tested_command([cli_path, "md5"], with_leak_check=True),
                                text=True, capture_output=True, input=test_input)
        print(result.stdout)
        assert hashlib.md5(test_input.encode("utf-8")).hexdigest() in result.stdout, "hash not found in output"

    @pytest.mark.parametrize("test_input", test_inputs)
    def test_algo(self, cli_path, test_input):
        result = subprocess.run(make_tested_command([cli_path, "md5", "-s", test_input], with_leak_check=True),
                                capture_output=True, text=True)
        assert hashlib.md5(test_input.encode("utf-8")).hexdigest() in result.stdout, "hash not found in output"
