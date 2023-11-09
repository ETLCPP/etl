///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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

#ifndef ETL_MACROS_INCLUDED
#define ETL_MACROS_INCLUDED

#define ETL_CONCAT2(X, Y)  X##Y
#define ETL_CONCAT(X, Y)   ETL_CONCAT2(X, Y)

#define ETL_STRINGIFY_1(...) #__VA_ARGS__
#define ETL_STRINGIFY(...) ETL_STRINGIFY_1(__VA_ARGS__)

#define ETL_STRING(X)      ETL_CONCAT(, ETL_STRINGIFY(X))
#define ETL_WIDE_STRING(X) ETL_CONCAT(L, ETL_STRINGIFY(X))
#define ETL_U8_STRING(X)   ETL_CONCAT(u8, ETL_STRINGIFY(X))
#define ETL_U16_STRING(X)  ETL_CONCAT(u, ETL_STRINGIFY(X))
#define ETL_U32_STRING(X)  ETL_CONCAT(U, ETL_STRINGIFY(X))

/**
 * Resolves a function-like macro overload with 1 or 2 arguments.
 *
 * This macro can be used to call other function-like macros depending on the number of arguments
 * provided by the caller.
 *
 * \param _1 is the first argument given to the called macro
 * \param _2 is either the second argument given to the called macro
 *           or the name of the macro which takes two arguments
 * \param SELECTED_MACRO_NAME is the name of the macro to be called
 * \param ... is a non-empty list of arguments:
 *        The beginning of the list is a list of all macro names which accept less arguments than
 *        `SELECTED_MACRO_NAME`. This list may be empty.
 *        The last element in the list is a dummy argument. For example `""`.
 *        For compatibility reasons `...` should not be empty.
 *
 * Use this function-like macro as follows:
 *
 *     #define TAKES1( a ) (a)    //!< example macro which takes 1 argument
 *     #define TAKES2(a, b) (a+b) //!< example macro which takes 2 arguments
 *     #define TAKES_1or2(...) GET_MACRO_OVERLOAD2(__VA_ARGS__, TAKES2, TAKES1, "")(__VA_ARGS__)
 *
 * Now `TAKES_1or2()` can be called with 1 or 2 arguments.
 */
#define GET_MACRO_OVERLOAD2(_1, _2, SELECTED_MACRO_NAME, ...) SELECTED_MACRO_NAME

#endif

