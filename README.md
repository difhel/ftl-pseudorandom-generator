Workshop on Pseudorandom Number Generator in FTL

## What does it do
This program receives as input the numbers A, B, C, D, seed_1, seed_2, seed_3, which set the generator of pseudorandom numbers by the recursive formula
$$x[seed] = (A * x[seed - 3] + B * x[seed - 2] + C * x[seed - 1]) \\\% D$$ and finds the following parameters of this generator:
- period
- pre-period
- its estimated characteristic Z, which is calculated as follows:
    - using the generator, we obtain 400 pseudorandom numbers
    - normalize these numbers (bring them to the half-interval `[0, 1)`)
    - divide the numbers into 20 equal segments `[0, 1/20)`, `[1/20, 2/20)`, ..., `[19/20, 20/20)` and count how many numbers are in each (let's call this distribution `arr`, where `arr[i]` is the number of numbers in the segment `[i/20, (i + 1) / 20)`.
    - calculate Z by the formula $$Z=\sum_{i=1}^{20} \frac{(arr_i-20)^2}{20^2}$$

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
