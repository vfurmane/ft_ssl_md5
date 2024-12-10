def make_tested_command(command, with_leak_check):
    full_command = []

    if with_leak_check:
        full_command = ["valgrind", "--leak-check=full", "--error-exitcode=42"] + full_command

    full_command += command

    return full_command
