///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 jwellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef ETL_CUMULATIVE_MOVING_AVERAGE_INCLUDED
#define ETL_CUMULATIVE_MOVING_AVERAGE_INCLUDED

#include "type_traits.h"

namespace etl
{
  //***************************************************************************
  /// Cumulative Moving Average
  /// \tparam T           The sample value type.
  /// \tparam SAMPLE_SIZE The number of samples to average over.
  /// \tparam SCALING     The scaling factor applied to samples. Default = 1.
  //***************************************************************************
  template <typename T,
            const size_t SAMPLE_SIZE,
            const size_t SCALING  = 1U,
            const bool IsIntegral = std::is_integral<T>::value,
            const bool IsFloat    = std::is_floating_point<T>::value>
  class cumulative_moving_average;

  //***************************************************************************
  /// Cumulative Moving Average
  /// For integral types.
  /// \tparam T           The sample value type.
  /// \tparam SAMPLE_SIZE The number of samples to average over.
  /// \tparam SCALING     The scaling factor applied to samples. Default = 1.
  //***************************************************************************
  template <typename T, const size_t SAMPLE_SIZE_, const size_t SCALING_>
  class cumulative_moving_average<T, SAMPLE_SIZE_, SCALING_, true, false>
  {
    typedef typename etl::conditional<etl::is_signed<T>::value, int32_t, uint32_t>::type scale_t;
    typedef typename etl::conditional<etl::is_signed<T>::value, int32_t, uint32_t>::type sample_t;

    static const sample_t SAMPLES = static_cast<sample_t>(SAMPLE_SIZE_);
    static const scale_t  SCALE   = static_cast<scale_t>(SCALING_);

  public:

    static const size_t SAMPLE_SIZE = SAMPLE_SIZE_; ///< The number of samples averaged over.
    static const size_t SCALING     = SCALING_;     ///< The sample scaling factor.

    //*************************************************************************
    /// Constructor
    /// \param initial_value The initial value for the average.
    //*************************************************************************
    cumulative_moving_average(const T initial_value)
      : average(initial_value * SCALE)
    {
    }

    //*************************************************************************
    /// Clears the average.
    /// \param initial_value The initial value for the average.
    //*************************************************************************
    void clear(const T initial_value)
    {
      average = (initial_value * SCALE);
    }

    //*************************************************************************
    /// Adds a new sample to the average.
    /// \param new_value The value to add.
    //*************************************************************************
    void add(T new_value)
    {
      average *= SAMPLES;
      average += SCALE * new_value;
      average /= SAMPLES + sample_t(1);
    }

    //*************************************************************************
    /// Gets the current cumulative average.
    /// \return The current average.
    //*************************************************************************
    T value() const
    {
      return average;
    }

  private:

    T average; ///< The current cumulative average.
  };

  //***************************************************************************
  /// Cumulative Moving Average
  /// For floating point types.
  /// \tparam T           The sample value type.
  /// \tparam SAMPLE_SIZE The number of samples to average over.
  //***************************************************************************
  template <typename T, const size_t SAMPLE_SIZE_>
  class cumulative_moving_average<T, SAMPLE_SIZE_, 1U, false, true>
  {
  public:

    static const size_t SAMPLE_SIZE = SAMPLE_SIZE_;

    //*************************************************************************
    /// Constructor
    /// \param initial_value The initial value for the average.
    //*************************************************************************
    cumulative_moving_average(const T initial_value)
      : sample_size(T(SAMPLE_SIZE_)),
        sample_size_plus_1(T(SAMPLE_SIZE_ + 1)),
        average(initial_value)
    {
    }

    //*************************************************************************
    /// Clears the average.
    /// \param initial_value The initial value for the average.
    //*************************************************************************
    void clear(const T initial_value)
    {
      average = initial_value;
    }

    //*************************************************************************
    /// Adds a new sample to the average.
    /// \param new_value The value to add.
    //*************************************************************************
    void add(const T new_value)
    {
      average *= sample_size;
      average += new_value;
      average /= sample_size_plus_1;
    }

    //*************************************************************************
    /// Gets the current cumulative average.
    /// \return The current average.
    //*************************************************************************
    T value() const
    {
      return average;
    }

  private:

    const T sample_size;        ///< The sample size to average over.
    const T sample_size_plus_1; ///< One greater than the sample size.
    T       average;            ///< The current cumulative average.
  };
}

#endif
