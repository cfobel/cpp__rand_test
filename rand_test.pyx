#cython: boundscheck=False
#cython: wraparound=False
cimport cython
from libc.math cimport ceil

cdef extern from "UniformRandomGenerator.hpp":
    cdef cppclass UniformRandomGenerator:
        UniformRandomGenerator() except +
        unsigned int irand(unsigned int max_value) nogil # Max value (inclusive)
        float frand() nogil


#cdef extern from "fastrand.h":
    #int fastrand()
    #void fast_srand(int seed)


cdef extern from "rand_sse.h":
    void srand_sse(unsigned int seed) nogil
    void rand_sse(unsigned int*) nogil
    void rand_sse_array(int count, unsigned int*) nogil
    void rand_array(int count, unsigned int*) nogil


def py_srand_sse(unsigned int seed):
    srand_sse(seed)


cdef inline void _rand_sse_4(unsigned int *output) nogil:
    '''
    Write 4 random unsigned integers to the output array.
    '''
    rand_sse(output)


cdef inline void _rand_sse(int count, unsigned int *out) nogil:
    cdef int set_count = <int>ceil(count / 4.)
    cdef int start_index
    cdef int i

    for i in xrange(set_count):
        start_index = i * 4
        _rand_sse_4(&out[start_index])


cdef inline void _rand_sse_cilk(int count, unsigned int *out) nogil:
    cdef int set_count = <int>ceil(count / 4.)
    rand_sse_array(count, out)


cdef inline void _rand(int count, unsigned int *out) nogil:
    rand_array(count, out)


def py_rand_sse(unsigned int [:] out):
    _rand_sse(out.shape[0], &out[0])


def py_rand_sse_cilk(unsigned int [:] out):
    _rand_sse_cilk(out.shape[0], &out[0])


def py_rand(unsigned int [:] out):
    _rand(out.shape[0], &out[0])


#cpdef int py_fastrand():
    #return fastrand()


#def py_fast_srand(int seed):
    #fast_srand(seed)


def py_uniform_rand(unsigned int [:] out):
    cdef UniformRandomGenerator *my_rng = new UniformRandomGenerator()
    cdef int i
    try:
        for i in xrange(out.shape[0]):
            out[i] = my_rng.irand(1 << 30)
    finally:
        del my_rng
