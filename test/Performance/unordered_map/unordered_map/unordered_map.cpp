// unordered_map.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>

#include <unordered_map>
#include "../../../../src/unordered_map.h"

LARGE_INTEGER frequency;
LARGE_INTEGER begin;

void StartTimer()
{
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&begin);

  frequency.QuadPart /= 1000;
}

uint64_t StopTimer()
{
  LARGE_INTEGER end;

  QueryPerformanceCounter(&end);

  return (end.QuadPart - begin.QuadPart) / frequency.QuadPart;
}

const size_t TESTSIZE = 10000000UL;
const size_t TESTINTERATIONS = 16UL;

typedef std::unordered_map<uint64_t, uint16_t> Stdmap;
typedef etl::unordered_map<uint64_t, uint16_t, TESTSIZE> Etlmap;

Stdmap stdmap;
Etlmap etlmap;

int main()
{
  StartTimer();

  for (size_t i = 0UL; i < TESTINTERATIONS; ++i)
  {
    for (size_t j = 0UL; j < TESTSIZE; ++j)
    {
      std::pair<Stdmap::iterator, bool> ok = stdmap.insert(std::make_pair(uint64_t(j), uint16_t(j)));
    }

    for (size_t j = 0UL; j < TESTSIZE; ++j)
    {
      stdmap.erase(j);
    }
  }

  uint64_t time;

  time = StopTimer();
  std::cout << "STD Time = " << time << "ms\n";

  StartTimer();

  for (size_t i = 0UL; i < TESTINTERATIONS; ++i)
  {
    for (size_t j = 0UL; j < TESTSIZE; ++j)
    {
      std::pair<Etlmap::iterator, bool> ok = etlmap.insert(std::make_pair(uint64_t(j), uint16_t(j)));
    }

    for (size_t j = 0UL; j < TESTSIZE; ++j)
    {
      etlmap.erase(j);
    }
  }

  time = StopTimer();
  std::cout << "ETL Time = " << time << "ms\n";

  return 0;
}

