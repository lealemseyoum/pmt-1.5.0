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
* Creation Date: 10/10/1999
*
* Original Author: 
* Philip Wu   hanbing.wu@kodak.com
* 
* Contributor(s):
*/
#include "XMPNameSpaceManager.h"
XMPNameSpaceManager XMPNameSpaceManager::mNameSpaceManager;
XMPNameSpaceManager::XMPNameSpaceManager(void)
{
	// Add some well know name spaces
	nameSpaceMap[L"adobe:ns:meta/"] = L"x";
	nameSpaceMap[L"http://www.w3.org/1999/02/22-rdf-syntax-ns#"] = L"rdf";
	nameSpaceMap[L"http://purl.org/dc/elements/1.1/"] = L"dc";
	nameSpaceMap[L"http://ns.adobe.com/xap/1.0/"] = L"xap";
	nameSpaceMap[L"http://ns.adobe.com/xap/1.0/mm/"] = L"xapMM";
	nameSpaceMap[L"http://ns.adobe.com/xap/1.0/sType/ResourceRef#"] = L"stRef";
	nameSpaceMap[L"http://ns.adobe.com/photoshop/1.0/"] = L"photoshop";
	nameSpaceMap[L"http://ns.adobe.com/tiff/1.0/"] = L"tiff";
	nameSpaceMap[L"http://ns.adobe.com/exif/1.0/"] = L"exif";
	nameSpaceMap[L"http://ns.adobe.com/photoshop/1.0/"] = L"photoshop";
	nameSpaceMap[L"http://ns.adobe.com/xmp/1.0/DynamicMedia/"] = L"xmpDM";
	nameSpaceMap[L"http://ns.microsoft.com/Photo/1.0/"] = L"MicrosoftPhoto";
}

XMPNameSpaceManager::~XMPNameSpaceManager(void)
{
	nameSpaceMap.clear();
}

wstring XMPNameSpaceManager::GetFullName(const wstring& strPrefix)
{
	map<wstring,wstring>::const_iterator it;
	for (it = mNameSpaceManager.nameSpaceMap.begin(); it != mNameSpaceManager.nameSpaceMap.end(); it ++)
	{
		if((*it).second == strPrefix)
			return (*it).first;
	}
	return NULL;
}
wstring XMPNameSpaceManager::GetPrefix(const wstring& strFullName)
{
	wstring strPrefix;
	if(mNameSpaceManager.nameSpaceMap.count(strFullName))
		strPrefix = mNameSpaceManager.nameSpaceMap[strFullName];
	return strPrefix;
}
bool XMPNameSpaceManager::UriToQName(const wstring& strUri, wstring& strQName)
{
	bool bStatus = false;
	wstring prefix;
	wstring uri = strUri;
	if(uri.size() > 0)
	{
		// The separator could be # and /
		int nSeperator;
		nSeperator = uri.rfind(L'#');
		if(nSeperator <= 0)
			nSeperator = uri.rfind(L"/");
		if(nSeperator > 0)
		{
			wstring strTag = uri.substr(nSeperator + 1, uri.size() - nSeperator - 1);
			wstring strNameSpace = uri.substr(0,nSeperator + 1);

			prefix = GetPrefix(strNameSpace);
			if(prefix.size() > 0)
			{
				strQName = prefix;
				strQName += L":";
				strQName += strTag;
				bStatus = true;
			}
		}
	}
	return true;
}
bool XMPNameSpaceManager::InsertNameSpace(const wstring& strPrefix, const wstring& strFullName)
{
	mNameSpaceManager.nameSpaceMap[strFullName] = strPrefix;
	return true;
}
