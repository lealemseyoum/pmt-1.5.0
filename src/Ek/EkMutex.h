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
 * Date: August 1997
 *
 * Original Author: 
 * Hoffman, Martin R. martin.hoffman@kodak.com
 * Copyright (C) 1997 Eastman Kodak Company
 * 
 * Contributor(s): 
 *
 */

#if !defined (EK_MUTEX_H)
#define EK_MUTEX_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#include <windows.h>
#endif

#include "EkCompiler.h"
#include "EkError.h"

#if ((! defined _MSC_VER) && (defined _POSIX_THREADS))
#include <pthread.h>
#endif


//! A mutual exclusion lock for thread synchronization within a process. 
/*! Once a thread successfully calls lock( ), all threads which then 
 call lock( ) are blocked until the thread which calls lock( ) calls 
 unlock( ) (in some implementations, it may not matter which thread calls 
 unlock, but you should assume that the thread which holds the lock is the 
 only one who can call unlock( )). This object works well with the guard 
 object in EkGuard.h.

 \b Limitations: Currently, the only supported implementations are for 
 Windows NT/95 (using Visual C++) and POSIX.  For other platforms, the 
 implementation of EkMutex is replaced by a stub, which does nothing.

 Important Note: Under Windows, this header file does not compile 
 standalone. One of the files "afxwin.h" or "windows.h" should be included
 before EkMutex.h in any compilation unit.
*/
class EK_DECL EkMutex
{
  public:
    //! This is the default constructor
    inline EkMutex(void);
    //! This is the destructor
     inline ~ EkMutex(void);

    //! Use this to lock all other objects out of a section of code
    inline void lock(void);
    //! Use this to unlock a section of code
    inline void unlock(void);

  protected:
#if ((defined _MSC_VER ) && (!defined _WIN32_WCE))
     CRITICAL_SECTION mLock;
#endif

#if ((! defined _MSC_VER) && (defined _POSIX_THREADS))
    void throwError(const char *msg, const char *filename, int line);
    pthread_mutex_t mLock;
#endif

  private:
    //! This is a private copy constructor
     inline EkMutex(const EkMutex & src)
    {
    };
    //! This is a private assignment operator
    inline EkMutex & operator=(EkMutex & src)
    {
        return *this;
    };
};

#if ((defined _MSC_VER ) && (!defined _WIN32_WCE))
#define EK_MUTEX_OK
// WINDOWS NT/95 USING VISUAL C++

inline EkMutex::EkMutex(void)
{
    InitializeCriticalSection(&mLock);
}

inline EkMutex::~EkMutex(void)
{
    DeleteCriticalSection(&mLock);
}

inline void EkMutex::lock(void)
{
    EnterCriticalSection(&mLock);
}

inline void EkMutex::unlock(void)
{
    LeaveCriticalSection(&mLock);
}
#endif

#if ((! defined _MSC_VER) && (defined _POSIX_THREADS))
#define EK_MUTEX_OK
// POSIX IMPLEMENTATION

inline EkMutex::EkMutex(void)
{
    if (pthread_mutex_init(&mLock, 0) != 0)
        throwError("Could not create pthread mutex", WHERE);
}

inline EkMutex::~EkMutex(void)
{
    if (pthread_mutex_destroy(&mLock) != 0)
        throwError("Error destroying pthread mutex", WHERE);
}

inline void EkMutex::lock(void)
{
#ifndef __APPLE_CC__
    if (pthread_mutex_lock(&mLock) != 0)
        throwError("pthread mutex lock failed", WHERE);
#else
    pthread_mutex_lock(&mLock);
#endif
}
                                                                                                 
inline void EkMutex::unlock(void)
{
#ifndef __APPLE_CC__
    if (pthread_mutex_unlock(&mLock) != 0)
        throwError("pthread mutex unlock failed", WHERE);
#else
    pthread_mutex_unlock(&mLock);
#endif
}
#endif
                                                                                                 

#if !defined(EK_MUTEX_OK)
// DEFAULT "NULL" IMPLEMENTATION

inline EkMutex::EkMutex(void)
{
}

inline EkMutex::~EkMutex(void)
{
}

inline void EkMutex::lock(void)
{
}

inline void EkMutex::unlock(void)
{
}

#if (!defined (_WIN32_WCE) & !defined (macintosh))
#warning There is no implementation of EkMutex for this platform.
#endif
#endif

#endif

