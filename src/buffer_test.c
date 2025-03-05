#ifdef TEST
#include "ion.h"

Test(buffer, buffer_create) {
  t_ion_buffer *buffer;

  buffer = ion_buffer_new();
  cr_expect(buffer != NULL);
  cr_expect(buffer->body != NULL);
  cr_expect(buffer->state_w != NULL);
  cr_expect(buffer->state_r != NULL);
  cr_expect(buffer->state_p != NULL);

  ion_buffer_free(buffer);
}

Test(buffer, buffer_free) {
  t_ion_buffer *buffer;

  buffer = ion_buffer_new();
  cr_expect(buffer != NULL);

  ion_buffer_free(buffer);
}

Test(buffer, buffer_io_u0) {
  t_ion_result_code result;
  t_ion_buffer *buffer;

  buffer = ion_buffer_new();
  cr_expect(buffer != NULL);

  result = ion_buffer_io_write_u0(buffer);
  cr_expect(result == RESULT_OK);

  result = ion_buffer_io_read_u0(buffer);
  cr_expect(result == RESULT_OK);

  ion_buffer_free(buffer);
}

Test(buffer, buffer_io_u8) {
  t_ion_result_code result;
  t_ion_buffer *buffer;
  uint8_t value;

  buffer = ion_buffer_new();
  cr_expect(buffer != NULL);

  for (uint8_t x = 0; x < UINT8_MAX; x++) {
    value = x;

    result = ion_buffer_io_write_u8(buffer, value);
    cr_expect(result == RESULT_OK);
  }

  for (uint8_t x = 0; x < UINT8_MAX; x++) {
    value = 0;

    result = ion_buffer_io_read_u8(buffer, &value);
    cr_expect(result == RESULT_OK);

    cr_expect(value == x);
  }

  ion_buffer_free(buffer);
}

Test(buffer, buffer_io_u16) {
  t_ion_result_code result;
  t_ion_buffer *buffer;
  uint16_t value;

  buffer = ion_buffer_new();
  cr_expect(buffer != NULL);

  for (uint16_t x = 0; x < UINT16_MAX; x++) {
    value = x;

    result = ion_buffer_io_write_u16(buffer, value);
    cr_expect(result == RESULT_OK);
  }

  for (uint16_t x = 0; x < UINT16_MAX; x++) {
    value = 0;

    result = ion_buffer_io_read_u16(buffer, &value);
    cr_expect(result == RESULT_OK);

    cr_expect(value == x);
  }

  ion_buffer_free(buffer);
}

Test(buffer, buffer_io_u32) {
  t_ion_result_code result;
  t_ion_buffer *buffer;
  uint32_t value;

  buffer = ion_buffer_new();
  cr_expect(buffer != NULL);

  for (uint32_t x = 0; x < UINT16_MAX; x++) {
    value = x;

    result = ion_buffer_io_write_u32(buffer, value);
    cr_expect(result == RESULT_OK);
  }

  for (uint32_t x = 0; x < UINT16_MAX; x++) {
    value = 0;

    result = ion_buffer_io_read_u32(buffer, &value);
    cr_expect(result == RESULT_OK);

    cr_expect(value == x);
  }

  ion_buffer_free(buffer);
}

Test(buffer, buffer_io_u64) {
  t_ion_result_code result;
  t_ion_buffer *buffer;
  uint64_t value;

  buffer = ion_buffer_new();
  cr_expect(buffer != NULL);

  for (uint64_t x = 0; x < UINT16_MAX; x++) {
    value = x;

    result = ion_buffer_io_write_u64(buffer, value);
    cr_expect(result == RESULT_OK);
  }

  for (uint64_t x = 0; x < UINT16_MAX; x++) {
    value = 0;

    result = ion_buffer_io_read_u64(buffer, &value);
    cr_expect(result == RESULT_OK);

    cr_expect(value == x);
  }

  ion_buffer_free(buffer);
}

Test(buffer, buffer_io_arr_check) {
  t_ion_result_code result;
  t_ion_buffer *buffer;
  t_ion_object_kind kind;
  uint8_t len;

  buffer = ion_buffer_new();
  cr_expect(buffer != NULL);

  result = ion_buffer_io_write_arr_open(buffer, U32);
  cr_expect(result == RESULT_OK);

  for (uint8_t x = 0; x < 100; x++) {
    ion_buffer_io_write_u32(buffer, 1337);
    cr_expect(result == RESULT_OK);
  }

  result = ion_buffer_io_write_arr_close(buffer);
  cr_expect(result == RESULT_OK);

  result = ion_buffer_io_read_arr_open(buffer, &kind, &len);
  cr_expect(result == RESULT_OK);

  result = ion_buffer_io_read_arr_check(buffer, kind, 99);
  cr_expect(result == RESULT_OK);

  result = ion_buffer_io_read_arr_check(buffer, kind, 100);
  cr_expect(result == RESULT_OK);

  result = ion_buffer_io_read_arr_check(buffer, kind, 101);
  cr_expect(result == RESULT_IO_TOO_SMALL);

  result = ion_buffer_io_read_arr_close(buffer);
  cr_expect(result == RESULT_OK);

  result = ion_buffer_io_peek_arr_open(buffer, &kind, &len);
  cr_expect(result == RESULT_OK);

  result = ion_buffer_io_peek_arr_check(buffer, kind, 99);
  cr_expect(result == RESULT_OK);

  result = ion_buffer_io_peek_arr_check(buffer, kind, 100);
  cr_expect(result == RESULT_OK);

  result = ion_buffer_io_peek_arr_check(buffer, kind, 101);
  cr_expect(result == RESULT_IO_TOO_SMALL);

  result = ion_buffer_io_peek_arr_close(buffer);
  cr_expect(result == RESULT_OK);

  ion_buffer_free(buffer);
}

typedef struct s_ion_buffer_test {
  char *s;
  uint64_t t;
} t_buffer_test;

Test(buffer, buffer_encode_struct) {
  t_buffer_test object;
  t_buffer_test other;
  t_ion_buffer *buffer;
  t_ion_result_code result;

  object.s = "hello world";
  object.t = 1337;

  buffer = ion_buffer_new();
  cr_expect(buffer != NULL);

  result = ion_buffer_io_write_arr_open(buffer, U8);
  cr_expect(result == RESULT_OK);

  for (uint8_t x = 0; object.s[x]; x++) {
    result = ion_buffer_io_write_u8(buffer, object.s[x]);
    cr_expect(result == RESULT_OK);
  }

  result = ion_buffer_io_write_arr_close(buffer);
  cr_expect(result == RESULT_OK);

  result = ion_buffer_io_write_u64(buffer, object.t);
  cr_expect(result == RESULT_OK);

  size_t len_expected = 23;
  uint8_t data_expected[23] = {
      0x05,                                                          // type ARR
      0x01,                                                          // item U8
      0x0B,                                                          // len  11
      'h',  'e',  'l',  'l',  'o',  ' ',  'w',  'o',  'r', 'l', 'd', // text

      0x04,                                           // type U64
      0x39, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // value 1337
  };

  size_t len = 0;
  uint8_t *data = ion_buffer_consume(ion_buffer_clone(buffer), &len);

  cr_expect(len == len_expected);
  cr_expect(memcmp(data, data_expected, 23) == 0);
  free(data);

  ion_buffer_free(buffer);
}

Test(buffer, buffer_encode_nested) {
  t_ion_buffer *buffer;
  t_ion_result_code result;

  buffer = ion_buffer_new();
  cr_expect(buffer != NULL);

  const uint32_t t = 1024;

  for (uint32_t x = 0; x < t; x++) {
    result = ion_buffer_io_write_list_open(buffer);
    cr_expect(result == RESULT_OK);

    result = ion_buffer_io_write_arr_open(buffer, U8);
    cr_expect(result == RESULT_OK);

    result = ion_buffer_io_write_u8(buffer, 'X');
    cr_expect(result == RESULT_OK);

    result = ion_buffer_io_write_u8(buffer, 'Y');
    cr_expect(result == RESULT_OK);

    result = ion_buffer_io_write_u8(buffer, 'Z');
    cr_expect(result == RESULT_OK);

    result = ion_buffer_io_write_arr_close(buffer);
    cr_expect(result == RESULT_OK);

    result = ion_buffer_io_write_u0(buffer);
    cr_expect(result == RESULT_OK);

    result = ion_buffer_io_write_u8(buffer, UINT8_MAX);
    cr_expect(result == RESULT_OK);

    result = ion_buffer_io_write_u16(buffer, UINT16_MAX);
    cr_expect(result == RESULT_OK);

    result = ion_buffer_io_write_u32(buffer, UINT32_MAX);
    cr_expect(result == RESULT_OK);

    result = ion_buffer_io_write_u64(buffer, UINT64_MAX);
    cr_expect(result == RESULT_OK);
  }

  for (uint32_t x = 0; x < t; x++) {
    result = ion_buffer_io_write_list_close(buffer);
    cr_expect(result == RESULT_OK);
  }

  for (uint32_t x = 0; x < t; x++) {
    uint8_t len = 0;
    result = ion_buffer_io_read_list_open(buffer, &len);
    cr_expect(result == RESULT_OK);

    if (x == (t - 1)) {
      cr_expect(len == 6);
    } else {
      cr_expect(len == 7);
    }

    t_ion_object_kind item = 0;
    result = ion_buffer_io_read_arr_open(buffer, &item, &len);
    cr_expect(result == RESULT_OK);
    cr_expect(item == U8);
    cr_expect(len == 3);

    uint8_t value_u8 = 0;
    uint16_t value_u16 = 0;
    uint32_t value_u32 = 0;
    uint64_t value_u64 = 0;

    result = ion_buffer_io_read_u8(buffer, &value_u8);
    cr_expect(result == RESULT_OK);
    cr_expect(value_u8 == 'X');

    result = ion_buffer_io_read_u8(buffer, &value_u8);
    cr_expect(result == RESULT_OK);
    cr_expect(value_u8 == 'Y');

    result = ion_buffer_io_read_u8(buffer, &value_u8);
    cr_expect(result == RESULT_OK);
    cr_expect(value_u8 == 'Z');

    result = ion_buffer_io_read_arr_close(buffer);
    cr_expect(result == RESULT_OK);

    result = ion_buffer_io_read_u0(buffer);
    cr_expect(result == RESULT_OK);

    result = ion_buffer_io_read_u8(buffer, &value_u8);
    cr_expect(result == RESULT_OK);
    cr_expect(value_u8 == UINT8_MAX);

    result = ion_buffer_io_read_u16(buffer, &value_u16);
    cr_expect(result == RESULT_OK);
    cr_expect(value_u16 == UINT16_MAX);

    result = ion_buffer_io_read_u32(buffer, &value_u32);
    cr_expect(result == RESULT_OK);
    cr_expect(value_u32 == UINT32_MAX);

    result = ion_buffer_io_read_u64(buffer, &value_u64);
    cr_expect(result == RESULT_OK);
    cr_expect(value_u64 == UINT64_MAX);
  }

  for (uint32_t x = 0; x < t; x++) {
    result = ion_buffer_io_read_list_close(buffer);
    cr_expect(result == RESULT_OK);
  }

  cr_expect(buffer->body->curr_r == buffer->body->curr_w);

  ion_buffer_free(buffer);
}

#endif
