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

#ifndef _XMP_SAX_PARSER_H_
#define _XMP_SAX_PARSER_H_

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#include "XMPBlockManager.h"
#include "XMPDataNode.h"
#include "PmtSAX2.h"
//! PmtXMPSAXParser is a class to parse XMP XML file
class PmtXMPSAXParser:public PmtSAX2DefaultHandler
{
public:
	PmtXMPSAXParser(void);
	~PmtXMPSAXParser(void);

	//! This function parse a memory block to a DataNode tree
	bool Parse(const unsigned char* pDataBytes, int nLength, CharacterSet characterSet, XMPDataNode* pXMPDataNode);

	// The callback function for the start of a new element.
	void startElement(const XMLCh* const uri,
		const XMLCh* const localname,
		const   XMLCh* const qname,
		const PmtSAX2Attributes& attributes);

	// The callback function for the characters in between
	virtual void xcharacters(const XMLCh* const chars, const unsigned int length);

	// The callback function for the end of an element.
	void endElement( const XMLCh* const namespaceUri,
		const XMLCh* const localName,
		const XMLCh* const qname);

	virtual void xendDocument();

	virtual void startPrefixMapping
		(
		const XMLCh* const prefix,
		const XMLCh* const uri
		) ;
private:
	bool bNewNode;
	XMPDataNode *pCurrentNode;
	XMPDataNode* pParentNode;
	bool bValidXMP;
};

#endif