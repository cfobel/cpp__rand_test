#include <sys/time.h> // for gettimeofday()
#include <stdio.h>
#include <cstdlib>
#include "rand_sse.h"
#include "UniformRandomGenerator.hpp"


int main(int argc, char* argv[argc]) {
    if(argc != 2) {
        return -1;
    }
    const int N = atoi(argv[1]);
    unsigned int *results = (unsigned int*)calloc(N, sizeof(unsigned int));
    timeval begin;
    timeval end;

    double runtimes[3][10];

    int i, k;
    for(i = 0; i < 3; i++) {
        for(k = 0; k < 10; k++) {
            gettimeofday(&begin, NULL);
            rand_array(N, results);
            gettimeofday(&end, NULL);
            runtimes[i][k] = (end.tv_sec - begin.tv_sec) * 1000.0;      // sec to ms
            runtimes[i][k] += (end.tv_usec - begin.tv_usec) / 1000.0;   // us to ms
        }
        __sec_reduce_add(runtimes[i][:]) / 10.;
    }
    double rand_time_spent = __sec_reduce_min(runtimes[:][0]) / 1000.;
    printf("rand runtime:         %.9fs\n", rand_time_spent);

    UniformRandomGenerator my_rng;
    for(i = 0; i < 3; i++) { for(k = 0; k < 10; k++) {
            gettimeofday(&begin, NULL);
            for(int n = 0; n < N; n++) {
                results[n] = my_rng.irand(1 << 31);
            }
            gettimeofday(&end, NULL);
            runtimes[i][k] = (end.tv_sec - begin.tv_sec) * 1000.0;      // sec to ms
            runtimes[i][k] += (end.tv_usec - begin.tv_usec) / 1000.0;   // us to ms
        }
        __sec_reduce_add(runtimes[i][:]) / 10.;
    }
    double uniform_rand_time_spent = __sec_reduce_min(runtimes[:][0]) / 1000.;
    double result = __sec_reduce_add(float(results[0:N]));
    printf("uniform rand runtime: %.9fs\n", uniform_rand_time_spent);

    for(i = 0; i < 3; i++) { for(k = 0; k < 10; k++) {
            gettimeofday(&begin, NULL);
            if(N > 10000) rand_sse_array_cilk(N, results);
            else rand_sse_array(N, results);
            gettimeofday(&end, NULL);
            runtimes[i][k] = (end.tv_sec - begin.tv_sec) * 1000.0;      // sec to ms
            runtimes[i][k] += (end.tv_usec - begin.tv_usec) / 1000.0;   // us to ms
        }
        __sec_reduce_add(runtimes[i][:]) / 10.;
    }
    double sse_rand_time_spent = __sec_reduce_min(runtimes[:][0]) / 1000.;
    printf("sse rand runtime:     %.9fs\n", sse_rand_time_spent);
    printf("sse speedup over rand: %.2fx\n", (rand_time_spent /
                                              sse_rand_time_spent));
    printf("sse speedup over uniform: %.2fx\n", (uniform_rand_time_spent /
                                                 sse_rand_time_spent));

    return 0;
}
