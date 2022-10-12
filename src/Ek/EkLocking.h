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
 * Gregory Gray greg.gray@kodak.com
*/

//! EkLocking.h - macros for lock handling

/*! This file defines two macros:
<pre>
   EK_DEFAULT_MUTEX - a default mutex class

   EK_GUARD(type, lock) - declare a guard (see EkGuard.h) using a
            particular lock of the given type
</pre>
 Configuration:
<p>
 The macro EK_SINGLETHREADED will determine whether the EK being built
 or used is single-threaded or multi-threaded.  If EK_SINGLETHREADED is 
 defined, then EK is single-threaded.  If it is not defined, EK is 
 multi-threaded.
<P>
 If EK_SINGLETHREADED is not defined, the EK_DEFAULT_MUTEX class is set
 to "EkMutex" otherwise it is set to the dummy "EkNullMutex" class.
<p>
 If EK_SINGLETHREADED is defined and TGT_DEBUG is not defined, the EK_GUARD
 macro does not define a guard at all.
<p>
 If EK_SINGLETHREADED is defined and TGT_DEBUG is defined, the EK_GUARD
 macro defines a guard that uses the EkNullMutex.
*/

#ifndef EK_LOCKING_H
#define EK_LOCKING_H

#if defined(EK_NOUSE_LOCKS)
#error Must define EK_SINGLETHREADED instead of EK_NOUSE_LOCKS
#endif

#ifdef EK_SINGLETHREADED
#include "EkNullMutex.h"
#define EK_DEFAULT_MUTEX EkNullMutex
#ifdef TGT_DEBUG
#include "EkGuard.h"
#define EK_GUARD(type,lock) EkGuard<type> _guardian(lock)
#else                           // !TGT_DEBUG
#define EK_GUARD(type,lock) (void)0
#endif

#else
#include "EkMutex.h"
#include "EkGuard.h"
#define EK_DEFAULT_MUTEX EkMutex
#define EK_GUARD(type,lock) EkGuard<type> _guardian(lock)
#endif

#endif                          // EK_LOCKING_H

