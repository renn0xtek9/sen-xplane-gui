#!/bin/bash
sudo mkdir -p /opt/sen-debug && sudo chown ubuntu /opt/sen-debug
cd /opt/sen-debug 
git clone https://github.com/airbus/sen.git
cd sen
git checkout 0.6.0
export SEN_GCC_VERSION=15
cp ~/.conan2/profiles/sen_gcc ~/.conan2/profiles/sen_gcc_debug
sed -i 's/build_type=Release/build_type=Debug/g' ~/.conan2/profiles/sen_gcc_debug
sed -i 's/compiler.version=12/compiler.version=15/g' ~/.conan2/profiles/sen_gcc_debug
conan config install -tf profiles .conan/profiles/sen_gcc
conan install . --profile:all=sen_gcc_debug --build=missing
cmake --preset conan-gcc-debug
cmake --build build/gcc/Debug/ --parallel 4
sudo mkdir -p $SEN_PREFIX-debug && sudo chown $(whoami) $SEN_PREFIX-debug
cmake --install build/gcc/Debug --prefix $SEN_PREFIX-debug
