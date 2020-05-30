/*******************************************************************************
 * @file
 * @author      Wynand Marais
 * @copyright   (c) 2020 Wynand Marais, MIT License
 ******************************************************************************/
#pragma once

namespace hawk::kernel
{

  class memory final
  {

  public:

    template <typename type_t> static void free(type_t ** ptr)
    {
      /* Set the poitner to null. */
      *(ptr) = nullptr;
    }
  };

  template <typename type_t> void swap(type_t & lhs, type_t & rhs)
  {
    type_t temp = lhs;
    lhs = rhs;
    rhs = temp;
  }


}

extern void * operator new (size_t size);
extern void operator delete (void * ptr);
