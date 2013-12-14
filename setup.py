#!/usr/bin/env python
import sys
from distutils.core import setup, Extension
from Cython.Build import cythonize

include_dirs = [] #cyplace.get_includes()
sys.path += include_dirs

cython_config = dict(include_dirs=include_dirs, language='c++',
                     libraries=['rt'], extra_compile_args=['-O3', '-std=c++0x',
                                                           '-Wfatal-errors'])

cython_exts = [Extension('fast_random.%s' % v,
                           ['fast_random/%s.pyx' % v],
                           #extra_objects=['fast_random/lib/%s.o' % lib
                                          #for lib in ('',
                                                      #'TilePairCilkUtil')],
                           **cython_config)
                 for v in ('FastRandom', )]


setup(name = "fast_random",
    version = "0.0.1",
    description = "C++ fast random number generation, with Cython bindings",
    keywords = "c++ random generator cython",
    author = "Christian Fobel",
    url = "https://github.com/cfobel/fast_random",
    license = "GPL",
    long_description = """""",
    packages = ['fast_random', ],
    ext_modules=cythonize(cython_exts,
                          pyrex_directives={'embedsignature': True})
)
