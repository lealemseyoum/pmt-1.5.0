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

#include "PmtXMPSAXParser.h"
#include "XMPTypeTranslater.h"
PmtXMPSAXParser::PmtXMPSAXParser(void)
{
	pCurrentNode = NULL;
	pParentNode = NULL;
	bValidXMP = false;
	bNewNode = true;
}

PmtXMPSAXParser::~PmtXMPSAXParser(void)
{
}

bool PmtXMPSAXParser::Parse(const unsigned char* pDataBytes, int nLength, CharacterSet characterSet, XMPDataNode* pXMPDataNode)
{
	pParentNode = pXMPDataNode;
	if(pParentNode != NULL)
		pCurrentNode = new XMPDataNode();

	//It will be set true in endDocument routine.
	bValidXMP = false;
	bNewNode = true;
	PmtSAX2XMLReader* parser = PmtCreateSAX2XMLReader();
	parser->setContentHandler(this);

	// Set the input stream source
	// put it in a parseable format (note: no NULL characters allowed until end)
	MemBufInputSource * memSrc = PmtGetSAX2MemorySource((const unsigned char *const) (pDataBytes),nLength,NULL);

	// parse the stream
#ifdef PMT_USE_XERCES
	memSrc->setEncoding(XMPBlockManager::EncodingTypeName(characterSet));
#endif
	parser->parse(*memSrc);
	PMT_DELETE_SAX2_PARSER(parser);

	//printf("end parse\n");
	return bValidXMP;
}

void PmtXMPSAXParser::startElement(  const   XMLCh* const    uri,
								   const   XMLCh* const    localname,
								   const   XMLCh* const    qname,
								   const PmtSAX2Attributes& attributes)
{
	//wcout << L"start element: uri = " << uri << L"localname = "<< localname << L"qname = " << qname << endl;
	if(!bNewNode)
		pCurrentNode = new XMPDataNode();

	wstring strFullName = XMLChToWideString(uri);
	strFullName += XMLChToWideString(localname);

	pCurrentNode->key = strFullName;
	pParentNode->AddSubNode(pCurrentNode);
	pParentNode = pCurrentNode;

	bNewNode = false;

	//wcout << L"attributes: " << endl;

	int nCountOfAttributes;
	PMT_GETSAX2ATTR_LEN(attributes, nCountOfAttributes);
	int i;
	wstring strValue;
	const XMLCh* szValue;
	for (i = 0; i < nCountOfAttributes; i ++)
	{
		strFullName.empty();
		PmtSAXString* attrURI = PmtGetSAX2AttributeURI(&attributes, i);
		strFullName = XMLChToWideString(attrURI->getXMLCh());
		delete attrURI;

		PmtSAXString* attrLocalName = PmtGetSAX2AttributeLocalName(&attributes,i);
		strFullName += XMLChToWideString(attrLocalName->getXMLCh());
		delete attrLocalName;

		if(strFullName.size() <= 0)
			continue;

		PMT_GETSAX2ATTR_VALUE(attributes,i,szValue);
		strValue = XMLChToWideString(szValue);
		pCurrentNode->AddAttribute(strFullName,strValue);

		//wcout << strFullName << L" = " << strValue << endl;
	}
}

// The callback function for the characters inbetween
void PmtXMPSAXParser::xcharacters(const XMLCh* const chars, const unsigned int length) 
{
	if(pCurrentNode)
	{
		wstring strContent = XMLChToWideString(chars);
		strContent = strContent.substr(0,length);
		//wcout << L"characters: " << strContent << L"length = " << length << endl;
		wstring strExistContent = pCurrentNode->Content();
		if(strExistContent.size() > 0)
		{
			if((length == 1) && (strContent[0] == L'\n'))
				return;
			if(strContent[0] == L'\n')
				return pCurrentNode->UpdateContent(strExistContent + strContent.substr(1,strContent.size() - 1));
			return pCurrentNode->UpdateContent(strExistContent + strContent);
		}
		else
		{
			int nFirst = strContent.find_first_not_of(L" \t\r\n");
			if(nFirst >= 0)
			{
				strContent = strContent.substr(nFirst, strContent.size() - nFirst);
				pCurrentNode->UpdateContent(strContent);
			}
		}
	}
}

// The callback function for the end of an element.
void PmtXMPSAXParser::endElement( const XMLCh* const namespaceUri,
								 const XMLCh* const localName,
								 const XMLCh* const qname)
{
	pCurrentNode = pCurrentNode->Parent();
	pParentNode = pParentNode->Parent();
	//wcout << L"endelement: " << localName << endl;
}
void  PmtXMPSAXParser::xendDocument()
{
	bValidXMP = true;
	//wcout << L"end document" << endl;
}

void PmtXMPSAXParser::startPrefixMapping
(
 const	XMLCh* const	prefix,
 const	XMLCh* const	uri
 ) 
{
	if(pCurrentNode)
	{
		if(!bNewNode)
		{
			pCurrentNode = new XMPDataNode();
			bNewNode = true;
		}
	}
	if(pCurrentNode)
		pCurrentNode->AddNameSpace(XMLChToWideString(prefix), XMLChToWideString(uri));
	//wcout << L"start prefix mapping: " << prefix << L" = " << uri << endl;
}
