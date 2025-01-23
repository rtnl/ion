#ifdef TEST
#include "ion.h"

Test(buffer, buffer_create) {
  t_ion_buffer *buffer;

  buffer = ion_buffer_new();
  cr_expect(buffer != NULL);
  cr_expect(buffer->body != NULL);
  cr_expect(buffer->state != NULL);

  ion_buffer_free(buffer);
}

Test(buffer, buffer_free) {
  t_ion_buffer *buffer;

  buffer = ion_buffer_new();
  cr_expect(buffer != NULL);

  ion_buffer_free(buffer);
}

#endif
