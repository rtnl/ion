#include "ion.h"

t_ion_buffer_state_io_entry *
ion_buffer_state_io_entry_new(t_ion_object_kind kind, t_ion_object_kind item,
                              size_t start) {
  t_ion_buffer_state_io_entry *self;

  self = malloc(sizeof(t_ion_buffer_state_io_entry));
  self->kind = kind;
  self->item = item;
  self->start = start;
  self->length = 0;

  return (self);
}

void ion_buffer_state_io_entry_free(t_ion_buffer_state_io_entry *self) {
  if (self == NULL)
    return;

  free(self);
}

t_ion_buffer_state *ion_buffer_state_new() {
  t_ion_buffer_state *self;

  self = (t_ion_buffer_state *)malloc(sizeof(t_ion_buffer_state));
  self->entry_list = vector_new(sizeof(t_ion_buffer_state_io_entry *));
  self->entry_level = -1;

  return (self);
}

void ion_buffer_state_free(t_ion_buffer_state *self) {
  if (self == NULL)
    return;

  free(vector_consume(self->entry_list));
  free(self);
}

t_ion_buffer_state *ion_buffer_state_clone(t_ion_buffer_state *self) {
  t_ion_buffer_state *other;

  if (self == NULL)
    return (NULL);

  other = malloc(sizeof(t_ion_buffer_state));
  other->entry_level = self->entry_level;
  other->entry_list = vector_clone(self->entry_list);

  return (other);
}

t_ion_result_code ion_buffer_state_io_write_increment(t_ion_buffer *self,
                                                      t_ion_object_kind kind) {
  t_ion_result_code result;
  t_ion_buffer_state *state;
  t_ion_buffer_state_io_entry **entry_ptr;
  t_ion_buffer_state_io_entry *entry;

  if (self == NULL)
    return RESULT_ERROR;

  state = self->state_w;

  if (state->entry_level >= 0) {
    result =
        vector_get(state->entry_list, (void **)&entry_ptr, state->entry_level);
    if (result != RESULT_OK)
      return result;

    entry = *entry_ptr;

    switch (entry->kind) {
    case ARR: {
      if (kind != entry->item) {
        return RESULT_ERROR;
      }

      break;
    }

    default:
      break;
    }

    entry->length++;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_state_io_write_open(t_ion_buffer *self,
                                                 t_ion_object_kind kind,
                                                 t_ion_object_kind item) {
  t_ion_result_code result;
  t_ion_buffer_state *state;
  t_ion_buffer_state_io_entry *entry;
  size_t start;
  t_ion_object_kind val_kind;
  uint8_t val_len;

  if (self == NULL)
    return RESULT_ERROR;

  state = self->state_w;
  state->entry_level++;

  start = self->body->curr_w;

  val_kind = kind;
  result = ion_buffer_write(self, &val_kind, 1);
  if (result != RESULT_OK) {
    return result;
  }

  switch (kind) {
  case ARR: {
    val_kind = item;
    result = ion_buffer_write(self, &val_kind, 1);
    if (result != RESULT_OK) {
      return result;
    }

    break;
  }

  case LIST:
    break;

  default:
    return RESULT_ERROR;
  }

  // object size that will be replaced when closed
  val_len = 0;
  result = ion_buffer_write(self, &val_len, 1);
  if (result != RESULT_OK) {
    return result;
  }

  entry = ion_buffer_state_io_entry_new(kind, item, start);
  result = vector_write(state->entry_list, &entry, 1);
  if (result != RESULT_OK) {
    return result;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_state_io_write_close(t_ion_buffer *self) {
  t_ion_result_code result;
  t_ion_buffer_state *state;
  t_ion_buffer_state_io_entry **entry_ptr;
  t_ion_buffer_state_io_entry *entry;
  uint8_t entry_length;
  uint8_t *entry_length_ptr;

  if (self == NULL)
    return RESULT_ERROR;

  state = self->state_w;

  if (state->entry_level < 0)
    return RESULT_ERROR;

  result =
      vector_get(state->entry_list, (void **)&entry_ptr, state->entry_level);
  if (result != RESULT_OK)
    return result;

  entry = *entry_ptr;
  entry_length = entry->length;
  entry_length_ptr = &entry_length;

  switch (entry->kind) {
  case ARR:
    result =
        vector_set(self->body, (void **)&entry_length_ptr, entry->start + 2);
    if (result != RESULT_OK)
      return result;

    break;

  case LIST:
    result =
        vector_set(self->body, (void **)&entry_length_ptr, entry->start + 1);
    if (result != RESULT_OK)
      return result;

    break;

  default:
    return RESULT_ERROR;
  }

  state->entry_level--;
  ion_buffer_state_io_write_increment(self, entry->kind);
  ion_buffer_state_io_entry_free(entry);
  vector_seek_relative_write(state->entry_list, -1);

  return RESULT_OK;
}

t_ion_result_code ion_buffer_state_io_read_increment(t_ion_buffer *self,
                                                     t_ion_object_kind kind) {
  if (self == NULL)
    return RESULT_ERROR;

  return RESULT_OK;
}

t_ion_result_code ion_buffer_state_io_peek_increment(t_ion_buffer *self,
                                                     t_ion_object_kind kind) {
  if (self == NULL) {
    return RESULT_ERROR;
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_state_io_read_open(t_ion_buffer *self,
                                                t_ion_object_kind kind,
                                                t_ion_object_kind *kind_item,
                                                uint8_t *len) {
  t_ion_result_code result;
  t_ion_buffer_state *state;
  t_ion_buffer_state_io_entry *entry;
  size_t start;
  t_ion_object_kind val_kind;

  if (self == NULL)
    return RESULT_ERROR;

  state = self->state_r;
  state->entry_level++;

  start = self->body->curr_r;

  result = ion_buffer_io_read_kind(self, &val_kind);
  if (result != RESULT_OK) {
    return (result);
  }

  switch (val_kind) {
  case ARR: {
    result = ion_buffer_io_read_kind(self, &val_kind);
    if (result != RESULT_OK) {
      return (result);
    }

    *kind_item = val_kind;

    result = ion_buffer_io_read_data(self, U8, len);
    if (result != RESULT_OK) {
      return (result);
    }

    break;
  }

  case LIST: {
    result = ion_buffer_io_read_data(self, U8, len);
    if (result != RESULT_OK) {
      return result;
    }

    break;
  }

  default:
    return (RESULT_ERROR);
  }

  entry = ion_buffer_state_io_entry_new(kind, val_kind, start);
  result = vector_write(state->entry_list, (void **)&entry, 1);
  if (result != RESULT_OK) {
    return (result);
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_state_io_read_close(t_ion_buffer *self) {
  t_ion_result_code result;
  t_ion_buffer_state *state;

  if (self == NULL)
    return RESULT_ERROR;

  state = self->state_r;
  state->entry_level--;

  return RESULT_OK;
}

t_ion_result_code ion_buffer_state_io_peek_open(t_ion_buffer *self,
                                                t_ion_object_kind kind,
                                                t_ion_object_kind *kind_item,
                                                uint8_t *len) {
  t_ion_result_code result;
  t_ion_buffer_state *state;
  t_ion_buffer_state_io_entry *entry;
  size_t start;
  t_ion_object_kind val_kind;

  if (self == NULL)
    return RESULT_ERROR;

  state = self->state_p;
  state->entry_level++;

  start = self->body->curr_p;

  result = ion_buffer_io_peek_kind(self, &val_kind);
  if (result != RESULT_OK) {
    return (result);
  }

  switch (val_kind) {
  case ARR: {
    result = ion_buffer_io_peek_kind(self, &val_kind);
    if (result != RESULT_OK) {
      return (result);
    }

    *kind_item = val_kind;

    result = ion_buffer_io_peek_data(self, U8, len);
    if (result != RESULT_OK) {
      return (result);
    }

    break;
  }

  case LIST: {
    result = ion_buffer_io_read_data(self, U8, len);
    if (result != RESULT_OK) {
      return result;
    }

    break;
  }

  default:
    return (RESULT_ERROR);
  }

  entry = ion_buffer_state_io_entry_new(kind, val_kind, start);
  result = vector_write(state->entry_list, (void **)&entry, 1);
  if (result != RESULT_OK) {
    return (result);
  }

  return RESULT_OK;
}

t_ion_result_code ion_buffer_state_io_peek_close(t_ion_buffer *self) {
  t_ion_result_code result;
  t_ion_buffer_state *state;

  if (self == NULL)
    return RESULT_ERROR;

  state = self->state_p;
  state->entry_level--;

  return RESULT_OK;
}
