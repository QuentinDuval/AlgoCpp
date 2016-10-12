#pragma once

#include <chrono>
#include <iostream>


//-----------------------------------------------------------------------------
// Easy library to time the duration of functions
//-----------------------------------------------------------------------------

using Clock = std::chrono::high_resolution_clock;

template <typename Unit>
using Duration = std::chrono::duration<double, Unit>;

template <typename Unit>
using TimePoint = std::chrono::time_point<Clock, Duration<Unit>>;

namespace details
{
   template<typename Unit, typename Function, typename... Args>
   Duration<Unit> time_diff(TimePoint<Unit> start, Function&& f, Args&&... args)
   {
      f(std::forward<Args>(args)...);
      return Clock::now() - start;
   }
}

template<typename Unit, typename Function, typename... Args>
Duration<Unit> time_it(Function&& f, Args&&... args)
{
   auto start = Clock::now();
   return details::time_diff(start, f, std::forward<Args>(args)...);
}

template<typename Unit>
auto timer_from_now()
{
   return [start = Clock::now()](auto f, auto&&... args) {
      return details::time_diff<Unit>(start, f, args...);
   };
}


//-----------------------------------------------------------------------------
// Allows timing the execution of a function, with perfect forwarding of the arguments
//-----------------------------------------------------------------------------

template <typename Unit>
struct scoped_timer
{
   scoped_timer(Duration<Unit>& duration)
      : m_duration(duration)
      , m_start(Clock::now())
   {}

   ~scoped_timer()
   {
      m_duration += Clock::now() - m_start;
   }

private:
   Duration<Unit>& m_duration;
   TimePoint<Unit> m_start;
};

template<typename Unit, typename Fct, typename... Args>
auto time(Duration<Unit>& duration, Fct&& fct, Args&&... args)
{
   scoped_timer<Unit> timer(duration);
   return fct(forward<Args>(args)...); //Also works for void
}


//-----------------------------------------------------------------------------
// Helpers to display the results
//-----------------------------------------------------------------------------

template<typename Unit = std::milli, typename Fct, typename... Args>
Duration<Unit> time_several(std::ostream& output, size_t tries, Fct const& fct, Args&&... args)
{
   std::chrono::duration<double, Unit> duration(0);
   for (size_t i = 0; i < tries; ++i)
   {
      Fct fctCopy(fct);
      time(duration, fctCopy, std::forward<Args>(args)...);
   }
   return duration;
}

template<typename Unit = std::milli, typename Fct, typename... Args>
void show_time(std::ostream& output, std::string const& text, size_t tries, Fct&& fct, Args&&... args)
{
   output << " - Time spent (" << text << "): ";
   auto duration = time_several(output, tries, fct, std::forward<Args>(args)...);
   output << (duration.count() / tries) << std::endl;
}
