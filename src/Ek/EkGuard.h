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
 * Creation Date: 11/14/1996
 *
 * Original Author: 
 * Thomas DeWeese thomas.deweese@kodak.com
 * 
 * Contributor(s): 
 * John Hall
*/

//! EkGuard - lock management

/*! An EkGuard object provides an easy and safe way to use a
 mutex or other lock object.  The lock object must implement
 two methods: lock() and unlock().

 When an EkGuard is constructed it locks the lock.  When it is
 destroyed, it unlocks the lock.

 An EkGuard that is declared as a local (neither extern, static, nor 
 on the heap) will always be destroyed at the end of the block in which
 it was declared, thus unlocking the lock.  This can help to avoid
 leaving the lock inadvertently locked, such as when an exception
 causes an immediate exit from the block.

 Example:
 <pre>
      Mutex useCountMutex;    // protects useCount
      long useCount;


      void bumpUseCount()
      { 
          EkGuard<Mutex> guard(useCountMutex);
          theUseCount++;
      }
 </pre>
*/

#ifndef EK_GUARD_H
#define EK_GUARD_H

template < class T > class EkGuard
{
  public:
    //: This is the constructor
  inline EkGuard(T & lock):mLockObj(lock)
    {
        mLockObj.lock();
    }
    //: This is the destructor
    inline ~ EkGuard()
    {
        mLockObj.unlock();
    }

  private:
    T & mLockObj;

    /* While the following is good programming practice, the GCC 2.96 compiler
       has problems with empty template methods (AI 10166)

       //: This is a default constructor
       inline EkGuard();
       //: This is the copy constructor
       inline EkGuard(const EkGuard&);
       //: This is the assignment operator
       inline EkGuard& operator=(EkGuard&);
     */
};

#endif

