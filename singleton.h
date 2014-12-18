///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

Copyright(c) 2014 jwellbelove

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

#ifndef __ETL_SINGLETON__
#define __ETL_SINGLETON__

///\defgroup singleton singleton
/// Templated version of the sing;eton pattern.
///\ingroup etl

namespace etl
{
	//*************************************************************************
    /// Singleton pattern base class.
    /// \ingroup singleton
    //*************************************************************************
    template<typename T>
    class singleton
    {  
    public:

        //*********************************************************************
        /// Constructor.
        //*********************************************************************
        singleton()
        {
        }

        //*********************************************************************
        /// Destructor.
        //*********************************************************************
        virtual ~singleton()
        {
        }

        //*********************************************************************
        /// Returns a reference to the instance.
        ///\return A reference to the instance.
        //*********************************************************************
        T& operator *()
        {
            return instance();
        }

        //*********************************************************************
        /// Returns a const reference to the instance.
        /// \return A const reference to the instance.
        //*********************************************************************
        // @short-legal-notice
        //*********************************************************************
        const T& operator *() const
        {
            return instance();
        }

        //*********************************************************************
        /// Returns a pointer to the instance.
        /// \return A pointer to the instance.
        //*********************************************************************
        T* operator ->()
        {
            return &instance();
        }

    //*********************************************************************
    /// Returns a const pointer to the instance.
    /// \return A const pointer to the instance.
    //*********************************************************************
    const T* operator ->() const
    {
      return &instance();
    }

    private:

    //*************************************************************************
    /// Returns a reference to the instance.
    ///\return A reference to the instance.
    //*************************************************************************
    static T &instance()
    {
      static T the_instance;

      return the_instance;
    }
  };
}

#endif
