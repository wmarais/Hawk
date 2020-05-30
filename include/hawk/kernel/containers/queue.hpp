/*******************************************************************************
 * @file
 * @author      Wynand Marais
 * @copyright   (c) 2020 Wynand Marais, MIT License
 ******************************************************************************/
#pragma once

#include "../memory.hpp"

namespace hawk::kernel
{
  /*****************************************************************************
   * Implements a Queue / Circular Queue / Ring Buffer / FIFO that can be used
   * for transfering message between tasks, peripherals, etc. The class is
   * lock free when:
   *
   *  - The index_t template parameters is specified to be a lock free atomic
   *    type.
   *  - The queue is used in a single producer - single consumer architecture.
   *
   * It is designed to be used primarily for high speed data propagation such
   * as packets in the network stack.
   ****************************************************************************/
  template <typename type_t, typename index_t, index_t max_count_c>
  class queue final
  {
    /** The list of elements in the queue. */
    unique_ptr<type_t[]> elements_v;

    /** The number of elements currently in the queue. */
    index_t count_v;

    /** The current head position in the queue. */
    index_t head_v;

    /** The current tail position in the queue. */
    index_t tail_v;

    /* Delete the copy constructor and copy assignment operator. */
    queue(const queue &) = delete;
    queue & operator = (const queue &) = delete;

  public:

    /***************************************************************************
     * Create the new queue and initialise all its value.
     **************************************************************************/
    queue() : elements_v(nullptr), count_v(0), head_v(0), tail_v(0)
    {
      /* Create the element memory of the queue. */
      elements_v = make_unique<type_t[]>(max_count_c);
    }

    /***************************************************************************
     * Check whether the queue is full.
     *
     * @return  If the queue is full the function returns true, otherwise if the
     *          queue is not fulle the function returns false.
     **************************************************************************/
    bool is_full() const
    {
      return count_v >= max_count_c;
    }

    /***************************************************************************
     * Check whether there are any elements in the queue.
     *
     * @return  If the queue contains no elements the function return true,
     *          otheriwse the function returns false.
     **************************************************************************/
    bool is_empty() const
    {
      return count_v == 0;
    }

    /***************************************************************************
     * Push an element into the queue by swapping the supplid "elem" with the
     * element in the head position of the queue. This is done to prevent
     * unecessary memory copy operations.
     *
     * @param[in,out] elem  The element that was swapped with the head element
     *                      in the queue.
     *
     * @return              If there was room to insert the element the function
     *                      returns true, otherwise if the queue is full the
     *                      function returns false.
     **************************************************************************/
    bool push_swap(type_t & elem)
    {
      /* Check if there is any space to add a new  element. */
      if(is_full()) return false;

      /* Swap the element at the head position. */
      swap(elements_v.get()[head_v], elem);

      /* Increment and wrap the head position. */
      ++head_v;
      if(head_v >= max_count_c)
        head_v = 0;

      /* Increment the count. */
      ++count_v;

      /* return true to indicate the value was swapped. */
      return true;
    }

    /***************************************************************************
     * Pop an element from the queue by swapping the supplied "elem" with the
     * element in the tail position of the queue. This is done to prevent
     * unecessary memory copy operations.
     *
     * @param[in,out] elem  The element that was swapped with the tail element
     *                      in the queue.
     *
     * @return              If there was an element in the queue that can be
     *                      poped the function return true, otherwise if the
     *                      queue was empty the function returns false.
     **************************************************************************/
    bool pop_swap(type_t & elem)
    {
      /* Check if there are any values that can be popped. */
      if(is_empty()) return false;

      /* Swap the element at the tail position. */
      swap(elements_v.get()[tail_v], elem);

      /* Increment and wrap the tail position. */
      ++tail_v;
      if(tail_v >= max_count_c)
        tail_v = 0;

      /* Decrement the count. */
      --count_v;

      /* Return true to indicate it all went well. */
      return true;
    }
  };
}
