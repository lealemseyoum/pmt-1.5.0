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
 * 
 * Contributor(s): 
 * Hovik Melikyan hovik@melikyan.com
 * Sam Fryer    samuel.fryer@kodak.com
*/

// This ptstring class originated from the PTypes library with the given 
// information below.  It was later modified extensively for inclusion 
// to the PMT by Sam Fryer.  


/* 
 *  LICENSE
 *
 *
 *  C++ Portable Types Library (PTypes)
 *  Version 1.7.0  Released 11-Oct-2002
 *  
 *  Copyright (c) 2001, 2002 Hovik Melikyan
 *
 *  http://www.melikyan.com/ptypes/
 *  http://ptypes.sourceforge.net/
 *
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the author be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 *
 *
 *  Hovik Melikyan
 *  hovik@melikyan.com
 *
 *
 *  C++ Portable Types Library (PTypes)
 *  Version 1.7.0  Released 11-Oct-2002
 *
 *  Copyright (c) 2001, 2002 Hovik Melikyan
 *
 *  http://www.melikyan.com/ptypes/
 *  http://ptypes.sourceforge.net/
 *
 */

#ifndef __PTSTRING_H__
#define __PTSTRING_H__

#include <iostream>

#ifdef _MSC_VER
#include <istream>
#endif

#ifdef __GNUC__
#include <wchar.h>
#include <string.h>
#endif

// Code copied from EkCompiler for DLL builds.
// Some of the many reasons it was copied were
// to stop circular includes and allow this class
// to be used independently of the rest of PMT.
#ifdef _MSC_VER
/* Windows platforms */
#if    (defined EK_BUILD_DLL)
#define PTSTRING_DECL __declspec(dllexport)
#elif  (defined EK_DLL)
#define PTSTRING_DECL __declspec(dllimport)
#endif

#define PTSTRING_EXPLICIT

#endif                          /*windows */

#ifndef PTSTRING_METHOD_DECL
#define PTSTRING_METHOD_DECL
#endif

#ifndef PTSTRING_DECL
#define PTSTRING_DECL
#endif

#ifndef PTSTRING_TMPL_EXT
#define PTSTRING_TMPL_EXT
#endif


// -------------------------------------------------------------------- //
// ---  ptstring class ------------------------------------------------ //
// -------------------------------------------------------------------- //


// SMF -- Note: right now, this is based completely on c strings.  I've 
// attempted to generalize the interface a little to hide this so that 
// in the future this could have the generic c string 
// function calls replaced (in anticipation of wide strings, etc.)


#ifdef __GNUC__
// Apples, and other machines that use weak symbols, have problems with GCC
// and pragmas.
#if !defined ( __APPLE_CC__ )
#pragma interface
#endif
#endif

template < class data_type > class PTSTRING_DECL ptstring
{
  public:
    // The types we'll use 
    typedef size_t size_type;

    // static error value
    PTSTRING_METHOD_DECL static const size_type npos;

  private:

    // This header is appended to the beginning of all "data" allocations
    // Note that length is the number of characters -- NOT the length in bytes
    struct _strrec
    {
        size_type refcount;
        size_type length;
    };
    typedef _strrec *_pstrrec;


#define STR_BASE(x)      (_pstrrec(x)-1)
#define STR_REFCOUNT(x)  (STR_BASE(x)->refcount)
#define STR_LENGTH(x)    (STR_BASE(x)->length)


  public:

    ptstring()
    {
        data = getEmptyStr();
    }
    ptstring(const data_type * sc, size_type initlen)
    {
        initialize(sc, initlen);
    }
    ptstring(const ptstring & sc, size_type initlen)
    {
        initialize(sc.c_str(), initlen);
    }
    ptstring(const data_type * sc)
    {
        initialize(sc);
    }
    ptstring(data_type c)
    {
        initialize(c);
    }
    ptstring(const ptstring & s)
    {
        initialize(s);
    }
    ptstring(const ptstring & s, size_type p, size_type len)
    {
        ptstring s1 = s.substr(p, len);
        initialize(s1);
    }
    ptstring(size_type x, data_type f)
    {
        initialize(f, x);
    }
    ptstring(const data_type * s1, size_type len1, const data_type * s2,
             size_type len2)
    {
        initialize(s1, len1, s2, len2);
    }


    ~ptstring()
    {
        finalize();
    }

    static const size_type strrecsize;

    inline void reserve ( size_type amount )
    {
        _alloc( amount );
    }
    
    inline void resize ( size_type amount )
    {
        _realloc( amount );
    }
    
    inline size_type length(const ptstring & s)
    {
        return STR_LENGTH(s.data);
    }
    inline size_type length() const
    {
        return STR_LENGTH((const data_type *) data);
    }

    inline size_type size() const
    {
        return length() * sizeof(data_type);
    }

    inline size_type refcount(const ptstring & s)
    {
        return STR_REFCOUNT(s.data);
    }
    inline void assign(ptstring & s, const data_type * buf, size_type len)
    {
        s.assign(buf, len);
    }
    inline void clear(ptstring & s)
    {
        s.finalize();
    }

    inline bool empty() const
    {
        return length() == 0;
    }
    inline bool isempty(const ptstring & s)
    {
        return s.empty();
    }


    inline const data_type *c_str() const
    {
        return (const data_type *) data;
    }

    inline size_type find(const ptstring & s) const
    {
        return this->pos(s);
    }
    inline size_type find(const ptstring & s, size_type p) const
    {
        ptstring s1 = copy(p, length() - p);
        size_type pp = s1.pos(s);
         return (pp == npos) ? npos : pp + p;
    }

    inline size_type find_first_not_of(const ptstring & s) const
    {
        size_type p = npos, i = 0;
        for (i = 0; (i < s.length()); i++)
            if ((p = s.pos((*this)[i])) == npos)
                 break;
         return i;
    }


    inline size_type find_first_of(const ptstring & s, size_type np=0) const
    {
        size_type pp = npos, p = npos, i = 0;
        for (i = 0; i < s.length(); i++)
            if ((p = pos(s[i],np)) < pp)
                 pp = p;
         return pp;
    }

    inline size_type find_last_of(const data_type c) const
    {
        size_type i = length();
        while ((i > 0) && ((const ptstring) (*this)[--i] != c));
        return ((*this)[i] == c) ? i : npos;
    }

    // a very ineffecient way to do this....
    // (so we'll scrunch it up and hide it in one line of code)
    inline size_type replace(size_type p, size_type cnt, const ptstring & s)
    {
        ptstring s1(s, cnt);
        del(*this, p, cnt);
        ins(s1, *this, p);
        return p;
    }

    inline bool compare(const ptstring & s) const
    {
        return (*this == s);
    };

    // Hack for GCC compatibility
    inline bool compare(const data_type * s, size_type theStart,
                        size_type theLength)
    {
        return compare(theStart, theLength, s);
    }

    inline bool compare(size_type theStart, size_type theLength,
                        const data_type * s)
    {
        ptstring a = substr(theStart, theLength);
        ptstring b(s, theLength);
        return a == b;
    }

    inline ptstring substr(size_type position, size_type cnt) const
    {
        size_type a = (position <= length())? position : length();
        size_type b = (cnt <= length() - a) ? cnt : length() - a;
        if (cnt == 1)
             return ptstring((*this)[a]);
         return copy(a, b);
    }

    inline ptstring substr(size_t x) const
    {
        return substr(x, length() - x);
    }

    inline void append(const ptstring & s1)
    {
        concat(*this,s1);
    };
    
    inline const void * begin()
    {
        return data;
    }
    
    inline const void * end()
    {
        return (data + (length()*sizeof(data_type)));
    }
    
    // Routines to compare the strings
    inline bool operator <(const ptstring & s) const
    {
        return ptstrcmp(data, s.data) < 0;
    }
    inline bool operator >(const ptstring & s) const
    {
        return ptstrcmp(data, s.data) > 0;
    }
    inline bool operator <=(const ptstring & s) const
    {
        return ptstrcmp(data, s.data) <= 0;
    }
    inline bool operator >=(const ptstring & s) const
    {
        return ptstrcmp(data, s.data) >= 0;
    }

    inline ptstring & operator=(const data_type * sc)
    {
        assign(sc);
        return *this;
    }
    inline ptstring & operator=(data_type c)
    {
        assign(c);
        return *this;
    }
    inline ptstring & operator=(const ptstring & s)
    {
        assign(s);
        return *this;
    }
    inline ptstring & operator+=(const data_type * sc)
    {
        concat(*this, sc);
        return *this;
    }
    inline ptstring & operator+=(data_type c)
    {
        concat(*this, c);
        return *this;
    }
    inline ptstring & operator+=(const ptstring & s)
    {
        concat(*this, s);
        return *this;
    }

    inline ptstring operator+(const data_type * sc) const
    {
        if (length() == 0)
            return ptstring < data_type > (sc);
        else
            return ptstring < data_type > (data, length(), sc,
                                           ptstrlen(sc));
    }

    PTSTRING_METHOD_DECL ptstring operator+(data_type c) const;
    PTSTRING_METHOD_DECL ptstring operator+(const ptstring & s) const;

    inline bool operator==(const data_type * sc) const
    {
        return ptstrcmp(data, sc) == 0;
    }
    PTSTRING_METHOD_DECL bool operator==(data_type) const;
    PTSTRING_METHOD_DECL bool operator==(const ptstring &) const;
    inline bool operator!=(const data_type * sc) const
    {
        return !(*this == sc);
    }
    inline bool operator!=(data_type c) const
    {
        return !(*this == c);
    }
    inline bool operator!=(const ptstring & s) const
    {
        return !(*this == s);
    }

// should we be checking bounds?
#ifdef PT_CHECK_BOUNDS
    data_type & operator[] (size_type i);
#else
    data_type & operator[] (const size_type i)
    {
        return this->unique()[i];
    }
    const data_type & operator[] (const size_type i) const
    {
        return ((ptstring *) this)->unique()[i];
    }
#endif

// if std strings are in play, let's bring them in
#if defined (__STRING__) || defined (_STRING_) || defined (_CPP_STRING) || defined(_GLIBCXX_STRING)
    ptstring(const std::string & s);
    ptstring & operator=(const std::string & s);
    operator std::string() const;
#endif

  private:

    PTSTRING_METHOD_DECL data_type *getEmptyStr();

    data_type *data;

    PTSTRING_METHOD_DECL void _alloc(size_type);
    PTSTRING_METHOD_DECL void _realloc(size_type);
    PTSTRING_METHOD_DECL void _free();

    PTSTRING_METHOD_DECL void initialize(const data_type *, size_type);
    PTSTRING_METHOD_DECL void initialize(const data_type, size_type);
    PTSTRING_METHOD_DECL inline void initialize(const data_type * sc)
    {
        initialize(sc, sc ? ptstrlen(sc) : 0);
    }
    PTSTRING_METHOD_DECL void initialize(data_type);
    PTSTRING_METHOD_DECL void initialize(const ptstring & s);
    PTSTRING_METHOD_DECL void initialize(const data_type *, size_type,
                                         const data_type *, size_type);
    PTSTRING_METHOD_DECL void finalize();

    PTSTRING_METHOD_DECL void assign(const data_type *, size_type);
    inline void assign(const data_type * sc)
    {
        if (sc != NULL)
            assign(sc, ptstrlen(sc));
        else
            this->finalize();
    }
    PTSTRING_METHOD_DECL void assign(const ptstring &);
    PTSTRING_METHOD_DECL void assign(data_type);

    PTSTRING_METHOD_DECL data_type *unique();
    PTSTRING_METHOD_DECL void setlength(size_type newlen);
    PTSTRING_METHOD_DECL void concat(ptstring & s, const data_type * sc,
                                     size_type catlen);
    inline void concat(ptstring & s, const data_type * sc)
    {
        if (sc != NULL)
            concat(s, sc, ptstrlen(sc));
    }

    PTSTRING_METHOD_DECL void concat(ptstring & s, data_type s1);
    PTSTRING_METHOD_DECL void concat(ptstring & s, const ptstring & s1);
    PTSTRING_METHOD_DECL ptstring copy(size_type from, size_type cnt) const;
    PTSTRING_METHOD_DECL void ins(const ptstring & s1, ptstring & s,
                                  size_type at);
    PTSTRING_METHOD_DECL void del(ptstring & s, size_type at, size_type cnt);
    PTSTRING_METHOD_DECL size_type pos(const data_type * s1, size_type at = 0) const;
    PTSTRING_METHOD_DECL size_type pos(data_type s1, size_type at = 0) const;
    inline size_type pos(const ptstring & s, size_type at = 0) const
    {
        return pos(s.data,at);
    }
    PTSTRING_METHOD_DECL size_type rpos(data_type s1) const;

    // Overloaded basic string functions -- need to add your own if 
    // you plan on adding different types 
    inline int ptstrcmp(const char *d1, const char *d2) const
    {
        return strcmp(d1, d2);
    }
    inline int ptstrcmp(const wchar_t * d1, const wchar_t * d2) const
    {
        return wcscmp(d1, d2);
    }
    inline size_type ptstrlen(const char *d) const
    {
        return strlen(d);
    }
    inline size_type ptstrlen(const wchar_t * d) const
    {
        return wcslen(d);
    }
};


// Let's allow a little redirection
template < class data_type >
    inline ptstring < data_type > &operator<<(ptstring < data_type > &s,
                                              const ptstring < data_type >
                                              &rhs)
{
    s += rhs;
    return s;
}

template < class data_type >
    inline ptstring < data_type > &operator<<(ptstring < data_type > &s,
                                              const data_type * rhs)
{
    s += rhs;
    return s;
}

template < class data_type >
    inline ptstring < data_type > &operator<<(ptstring < data_type > &s,
                                              const data_type rhs)
{
    s += rhs;
    return s;
}

// Adding strings to a character is always fun....
template < class data_type >
    inline ptstring < data_type > operator+(data_type c,
                                            const ptstring < data_type > &s)
{
    if (s.length() == 0)
        return ptstring < data_type > (c);
    else
        return ptstring < data_type > (&c, 1, s.c_str(), s.length());
}

// ... As is adding strings to a character string
template < class data_type >
    inline ptstring < data_type > operator+(const data_type * sc,
                                            const ptstring < data_type > &s)
{
    if (s.length() == 0)
        return ptstring < data_type > (sc);
    else if (sc == NULL)
        return s;
    else
        return ptstring < data_type > (sc, strlen(sc), s.c_str(), s.length());
}

// And finally, some stream operators for good measure
#if defined (_ISTREAM_) || defined (__ISTREAM__) || defined(_IOSTREAM_H) || defined(_CPP_ISTREAM) || defined(_GLIBCXX_ISTREAM) || defined(_GLIBCXX_IOSTREAM) 
// Does istream support wide characters?  What about a max strength length > 1024????
template < class data_type >
    inline std::istream & operator >>(std::istream & c,
                                      ptstring < data_type > &s)
{
    data_type x[1024];
    c >> x;
    s = x;
    return c;
}
#endif
#if defined (_OSTREAM_) || defined (__OSTREAM__) || defined(_IOSTREAM_H) || defined(_CPP_OSTREAM) || defined(_GLIBCXX_OSTREAM) || defined(_GLIBCXX_IOSTREAM)
// Does ostream support all the data types?
template < class data_type >
    inline std::ostream & operator <<(std::ostream & c,
                                      const ptstring < data_type > &s)
{
    c << s.c_str();
    return c;
};

#endif


// __PTSTRING_H__
#endif

