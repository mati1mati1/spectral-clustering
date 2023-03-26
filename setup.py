from setuptools import Extension, setup

module = Extension("spkmeansmodule", sources=['spkmeansmodule.c', 'matrix.c', 'jacobi.c', 'file_utils.c', 'spkmeans.c', 'kmeans.c'])
setup(name='spkmeansmodule',
     version='1.0',
     description='Python wrapper for custom C extension',
     ext_modules=[module])