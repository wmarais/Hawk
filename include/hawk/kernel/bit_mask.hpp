/*******************************************************************************
 * @file
 * @author      Wynand Marais
 * @copyright   (c) 2020 Wynand Marais, MIT License
 *
 * These functions are used to calculate bit masks for manipulating strongly
 * typed register fields. These functions are to aid the programmer and reduce
 * the likely hood of silly mistakes due to incorrect register masking etc. It
 * has been tested on gcc 10 to have no additional cost overhead to using bit
 * masks directly.
 ******************************************************************************/
#pragma once

#include "panic.hpp"

namespace hawk
{
  namespace kernel
  {
    /***************************************************************************
     * Create a bit mask with a single bit at the specified position.
     *
     * @param [in] bit  The bit to mask off.
     * @return          The created bit mask.
     **************************************************************************/
    template <typename type_t, typename index_t>
    type_t bit_mask(const index_t & bit)
    {
      /* Check that the specified bit is in bounds of the type. */
      HAWK_KERNEL_ASSERT(bit >= 0 &&
                         bit < static_cast<index_t>(sizeof(type_t) * 8 - 1),
                         panic_codes_t::register_bit_out_of_bounds);

      /* Compute the bit mask. */
      return type_t(1) << bit;
    }

    /***************************************************************************
     * Calculate a bitmask of a specific type that allows for bit manipulation
     * of values. The function was designed to be used by the kernel to
     * manipulate the bit fields of registers, but it can be used for normal
     * user space programming if the HAWK_KERNEL_ASSERT()s are stripped out. It
     * has been specifically designed to provide no performance or cost overhead
     * compared to C when compiled with O3. The arguments are set such that the
     * msb is passed in first, then the lsb to match documentation convention
     * for registers where ranges are specified as [msb:lsb].
     *
     * <b>Example 1</b>
     *
     * To mask off the four least-significant-bits of a 32bit unsigned integer:
     *
     * @code
     * val & bit_mask<unsigned int, int>(3, 0);
     * @endcode
     *
     * This is equivalent to C:
     *
     * @code
     * val & 0x0000000F;
     * @endcode
     *
     * <b>Example 2</b>
     *
     * To mask off the seven most-significant-bits of a 32bit unsigned integer:
     *
     * @code
     * val & bit_mask<unsigned int, int>(31, 25);
     * @endcode
     *
     * This is equivalent to C:
     *
     * @code
     * val & 0xFE000000;
     * @endcode
     *
     * @param[in] msb   The most-significant-bit (msb) of the mask. This is
     *                  inclusive, thus the msb bit will be part of the mask.
     * @param[in] lsb   The least-significant-bit (lsb) of the bit mask. This is
     *                  inclusive, thus the lsb bit will be part of the mask.
     * @return          The bit mask that will mask of the msb:lsb range.
     **************************************************************************/
    template <typename type_t, typename index_t>
    type_t bit_mask(index_t msb, const index_t & lsb)
    {
      /* Check that the msb is iside the type_t bit width. */
      HAWK_KERNEL_ASSERT(msb >= 0 &&
                         msb < static_cast<index_t>(sizeof(type_t) * 8 - 1),
                         panic_codes_t::register_bit_out_of_bounds);

      /* Check that the lsb is iside the type_t bit width. */
      HAWK_KERNEL_ASSERT(lsb >= 0 &&
                         lsb < static_cast<index_t>(sizeof(type_t) * 8 - 1),
                         panic_codes_t::register_bit_out_of_bounds);

      /* The mask being computed. */
      type_t mask = 1 << msb;

      /* Calculate the bit mask. */
      while(msb > lsb)
      {
        mask |= 1 << msb;
        --msb;
      }

      /* Return the computed mask. */
      return mask;
    }
  }
}
