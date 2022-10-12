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
 * Creation Date: 05/07/2001
 *
 * Original Author: 
 * Dan Rupe  rupe@kodak.com
 * 
 * Contributor(s): 
*/

#include "PmtCoreCommon.h"
#include "PmtKeyParser.h"


bool PmtKeyParser::parseKey(PmtKeyType & parsedKey)
{
    mParsedKey = mOrigKey;
    bool isValidKey = true;

    if (!mParsedKey.empty())
    {
//        stripWhite();

        if (isInitialCharOk())
        {
            parsedKey = mParsedKey; // Will use this unless we return false below.

            if (!isMultiLevelWildcardTestOk())
                isValidKey = false;
        }
    }
    else
    {
        parsedKey = mParsedKey;
    }


    return isValidKey;
}


// This routine removes all spaces in a string
// (Should it remove tabs, etc., as well?)
void PmtKeyParser::stripWhite()
{
    // If no white space, do nothing
    if (mParsedKey.find(' ') != PmtKeyType::npos)
    {
        char *str;
        str = new char[mParsedKey.size() + 1];
        const char *p1 = mParsedKey.c_str();
        char *p2 = str; // leave str intact for delete [] below.

        // assumes a string is terminated by a null character.
        while (*p1 != 0)
        {
            if (*p1 != ' ')
            {
                *p2 = *p1;
                p2++;
            }
            p1++;
        }
        
        // terminate the string
        *p2 = 0; 
        
        mParsedKey = str;

        delete[]str;
    }
}


bool PmtKeyParser::isOneSegment()
{
    PmtKeyType::size_type pos;

    pos = mParsedKey.find('.');
    if (pos != PmtKeyType::npos)
        return false;
    else
        return true;
}


bool PmtKeyParser::isInitialCharOk()
{
    bool bIsInitialCharOk = false;

    if (((mParsedKey[0] >= 'A') && (mParsedKey[0] <= 'Z')) ||
        ((mParsedKey[0] >= 'a') && (mParsedKey[0] <= 'z')) ||
        (mParsedKey[0] == PMT_SINGLE_LEVEL_WILDCARD_CHAR) )
        bIsInitialCharOk = true;
    else
    if ((isOneSegment()) &&
        (mParsedKey[0] == PMT_MULTI_LEVEL_WILDCARD_CHAR))
        bIsInitialCharOk = true;

    return bIsInitialCharOk;
}


bool PmtKeyParser::isOccurrenceWildcard(PmtKeyType::size_type pos)
{
    // If the wildcard is at position 0, a '[' cannot be before it.
    if (pos == 0)
        return false;

    if ((mParsedKey[pos - 1] == '[') && (mParsedKey[pos + 1] == ']'))
        return true;
    else
        return false;
}


// Ensure any potential '*' wildcard is in an ok location.
// Keys may only have a '*' at the end of the key. (Remembering
// that occurrence specifications may also follow.)
bool PmtKeyParser::isMultiLevelWildcardTestOk()
{
    PmtKeyType::size_type pos;
    PmtKeyType::size_type wildcardPos = 0;
    int numWildcards = 0;
    bool returnValue = true;

    // Just count all the '*''s, and if there are more than 1 that's not
    // an occurrence wildcard, then the key is invalid.

    pos = mParsedKey.find(PMT_MULTI_LEVEL_WILDCARD_CHAR);
    while (pos != PmtKeyType::npos) // while '*' found
    {
        if ( (pos == 0) || 
             !((mParsedKey[pos - 1] == '[') && (mParsedKey[pos + 1] == ']')) )
        {
            wildcardPos = pos;
            numWildcards++;
        }

        pos = mParsedKey.find(PMT_MULTI_LEVEL_WILDCARD_CHAR, pos + 1);
    }

    // Now make sure that the wildcard is in the last position (ignoring occurrences.)
    if (numWildcards == 1)
    {
        // If '*' not at absolute end.
        if (wildcardPos != (mParsedKey.size() - 1))
        {
            // Make sure next char is '['.
            if (mParsedKey[wildcardPos + 1] == '[')
            {
                // Now make sure there's nothing after the closing ']'.
                pos = mParsedKey.find(']', wildcardPos + 2);
                if (pos != (mParsedKey.size() - 1))
                    returnValue = false;
            } 
            else
                returnValue = false;
        }
    }
    else if (numWildcards != 0)
        returnValue = false;

    return returnValue;
}

