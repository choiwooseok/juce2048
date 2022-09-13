#! /bin/sh

if [ ! -d build ];
then
    mkdir build
fi

cd build 
cmake .. -DCMAKE_PREFIX_PATH=thirdparty/JUCE/install
make -j8
cd -
