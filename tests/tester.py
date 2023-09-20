"""Tester"""
import os
import subprocess
import unittest


class TestingGroup(unittest.TestCase):
    """Base class for unittest"""
    def test(self):
        """Test method for VS Code"""
        self.assertEqual(40 + 2, 42)


def read_file(path: str) -> str:
    """Reads a file and returns the file contents"""
    res = ""
    with open(path, encoding="utf-8") as file:
        res = file.read().strip()
    return res


def builder():
    """Build C++ file."""
    print("Building...")
    subprocess.check_call(
        ["g++", "main.cpp", "-o", "/tmp/testing.o", "-O2", "-I."],
        stdout=subprocess.DEVNULL
    )
    print("Done.")

builder()

def generate_test(ans: str, out: str, test_name: str):
    """Генерирует тест для сравнения верного ответа и вывода программы"""
    def test():
        assert ans == out, f"WA {test_name}\nAns: {ans}\nOut: {out}"
        print(f"{test_name} OK")
    return test


for x in os.walk("./tests"):
    if x[0] == "./tests": # parent folder is not a test group
        continue
    testing_group = x[0].split('/')[-1]
    if testing_group.startswith("ignore_"):
        continue
    if testing_group == "__pycache__": # python cache
        continue

    suite = unittest.TestSuite()
    print(f"Testing subgroup '{testing_group}'...")
    for test_name in x[2]:
        if test_name.endswith(".a"):
            # this file is an answer, not a test
            continue
        os.system(f"cat ./tests/{testing_group}/{test_name} | /tmp/testing.o > /tmp/testing.out")
        ans = read_file(f"./tests/{testing_group}/{test_name}.a").strip()
        user_ans = read_file("/tmp/testing.out").strip()
        suite.addTest(unittest.FunctionTestCase(
            generate_test(ans, user_ans, test_name)
        ))
    unittest.TextTestRunner().run(suite)

# unittest.main()
