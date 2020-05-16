/*******************************************************************************
 * @file
 * @author      Wynand Marais
 * @copyright   (c) 2020 Wynand Marais, MIT License
 ******************************************************************************/
#pragma once

#include "bit_mask.hpp"

namespace hawk::kernel
{
  /*****************************************************************************
   * This class is used to manipulate memory mapped registers such as those
   * used on the CPU and attached peripherals. The class requires two template
   * parameters to be specified:
   *
   *  -<b>type_t</b>  Specifies the data type of the register. Commonly
   *                  registers tend to be 8, 16, 32, 64 bits in size. It is
   *                  best to use guarenteed type sizes such as uint8_t,
   *                  uint16_t, uint32_t, uint64_t etc.
   *
   *  -<b>index_t</b> Specify the data type to used for the bit indexes.
   *                  Basically this can be any type that can hold values
   *                  greater than  the maximum width of the register. It is
   *                  best to use fast types here that guarentee the minimum
   *                  sizes required. I.e. something like uint_fast8_t will
   *                  work well here.
   ****************************************************************************/
  template <typename type_t, typename index_t> class register_t
  {
    /** The physical address of the register on the CPU. It is the same type
     * as the register type which allows it to be dereferenced and manipulated
     * easily. */
    type_t volatile * addr_v;

  public:

    /***************************************************************************
     * Create a new register object pointing to the specified address.
     *
     * @param[in] addr  The address in memory where the register has been
     *                  mapped too.
     **************************************************************************/
    register_t(type_t * addr) : addr_v(addr) {}

    /***************************************************************************
     * Check if the bit at the specified location in the register is "1".
     *
     * @param[in] bit The bit in the register to check.
     * @return        If the bit is "1" the function returns true, otherwise if
     *                the bit is "0" the function returns false.
     **************************************************************************/
    bool is_set(const index_t & bit) const
    {
      return (*addr_v) & bit_mask<type_t, index_t>(bit);
    }

    /***************************************************************************
     * Check if the bit at the specified location in the register is "0".
     *
     * @param[in] bit The bit in the register to check.
     * @return        If the bit is "0" the function returns true, otherwise if
     *                the bit is "1" the function returns false.
     **************************************************************************/
    bool is_clear(const index_t & bit) const
    {
      return !((*addr_v) & bit_mask<type_t, index_t>(bit));
    }

    /***************************************************************************
     * Read the value of a field inside the register at the position given by
     * the range [msb:lsb] inclusively. For example, consider the register given
     * by:
     *
     *  | FIELD | BITS  |
     *  |:-----:|:-----:|
     *  | VAL2  | 31:25 |
     *  | VAL1  | 24:16 |
     *  | VAL0  | 15:0  |
     *
     *  The value of the fields can be read by calling:
     *
     * @code
     * // Create a 32 bit register object pointing to memory location
     * // 0x00AABBCC (i.e. the address of a memory mapped 32bit register).
     * register_t<uint32_t, int> test_reg(0x00AABBCC);
     *
     * // Read VAL0.
     * uint32_t val0 = test_reg.set_value(15, 0);
     *
     * // Read VAL1
     * uint32_t val1 = test_reg.set_value(24, 16);
     *
     * // Read VAL2.
     * uint32_t val2 = test_reg.set_value(31, 25);
     * @endcode
     *
     * @param[in] msb The bit position in the register where the
     *                most-significant-bit of the value range is located.
     *
     * @param[in] lsb The bit position in the register where the
     *                least-significant-bit of the value range is located.
     *
     * @return        The value of stored in the range [msb:lsb].
     **************************************************************************/
    type_t get_value(const index_t & msb, const index_t & lsb) const
    {
      /* Mask of the value. */
      type_t val = (*addr_v) & bit_mask<type_t, index_t>(msb, lsb);

      /* Shift it down. */
      val >>= lsb;

      /* Clear any bits not in range. */
      return val & bit_mask<type_t, index_t>(msb - lsb, 0);
    }

    /***************************************************************************
     * Set the value of a field inside the register at the position given by the
     * range [msb:lsb] inclusively. For example, consider the register given by:
     *
     *  | FIELD | BITS  |
     *  |:-----:|:-----:|
     *  | VAL2  | 31:25 |
     *  | VAL1  | 24:16 |
     *  | VAL0  | 15:0  |
     *
     * The values of the fields in the register can be set by calling:
     *
     * @code
     * // Create a 32 bit register object pointing to memory location
     * // 0x00AABBCC (i.e. the address of a memory mapped 32bit register).
     * register_t<uint32_t, int> test_reg(0x00AABBCC);
     *
     * // Set VAL0 = 1.
     * test_reg.set_value(15, 0, 1);
     *
     * // Set VAL1 = 2.
     * test_reg.set_value(24, 16, 2);
     *
     * // Set VAL2 = 3.
     * test_reg.set_value(31, 25, 3);
     * @endcode
     *
     * @param[in] msb The location in the register where the
     *                most-significant-bit of the value range appear.
     * @param[in] lsb The location in the register where the
     *                least-significant-bit of the valur range appear.
     * @param[in] val The value to write into the range specified by [msb:lsb].
     **************************************************************************/
    void set_value(const index_t & msb, const index_t & lsb, type_t val)
    {
      /* Calculate the bit mask. */
      type_t mask = bit_mask<type_t, index_t>(msb, lsb);

      /* Shift the value into place. */
      val <<= lsb;

      /* Trim any other bits that were set. Since this part of the kernel is
       * generic, this step is taken because there is no guarentee that all
       * shift operations will behave be the same on all architectures. */
      val &= mask;

      /* Clear the memory for the value to be set. */
      (*addr_v) &= ~mask;

      /* Set the value in the range. */
      (*addr_v) |= val;
    }
  };
}
