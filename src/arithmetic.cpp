#include <arithmetic.hpp>

#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <vector>


//-----------------------------------------------------------------------------

static int largest_doubling(int a, int b)
{
   if (b <= a - b)
      return largest_doubling(a, b + b);
   return b;
}

template<typename OutputIterator>
void fill_doublings(int a, int b, OutputIterator out)
{
   for (; b <= a; b += b, ++out)
      *out = b;
}

//-----------------------------------------------------------------------------

int slow_remainder(int a, int b)
{
   while (b <= a)
      a = a - b;
   return a;
}

int fast_remainder_rec(int a, int b)
{
   if (b <= a - b)
      a = fast_remainder_rec(a, b + b);
   return a >= b ? a - b : a;
}

int fast_remainder_iter(int a, int b)
{
   std::vector<int> factors;
   fill_doublings(a, b, std::back_inserter(factors));

   auto accumulator = [](int a, int f) { return a >= f ? a - f : a; };
   return std::accumulate(factors.rbegin(), factors.rend(), a, accumulator);
}

int fast_remainder_half(int a, int b)
{
   for (int f = largest_doubling(a, b); f >= b; f = f >> 1)
   {
      if (a >= f)
         a = a - f;
   }
   return a;
}

static int fast_remainder_cps_impl(int a, int b, std::function<int(int)> cont)
{
   auto nextCont = [b, cont](int a) { return cont(a >= b ? a - b : a); };
   if (b <= a - b)
      return fast_remainder_cps_impl(a, b + b, nextCont);
   return nextCont(a);
}

int fast_remainder_cps(int a, int b)
{
   return fast_remainder_cps_impl(a, b, [](int i) { return i; });
}

//-----------------------------------------------------------------------------

std::pair<int, int> slow_quotient(int a, int b)
{
   int q = 0;
   for (; b <= a; a = a - b) ++q;
   return { q, a };
}

static int fast_quotient_step(int& a, int b, int q)
{
   if (a < b) return q;
   a = a - b;
   return q + 1;
}

static int fast_quotient_rec_impl(int& a, int b)
{
   int q = b <= a - b ?
      fast_quotient_rec_impl(a, b + b) : 0;
   return fast_quotient_step(a, b, q + q);
}

std::pair<int, int> fast_quotient_rec(int a, int b)
{
   int q = fast_quotient_rec_impl(a, b);
   return { q, a };
}

std::pair<int, int> fast_quotient_iter(int a, int b)
{
   std::vector<int> factors;
   fill_doublings(a, b, std::back_inserter(factors));

   auto accumulator = [&a](int q, int f) { return fast_quotient_step(a, f, q + q); };
   int q = std::accumulate(factors.rbegin(), factors.rend(), 0, accumulator);
   return { q, a };
}

std::pair<int, int> fast_quotient_half(int a, int b)
{
   int q = 0;
   for (int f = largest_doubling(a, b); f >= b; f = f >> 1)
      q = fast_quotient_step(a, f, q + q);
   return { q, a };
}

//-----------------------------------------------------------------------------

