"""Tester"""
import os
import subprocess
import unittest


class TestingGroup(unittest.TestCase):
    """pylint go brrr"""
    def test():
        """Тестовый метод для VS Code"""
        self.assertEqual(42, 42)


def read_file(path: str) -> str:
    """pylint go brrr"""
    res = ""
    with open(path, encoding="utf-8") as file:
        res = file.read().strip()
    return res


def builder():
    """Build C++ file."""
    print("Building...")
    subprocess.check_call(["g++", "main.cpp", "-o", "/tmp/testing.o", "-O2", "-I."], stdout=subprocess.DEVNULL)
    print("Done.")

builder()

def generate_test(ans: str, out: str, test_name: str):
    """Генерирует тест для сравнения верного ответа и вывода программы"""
    def test():
        assert ans == out, f"WA {test_name}\nAns: {ans}\nOut: {out}"
        print(f"{test_name} OK")
    return test


for x in os.walk("./tests"):
    if x[0] == "./tests":
        continue
    if x[0].split('/')[-1].startswith("ignore_"):
        continue
    if x[0].split('/')[-1] == "__pycache__":
        continue

    suite = unittest.TestSuite()
    print(f"Testing subgroup '{x[0].split('/')[-1]}'...")
    for f in x[2]:
        if f.endswith(".a"): # ответ на тест
            continue
        os.system(f"cat {x[0]}/{f} | /tmp/testing.o > /tmp/testing.out")
        ans = read_file(x[0] + "/" + f + ".a").strip()
        user_ans = read_file("/tmp/testing.out").strip()
        suite.addTest(unittest.FunctionTestCase(
            generate_test(ans, user_ans, f)
        ))
    unittest.TextTestRunner().run(suite)

# unittest.main()