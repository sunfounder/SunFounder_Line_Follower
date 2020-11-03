# Line Follower Modification for Using a C Module 

## Overview:
Line_Follower.c has the same functionality as the original Line_Follower.py
Line_Follower.c is compiled as a shared library with a name libLine_Follower.so
This library can then be called from a C or Python application.
The two '*_Test' files show how this is done respectively in C and Python.

## Main Files for the Library:
Line_Follower.h <br/>
Line_Follower.c <br/>
Makefile

## Test Files:
Line_Follower_Test.c <br/>
Line_Follower_Test.py

## Building the project:
make <br/>
make all - Does the same

## Cleaning the project:
make clean

## Running tests:
### Running the Line_Follower_Test C Application
export LD_LIBRARY_PATH=. <br/>
./Line_Follower <br/>
The following command runs the C Application also:<br/>
make test - Runs the Line_Follower_Test C executable <br/>
### Running the Line_Follower_Test.py Python Application
python3 Line_Follower_Test.py - Runs the Python Test
