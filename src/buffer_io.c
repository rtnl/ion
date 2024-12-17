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

void ion_buffer_io_write_object_increment(t_ion_buffer *self) {
  switch (self->state.io_write_current) {
  case ARR:
  case LIST: {
    self->state.io_write_current_length++;
    break;
  }

  default:
    break;
  }
}

void ion_buffer_io_write_u0(t_ion_buffer *self) {
  ion_buffer_io_write_kind(self, U0);
  ion_buffer_io_write_object_increment(self);
}

void ion_buffer_io_write_u8(t_ion_buffer *self, uint8_t value) {
  ion_buffer_io_write_kind(self, U8);
  ion_buffer_io_write_data(self, U8, &value);
  ion_buffer_io_write_object_increment(self);
}

void ion_buffer_io_write_u16(t_ion_buffer *self, uint16_t value) {
  ion_buffer_io_write_kind(self, U16);
  ion_buffer_io_write_data(self, U16, &value);
  ion_buffer_io_write_object_increment(self);
}

void ion_buffer_io_write_u32(t_ion_buffer *self, uint32_t value) {
  ion_buffer_io_write_kind(self, U32);
  ion_buffer_io_write_data(self, U32, &value);
  ion_buffer_io_write_object_increment(self);
}

void ion_buffer_io_write_u64(t_ion_buffer *self, uint64_t value) {
  ion_buffer_io_write_kind(self, U64);
  ion_buffer_io_write_data(self, U64, &value);
  ion_buffer_io_write_object_increment(self);
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

  ion_buffer_io_write_object_increment(self);
}

void ion_buffer_io_write_list_open(t_ion_buffer *self) {
  if (self == NULL)
    return;

  self->state.io_write_current = LIST;
  self->state.io_write_current_start = self->curr;
  self->state.io_write_current_length = 0;

  ion_buffer_io_write_kind(self, LIST);
  ion_buffer_write(self, &self->state.io_write_current_length, 1);
}

void ion_buffer_io_write_list_close(t_ion_buffer *self) {
  uint8_t *ptr;

  if (self == NULL)
    return;

  ptr = &self->body[self->state.io_write_current_start];
  ptr++;
  *ptr = self->state.io_write_current_length;

  self->state.io_write_current = U0;

  ion_buffer_io_write_object_increment(self);
}
