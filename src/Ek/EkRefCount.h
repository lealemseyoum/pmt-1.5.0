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
 * Thomas DeWeese thomas.deweese@kodak.com
 * Copyright (C) 1997 Eastman Kodak Company
 *
 * Contributor(s): 
 * John Hall
 * Martin R. Hoffman martin.hoffman@kodak.com
 *
 * Date: 1997
 *
 */

#if !defined (EK_REFCOUNT_H)
#define EK_REFCOUNT_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#if !defined (EK_COMPILER_H)
#include "EkCompiler.h"
#endif

#if !defined (EK_BOOL_H)
#include "EkBool.h"
#endif

#include "EkLocking.h"

//! This provides the smart point reference counting functionality.
/*! The smart pointer implementation (EkSmartPtr) is built on the
 notion of reference counting objects.  This means that when someone
 new starts to point at an object they increment a use count variable
 in the object they are pointing at.  When they cease to point at that
 object they decrement the use count variable.  If the use count
 variable ever goes to zero the object should be deleted as it is
 no-longer being referenced by anyone.
 <p>
 Therefore, any class instance that is to be accessed through a smart
 pointer must support reference counting. The easiest way to obtain
 reference counting support is to have the class inherit from the
 EkRefCount class. In the present implementation, each instance of
 EkRefCount has its own mutex guard.
 <p>
 Usage:
 \code
 class SomeClass ;
 typedef EkSmartPtr<SomeClass>      SomeClassPtr;
 typedef EkConstSmartPtr<SomeClass> ConstSomeClassPtr;
 class SomeClass: public EkRefCount<EK_DEFAULT_MUTEX>
 {
   ....
 }
 \endcode
*/
template < class mutex_type > class EkRefCount
{
  public:
    //! Constructor simply sets useCount to zero.
    inline EkRefCount(void);

    //! Destructor. Does <b>not</b> throw an error if mUseCount != 0. See also, VEkRefCount.
    inline ~ EkRefCount(void);

    //! Copy constructor, does <b>NOT</b> copy useCount.
    inline EkRefCount(const EkRefCount & obj);

    //! Assignment operator, does <b>NOT</b> copy useCount.
    inline EkRefCount & operator=(const EkRefCount & obj);

    //! Increment useCount by one
    inline void addRef(void);

    //! Decrements useCount by one, returns true if the object should be deleted.
    inline bool deleteRef(void);

    //! This returns the useCount
    inline long useCount() const
    {
        return mUseCount;
    };

  private:
    //! The count of objects pointing at this object.
    // long only in 32 bit environments to support assembler
    long mUseCount;

#if (!defined (_M_IX86)) || !(defined (__GNUC__) && ( __GNUC__ > 2 ) && defined (i386))
    mutex_type mMutexLock;
#endif
};

//! Equivalent to EkRefCount; however, the destructor throws an error if the ref count != 0
template < class mutex_type > class VEkRefCount
{
  public:
    //! Constructor simply sets useCount to zero.
    inline VEkRefCount(void);

    //! Destructor. This throws an error if mUseCount != 0. See also, EkRefCount.
    virtual ~ VEkRefCount(void);

    //! Copy constructor, does <B>not</B> copy useCount.
    inline VEkRefCount(const VEkRefCount & obj);

    //! Assignment operator, does <B>not</B> copy useCount.
    inline virtual VEkRefCount & operator=(const VEkRefCount & obj);

    //! Increment useCount by one
    inline virtual void addRef(void);

    //! Decrements useCount by one, returns true if useCount==0 after decrement.
    inline virtual bool deleteRef(void);

    //! Returns the useCount
    inline long useCount() const
    {
        return mUseCount;
    };

  private:
    //! The count of objects pointing at this object.
    // long only in 32 bit environments to support assembler
    long mUseCount;

#if !defined (_M_IX86)
    mutex_type mMutexLock;
#endif
};

template < class mutex_type >
    inline EkRefCount < mutex_type >::EkRefCount(void):mUseCount(0)
{
}

template < class mutex_type >
    inline EkRefCount <
    mutex_type >::EkRefCount(const EkRefCount &):mUseCount(0)
{
}

template < class mutex_type >
    inline EkRefCount < mutex_type > &EkRefCount <
    mutex_type >::operator=(const EkRefCount < mutex_type > &)
{
    return *this;
}

template < class mutex_type >
    inline void EkRefCount < mutex_type >::addRef(void)
{
#if defined (_M_IX86)
    _asm
    {
        mov eax, this           // point to class
         lock inc[eax] this.mUseCount   // atomic increment the count
    }
#else
#if defined (__GNUC__) && ( __GNUC__ > 2 ) && defined (i386)
  asm(" lock incl %0 ":"=m"(mUseCount) : "m"(mUseCount) );
#else
    EK_GUARD(mutex_type, mMutexLock);
    mUseCount++;
#endif
#endif
}

template < class mutex_type >
    inline bool EkRefCount < mutex_type >::deleteRef(void)
{
    bool status;
#if defined (_M_IX86)
    _asm
    {
        mov eax, this           // point to class
         lock dec[eax] this.mUseCount   // atomic decrement the count
         setz status            // save state of "mUseCount == 0"
    }
#else
#if defined (__GNUC__) && ( __GNUC__ > 2 ) && defined (i386)
  asm(" lock decl %0 ; setz %1 ":"=m"(mUseCount), "=g"(status) : "m"(mUseCount));
#else
    EK_GUARD(mutex_type, mMutexLock);
    mUseCount--;
    status = (mUseCount == 0);
#endif
#endif
    return status;
}

template < class mutex_type > inline VEkRefCount < mutex_type >::VEkRefCount(void):mUseCount
    (0)
{
}

template < class mutex_type >
    inline VEkRefCount <
    mutex_type >::VEkRefCount(const VEkRefCount &):mUseCount(0)
{
}

template < class mutex_type >
    inline VEkRefCount < mutex_type > &VEkRefCount <
    mutex_type >::operator=(const VEkRefCount < mutex_type > &)
{
    return *this;
}

template < class mutex_type >
    inline void VEkRefCount < mutex_type >::addRef(void)
{
#if defined (_M_IX86)
    _asm
    {
        mov eax, this           // point to class
         lock inc[eax] this.mUseCount   // atomic increment the count
    }
#else
#if defined (__GNUC__) && ( __GNUC__ > 2 ) && defined (i386)
  asm(" lock incl %0 ":"=m"(mUseCount) : "m"(mUseCount));
#else
    EK_GUARD(mutex_type, mMutexLock);
    mUseCount++;
#endif
#endif
}

template < class mutex_type >
    inline bool VEkRefCount < mutex_type >::deleteRef(void)
{
    bool status;
#if defined (_M_IX86)
    _asm
    {
        mov eax, this           // point to class
         lock dec[eax] this.mUseCount   // atomic decrement the count
         setz status            // save state of "mUseCount == 0"
    }
#else
#if defined (__GNUC__) && ( __GNUC__ > 2 ) && defined (i386)
  asm(" lock decl %0 ; setz %1 ":"=m"(mUseCount), "=g"(status) : "m"(mUseCount));
#else
    EK_GUARD(mutex_type, mMutexLock);
    mUseCount--;
    status = (mUseCount == 0);
#endif
#endif
    return status;
}

template < class mutex_type > EkRefCount < mutex_type >::~EkRefCount(void)
{
    mUseCount = -1;
}

template < class mutex_type > VEkRefCount < mutex_type >::~VEkRefCount(void)
{
    mUseCount = -1;
}

#ifdef _MSC_VER
#pragma warning (disable:4231)
#endif

#if defined(EK_EXPLICIT)
#include "EkLocking.h"
#include "EkNullMutex.h"

EK_TMPL_EXT template class EK_DECL EkRefCount < EK_DEFAULT_MUTEX >;
EK_TMPL_EXT template class EK_DECL VEkRefCount < EK_DEFAULT_MUTEX >;

EK_TMPL_EXT template class EK_DECL EkRefCount < EkNullMutex >;
EK_TMPL_EXT template class EK_DECL VEkRefCount < EkNullMutex >;

#include <map>
EK_TMPL_EXT template class std::map < const void *,
    EkRefCount < EK_DEFAULT_MUTEX > *>;
EK_TMPL_EXT template class std::map < const void *,
    EkRefCount < EkNullMutex > *>;
#endif

#endif

