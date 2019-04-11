from distutils.core import setup, Extension


module = Extension('cmdftools',
                    sources = ['cmdftools.cpp'],
                    include_dirs = [],
                    libraries = ['cmdftools'],
                    library_dirs = ['/usr/local/lib', './'],                    
                    extra_compile_args=['-std=c++11'])
 
setup(name = 'cmdftools',
      version = '1.0',
      description = 'cascaded multi-domain filter tool for fast image denoising',
      ext_modules = [module])
