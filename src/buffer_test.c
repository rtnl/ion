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

#endif
