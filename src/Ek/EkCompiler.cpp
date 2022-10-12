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
 * Creation Date: 02/1998
 *
 * Original Author: 
 * Martin R. Hoffman martin.hoffman@kodak.com
 * 
 * Contributor(s): 
*/


#include "EkPrecompiled.h"
#include "EkCompiler.h"

#ifdef _MSC_VER

#include <stdio.h>
// A non-MFC version of TRACE which is (in theory) completely
// compatible with TRACE (even with respect to unicode) -- this
// code was lifted from UTILS.CPP in CIA\Support

void EK_TRACE_IMPL(LPCTSTR format, ...)
{
    TCHAR szBuf[1024];
    szBuf[(sizeof(szBuf) / sizeof(TCHAR)) - 1] = 0;

    va_list args;

    va_start(args, format);
#if (!defined UNICODE) && (!defined _UNICODE)
    _vsnprintf(szBuf, (sizeof(szBuf) / sizeof(TCHAR)) - 1, format, args);
#else
    _vsnwprintf(szBuf, (sizeof(szBuf) / sizeof(TCHAR)) - 1, format, args);
#endif
    va_end(args);

    OutputDebugString(szBuf);
}

#endif

