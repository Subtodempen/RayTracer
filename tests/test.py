from _testModule import ffi
lib = ffi.dlopen("../build/libRayLib.so")

v = ffi.new("vec3*", {'x': 2, 'y': 2, 'z': 2})
print(lib.magnitude(v[0]))