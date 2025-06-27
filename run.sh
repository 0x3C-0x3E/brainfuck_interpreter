#!/bin/fish

cd build/
make -j8
cd ..
build/bf_interp res/hello_world.bf
