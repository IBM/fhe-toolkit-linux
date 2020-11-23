/*
* MIT License
*
* Copyright (c) 2020 International Business Machines
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef SRC_HEDGE_SIMPLETIMER_H
#define SRC_HEDGE_SIMPLETIMER_H

#include <cstdint>
#include <chrono>
#include <map>
#include <string>
#include <sstream>
#include <mutex>

#ifndef NO_INTERNAL_HEDGE_PROFILING

#define HEDGE_TIMER(title) hedge::HedgeTimer _hedge_timer(title)
#define HEDGE_TIMER_SECTION(title) hedge::HedgeTimer::Guard guard(title)
#define HEDGE_TIMER_PUSH(title) hedge::HedgeTimer::push(title)
#define HEDGE_TIMER_POP() hedge::HedgeTimer::pop()
#define HEDGE_TIMER_POP_COUNT(count) hedge::HedgeTimer::pop(count)

#define HEDGE_TIMER_SECTION_THREAD_MODE(section)                               \
  hedge::HedgeTimer::pushThreadMode(section)

#define HEDGE_TIMER_POP_THREAD_MODE(section)                                   \
  hedge::HedgeTimer::popThreadMode(section)

#define HEDGE_TIMER_PRINT_STATE(title) hedge::HedgeTimer::printState(title)
#define HEDGE_TIMER_GET_SUM(title) hedge::HedgeTimer::getSum(title)
#define HEDGE_TIMER_RESTART(title) _hedge_timer.restart(title)
#define HEDGE_TIMER_STOP() _hedge_timer.stop()

#define HEDGE_TIMER_PRINT_MEASURES_SUMMARY()                                   \
  hedge::HedgeTimer::printMeasuresSummary()

#define HEDGE_TIMER_PRINT_MEASURES_SUMMARY_FLAT()                              \
  hedge::HedgeTimer::printMeasuresSummaryFlat()

#define HEDGE_TIMER_GET_DURATION_AS_STRING(microsecs)                          \
  hedge::HedgeTimer::getDurationAsString(microsecs)

#define HEDGE_TIMER_IS_MULTITHREAD_MODE hedge::HedgeTimer::isMultiThreadMode()
#define HEDGE_IMER_GET_PROCESS_CPU_TIME hedge::HedgeTimer::getProcessCPUTime()

#else

#define HEDGE_TIMER(title)
#define HEDGE_TIMER_SECTION(title)
#define HEDGE_TIMER_PUSH(title)
#define HEDGE_TIMER_POP()
#define HEDGE_TIMER_POP_COUNT(count)
#define HEDGE_TIMER_SECTION_THREAD_MODE(section)
#define HEDGE_TIMER_POP_THREAD_MODE(section)
#define HEDGE_TIMER_PRINT_STATE(title)
#define HEDGE_TIMER_GET_SUM(title)
#define HEDGE_TIMER_RESTART(st, title)
#define HEDGE_TIMER_STOP(st)
#define HEDGE_TIMER_PRINT_MEASURES_SUMMARY
#define HEDGE_TIMER_PRINT_MEASURES_SUMMARY_FLAT
#define HEDGE_TIMER_GET_DURATION_AS_STRING(microsecs)
#define HEDGE_TIMER_IS_MULTITHREAD_MODE()
#define HEDGE_IMER_GET_PROCESS_CPU_TIME()

#endif

namespace hedge {

class HedgeTimer
{
  struct SectionInfo
  {
    std::mutex mtx;
    std::int64_t sum = 0;
    std::int64_t sumSquares = 0;
    std::int64_t count = 0;
    std::int64_t sumCPU = 0;

    std::chrono::high_resolution_clock::time_point start;
    int64_t cpu_start;

    std::map<std::string, SectionInfo> subSections;
    SectionInfo* parent = NULL;
    std::string name;

    SectionInfo() {}
    SectionInfo(const std::string& n) : name(n) {}
    SectionInfo(const SectionInfo& other)
        : name(other.name),
          sum(other.sum),
          sumSquares(other.sumSquares),
          count(other.count),
          sumCPU(other.sumCPU),
          subSections(other.subSections)
    {
    }

    SectionInfo& operator=(const SectionInfo& other)
    {
      if (&other != this) {
        sum = other.sum;
        sumSquares = other.sumSquares;
        count = other.count;
        sumCPU = other.sumCPU;
        subSections = other.subSections;
        parent = other.parent;
        name = other.name;
      }
      return *this;
    }

    void printMeasuresSummary(int depth);
    void addToFlat(std::map<std::string, SectionInfo>& flat);

    SectionInfo& find(const std::string& title, const std::string& prefix);

    void addMeasure(std::chrono::high_resolution_clock::time_point s,
                    int64_t cpu_s);
    SectionInfo& getSubSection(const std::string& title);
    void add(const SectionInfo& other);
  };
  static SectionInfo top;
  static SectionInfo* current;
  static bool multiThreadMode;

public:
  // Define a HedgeTimer::Guard local variable to automatically push a timer
  // section and pop it when the scope ends.
  class Guard
  {
  public:
    Guard(const std::string& title) { HedgeTimer::push(title); }
    ~Guard() { HedgeTimer::pop(); }
  };

  HedgeTimer();
  HedgeTimer(const std::string& title);
  ~HedgeTimer();

  static void push(const std::string& section);
  static void pop();
  static void pop(int count);

  static void pushThreadMode(const std::string& section);
  static void popThreadMode(const std::string& section);

  static void printState(const std::string& title = "");

  static int getSum(const std::string& title);

  void restart(const std::string& title);

  void stop();

  static void printMeasuresSummary();

  static void printMeasuresSummaryFlat();

  static std::string getDurationAsString(std::int64_t microsecs);

  static bool isMultiThreadMode() { return multiThreadMode; }

  // Return the current process CPU time (accumulated for all threads) in
  // nanoseconds
  static int64_t getProcessCPUTime();

private:
  int calcDuration(std::string fullTitle,
                   std::chrono::high_resolution_clock::time_point lastStart);

  std::chrono::high_resolution_clock::time_point start;
  std::chrono::high_resolution_clock::time_point last;
  int64_t cpu_start;
  int64_t cpu_last;
  std::string title;
  SectionInfo* info = NULL;

  bool lastSet;
};
}

#endif /* SRC_HEDGE_SIMPLETIMER_H */
