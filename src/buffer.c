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

void ion_buffer_write(t_ion_buffer *self, void *src, size_t len) {
  uint8_t *srcPtr;
  size_t x;
  size_t curr;
  size_t size;

  if (self == NULL)
    return;

  if (src == NULL)
    return;

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
}

void ion_buffer_read(t_ion_buffer *self, void *dst, size_t len) {
  uint8_t *dstPtr;
  size_t t;
  size_t x;

  if (self == NULL)
    return;

  if (dst == NULL)
    return;

  dstPtr = (uint8_t *)dst;

  t = min_sizet(self->curr, len);
  for (x = 0; x < t; x++) {
    dstPtr[x] = self->body[x];
  }
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
