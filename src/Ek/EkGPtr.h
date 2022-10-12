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
 * Creation Date: 01/1999
 *
 * Original Author: 
 * George Sotak (original author is Thomas DeWeese) george.sotak@kodak.com
 * 
 * Contributor(s): 
 */


#ifndef EK_GPTR_H
#define EK_GPTR_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#if (defined _MSC_VER) && (!defined _WINDOWS_) && (!defined __AFXWIN_H__)
#include "windows.h"
#endif

#include <cstdlib>

//! The EkGPtr class is used for ensuring that objects are available at global initialization time
/*!
 Basically you can define a static
 data member of this type (templated on the desired object type).
 Then you can reference that 'pointer' at any time without concern
 for when/if global Ctors have been called for that object yet.

 Note: This class can _only_ be instantiated in global space (it
 relies on being initialized to zero...).
*/
template < class T > class EkGPtr
{
  public:
    //! This is the default constructor
    inline EkGPtr()
    {
    }
    //! This is the destructor
    ~EkGPtr();

    //! This is a dereferencing method
    inline operator    T *&()
    {
        return ptr();
    }
    //! This is a dereferencing method
    inline T *&operator->()
    {
        return ptr();
    }
    //! This is a dereferencing method
    inline T & operator*()
    {
        return *ptr();
    }

    //! This returns what <TYPE> is pointing to
    inline T *&ptr();

  private:
    T * mPtr;

    //! This is the copy constructor
    inline EkGPtr(const EkGPtr &)
    {
    }
    //! This is the assignment operator
    inline EkGPtr & operator=(EkGPtr &)
    {
    }
};


template < class T > class EkGSmartPtr
{
  public:
    //! This is the default constructor
    inline EkGSmartPtr()
    {
    }
    //! This is the destructor
    ~EkGSmartPtr();

    //! This is a dereferencing method
    inline operator    T *&()
    {
        return ptr();
    }
    //! This is a dereferencing method
    inline T *&operator->()
    {
        return ptr();
    }
    //! This is a dereferencing method
    inline T & operator*()
    {
        return *ptr();
    }

    //! This returns what <TYPE> is pointing to
    inline T *&ptr();

  private:
    T * mPtr;

    //! This is the copy constructor
    inline EkGSmartPtr(const EkGSmartPtr &)
    {
    }
    //! This is the assignment operator
    inline EkGSmartPtr & operator=(EkGSmartPtr &)
    {
    }
};

template < class T > EkGPtr < T >::~EkGPtr()
{
    delete mPtr;
    mPtr = NULL;
}

template < class T > T * &EkGPtr < T >::ptr()
{
    if (mPtr == NULL)
        mPtr = new T();
    return mPtr;
}

template < class T > EkGSmartPtr < T >::~EkGSmartPtr()
{                               // I was the last guy referencing it...
    if (mPtr && mPtr->deleteRef())
        delete mPtr;
    mPtr = NULL;
}

template < class T > T * &EkGSmartPtr < T >::ptr()
{
    if (mPtr == NULL)
    {
        mPtr = new T();
        mPtr->addRef();
    }
    return mPtr;
}

#endif                          // EK_GPTR_H

