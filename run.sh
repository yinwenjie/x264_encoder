#! /bin/bash
rm -f ./build/output.h264
./build/video_encoder /root/data/input_1280x720.yuv 1280 720 ./build/output.h264