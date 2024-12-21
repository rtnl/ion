#include "ion.h"

const size_t SIZE_DEFAULT = 1024;

t_ion_vector *vector_new(size_t unit) {
  t_ion_vector *self;

  self = (t_ion_vector *)malloc(sizeof(t_ion_vector));
  self->unit = unit;
  self->size = SIZE_DEFAULT;
  self->curr = 0;
  self->body = calloc(self->unit, self->size);

  return (self);
}

t_ion_vector *vector_clone(t_ion_vector *self) {
  t_ion_vector *other;

  if (self == NULL)
    return (NULL);

  other = (t_ion_vector *)malloc(sizeof(t_ion_vector));
  other->body = calloc(self->unit, self->size);
  other->unit = self->unit;
  other->size = self->size;
  other->curr = self->curr;

  memcpy(other->body, self->body, self->unit * self->size);

  return (other);
}

t_ion_result_code vector_seek(t_ion_vector *self, size_t index) {
  if (self == NULL)
    return RESULT_ERROR;

  if (index < 0)
    return RESULT_ERROR;

  if (index >= self->size)
    return RESULT_ERROR;

  self->curr = index;

  return RESULT_OK;
}

t_ion_result_code vector_seek_relative(t_ion_vector *self, size_t diff) {
  if (self == NULL)
    return RESULT_ERROR;

  return vector_seek(self, self->curr + diff);
}

t_ion_result_code vector_extend(t_ion_vector *self) {
  void *body;
  size_t size;

  size = self->size * 2;
  body = calloc(self->unit, size);

  memcpy(body, self->body, self->unit * self->size);
  free(self->body);

  self->body = body;
  self->size = size;

  return RESULT_OK;
}

t_ion_result_code vector_write(t_ion_vector *self, void *src, size_t len) {
  t_ion_result_code result;
  size_t len_total;
  size_t curr_total;

  if (self == NULL)
    return RESULT_ERROR;

  len_total = self->unit * len;
  curr_total = self->unit * self->curr;

  if ((self->curr + len_total) >= self->size) {
    result = vector_extend(self);
    if (result != RESULT_OK)
      return result;
  }

  memcpy(self->body + (self->unit * self->curr), src, len_total);
  self->curr += len;

  return RESULT_OK;
}

t_ion_result_code vector_read(t_ion_vector *self, void *dst, size_t len) {
  if (self == NULL)
    return RESULT_ERROR;

  if (len > (self->size - self->curr)) {
    return RESULT_ERROR;
  }

  memcpy(dst, self->body + (self->unit * self->curr), self->unit * len);
  self->curr += len;

  return RESULT_OK;
}

t_ion_result_code vector_get(t_ion_vector *self, void **value, size_t index) {
  if (self == NULL)
    return RESULT_ERROR;

  if (value == NULL)
    return RESULT_ERROR;

  if (index < 0)
    return RESULT_ERROR;

  if (index >= self->size)
    return RESULT_ERROR;

  *value = self->body + (self->unit * index);

  return RESULT_OK;
}

t_ion_result_code vector_set(t_ion_vector *self, void **value, size_t index) {
  if (self == NULL)
    return RESULT_ERROR;

  if (value == NULL)
    return RESULT_ERROR;

  if (index < 0)
    return RESULT_ERROR;

  if (index >= self->size)
    return RESULT_ERROR;

  memcpy(self->body + (self->unit * index), *value, self->unit);

  return RESULT_OK;
}

void *vector_consume(t_ion_vector *self) {
  void *result;

  result = self->body;
  free(self);

  return (result);
}

void vector_display(t_ion_vector *self) {
  printf("vector->[");
  printf(";unit=%lu;size=%lu;curr=%lu", self->unit, self->size, self->curr);
  printf(";body=[");
  for (size_t x = 0; x < (self->unit * self->size); x++) {
    printf("%02X ", ((uint8_t *)self->body)[x]);
  }
  printf("]");
  printf("]\n");
}
