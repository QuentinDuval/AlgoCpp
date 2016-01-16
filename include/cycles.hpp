#ifndef CYCLES_HPP
#define CYCLES_HPP

#include <internal/cycles.hpp>


template<typename Value, typename Function, typename IsDefined>
bool is_cyclic(Value init, Function fct, IsDefined is_defined)
{
   if (!is_defined(init))
      return false;

   for (Value next = fct(init); next != init; init = fct(init))
   {
      if (!details::cycle_step(next, fct, is_defined)) return false;
      if (!details::cycle_step(next, fct, is_defined)) return false;
   }
   return true;
}



#endif
