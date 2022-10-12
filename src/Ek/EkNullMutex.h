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
 * Date: 1996
 *
 * Original Author: 
 * Thomas Deweese thomas.deweese@kodak.com 
 * 
 * Copyright (C) 1996 Eastman Kodak Company
 *
 * Contributor(s): 
 * John Hall 
 * Martin R. Hoffman martin.hoffman@kodak.com
 *
 */



#if !defined(EK_NULL_MUTEX_H)
#define EK_NULL_MUTEX_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#if !defined(EK_COMPILER_H)
#include "EkCompiler.h"
#endif

#if !defined(EK_TEMPLATE_H)
#include "EkTemplate.h"
#endif

#if !defined(EK_GUARD_H)
#include "EkGuard.h"
#endif

//! The whole purpose of this class is to do \b nothing.  
/*! It is a stand in for something that will block when trying to get
  the lock if it is already locked.  This class is meant to be used in
  single-threaded environment.
 
  Most users of mutex classes will define a macro which identifies the
  type of mutex to use within a particular context.  When a build of
  the application is done for a single threaded environment, these
  macros can be assigned the value EkNullMutex.  When a build is done
  for a multi-threaded environment, the macro can be defined to refer
  to a class which implements real mutual exclusion.
 
  #define EK_DEBUG_NULL_MUTEX, if you want some minimal code added to
  EkNullMutex to allow for synchronization checks.  With this symbol
  defined the lock( ) and unlock( ) functions will check to make sure
  that the mutex is in the correct (i.e., opposite) state. If an error
  occurs, the static function EkNullMutex::mutexError will be called.
*/

#if !defined(EK_DEBUG_NULL_MUTEX)
class EkNullMutex
{
  public:
    inline EkNullMutex()
    {
    }
    inline ~ EkNullMutex()
    {
    }
    inline void lock(void)
    {
    }
    inline void unlock(void)
    {
    }

  private:
    //Copy constructor and assignment operator
    inline EkNullMutex(const EkNullMutex & src)
    {
    };
    inline EkNullMutex & operator=(const EkNullMutex & src)
    {
        return *this;
    };
};
#else                           // EK_DEBUG_NULL_MUTEX

class EkNullMutex
{
  public:
    int count;

    inline EkNullMutex(void):count(0)
    {
    }
    inline ~ EkNullMutex()
    {
    }

    inline static void mutexError(void)
    {
        printf("Mutex Error occured\n");
    }

    inline void unlock();
    inline void lock();

  private:
    //Copy constructor and assignment operator
    inline EkMutex(const EkMutex & src)
    {
    };
    inline EkMutex & operator=(EkMutex & src)
    {
    };
};

inline void lock(void)
{
    if ((count++) != 0)
        mutexError();
}

inline void unlock(void)
{
    if ((count--) != 1)
        mutexError();
}
#endif                          // EK_DEBUG_NULL_MUTEX

#ifndef EKNULLMUTEX_CPP
EK_EXTERN_CLASS_TMPL(EkGuard, EkNullMutex);
#endif

#endif                          // EK_NULL_MUTEX_H

