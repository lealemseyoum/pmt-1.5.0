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
 * Creation Date: 11/02/2000
 *
 * Original Author: 
 * Dan Rupe  rupe@kodak.com
 * 
 * Contributor(s): 
*/

#ifdef _MSC_VER
#pragma warning (disable:4786)
#pragma warning (disable:4503)
#endif

#include <algorithm>
#include "PmtAliases.h"

void PmtAliases::insert(const PmtKeyType & aliasKey,
                        const PmtKeyList & aliasMembers)
{
    EK_GUARD(EK_DEFAULT_MUTEX, mLock);

    mAliasMap[aliasKey] = aliasMembers;
}

void PmtAliases::clear()
{
    EK_GUARD(EK_DEFAULT_MUTEX, mLock);

    mAliasMap.clear();
}

void PmtAliases::getKeyList(const PmtKeyType & aliasKey,
                            PmtKeyList & returnKeyList) const
{
    EK_GUARD(EK_DEFAULT_MUTEX, mLock);

    PmtAliasMap::const_iterator aliasLow, aliasHigh;
    findAliases(aliasKey, aliasLow, aliasHigh);
    while (aliasLow != aliasHigh)
    {
        returnKeyList.insert(returnKeyList.end(), (*aliasLow).second.begin(),
                             (*aliasLow).second.end());
        aliasLow++;
    }


    // remove duplicate entries due to alias
    returnKeyList.sort();
    returnKeyList.unique();
}


void PmtAliases::findAliases(const PmtKeyType & key,
                             PmtAliasMap::const_iterator & begin,
                             PmtAliasMap::const_iterator & end) const
{
    if (key[key.length() - 1] == '*')
    {
        // remove the trailing '*'
        PmtKeyType theKey(key, 0, key.length() - 1);

        PmtAliasMap::value_type tmpEntry(theKey, PmtKeyList());

        begin = find_if(mAliasMap.begin(), mAliasMap.end(),
                        bind2nd(AliasSubStrComp(), tmpEntry));

        end = find_if(begin, (PmtAliasMap::const_iterator) mAliasMap.end(),
                      bind2nd(not2(AliasSubStrComp()), tmpEntry));
    }
    else
    {
        begin = mAliasMap.find(key);
        end = mAliasMap.end();
        if (begin != end)
        {
            end = begin;
            end++;
        }
    }
}


void PmtAliases::show()
{
    EK_GUARD(EK_DEFAULT_MUTEX, mLock);

    PmtAliasMap::iterator curIter = mAliasMap.begin();
    PmtAliasMap::iterator end = mAliasMap.end();

    while (curIter != end)
    {
        showKeyList((*curIter).second);
        curIter++;
    }
}


void PmtAliases::showKeyList(const PmtKeyList & keyList)
{
    cout << endl << endl << "The key list's contents are:";

    PmtKeyList::const_iterator curIter = keyList.begin();
    PmtKeyList::const_iterator end = keyList.end();

    while (curIter != end)
    {
        cout << endl << "list item: " << (*curIter).c_str();
        curIter++;
    }

    cout << endl << endl;
}

