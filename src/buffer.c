#include "ion.h"

t_ion_buffer *ion_buffer_new() {
  t_ion_buffer *self;

  self = malloc(sizeof(t_ion_buffer));
  self->body = vector_new(sizeof(uint8_t));
  self->state = ion_buffer_state_new();

  return (self);
}

void ion_buffer_free(t_ion_buffer *self) {
  if (self == NULL)
    return;

  ion_buffer_state_free(self->state);
  free(vector_consume(self->body));
  free(self);
}

t_ion_buffer *ion_buffer_clone(t_ion_buffer *self) {
  t_ion_buffer *other;

  if (self == NULL)
    return (NULL);

  other = malloc(sizeof(t_ion_buffer));
  other->body = vector_clone(self->body);
  other->state = ion_buffer_state_clone(self->state);

  return (other);
}

t_ion_result_code ion_buffer_seek_read(t_ion_buffer *self, uint8_t curr) {
  if (self == NULL)
    return RESULT_ERROR;

  return vector_seek_read(self->body, (size_t)curr);
}

t_ion_result_code ion_buffer_seek_write(t_ion_buffer *self, uint8_t curr) {
  if (self == NULL)
    return RESULT_ERROR;

  return vector_seek_write(self->body, (size_t)curr);
}

t_ion_result_code ion_buffer_write(t_ion_buffer *self, void *src, size_t len) {
  if (self == NULL)
    return RESULT_ERROR;

  return vector_write(self->body, src, len);
}

t_ion_result_code ion_buffer_read(t_ion_buffer *self, void *dst, size_t len) {
  if (self == NULL)
    return RESULT_ERROR;

  return vector_read(self->body, dst, len);
}

t_ion_result_code ion_buffer_reduce(t_ion_buffer *self) {
  t_ion_result_code result;

  if (self == NULL) {
    return RESULT_ERROR;
  }

  result = vector_reduce(self->body);
  if (result != RESULT_OK) {
    return result;
  }

  return RESULT_OK;
}

uint8_t *ion_buffer_consume(t_ion_buffer *self, size_t *len) {
  uint8_t *result;

  if (self == NULL)
    return (NULL);

  *len = self->body->unit * self->body->curr_w;

  result = vector_consume(vector_clone(self->body));

  ion_buffer_free(self);

  return (result);
}
