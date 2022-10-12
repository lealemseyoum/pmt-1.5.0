/*
 * Copyright (c) 2000-2009, Eastman Kodak Company
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification,are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice, 
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the 
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Eastman Kodak Company nor the names of its 
 *       contributors may be used to endorse or promote products derived from 
 *       this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * 
 * Original Author: 
 * Mike W. Pryor michael.pryor@kodak.com
 * 
 * Copyright (C) 1998 Eastman Kodak Company.
 *
 * Contributor(s): 
 *
 * Date: February 1998
 *
*/



#ifndef EK_STRINGS_H
#define EK_STRINGS_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#if !defined (EK_COMPILER_H)
#include "EkCompiler.h"
#endif

#include <string>

/*  Class wstring
 *
 *      For some reason, the GNU compiler has commented out its typedef
 *      for wEkString (basic_EkString<wchar_t>).  We'll cross our fingers 
 *      and add it back in over here.
 */

#if defined (__GNUC__) && ( __GNUC__ < 3 )
#include <wchar.h>
typedef basic_string < wchar_t > wstring;
#endif

// #if ((defined _MSC_VER) && (_MSC_VER >= 1300))
// We didn't want to be in the business of maintaining a string class, but
// nobody has a good thread-safe class built in.  MS has one in VC 7.0 +, but
// it chewed through 1.3 more MB in our tests, and added .3 seconds.
// So, we set _MSC_VER much into the future, hoping that by then they will 
// have fixed these problems....
#if ((defined _MSC_VER) && (_MSC_VER >= 9300))

#define SPmtString string

#else

#define PMT_USING_PTSTRING
#include "ptstring.h"
typedef ptstring < char > SPmtString;

// Since we're starting use safe features, we use the GNU CC versions in
// our code and must translate when on Windows.
#ifdef _MSC_VER
#ifndef snprintf
#define snprintf sprintf_s
#endif
#endif


#if defined(EK_EXPLICIT)
#ifdef _MSC_VER
#pragma warning (disable:4231)
#endif
EK_TMPL_EXT template class EK_DECL ptstring < char >;
#endif

#endif 

/*! \file
  \brief Functions to support miscellaneous <SPmtString> manipulation.

  These functions support for miscellaneous <SPmtString> functions, such 
  as conversion of wide char strings to ASCII strings, and vice-versa. Use 
  the EKL macro to control casting of <SPmtString> literals i.e., EKL("literal") 
  under UNICODE will produce L"literal" 
 
  For some inexplicable reason, declaring ostream "<<" or istream ">>" 
  operators as dllexport does not work.  The code compiles, but these operators
  do not show up in the list of exports for the DLL.  The solution is to 
  implement "Put" and "Get" functions which the inlined operators then call.
  The "Put" and "Get" functions cannot return the stream.  If they do, then the
  exporting of them is also ignored.
*/

#if defined(_UNICODE)

typedef wstring EkString;

#if !defined(EK_STR2WIDE)
#define EK_STR2WIDE(str) EkAToW( str )
#endif

#if !defined(EK_WIDE2STR)
#define EK_WIDE2STR(str) EkWToA( str )
#endif

#if !defined(EK_L)
#define EK_L(x) L ## x
#endif

#include <iostream>
#else

typedef SPmtString EkString;

#endif

#if !defined(EK_L)
#define EK_L(x) x
#endif

#if !defined(EK_STR2WIDE)
#define EK_STR2WIDE(str) str
#endif

#if !defined(EK_WIDE2STR)
#define EK_WIDE2STR(str) str
#endif

/*! 
  \brief Writes a wide string to an output stream
  \param stream The handle to the output stream
  \param ws The handle to the wide string to be written out
*/
void EK_DECL EkPutWString(ostream & stream, const wstring & ws);

/*! 
  \brief Reads a wide string from an input stream
  \param stream The handle to the input stream
  \param ws The handle to the wide string to be read into
*/
void EK_DECL EkGetWString(istream & stream, wstring & ws);


/*! 
  \brief Converts an ASCII string to a wide string
  \param str  The handle to the ASCII string to be converted
  \return wstring The resulting wide string
*/
wstring EK_DECL EkAToW(const SPmtString & str);

/*! 
  \brief  Converts a unicode string to ASCII
  \param str  The handle to the ASCII string to be converted
  \return wstring The resulting wide string

 Converts a unicode string to ASCII using operating system dependent
 functions, which may depend on current locale settings.

 By default, any character which cannot be converted from unicode
 to 8-bit ASCII will be replaced by '?'.  If you'd like to use a
 different character for these conversion failures, pass it as 
 the second argument.

 If you don't want substitution characters used when a conversion
 error occurs, you may pass a null-character as the second
 argument ('\0').  In that case, if a conversion error occurs,
 the conversion procedure stops and EkWTOA returns the third
 argument to the function as a string.

 Examples:
 \code
      EkWTOA (L"ABC") 				returns the string "ABC"
      EkWTOA (L"A\400C")  			returns the string "A?C"
      EkWTOA (L"A\400C",'.')  		returns the string "A.C"
      EkWTOA (L"A\400C",'\0') 		returns the string ""
      EkWTOA (L"A\400C",'\0',"DARN!") returns the string "DARN!"
 \endcode
*/
SPmtString EK_DECL EkWToA(const wstring & ws, char f = '?', const char *e = 0);

#endif

