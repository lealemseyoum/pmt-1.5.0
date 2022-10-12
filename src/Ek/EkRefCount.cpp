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
 * Contributor(s): 
 * John Hall
 * Martin R. Hoffman martin.hoffman@kodak.com
 *
 * Date: December 1996
 *
 * Comments: Visual C++ follows the Borland model of templates, in which the
 * entire template implementation is included in the header file (even non-
 * inline functions).  The linker handles cleaning up multiple definitions of 
 * these functions.  If the non-inline functions are placed outside the header 
 * file, they will result in unresolved references for any template 
 * instantiation which is not made explicit in the file in which the 
 * implementation occurs.  In this case, the instantiation for EK_DEFAULT_MUTEX
 * is explicitly declared below, so that was the only class for which an 
 * instantiation of EkRefCount and VEkRefCount did NOT result in unresolved 
 * references.  To fix the problem, I moved the non-inline functions into the 
 * .h file.  This solves the problem for Visual C++ and other compilers which 
 * follow the Borland model, but may cause us grief later.
*/

#if defined (_MSC_VER)
#pragma warning(disable:4786)
#endif

#include "EkPrecompiled.h"
#include "EkRefCount.h"
#include "EkTemplate.h"

EK_IMPL_CLASS_TMPL(EkRefCount, EK_DEFAULT_MUTEX);
EK_IMPL_CLASS_TMPL(VEkRefCount, EK_DEFAULT_MUTEX);

