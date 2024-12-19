#include "ion.h"

size_t min_sizet(size_t x, size_t y) {
  if (x < y) {
    return x;
  } else {
    return y;
  }
}

uint8_t min_u8(uint8_t x, uint8_t y) {
  if (x < y) {
    return x;
  } else {
    return y;
  }
}
