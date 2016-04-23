#pragma once

#include <chrono>
#include <iostream>


//-----------------------------------------------------------------------------
// Allows timing the execution of a function, with perfect forwarding of the arguments
//-----------------------------------------------------------------------------

template <typename Duration>
struct scoped_timer
{
   using Clock = std::chrono::high_resolution_clock;
   using TimePoint = std::chrono::time_point < Clock, Duration >;

   scoped_timer(Duration& duration)
      : m_duration(duration)
      , m_start(Clock::now())
   {}

   ~scoped_timer()
   {
      m_duration += Clock::now() - m_start;
   }

private:
   Duration& m_duration;
   TimePoint m_start;
};

template<typename Duration, typename Fct, typename... Args>
auto time(Duration& duration, Fct&& fct, Args&&... args)
{
   scoped_timer<Duration> timer(duration);
   return fct(forward<Args>(args)...); //Also works for void
}


//-----------------------------------------------------------------------------
// Helpers to display the results
//-----------------------------------------------------------------------------

template<typename Unit = std::milli, typename Fct, typename... Args>
std::chrono::duration<double, Unit> time_several(std::ostream& output, size_t tries, Fct const& fct, Args&&... args)
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
