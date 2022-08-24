#pragma once

#include "platform.h"

template <typename TDestination, typename TSource>
typename etl::enable_if<(sizeof(TDestination) == sizeof(TSource))   && 
                         etl::is_trivially_copyable<TSource>::value && 
                         etl::is_trivially_copyable<TDestination>::value, TDestination>::type
  bit_cast(const TSource& source) ETL_NOEXCEPT
{
  TDestination destination;

  memcpy(&destination, &source, sizeof(TDestination));

  return destination;
}

template <typename TDestination, typename TSource>
ETL_CONSTEXPR
typename etl::enable_if<(sizeof(TDestination) == sizeof(TSource))   &&
                         etl::is_trivially_copyable<TSource>::value &&
                         etl::is_trivially_copyable<TDestination>::value, TDestination>::type
  bit_cast(const TSource& source) ETL_NOEXCEPT
{
  TDestination destination;

  __builtin_memcpy(&destination, &source, sizeof(TDestination));

  return destination;
}



