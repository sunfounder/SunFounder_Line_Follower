# ctypes_test.py
import ctypes
import pathlib

if __name__ == "__main__":
    print("Line Follower Python Test Application Running")
    # Load the shared library into ctypes
    libname = pathlib.Path().absolute() / "libLine_Follower.so"
    c_lib = ctypes.CDLL(libname)
    dt_list = c_lib.read_digital()
    ptr = ctypes.cast(dt_list,ctypes.POINTER(ctypes.c_int))
    print(ptr[0])
    print(ptr[1])
    print(ptr[2])
    print(ptr[3])
    print(ptr[4])
