!# /bin/bash

rm -rf x264 x264_install
mkdir x264_install
git clone git@github.com:yinwenjie/x264.git
cd x264
./configure --prefix=../x264_install --enable-static --disable-opencl --disable-win32thread --disable-interlaced --disable-asm --enable-debug --disable-avs --disable-swscale --disable-lavf --disable-ffms --disable-gpac --disable-lsmash
make && make install