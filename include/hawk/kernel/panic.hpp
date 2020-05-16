/*******************************************************************************
 * @file
 * @author      Wynand Marais
 * @copyright   (c) 2020 Wynand Marais, MIT License
 ******************************************************************************/
#pragma once

#include "../types.hpp"

/* Only enable kernel asserts for debug mode. */
#ifdef HAWK_KERNEL_DEBUG
  #define HAWK_KERNEL_ASSERT(test, code)  if(!(test)) hawk::kernel::panic(code)
#else
  #define HAWK_KERNEL_ASSERT(test, code)
#endif /* HAWK_KERNEL_DEBUG */

namespace hawk::kernel
{
  enum class panic_codes_t : hawk::uint32_t
  {
    /** The enter_critical_section() too many times without a corresponding
     * exit_critical_section() and the crit_sec_call_counter counter has
     * overflown. This means that the counter will wrap around to 0 calls and
     * CPU interrupts may be either be completely disabled (i.e. if
     * exit_critical_section()) is called immediately after the last call to
     * enter_critical_section()), or interrupts will be enabled at the wrong
     * time thereby compromising kernel integrity. */
    enter_crit_sec,
    exit_crit_sec,


    mask_bit_out_of_bounds
  };


  void panic(panic_codes_t pc) { (void)pc; }
}
