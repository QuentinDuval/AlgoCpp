#ifndef COUNTING_SORT_HPP
#define COUNTING_SORT_HPP

#include <algorithm>
#include <iterator>
#include <vector>


//-----------------------------------------------------------------------------
// Counting sort
// Sort in linear time, if the inputs can be projected in [0..M] integer space
//-----------------------------------------------------------------------------

template<typename InoutIterator, typename IntegerProjection>
void counting_sort(InoutIterator first, InoutIterator last,
                   IntegerProjection proj, size_t max_proj_value)
{
   using ValueType = typename std::iterator_traits<InoutIterator>::value_type;
   std::vector<std::vector<ValueType>> counts(max_proj_value + 1);
   for (auto curr = first; curr != last; ++curr)
      counts[proj(curr)].push_back(*curr);

   for (auto& vals : counts)
      first = std::copy(begin(vals), end(vals), first);
}

template<typename InoutIterator, typename IntegerProjection>
void counting_sort(InoutIterator first, InoutIterator last, IntegerProjection proj)
{
   if (first == last)
      return;

   std::vector<size_t> proj_values;
   std::transform(first, last, std::back_inserter(proj_values), proj);
   auto max_proj_value = std::max_element(begin(proj_values), end(proj_values));
   counting_sort(first, last, proj, *max_proj_value);
}


#endif
