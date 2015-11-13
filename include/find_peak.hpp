#ifndef FIND_PEAK_HPP
#define FIND_PEAK_HPP

#include <iterator>


//-----------------------------------------------------------------------------
// Find the peak of a function, for random access iterators (binary search use)
// Complexity is O(log(n))
//-----------------------------------------------------------------------------

template <
   typename RandomAccessIterator,
   typename Less = std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>
>
RandomAccessIterator find_peak(RandomAccessIterator first, RandomAccessIterator last, Less less = Less())
{
   using Diff = typename std::iterator_traits<RandomAccessIterator>::difference_type;
   Diff dist = std::distance(first, last);
   RandomAccessIterator curr = first + dist / 2;
   if (curr == first)
      return curr;
   
   RandomAccessIterator prev = curr - 1;
   if (less(*curr, *prev))
      return find_peak(first, curr, less);

   RandomAccessIterator next = curr + 1;
   if (less(*curr, *next))
      return find_peak(next, last, less);

   return curr;
}

template<typename Container,
         typename Less = std::less<typename Container::value_type>>
typename Container::const_iterator find_peak(Container const& container, Less less = Less())
{
   return find_peak(begin(container), end(container), less);
}


#endif
