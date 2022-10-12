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
 * Creation Date: 04/11/2001
 *
 * Original Author: 
 * Dan Rupe  rupe@kodak.com
 * Copyright (C) 2001 Eastman Kodak Company. All Rights Reserved.
 * 
 * Contributor(s): 
*/

#ifndef PMT_OCCURRENCE_H
#define PMT_OCCURRENCE_H


#ifdef _MSC_VER
#include <windows.h>
#endif
#include "EkRefCount.h"
#include "EkSmartPtr.h"

#include "PmtCommon.h"

class PmtOccurrence;

typedef vector < int >PmtOccurrenceList;
typedef EkSmartPtr < PmtOccurrence > PmtOccurrencePtr;
typedef EkConstSmartPtr < PmtOccurrence > PmtConstOccurrencePtr;


class PmtOccurrence:public EkRefCount < EK_DEFAULT_MUTEX >
{
  public:
    enum
    { OCCURRENCE_END = -1 };

    inline PmtOccurrence():mIsUnspecifiedOccurrence(true), mIsWildcard(false),
        mIsExplicitlySpecifiedSingleOccurrence(false), mIndex(0)
    {
    }
    inline ~ PmtOccurrence()
    {
    }

    bool parseKeyForOccurrences(PmtKeyType & key);
    inline bool isUnspecifiedOccurrence() const
    {
        return mIsUnspecifiedOccurrence;
    }
    inline bool isWildcard() const
    {
        return mIsWildcard;
    }
    inline bool isExplicitlySpecifiedSingleOccurrence() const
    {
        return mIsExplicitlySpecifiedSingleOccurrence;
    }
    int start() const;
    int next() const;
    void show() const;

  private:
    bool parseOccurrenceSpecification(const SPmtString & occurrence);
    bool parseArgument(SPmtString argument);
    bool parseRange(SPmtString argument,SPmtString::size_type pos);
    bool isNumber(const char *numberString);
    void addOccurrence(int occurrence);

    bool mIsUnspecifiedOccurrence;
    bool mIsWildcard;
    bool mIsExplicitlySpecifiedSingleOccurrence;
    PmtOccurrenceList mOccurrenceList;
    mutable unsigned int mIndex;
};


#endif                          // PMT_OCCURRENCE_H

