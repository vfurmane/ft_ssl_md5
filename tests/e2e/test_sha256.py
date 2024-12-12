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
    def test_quiet_stdin(self, cli_path):
        test_input = "42 is nice\n"
        result = subprocess.run(make_tested_command([cli_path, "sha256", "-q"], with_leak_check),
                                text=True, capture_output=True, input=test_input)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == 'a5482539287a4069ebd3eb45a13a47b1968316c442a7e69bc6b9c100b101d65d\n', "output does not meet expectations"

    def test_quiet_pipe(self, cli_path):
        test_input = "42 is nice\n"
        result = subprocess.run(make_tested_command([cli_path, "sha256", "-q", "-p"], with_leak_check),
                                text=True, capture_output=True, input=test_input)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '42 is nice\na5482539287a4069ebd3eb45a13a47b1968316c442a7e69bc6b9c100b101d65d\n', "output does not meet expectations"

    def test_quiet_string(self, cli_path):
        test_input = "hello, world!"
        result = subprocess.run(make_tested_command([cli_path, "sha256", "-q", "-s", test_input], with_leak_check),
                                text=True, capture_output=True)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '68e656b251e67e8358bef8483ab0d51c6619f3e7a1a9f0e75838d41ff368f728\n', "output does not meet expectations"

    def test_quiet_pipe_string(self, cli_path):
        test_input_pipe = "42 is nice\n"
        test_input_string = "hello, world!"
        result = subprocess.run(
            make_tested_command([cli_path, "sha256", "-q", "-p", "-s", test_input_string], with_leak_check),
            text=True, capture_output=True, input=test_input_pipe)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '42 is nice\na5482539287a4069ebd3eb45a13a47b1968316c442a7e69bc6b9c100b101d65d\n68e656b251e67e8358bef8483ab0d51c6619f3e7a1a9f0e75838d41ff368f728\n', "output does not meet expectations"

    def test_quiet_string_pipe(self, cli_path):
        test_input_pipe = "42 is nice\n"
        test_input_string = "hello, world!"
        result = subprocess.run(
            make_tested_command([cli_path, "sha256", "-q", "-s", test_input_string, "-p"], with_leak_check),
            text=True, capture_output=True, input=test_input_pipe)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '68e656b251e67e8358bef8483ab0d51c6619f3e7a1a9f0e75838d41ff368f728\n42 is nice\na5482539287a4069ebd3eb45a13a47b1968316c442a7e69bc6b9c100b101d65d\n', "output does not meet expectations"

    def test_string_quiet(self, cli_path):
        test_input = "hello, world!"
        result = subprocess.run(make_tested_command([cli_path, "sha256", "-s", test_input, "-q"], with_leak_check),
                                text=True, capture_output=True)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '68e656b251e67e8358bef8483ab0d51c6619f3e7a1a9f0e75838d41ff368f728\n', "output does not meet expectations"

    def test_reverse_string(self, cli_path):
        test_input = "hello, world!"
        result = subprocess.run(make_tested_command([cli_path, "sha256", "-r", "-s", test_input], with_leak_check),
                                text=True, capture_output=True)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '68e656b251e67e8358bef8483ab0d51c6619f3e7a1a9f0e75838d41ff368f728 "hello, world!"\n', "output does not meet expectations"

    def test_string_reverse(self, cli_path):
        test_input = "hello, world!"
        result = subprocess.run(make_tested_command([cli_path, "sha256", "-s", test_input, "-r"], with_leak_check),
                                text=True, capture_output=True)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '68e656b251e67e8358bef8483ab0d51c6619f3e7a1a9f0e75838d41ff368f728 "hello, world!"\n', "output does not meet expectations"

    def test_reverse_pipe(self, cli_path):
        test_input = "42 is nice\n"
        result = subprocess.run(make_tested_command([cli_path, "sha256", "-r", "-p"], with_leak_check),
                                text=True, capture_output=True, input=test_input)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '("42 is nice")= a5482539287a4069ebd3eb45a13a47b1968316c442a7e69bc6b9c100b101d65d\n', "output does not meet expectations"

    def test_file(self, cli_path):
        test_input = "hello.txt"
        result = subprocess.run(make_tested_command([cli_path, "sha256", test_input], with_leak_check),
                                text=True, capture_output=True)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == 'SHA256 (hello.txt) = 68e656b251e67e8358bef8483ab0d51c6619f3e7a1a9f0e75838d41ff368f728\n', "output does not meet expectations"

    def test_lorem_file(self, cli_path):
        test_input = "lorem.txt"
        result = subprocess.run(make_tested_command([cli_path, "sha256", test_input], with_leak_check),
                                text=True, capture_output=True)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == 'SHA256 (lorem.txt) = 6e816c6c2e401ee9cbe9a7c15fc73357c7aa7d383727c52a25344dd24d3822db\n', "output does not meet expectations"

    @pytest.mark.parametrize("test_input", test_inputs)
    def test_stdin(self, cli_path, test_input):
        result = subprocess.run(make_tested_command([cli_path, "sha256"], with_leak_check),
                                text=True, capture_output=True, input=test_input)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert hashlib.sha256(test_input.encode("utf-8")).hexdigest() in result.stdout, "hash not found in output"

    @pytest.mark.parametrize("test_input", test_inputs)
    def test_string_flag(self, cli_path, test_input):
        result = subprocess.run(make_tested_command([cli_path, "sha256", "-s", test_input], with_leak_check),
                                capture_output=True, text=True)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert hashlib.sha256(test_input.encode("utf-8")).hexdigest() in result.stdout, "hash not found in output"
