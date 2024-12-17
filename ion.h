#ifndef ION_H
#define ION_H

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_ion_buffer {
  uint8_t *body;
  size_t size;
  size_t curr;
} t_ion_buffer;

extern const size_t SIZE_DEFAULT;

size_t min_sizet(size_t x, size_t y);
t_ion_buffer *ion_buffer_new();
void ion_buffer_free(t_ion_buffer *self);
void ion_buffer_write(t_ion_buffer *self, void *src, size_t len);
void ion_buffer_read(t_ion_buffer *self, void *dst, size_t len);
uint8_t *ion_buffer_consume(t_ion_buffer *self);

#endif
