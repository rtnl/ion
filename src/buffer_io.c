#include "ion.h"

t_ion_result_code ion_buffer_io_write_kind(t_ion_buffer *self,
                                           t_ion_object_kind kind) {
  uint8_t index;

  if (self == NULL)
    return RESULT_ERROR;

  index = ion_object_kind_index(kind);

  return ion_buffer_write(self, &index, sizeof(index));
}

t_ion_result_code ion_buffer_io_write_data(t_ion_buffer *self,
                                           t_ion_object_kind kind, void *src) {
  if (self == NULL)
    return RESULT_ERROR;

  if (src == NULL)
    return RESULT_ERROR;

  return ion_buffer_write(self, src, ion_object_kind_size(kind));
}

t_ion_result_code ion_buffer_io_read_kind(t_ion_buffer *self,
                                          t_ion_object_kind *value) {
  t_ion_result_code result;
  uint8_t index;

  if (self == NULL)
    return RESULT_ERROR;

  result = ion_buffer_read(self, &index, sizeof(index));
  if (result != RESULT_OK) {
    return result;
  }

  *value = ion_object_kind_from_index(index);

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_data(t_ion_buffer *self,
                                          t_ion_object_kind kind, void *dst) {
  if (self == NULL)
    return RESULT_ERROR;

  return ion_buffer_read(self, dst, ion_object_kind_size(kind));
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

void ion_buffer_io_read_object_increment(t_ion_buffer *self) {
  switch (self->state.io_read_current) {
  case ARR:
  case LIST: {
    self->state.io_read_current_length++;
    break;
  }

  default:
    break;
  }
}

t_ion_result_code ion_buffer_io_write_u0(t_ion_buffer *self) {
  t_ion_result_code result;

  result = ion_buffer_io_write_kind(self, U0);
  if (result != RESULT_OK) {
    return result;
  }

  ion_buffer_io_write_object_increment(self);

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_write_u8(t_ion_buffer *self, uint8_t value) {
  t_ion_result_code result;

  result = ion_buffer_io_write_kind(self, U8);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_io_write_data(self, U8, &value);
  if (result != RESULT_OK) {
    return result;
  }

  ion_buffer_io_write_object_increment(self);

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_write_u16(t_ion_buffer *self, uint16_t value) {
  t_ion_result_code result;

  result = ion_buffer_io_write_kind(self, U16);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_io_write_data(self, U16, &value);
  if (result != RESULT_OK) {
    return result;
  }

  ion_buffer_io_write_object_increment(self);

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_write_u32(t_ion_buffer *self, uint32_t value) {
  t_ion_result_code result;

  result = ion_buffer_io_write_kind(self, U32);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_io_write_data(self, U32, &value);
  if (result != RESULT_OK) {
    return result;
  }

  ion_buffer_io_write_object_increment(self);

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_write_u64(t_ion_buffer *self, uint64_t value) {
  t_ion_result_code result;

  result = ion_buffer_io_write_kind(self, U64);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_io_write_data(self, U64, &value);
  if (result != RESULT_OK) {
    return result;
  }

  ion_buffer_io_write_object_increment(self);

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_write_arr(t_ion_buffer *self,
                                          t_ion_object_kind kind, void *value,
                                          uint8_t len) {
  t_ion_result_code result;
  size_t size;
  uint8_t x;

  result = ion_buffer_io_write_kind(self, ARR);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_io_write_kind(self, kind);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_io_write_data(self, U8, &len);
  if (result != RESULT_OK) {
    return result;
  }

  size = ion_object_kind_size(kind);
  for (x = 0; x < len; x++) {
    result = ion_buffer_io_write_data(self, kind, value + size * x);
    if (result != RESULT_OK) {
      return result;
    }
  }

  ion_buffer_io_write_object_increment(self);

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_write_list_open(t_ion_buffer *self) {
  t_ion_result_code result;

  if (self == NULL)
    return RESULT_ERROR;

  self->state.io_write_current = LIST;
  self->state.io_write_current_start = self->curr;
  self->state.io_write_current_length = 0;

  result = ion_buffer_io_write_kind(self, LIST);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_write(self, &self->state.io_write_current_length, 1);
  if (result != RESULT_OK) {
    return result;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_write_list_close(t_ion_buffer *self) {
  uint8_t *ptr;

  if (self == NULL)
    return RESULT_ERROR;

  ptr = &self->body[self->state.io_write_current_start];
  ptr++;
  *ptr = self->state.io_write_current_length;

  self->state.io_write_current = U0;

  ion_buffer_io_write_object_increment(self);

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_u0(t_ion_buffer *self) {
  t_ion_result_code result;
  t_ion_object_kind kind;

  result = ion_buffer_io_read_kind(self, &kind);
  if (result != RESULT_OK) {
    return result;
  }

  if (kind != U0) {
    return RESULT_ERROR;
  }

  ion_buffer_io_read_object_increment(self);

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_u8(t_ion_buffer *self, uint8_t *value) {
  t_ion_result_code result;
  t_ion_object_kind kind;

  result = ion_buffer_io_read_kind(self, &kind);
  if (result != RESULT_OK) {
    return result;
  }

  if (kind != U8) {
    return RESULT_ERROR;
  }

  result = ion_buffer_io_read_data(self, U8, value);
  if (result != RESULT_OK) {
    return result;
  }

  ion_buffer_io_read_object_increment(self);

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_u16(t_ion_buffer *self, uint16_t *value) {
  t_ion_result_code result;
  t_ion_object_kind kind;

  result = ion_buffer_io_read_kind(self, &kind);
  if (result != RESULT_OK) {
    return result;
  }

  if (kind != U16) {
    return RESULT_ERROR;
  }

  result = ion_buffer_io_read_data(self, U16, value);
  if (result != RESULT_OK) {
    return result;
  }

  ion_buffer_io_read_object_increment(self);

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_u32(t_ion_buffer *self, uint32_t *value) {
  t_ion_result_code result;
  t_ion_object_kind kind;

  result = ion_buffer_io_read_kind(self, &kind);
  if (result != RESULT_OK) {
    return result;
  }

  if (kind != U32) {
    return RESULT_ERROR;
  }

  result = ion_buffer_io_read_data(self, U32, value);
  if (result != RESULT_OK) {
    return result;
  }

  ion_buffer_io_read_object_increment(self);

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_u64(t_ion_buffer *self, uint64_t *value) {
  t_ion_result_code result;
  t_ion_object_kind kind;

  result = ion_buffer_io_read_kind(self, &kind);
  if (result != RESULT_OK) {
    return result;
  }

  if (kind != U64) {
    return RESULT_ERROR;
  }

  result = ion_buffer_io_read_data(self, U64, value);
  if (result != RESULT_OK) {
    return result;
  }

  ion_buffer_io_read_object_increment(self);

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_arr(t_ion_buffer *self,
                                         t_ion_object_kind kind, void *value,
                                         uint8_t limit) {
  t_ion_result_code result;
  t_ion_object_kind kind_root;
  t_ion_object_kind kind_item;
  uint8_t len;
  size_t size;
  uint8_t t;
  uint8_t x;

  result = ion_buffer_io_read_kind(self, &kind_root);
  if (result != RESULT_OK) {
    return result;
  }

  if (kind_root != ARR) {
    return RESULT_ERROR;
  }

  result = ion_buffer_io_read_kind(self, &kind_item);
  if (result != RESULT_OK) {
    return result;
  }

  if (kind_item != kind) {
    return RESULT_ERROR;
  }

  result = ion_buffer_io_read_data(self, U8, &len);
  if (result != RESULT_OK) {
    return result;
  }

  t = min_u8(limit, len);

  size = ion_object_kind_size(kind);
  for (x = 0; x < t; x++) {
    result = ion_buffer_io_read_data(self, kind, value + size * x);
    if (result != RESULT_OK) {
      return result;
    }
  }

  ion_buffer_io_read_object_increment(self);

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_list_open(t_ion_buffer *self,
                                               uint8_t *len) {
  t_ion_result_code result;
  t_ion_object_kind kind;

  if (self == NULL)
    return RESULT_ERROR;

  result = ion_buffer_io_read_kind(self, &kind);
  if (result != RESULT_OK) {
    return RESULT_ERROR;
  }

  if (kind != LIST) {
    return RESULT_ERROR;
  }

  result = ion_buffer_read(self, len, 1);
  if (result != RESULT_OK) {
    return RESULT_ERROR;
  }

  self->state.io_read_current = LIST;
  self->state.io_read_current_start = self->curr;
  self->state.io_read_current_length = 0;

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_list_close(t_ion_buffer *self) {
  uint8_t *ptr;

  if (self == NULL)
    return RESULT_ERROR;

  ion_buffer_io_read_object_increment(self);

  return RESULT_OK;
}
