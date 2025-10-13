from cffi import FFI
import os

ffi = FFI()

headerPath = '../include/'
srcPath = '../src/'

cdefStr = ""
with open(headerPath + "ray.h") as hFile:
    cdefStr = hFile.read()

ffi.cdef(cdefStr)

# load them into ffi.cdef
# ffi.dload the shared library or load it in from src
ffi.set_source(
    "_testModule", None
)
