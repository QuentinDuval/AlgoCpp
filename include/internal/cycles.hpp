#ifndef INTERNAL_CYCLES_HPP
#define INTERNAL_CYCLES_HPP

namespace details
{
   template<typename Value, typename Function, typename IsDefined>
   bool cycle_step(Value& val, Function fct, IsDefined is_defined)
   {
      if (!is_defined(val)) return false;
      val = fct(val);
      return true;
   }
}

#endif
