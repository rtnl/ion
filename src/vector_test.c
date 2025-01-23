#ifdef TEST
#include "ion.h"

Test(vector, vector_create) {
  t_ion_vector *vector;

  vector = vector_new(sizeof(int32_t));
  cr_expect(vector != NULL);

  cr_expect(vector->body != NULL);
  cr_expect(vector->unit == sizeof(int32_t));
  cr_expect(vector->size > 0);
  cr_expect(vector->curr_r == 0);
  cr_expect(vector->curr_w == 0);

  // this should fail
  cr_expect(1 == 0);
}

#endif
