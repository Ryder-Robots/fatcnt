#!/bin/bash
#
#
if [ -d "build" ];then
	rm -fR build
fi
mkdir build
cd build
cmake -S .. -B .
cmake --build . --target install/strip
cmake --build . --target test
cpack -G DEB
