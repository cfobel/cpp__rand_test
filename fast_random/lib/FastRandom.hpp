#include <stdexcept>
#include "AlignedNew.hpp"
#include "rand_sse.h"


class FastRandom {
    /* Overview
     * ========
     *
     * Each `FastRandom` instance provides a random number generator context,
     * where 32-bit, random unsigned integers can be generated in sets of 4
     * using [SSE instructions][1].  This leads to dramatically reduced
     * run-time compared to using the `rand` function from the `C` standard
     * library.
     *
     * This class uses code from [Intel for SSE random number generation][1].
     *
     * [1]: http://software.intel.com/en-us/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor/
     */
public:
    __declspec(align(16)) __m128i cur_seed_;

    static FastRandom *create_aligned(uint32_t seed=1) {
        //char *buffer = (char *)_mm_malloc(sizeof(FastRandom), 16);
        //if (buffer == NULL) {
            //throw std::runtime_error("Could not allocate memory for object.");
        //}
        //FastRandom *fast_random = new (buffer) FastRandom(seed);
        return new (16) FastRandom(seed);
    }

    FastRandom() {
        this->seed(1);
    }
    FastRandom(uint32_t seed) {
        this->seed(seed);
#if 0
        uint32_t temp[4];

        const size_t PRIME_COUNT = 10;

        for (int i = 0; i < PRIME_COUNT; i++) {
            this->rand4(temp);
        }
#endif
    }

    void seed(uint32_t seed) {
        srand_sse(this->cur_seed_, seed);
    }

    void frand4(float out[4]) {
        /* Fill `out` with four random floating-point numbers in the inclusive
         * range, `[0..1]`. */
        frand_sse(this->cur_seed_, out);
    }

    void rand4(uint32_t out[4]) {
        /* Fill `out` with four random unsigned integers in the inclusive
         * range, `[0..2^32 - 1]` _(i.e., `[0..4294967295]`)_. */
        rand_sse(this->cur_seed_, out);
    }

    void frand_array(int count, float out[count]) {
        /* Fill `out` with random floating-point numbers in the inclusive
         * range, `[0..1]`.
         *
         * __NB__ The length of the `out` array must be divisible by 4! */
        frand_sse_array(this->cur_seed_, count, out);
    }

    void rand_array(int count, uint32_t out[count]) {
        /* Fill `out` with random unsigned integers in the inclusive range,
         * `[0..2^32 - 1]` _(i.e., `[0..4294967295]`)_.
         *
         * __NB__ The length of the `out` array must be divisible by 4! */
        rand_sse_array(this->cur_seed_, count, out);
    }

    void cilk_rand_array(int count, uint32_t out[count]) {
        /* __NB__ __TODO__ This method likely produces undefined behaviour,
         * since the `cur_seed_` variable is modified by more than one thread
         * at once!
         *
         * Fill `out` with random unsigned integers in the inclusive range,
         * `[0..2^32 - 1]` _(i.e., `[0..4294967295]`)_.  This method uses
         * `cilk_for` to parallelize calls.
         *
         * __NB__ The length of the `out` array must be divisible by 4! */
        rand_sse_array_cilk(this->cur_seed_, count, out);
    }

    DYNAMIC_ALIGNMENT(FastRandom);
};
