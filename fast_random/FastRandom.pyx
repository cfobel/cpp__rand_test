#cython: embedsignature=True
#cython: boundscheck=False
#cython: wraparound=False
#cython: nonecheck=False
cimport cython


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


cdef class cFastRandom:
    '''
    cFastRandom()
    '''
    cdef FastRandom *thisptr

    def __cinit__(self, seed=None):
        if seed is None:
            seed = 1
        self.thisptr = FastRandom__create_aligned(seed)

    def __dealloc__(self):
        del self.thisptr

    def seed(self, uint32_t seed):
        self.thisptr.seed(seed)

    def frand4(self, float [:] out):
        self.thisptr.frand4(&out[0])

    def rand4(self, uint32_t [:] out):
        self.thisptr.rand4(&out[0])

    def frand_array(self, float [:] out):
        self.thisptr.frand_array(len(out), &out[0])

    def rand_array(self, uint32_t [:] out):
        self.thisptr.rand_array(len(out), &out[0])

    #def cilk_rand_array(self, uint32_t [:] out):
        #self.thisptr.cilk_rand_array(len(out), &out[0])
