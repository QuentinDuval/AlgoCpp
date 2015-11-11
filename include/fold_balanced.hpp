#ifndef FOLD_BALANCED_HPP
#define FOLD_BALANCED_HPP


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
      : m_op(op)
      , m_results(counter_size)
      , m_marked(counter_size, false)
   {}
   
   void add(Result res)
   {
      for (size_t i = 0; i < m_marked.size(); ++i)
      {
         if (!m_marked[i])
         {
            m_marked[i] = true;
            m_results[i] = res;
            return;
         }

         res = m_op(m_results[i], res);
         m_marked[i] = false;
      }
   }

   Result finalize(Result res)
   {
      for (size_t i = m_marked.size(); i != 0; --i)
         if (m_marked[i - 1])
            res = m_op(res, m_results[i - 1]);
      return res;
   }

private:
   BinaryOp            m_op;
   std::vector<Result> m_results;
   std::vector<bool>   m_marked;
};

template<typename FwdIterator, typename Result, typename BinaryOp>
Result fold_balanced(FwdIterator first, FwdIterator last, Result init, BinaryOp op)
{
   BalancedAccumulator<BinaryOp, Result> accOp(op, 64);
   for (; first != last; ++first)
      accOp.add(*first);
   return accOp.finalize(init);
}

template<typename Container, typename Result, typename BinaryOp>
Result fold_balanced(Container const& container, Result init, BinaryOp op)
{
   return fold_balanced(begin(container), end(container), init, op);
}


#endif
