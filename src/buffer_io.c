#include "ion.h"

t_ion_result_code ion_buffer_io_write_kind(t_ion_buffer *self,
                                           t_ion_object_kind kind) {
  t_ion_result_code result;
  t_ion_buffer_state *state;
  t_ion_buffer_state_io_entry **entry_ptr;
  t_ion_buffer_state_io_entry *entry;
  uint8_t index;
  uint8_t flag;

  if (self == NULL)
    return RESULT_ERROR;

  state = self->state;
  if (state->entry_level < 0) {
    flag = true;
  } else {
    result =
        vector_get(state->entry_list, (void **)&entry_ptr, state->entry_level);
    if (result != RESULT_OK) {
      return result;
    }

    entry = *entry_ptr;

    switch (entry->kind) {
    case ARR: {
      flag = false;
      break;
    }

    default: {
      flag = true;
      break;
    }
    }
  }

  index = ion_object_kind_index(kind);

  if (flag == true) {
    result = ion_buffer_write(self, &index, sizeof(index));
    if (result != RESULT_OK) {
      return result;
    }
  }

  return RESULT_OK;
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

  result = ion_buffer_read(self, &index, sizeof(index));
  if (result != RESULT_OK) {
    return result;
  }

  *value = ion_object_kind_from_index(index);

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_check_kind(t_ion_buffer *self,
                                           t_ion_object_kind expected) {
  t_ion_result_code result;
  t_ion_buffer_state *state;
  t_ion_buffer_state_io_entry **entry_ptr;
  t_ion_buffer_state_io_entry *entry;
  t_ion_object_kind value;
  uint8_t index;
  uint8_t flag;

  if (self == NULL)
    return RESULT_ERROR;

  state = self->state;
  if (state->entry_level < 0) {
    flag = true;
  } else {
    result =
        vector_get(state->entry_list, (void **)&entry_ptr, state->entry_level);
    if (result != RESULT_OK) {
      return result;
    }

    entry = *entry_ptr;

    switch (entry->kind) {
    case ARR: {
      flag = false;
      break;
    }

    default: {
      flag = true;
      break;
    }
    }
  }

  if (!flag) {
    return RESULT_OK;
  }

  result = ion_buffer_read(self, &index, sizeof(index));
  if (result != RESULT_OK) {
    return result;
  }

  value = ion_object_kind_from_index(index);
  if (value != expected) {
    return RESULT_ERROR;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_data(t_ion_buffer *self,
                                          t_ion_object_kind kind, void *dst) {
  if (self == NULL)
    return RESULT_ERROR;

  return ion_buffer_read(self, dst, ion_object_kind_size(kind));
}

t_ion_result_code ion_buffer_io_write_u0(t_ion_buffer *self) {
  t_ion_result_code result;

  result = ion_buffer_io_write_kind(self, U0);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_state_io_write_increment(self, U0);
  if (result != RESULT_OK) {
    return result;
  }

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

  result = ion_buffer_state_io_write_increment(self, U8);
  if (result != RESULT_OK) {
    return result;
  }

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

  result = ion_buffer_state_io_write_increment(self, U16);
  if (result != RESULT_OK) {
    return result;
  }

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

  result = ion_buffer_state_io_write_increment(self, U32);
  if (result != RESULT_OK) {
    return result;
  }

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

  result = ion_buffer_state_io_write_increment(self, U64);
  if (result != RESULT_OK) {
    return result;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_write_arr_open(t_ion_buffer *self,
                                               t_ion_object_kind kind) {
  t_ion_result_code result;

  if (self == NULL)
    return RESULT_ERROR;

  result = ion_buffer_state_io_write_open(self, ARR, kind);
  if (result != RESULT_OK) {
    return result;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_write_arr_close(t_ion_buffer *self) {
  t_ion_result_code result;
  uint8_t *ptr;

  if (self == NULL)
    return RESULT_ERROR;

  result = ion_buffer_state_io_write_close(self);
  if (result != RESULT_OK) {
    return result;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_write_list_open(t_ion_buffer *self) {
  t_ion_result_code result;

  if (self == NULL)
    return RESULT_ERROR;

  result = ion_buffer_state_io_write_open(self, LIST, U0);
  if (result != RESULT_OK) {
    return result;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_write_list_close(t_ion_buffer *self) {
  t_ion_result_code result;
  uint8_t *ptr;

  if (self == NULL)
    return RESULT_ERROR;

  result = ion_buffer_state_io_write_close(self);
  if (result != RESULT_OK) {
    return result;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_u0(t_ion_buffer *self) {
  t_ion_result_code result;

  result = ion_buffer_io_check_kind(self, U0);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_state_io_read_increment(self, U0);
  if (result != RESULT_OK) {
    return result;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_u8(t_ion_buffer *self, uint8_t *value) {
  t_ion_result_code result;

  result = ion_buffer_io_check_kind(self, U8);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_io_read_data(self, U8, value);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_state_io_read_increment(self, U8);
  if (result != RESULT_OK) {
    return result;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_u16(t_ion_buffer *self, uint16_t *value) {
  t_ion_result_code result;

  result = ion_buffer_io_check_kind(self, U16);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_io_read_data(self, U16, value);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_state_io_read_increment(self, U16);
  if (result != RESULT_OK) {
    return result;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_u32(t_ion_buffer *self, uint32_t *value) {
  t_ion_result_code result;

  result = ion_buffer_io_check_kind(self, U32);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_io_read_data(self, U32, value);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_state_io_read_increment(self, U32);
  if (result != RESULT_OK) {
    return result;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_u64(t_ion_buffer *self, uint64_t *value) {
  t_ion_result_code result;

  result = ion_buffer_io_check_kind(self, U64);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_io_read_data(self, U64, value);
  if (result != RESULT_OK) {
    return result;
  }

  result = ion_buffer_state_io_read_increment(self, U64);
  if (result != RESULT_OK) {
    return result;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_arr_open(t_ion_buffer *self,
                                              t_ion_object_kind *kind,
                                              uint8_t *len) {
  t_ion_result_code result;

  if (self == NULL) {
    return RESULT_ERROR;
  }

  result = ion_buffer_state_io_read_open(self, ARR, kind, len);
  if (result != RESULT_OK) {
    return RESULT_ERROR;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_arr_close(t_ion_buffer *self) {
  t_ion_result_code result;
  uint8_t *ptr;

  if (self == NULL)
    return RESULT_ERROR;

  result = ion_buffer_state_io_read_close(self);
  if (result != RESULT_OK) {
    return RESULT_ERROR;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_list_open(t_ion_buffer *self,
                                               uint8_t *len) {
  t_ion_result_code result;
  t_ion_object_kind kind;

  if (self == NULL)
    return RESULT_ERROR;

  result = ion_buffer_state_io_read_open(self, LIST, &kind, len);
  if (result != RESULT_OK) {
    return RESULT_ERROR;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_io_read_list_close(t_ion_buffer *self) {
  t_ion_result_code result;
  uint8_t *ptr;

  if (self == NULL)
    return RESULT_ERROR;

  result = ion_buffer_state_io_read_close(self);
  if (result != RESULT_OK) {
    return RESULT_ERROR;
  }

  return RESULT_OK;
}
