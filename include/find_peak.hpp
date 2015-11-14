#ifndef FIND_PEAK_HPP
#define FIND_PEAK_HPP

#include <iterator>


//-----------------------------------------------------------------------------
// Find the peak of a function, for random access iterators (binary search use)
// Complexity is O(log(n))
//-----------------------------------------------------------------------------

template<typename Iterator>
using DefLessIter = std::less<typename std::iterator_traits<Iterator>::value_type>;

template<typename Container>
using DefLessCont = std::less<typename Container::value_type>;

template<typename RandomAccessIterator, typename Less>
auto find_peak_impl(RandomAccessIterator first, RandomAccessIterator last,
                    Less less, std::random_access_iterator_tag)
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

template<typename ForwardIterator, typename Less>
auto find_peak_impl(ForwardIterator first, ForwardIterator last,
                    Less less, std::bidirectional_iterator_tag)
{
   if (first == last)
      return first;

   auto prev = first++;
   if (less(*first, *prev))
      return prev;

   for (; first != last; ++prev, ++first)
   {
      auto next = std::next(first);
      if (next == last)
         return first;

      if (less(*prev, *first) && less(*next, *first))
         return first;
   }
   return prev;
}

template<typename Iterator, typename Less = DefLessIter<Iterator>>
Iterator find_peak(Iterator first, Iterator last, Less less = Less())
{
   return find_peak_impl(first, last, less, typename std::iterator_traits<Iterator>::iterator_category());
}

template<typename Container, typename Less = DefLessCont<Container>>
auto find_peak(Container const& container, Less less = Less())
{
   return find_peak(begin(container), end(container), less);
}


#endif
