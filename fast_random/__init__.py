import os


def get_includes():
    import fast_random
    """
    Return the directory that contains the fast_random Cython \\*.hpp
    and \\*.pxd header files.

    Extension modules that need to compile against fast_random should
    use this function to locate the appropriate include directory.

    Notes
    -----
    When using ``distutils``, for example in ``setup.py``.
    ::

        import fast_random
        ...
        Extension('extension_name', ...
                  include_dirs=[...] + fast_random.get_includes())
        ...

    """
    return [os.path.abspath(os.path.join(os.path.dirname(fast_random.__file__),
                                         'lib'))]

