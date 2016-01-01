#ifndef FILTERING_HPP
#define FILTERING_HPP

#include <utility>


template<typename ForwardIt, typename Predicate>
ForwardIt filter_if(ForwardIt first, ForwardIt last, Predicate pred)
{
   ForwardIt last_valid = first;
   for (; first != last; ++first)
   {
      if (!pred(*first))
         continue;
      
      *last_valid = std::move(*first);
      ++last_valid;
   }
   return last_valid;
}

//-----------------------------------------------------------------------------

template<typename BidirectionalIt, typename Predicate>
BidirectionalIt unstable_filter_if(BidirectionalIt first, BidirectionalIt last, Predicate pred)
{
   if (first == last)
      return first;

   while (first != last)
   {
      if (!pred(*first))
      {
         --last;
         *first = std::move(*last);
      }
      else
      {
         ++first;
      }
   }
   return last;
}


#endif
