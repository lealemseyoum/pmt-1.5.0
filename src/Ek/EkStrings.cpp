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
 * Mike W. Pryor michael.pryor@kodak.com 
 * 
 * Contributor(s): 
 * FirstName LastName <email address>
 * FirstName LastName <email address>
 *
 * Date: February 1998
 *
 * Comments: These functions support for miscellaneous string functions, such
 * as conversion of wide char strings to ASCII strings, and vice-versa.  String 
 * handling functions were moved to EkString from ImaUnicode
*/

#include <limits.h>
#include <stdlib.h>
//#include <string>
#include "EkPrecompiled.h"
#include "EkStrings.h"

void EkPutWString(ostream & stream, const wstring & ws)
{
    SPmtString str = EkWToA(ws);
    stream << str;
}

void EkGetWString(istream & stream, wstring & ws)
{
    SPmtString str;
    stream >> str;
    ws = EkAToW(str);
}

wstring EkAToW(const SPmtString & str)
{
    if (str.length() == 0)
        return wstring();

    wchar_t *result = new wchar_t[str.length() + 1];

    // since unicode supports all of 8-bit ASCII, this call should
    // never fail (unless there's some locale dependency that no 
    // one has told me about).
    mbstowcs(result, str.c_str(), str.length() + 1);

    wstring returnVal(result);
    delete[]result;
    return returnVal;
}

SPmtString EkWToA(const wstring & ws, char f, const char *e)
{
    if (!e)
        e = "";

    // we need to do this in a loop, character-by-character, in order
    // to handle conversion failures. Not sure this will work if f=='\0'
    // but there's no documentation which says that it won't work.
    SPmtString str(ws.length(), f);

    wstring::const_iterator wIter = ws.begin();
//    SPmtString::data_type *data = (SPmtString::data_type *)str.c_str();

    int i = 0;

    for (i = 0; wIter != ws.end(); wIter++, i++)
    {
        wchar_t unicodeChar = (*wIter);

        char mbc[MB_LEN_MAX];

        if (wctomb(mbc, unicodeChar) == 1)
            str[i] = mbc[0];
        else if (!f)
            return e;
    }
    return str;
}

