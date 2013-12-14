cpp__rand_test
==============

Random number generator tests

Compile with (needs `icc`, since we utilize Cilk Plus [1]):

    icc -O2 -o main main.cpp

Usage:

    ./main <random number count>

Sample output from running `./main 100000`:

    rand runtime:         0.001405000s
    uniform rand runtime: 0.000217000s
    sse rand runtime:     0.000037000s
    sse speedup over rand: 37.97x
    sse speedup over uniform: 5.86x


Explanation of output:

 * `rand runtime`: Generate `N` random numbers using `rand()` function.
 * `uniform rand runtime`: Generate `N` random numbers using
                           `UniformRandomGenerator`, based on VPR's [2]
                           `my_irand`.
 * `sse rand runtime`: Generate `N` random numbers using `sse_rand`[3].  If
                       `N > 10000`, use `cilk_for` to parallelize.

[1]: http://software.intel.com/en-us/intel-cilk-plus
[2]: http://dx.doi.org/10.1007%2f3-540-63465-7_226
[3]: http://software.intel.com/en-us/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor/
