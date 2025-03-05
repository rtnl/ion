#include "ion.h"

const size_t SIZE_DEFAULT = 1024;

t_ion_vector *vector_new(size_t unit) {
  t_ion_vector *self;

  self = (t_ion_vector *)malloc(sizeof(t_ion_vector));
  self->unit = unit;
  self->size = SIZE_DEFAULT;
  self->curr_w = 0;
  self->curr_r = 0;
  self->curr_p = 0;
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
  other->curr_w = self->curr_w;
  other->curr_r = self->curr_r;
  other->curr_p = self->curr_p;

  memcpy(other->body, self->body, self->unit * self->size);

  return (other);
}

t_ion_result_code vector_seek_read(t_ion_vector *self, size_t index) {
  if (self == NULL)
    return RESULT_NULL;

  if (index < 0)
    return RESULT_ERROR;

  if (index >= self->size)
    return RESULT_ERROR;

  self->curr_r = index;

  return RESULT_OK;
}

t_ion_result_code vector_seek_write(t_ion_vector *self, size_t index) {
  if (self == NULL)
    return RESULT_NULL;

  if (index < 0)
    return RESULT_ERROR;

  if (index >= self->size)
    return RESULT_ERROR;

  self->curr_w = index;

  return RESULT_OK;
}

t_ion_result_code vector_seek_peek(t_ion_vector *self, size_t index) {
  if (self == NULL)
    return RESULT_NULL;

  if (index < 0)
    return RESULT_ERROR;

  if (index >= self->size)
    return RESULT_ERROR;

  self->curr_p = index;

  return RESULT_OK;
}

t_ion_result_code vector_seek_relative_read(t_ion_vector *self, int64_t diff) {
  if (self == NULL)
    return RESULT_NULL;

  return vector_seek_read(self, (size_t)((int64_t)self->curr_r + diff));
}

t_ion_result_code vector_seek_relative_write(t_ion_vector *self, int64_t diff) {
  if (self == NULL)
    return RESULT_NULL;

  return vector_seek_write(self, (size_t)((int64_t)self->curr_w + diff));
}

t_ion_result_code vector_seek_relative_peek(t_ion_vector *self, int64_t diff) {
  if (self == NULL)
    return RESULT_NULL;

  return vector_seek_peek(self, (size_t)((int64_t)self->curr_p + diff));
}

t_ion_result_code vector_extend(t_ion_vector *self) {
  void *body;
  size_t size;

  if (self == NULL)
    return RESULT_NULL;

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
    return RESULT_NULL;

  if (src == NULL)
    return RESULT_NULL;

  len_total = self->unit * len;
  curr_total = self->unit * self->curr_w;

  while ((self->curr_w + len) >= self->size) {
    result = vector_extend(self);
    if (result != RESULT_OK)
      return result;
  }

  memcpy(self->body + curr_total, src, len_total);
  self->curr_w += len;

  return RESULT_OK;
}

t_ion_result_code vector_read(t_ion_vector *self, void *dst, size_t len) {
  if (self == NULL)
    return RESULT_NULL;

  if (dst == NULL)
    return RESULT_NULL;

  if (len > (self->size - self->curr_r)) {
    return RESULT_ERROR;
  }

  memcpy(dst, self->body + (self->unit * self->curr_r), self->unit * len);
  self->curr_r += len;
  self->curr_p = self->curr_r;

  return RESULT_OK;
}

t_ion_result_code vector_peek(t_ion_vector *self, void *dst, size_t len) {
  if (self == NULL)
    return RESULT_NULL;

  if (dst == NULL)
    return RESULT_NULL;

  if (len > (self->size - self->curr_p)) {
    return RESULT_ERROR;
  }

  memcpy(dst, self->body + (self->unit * self->curr_p), self->unit * len);
  self->curr_p += len;

  return RESULT_OK;
}

t_ion_result_code vector_get(t_ion_vector *self, void **value, size_t index) {
  if (self == NULL)
    return RESULT_NULL;

  if (value == NULL)
    return RESULT_NULL;

  if (index < 0)
    return RESULT_ERROR;

  if (index >= self->size)
    return RESULT_ERROR;

  *value = self->body + (self->unit * index);

  return RESULT_OK;
}

t_ion_result_code vector_set(t_ion_vector *self, void **value, size_t index) {
  if (self == NULL)
    return RESULT_NULL;

  if (value == NULL)
    return RESULT_NULL;

  if (index < 0)
    return RESULT_ERROR;

  if (index >= self->size)
    return RESULT_ERROR;

  memcpy(self->body + (self->unit * index), *value, self->unit);

  return RESULT_OK;
}

t_ion_result_code vector_reduce(t_ion_vector *self) {
  size_t size_min;
  size_t size_max;
  size_t len_min;
  size_t len_max;
  void *body;

  if (self == NULL) {
    return RESULT_NULL;
  }

  size_min = min_sizet(self->curr_w, min_sizet(self->curr_r, self->curr_p));
  size_max = max_sizet(self->curr_w, max_sizet(self->curr_r, self->curr_p));
  len_min = size_min * self->unit;
  len_max = size_max * self->unit;

  // Todo: realloc
  body = calloc(self->size, self->unit);
  memcpy(body, self->body + len_min, len_max);
  free(self->body);
  self->body = body;
  self->curr_w -= size_min;
  self->curr_r -= size_min;
  self->curr_p -= size_min;

  return RESULT_OK;
}

void *vector_consume(t_ion_vector *self) {
  void *result;

  if (self == NULL)
    return NULL;

  result = self->body;
  free(self);

  return (result);
}

void vector_display(t_ion_vector *self) {
  if (self == NULL) {
    printf("vector->null\n");
    return;
  }

  printf("vector->[");
  printf(";unit=%lu;size=%lu;curr_r=%lu;curr_w=%lu", self->unit, self->size,
         self->curr_r, self->curr_w);
  printf(";body=[");
  for (size_t x = 0; x < (self->unit * self->size); x++) {
    printf("%02X ", ((uint8_t *)self->body)[x]);
  }
  printf("]");
  printf("]\n");
}
