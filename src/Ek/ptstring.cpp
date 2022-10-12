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

// included for Windows string compatibility....
#include <string>

// other necessary includes
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ptstring.h"

namespace std { } ;
using namespace std;

#ifdef __GNUC__
// Apples, and other machines that use weak symbols, have problems with GCC
// and pragmas.
#if !defined ( __APPLE_CC__ )
#pragma implementation
#endif
#endif

template < class data_type >
    const typename ptstring < data_type >::size_type
    ptstring < data_type >::strrecsize = sizeof(_strrec);
template < class data_type >
    const typename ptstring < data_type >::size_type
    ptstring < data_type >::npos = static_cast<typename ptstring < data_type >::size_type>(-1);

const int quant = 64;
const int qmask = ~63;

const int quant2 = 4096;
const int qmask2 = ~4095;

// by making this an int, we solve some alignment issues some 
// platforms may have.  By making this a "static const", we solve
// some data initialization issues other platforms may have.
static const int _ptstring_emptyStr[10] = {0,0,0,0,0,0,0,0,0,0};

#define imin(x,y) (x<y)?x:y

#if defined(__GNUC__)
#  if defined(__i386__) || defined(__I386__) || defined(__x86_64__)
#    define GCC_i386
#  else
#    include "EkCompiler.h"
#    include "EkLocking.h"
EkMutex gPtstringMutex;         //global mutex
#  endif
#elif ( defined(_MSC_VER) || defined(__BORLANDC__) ) && defined(_M_IX86)
#  define MSC_i386
#else
#include "EkCompiler.h"
#include "EkLocking.h"
EkMutex gPtstringMutex;         // global mutex
#endif

#ifdef _MSC_VER
// MSVC doesn't like the fact that we don't have return statements from the
// inline assembly.  So we tell it to be quiet for once.
#pragma warning(disable:4035)
#endif

int pincrement(size_t * target)
{
#if defined(MSC_i386) || defined(_M_IX86)
    __asm
    {
        mov ecx,target
        mov eax, 1;
        lock xadd[ecx], eax;
    }
#elif defined(GCC_i386)
    int temp = 1;
    __asm__ __volatile("lock ; xaddl %0,(%1)":"+r"(temp):"r"(target));
#else
    EK_GUARD(EK_DEFAULT_MUTEX, gPtstringMutex);
    ++(*target);
#endif
    return *target;
}


int pdecrement(size_t * target)
{
#if defined(MSC_i386) || defined(_M_IX86)
    __asm
    {
        mov ecx,target
        mov eax, -1;
        lock xadd[ecx], eax;
    }
#elif defined(GCC_i386)
    int temp = -1;
    __asm__ __volatile("lock ; xaddl %0,(%1)":"+r"(temp):"r"(target));
#else
    EK_GUARD(EK_DEFAULT_MUTEX, gPtstringMutex);
    --(*target);
#endif
    return *target;
}

int pdoubledecrement(size_t * target)
{
#if defined(MSC_i386) || defined(_M_IX86)
    __asm
    {
        mov ecx,target
        mov eax, -2;
        lock xadd[ecx], eax;
    }
#elif defined(GCC_i386)
    int temp = -2;
    __asm__ __volatile("lock ; xaddl %0,(%1)":"+r"(temp):"r"(target));
#else
    EK_GUARD(EK_DEFAULT_MUTEX, gPtstringMutex);
    --(*target);
    --(*target);
#endif
    return *target;
}

#ifdef _MSC_VER
// Now turn back on the warnings for everyone else.
#pragma warning(default:4035)
#endif

static void fatal(int code = 21, char *msg = "String overflow")
{
#ifdef WANT_EXCEPTIONS
    throw(msg);
#else
    printf("PTSTRING ERROR: %d : %s\n",code,msg);
#endif
}


static size_t quantize(size_t numchars, size_t data_size, size_t rec_size)
{
    size_t numalloc = (numchars + 1) * data_size + rec_size;
    if (numalloc <= 16)
        return 16;
    if (numalloc <= 32)
        return 32;
    else if (numalloc <= 2048)
        return (numalloc + quant - 1) & qmask;
    else
        return (numalloc + quant2 - 1) & qmask2;
}

template < class data_type >
    PTSTRING_METHOD_DECL data_type * ptstring < data_type >::getEmptyStr()
{ 
    //return (data_type *) &_ptstring_emptyStr[sizeof(struct _strrec)+1];
    // we pick 5 here just for some room in either direction.  If 
    // _strrec is changed, this may need to be revisited.
    return (data_type*)(void*)&_ptstring_emptyStr[5];
}

template < class data_type > PTSTRING_METHOD_DECL
    void ptstring < data_type >::_alloc(const size_type numchars)
{
    // Make sure we're not requesting too many here
    if (numchars == npos)
        fatal(21, "Attempted to malloc npos bytes.");

    size_t a = quantize(numchars, sizeof(data_type), strrecsize);
    data = (data_type *) (malloc(a));

    // make sure malloc didn't return NULL (0)
    if (data == 0)
        fatal();

    // set the header variables
    ((_strrec *) data)->refcount = 1;
    ((_strrec *) data)->length = numchars;

    // move the pointer to only point at the character data (not the header)
    data = &data[strrecsize / sizeof(data_type)];

    data[numchars] = 0;
}


template < class data_type >
    PTSTRING_METHOD_DECL void ptstring <
    data_type >::_realloc(const size_type numchars)
{
    if (numchars <= 0 || STR_LENGTH(data) <= 0)
        fatal();
    size_type a = quantize(numchars, sizeof(data_type), strrecsize);
    size_type b = quantize(STR_LENGTH(data), sizeof(data_type), strrecsize);
    if (a != b)
    {
        data_type *olddata = data;
        this->_alloc(numchars);
        memcpy(data, olddata,
               imin(numchars, STR_LENGTH(olddata)) * sizeof(data_type));

        // see what we need to do with the old data
        if (pincrement(&STR_REFCOUNT(olddata)) > 2)
        {
            // Someone else is also using this buffer ... we need to 
            // subtract 2 from the refcount (1 for the pincrement we
            // just did and 1 for our original reference)
            pdoubledecrement(&STR_REFCOUNT(olddata));
        }
        else                    // just free it
        if (olddata != getEmptyStr())
            free((data_type *) (STR_BASE(olddata)));

// GCC had a problem with realloc....            
//         data = (data_type*)(realloc(data - strrecsize, a)) + strrecsize;
    }
    // _alloc already did this for us?
    STR_LENGTH(data) = numchars;
    data[numchars] = 0;
}


template < class data_type >
    PTSTRING_METHOD_DECL void ptstring < data_type >::_free()
{
    if ((data) && (data != getEmptyStr()))
        free((data_type *) (STR_BASE(data)));
    data = getEmptyStr();
}


template < class data_type >
    PTSTRING_METHOD_DECL void ptstring <
    data_type >::initialize(const data_type * sc, size_type initlen)
{
    if (initlen <= 0 || sc == NULL)
        data = getEmptyStr();
    else
    {
        _alloc(initlen);
        memmove(data, sc, initlen * sizeof(data_type));
    }
}


template < class data_type >
    PTSTRING_METHOD_DECL void ptstring < data_type >::
    initialize(const data_type sc, size_type initlen)
{
    if (initlen <= 0)
        data = getEmptyStr();
    else
    {
        _alloc(initlen);
        data_type * tmpptr = data;
        for (size_type i = 0; i < initlen; i++, tmpptr++)
            *tmpptr = sc;
    }
}


template < class data_type >
    PTSTRING_METHOD_DECL void ptstring < data_type >::initialize(data_type c)
{
    _alloc(1);
    data[0] = c;
}


template < class data_type >
    PTSTRING_METHOD_DECL void ptstring <
    data_type >::initialize(const ptstring < data_type > &s)
{
    data = s.data;
    if (STR_LENGTH(data) != 0)
        pincrement(&STR_REFCOUNT(data));
}


template < class data_type >
    PTSTRING_METHOD_DECL void ptstring < data_type >::finalize()
{
    if (data)
        if (STR_LENGTH(data) != 0)
            if (pdecrement(&STR_REFCOUNT(data)) == 0)
                _free();
    data = getEmptyStr();
}


template < class data_type >
    PTSTRING_METHOD_DECL data_type * ptstring < data_type >::unique()
{
    if (STR_LENGTH(this->data) != 0)
    {
        // by incrementing the refcount for this object we "lock" the buffer
        // from being concurrently unique'd
        // if refcount > 2 it means it was actually > 1 before
        if (pincrement(&STR_REFCOUNT(this->data)) > 2)
        {
            ptstring < data_type > t;
            size_type len = STR_LENGTH(this->data);

            // the refcount is already incremented, so that we can assign 
            // the pointer directly
            t.data = this->data;

            // allocate and copy data
            this->_alloc(len);
            memcpy(this->data, t.data, len * sizeof(data_type));

            // we must decrement refcount for t since we detached 
            // its buffer 'illegally'
            pdecrement(&STR_REFCOUNT(t.data));

            // here the destructor for t is being called; the buffer may be
            // freed if ptstring buffer was detached concurrently
        }
        else
            // refcount was 1 before incrementing, so we can assign it directly
            // instead of costly pdecrement()
            STR_REFCOUNT(this->data) = 1;
    }

    return this->data;
}


template < class data_type >
    PTSTRING_METHOD_DECL void ptstring <
    data_type >::setlength(size_type newlen)
{
    size_type curlen = this->length();
    if (newlen < 0)
        return;

    // if becoming empty
    if (newlen == 0)
        this->finalize();

    // if otherwise s was empty before
    else if (curlen == 0)
        this->_alloc(newlen);

    // if length is not changing, return a unique ptstring
    else if (newlen == curlen)
        this->unique();

    // thread-safe reallocation; see comments in unique()
    else
    {
        if (pincrement(&STR_REFCOUNT(this->data)) > 2)
        {
            ptstring < data_type > t;
            t.data = this->data;
            this->_alloc(newlen);
            memcpy(this->data, t.data,
                   imin(curlen, newlen) * sizeof(data_type));
            pdecrement(&STR_REFCOUNT(t.data));
        }
        else
        {
            STR_REFCOUNT(this->data) = 1;
            this->_realloc(newlen);
        }
    }
}


template < class data_type >
    PTSTRING_METHOD_DECL void ptstring <
    data_type >::assign(const data_type * sc, size_type initlen)
{
    if (STR_LENGTH(data) > 0 && initlen > 0)
    {
        // thread-safe assignment with possible
        // reuse of the buffer
        if (pincrement(&STR_REFCOUNT(data)) > 2)
        {
            ptstring < data_type > t;
            t.data = data;
            _alloc(initlen);
            memcpy(data, sc, initlen * sizeof(data_type));
            pdecrement(&STR_REFCOUNT(t.data));
        }
        else
        {
            STR_REFCOUNT(data) = 1;
            _realloc(initlen);
            memmove(data, sc, initlen * sizeof(data_type));
        }
    }
    else
    {
        finalize();
        if (initlen == 1)
            initialize(sc[0]);
        else
            initialize(sc, initlen);
    }
}


template < class data_type >
    PTSTRING_METHOD_DECL void ptstring < data_type >::assign(data_type c)
{
    assign(&c, 1);
}


template < class data_type >
    PTSTRING_METHOD_DECL void ptstring < data_type >::assign(const ptstring <
                                                             data_type > &s)
{
    if (data != s.data)
    {
        finalize();
        initialize(s);
    }
}


template < class data_type >
    PTSTRING_METHOD_DECL void ptstring <
    data_type >::initialize(const data_type * s1, size_type len1,
                            const data_type * s2, size_type len2)
{
    if (len1 <= 0)
        initialize(s2, len2);
    else if (len2 <= 0)
        initialize(s1, len1);
    else
    {
        _alloc(len1 + len2);
        memcpy(data, s1, len1 * sizeof(data_type));
        memcpy(data + len1, s2, len2 * sizeof(data_type));
    }
}


#ifdef CHECK_BOUNDS

template < class data_type >
    PTSTRING_METHOD_DECL data_type & ptstring <
    data_type >::operator[](size_type i)
{
    this->unique();
    if (i < 0 || i >= length(*this))
        fatal(20, "String index overflow");
    return data[i];
}

#endif


template < class data_type >
    PTSTRING_METHOD_DECL void ptstring < data_type >::concat(ptstring <
                                                             data_type > &s,
                                                             const data_type *
                                                             sc,
                                                             size_type catlen)
{
    if (s.length() == 0)
        s.assign(sc, catlen);
    else if (catlen > 0)
    {
        size_type oldlen = s.length();

        // we must check this before calling setlength(), since
        // the buffer pointer may be changed during reallocation
        if (s.data == sc)
        {
            s.setlength(s.length() + catlen);
            memmove(s.data + oldlen, s.data, catlen * sizeof(data_type));
        }
        else
        {
            s.setlength(s.length() + catlen);
            memmove(s.data + oldlen, sc, catlen * sizeof(data_type));
        }
    }
}



template < class data_type >
    PTSTRING_METHOD_DECL void ptstring < data_type >::concat(ptstring <
                                                             data_type > &s,
                                                             data_type c)
{
    if (s.length() == 0)
        s.assign(c);
    else
    {
        s.setlength(s.length() + 1);
        s.data[s.length() - 1] = c;
    }
}


template < class data_type >
    PTSTRING_METHOD_DECL void ptstring < data_type >::concat(ptstring <
                                                             data_type > &s,
                                                             const ptstring <
                                                             data_type > &s1)
{
    if (s.length() == 0)
        s = s1;
    else if (s1.length() > 0)
        concat(s, s1.data, s1.length());
}


template < class data_type >
    PTSTRING_METHOD_DECL ptstring < data_type > ptstring <
    data_type >::operator+(data_type c) const
{
    if (length() == 0)
        return ptstring < data_type > (c);
    else
        return ptstring < data_type > (data, length(), &c, 1);
}


template < class data_type >
    PTSTRING_METHOD_DECL ptstring < data_type >
    ptstring < data_type >::operator+(const ptstring < data_type > &s) const
{
    if (length() == 0)
        return s;
    else if (s.length() == 0)
        return *this;
    else
        return ptstring < data_type > (data, length(), s.data, s.length());
}


template < class data_type >
    PTSTRING_METHOD_DECL bool
    ptstring < data_type >::operator==(const ptstring < data_type > &s) const
{
    return (length() == s.length())
        && ((length() == 0) ||
            (memcmp(data, s.data, length() * sizeof(data_type)) == 0));
}


template < class data_type >
    PTSTRING_METHOD_DECL bool ptstring <
    data_type >::operator==(data_type c) const
{
    return (length() == 1) && (data[0] == c);
}


template < class data_type >
    PTSTRING_METHOD_DECL ptstring < data_type >
    ptstring < data_type >::copy( typename ptstring < data_type >::size_type from,
                                  typename ptstring < data_type >::size_type cnt) const
{
    ptstring < data_type > t;
    if (length() > 0 && from >= 0 && from < length())
    {
        size_type l = imin(cnt, length() - from);
        if (from == 0 && l == length())
            t = *this;
        else if (l > 0)
        {
            t._alloc(l);
            memmove(t.data, data + from, l * sizeof(data_type));
            t.data[l] = 0;
        }
    }
    return t;
}


template < class data_type >
    PTSTRING_METHOD_DECL void ptstring < data_type >::ins(const ptstring <
    data_type > &s1, ptstring < data_type > &s,
    typename ptstring < data_type >::size_type at)
{
    size_type s1len = s1.length();
    size_type curlen = s.length();
    if (s1len > 0 && at >= 0 && at < curlen)
    {
        if (curlen == 0)
            s.assign(s1.data, s1len);
        else
        {
            s.setlength(curlen + s1len);
            size_type t = s.length() - at - s1len;
            data_type *p = s.data + at;
            if (t > 0)
                memmove(p + s1len, p, t * sizeof(data_type));
            memmove(p, s1.data, s1len * sizeof(data_type));
        }
    }
}


template < class data_type >
    PTSTRING_METHOD_DECL void ptstring < data_type >::del(ptstring <
                                                          data_type > &s,
                                                          size_type from,
                                                          size_type cnt)
{
    size_type l = s.length();
    if (from >= 0 && from < l && cnt > 0)
    {
        if (from + cnt >= l)
            cnt = l - from;
        else if (l - from > cnt)
        {
            s.unique();
            memmove(s.data + from, s.data + from + cnt,
                    (l - from - cnt) * sizeof(data_type));
        }
        s.setlength(l - cnt);
    }
}


template < class data_type >
    PTSTRING_METHOD_DECL typename ptstring < data_type >::size_type
    ptstring < data_type >::pos(const data_type * sc, size_type at) const
{
	data_type * t1 = data + at;
    const data_type *t = (data_type *) strstr(t1, sc);
    return (t == NULL ? npos : (t - data));
}

template<>
ptstring < wchar_t >::size_type ptstring < wchar_t >::pos(const wchar_t * sc, size_type at) const
{
	wchar_t * t1 = data + at;
    const wchar_t *t = (wchar_t *) wcsstr(t1, sc);
    return (t == NULL ? npos : (t - data) / sizeof(wchar_t));
}

template < class data_type >
    PTSTRING_METHOD_DECL typename ptstring < data_type >::size_type
    ptstring < data_type >::pos(data_type c, size_type at) const
{
	data_type * t1 = data + at;
    const data_type *t = (data_type *) strchr(t1, c);
    return (t == NULL ? npos : (t - data));
}

template<>
PTSTRING_METHOD_DECL ptstring < wchar_t >::size_type
    ptstring < wchar_t >::pos(wchar_t c, size_type at) const
{
	wchar_t * t1 = data + at;
    const wchar_t *t = (wchar_t *) wcschr(t1, c);
    return (t == NULL ? npos : (t - data) / sizeof(wchar_t));
}

template < class data_type >
    PTSTRING_METHOD_DECL typename ptstring < data_type >::size_type
    ptstring < data_type >::rpos(data_type c) const
{
    const data_type *t = (data_type *) strrchr(data, c);
    return (t == NULL ? npos : (t - data));
}

template<>
PTSTRING_METHOD_DECL ptstring < wchar_t >::size_type
    ptstring < wchar_t >::rpos(wchar_t c) const
{
    const wchar_t *t = (wchar_t *) wcsrchr(data, c);
    return (t == NULL ? npos : (t - data) / sizeof(wchar_t));
}

#if defined (__STRING__) || defined (_STRING_) || defined (_CPP_STRING) || defined(_GLIBCXX_STRING)
template < class data_type >
PTSTRING_METHOD_DECL ptstring < data_type >:: ptstring(const std::string & s)
{
    initialize(s.c_str());
}
template < class data_type >
PTSTRING_METHOD_DECL ptstring < data_type > & ptstring < data_type >::
    operator=(const std::string & s)
{
    assign(s.c_str());
    return *this;
}
template < class data_type >
PTSTRING_METHOD_DECL ptstring < data_type >:: operator std::string() const
{
    std::string st(this->c_str());
    return st;
}

template<>
PTSTRING_METHOD_DECL ptstring < wchar_t >:: ptstring(const std::string & s)
{
    if (s.length() <= 0)
        data = getEmptyStr();
    else
    {
        _alloc(s.length()+1);
        mbstowcs(data,s.c_str(),s.length());
    }
}
template<>
PTSTRING_METHOD_DECL ptstring < wchar_t > & ptstring < wchar_t >::
    operator=(const std::string & s)
{
    if (s.length() <= 0)
        data = getEmptyStr();
    else
    {
        _alloc(s.length()+1);
        mbstowcs(data,s.c_str(),s.length());
    }
    return *this;
}
template<>
PTSTRING_METHOD_DECL ptstring < wchar_t >:: operator std::string() const
{
    std::string st;
    if (this->length() > 0)
    {
        char * x = new char [this->length()*2];
        wcstombs(x,this->data,this->length());
        st = x;
        delete [] x;
    }
    return st;
}
#endif


// Instatiation of the template so the compiler will only put the code for it 
// in this file and not every file.
// Note that we have to instatiate some other functions as well ...

// Start with char
template class PTSTRING_DECL ptstring < char >;
//template class ptstring<char> operator+(char const *, ptstring<char> const &);
//template class ptstring<char> operator+(char const , ptstring<char> const &);

// ... and also wchar_t
template class PTSTRING_DECL ptstring < wchar_t >;
// //template class ptstring<wchar_t> operator+(wchar_t const *, 
// //                                           ptstring<wchar_t> const &);
// template class ptstring<wchar_t> operator+(wchar_t const , 
//                                            ptstring<wchar_t> const &);

