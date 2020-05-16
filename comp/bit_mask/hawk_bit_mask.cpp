#include "../../include/hawk/kernel/bit_mask.hpp"

unsigned int is_set(unsigned int val)
{
  return val & hawk::kernel::bit_mask<unsigned int, int>(1);
}

//unsigned int is_set2(unsigned int val)
//{
//  return val & hawk::kernel::register_t<unsigned int, int>::bit_mask(1);
//}

unsigned int is_sub_val(unsigned int val)
{
  return val & hawk::kernel::bit_mask<unsigned int, int>(3, 0);
}
