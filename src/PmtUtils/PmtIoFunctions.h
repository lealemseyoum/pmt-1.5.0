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
 * Creation Date: 11/02/2000
 *
 * Original Author: 
 * George Sotak  george.sotak@kodak.com 
 * 
 * Contributor(s): 
 */

#ifndef PMT_IO_FUNCTIONS_H
#define PMT_IO_FUNCTIONS_H

#include "EkCompiler.h"
#include "EkRational.h"
#include "EkStrings.h"

#define PMT_SCHEMA_TRUE     "true"
#define PMT_SCHEMA_FALSE    "false"

/** \file 
    \brief Miscellaneous function to support metadata I/O.
*/


/** \name Primitive Value Type I/O
 * Functions to perform I/O of metadata values with primitive types
 */
//@{
/** \brief Write out an <b>unsigned short</b> value to an output stream */
inline ostream & PmtPrintValue(ostream & c, const unsigned short &v)
{
    return c << v;
}

/** \brief Write out a \b short value to an output stream */
inline ostream & PmtPrintValue(ostream & c, const short &v)
{
    return c << v;
}

/** \brief Write out an <b>unsigned long</b> value to an output stream */
inline ostream & PmtPrintValue(ostream & c, const unsigned long &v)
{
    return c << v;
}

/** \brief Write out a \b long value to an output stream */
inline ostream & PmtPrintValue(ostream & c, const long &v)
{
    return c << v;
}

/** \brief Write out an <b>unsigned int</b> value to an output stream */
inline ostream & PmtPrintValue(ostream & c, const unsigned int &v)
{
    return c << v;
}

/** \brief Write out an \b int value to an output stream */
inline ostream & PmtPrintValue(ostream & c, const int &v)
{
    return c << v;
}

/** \brief Write out an \b float value to an output stream */
inline ostream & PmtPrintValue(ostream & c, const float &v)
{
    return c << v;
}

/** \brief Write out an \b double value to an output stream */
inline ostream & PmtPrintValue(ostream & c, const double &v)
{
    return c << v;
}

/** \brief Write out an \b SPmtString value to an output stream */
inline ostream & PmtPrintValue(ostream & c, const SPmtString & v)
{
    return c << v;
}

#ifdef PMT_USING_PTSTRING
/** \brief Write out an \b string value to an output stream */
inline ostream & PmtPrintValue(ostream & c, const string & v)
{
    return c << v;
}
#endif

/** \brief Write an <b>unsigned char</b> value to an output stream 

    Override iostream implementation for unsigned char and char. Read/write as 
    numbers rather than letters (e.g. '65' rather than 'A').
*/
inline ostream & PmtPrintValue(ostream & c, const unsigned char &uc)
{
    unsigned short s = uc;
    return c << s;
}


/** \brief Write a \b char value to an output stream 

    Override iostream implementation for unsigned char and char. Read/write as 
    numbers rather than letters (e.g. '65' rather than 'A').
*/
inline ostream & PmtPrintValue(ostream & c, const char &uc)
{
    short s = uc;
    return c << s;
}


/** \brief Write a <b> wide string </b> value to an output stream

 This code must be enhanced, non-ascii characters are not handled properly.
<pre>
     - PmtPrintValue -- currently writes non-ascii chars as '?'
     - PmtReadValue -- will need to be modified as per any changes to
                       PmtPrintValue
</pre>
*/
inline ostream & PmtPrintValue(ostream & stream, const wstring & ws)
{
    SPmtString s = EkWToA(ws);
    return PmtPrintValue(stream, s);
}


/** \brief Write out a \b boolean value to an output stream */
inline ostream & PmtPrintValue(ostream & c, const bool & v)
{
    if (v)
        return c << PMT_SCHEMA_TRUE;
    else
        return c << PMT_SCHEMA_FALSE;
}


/*!\brief  Output to a stream a value of type EkRational
  \param c The output stream
  \param v The value with type EkRational
*/
template < class TYPE >
    inline ostream & PmtPrintValue(ostream & c, const EkRational < TYPE > &v)
{
    return c << v;
}


/*!\brief  Output to a stream a vector of values
  \param c The output stream
  \param v The vector of values
*/
template < class TYPE >
    inline ostream & PmtPrintValue(ostream & c, const vector < TYPE > &v)
{
    int i = v.size();

    for (int j = 0; j < i; j++)
    {
        // Only put spaces between items - not before first item, or after last item.
        if (j != 0)
            c << " ";
        PmtPrintValue(c, v[j]);
    }

    return c;
}




// Side effect: all whitespace is eliminated at the front of the stream
inline bool PmtStreamIsNegative(istream & s)
{
    char c;
    while (s.good() && (s.peek() <= 32))
        s.get(c);               // remove leading white space
    return s.peek() == '-';     // only negative numbers have a minus sign!
}

/** \brief Read in an <b>unsigned short</b> value from an input stream */
inline istream & PmtReadValue(istream & c, unsigned short &v)
{
    unsigned short x;
    v = 0;
    return PmtStreamIsNegative(c) ? c >> x : c >> v;
}

/** \brief Read in an \b short value from an input stream */
inline istream & PmtReadValue(istream & c, short &v)
{
    return c >> v;
}

/** \brief Read in an <b>unsigned long</b> value from an input stream */
inline istream & PmtReadValue(istream & c, unsigned long &v)
{
    unsigned long x;
    v = 0;
    return PmtStreamIsNegative(c) ? c >> x : c >> v;
}

/** \brief Read in an \b long value from an input stream */
inline istream & PmtReadValue(istream & c, long &v)
{
    return c >> v;
}

/** \brief Read in an <b>unsigned int</b> value from an input stream */
inline istream & PmtReadValue(istream & c, unsigned int &v)
{
    unsigned int x;
    v = 0;
    return PmtStreamIsNegative(c) ? c >> x : c >> v;
}

/** \brief Read in an \b int value from an input stream */
inline istream & PmtReadValue(istream & c, int &v)
{
    return c >> v;
}

/** \brief Read in an \b float value from an input stream */
inline istream & PmtReadValue(istream & c, float &v)
{
    return c >> v;
}

/** \brief Read in an \b double value from an input stream */
inline istream & PmtReadValue(istream & c, double &v)
{
    return c >> v;
}


/** \brief Read an <b>unsigned char</b> value from an input stream 

    Override iostream implementation for unsigned char and char. Read/write as 
    numbers rather than letters (e.g. '65' rather than 'A').
*/
inline istream & PmtReadValue(istream & c, unsigned char &uc)
{
    unsigned short s;
    c >> s;
    uc = (unsigned char) s;
    return c;
}


/** \brief Read a \b char value from an input stream 

    Override iostream implementation for unsigned char and char. Read/write as 
    numbers rather than letters (e.g. '65' rather than 'A').
*/
inline istream & PmtReadValue(istream & c, char &uc)
{
    short s;
    c >> s;
    uc = (char) s;
    return c;
}


/** \brief Read in an \b SPmtString value from an input stream */
inline istream & PmtReadValue(istream & c, SPmtString & v)
{
    v = "";
    bool valid = false;
    char ch;

    while (c.read(&ch, 1))
    {
        valid = true;
        v += ch;
    }

    // We read in a value, but (in order to terminate the above while-loop)
    // the stream is in an invalid state (failbit & badbit are probably set).
    // Make sure that the caller considers the stream to be good, so that 
    // they know that `v' is valid.
    if (valid)
        c.clear();

    return c;
}

#ifdef PMT_USING_PTSTRING
/** \brief Read in an \b string value from an input stream */
inline istream & PmtReadValue(istream & c, string & v)
{
    v = "";
    bool valid = false;
    char ch;

    while (c.read(&ch, 1))
    {
        valid = true;
        v += ch;
    }

    // We read in a value, but (in order to terminate the above while-loop)
    // the stream is in an invalid state (failbit & badbit are probably set).
    // Make sure that the caller considers the stream to be good, so that 
    // they know that `v' is valid.
    if (valid)
        c.clear();

    return c;
}
#endif

/** \brief Read in a <b>wide string</b> value from an input stream */
inline istream & PmtReadValue(istream & stream, wstring & ws)
{
    SPmtString s;
    PmtReadValue(stream, s);
    ws = EkAToW(s);
    return stream;
}


/** \brief Read in a \b boolean value from an input stream */
inline istream & PmtReadValue(istream & c, bool & v)
{
    SPmtString s;
    if ((c >> s) && (s == PMT_SCHEMA_TRUE))
        v = true;
    else
        v = false;
    return c;
}


/*!\brief  Input from a stream a value of type EkRational
  \param c The input stream
  \param v The value with type EkRational
*/
template < class TYPE >
    inline istream & PmtReadValue(istream & c, EkRational < TYPE > &v)
{
    return c >> v;
}


/*!\brief  Input from a stream a vector of values
  \param c The input stream
  \param v The vector of values
*/
template < class TYPE >
    inline istream & PmtReadValue(istream & c, vector < TYPE > &v)
{
    v.clear();

    TYPE d;

    while (PmtReadValue(c, d))
    {
        v.push_back(d);
    }

    return c;
}


//@}


#endif

