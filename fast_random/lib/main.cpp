#include <sys/time.h> // for gettimeofday()
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "FastRandom.hpp"
#include "UniformRandomGenerator.hpp"

using namespace std;


int main(int argc, char* argv[argc]) {
    if(argc != 2) {
        return -1;
    }
    const int N = atoi(argv[1]);
    unsigned int *results = (unsigned int*)calloc(N, sizeof(unsigned int));

    FastRandom *rng = FastRandom::create_aligned(10);
    cout << "  " << (((size_t)&rng->cur_seed_) % 16) << endl;

    float frand_values[4];

    rng->frand4(frand_values);

    cout << "frand_values = [";
    cout << frand_values[:] << ", ";
    cout << "]" << endl;

    rng->rand4(results);
    rng->rand_array(N, results);
    rng->cilk_rand_array(N, results);
    _mm_free(rng);
#if 0
    timeval begin;
    timeval end;

    const int LOOP_COUNT = 10;
    double runtimes[3][LOOP_COUNT];

    int i, k;
    for(i = 0; i < 3; i++) {
        for(k = 0; k < LOOP_COUNT; k++) {
            gettimeofday(&begin, NULL);
            rand_array(N, results);
            gettimeofday(&end, NULL);
            runtimes[i][k] = (end.tv_sec - begin.tv_sec) * 1000.0;      // sec to ms
            runtimes[i][k] += (end.tv_usec - begin.tv_usec) / 1000.0;   // us to ms
        }
        __sec_reduce_add(runtimes[i][:]) / float(LOOP_COUNT);
    }
    double rand_time_spent = __sec_reduce_min(runtimes[:][0]) / 1000.;
    printf("rand runtime:         %.9fs (%.3fns/item)\n", rand_time_spent,
            rand_time_spent / float(N) / 1e-9);

    UniformRandomGenerator my_rng;
    for(i = 0; i < 3; i++) { for(k = 0; k < LOOP_COUNT; k++) {
            gettimeofday(&begin, NULL);
            for(int n = 0; n < N; n++) {
                results[n] = my_rng.irand(1 << 31);
            }
            gettimeofday(&end, NULL);
            runtimes[i][k] = (end.tv_sec - begin.tv_sec) * 1000.0;      // sec to ms
            runtimes[i][k] += (end.tv_usec - begin.tv_usec) / 1000.0;   // us to ms
        }
        __sec_reduce_add(runtimes[i][:]) / float(LOOP_COUNT);
    }
    double uniform_rand_time_spent = __sec_reduce_min(runtimes[:][0]) / 1000.;
    double result = __sec_reduce_add(float(results[0:N]));
    printf("uniform rand runtime: %.9fs (%.3fns/item)\n", uniform_rand_time_spent,
            uniform_rand_time_spent / float(N) / 1e-9);

    for(i = 0; i < 3; i++) { for(k = 0; k < LOOP_COUNT; k++) {
            gettimeofday(&begin, NULL);
            if(N > 10000) rand_sse_array_cilk(N, results);
            else rand_sse_array(N, results);
            gettimeofday(&end, NULL);
            runtimes[i][k] = (end.tv_sec - begin.tv_sec) * 1000.0;      // sec to ms
            runtimes[i][k] += (end.tv_usec - begin.tv_usec) / 1000.0;   // us to ms
        }
        __sec_reduce_add(runtimes[i][:]) / float(LOOP_COUNT);
    }
    double sse_rand_time_spent = __sec_reduce_min(runtimes[:][0]) / 1000.;
    printf("sse rand runtime:     %.9fs (%.3fns/item)\n", sse_rand_time_spent,
            sse_rand_time_spent / float(N) / 1e-9);
    printf("sse speedup over rand: %.2fx\n", (rand_time_spent /
                                              sse_rand_time_spent));
    printf("sse speedup over uniform: %.2fx\n", (uniform_rand_time_spent /
                                                 sse_rand_time_spent));

#endif
    return 0;
}
