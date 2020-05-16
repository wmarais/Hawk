/*******************************************************************************
 * @file
 * @author      Wynand Marais
 * @copyright   (c) 2020 Wynand Marais, MIT License
 ******************************************************************************/
#include <bitset>

unsigned int is_sub_val(unsigned int val)
{
  return static_cast<unsigned int>((std::bitset<32>(val) &
                                    std::bitset<32>(0x0000000F)).to_ulong());
}
