#! /bin/bash
rm -rf build
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j8
./video_encoder ~/Video/input_1280x720.yuv 1280 720 output.h264