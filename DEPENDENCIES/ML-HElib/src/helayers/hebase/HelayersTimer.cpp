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

#include "HelayersTimer.h"
#include "AlwaysAssert.h"
#include <iostream>
#include <iomanip>
#include <mutex>
#include <math.h>
#include <time.h>
#include <omp.h>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std::chrono;
using namespace std;

namespace helayers {

HelayersTimer::SectionInfo HelayersTimer::top("all");
HelayersTimer::SectionInfo* HelayersTimer::current = &top;
bool HelayersTimer::multiThreadMode = false;

HelayersTimer::HelayersTimer()
{
  lastSet = false;
  start = high_resolution_clock::now();
  cpu_start = getProcessCPUTime();
}

HelayersTimer::SectionInfo& HelayersTimer::SectionInfo::getSubSection(
    const std::string& title)
{
  const lock_guard<mutex> lock(mtx);
  if (subSections.count(title) == 0) {
    SectionInfo& res = subSections[title];
    res.parent = this;
    res.name = title;
    return res;
  }
  return subSections[title];
}

HelayersTimer::SectionInfo& HelayersTimer::SectionInfo::find(
    const std::string& title,
    const std::string& prefix)
{
  if (title.empty())
    return *this;

  size_t p = title.find(".");
  string key = title;
  string cont = "";

  if (p != string::npos) {
    key = title.substr(0, p);
    cont = title.substr(p + 1);
  }
  map<string, SectionInfo>::iterator n = subSections.find(key);
  if (n == subSections.end())
    throw invalid_argument("missing " + key + " in " + prefix);
  return n->second.find(cont, prefix + key + ".");
}

HelayersTimer::HelayersTimer(const string& title) : HelayersTimer()
{
  restart(title);
}

HelayersTimer::~HelayersTimer() { stop(); }

void HelayersTimer::restart(const string& t)
{
  stop();

  title = t;
  last = high_resolution_clock::now();
  cpu_last = getProcessCPUTime();
  lastSet = true;
  info = &current->getSubSection(title);
}

void HelayersTimer::push(const std::string& section)
{
  if (omp_in_parallel())
    return;
  current = &current->getSubSection(section);
  current->start = high_resolution_clock::now();
  current->cpu_start = getProcessCPUTime();
  // if (section=="innerSum")
  //    cout << "TIME innerSum START" << endl;
}

void HelayersTimer::pop()
{
  if (omp_in_parallel())
    return;
  if (current->parent == NULL) {
    throw runtime_error("already at top. current name=" + current->name);
  }

  current->addMeasure(current->start, current->cpu_start);
  current = current->parent;
}

void HelayersTimer::pop(int count)
{
  while (count > 0) {
    pop();
    --count;
  }
}

void HelayersTimer::stop()
{
  if (lastSet)
    info->addMeasure(last, cpu_last);
  lastSet = false;
  info = NULL;
}

void HelayersTimer::SectionInfo::addMeasure(
    std::chrono::high_resolution_clock::time_point s,
    int64_t cpu_s)
{
  auto end_time = high_resolution_clock::now();
  int64_t microsecs = duration_cast<microseconds>(end_time - s).count();

  auto end_cpu = getProcessCPUTime();
  int64_t cpu_microsecs = (end_cpu - cpu_s) / 1000;

  const lock_guard<mutex> lock(mtx);
  sum += microsecs;
  sumSquares += microsecs * microsecs;
  count += 1;
  sumCPU += cpu_microsecs;
}

int HelayersTimer::getSum(const std::string& title)
{
  return top.find(title, "").sum;
}

void HelayersTimer::printMeasureSummary(const string& sectionName,
                                        std::ostream& out)
{
  top.printMeasureSummary(sectionName, out);
}

void HelayersTimer::printMeasuresSummary(std::ostream& out)
{
  top.printMeasuresSummary(-1, out);
}

void HelayersTimer::printMeasuresSummaryFlat(std::ostream& out)
{
  cout << "Flat summary:" << endl;
  std::map<std::string, SectionInfo> flat;
  top.addToFlat(flat);
  for (std::map<string, SectionInfo>::iterator iter = flat.begin();
       iter != flat.end();
       ++iter)
    iter->second.printMeasuresSummary(0, out);
}

int64_t HelayersTimer::getProcessCPUTime()
{
#ifndef _WIN32
  struct timespec ts;
  int res = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
  if (res != 0) {
    throw runtime_error("clock_gettime failed, errno=" + to_string(errno));
  }
  return static_cast<int64_t>(ts.tv_sec) * static_cast<int64_t>(1000000000) +
         static_cast<int64_t>(ts.tv_nsec);
#else
  FILETIME createTime;
  FILETIME exitTime;
  FILETIME kernelTime;
  FILETIME userTime;

  if (GetProcessTimes(GetCurrentProcess(),
                      &createTime,
                      &exitTime,
                      &kernelTime,
                      &userTime) != -1) {
    SYSTEMTIME userSystemTime;
    if (FileTimeToSystemTime(&userTime, &userSystemTime) != -1) {
      return (int64_t)userSystemTime.wHour * 3600.0 +
             (int64_t)userSystemTime.wMinute * 60.0 +
             (int64_t)userSystemTime.wSecond +
             (int64_t)userSystemTime.wMilliseconds / 1000.0;
    } else {
      throw runtime_error("FileTimeToSystemTime failed.");
    }
  } else {
    throw runtime_error("GetProcessTimes failed.");
  }
#endif
}

void HelayersTimer::SectionInfo::add(const SectionInfo& other)
{
  const lock_guard<mutex> lock(mtx);
  count += other.count;
  sum += other.sum;
  sumSquares += other.sumSquares;
}

void HelayersTimer::SectionInfo::addToFlat(map<string, SectionInfo>& flat)
{

  if (subSections.size() > 0) {
    for (std::map<string, SectionInfo>::iterator iter = subSections.begin();
         iter != subSections.end();
         ++iter)
      iter->second.addToFlat(flat);
    return;
  }

  std::map<string, SectionInfo>::iterator iter = flat.find(name);
  if (iter != flat.end()) {
    iter->second.add(*this);
  } else {
    flat[name] = *this;
  }
}

void HelayersTimer::SectionInfo::printTopMeasureSummary(int depth,
                                                        std::ostream& out) const
{
  for (int i = 0; i < depth; ++i)
    out << "---";
  if (depth > 0)
    out << " ";
  out << name << "=" << getDurationAsString(sum) << " (secs)";
  if (count == 0) {
    out << " (0)";
  } else {
    const double mean = ((double)sum) / count;
    const double var = (((double)sumSquares) / count) - (mean * mean);

    out << " ( " << getDurationAsString(mean);
    if (count > 1) {
      const int SD = (int)((var > 0) ? sqrt(var) : 0);
      out << "+-" << getDurationAsString(SD);
    }
    out << " X " << count << ")";
  }
  out << "   [CPU: " << getDurationAsString(sumCPU) << ", " << std::fixed
      << std::setprecision(2) << (((double)sumCPU / (double)sum) * 100.0)
      << "%]";
  out << endl;
}

void HelayersTimer::SectionInfo::printMeasureSummary(const string& sectionName,
                                                     std::ostream& out) const
{
  if (name == sectionName) {
    printTopMeasureSummary(0, out);
  }

  for (std::map<string, SectionInfo>::const_iterator iter = subSections.begin();
       iter != subSections.end();
       ++iter)
    iter->second.printMeasureSummary(sectionName, out);
}

void HelayersTimer::SectionInfo::printMeasuresSummary(int depth,
                                                      std::ostream& out)
{

  if (depth >= 0) {
    printTopMeasureSummary(depth, out);
  }

  for (std::map<string, SectionInfo>::iterator iter = subSections.begin();
       iter != subSections.end();
       ++iter)
    iter->second.printMeasuresSummary(depth + 1, out);
}

string HelayersTimer::getDurationAsString(int64_t microsecs)
{
  const int64_t secs = microsecs / 1000000LL;
  const int64_t millis = (microsecs % 1000000LL) / 1000;
  ostringstream durationString;
  if (secs <= 60) {
    durationString << secs << "." << setfill('0') << setw(3) << millis;
  } else {
    const int minutes = secs / 60;
    const int secs_remain = secs % 60;
    const int tenths = millis / 100;
    durationString << minutes << ":" << setfill('0') << setw(2) << secs_remain
                   << "." << setfill('0') << setw(1) << tenths;
  }
  return durationString.str();
}

void HelayersTimer::printState(const std::string& title)
{
  cout << "HelayersTimer state: " << title << endl;
  if (multiThreadMode)
    cout << "**MULTITHREADMODE**" << endl;
  SectionInfo* si = current;
  while (si != NULL) {
    cout << si->name << endl;
    si = si->parent;
  }
}

void HelayersTimer::printOverview(std::ostream& out)
{
  out << "Timing statistics overview:" << endl;
  printMeasureSummary("context-init", out);
  printMeasureSummary("model-encrypt", out);
  printMeasureSummary("data-encrypt", out);
  printMeasureSummary("model-predict", out);
  printMeasureSummary("data-decrypt", out);
}
}
