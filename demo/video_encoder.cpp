#include <stdint.h>
#include <stdlib.h>
#include <x264.h>

#include <iostream>

#include "io_data.h"

static void usage() {
  std::cout << "Usage: video_encoder input_file width height output_file"
            << std::endl;
}

int main(int argc, char **argv) {
  std::cout << "Video Encoder." << std::endl;
  if (argc < 5) {
    usage();
    return EXIT_FAILURE;
  }

  char *input_file_name = argv[1];
  int32_t frame_width = atoi(argv[2]);
  int32_t frame_height = atoi(argv[3]);
  char *output_file_name = argv[4];

  std::cout << "Input file:" << std::string(input_file_name) << std::endl;
  std::cout << "output file:" << std::string(output_file_name) << std::endl;
  std::cout << "Frame size:" << frame_width << "x" << frame_height << std::endl;

  x264_param_t param;
  x264_picture_t pic;
  x264_picture_t pic_out;
  x264_t *h;
  int i_frame = 0;
  int i_frame_size;
  x264_nal_t *nal;
  int i_nal;

  int32_t result = open_input_output_files(input_file_name, output_file_name);
  if (result < 0) {
    return result;
  }

  /* Get default params for preset/tuning */
  if (x264_param_default_preset(&param, "medium", NULL) < 0) {
    goto fail;
  }

  /* Configure non-default params */
  param.i_bitdepth = 8;
  param.i_csp = X264_CSP_I420;
  param.i_width = frame_width;
  param.i_height = frame_height;
  param.b_vfr_input = 0;
  param.b_repeat_headers = 1;
  param.b_annexb = 1;

  /* Apply profile restrictions. */
  result = x264_param_apply_profile(&param, "high");
  if (result < 0) {
    goto fail;
  }
  result = x264_picture_alloc(&pic, param.i_csp, param.i_width, param.i_height);
  if (result < 0) {
    goto fail;
  }
#undef fail
#define fail fail2

  h = x264_encoder_open(&param);
  if (!h) {
    goto fail;
  }
#undef fail
#define fail fail3

  for (;; i_frame++) {
    result = read_yuv_to_picture(&pic, frame_width, frame_height);
    if (result < 0) {
      break;
    }
    pic.i_pts = i_frame;
    i_frame_size = x264_encoder_encode(h, &nal, &i_nal, &pic, &pic_out);
    std::cout << "Encode frame:" << i_frame << ",size:" << i_frame_size
              << std::endl;
    if (i_frame_size < 0) {
      goto fail;
    } else if (i_frame_size) {
      result = write_picture_to_file(nal->p_payload, i_frame_size);
      if (result < 0) {
        goto fail;
      }
    }
  }

  /* Flush delayed frames */
  while (x264_encoder_delayed_frames(h)) {
    i_frame_size = x264_encoder_encode(h, &nal, &i_nal, NULL, &pic_out);
    if (i_frame_size < 0) {
      goto fail;
    } else if (i_frame_size) {
      result = write_picture_to_file(nal->p_payload, i_frame_size);
      if (result < 0) {
        goto fail;
      }
    }
  }

  close_input_output_files();
  return EXIT_SUCCESS;

#undef fail
fail3:
  x264_encoder_close(h);
fail2:
  x264_picture_clean(&pic);
fail:
  return EXIT_FAILURE;
}