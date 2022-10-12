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
#ifndef XMP_SPECIAL_METADATAS_H
#define XMP_SPECIAL_METADATAS_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#include "PmtCommon.h"
#include <vector>
using namespace std;
//! XMPSpecialMetadataElement maintenance a special element, consist of a key, a tag and the member name of the tag
class XMPSpecialMetadataElement
{
public:
	//! PMT metadata name
	SPmtString mKey;

	//! XMP structure name
	SPmtString mTag;

	//! Member name of the tag
	SPmtString mMember;

	//! Constructor
	XMPSpecialMetadataElement(const char* strKey, const char* strTag, const char* strMember):mKey(strKey),mTag(strTag),mMember(strMember){}

	//! Constructor
	XMPSpecialMetadataElement(const SPmtString& strKey, const SPmtString& strTag, const SPmtString& strMember):mKey(strKey),mTag(strTag),mMember(strMember){}
};
typedef vector<XMPSpecialMetadataElement*> XMPSpecialMetadataElements;

class XMPSpecialMetadataManager
{
public:
	//! Find the special metadata element by PMT metadata name
	static XMPSpecialMetadataElement* FindElementByKey(const SPmtString& strKey);

private:
	//! Constructor
	XMPSpecialMetadataManager();

	//! Destructor
	~XMPSpecialMetadataManager();

	//! Special metadata vector
	XMPSpecialMetadataElements mVElements;

	//! Initialize all the special elements
	void Initialize();

	//! Destroy all the element and release resource
	void Destroy();

	//! Instance of the manager
	static XMPSpecialMetadataManager mSingle;
};

#endif