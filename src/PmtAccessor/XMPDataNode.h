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

#ifndef XMP_DATA_NODE_H_

#define XMP_DATA_NODE_H_



#if ((defined _MSC_VER) && (_MSC_VER > 1000))

#pragma once

#endif



#include "EkCompiler.h"

#include "PmtSAX2.h"



//! XMPDataAttribute maintenance an attribute of a XMP data node

class XMPDataAttribute

{

public:

	//! Constructor

	XMPDataAttribute(const wstring& strKey,  const wstring& strValue):key(strKey),value(strValue){}



	//! Returns the key of the attribute

	const wstring& Key() const { return key; }



	//! Returns the value of the attribute

	const wstring& Value() const { return value; }



	//! Update the value of the attribute

	void UpdateValue(const wstring& strValue) { value = strValue; }



	//! Operator to save the attribute to a wostream

	friend wostream& operator << (wostream& outStream, const XMPDataAttribute& attr);

private:

	//! Key of the attribute

	wstring key;



	//! Value of the attribute

	wstring value;

};



//! Operator to save the attribute to a wostream

inline wostream& operator << (wostream& outStream, const XMPDataAttribute& attr)

{

	//key="value"

	outStream << attr.key.c_str() << L"=\"" << attr.value.c_str() << L"\"";

	return outStream;

}



//! XMPDataNameSpace maintenance a name space of XMP

class XMPDataNameSpace

{

public:

	//! Constructor

	XMPDataNameSpace(const wstring& strPrefix, const wstring& strUri):prefix(strPrefix),uri(strUri){}



	//! Returns the prefix of the name space

	const wstring& Prefix() const { return prefix; }



	//! Returns the uri of the name space

	const wstring& Uri() const { return uri; }



	//! Operator to save the name space to a wostream

	friend wostream& operator << (wostream& outStream, const XMPDataNameSpace& nameSpace);

private:

	//! Prefix of the name space

	wstring prefix;



	//! Uri of the name space

	wstring uri;

};



//! Operator to save the name space to a wostream

inline wostream& operator << (wostream& outStream, const XMPDataNameSpace& nameSpace)

{

	//xmlns:prefix="uri"

	outStream << L"xmlns:" << nameSpace.prefix.c_str() << L"=\"" << nameSpace.uri.c_str() << L"\"";

	return outStream;

}

#define RDF_XMP_TAG_NAME L"rdf:RDF"

#define ROOT_XMP_TAG_NAME L"x:xmpmeta"



class PmtXMPSAXParser;



//! XMPDataNode maintenance a xmp node

class XMPDataNode

{

	friend class PmtXMPSAXParser;

public:



	//! Create a root node

	static XMPDataNode* Create();



	static void Destroy(XMPDataNode* pNode){ delete pNode; };

	//! Returns the key of the node

	const wstring& Key() const { return key; }



	//! Returns the content of the node

	const wstring& Content() const { return content; }



	//! Get pointer of the parent node 

	XMPDataNode* Parent() const { return pParent; }



	

	//! Updates the content of the node

	void UpdateContent(const wstring& strContent) { content = strContent; }



	//! Adds a sub node 

	XMPDataNode* AddSubNode(const wstring& strKey);



	//! Adds a sub node with given key and content

	XMPDataNode* AddSubNode(const wstring& strKey,const wstring& strContent);



	//! Adds an attribute with given key and value

	XMPDataAttribute* AddAttribute(const wstring& key, const wstring& value);



	//! adds a name space with given prefix and uri

	XMPDataNameSpace* AddNameSpace(const wstring& prefix, const wstring& uri);

	

	//! Appends a XMP node tree to current tree

	bool AppendNode(const XMPDataNode& node);



	//! Updates the node tree with given node

	bool UpdateNode(const XMPDataNode& node);

	

	//! Returns the XMPDataNode with specified key.

	XMPDataNode* GetXMPNode(const wstring& strKey, bool bCreateIfNotExist = false);



	//! Returns the root node in the node tree

	XMPDataNode* RootNode();



	//! Returns the RDF root node of the node tree

	XMPDataNode* RDFRootNode();



	/*! Gets value of specified key, if the key is found at node element, it is the content of the key,

		if the key if found at attribute, it is the attribute value. If can not find it anywhere, NULL is returned.*/

	const wchar_t* GetKeyValue(const wstring& strKey) const;



	//Get and set composited values. Bag, Seq and Alt.

	bool GetKeyValueSeq(const wstring& strKey, vector<wstring>& vString);

	bool GetKeyValueBag(const wstring& strKey, vector<wstring>& vString);

	bool GetKeyValueLanguage(const wstring& strKey, map<wstring, wstring>& mapString);

	bool GetKeyValueAlt(const wstring& strKey, const wstring& strAttributeName, map<wstring, wstring>& mapString);

	bool UpdateAttributeValue(const wstring& strKey, const wstring& strValue, bool bCreateIfNotExist = false);

	bool UpdateSubKeyValue(const wstring& strKey, const wstring& strValue, bool bCreateIfNotExist = false);

	bool UpdateKeyValue(const wstring& strKey, const wstring& strValue, bool bCreateIfNotExist = false);

	bool UpdateKeyValueSeq(const wstring& strKey, const vector<wstring>& vString, bool bCreateIfNotExist = false);

	bool UpdateKeyValueSeqIndex(const wstring& strKey, const wstring& strValue, unsigned int nIndex, bool bCreateIfNotExist = false);

	bool UpdateKeyValueBag(const wstring& strKey, const vector<wstring>& vString, bool bCreateIfNotExist = false);

	bool UpdateKeyValueBagIndex(const wstring& strKey, const wstring& strValue, unsigned int nIndex, bool bCreateIfNotExist = false);

	bool UpdateKeyValueLanguage(const wstring& strKey, const map<wstring,wstring>& mapString, bool bCreateIfNotExist = false);

	bool UpdateKeyValueAlt(const wstring& strKey, const wstring& strAttributeName, const map<wstring,wstring>& mapString, bool bCreateIfNotExist = false);



	//! Removes the specified in the node tree, it maybe an attribute or a sub node

	bool RemoveKey(const wstring& strKey);



	//! Removes an item of a sequence of values of the node with given key

	bool RemoveKeyVectorIndex(const wstring& strKey, unsigned int nIndex);



	//! Returns the first iterator of the sub node list

	list<XMPDataNode*>::iterator FirstSubNode() { return mSubNodeList.begin(); }



	//! Returns the end iterator of the sub node list

	list<XMPDataNode*>::iterator EndSubNode() { return mSubNodeList.end(); }



	//! Returns the first iterator of the attribute list

	list<XMPDataAttribute*>::iterator FirstAttribute() { return mAttributeList.begin(); }



	//! Returns the end iterator of the attribute list

	list<XMPDataAttribute*>::iterator EndAttribute() { return mAttributeList.end(); }

	

	//! Validate the name spaces for all key of node  and attribute

	bool ValidateNameSpace();

private:

	XMPDataNode();

	//! Constructor

	XMPDataNode(const wstring& strKey);



	//! Destructor

	~XMPDataNode(void);



	//! Update node with given node

	bool Assign(const XMPDataNode& node);



	//! Parent node

	XMPDataNode *pParent;



	//! Key of node

	wstring key;



	//! Content of the node

	wstring content;



	//! Deep of the node in the tree, this will be used when output

	int nDeep;



	//! Attribute list

	list<XMPDataAttribute*> mAttributeList;



	//! Sub node list

	list<XMPDataNode*> mSubNodeList;



	//! Name space list

	list<XMPDataNameSpace*> mNameSpaceList;

	

	//! Adds sub node to the node tree

	XMPDataNode* AddSubNode(XMPDataNode* pSubNode);





	bool GetKeyValueVector(const wstring& strKey, vector<wstring>& vString, bool bSeq = true);

	bool UpdateKeyValueVector(const wstring& strKey, const vector<wstring>& vString, bool bSeq = true, bool bCreateIfNotExist = false);

	bool UpdateKeyValueVectorIndex(const wstring& strKey, const wstring& strValue, unsigned int nIndex, bool bSeq = true, bool bCreateIfNotExist = false);

	

	bool ValidateKey(const wstring& strKey);



	//! Whether the node is a XMP structure

	bool IsStructureNode();



	//! Get the name space of the key

	bool GetNameSpace(wstring& strNameSpace) const;



	//! Split the full name to name space and local name

	bool SplitFullName(const wstring& strFullName, wstring& strNameSpace, wstring& localName) const;



	//! Gets the prefix of the name space

	bool GetNameSpacePrefix(const wstring& strNameSpace, wstring& strPrefix) const;



	//! Gets the QName from the full name

	bool GetQName(const wstring& strFullName, wstring& strQName) const;



	//! Clears all the name spaces, attributes and sub nodes.

	void Clear();



	//! Operator to output the node tree to the wostream

	friend wostream& operator << (wostream& outStream, XMPDataNode& node) ;

};



#endif