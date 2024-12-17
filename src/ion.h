#ifndef ION_H
#define ION_H

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef enum e_ion_object_kind {
    U0,
    U8,
    U16,
    U32,
    U64,
    ARR,
    LIST,
} t_ion_object_kind;

typedef struct s_ion_buffer_state {
  t_ion_object_kind io_write_current;
  size_t io_write_current_start;
}   t_ion_buffer_state;

typedef struct s_ion_buffer {
  uint8_t *body;
  size_t size;
  size_t curr;
  t_ion_buffer_state state;
} t_ion_buffer;

extern const size_t SIZE_DEFAULT;

// math
size_t min_sizet(size_t x, size_t y);

// types
uint8_t ion_object_kind_index(t_ion_object_kind kind);
size_t ion_object_kind_size(t_ion_object_kind kind);

// buffer
t_ion_buffer *ion_buffer_new();
void ion_buffer_free(t_ion_buffer *self);
void ion_buffer_write(t_ion_buffer *self, void *src, size_t len);
void ion_buffer_read(t_ion_buffer *self, void *dst, size_t len);
uint8_t *ion_buffer_consume(t_ion_buffer *self);

// buffer_state
t_ion_buffer_state ion_buffer_state_new();

// buffer_io
void ion_buffer_io_write_kind(t_ion_buffer *self, t_ion_object_kind kind);
void ion_buffer_io_write_data(t_ion_buffer *self, t_ion_object_kind kind, void *src);
void ion_buffer_io_write_u0(t_ion_buffer *self);
void ion_buffer_io_write_u8(t_ion_buffer *self, uint8_t value);
void ion_buffer_io_write_u16(t_ion_buffer *self, uint16_t value);
void ion_buffer_io_write_u32(t_ion_buffer *self, uint32_t value);
void ion_buffer_io_write_u64(t_ion_buffer *self, uint64_t value);
void ion_buffer_io_write_arr(t_ion_buffer *self, t_ion_object_kind kind, void *value, uint8_t len);

#endif
