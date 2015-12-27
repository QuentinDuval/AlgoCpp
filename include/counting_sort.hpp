#ifndef COUNTING_SORT_HPP
#define COUNTING_SORT_HPP

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>


//-----------------------------------------------------------------------------
// Counting sort
// Sort in linear time, if the inputs can be projected in [0..M] integer space
//-----------------------------------------------------------------------------

template<typename InoutIterator, typename ProjectionIt>
void counting_sort(InoutIterator first, InoutIterator last,
                   ProjectionIt proj_first, ProjectionIt proj_last)
{
   if (first == last)
      return;

   using ValueType = typename std::iterator_traits<InoutIterator>::value_type;
   auto proj_value_range = std::minmax_element(proj_first, proj_last);
   auto min_proj = *proj_value_range.first;
   auto max_proj = *proj_value_range.second;

   std::vector<std::vector<ValueType>> counts(max_proj - min_proj + 1);
   for (auto curr = first; curr != last; ++curr, ++proj_first)
      counts[*proj_first - min_proj].push_back(std::move(*curr));

   for (auto& vals : counts)
      first = std::move(begin(vals), end(vals), first);
}

template<typename InoutIterator, typename Projection>
void counting_sort(InoutIterator first, InoutIterator last, Projection proj)
{
   std::vector<size_t> proj_values;
   std::transform(first, last, std::back_inserter(proj_values), proj);
   counting_sort(first, last, begin(proj_values), end(proj_values));
}


#endif
