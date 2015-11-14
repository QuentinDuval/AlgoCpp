#ifndef FIND_PEAK_HPP
#define FIND_PEAK_HPP

#include <iterator>


//-----------------------------------------------------------------------------

template<typename Iterator>
using DefLessIter = std::less<typename std::iterator_traits<Iterator>::value_type>;

template<typename Container>
using DefLessCont = std::less<typename Container::value_type>;


//-----------------------------------------------------------------------------
// Find the peak of a function, for random access iterators (binary search use)
// Complexity is O(log(n))
//-----------------------------------------------------------------------------

template<typename RandomAccessIterator,
         typename Less = DefLessIter<RandomAccessIterator>>
RandomAccessIterator find_peak_rec(RandomAccessIterator first, RandomAccessIterator last, Less less = Less())
{
   auto curr = first + std::distance(first, last) / 2;
   if (curr == first)
      return curr;
   
   auto prev = curr - 1;
   if (less(*curr, *prev))
      return find_peak_rec(first, curr, less);

   auto next = curr + 1;
   if (less(*curr, *next))
      return find_peak_rec(next, last, less);

   return curr;
}

template<typename Container, typename Less = DefLessCont<Container>>
auto find_peak_rec(Container const& container, Less less = Less())
{
   return find_peak_rec(begin(container), end(container), less);
}

//-----------------------------------------------------------------------------

template<typename RandomAccessIterator,
         typename Less = DefLessIter<RandomAccessIterator>>
RandomAccessIterator find_peak(RandomAccessIterator first, RandomAccessIterator last, Less less = Less())
{
   while (true)
   {
      auto curr = first + std::distance(first, last) / 2;
      if (curr == first)
         return curr;

      auto prev = curr - 1;
      auto next = curr + 1;
      if      (less(*curr, *prev))  last = curr;
      else if (less(*curr, *next))  first = next;
      else                          return curr;
   }
}

template<typename Container, typename Less = DefLessCont<Container>>
auto find_peak(Container const& container, Less less = Less())
{
   return find_peak(begin(container), end(container), less);
}

//-----------------------------------------------------------------------------


#endif
