#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "turbojpeg.h"

#define EXIT_WITH_MSG(msg)                                                     \
  {                                                                            \
    fprintf(stderr, msg);                                                      \
    return EXIT_FAILURE;                                                       \
  }

// https://paulbourke.net/dataformats/asciiart/
#define MORE_DETAIL
#ifdef MORE_DETAIL
const char *BRIGHTNESS_MAP =
    "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
#else
const char *BRIGHTNESS_MAP = " .:-=+*#%@";
#endif

int get_region(int index, int m, int n) {
  int region_size = m / n;
  int n_extra_regions = m % n;
  int boundary = n_extra_regions * (region_size + 1);
  if (index < boundary) {
    return index / (region_size + 1); /* lower regions get an extra element */
  } else {
    return (index - boundary) / region_size + n_extra_regions;
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    EXIT_WITH_MSG(".jpeg file not given as argument\n");

  tjhandle tj_instance = tj3Init(TJINIT_DECOMPRESS);
  if (!tj_instance)
    EXIT_WITH_MSG("failed to initialize libjpeg-turbo\n");

  FILE *jpeg_file = fopen(argv[1], "rb");
  if (!jpeg_file)
    EXIT_WITH_MSG("failed to open test JPG file\n");

  /* file discovery */
  size_t jpeg_size;
  if (fseek(jpeg_file, 0, SEEK_END) < 0 ||
      ((jpeg_size = ftell(jpeg_file)) < 0) || fseek(jpeg_file, 0, SEEK_SET) < 0)
    EXIT_WITH_MSG("couldn't determine file size\n");

  /* buffer file */
  unsigned char *jpeg_buf = malloc(jpeg_size);
  if (!jpeg_buf)
    EXIT_WITH_MSG("failed to allocate memory for JPEG buffer\n");
  if (fread(jpeg_buf, jpeg_size, 1, jpeg_file) < 1)
    EXIT_WITH_MSG("failed reading input file\n");
  fclose(jpeg_file);
  jpeg_file = NULL;

  /* extract info */
  if (tj3DecompressHeader(tj_instance, jpeg_buf, jpeg_size) < 0)
    EXIT_WITH_MSG("failed to read JPEG header\n");

  int width = tj3Get(tj_instance, TJPARAM_JPEGWIDTH);
  int height = tj3Get(tj_instance, TJPARAM_JPEGHEIGHT);
  int precision = tj3Get(tj_instance, TJPARAM_PRECISION);
  if (precision > 8)
    EXIT_WITH_MSG("only 8-bit lossy files supported\n");

  int colorspace = tj3Get(tj_instance, TJPARAM_COLORSPACE);
  int pixel_format = TJPF_UNKNOWN;
  if (colorspace == TJCS_GRAY)
    pixel_format = TJPF_GRAY;
  else if (colorspace == TJCS_CMYK || colorspace == TJCS_YCCK)
    pixel_format = TJPF_CMYK;
  else
    pixel_format = TJPF_RGB;
  size_t raw_buf_size = width * height * tjPixelSize[pixel_format];
  unsigned char *jpeg_raw_buf = malloc(raw_buf_size);
  if (tj3Decompress8(tj_instance, jpeg_buf, jpeg_size, jpeg_raw_buf, 0,
                     pixel_format) < 0)
    EXIT_WITH_MSG("failed to decompress image into buffer\n");
  tj3Free(jpeg_buf);
  jpeg_buf = NULL;

  int incr = pixel_format == TJPF_GRAY ? 1 : 3;
  for (int i = 0; i < raw_buf_size; i += incr) {
    int val;
    if (pixel_format == TJPF_GRAY) {
      val = jpeg_raw_buf[i];
    } else {
      unsigned char red = jpeg_raw_buf[i];
      unsigned char green = jpeg_raw_buf[i + 1];
      unsigned char blue = jpeg_raw_buf[i + 2];
      // int val = (red + green + blue) / 3;
      val = round(red * 0.2126 + green * 0.7152 + blue * 0.0722);
    }
    int mapped_idx = get_region(val, 256, strlen(BRIGHTNESS_MAP));
    if (i % width == 0 && i > 0) {
      puts("");
    }
    printf("%c%c", BRIGHTNESS_MAP[mapped_idx], BRIGHTNESS_MAP[mapped_idx]);
  }
  puts("");

  tj3Destroy(tj_instance);
  tj3Free(jpeg_raw_buf);
  jpeg_buf = NULL;
  return EXIT_SUCCESS;
}
