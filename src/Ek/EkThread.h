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
 * Contributor(s): 
 * Greg Kuberka
 * Sam Fryer    samuel.fryer@kodak.com
*/

//////////////////////////////////////////////////////////////////////////////
//
// Currently, real implementations of EkThread do not exist for the SGI
// and PSOS+ platforms.  Only Windows NT/95 (using VC++) and POSIX 
// implementations are provided.  Alledgedly, POSIX support is optional on SGI. 
//
//////////////////////////////////////////////////////////////////////////////

#ifndef EK_THREAD_H
#define EK_THREAD_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

//////////////////////////////////////////////////////////////////////////////
//
// Class:   EkThread
// Author:  SJ Mueller
//
//////////////////////////////////////////////////////////////////////////////
//
// Description:
//
//      EkThread encapsulates the OS notion of a thread of execution within
//      a process.  It is parameterized by the type of lock object used by the 
//      Reference Counting base class.
//
//      An instance of EkThread derivative is created and then "Launched" to
//      to acutally spawn the new thread of execution.  Once "Launched" the 
//      main() method is invoked by the new thread passing it the address of the
//      thread object.  main() in turn invokes the run() method implemented by the
//      specific EkThread derivative to initiate the thread's specific processing.
//
//      This allows derivatives of EkThread to be developed which encapsulate
//      some unit of work which is desired to be completed in a separate thread
//      of execution.
//
//////////////////////////////////////////////////////////////////////////////
//
// Important Note:
//
//              Under Windows, this header file does not compile standalone.
//              One of the files "afxwin.h" or "windows.h" should be included
//              before EkThread.h in any compilation unit.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef EK_COMPILER_H
#include "EkCompiler.h"
#endif

#ifndef _EKREFCOUNT_H_
#include "EkRefCount.h"
#endif

#ifndef EKERROR_H
#include "EkError.h"
#endif

#ifndef _EKSMARTPTR_H_
#include "EkSmartPtr.h"
#endif

#ifdef _MSC_VER
#include <process.h>
#ifndef EK_SINGLETHREADED
typedef unsigned long EkThreadId;
#define EK_THREADS_OK
#endif
#endif

#if ((! defined _MSC_VER) && (defined _POSIX_THREADS))
#include <pthread.h>
typedef pthread_t EkThreadId;
#define EK_THREADS_OK
#endif

#if ((defined __MWERKS__) && (!defined _POSIX_THREADS))
typedef MPTaskID EkThreadId;
#define EK_THREADS_OK
#endif

#ifndef EK_THREADS_OK
typedef int EkThreadId;         // stub thread implementation
#endif

//////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#include "EkLocking.h"
#include <map>

class EK_DECL EkThreadMap
{
    // In order to wait for thread completion, under Win32, we need the
    // handle which was returned by beginthread.  Unfortunately, given 
    // a thread id, there is no way to get that handle, so we need to 
    // maintain the mapping from thread id to handle ourselves.
    //
    // This class maintains that mapping for all instantiations of any
    // EkThread<>.
  public:

    static void insert(EkThreadId id, unsigned long threadHandle);

    static void remove(EkThreadId id);

    static unsigned long find(EkThreadId id);   // returns 0 if id is not found

  private:

    static EK_DEFAULT_MUTEX lock;

    static map < EkThreadId, unsigned long >threadMap;
};
#endif

//////////////////////////////////////////////////////////////////////////////

template < class mutex_type > class EkThread:public EkRefCount < mutex_type >
{

  public:

    enum EkThreadState
    { NOT_STARTED, RUNNING, TERMINATED };
    // possible states of the thread

    inline EkThread():EkRefCount < mutex_type > (), runError(0),
        state(NOT_STARTED), threadId(0)
    {
    }
    // constructor

    virtual ~ EkThread(void);
    // destructor

    EkErrp getError(void)
    {
        return runError;
    };
    // Returns a smart pointer to an error generated by the Run method.

    EkThreadState getState(void)
    {
        return state;
    };
    // Returns current state of the thread.

    EkThreadId getThreadId(void)
    {
        return threadId;
    };
    // Returns the thread id of the thread represented by this object.

    bool setPriority(int relative_priority);
    // sets the priority of the thread relative to its original
    // priority at creation time.  the relative_priority value 
    // should be in the range -2 to +2.
    //
    // this function may not be implemented on all platforms.
    //
    // the function returns false, if it is not implemented or
    // if the system call to set the priority fails.

    static EkThreadId getCurrentThreadId(void)
    {
        // static method returns the thread id of the current thread.
#ifdef _MSC_VER
        return GetCurrentThreadId();
#endif

#if ((! defined _MSC_VER) && (defined _POSIX_THREADS))
        return pthread_self();
#endif

#if ((defined __MWERKS__) && (!defined _POSIX_THREADS))
        return MPCurrentTaskID();
#endif

#ifndef EK_THREADS_OK
        return 0;
#endif
    }

    static bool waitForThread(EkThreadId id)
    {
        // blocks current thread of execution until specified
        // thread has completed.  the function returns true, 
        // if the wait was successfully accomplished.

#ifdef _MSC_VER
        HANDLE hEvent = (HANDLE) EkThreadMap::find(id);

        if (!hEvent)
            return true;

        while (true)
        {
            // Wait for the event and for messages.

            DWORD dwReturn = MsgWaitForMultipleObjects(1, &hEvent,
                                                       false, INFINITE,
                                                       QS_ALLINPUT);
            if (dwReturn == WAIT_OBJECT_0)
                return true;    // Our event happened.
            else if (dwReturn == WAIT_OBJECT_0 + 1)
            {                   // Handle message to keep client alive.
                MSG msg;
                while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                    ::DispatchMessage(&msg);
            }
            else
                return false;
        }
#endif

#if ((! defined _MSC_VER) && (defined _POSIX_THREADS))
        return ((pthread_join(id, 0) == 0) ? true : false);
#endif

#ifndef EK_THREADS_OK
        return false;
#endif
    }

    // Launches the thread by calling the protected static method
    // main() in the new thread.
    //
    inline void launch(void)
    {
        int error = 0;

        // Bump the thread reference count so that we do not lose
        // the thread object across the thread boundary.  Decrement
        // below if error, otherwise, decrement in EkThread::main().
        this->addRef();

#ifdef _MSC_VER

#ifdef EK_SINGLETHREADED
        unsigned long threadHandle = 0;
#else
        unsigned long threadHandle = _beginthreadex((void *) NULL, 0,
                                                    (unsigned (__stdcall *)
                                                     (void *)) EkThread::main,
                                                    (void *) this, 0,
                                                    (unsigned *) &threadId);
#endif
        if (threadHandle == 0)
            error = (errno ? errno : -1);
        else
            EkThreadMap::insert(threadId, threadHandle);
#endif

#if ((! defined _MSC_VER) && (defined _POSIX_THREADS))
        error =
            pthread_create(&threadId, 0, (void *(*)(void *)) EkThread::main,
                           this);
#endif

#ifndef EK_THREADS_OK
        error = -1;             // unconditional error -- threads not implemented
#endif

        if (error)
        {
            // Since the thread did not get created properly, decrement
            // the ref count here so that the thread object will get
            // cleaned up.
            this->deleteRef();

            runError = new EkError("Ek", error, WHERE);
            state = TERMINATED;
        }
        else
        {
            state = RUNNING;
        }
    }

    virtual void run(void) = 0;
    // implemented by subclasses

  protected:

    // Calls the run() method on its argument.
    //
#ifdef _MSC_VER
    static unsigned __stdcall
#else
    static void
#endif
     main(void *thread)
    {
        // Create a new smart pointer for the thread object,
        // then decrement the ref count to counter the add ref
        // done in the EkThread::launch method (which got the
        // thread object safely across the thread boundary).
        //
        EkSmartPtr < EkThread > threadPtr =
            reinterpret_cast < EkThread * >(thread);

        // The parent thread sets the member variable 'threadId' but if the
        // sets it after launching the child thread then its possible that
        // the child could query its 'threadId' and not have it set.  The
        // simple check below will correct this race condition.
        if (threadPtr->threadId == 0)
            threadPtr->threadId = getCurrentThreadId();

#ifdef _MSC_VER
        EkThreadId the_thread_id = threadPtr->threadId;
#endif

        threadPtr->deleteRef();

        threadPtr->run();

        threadPtr->state = TERMINATED;

        // Zero out the thread pointer so that the thread object
        // can safely go away now that it is no longer in use.
        //
        threadPtr = 0;

#ifdef _MSC_VER

        HANDLE threadHandle = (HANDLE) EkThreadMap::find(the_thread_id);
        // get a handle to the thread

        EkThreadMap::remove(the_thread_id);
        // this could introduce a race condition, whereby our
        // user may think the thread has terminated before we
        // actually execute the endthreadex() call.  hopefully
        // this will not be a problem. for anyone.

        if (threadHandle != (HANDLE) 0)
            CloseHandle(threadHandle);

#ifndef EK_SINGLETHREADED
        _endthreadex(0);
#endif

        return 0;
#endif

#if ((! defined _MSC_VER) && (defined _POSIX_THREADS))
        pthread_exit(0);
#endif
    }

    EkErrp runError;
    EkThreadState state;
    EkThreadId threadId;

};

//////////////////////////////////////////////////////////////////////////////

template < class mutex_type > EkThread < mutex_type >::~EkThread(void)
{
}

//////////////////////////////////////////////////////////////////////////////

template < class mutex_type >
    bool EkThread < mutex_type >::setPriority(int relative_priority)
{
#ifdef _MSC_VER

    HANDLE id = (HANDLE) EkThreadMap::find(threadId);
    BOOL result = false;

    switch (relative_priority)
    {
    case -2:
        result = SetThreadPriority(id, THREAD_PRIORITY_LOWEST);
        break;
    case -1:
        result = SetThreadPriority(id, THREAD_PRIORITY_BELOW_NORMAL);
        break;
    case 0:
        result = SetThreadPriority(id, THREAD_PRIORITY_NORMAL);
        break;
    case 1:
        result = SetThreadPriority(id, THREAD_PRIORITY_ABOVE_NORMAL);
        break;
    case 2:
        result = SetThreadPriority(id, THREAD_PRIORITY_HIGHEST);
        break;
    default:
        break;
    };

    return (result ? true : false);
#endif

#if ((! defined _MSC_VER) && (defined _POSIX_THREADS))
    return false;
#endif

#ifndef EK_THREADS_OK
    return false;
#endif
}

//////////////////////////////////////////////////////////////////////////////

#ifdef EK_EXPLICIT
#include "EkLocking.h"
#include "EkNullMutex.h"

EK_TMPL_EXT template class EK_DECL EkThread < EK_DEFAULT_MUTEX >;
EK_TMPL_EXT template class EK_DECL EkThread < EkNullMutex >;
#endif

#endif

