#include "ion.h"

t_ion_buffer_state ion_buffer_state_new() {
  t_ion_buffer_state self;

  self.io_write_current = 0x00;
  self.io_write_current_start = 0;
  self.io_write_current_length = 0;

  return (self);
}
