///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#ifndef ETL_FUNCTION_INCLUDED
#define ETL_FUNCTION_INCLUDED

#include "platform.h"

//*****************************************************************************
///\defgroup function function
/// A set of wrapper templates to allow a member or static function to be called
/// without the caller having to know the specific details of the callee.
/// This template class may be used to link interrupt vectors to specific member
/// functions of a handler class.
///\ingroup utilities
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  ///\ingroup function
  /// The base interface template for function template specialisations.
  ///\tparam TParameter The parameter type expected by the function.
  //***************************************************************************
  template <typename TParameter>
  class ifunction
  {
  public:

    typedef TParameter parameter_type; ///< The type of parameter sent to the function.

    virtual ~ifunction() {}
  
    //*************************************************************************
    /// The function operator that will be overridden.
    //*************************************************************************
    virtual void operator ()(TParameter) const = 0;
  };

  //***************************************************************************
  ///\ingroup function
  /// The base interface template for functions taking <b>void</b> parameters.
  //***************************************************************************
  template <>
  class ifunction<void>
  {
  public:

    typedef void parameter_type; ///< The type of parameter sent to the function.

    virtual ~ifunction() {}
  
    //*************************************************************************
    /// The function operator that will be overridden.
    //*************************************************************************
    virtual void operator ()() const = 0;
  };

  //***************************************************************************
  ///\ingroup function
  /// A derived function template that takes an object type and parameter type.
  ///\tparam TObject    The object type that contains the member function.
  ///\tparam TParameter The parameter type accepted by the member function.
  //***************************************************************************
  template <typename TObject, typename TParameter>
  class function : public ifunction<TParameter>
  {
  public:

    typedef TObject    object_type;    ///< The type of object.
    typedef TParameter parameter_type; ///< The type of parameter sent to the function.

    //*************************************************************************
    /// Constructor.
    ///\param object    Reference to the object
    ///\param p_function Pointer to the member function
    //*************************************************************************
    function(TObject& object_, void(TObject::* p_function_)(TParameter))
      : p_object(&object_),
        p_function(p_function_)
    {
    }

    //*************************************************************************
    /// The function operator that calls the destination function.
    ///\param data The data to pass to the function.
    //*************************************************************************
    virtual void operator ()(TParameter data) const ETL_OVERRIDE
    {
      // Call the object's member function with the data.
      (p_object->*p_function)(data);
    }

  private:

    TObject* p_object;                        ///< Pointer to the object that contains the function.
    void (TObject::* p_function)(TParameter); ///< Pointer to the member function.
  };

  //***************************************************************************
  ///\ingroup function
  /// A derived function template that takes a parameter type.
  ///\tparam TObject The object type that contains the member function.
  //***************************************************************************
  template <typename TObject>
  class function<TObject, void> : public ifunction<void>
  {
  public:

    //*************************************************************************
    /// Constructor.
    ///\param object   Reference to the object
    ///\param p_function Pointer to the member function
    //*************************************************************************
    function(TObject& object_, void(TObject::* p_function_)(void))
      : p_object(&object_),
        p_function(p_function_)
    {
    }

    //*************************************************************************
    /// The function operator that calls the destination function.
    //*************************************************************************
    virtual void operator ()() const ETL_OVERRIDE
    {
      // Call the object's member function.
      (p_object->*p_function)();
    }

  private:

    TObject* p_object;              ///< Pointer to the object that contains the function.
    void (TObject::* p_function)(); ///< Pointer to the member function.
  };

  //***************************************************************************
  ///\ingroup function
  /// Specialisation for static or global functions that takes a parameter.
  //***************************************************************************
  template <typename TParameter>
  class function<void, TParameter> : public ifunction<TParameter>
  {
  public:

    //*************************************************************************
    /// Constructor.
    ///\param p_function Pointer to the function
    //*************************************************************************
    explicit function(void(*p_function_)(TParameter))
      : p_function(p_function_)
    {
    }

    //*************************************************************************
    /// The function operator that calls the destination function.
    ///\param data The data to pass to the function.
    //*************************************************************************
    virtual void operator ()(TParameter data) const ETL_OVERRIDE
    {
      // Call the function with the data.
      (*p_function)(data);
    }

  private:

    void (*p_function)(TParameter); ///< Pointer to the function.
  };

  //***************************************************************************
  ///\ingroup function
  /// Specialisation static functions taking void parameter.
  //***************************************************************************
  template <>
  class function<void, void> : public ifunction<void>
  {
  public:

    //*************************************************************************
    /// Constructor.
    ///\param p_function Pointer to the function.
    //*************************************************************************
    explicit function(void(*p_function_)(void))
      : p_function(p_function_)
    {
    }

    //*************************************************************************
    /// The function operator that calls the destination function.
    //*************************************************************************
    virtual void operator ()() const ETL_OVERRIDE
    {
      // Call the function.
      (*p_function)();
    }

  private:

    void (*p_function)(); ///< Pointer to the function.
  };

  //***************************************************************************
  ///\ingroup function
  /// A derived function template that takes an object type and parameter type.
  ///\tparam TObject    The object type that contains the member function.
  ///\tparam TParameter The parameter type accepted by the member function.
  //***************************************************************************
  template <typename TObject, typename TParameter, void (TObject::*Function)(TParameter)>
  class function_mp : public ifunction<TParameter>
  {
  public:

    typedef TObject    object_type;    ///< The type of object.
    typedef TParameter parameter_type; ///< The type of parameter sent to the function.

    //*************************************************************************
    /// Constructor.
    ///\param object    Reference to the object
    //*************************************************************************
    explicit function_mp(TObject& object_)
      : p_object(&object_)
    {
    }

    //*************************************************************************
    /// The function operator that calls the destination function.
    ///\param data The data to pass to the function.
    //*************************************************************************
    virtual void operator ()(TParameter data) const ETL_OVERRIDE
    {
      // Call the object's member function with the data.
      (p_object->*Function)(data);
    }

  private:

    TObject* p_object; ///< Pointer to the object that contains the function.
  };

  //***************************************************************************
  ///\ingroup function
  /// A derived function template that takes an object type and parameter type.
  ///\tparam TObject    The object type that contains the member function.
  ///\tparam TParameter The parameter type accepted by the member function.
  //***************************************************************************
  template <typename TObject, void (TObject::*Function)(void)>
  class function_mv : public ifunction<void>
  {
  public:

    typedef TObject object_type;    ///< The type of object.
    typedef void    parameter_type; ///< The type of parameter sent to the function.

    //*************************************************************************
    /// Constructor.
    ///\param object    Reference to the object
    //*************************************************************************
    explicit function_mv(TObject& object_)
      : p_object(&object_)
    {
    }

    //*************************************************************************
    /// The function operator that calls the destination function.
    ///\param data The data to pass to the function.
    //*************************************************************************
    virtual void operator ()() const ETL_OVERRIDE
    {
      // Call the object's member function.
      (p_object->*Function)();
    }

  private:

    TObject* p_object; ///< Pointer to the object that contains the function.
  };

  //***************************************************************************
  ///\ingroup function
  /// A derived function template that takes an object type and parameter type.
  ///\tparam TObject    The object type that contains the member function.
  ///\tparam TParameter The parameter type accepted by the member function.
  //***************************************************************************
  template <typename TObject, typename TParameter, TObject& Instance, void (TObject::*Function)(TParameter)>
  class function_imp : public ifunction<TParameter>
  {
  public:

    typedef TObject    object_type;    ///< The type of object.
    typedef TParameter parameter_type; ///< The type of parameter sent to the function.

    //*************************************************************************
    /// The function operator that calls the destination function.
    ///\param data The data to pass to the function.
    //*************************************************************************
    virtual void operator ()(TParameter data) const ETL_OVERRIDE
    {
      // Call the object's member function with the data.
      (Instance.*Function)(data);
    }
  };

  //***************************************************************************
  ///\ingroup function
  /// A derived function template that takes an object type and parameter type.
  ///\tparam TObject    The object type that contains the member function.
  ///\tparam TParameter The parameter type accepted by the member function.
  //***************************************************************************
  template <typename TObject, TObject& Instance, void (TObject::*Function)(void)>
  class function_imv : public ifunction<void>
  {
  public:

    typedef TObject object_type;    ///< The type of object.
    typedef void    parameter_type; ///< The type of parameter sent to the function.

    //*************************************************************************
    /// The function operator that calls the destination function.
    ///\param data The data to pass to the function.
    //*************************************************************************
    virtual void operator ()() const ETL_OVERRIDE
    {
      // Call the object's member function.
      (Instance.*Function)();
    }
  };

  //***************************************************************************
  ///\ingroup function
  /// A derived function template that takes a parameter type.
  ///\tparam TParameter The parameter type accepted by the member function.
  //***************************************************************************
  template <typename TParameter, void (*Function)(TParameter)>
  class function_fp : public ifunction<TParameter>
  {
  public:

    typedef TParameter parameter_type; ///< The type of parameter sent to the function.

    //*************************************************************************
    /// Constructor.
    ///\param object    Reference to the object
    ///\param p_function Pointer to the member function
    //*************************************************************************
    function_fp()
    {
    }

    //*************************************************************************
    /// The function operator that calls the destination function.
    ///\param data The data to pass to the function.
    //*************************************************************************
    virtual void operator ()(TParameter data) const ETL_OVERRIDE
    {
      // Call the object's member function with the data.
      (*Function)(data);
    }
  };

  //***************************************************************************
  ///\ingroup function
  /// A derived function template that takes a parameter type.
  ///\tparam TParameter The parameter type accepted by the member function.
  //***************************************************************************
  template <void(*Function)(void)>
  class function_fv : public ifunction<void>
  {
  public:

    typedef void parameter_type; ///< The type of parameter sent to the function.

    //*************************************************************************
    /// Constructor.
    ///\param object    Reference to the object
    ///\param p_function Pointer to the member function
    //*************************************************************************
    function_fv()
    {
    }

    //*************************************************************************
    /// The function operator that calls the destination function.
    ///\param data The data to pass to the function.
    //*************************************************************************
    virtual void operator ()() const ETL_OVERRIDE
    {
      // Call the function.
      (*Function)();
    }
  };

}

#endif
