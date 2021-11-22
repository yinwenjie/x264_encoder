// io_data.h
#ifndef IO_DATA_H
#define IO_DATA_H
#include <stdint.h>
extern "C" {
#include <x264.h>
}

int32_t open_input_output_files(const char* input_name,
                                const char* output_name);
void close_input_output_files();

int32_t read_yuv_to_picture(x264_picture_t* pic, int32_t frame_width,
                            int32_t frame_height);

int32_t write_picture_to_file(uint8_t* buf, int size);
#endif