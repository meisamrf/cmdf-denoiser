from distutils.core import setup, Extension
import platform

libname = 'bmcnn'
if platform.system()=='Darwin':
     libname = 'bmcnn.mac'

module = Extension('bmcnn',
                    sources = ['bmcnn.cpp'],
                    include_dirs = [],
                    libraries = [libname],
                    library_dirs = ['/usr/local/lib', './'],                    
                    extra_compile_args=['-std=c++11'])
 
setup(name = 'bmcnn',
      version = '1.0',
      description = 'blockmatching cnn tool for denoising',
      ext_modules = [module])
