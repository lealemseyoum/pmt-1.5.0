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
#ifndef XMP_NAMESPACE_MANAGER__H
#define XMP_NAMESPACE_MANAGER__H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif
#include "EkCompiler.h"
#include "PmtSAX2.h"

//! XMPNameSpaceManager offers some functions to manager the XMP name space
class XMPNameSpaceManager
{
public:
	//! Returns the full name of the given prefix
	static wstring GetFullName(const wstring& strPrefix);

	//! Returns the prefix of the given full name
	static wstring GetPrefix(const wstring& strFullName);

	//! Gets full QName of the given uri
	static bool UriToQName(const wstring& strUri, wstring& strQName);

	//! Insert a name space to the manager
	static bool InsertNameSpace(const wstring& strPrefix, const wstring& strFullName);
private:
	//! Map between prefix and full name
	map<wstring, wstring> nameSpaceMap;

	//! Global manager instance
	static XMPNameSpaceManager mNameSpaceManager;
protected:
	//! Constructor
	XMPNameSpaceManager(void);

	//! Destructor
	~XMPNameSpaceManager(void);
};


#endif