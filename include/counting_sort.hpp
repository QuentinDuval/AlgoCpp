#ifndef COUNTING_SORT_HPP
#define COUNTING_SORT_HPP

#include <algorithm>
#include <iterator>
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


//-----------------------------------------------------------------------------
// Counting sort in place for random access iterators
// Sort in linear time, if the inputs can be projected in [0..M] integer space
//-----------------------------------------------------------------------------

template<typename InoutIterator, typename Projection>
void counting_sort_in_place(InoutIterator first, InoutIterator last, Projection proj)
{
   if (first == last)
      return;

   using ValueType = typename std::iterator_traits<InoutIterator>::value_type;
   auto proj_value_range = std::minmax_element(first, last, [&proj](auto& lhs, auto& rhs) { return proj(lhs) < proj(rhs); });
   auto min_proj = proj(*proj_value_range.first);
   auto max_proj = proj(*proj_value_range.second);
   auto proj_gap = max_proj - min_proj + 1;

   //Cout each occurrences
   std::vector<size_t> counts(proj_gap);
   for (auto curr = first; curr != last; ++curr)
      counts[proj(*curr) - min_proj] += 1;

   //Compute the ranges
   std::vector<InoutIterator> starts(proj_gap, first);
   for (decltype(proj_gap) i = 1; i < proj_gap; ++i)
      starts[i] = starts[i - 1] + counts[i - 1];
   std::vector<InoutIterator> ends = starts;

   //Loop on the ranges
   for (decltype(proj_gap) i = 0; i < proj_gap - 1; ++i)
   {
      while (ends[i] != starts[i+1])
      {
         auto& elem = *(ends[i]);
         auto proj_val = proj(elem) - min_proj;
         if (proj_val == i)
         {
            ++(ends[i]);
         }
         else
         {
            std::swap(elem, *ends[proj_val]);
            ++(ends[proj_val]);
         }
      }
   }
}



#endif
