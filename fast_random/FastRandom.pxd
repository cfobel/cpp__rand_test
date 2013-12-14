from libc.math cimport ceil

cdef extern from "lib/UniformRandomGenerator.hpp":
    cdef cppclass UniformRandomGenerator:
        UniformRandomGenerator() except +
        unsigned int irand(unsigned int max_value) nogil # Max value (inclusive)
        float frand() nogil


cdef extern from "lib/rand_sse.h":
    void c_srand_sse "srand_sse" (unsigned int seed) nogil
    void c_rand_sse "rand_sse" (unsigned int*) nogil
    void c_rand_sse_array "rand_sse_array" (int count, unsigned int*) nogil
    void c_rand_array "rand_array" (int count, unsigned int*) nogil


cdef inline void cy_rand_sse_4(unsigned int *output) nogil:
    '''
    Write 4 random unsigned integers to the output array.
    '''
    c_rand_sse(output)


cdef inline void cy_rand_sse(int count, unsigned int *out) nogil:
    cdef int set_count = <int>ceil(count / 4.)
    cdef int start_index
    cdef int i

    for i in xrange(set_count):
        start_index = i * 4
        cy_rand_sse_4(&out[start_index])


cdef inline void cy_rand_sse_cilk(int count, unsigned int *out) nogil:
    cdef int set_count = <int>ceil(count / 4.)
    c_rand_sse_array(count, out)


cdef inline void cy_rand_array(int count, unsigned int *out) nogil:
    c_rand_array(count, out)
