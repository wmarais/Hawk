/*******************************************************************************
 * @file
 * @author      Wynand Marais
 * @copyright   (c) 2020 Wynand Marais, MIT License
 ******************************************************************************/

namespace hawk::kernel
{
  /*****************************************************************************
   * Implemenets the unique_ptr<> class as per C++17 interface.
   ****************************************************************************/
  template <typename type_t> class unique_ptr final
  {
    /** The raw pointer to object memory. */
    type_t ptr_v;

    /* Delete the copy constructor and copy assignment operator. Unique pointers
     * must only every be moved. */
    unique_ptr(const unique_ptr<type_t>&) = delete;
    unique_ptr<type_t> & operator = (const unique_ptr<type_t>&) = delete;
  public:

    /***************************************************************************
     * Initialise the value of the pointer to null or to the supplied pointer
     * address.
     **************************************************************************/
    unique_ptr(type_t ptr = nullptr) : ptr_v(ptr) {}

    /***************************************************************************
     * Deallocate the memory when the unique_ptr object is destroyed.
     **************************************************************************/
    ~unique_ptr()
    {
      /* Deallocate the object. */
      memory::free(&ptr_v);
    }

    /***************************************************************************
     * Return the raw pointer to the pointer contained within this unique_ptr
     * object.
     **************************************************************************/
    type_t * get()
    {
      return ptr_v;
    }
  };

  /*****************************************************************************
   * Provides the facility to create a pointer to an object and automatically
   * invoke the respective constructor, i.e. make_unique<type_t>(args). If the
   * required memory can not be allocated a kernel panic will occur.
   *
   * <b>Example 1</b>
   * The following example demonstrates how to create a pointer to a single
   * integer initialise to 5.
   * @code
   * unique_ptr<int> temp = make_unique<int>(5);
   * @endcode
   *
   * @tparam type_t     The type of thje object the thje unique pointer will
   *                    point too.
   *
   * @tparam args_t     The types of the object's constructor arguments.
   *
   * @param[in] args    The arguments that will be passed to the constructor of
   *                    the object.
   *
   * @return            The pointer to the object that was created.
   ****************************************************************************/
  template <typename type_t, typename ... args_t>
  unique_ptr<T> make_unique(args_t&&... args)
  {
  }

  /*****************************************************************************
   * Provides the facility to create an array of objects, i.e.
   * make_unique<type_t[]>(count). If the required memory can not be allocated
   * a kernel panic will occur.
   *
   * <b>Example 1</b>
   * The following example demonstrates how to create a pointer to a an array
   * of 50 integers.
   * @code
   * unique_ptr<int[]> array = make_unique<int[]>(50);
   * @endcode
   *
   * @tparam type_t     The type of thje object the thje unique pointer will
   *                    point too.
   *
   * @tparam args_t     The types of the object's constructor arguments.
   *
   * @param [in] count  The number of elements to allocate space for.
   *
   * @return            The pointer to the array of objects that was created.
   ****************************************************************************/
  template <class T>
  unique_ptr<T> make_unique(size_t count)
  {

  }

  /*****************************************************************************
   * Deleted to disallow the use of make_unique<type_t[N]>().
   ****************************************************************************/
  template <class T, class... Args>
  /* unspecified */ make_unique(Args&&...) = delete;
}

