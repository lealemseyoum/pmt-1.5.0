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
* Creation Date: 12/11/1999
*
* Original Author: 
* Philip Wu   hanbing.wu@kodak.com
* 
* Contributor(s):
*/

#include "XMLEncoder.h"
using namespace std;

CXMLEncoder CXMLEncoder::mParser;
CXMLEncoder::CXMLEncoder(void)
{
	Initialize();
}

CXMLEncoder::~CXMLEncoder(void)
{
}

void CXMLEncoder::Initialize()
{
	mCharacterMappings.insert(std::map<wchar_t,wstring>::value_type(L'&', L"&amp;"));
	mCharacterMappings.insert(std::map<wchar_t,wstring>::value_type(L'<', L"&lt;"));
	mCharacterMappings.insert(std::map<wchar_t,wstring>::value_type(L'>', L"&gt;"));
	mCharacterMappings.insert(std::map<wchar_t,wstring>::value_type(L'\"', L"&quot;"));
	//mCharacterMappings.insert(std::map<wchar_t,wstring>::value_type(L' ', L"&nbsp;"));
}

wstring CXMLEncoder::Encode(const wstring& strOrg)
{
	wstring strResult;
	wstring::const_iterator itBeg;
	std::map<wchar_t,wstring>::iterator itFind;
	for (itBeg = strOrg.begin(); itBeg != strOrg.end(); itBeg ++)
	{
		itFind = mParser.mCharacterMappings.find(*itBeg);
		if(itFind != mParser.mCharacterMappings.end())
			strResult.append(itFind->second);
		else
			strResult.append(1,*itBeg);
	}
	return strResult;
}