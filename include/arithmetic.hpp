#ifndef ARITHMETIC_HPP
#define ARITHMETIC_HPP

#include <utility>


//-----------------------------------------------------------------------------
// Computing remainders, using only + and -.
//-----------------------------------------------------------------------------

int slow_remainder      (int a, int b);
int fast_remainder_rec  (int a, int b);
int fast_remainder_iter (int a, int b);
int fast_remainder_half (int a, int b);
int fast_remainder_cps  (int a, int b);

//-----------------------------------------------------------------------------
// Computing quotient, using only + and -.
//-----------------------------------------------------------------------------

std::pair<int, int> slow_quotient      (int a, int b);
std::pair<int, int> fast_quotient_rec  (int a, int b);
std::pair<int, int> fast_quotient_iter (int a, int b);
std::pair<int, int> fast_quotient_half (int a, int b);


#endif
