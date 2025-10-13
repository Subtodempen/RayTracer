from cffi import FFI
import os

ffi = FFI()

headerPath = '../include/'
srcPath = '../src/'

headerFiles = ["ray.h", "ppm.h", "color.h"]

cdefStr = ""
for header in headerFiles:
    with open(headerPath + header) as hFile:
        cdefStr += hFile.read() + '\n'

ffi.cdef(cdefStr)

# load them into ffi.cdef
# ffi.dload the shared library or load it in from src
ffi.set_source(
    "_testModule", None
)
