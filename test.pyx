import numpy as np
cimport numpy as np
cimport cython

cdef extern from "fastrand.h":
    int fastrand()
    void fast_srand(int seed)


cdef extern from "rand_sse.h":
    void srand_sse(unsigned int seed)
    void rand_sse(unsigned int*)
    void rand_sse_array(int count, unsigned int*)
    void rand_array(int count, unsigned int*)


def py_srand_sse(unsigned int seed):
    srand_sse(seed)


cdef _rand_sse_4(unsigned int *output):
    '''
    Write 4 random unsigned integers to the output array.
    '''
    rand_sse(output)


cdef inline _rand_sse(int count):
    cdef int set_count = np.ceil(count / 4.)
    cdef unsigned int [:] output = np.empty(4 * set_count, dtype=np.uint32)
    cdef int start_index
    cdef int i

    for i in xrange(set_count):
        start_index = i * 4
        _rand_sse_4(&output[start_index])
    return output


cdef inline _rand_sse_cilk(int count):
    cdef int set_count = np.ceil(count / 4.)
    cdef unsigned int [:] output = np.empty(4 * set_count, dtype=np.uint32)
    rand_sse_array(count, &output[0])
    return output


cdef inline _rand(int count):
    cdef unsigned int [:] output = np.empty(count, dtype=np.uint32)
    rand_array(count, &output[0])
    return output


def py_rand_sse(count=1):
    return _rand_sse(count)


def py_rand_sse_cilk(count=1):
    return _rand_sse_cilk(count)


def py_rand(count=1):
    return _rand(count)


cpdef int py_fastrand():
    return fastrand()


def py_fast_srand(int seed):
    fast_srand(seed)
