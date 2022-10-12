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

#ifdef _MSC_VER

#include "windows.h"

#include "EkThread.h"

EK_DEFAULT_MUTEX EkThreadMap::lock;

map < EkThreadId, unsigned long >EkThreadMap::threadMap;

void EkThreadMap::insert(EkThreadId id, unsigned long threadHandle)
{
    EK_GUARD(EK_DEFAULT_MUTEX, lock);
    threadMap[id] = threadHandle;
}

void EkThreadMap::remove(EkThreadId id)
{
    EK_GUARD(EK_DEFAULT_MUTEX, lock);
    map < EkThreadId, unsigned long >::iterator i = threadMap.find(id);
    if (i != threadMap.end())
        threadMap.erase(i);
}

unsigned long EkThreadMap::find(EkThreadId id)  // returns 0 if id is not found
{
    EK_GUARD(EK_DEFAULT_MUTEX, lock);
    map < EkThreadId, unsigned long >::iterator i = threadMap.find(id);
    if (i == threadMap.end())
        return 0;
    return (*i).second;
}

#endif

