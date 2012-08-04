from distutils.core import setup, Extension

telldus = Extension(
    'telldus',
    include_dirs = ['/usr/local/include'],
    libraries = ['telldus-core'],
    library_dirs = ['/usr/lib'],
    sources = ['telldus.c']
)

setup(
    name = 'telldus',
    version = '1.0',
    description = 'Python bindings for telldus',
    author='Oyvind Saltvik',
    author_email='oyvind.saltvik@gmail.com',
    url='http://github.com/fivethreeo/telldus/',
    ext_modules = [telldus]
)