///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 John Wellbelove
Copyright(c) 2024 BMW AG

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

#ifndef ETL_SINGLETON_BASE_INCLUDED
#define ETL_SINGLETON_BASE_INCLUDED

///\defgroup singleton_base singleton_base
/// Templated version of the singleton pattern, implemented as base class
/// for singletons.
///\ingroup etl

#include "platform.h"
#include "error_handler.h"
#include "nullptr.h"
#include "file_error_numbers.h"

namespace etl
{
  //*************************************************************************
  /// Base singleton error exception.
  //*************************************************************************
  class singleton_base_exception : public etl::exception
  {
  public:

    singleton_base_exception(string_type reason_, string_type file_name_, numeric_type line_number_)
      : exception(reason_, file_name_, line_number_)
    {
    }
  };

  //*************************************************************************
  /// Singleton not created error exception.
  //*************************************************************************
  class singleton_base_not_created : public etl::singleton_base_exception
  {
  public:

    singleton_base_not_created(string_type file_name_, numeric_type line_number_)
      : singleton_base_exception(ETL_ERROR_TEXT("singleton_base:not created", ETL_SINGLETON_BASE_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //*************************************************************************
  /// Singleton instance already exists.
  //*************************************************************************
  class singleton_base_already_created : public etl::singleton_base_exception
  {
  public:

    singleton_base_already_created(string_type file_name_, numeric_type line_number_)
      : singleton_base_exception(ETL_ERROR_TEXT("singleton_base:already created", ETL_SINGLETON_BASE_FILE_ID"A"), file_name_, line_number_)
    {
    }
  };

  //***********************************************************************
  /// Base class for singletons.
  /// \tparam T   Any type that wants to expose the instance() interface.
  ///
  /// This class is designed to work as a generic base class for any class that wants to
  /// provide a singleton interface. It'll also work for classes that do not have a
  /// default constructor.
  ///
  /// Usage example:
  ///
  /// class Origin
  ///   : singleton<Origin>
  /// {
  /// public:
  ///     Origin(int x, int y)
  ///     :   singleton<Origin>(*this)
  ///     {}
  ///
  ///     int getX() const;
  /// } theOrigin(0, 0);
  ///
  /// int x = Origin::instance().getX();
  ///
  ///
  /// Note:
  ///
  /// It is important that a call to instance() will not create the instance of the class. It needs
  /// to be created by the user before calling instance(). This way, the user has better control
  /// over the instance lifetime instead of e.g. lazy initialization.
  //***********************************************************************
  template <typename T>
  class singleton_base
  {
  public:

    //***********************************************************************
    // Returns a reference to the instance.
    //***********************************************************************
    static T& instance()
    {
      ETL_ASSERT(m_self != ETL_NULLPTR, ETL_ERROR(etl::singleton_base_not_created));

      return *m_self;
    }

    //***********************************************************************
    /// Returns whether an instance has been attached to singleton<T> or not.
    //***********************************************************************
    static bool is_valid() 
    { 
      return (m_self != ETL_NULLPTR); 
    }

  protected:

    //***********************************************************************
    /// Constructs the instance of singleton.
    /// theInstance Reference to T, which will be returned when instance() is called.
    //***********************************************************************
    explicit singleton_base(T& theInstance)
    {
      ETL_ASSERT(m_self == ETL_NULLPTR, ETL_ERROR(etl::singleton_base_already_created));

      m_self = &theInstance;
    }

    //***********************************************************************
    /// Removes the internal reference to the instance passed in the constructor.
    //***********************************************************************
    ~singleton_base() 
    { 
      m_self = ETL_NULLPTR; 
    }

  private:

    static T* m_self;
  };

  //***********************************************************************
  /// No violation of one definition rule as this is a class template
  //***********************************************************************
  template<class T>
  T* singleton_base<T>::m_self = ETL_NULLPTR;
}

#endif
