// io_data.cpp
#include "io_data.h"

#include <stdlib.h>
#include <string.h>

#include <iostream>

static FILE* input_file = nullptr;
static FILE* output_file = nullptr;

int32_t open_input_output_files(const char* input_name,
                                const char* output_name) {
  if (strlen(input_name) == 0 || strlen(output_name) == 0) {
    std::cerr << "Error: empty input or output file name." << std::endl;
    return -1;
  }

  close_input_output_files();

  input_file = fopen(input_name, "rb");
  if (input_file == nullptr) {
    std::cerr << "Error: failed to open input file." << std::endl;
    return -1;
  }
  output_file = fopen(output_name, "wb");
  if (output_file == nullptr) {
    std::cerr << "Error: failed to open output file." << std::endl;
    return -1;
  }

  return 0;
}

void close_input_output_files() {
  if (input_file != nullptr) {
    fclose(input_file);
    input_file = nullptr;
  }
  if (output_file != nullptr) {
    fclose(output_file);
    output_file = nullptr;
  }
}

int32_t read_yuv_to_picture(x264_picture_t* pic, int32_t frame_width,
                            int32_t frame_height) {
  int luma_size = frame_width * frame_height;
  int chroma_size = luma_size / 4;
  int size = fread(pic->img.plane[0], 1, luma_size, input_file);
  if (size != luma_size) {
    return -1;
  }
  size = fread(pic->img.plane[1], 1, chroma_size, input_file);
  if (size != chroma_size) {
    return -1;
  }
  size = fread(pic->img.plane[2], 1, chroma_size, input_file);
  if (size != chroma_size) {
    return -1;
  }
  return 0;
}

int32_t write_picture_to_file(uint8_t* buf, int size) {
  return fwrite(buf, size, 1, output_file);
}