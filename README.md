## Launch
```bash
$ g++ main.cpp -o main.o -I.
```
> Note: we need "-I." to include local file `debug.h`.

## Testing
```bash
$ python3 tests/tester.py
Building...
Done.
Testing subgroup 'main_tests'...
1 OK
.2 OK
.3 OK
.4 OK
.5 OK
.6 OK
.
----------------------------------------------------------------------
Ran 6 tests in 0.001s

OK
```