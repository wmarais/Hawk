/*******************************************************************************
 * @file
 * @author      Wynand Marais
 * @copyright   (c) 2020 Wynand Marais, MIT License
 ******************************************************************************/
#include "../../../include/hawk/kernel/kernel.hpp"
#include "../../../include/hawk/kernel/register.hpp"

using namespace hawk::kernel;



/******************************************************************************/
kernel::crit_sec_counter_t kernel::crit_sec_call_counter = 0;

/******************************************************************************/
void kernel::disable_interrupts()
{

}

/******************************************************************************/
void kernel::enable_interrupts()
{


}

/******************************************************************************/
int main(int argc, char * argv[])
{
  (void)argc;
  (void)argv;
  return 0;
}
