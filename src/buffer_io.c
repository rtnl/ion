#include "ion.h"

void ion_buffer_io_write_kind(t_ion_buffer *self, t_ion_object_kind kind) {
  uint8_t index;

  if (self == NULL)
    return;

  index = ion_object_kind_index(kind);

  ion_buffer_write(self, &index, sizeof(index));
}

void ion_buffer_io_write_data(t_ion_buffer *self, t_ion_object_kind kind,
                              void *src) {
  if (self == NULL)
    return;

  if (src == NULL)
    return;

  ion_buffer_write(self, src, ion_object_kind_size(kind));
}

void ion_buffer_io_write_u0(t_ion_buffer *self) {
  ion_buffer_io_write_kind(self, U0);
}

void ion_buffer_io_write_u8(t_ion_buffer *self, uint8_t value) {
  ion_buffer_io_write_kind(self, U8);
  ion_buffer_io_write_data(self, U8, &value);
}

void ion_buffer_io_write_u16(t_ion_buffer *self, uint16_t value) {
  ion_buffer_io_write_kind(self, U16);
  ion_buffer_io_write_data(self, U16, &value);
}

void ion_buffer_io_write_u32(t_ion_buffer *self, uint32_t value) {
  ion_buffer_io_write_kind(self, U32);
  ion_buffer_io_write_data(self, U32, &value);
}

void ion_buffer_io_write_u64(t_ion_buffer *self, uint64_t value) {
  ion_buffer_io_write_kind(self, U64);
  ion_buffer_io_write_data(self, U64, &value);
}

void ion_buffer_io_write_arr(t_ion_buffer *self, t_ion_object_kind kind,
                             void *value, uint8_t len) {
  size_t size;
  uint8_t x;

  ion_buffer_io_write_kind(self, ARR);
  ion_buffer_io_write_kind(self, kind);
  ion_buffer_io_write_data(self, U8, &len);

  size = ion_object_kind_size(kind);
  for (x = 0; x < len; x++) {
    ion_buffer_io_write_data(self, kind, value + size * x);
  }
}
