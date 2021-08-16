# ctypes_test.py
import ctypes
import pathlib
import os 
import time 

if __name__ == "__main__":
    print("Line Follower Python Test Application Running")
    # Load the shared library into ctypes
    libname = os.path.abspath(".") + "/" + "libLine_Follower.so";
    print(libname)
    c_lib = ctypes.CDLL(libname)
    while True:
       dt_list = c_lib.read_digital()
       ptr = ctypes.cast(dt_list,ctypes.POINTER(ctypes.c_int))
       print(hex(id(ptr)))
       print(ptr[0], ptr[1], ptr[2], ptr[3], ptr[4])
       time.sleep(1)
