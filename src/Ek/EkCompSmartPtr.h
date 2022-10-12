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
 * Thomas Deweese thomas.deweese@kodak.com 
 * 
 * Copyright (C) 1996 Eastman Kodak Company
 *
 * Contributor(s): 
 * John Hall
 *
 * Date: April 1996
 *
 */

#if !defined (EK_COMP_SMART_PTR_H)
#define EK_COMP_SMART_PTR_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#include "EkCompiler.h"
#include "EkBool.h"
#include "EkTemplate.h"

//! Smart Point implementation based on reference counting.
/*! These classes are reference-counted and have shallow copy
 semantics.  Memory management can be a tedious and error prone
 process.  The idea behind smart pointers is to handle the majority of
 the book work for the user.

 <p> 
 This smart pointer implementation is built on the notion of
 reference counting objects.  This means that when someone new starts
 to point at an object they increment a use count variable in the
 object they are pointing at.  When they cease to point at that object
 they decrement the use count variable.  If the use count variable
 ever goes to zero the object should be deleted as it is no-longer
 being referenced by anyone.

 <p>
 Reference counting has been used for many years to manage shared
 objects.  However with the advent of C++ and operator overloading it
 has become possible to hide most tedious aspects of reference
 counting in a 'pointer' class.  This 'pointer' class attempts to
 mimic the behaviour of a regular C++ pointer through operator
 overloading.  In the proper places (construction, deletion, and
 assignment) it calls the addRef, and deleteRef methods of the object
 it is pointing at.

 <p>
 The required methods are defined by class EkRefCount.  Because
 EkConstSmartPtr and EkSmartPtr are templated it is not required that
 the object they point to be subclasses of EkRefCount (although that
 is a quick way to obtain the needed functionality).

 <p>
 In certain cases the user may wish to reimplement addRef/deleteRef to
 handle tricky situations.

 <p>
 In general a EkSmartPtr can be treated the same as a regular pointer.
 However it should never be used to point at a stack based object
 (otherwise a double delete will happen).  Similarly, it should never
 be used to point at a static object, for it will attempt to call
 "delete" on that object.

 <p>
 Note that EkConstSmartPtr is a base class for EkSmartPtr this is so
 that proper constness symantics can be followed.  Just like a char*
 can be converted to a const char * a EkSmartPtr<foo> can be converted
 to a EkConstSmartPtr<foo> (the constness refers to the object being
 pointed at, not the EkSmartPtr object).

 <p>
 Note: The template argument for EkSmartPtr and EkConstSmartPtr must
 be a class that provides the following member functions:

     void addRef(void) - add one to the use count for the object.
     bool deleteRef(void) - subtract one from the use count for the object
          If the object should be deleted return non-zero.

 <p>
  This requirement may be satisfied by basing the class on EkRefCount
*/
template < class obj > class EkConstCompSmartPtr
{
  public:
    //! Void constructor sets the ptr to NULL
    inline EkConstCompSmartPtr(void);

    //! Destructor - Calls deleteRef if ptr is not NULL.
    inline ~ EkConstCompSmartPtr(void);

    //! Create a new reference to the object.  Calls addRef if ptr != NULL
    inline EkConstCompSmartPtr(const EkConstCompSmartPtr < obj > &sp);
    //! constructor
    inline EkConstCompSmartPtr(const obj * objPtr);

    //! Point this object at a new spot.  Calls deleteRef for current obj, and addRef for new Object... Assuming they aren't null.
    inline EkConstCompSmartPtr < obj > &operator=(const EkConstCompSmartPtr <
                                                  obj > &sp);
    //! assignment operator
    inline EkConstCompSmartPtr < obj > &operator=(const obj * objPtr);

    //! Member selection through pointer
    inline const obj *operator->(void) const;

    //! Get the raw pointer value be careful not to delete the pointer.
    inline const obj *ptr(void) const;

    //! Dereference operator
    inline const obj & operator*(void) const;

    // Allow for pointer equality tests.
    //! Equal operator
    inline bool operator==(const EkConstCompSmartPtr < obj > &nSP) const;
    //! Another equal operator
    inline bool operator==(const obj * objPtr) const;
    //! Not equal operator
    inline bool operator!=(const EkConstCompSmartPtr < obj > &nSP) const;
    //! Another not equal operator
    inline bool operator!=(const obj * objPtr) const;

    // comparision operators (especially for STL)
    //! Less than operator
    inline bool operator<(const EkConstCompSmartPtr < obj > &nsp) const;
    //! Another less than operator
    inline bool operator<(const obj * optr) const;

    //! Convert to bool. Allow for code like: if(ptr)
    inline operator    bool(void) const;

  protected:
    obj * mPtr;
    inline void addRef(void) const;
    inline void deleteRef(void) const;
};

template < class obj >
    inline bool operator==(const obj * optr,
                           const EkConstCompSmartPtr < obj > &sp)
{
    return optr == sp.ptr();
}

template < class obj >
    inline bool operator!=(const obj * optr,
                           const EkConstCompSmartPtr < obj > &sp)
{
    return optr != sp.ptr();
}

template < class obj >
    inline bool operator<(const obj * optr,
                          const EkConstCompSmartPtr < obj > &sp)
{
    return optr < sp.ptr();
}

//! EkCompSmartPtr - Templated, use count based Pointer class.

/*! This is a very simple subclass of EkConstCompSmartPtr that returns
 non-const pointers, and references.  It is a subclass so it can
 be automatically coerced into a EkConstCompSmartPtr when needed by
 the compiler.
 <p>
 \sa EkConstCompSmartPtr for a full description
*/

template < class obj > class EkCompSmartPtr:public EkConstCompSmartPtr < obj >
{
  public:
    //! Default constructor
  inline EkCompSmartPtr(void):EkConstCompSmartPtr < obj > ()
    {
    };

    //! Copy constructor
    inline EkCompSmartPtr(const EkCompSmartPtr < obj >
                          &nSP):EkConstCompSmartPtr < obj > (nSP)
    {
    };
    //! copy constructor
    inline EkCompSmartPtr(obj * objPtr):EkConstCompSmartPtr < obj > (objPtr)
    {
    };

    //! Point this object at a new spot.  Calls deleteRef for current obj, and addRef for new Object... Assuming they aren't null.
    inline EkCompSmartPtr < obj > &operator=(const EkCompSmartPtr < obj >
                                             &nSP);
    //! Another assignment operator
    inline EkCompSmartPtr < obj > &operator=(obj * objPtr);

    //! Member selection through pointer
    inline obj *operator->(void) const;

    //! Dereference operator
    inline obj & operator*(void) const;

    //! Get the raw pointer value
    inline obj *ptr(void) const;
};

template < class obj >
    EkConstCompSmartPtr < obj >::EkConstCompSmartPtr(void):mPtr(0)
{
}

template < class obj >
    EkConstCompSmartPtr <
    obj >::EkConstCompSmartPtr(const EkConstCompSmartPtr < obj >
                               &nSP):mPtr(nSP.mPtr)
{
    addRef();
}

template < class obj >
    EkConstCompSmartPtr <
    obj >::EkConstCompSmartPtr(const obj * objPtr):mPtr(const_cast <
                                                        obj * >(objPtr))
{
    addRef();
}

template < class obj > EkConstCompSmartPtr < obj >::~EkConstCompSmartPtr(void)
{
    deleteRef();
}


template < class obj > void EkConstCompSmartPtr < obj >::addRef(void) const
{
    if (mPtr)
    {
        if (mPtr->addRef())
            mPtr->referenced();
    }
    return;
}

template < class obj > void EkConstCompSmartPtr < obj >::deleteRef(void) const
{
    if (mPtr && mPtr->deleteRef())
        mPtr->unreferenced();
    return;
//delete mPtr;
//    mPtr = 0;
}


template < class obj >
    inline const obj *EkConstCompSmartPtr < obj >::operator->(void) const
{
    return mPtr;
}

template < class obj > inline const obj *EkConstCompSmartPtr < obj >::ptr(void) const
{
    return mPtr;
}

template < class obj >
    inline const obj & EkConstCompSmartPtr < obj >::operator*(void) const
{
    return *mPtr;
}

template < class obj >
    inline bool
    EkConstCompSmartPtr < obj >::operator==(const EkConstCompSmartPtr < obj >
                                            &nSP) const
{
    return mPtr == nSP.mPtr;
}

template < class obj >
    inline bool EkConstCompSmartPtr <
    obj >::operator==(const obj * objPtr) const
{
    return mPtr == objPtr;
}

template < class obj >
    inline bool
    EkConstCompSmartPtr < obj >::operator!=(const EkConstCompSmartPtr < obj >
                                            &nSP) const
{
    return mPtr != nSP.mPtr;
}

template < class obj >
    inline bool EkConstCompSmartPtr <
    obj >::operator!=(const obj * objPtr) const
{
    return mPtr != objPtr;
}

template < class obj >
    inline bool
    EkConstCompSmartPtr < obj >::operator<(const EkConstCompSmartPtr < obj >
                                           &nSP) const
{
    return mPtr < nSP.mPtr;
}

template < class obj >
    inline bool EkConstCompSmartPtr <
    obj >::operator<(const obj * objPtr) const
{
    return mPtr < objPtr;
}

template < class obj > inline EkConstCompSmartPtr <
    obj >::operator    bool(void) const
{
    return mPtr != 0;
}

template < class obj > EkConstCompSmartPtr < obj > &EkConstCompSmartPtr <
    obj >::operator=(const EkConstCompSmartPtr < obj > &nSP)
{
    if (mPtr != nSP.mPtr)
    {
        // BUG: This block of code is not safe against exceptions
        // in addRef or deleteRef

        deleteRef();

        mPtr = nSP.mPtr;
        addRef();
    }
    return *this;
}

template < class obj > EkConstCompSmartPtr < obj > &EkConstCompSmartPtr <
    obj >::operator=(const obj * objPtr)
{
    if (mPtr != objPtr)
    {
        // BUG: This block of code is not safe against exceptions in addRef or deleteRef

        deleteRef();

        mPtr = const_cast < obj * >(objPtr);
        addRef();
    }
    return *this;
}

template < class obj >
    inline EkCompSmartPtr < obj > &EkCompSmartPtr <
    obj >::operator=(const EkCompSmartPtr < obj > &nSP)
{
    EkConstCompSmartPtr < obj >::operator=(nSP);
    return *this;
}

template < class obj >
    inline EkCompSmartPtr < obj > &EkCompSmartPtr <
    obj >::operator=(obj * objPtr)
{
    EkConstCompSmartPtr < obj >::operator=(objPtr);
    return *this;
}

template < class obj > inline obj * EkCompSmartPtr < obj >::operator->(void) const
{
    return mPtr;
}

template < class obj > inline obj * EkCompSmartPtr < obj >::ptr(void) const
{
    return mPtr;
}

template < class obj > inline obj & EkCompSmartPtr < obj >::operator*(void) const
{
    return *mPtr;
}

///////////////////////////////////////////////////////////////////
// Macros to explicitly control template instantiation
///////////////////////////////////////////////////////////////////


// This 'defines' the EkCompSmartPtr class which also expands the template
// code so it gets inlined properly.
#define EK_DEFINE_SMARTPTR(T)                                          \
    EK_EXTERN_CLASS_TMPL(EkConstCompSmartPtr,T);                             \
    EK_EXTERN_CLASS_TMPL(EkCompSmartPtr,T);                                  \
    typedef EkConstCompSmartPtr< T > Const##T##Ptr;                           \
    typedef EkCompSmartPtr< T > T##Ptr

// This 'implements' the EkCompSmartPtr class (basically outputs constructor)
#define EK_IMPL_SMARTPTR(T)                                            \
    EK_IMPL_CLASS_TMPL(EkConstCompSmartPtr,T);                               \
    EK_IMPL_CLASS_TMPL(EkCompSmartPtr,T)

#endif                          //EK_SMART_PTR_H

