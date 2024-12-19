#ifndef ION_H
#define ION_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

typedef enum e_ion_result_code {
    RESULT_OK,
    RESULT_ERROR,
} t_ion_result_code;

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
  uint8_t io_write_current_length;

  t_ion_object_kind io_read_current;
  size_t io_read_current_start;
  uint8_t io_read_current_length;
} t_ion_buffer_state;

typedef struct s_ion_buffer {
  uint8_t *body;
  size_t size;
  size_t curr;
  t_ion_buffer_state state;
} t_ion_buffer;

extern const size_t SIZE_DEFAULT;

// math
size_t min_sizet(size_t x, size_t y);
uint8_t min_u8(uint8_t x, uint8_t y);

// types
t_ion_object_kind ion_object_kind_from_index(uint8_t index);
uint8_t ion_object_kind_index(t_ion_object_kind kind);
size_t ion_object_kind_size(t_ion_object_kind kind);

// buffer
t_ion_buffer *ion_buffer_new();
void ion_buffer_free(t_ion_buffer *self);
t_ion_buffer *ion_buffer_clone(t_ion_buffer *self);
t_ion_result_code ion_buffer_seek(t_ion_buffer *self, uint8_t curr);
t_ion_result_code ion_buffer_write(t_ion_buffer *self, void *src, size_t len);
t_ion_result_code ion_buffer_read(t_ion_buffer *self, void *dst, size_t len);
uint8_t *ion_buffer_consume(t_ion_buffer *self);

// buffer_state
t_ion_buffer_state ion_buffer_state_new();

// buffer_io
t_ion_result_code ion_buffer_io_read_kind(t_ion_buffer *self, t_ion_object_kind *value);
t_ion_result_code ion_buffer_io_write_data(t_ion_buffer *self, t_ion_object_kind kind, void *src);
t_ion_result_code ion_buffer_io_read_kind(t_ion_buffer *self, t_ion_object_kind *value);
t_ion_result_code ion_buffer_io_write_u0(t_ion_buffer *self);
t_ion_result_code ion_buffer_io_write_u8(t_ion_buffer *self, uint8_t value);
t_ion_result_code ion_buffer_io_write_u16(t_ion_buffer *self, uint16_t value);
t_ion_result_code ion_buffer_io_write_u32(t_ion_buffer *self, uint32_t value);
t_ion_result_code ion_buffer_io_write_u64(t_ion_buffer *self, uint64_t value);
t_ion_result_code ion_buffer_io_write_arr(t_ion_buffer *self, t_ion_object_kind kind, void *value, uint8_t len);
t_ion_result_code ion_buffer_io_write_list_open(t_ion_buffer *self);
t_ion_result_code ion_buffer_io_write_list_close(t_ion_buffer *self);
t_ion_result_code ion_buffer_io_read_data(t_ion_buffer *self, t_ion_object_kind kind, void *dst);
t_ion_result_code ion_buffer_io_read_u0(t_ion_buffer *self);
t_ion_result_code ion_buffer_io_read_u8(t_ion_buffer *self, uint8_t *value);
t_ion_result_code ion_buffer_io_read_u16(t_ion_buffer *self, uint16_t *value);
t_ion_result_code ion_buffer_io_read_u32(t_ion_buffer *self, uint32_t *value);
t_ion_result_code ion_buffer_io_read_u64(t_ion_buffer *self, uint64_t *value);
t_ion_result_code ion_buffer_io_read_arr(t_ion_buffer *self, t_ion_object_kind kind, void *value, uint8_t limit);
t_ion_result_code ion_buffer_io_read_list_open(t_ion_buffer *self, uint8_t *len);
t_ion_result_code ion_buffer_io_read_list_close(t_ion_buffer *self);


#endif
