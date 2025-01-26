#ifndef ION_H
#define ION_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#ifdef TEST
#include <criterion/criterion.h>
#endif

extern const size_t SIZE_DEFAULT;

typedef struct s_ion_vector {
    void *body;
    size_t unit;
    size_t size;
    size_t curr_r;
    size_t curr_w;
    size_t curr_p;
} t_ion_vector;

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

typedef struct s_ion_buffer_state_io_entry {
    t_ion_object_kind kind;
    t_ion_object_kind item;
    size_t start;
    uint8_t length;
} t_ion_buffer_state_io_entry;

typedef struct s_ion_buffer_state {
    t_ion_vector *entry_list;
    int32_t entry_level;
} t_ion_buffer_state;

typedef struct s_ion_buffer {
    t_ion_vector *body;
    t_ion_buffer_state *state;
} t_ion_buffer;

// math
size_t min_sizet(size_t x, size_t y);
uint8_t min_u8(uint8_t x, uint8_t y);

// types
t_ion_object_kind ion_object_kind_from_index(uint8_t index);
uint8_t ion_object_kind_index(t_ion_object_kind kind);
size_t ion_object_kind_size(t_ion_object_kind kind);

// vector
t_ion_vector *vector_new(size_t unit);
t_ion_vector *vector_clone(t_ion_vector *self);
t_ion_result_code vector_seek_read(t_ion_vector *self, size_t index);
t_ion_result_code vector_seek_write(t_ion_vector *self, size_t index);
t_ion_result_code vector_seek_relative_read(t_ion_vector *self, int64_t diff);
t_ion_result_code vector_seek_relative_write(t_ion_vector *self, int64_t diff);
t_ion_result_code vector_extend(t_ion_vector *self);
t_ion_result_code vector_write(t_ion_vector *self, void *src, size_t len);
t_ion_result_code vector_read(t_ion_vector *self, void *dst, size_t len);
t_ion_result_code vector_peek(t_ion_vector *self, void *dst, size_t len);
t_ion_result_code vector_get(t_ion_vector *self, void **value, size_t index);
t_ion_result_code vector_set(t_ion_vector *self, void **value, size_t index);
t_ion_result_code vector_reduce(t_ion_vector *self);
void *vector_consume(t_ion_vector *self);
void vector_display(t_ion_vector *self);

// buffer
t_ion_buffer *ion_buffer_new();
void ion_buffer_free(t_ion_buffer *self);
t_ion_buffer *ion_buffer_clone(t_ion_buffer *self);
t_ion_result_code ion_buffer_seek_read(t_ion_buffer *self, uint8_t curr);
t_ion_result_code ion_buffer_seek_write(t_ion_buffer *self, uint8_t curr);
t_ion_result_code ion_buffer_write(t_ion_buffer *self, void *src, size_t len);
t_ion_result_code ion_buffer_read(t_ion_buffer *self, void *dst, size_t len);
t_ion_result_code ion_buffer_reduce(t_ion_buffer *self);
uint8_t *ion_buffer_consume(t_ion_buffer *self, size_t *len);

// buffer_state
t_ion_buffer_state *ion_buffer_state_new();
void ion_buffer_state_free(t_ion_buffer_state *self);
t_ion_buffer_state *ion_buffer_state_clone(t_ion_buffer_state *self);
t_ion_result_code ion_buffer_state_io_write_increment(t_ion_buffer *self, t_ion_object_kind kind);
t_ion_result_code ion_buffer_state_io_read_increment(t_ion_buffer *self, t_ion_object_kind kind);
t_ion_buffer_state_io_entry *ion_buffer_state_io_entry_new(t_ion_object_kind kind, t_ion_object_kind item, size_t start);
void ion_buffer_state_io_entry_free(t_ion_buffer_state_io_entry *self);
t_ion_result_code ion_buffer_state_io_write_open(t_ion_buffer *self, t_ion_object_kind kind, t_ion_object_kind item);
t_ion_result_code ion_buffer_state_io_write_close(t_ion_buffer *self);
t_ion_result_code ion_buffer_state_io_read_open(t_ion_buffer *self, t_ion_object_kind kind, t_ion_object_kind *kind_item, uint8_t *len);
t_ion_result_code ion_buffer_state_io_read_close(t_ion_buffer *self);

// buffer_io
t_ion_result_code ion_buffer_io_write_kind(t_ion_buffer *self, t_ion_object_kind value);
t_ion_result_code ion_buffer_io_write_data(t_ion_buffer *self, t_ion_object_kind kind, void *src);
t_ion_result_code ion_buffer_io_read_kind(t_ion_buffer *self, t_ion_object_kind *value);
t_ion_result_code ion_buffer_io_check_kind(t_ion_buffer *self, t_ion_object_kind expected);
t_ion_result_code ion_buffer_io_write_u0(t_ion_buffer *self);
t_ion_result_code ion_buffer_io_write_u8(t_ion_buffer *self, uint8_t value);
t_ion_result_code ion_buffer_io_write_u16(t_ion_buffer *self, uint16_t value);
t_ion_result_code ion_buffer_io_write_u32(t_ion_buffer *self, uint32_t value);
t_ion_result_code ion_buffer_io_write_u64(t_ion_buffer *self, uint64_t value);
t_ion_result_code ion_buffer_io_write_arr_open(t_ion_buffer *self, t_ion_object_kind kind);
t_ion_result_code ion_buffer_io_write_arr_close(t_ion_buffer *self);
t_ion_result_code ion_buffer_io_write_list_open(t_ion_buffer *self);
t_ion_result_code ion_buffer_io_write_list_close(t_ion_buffer *self);
t_ion_result_code ion_buffer_io_read_data(t_ion_buffer *self, t_ion_object_kind kind, void *dst);
t_ion_result_code ion_buffer_io_read_u0(t_ion_buffer *self);
t_ion_result_code ion_buffer_io_read_u8(t_ion_buffer *self, uint8_t *value);
t_ion_result_code ion_buffer_io_read_u16(t_ion_buffer *self, uint16_t *value);
t_ion_result_code ion_buffer_io_read_u32(t_ion_buffer *self, uint32_t *value);
t_ion_result_code ion_buffer_io_read_u64(t_ion_buffer *self, uint64_t *value);
t_ion_result_code ion_buffer_io_read_arr_open(t_ion_buffer *self, t_ion_object_kind *kind, uint8_t *len);
t_ion_result_code ion_buffer_io_read_arr_close(t_ion_buffer *self);
t_ion_result_code ion_buffer_io_read_list_open(t_ion_buffer *self, uint8_t *len);
t_ion_result_code ion_buffer_io_read_list_close(t_ion_buffer *self);

#endif
