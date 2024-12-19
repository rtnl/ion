#include "ion.h"

const size_t SIZE_DEFAULT = 1024;

t_ion_buffer *ion_buffer_new() {
  t_ion_buffer *self;

  self = malloc(sizeof(t_ion_buffer));
  self->state = ion_buffer_state_new();
  self->body = malloc(sizeof(uint8_t) * SIZE_DEFAULT);
  self->size = SIZE_DEFAULT;
  self->curr = 0;

  return (self);
}

void ion_buffer_free(t_ion_buffer *self) {
  if (self == NULL)
    return;

  free(self->body);
  free(self);
}

t_ion_buffer *ion_buffer_clone(t_ion_buffer *self) {
  t_ion_buffer *other;

  if (self == NULL)
    return (NULL);

  other = malloc(sizeof(t_ion_buffer));
  other->size = self->size;
  other->curr = self->curr;
  other->body = malloc(self->size);

  memcpy(other->body, self->body, self->curr);

  return (other);
}

t_ion_result_code ion_buffer_seek(t_ion_buffer *self, uint8_t curr) {
  if (self == NULL)
    return RESULT_ERROR;

  if (curr < 0)
    return RESULT_ERROR;

  if (curr >= self->size)
    return RESULT_ERROR;

  self->curr = curr;

  return RESULT_OK;
}

t_ion_result_code ion_buffer_write(t_ion_buffer *self, void *src, size_t len) {
  uint8_t *srcPtr;
  size_t x;
  size_t curr;
  size_t size;

  if (self == NULL)
    return RESULT_ERROR;

  if (src == NULL)
    return RESULT_ERROR;

  srcPtr = (uint8_t *)src;

  curr = self->curr;
  size = self->size;
  if (curr + len > size) {
    size += SIZE_DEFAULT;
    self->body = realloc(self->body, sizeof(uint8_t) * size);
    self->size = size;
  }
  for (x = 0; x < len; x++) {
    self->body[curr + x] = srcPtr[x];
  }
  self->curr += len;

  return RESULT_OK;
}

t_ion_result_code ion_buffer_read(t_ion_buffer *self, void *dst, size_t len) {
  uint8_t *dstPtr;
  size_t t;
  size_t x;

  if (self == NULL)
    return RESULT_ERROR;

  if (dst == NULL)
    return RESULT_ERROR;

  if ((self->curr + len) >= self->size) {
    return RESULT_ERROR;
  }

  dstPtr = (uint8_t *)dst;

  t = min_sizet(self->size - self->curr, len);
  for (x = 0; x < t; x++) {
    dstPtr[x] = self->body[self->curr + x];
  }

  self->curr += t;

  return RESULT_OK;
}

uint8_t *ion_buffer_consume(t_ion_buffer *self) {
  uint8_t *result;
  size_t x;
  size_t t;

  if (self == NULL)
    return (NULL);

  result = malloc(self->curr);
  t = self->curr;
  for (x = 0; x < t; x++) {
    result[x] = self->body[x];
  }

  ion_buffer_free(self);
  return (result);
}
