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


class TestMd5:
    def test_subject_ft_is_nice(self, cli_path):
        test_input = "42 is nice\n"
        result = subprocess.run(make_tested_command([cli_path, "md5"], with_leak_check),
                                text=True, capture_output=True, input=test_input)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '(stdin)= 35f1d6de0302e2086a4e472266efb3a9\n', "output is different from the subject"

    def test_subject_pipe_ft_is_nice(self, cli_path):
        test_input = "42 is nice\n"
        result = subprocess.run(make_tested_command([cli_path, "md5", "-p"], with_leak_check),
                                text=True, capture_output=True, input=test_input)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '("42 is nice")= 35f1d6de0302e2086a4e472266efb3a9\n', "output is different from the subject"

    def test_subject_pity_the_living(self, cli_path):
        test_input = "Pity the living.\n"
        result = subprocess.run(make_tested_command([cli_path, "md5", "-q", "-r"], with_leak_check),
                                text=True, capture_output=True, input=test_input)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == 'e20c3b973f63482a778f3fd1869b7f25\n', "output is different from the subject"

    def test_subject_string_pity_those(self, cli_path):
        test_input = "pity those that aren't following baerista on spotify."
        result = subprocess.run(make_tested_command([cli_path, "md5", "-s", test_input], with_leak_check),
                                capture_output=True, text=True)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == 'MD5 ("pity those that aren\'t following baerista on spotify.") = a3c990a1964705d9bf0e602f44572f5f\n', "output is different from the subject"

    def test_subject_edge_cases(self, cli_path):
        test_input_pipe = "be sure to handle edge cases carefully\n"
        test_input_file = "hello.txt"
        result = subprocess.run(make_tested_command([cli_path, "md5", "-p", test_input_file], with_leak_check),
                                capture_output=True, text=True, input=test_input_pipe)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '("be sure to handle edge cases carefully")= 3553dc7dc5963b583c056d1b9fa3349c\nMD5 (hello.txt) = 3adbbad1791fbae3ec908894c4963870\n', "output is different from the subject"

    def test_subject_not_make_sense(self, cli_path):
        test_input_pipe = "some of this will not make sense at first\n"
        test_input_file = "hello.txt"
        result = subprocess.run(make_tested_command([cli_path, "md5", test_input_file], with_leak_check),
                                capture_output=True, text=True, input=test_input_pipe)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == 'MD5 (hello.txt) = 3adbbad1791fbae3ec908894c4963870\n', "output is different from the subject"

    def test_subject_eventually_understand(self, cli_path):
        test_input_pipe = "but eventually you will understand\n"
        test_input_file = "hello.txt"
        result = subprocess.run(make_tested_command([cli_path, "md5", "-p", "-r", test_input_file], with_leak_check),
                                capture_output=True, text=True, input=test_input_pipe)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '("but eventually you will understand")= dcdd84e0f635694d2a943fa8d3905281\n3adbbad1791fbae3ec908894c4963870 hello.txt\n', "output is different from the subject"

    def test_subject_glhf(self, cli_path):
        test_input_pipe = "GL HF let's go\n"
        test_input_string = "foo"
        test_input_file = "hello.txt"
        result = subprocess.run(
            make_tested_command([cli_path, "md5", "-p", "-s", test_input_string, test_input_file], with_leak_check),
            capture_output=True, text=True, input=test_input_pipe)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '("GL HF let\'s go")= d1e3cc342b6da09480b27ec57ff243e2\nMD5 ("foo") = acbd18db4cc2f85cedef654fccc4a4d8\nMD5 (hello.txt) = 3adbbad1791fbae3ec908894c4963870\n', "output is different from the subject"

    def test_subject_one_more_thing(self, cli_path):
        test_input_pipe = "one more thing\n"
        test_input_string = "foo"
        test_input_file = "hello.txt"
        result = subprocess.run(
            make_tested_command([cli_path, "md5", "-r", "-p", "-s", test_input_string, test_input_file, "-s", "bar"],
                                with_leak_check),
            capture_output=True, text=True, input=test_input_pipe)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '("one more thing")= a0bd1876c6f011dd50fae52827f445f5\nacbd18db4cc2f85cedef654fccc4a4d8 "foo"\n3adbbad1791fbae3ec908894c4963870 hello.txt\n', "output is different from the subject"
        assert './ft_ssl: md5: -s: No such file or directory\n./ft_ssl: md5: bar: No such file or directory' in result.stderr, "output is different from the subject"

    def test_subject_extra_clear(self, cli_path):
        test_input_pipe = "just to be extra clear\n"
        test_input_string = "foo"
        test_input_file = "hello.txt"
        result = subprocess.run(
            make_tested_command([cli_path, "md5", "-r", "-q", "-p", "-s", test_input_string, test_input_file],
                                with_leak_check),
            capture_output=True, text=True, input=test_input_pipe)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == 'just to be extra clear\n3ba35f1ea0d170cb3b9a752e3360286c\nacbd18db4cc2f85cedef654fccc4a4d8\n3adbbad1791fbae3ec908894c4963870\n', "output is different from the subject"

    def test_quiet_stdin(self, cli_path):
        test_input = "42 is nice\n"
        result = subprocess.run(make_tested_command([cli_path, "md5", "-q"], with_leak_check),
                                text=True, capture_output=True, input=test_input)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '35f1d6de0302e2086a4e472266efb3a9\n', "output does not meet expectations"

    def test_quiet_pipe(self, cli_path):
        test_input = "42 is nice\n"
        result = subprocess.run(make_tested_command([cli_path, "md5", "-q", "-p"], with_leak_check),
                                text=True, capture_output=True, input=test_input)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '42 is nice\n35f1d6de0302e2086a4e472266efb3a9\n', "output does not meet expectations"

    def test_quiet_string(self, cli_path):
        test_input = "hello, world!"
        result = subprocess.run(make_tested_command([cli_path, "md5", "-q", "-s", test_input], with_leak_check),
                                text=True, capture_output=True)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '3adbbad1791fbae3ec908894c4963870\n', "output does not meet expectations"

    def test_quiet_pipe_string(self, cli_path):
        test_input_pipe = "42 is nice\n"
        test_input_string = "hello, world!"
        result = subprocess.run(
            make_tested_command([cli_path, "md5", "-q", "-p", "-s", test_input_string], with_leak_check),
            text=True, capture_output=True, input=test_input_pipe)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '42 is nice\n35f1d6de0302e2086a4e472266efb3a9\n3adbbad1791fbae3ec908894c4963870\n', "output does not meet expectations"

    def test_quiet_string_pipe(self, cli_path):
        test_input_pipe = "42 is nice\n"
        test_input_string = "hello, world!"
        result = subprocess.run(
            make_tested_command([cli_path, "md5", "-q", "-s", test_input_string, "-p"], with_leak_check),
            text=True, capture_output=True, input=test_input_pipe)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '3adbbad1791fbae3ec908894c4963870\n42 is nice\n35f1d6de0302e2086a4e472266efb3a9\n', "output does not meet expectations"

    def test_string_quiet(self, cli_path):
        test_input = "hello, world!"
        result = subprocess.run(make_tested_command([cli_path, "md5", "-s", test_input, "-q"], with_leak_check),
                                text=True, capture_output=True)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '3adbbad1791fbae3ec908894c4963870\n', "output does not meet expectations"

    def test_reverse_string(self, cli_path):
        test_input = "hello, world!"
        result = subprocess.run(make_tested_command([cli_path, "md5", "-r", "-s", test_input], with_leak_check),
                                text=True, capture_output=True)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '3adbbad1791fbae3ec908894c4963870 "hello, world!"\n', "output does not meet expectations"

    def test_string_reverse(self, cli_path):
        test_input = "hello, world!"
        result = subprocess.run(make_tested_command([cli_path, "md5", "-s", test_input, "-r"], with_leak_check),
                                text=True, capture_output=True)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '3adbbad1791fbae3ec908894c4963870 "hello, world!"\n', "output does not meet expectations"

    def test_reverse_pipe(self, cli_path):
        test_input = "42 is nice\n"
        result = subprocess.run(make_tested_command([cli_path, "md5", "-r", "-p"], with_leak_check),
                                text=True, capture_output=True, input=test_input)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == '("42 is nice")= 35f1d6de0302e2086a4e472266efb3a9\n', "output does not meet expectations"

    def test_file(self, cli_path):
        test_input = "hello.txt"
        result = subprocess.run(make_tested_command([cli_path, "md5", test_input], with_leak_check),
                                text=True, capture_output=True)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == 'MD5 (hello.txt) = 3adbbad1791fbae3ec908894c4963870\n', "output does not meet expectations"

    def test_lorem_file(self, cli_path):
        test_input = "lorem.txt"
        result = subprocess.run(make_tested_command([cli_path, "md5", test_input], with_leak_check),
                                text=True, capture_output=True)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert result.stdout == 'MD5 (lorem.txt) = 9ab92ccc0f9919855cf74bea764f9d5d\n', "output does not meet expectations"

    @pytest.mark.parametrize("test_input", test_inputs)
    def test_stdin(self, cli_path, test_input):
        result = subprocess.run(make_tested_command([cli_path, "md5"], with_leak_check),
                                text=True, capture_output=True, input=test_input)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert hashlib.md5(test_input.encode("utf-8")).hexdigest() in result.stdout, "hash not found in output"

    @pytest.mark.parametrize("test_input", test_inputs)
    def test_string_flag(self, cli_path, test_input):
        result = subprocess.run(make_tested_command([cli_path, "md5", "-s", test_input], with_leak_check),
                                capture_output=True, text=True)
        assert result.returncode == 0, "return code should be 0 (ok)"
        assert hashlib.md5(test_input.encode("utf-8")).hexdigest() in result.stdout, "hash not found in output"
