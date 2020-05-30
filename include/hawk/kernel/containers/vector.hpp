/*******************************************************************************
 * @file
 * @author      Wynand Marais
 * @copyright   (c) 2020 Wynand Marais, MIT License
 ******************************************************************************/
#pragma once

#include "../memory.hpp"

namespace hawk::kernel
{
  template <typename type_t> class vector final
  {
    /** The list of elements in the vector. */
    unique_ptr<type_t> elements_v;

  public:

  };
}






