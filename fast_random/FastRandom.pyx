#cython: embedsignature=True
#cython: boundscheck=False
#cython: wraparound=False
#cython: nonecheck=False
cimport cython


def srand_sse(unsigned int seed):
    c_srand_sse(seed)


def rand_sse(unsigned int [:] out):
    cy_rand_sse(out.shape[0], &out[0])


def rand_sse_cilk(unsigned int [:] out):
    cy_rand_sse_cilk(out.shape[0], &out[0])


def rand(unsigned int [:] out):
    cy_rand_array(out.shape[0], &out[0])


def uniform_rand(unsigned int [:] out):
    cdef UniformRandomGenerator *my_rng = new UniformRandomGenerator()
    cdef int i
    try:
        for i in xrange(out.shape[0]):
            out[i] = my_rng.irand(1 << 30)
    finally:
        del my_rng
