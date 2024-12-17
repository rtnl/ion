#include "ion.h"

uint8_t ion_object_kind_index(t_ion_object_kind kind) {
  switch (kind) {
  case U0:
    return 0x00;

  case U8:
    return 0x01;

  case U16:
    return 0x02;

  case U32:
    return 0x03;

  case U64:
    return 0x04;

  case ARR:
    return 0x05;

  case LIST:
    return 0x06;
  }
}

size_t ion_object_kind_size(t_ion_object_kind kind) {
  switch (kind) {
  case U0:
    return 0;

  case U8:
    return 1;

  case U16:
    return 2;

  case U32:
    return 4;

  case U64:
    return 8;

  case ARR:
    return 16;

  case LIST:
    return 8;
  }
}