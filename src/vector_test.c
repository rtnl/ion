#ifdef TEST
#include "ion.h"

Test(vector, vector_create) {
  t_ion_vector *vector;

  vector = vector_new(sizeof(int32_t));
  cr_expect(vector != NULL);

  cr_expect(vector->body != NULL);
  cr_expect(vector->unit == sizeof(int32_t));
  cr_expect(vector->size > 0);
  cr_expect(vector->curr_w == 0);
  cr_expect(vector->curr_r == 0);
  cr_expect(vector->curr_p == 0);

  free(vector_consume(vector));
}

Test(vector, vector_consume) {
  t_ion_vector *vector;
  void *data;

  vector = vector_new(sizeof(int32_t));
  cr_expect(vector != NULL);

  data = vector_consume(vector);
  cr_expect(data != NULL);

  free(data);
}

Test(vector, vector_write_loop) {
  t_ion_result_code result;
  t_ion_vector *vector;
  void *data;
  int32_t x;

  vector = vector_new(sizeof(int32_t));
  cr_expect(vector != NULL);

  for (x = 0; x < 1024; x++) {
    cr_expect(vector->curr_w == x);
    cr_expect(vector->curr_r == 0);
    cr_expect(vector->curr_p == 0);
    result = vector_write(vector, &x, 1);
    cr_expect(result == RESULT_OK);
    cr_expect(vector->curr_w == x + 1);
    cr_expect(vector->curr_r == 0);
    cr_expect(vector->curr_p == 0);
  }

  free(vector_consume(vector));
}

Test(vector, vector_write_array) {
  t_ion_result_code result;
  t_ion_vector *vector;
  int32_t *t = calloc(1000000, sizeof(int));
  int32_t *o = calloc(1000000, sizeof(int));
  int32_t x;

  vector = vector_new(sizeof(int32_t));
  cr_expect(vector != NULL);

  for (x = 0; x < 1000000; x++) {
    t[x] = x;
  }

  result = vector_write(vector, t, 1000000);
  cr_expect(result == RESULT_OK);
  cr_expect(vector->curr_w == 1000000);

  result = vector_read(vector, o, 1000000);
  cr_expect(result == RESULT_OK);
  cr_expect(vector->curr_r == 1000000);

  for (x = 0; x < 1000000; x++) {
    cr_expect(t[x] == o[x]);
  }

  free(vector_consume(vector));
  free(t);
  free(o);
}

Test(vector, vector_write_string) {
  t_ion_result_code result;
  t_ion_vector *vector;
  char *src;
  char dst[100] = {};

  src = "In the beginning God created the heaven and the earth.";

  vector = vector_new(sizeof(char));
  cr_expect(vector != NULL);

  result = vector_write(vector, src, strlen(src));
  cr_expect(result == RESULT_OK);

  result = vector_read(vector, dst, strlen(src));
  cr_expect(result == RESULT_OK);

  cr_expect(strcmp(src, dst) == 0);

  free(vector_consume(vector));
}

Test(vector, vector_peek) {
  t_ion_result_code result;
  t_ion_vector *vector;
  char *src;
  char dst[100] = {};

  src = "And the earth was without form, and void;";

  vector = vector_new(sizeof(char));
  cr_expect(vector != NULL);
  cr_expect(vector->curr_p == 0);

  result = vector_write(vector, src, strlen(src));
  cr_expect(result == RESULT_OK);
  cr_expect(vector->curr_w == strlen(src));

  result = vector_peek(vector, dst, strlen(src));
  cr_expect(result == RESULT_OK);
  cr_expect(vector->curr_r == 0);
  cr_expect(vector->curr_p == strlen(src));
  cr_expect(strcmp(src, dst) == 0);

  free(vector_consume(vector));
}

#endif
