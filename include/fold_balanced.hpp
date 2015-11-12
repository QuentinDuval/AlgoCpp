#ifndef FOLD_BALANCED_HPP
#define FOLD_BALANCED_HPP

#include <algorithm>
#include <numeric>


//-----------------------------------------------------------------------------
// Version of accumulate with balance (group elements by number of accumulation)
// @require:
// - The operation to be associative
// - The operation to work on two results (elements scanned are results too)
//-----------------------------------------------------------------------------

template<typename BinaryOp, typename Result>
class BalancedAccumulator
{
public:
   BalancedAccumulator(BinaryOp op, size_t counter_size)
      : m_op(op), m_results(), m_count(0)
   {
      m_results.reserve(counter_size);
   }
   
   void operator()(Result res)
   {
      size_t relative_count = m_count++;
      while (true)
      {
         if (relative_count % 2 == 0)
         {
            m_results.push_back(std::move(res));
            return;
         }

         res = m_op(m_results.back(), res);
         m_results.pop_back();
         relative_count = relative_count / 2;
      }
   }

   Result finalize(Result res)
   {
      return std::accumulate(begin(m_results), end(m_results), res, m_op);
   }

private:
   BinaryOp            m_op;
   std::vector<Result> m_results;
   size_t              m_count;
};

template<typename FwdIterator, typename Result, typename BinaryOp>
Result fold_balanced(FwdIterator first, FwdIterator last, Result init, BinaryOp op)
{
   BalancedAccumulator<BinaryOp, Result> accOp(op, 64);
   std::for_each(first, last, std::ref(accOp));
   return accOp.finalize(init);
}

template<typename Container, typename Result, typename BinaryOp>
Result fold_balanced(Container const& container, Result init, BinaryOp op)
{
   return fold_balanced(begin(container), end(container), init, op);
}


#endif
