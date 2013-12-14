from libc.stdint cimport uint32_t
from libc.math cimport ceil

cdef extern from "lib/UniformRandomGenerator.hpp":
    cdef cppclass UniformRandomGenerator:
        UniformRandomGenerator() except +
        unsigned int irand(unsigned int max_value) nogil # Max value (inclusive)
        float frand() nogil


cdef extern from "lib/FastRandom.hpp":
    cdef cppclass FastRandom:
        uint32_t *cur_seed_

        FastRandom() except +
        FastRandom(uint32_t seed) except +
        void seed(uint32_t seed) nogil
        void frand4(float *out) nogil
        void rand4(uint32_t *out) nogil
        void rand_array(int count, uint32_t *out) nogil
        void frand_array(int count, float *out) nogil
        void cilk_rand_array(int count, uint32_t *out) nogil

    void c_rand_array "rand_array" (int count, unsigned int*) nogil

cdef extern from "lib/FastRandom.hpp":
    cdef FastRandom * FastRandom__create_aligned "FastRandom::create_aligned" (uint32_t seed)


cdef inline void cy_rand_array(int count, uint32_t *out) nogil:
    c_rand_array(count, out)
