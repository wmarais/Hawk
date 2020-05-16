
//static void escape(void * p)
//{
//  asm volatile("" : : "g"(p) : "memory");
//}

//static void clobber()
//{
//  asm volatile("" : : : "memory");
//}


unsigned int is_set(unsigned int val)
{
  unsigned int mask = 0x00000001;

  return val & mask;
}

//unsigned int is_set2(unsigned int val)
//{
//  unsigned int mask = 0x00000001;

//  return val & mask;
//}

unsigned int is_sub_val(unsigned int val)
{
  return val & 0x0000000F;
}
