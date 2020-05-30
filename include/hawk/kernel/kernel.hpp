/*******************************************************************************
 * @file
 * @author      Wynand Marais
 * @copyright   (c) 2020 Wynand Marais, MIT License
 ******************************************************************************/
#pragma once

#include "panic.hpp"
#include "scheduler.hpp"

#include <limits>

namespace hawk::kernel
{
  class kernel final
  {


    typedef hawk::uint32_t  crit_sec_counter_t;


    static crit_sec_counter_t crit_sec_call_counter;


    static void disable_interrupts();
    static void enable_interrupts();

  public:
    /***************************************************************************
     *
     **************************************************************************/
    inline static void enter_critical_section()
    {
      /* Check if the counter is going to overflow. */
      if(std::numeric_limits<crit_sec_counter_t>::max())
        panic(panic_codes_t::enter_crit_sec);

      /* Check if interrupts must be disabled. */
      if(!crit_sec_call_counter)
        disable_interrupts();

      /* Increment the number of times the function has been called. */
      ++crit_sec_call_counter;
    }

    /***************************************************************************
     *
     **************************************************************************/
    inline static void exit_critical_section()
    {
      /* Check if the counter will underflow. */
      if(crit_sec_call_counter == 0)
        panic(panic_codes_t::exit_crit_sec);

      /* Decrement the call counter. */
      --crit_sec_call_counter;

      /* Check if interrupts must be enabled. */
      if(crit_sec_call_counter == 0)
        enable_interrupts();
    }
  };
}
