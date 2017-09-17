#!/bin/sh
set -e

g++ -L/usr/X11R6/lib -lX11 -lstdc++ --std=c++11 -c xgamelib.cpp
ar rcs ../../lib/xgamelib.a  xgamelib.o
rm -rf *.o *.gch