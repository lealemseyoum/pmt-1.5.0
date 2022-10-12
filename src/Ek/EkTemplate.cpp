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
 * Greg L. Gray greg.gray@kodak.com 
 * 
 * Contributor(s): 
 *
 * Date: June 1999
 *
 * Comments: The purpose of this file is to make sure that any DLL exports of 
 * template instantiations declared in an EK header file actually end up in the
 * EK DLL.  The easiest way to make sure of this is to implement a source 
 * module which includes every EK header file. EkLocking.h is a particularly 
 * mean litte beast, that requires us to define what we want the default mutex 
 * to be.  I'm going to punt and not include this file, since I don't think we 
 * need it.
*/

#include "EkPrecompiled.h"      // always first  #include!
#include "EkCompiler.h"         // always second #include!

#if defined (EK_EXPLICIT)
#include "EkBool.h"
#include "EkError.h"
#include "EkLocking.h"
#include "EkNullMutex.h"
#include "EkRefCount.h"
#include "EkSmartPtr.h"
#include "EkStrings.h"
#include "EkTemplate.h"
#endif

