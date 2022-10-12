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
#include "XMPDataNode.h"
#include "XMPCommonTags.h"
#include "XMPNameSpaceManager.h"
#include "XMLEncoder.h"
#define XML_NAMESPACE_PREFIX L"http://www.w3.org/XML/1998/namespace"


XMPDataNode::XMPDataNode():pParent(NULL),nDeep(-1)
{
}
XMPDataNode::XMPDataNode(const wstring& strKey):pParent(NULL),nDeep(-1),key(strKey)
{
}

bool XMPDataNode::Assign(const XMPDataNode& node)
{
	key = node.key;
	content = node.content;

	list<XMPDataAttribute*>::const_iterator itAttr;
	for(itAttr = node.mAttributeList.begin(); itAttr != node.mAttributeList.end(); itAttr ++)
	{
		if(AddAttribute((*itAttr)->Key(), (*itAttr)->Value()) == NULL)
			return false;
	}
	list<XMPDataNameSpace*>::const_iterator itName;
	for(itName = node.mNameSpaceList.begin(); itName != node.mNameSpaceList.end(); itName ++)
	{
		if(AddNameSpace((*itName)->Prefix(), (*itName)->Uri()) == NULL)
			return false;
	}
	list<XMPDataNode*>::const_iterator itSub;
	for (itSub = node.mSubNodeList.begin(); itSub != node.mSubNodeList.end(); itSub ++)
	{
		XMPDataNode* pSub = new XMPDataNode();
		pSub->pParent = this;
		pSub->nDeep = nDeep + 1;
		mSubNodeList.push_back(pSub);
		if(pSub->Assign(**itSub) == false)
			return false;
	}
	return true;
}
XMPDataNode::~XMPDataNode(void)
{
	Clear();
}
XMPDataAttribute* XMPDataNode::AddAttribute(const wstring& key, const wstring& value)
{
	XMPDataAttribute *pNewAttribute = new XMPDataAttribute(key, value);
	mAttributeList.push_back(pNewAttribute);
	return pNewAttribute;
}
XMPDataNameSpace* XMPDataNode::AddNameSpace(const wstring& prefix, const wstring& uri)
{
	XMPDataNameSpace *pNewNameSpace = new XMPDataNameSpace(prefix, uri);
	mNameSpaceList.push_back(pNewNameSpace);
	return pNewNameSpace;
}
XMPDataNode* XMPDataNode::AddSubNode(const wstring& strKey )
{
	XMPDataNode* pSubNode = new XMPDataNode(strKey);
	pSubNode = AddSubNode(pSubNode);
	return pSubNode;
}

XMPDataNode* XMPDataNode::AddSubNode(XMPDataNode* pSubNode)
{
	pSubNode->nDeep = nDeep + 1;
	pSubNode->pParent = this;
	mSubNodeList.push_back(pSubNode);
	return pSubNode;
}
XMPDataNode* XMPDataNode::AddSubNode(const wstring& strKey,const wstring& strContent)
{
	XMPDataNode* pNode = AddSubNode(strKey);
	pNode->UpdateContent(strContent);
	return pNode;
}
XMPDataNode* XMPDataNode::GetXMPNode(const wstring& strKey, bool bCreateIfNotExist)
{
	if (key == strKey)
		return this;
	list<XMPDataNode*>::iterator itSub;
	XMPDataNode* pFoundNode;
	for (itSub = mSubNodeList.begin(); itSub != mSubNodeList.end(); itSub ++)
	{
		pFoundNode = (*itSub)->GetXMPNode(strKey);
		if(pFoundNode != NULL)
			return pFoundNode;
	}

	if(bCreateIfNotExist)
		return AddSubNode(strKey);
	else
		return NULL;
}
bool XMPDataNode::UpdateNode(const XMPDataNode& node)
{
	bool bRetValue = false;
	if(key == node.Key())
	{
		Clear();
		bRetValue = Assign(node);
	}
	return bRetValue;
}
void XMPDataNode::Clear()
{
	list<XMPDataAttribute*>::const_iterator itAttr;
	for(itAttr = mAttributeList.begin(); itAttr != mAttributeList.end(); itAttr ++)
	{
		delete *itAttr;
	}
	mAttributeList.clear();
	list<XMPDataNameSpace*>::const_iterator itName;
	for (itName = mNameSpaceList.begin(); itName != mNameSpaceList.end(); itName ++)
	{
		delete *itName;
	}
	mNameSpaceList.clear();
	list<XMPDataNode*>::const_iterator itSub;
	for (itSub = mSubNodeList.begin(); itSub != mSubNodeList.end(); itSub ++)
	{
		delete *itSub;
	}
	mSubNodeList.clear();
}
bool XMPDataNode::AppendNode(const XMPDataNode& node)
{
	if(key != node.key)
	{
		XMPDataNode* pSubNode = GetXMPNode(node.key);
		if(pSubNode)
			return pSubNode->AppendNode(node);
		else
			return false;
	}
	if(IsStructureNode())
		return UpdateNode(node);
	UpdateContent(node.Content());
	//Add all attributes
	list<XMPDataAttribute*>::const_iterator itAttr;
	for (itAttr = node.mAttributeList.begin(); itAttr != node.mAttributeList.end(); itAttr ++)
	{
		UpdateAttributeValue((*itAttr)->Key(), (*itAttr)->Value(), true);
	}

	//Add all namespace.
	list<XMPDataNameSpace*>::const_iterator itName;
	list<XMPDataNameSpace*>::const_iterator itLocalName;
	for (itName = node.mNameSpaceList.begin(); itName != node.mNameSpaceList.end(); itName ++)
	{
		for (itLocalName = mNameSpaceList.begin(); itLocalName != mNameSpaceList.end(); itLocalName ++)
		{			
			if((*itLocalName)->Uri() == (*itName)->Uri())
				break;
		}
		if(itLocalName == mNameSpaceList.end())
			AddNameSpace((*itName)->Prefix(), (*itName)->Uri());
	}

	//Add all subnode.
	list<XMPDataNode*>::const_iterator itSub;
	list<XMPDataNode*>::iterator itLocalSub;
	for (itSub = node.mSubNodeList.begin(); itSub != node.mSubNodeList.end(); itSub ++)
	{
		for (itLocalSub = mSubNodeList.begin(); itLocalSub != mSubNodeList.end(); itLocalSub ++)
		{
			if((*itLocalSub)->Key() == (*itSub)->Key())
			{
				if((*itLocalSub)->AppendNode(**itSub))
					break;
				else
					return false;
			}
		}
		if(itLocalSub == mSubNodeList.end())
		{
			XMPDataNode* pSubNode = AddSubNode((*itSub)->Key(), (*itSub)->Content());
			if(!(pSubNode->AppendNode(**itSub)))
					return false;
		}
	}
	return true;
}
XMPDataNode* XMPDataNode::Create()
{
	return new XMPDataNode();
}
XMPDataNode* XMPDataNode::RootNode()
{
	if(pParent == NULL)
		return GetXMPNode(ROOT_XMP_TAG_NAME);
	XMPDataNode* parent = pParent;
	while(parent->Parent() != NULL)
	{
		parent = parent->Parent();
	}
	return parent->GetXMPNode(ROOT_XMP_TAG_NAME);
}
XMPDataNode* XMPDataNode::RDFRootNode()
{
	if(pParent == NULL)
		return GetXMPNode(RDF_XMP_TAG_NAME);
	XMPDataNode* parent = pParent;
	while(parent->Parent() != NULL)
	{
		parent = parent->Parent();
	}
	return parent->GetXMPNode(RDF_XMP_TAG_NAME);
}

const wchar_t* XMPDataNode::GetKeyValue(const wstring& strKey) const
{
	const wchar_t* strValue = NULL;
	if(key == strKey)
		return content.c_str();
	list<XMPDataAttribute*>::const_iterator itAttr;
	for (itAttr = mAttributeList.begin(); itAttr != mAttributeList.end(); itAttr ++ )
	{
		if((*itAttr)->Key() == strKey)
			return (*itAttr)->Value().c_str();
	}
	list<XMPDataNode*>::const_iterator itSub;
	for (itSub = mSubNodeList.begin(); itSub != mSubNodeList.end(); itSub ++)
	{
		strValue = (*itSub)->GetKeyValue(strKey);
		if(strValue != NULL)
			return strValue;
	}
	return NULL;
}

bool XMPDataNode::GetNameSpace(wstring& strNameSpace) const
{
	wstring strLocalName;
	return SplitFullName(key, strNameSpace, strLocalName);
}
bool XMPDataNode::GetNameSpacePrefix(const wstring& strNameSpace, wstring& strPrefix) const
{
	bool bGot = false;
	list<XMPDataNameSpace*>::const_iterator itNameSpace;
	for (itNameSpace = mNameSpaceList.begin(); itNameSpace != mNameSpaceList.end(); itNameSpace ++)
	{
		if(strNameSpace == (*itNameSpace)->Uri())
		{
			strPrefix = (*itNameSpace)->Prefix();
			return true;
		}
	}
	if(Parent())
		return Parent()->GetNameSpacePrefix(strNameSpace, strPrefix);
	return false;
}
bool XMPDataNode::SplitFullName(const wstring& strFullName, wstring& strNameSpace, wstring& localName) const
{
	bool bRetValue = false;
	if(strFullName.size() > 0)
	{
		int nSeperator;
		nSeperator = strFullName.rfind(L'#');
		if(nSeperator <= 0)
			nSeperator = strFullName.rfind(L"/");
		if(nSeperator > 0)
		{
			strNameSpace = strFullName.substr(0,nSeperator + 1);
			localName = strFullName.substr(nSeperator + 1, strFullName.size() - nSeperator - 1);
			bRetValue = true;
		}
	}
	return bRetValue;
}
bool XMPDataNode::GetQName(const wstring& strFullName, wstring& strQName) const
{
	bool bRetValue = false;
	wstring strNameSpace, strLocalName;
	wstring strXMLPrefix = XML_NAMESPACE_PREFIX;
	if(SplitFullName(strFullName, strNameSpace, strLocalName) )
	{
		wstring strPrefix;
		if(GetNameSpacePrefix(strNameSpace, strPrefix))
		{
			strQName = strPrefix + L":";
			strQName += strLocalName;
			bRetValue = true;
		}
		else
		{
			wstring::size_type nPos = strFullName.find(strXMLPrefix);
			if( nPos != wstring::npos)
			{
				strQName = strFullName;
				strQName.replace(nPos, strXMLPrefix.length(), L"xml:");
				bRetValue = true;
			}
		}
	}
	return bRetValue;
}
bool XMPDataNode::ValidateNameSpace()
{
	ValidateKey(Key());
	list<XMPDataAttribute*>::iterator itAttr;
	for (itAttr = mAttributeList.begin(); itAttr != mAttributeList.end(); itAttr ++)
	{
		ValidateKey((*itAttr)->Key());
	}
	list<XMPDataNode*>::iterator itSub;
	for (itSub = mSubNodeList.begin(); itSub != mSubNodeList.end(); itSub ++)
	{
		(*itSub)->ValidateNameSpace();
	}
	return true;
}
bool XMPDataNode::ValidateKey(const wstring& strKey)
{
	bool bRetValue = false;
	wstring strNameSpace, strLocalName;
	wstring strXMLPrefix = XML_NAMESPACE_PREFIX;
	if(SplitFullName(strKey, strNameSpace, strLocalName))
	{
		wstring strPrefix;
		if(GetNameSpacePrefix(strNameSpace, strPrefix))
			bRetValue = true;
		else
		{
			wstring::size_type nPos = strKey.find(strXMLPrefix);
			if(nPos == wstring::npos)
			{
				wstring strPrefix = XMPNameSpaceManager::GetPrefix(strNameSpace);
				if(strPrefix.size() > 0)
				{
					AddNameSpace(strPrefix, strNameSpace);
					bRetValue = true;
				}
			}	
			else
				bRetValue = true;
		}
	}
	return bRetValue;
}
bool XMPDataNode::UpdateAttributeValue(const wstring& strKey, const wstring& strValue, bool bCreateIfNotExist /*= false*/)
{
	bool bRet = false;
	list<XMPDataAttribute*>::iterator itAttr;
	for (itAttr = mAttributeList.begin(); itAttr != mAttributeList.end(); itAttr ++ )
	{
		if((*itAttr)->Key() == strKey)
		{
			(*itAttr)->UpdateValue(strValue);
			return true;
		}
	}
	list<XMPDataNode*>::iterator itSub;
	for (itSub = mSubNodeList.begin(); itSub != mSubNodeList.end(); itSub ++)
	{
		bRet = (*itSub)->UpdateAttributeValue(strKey,strValue);
		if(bRet)
			return bRet;
	}
	if(bCreateIfNotExist)
		return AddAttribute(strKey,strValue) != NULL;
	else
		return false;
}
bool XMPDataNode::UpdateSubKeyValue(const wstring& strKey, const wstring& strValue, bool bCreateIfNotExist /*= false*/)
{
	if(key == strKey)
	{
		content = strValue;
		return true;
	}
	bool bRet = false;
	
	list<XMPDataNode*>::iterator itSub;
	for (itSub = mSubNodeList.begin(); itSub != mSubNodeList.end(); itSub ++)
	{
		bRet = (*itSub)->UpdateSubKeyValue(strKey,strValue);
		if(bRet)
			return bRet;
	}
	if(bCreateIfNotExist)
		return AddSubNode(strKey,strValue) != NULL;
	else
		return bRet;
}
bool XMPDataNode::UpdateKeyValue(const wstring& strKey, const wstring& strValue, bool bCreateIfNotExist /*= false*/)
{
	if(key == strKey)
	{
		content = strValue;
		return true;
	}
	bool bRet = false;
	list<XMPDataAttribute*>::iterator itAttr;
	for (itAttr = mAttributeList.begin(); itAttr != mAttributeList.end(); itAttr ++ )
	{
		if((*itAttr)->Key() == strKey)
		{
			(*itAttr)->UpdateValue(strValue);
			return true;
		}
	}
	list<XMPDataNode*>::iterator itSub;
	for (itSub = mSubNodeList.begin(); itSub != mSubNodeList.end(); itSub ++)
	{
		bRet = (*itSub)->UpdateKeyValue(strKey,strValue);
		if(bRet)
			return bRet;
	}
	if(bCreateIfNotExist)
		return AddAttribute(strKey,strValue) != NULL;
	else
		return bRet;
}
bool XMPDataNode::GetKeyValueSeq(const wstring& strKey, vector<wstring>& vString)
{
	return GetKeyValueVector(strKey, vString, true);
}
bool XMPDataNode::GetKeyValueBag(const wstring& strKey, vector<wstring>& vString)
{
	return GetKeyValueVector(strKey, vString, false);
}
bool XMPDataNode::UpdateKeyValueSeq(const wstring& strKey, const vector<wstring>& vString, bool bCreateIfNotExist /*= false*/)
{
	return UpdateKeyValueVector(strKey, vString, true, bCreateIfNotExist);
}
bool XMPDataNode::UpdateKeyValueSeqIndex(const wstring& strKey, const wstring& strValue, unsigned int nIndex, bool bCreateIfNotExist /*= false*/)
{
	return UpdateKeyValueVectorIndex(strKey, strValue, nIndex, true, bCreateIfNotExist);
}
bool XMPDataNode::UpdateKeyValueBag(const wstring& strKey, const vector<wstring>& vString, bool bCreateIfNotExist /*= false*/)
{	
	return UpdateKeyValueVector(strKey, vString, false, bCreateIfNotExist);
}
bool XMPDataNode::UpdateKeyValueBagIndex(const wstring& strKey, const wstring& strValue, unsigned int nIndex, bool bCreateIfNotExist /*= false*/)
{
	return UpdateKeyValueVectorIndex(strKey, strValue, nIndex, false, bCreateIfNotExist);
}
bool XMPDataNode::GetKeyValueVector(const wstring& strKey, vector<wstring>& vString, bool bSeq /*= true*/)
{
	bool bRetValue = false;
	XMPDataNode *pNode = GetXMPNode(strKey);
	if(pNode)
	{
		//There should be a sub node with key rdf:Seq or rdf:bag.
		XMPDataNode* pSeq;
		if(bSeq)
			pSeq = pNode->GetXMPNode(XMPMETADATA_SEQTAG);
		else
			pSeq = pNode->GetXMPNode(XMPMETADATA_BAGTAG);
		if(pSeq != NULL)
		{
			list<XMPDataNode*>::const_iterator itSub;;
			for (itSub = pSeq->mSubNodeList.begin(); itSub != pSeq->mSubNodeList.end(); itSub ++)
			{
				if((*itSub)->Key() == XMPMETADATA_LITAG)
					vString.push_back((*itSub)->content);
				else
					break;
			}
			bRetValue = (itSub == pSeq->mSubNodeList.end());
		}
	}
	return bRetValue;
}
bool XMPDataNode::UpdateKeyValueVector(const wstring& strKey, const vector<wstring>& vString, bool bSeq /*= true*/, bool bCreateIfNotExist /*= false*/)
{
	bool bRetValue = false;
	XMPDataNode* pNode = GetXMPNode(strKey, bCreateIfNotExist);
	XMPDataNode* pBag = NULL;
	if(pNode)
	{
		if(bSeq)
			pBag = pNode->GetXMPNode(XMPMETADATA_SEQTAG, bCreateIfNotExist);
		else
			pBag = pNode->GetXMPNode(XMPMETADATA_BAGTAG, bCreateIfNotExist);
	}
	if(pBag)
	{
		list<XMPDataNode*>::iterator itSub;
		for (itSub = pBag->mSubNodeList.begin(); itSub != pBag->mSubNodeList.end(); itSub ++)
		{
			delete *itSub;
		}
		pBag->mSubNodeList.clear();
		vector<wstring>::const_iterator itString;
		for (itString = vString.begin(); itString != vString.end(); itString ++)
		{
			if(!pBag->AddSubNode(XMPMETADATA_LITAG,(*itString).c_str()))
				break;
		}
		bRetValue = (itString == vString.end());
	}
	return bRetValue;
}
bool XMPDataNode::UpdateKeyValueVectorIndex(const wstring& strKey, const wstring& strValue, unsigned int nIndex, bool bSeq /*= true*/, bool bCreateIfNotExist /*= false*/)
{
	bool bRetValue = false;
	XMPDataNode* pNode = GetXMPNode(strKey, bCreateIfNotExist);
	XMPDataNode* pBag = NULL;
	if(pNode)
	{
		if(bSeq)
			pBag = pNode->GetXMPNode(XMPMETADATA_SEQTAG, bCreateIfNotExist);
		else
			pBag = pNode->GetXMPNode(XMPMETADATA_BAGTAG, bCreateIfNotExist);
	}
	if(pBag)
	{
		list<XMPDataNode*>::iterator itSub;
		unsigned int nCount = 0;
		for (itSub = pBag->mSubNodeList.begin(); itSub != pBag->mSubNodeList.end(); itSub ++)
		{
			if(nCount == nIndex)
			{
				(*itSub)->UpdateContent(strValue);
				bRetValue = true;
				break;
			}
			nCount ++;
		}
		if(!bRetValue && bCreateIfNotExist)
		{
			bRetValue = (pBag->AddSubNode(XMPMETADATA_LITAG,strValue) != NULL);
		}
	}
	return bRetValue;
}
bool XMPDataNode::UpdateKeyValueLanguage(const wstring& strKey, const map<wstring,wstring>& mapString, bool bCreateIfNotExist /*= false*/)
{
	return UpdateKeyValueAlt(strKey, XMLMETADATA_LANGTAG, mapString, bCreateIfNotExist);
}
bool XMPDataNode::UpdateKeyValueAlt(const wstring& strKey, const wstring& strAttributeName, const map<wstring,wstring>& mapString, bool bCreateIfNotExist /*= false*/)
{
	XMPDataNode* pNode = GetXMPNode(strKey, bCreateIfNotExist);
	if(pNode == NULL)
		return false;
	
	pNode->RemoveKey(XMPMETADATA_ALTTAG);
	
	XMPDataNode* pAltNode = pNode->AddSubNode(XMPMETADATA_ALTTAG);
	//Make sure the x-default is at the first.
	XMPDataNode *pItemNode = NULL;
	if(mapString.count(L"x-default"))
	{
		pItemNode = pAltNode->AddSubNode(XMPMETADATA_LITAG);
		pItemNode->AddAttribute(strAttributeName, L"x-default");
		pItemNode->UpdateContent((mapString.find(L"x-default"))->second.c_str());
	}
	map<wstring, wstring>::const_iterator itStringPair;
	for (itStringPair = mapString.begin(); itStringPair != mapString.end(); itStringPair ++)
	{
		if((*itStringPair).first == L"x-default")
			continue;
		pItemNode = pAltNode->AddSubNode(XMPMETADATA_LITAG);
		pItemNode->AddAttribute(strAttributeName, (*itStringPair).first.c_str());
		pItemNode->UpdateContent((*itStringPair).second.c_str());
	}
	return true;
}

bool XMPDataNode::GetKeyValueLanguage(const wstring& strKey, map<wstring, wstring>& mapString)
{
	return GetKeyValueAlt(strKey, XMLMETADATA_LANGTAG, mapString);
}
bool XMPDataNode::GetKeyValueAlt(const wstring& strKey, const wstring& strAttributeName, map<wstring, wstring>& mapString)
{
	XMPDataNode* pNode = GetXMPNode(strKey);
	if(pNode == NULL)
		return false;
	mapString.clear();
	pNode = pNode->GetXMPNode(XMPMETADATA_ALTTAG);
	if(pNode == NULL)
		return false;
	list<XMPDataNode*>::const_iterator itSub;
	const wchar_t* strValue;
	pair<map<wstring,wstring>::const_iterator, bool> retPair;
	for (itSub = pNode->mSubNodeList.begin(); itSub != pNode->mSubNodeList.end(); itSub ++)
	{
		if(((*itSub)->Key() == XMPMETADATA_LITAG) && ((strValue = (*itSub)->GetKeyValue(strAttributeName)) != NULL))
		{
			retPair = mapString.insert(make_pair(wstring(strValue), wstring((*itSub)->Content())));
			//mapString[strValue] = (*itSub)->Content();
			//Make sure it has been inserted.
			if(!retPair.second)
			{
				mapString.clear();
				return false;
			}
		}
	}
	return true;
}
bool XMPDataNode::RemoveKey(const wstring& strKey)
{
	bool bRet = false;
	list<XMPDataAttribute*>::iterator itAttr;
	for (itAttr = mAttributeList.begin(); itAttr != mAttributeList.end(); itAttr ++ )
	{
		if((*itAttr)->Key() == strKey)
		{
			delete *itAttr;
			mAttributeList.erase(itAttr);
			return true;
		}
	}
	list<XMPDataNode*>::iterator itSub;
	for (itSub = mSubNodeList.begin(); itSub != mSubNodeList.end(); itSub ++)
	{
		if((*itSub)->Key() == strKey)
		{
			delete *itSub;
			mSubNodeList.remove(*itSub);
			return true;
		}
		bRet = (*itSub)->RemoveKey(strKey);
		if(bRet)
			return bRet;
	}
	return bRet;
}
bool XMPDataNode::RemoveKeyVectorIndex(const wstring& strKey, unsigned int nIndex)
{
	bool bRet = false;
	list<XMPDataNode*>::iterator itSub;
	for (itSub = mSubNodeList.begin(); itSub != mSubNodeList.end(); itSub ++)
	{
		if((*itSub)->Key() == strKey)
		{
			XMPDataNode* pBag = NULL;
			pBag = (*itSub)->GetXMPNode(XMPMETADATA_SEQTAG, false);
			if(pBag == NULL)
				pBag = (*itSub)->GetXMPNode(XMPMETADATA_BAGTAG, false);
			if(pBag)
			{
				list<XMPDataNode*>::iterator itItem;
				unsigned int nCount = 0;
				for (itItem = pBag->mSubNodeList.begin(); itItem != pBag->mSubNodeList.end(); itItem ++)
				{
					if(nCount == nIndex)
					{
						delete *itItem;
						pBag->mSubNodeList.remove(*itItem);
						break;
					}
				}
				if(pBag->mSubNodeList.size() == 0)
				{
					RemoveKey(strKey);
				}
				bRet = true;
				break;
			}
			
		}
		bRet = (*itSub)->RemoveKeyVectorIndex(strKey, nIndex);
		if(bRet)
			break;
	}
	return bRet;
}
bool XMPDataNode::IsStructureNode()
{
	bool bRetValue = false;
	if(key == XMPMETADATA_ALTTAG)
		bRetValue = true;
	else if(key == XMPMETADATA_BAGTAG)
		bRetValue = true;
	else if(key == XMPMETADATA_SEQTAG)
		bRetValue = true;
	return bRetValue;
}
wostream&  operator << (wostream& outStream, XMPDataNode& node)
{
	wstring strSpace;//(' ',node.nDeep);
	wstring strNodeQName;
	node.GetQName(node.Key(), strNodeQName);
	strSpace.assign(node.nDeep,L' ');
	wstring strSubSpace;
	strSubSpace.assign(node.nDeep + 1, L' ');
	outStream << strSpace << L"<" << strNodeQName;
	
	//If the total count of the name space and attribute is not bigger tham 2, then output them at one line.
	if(node.mAttributeList.size() + node.mNameSpaceList.size() <= 2)
	{
		list<XMPDataNameSpace*>::iterator itNameSpace;
		for (itNameSpace = node.mNameSpaceList.begin(); itNameSpace != node.mNameSpaceList.end(); itNameSpace ++)
		{
			outStream << L" " << **itNameSpace ;
		}

		wstring strQName;
		list<XMPDataAttribute*>::iterator itAttr;
		for (itAttr = node.mAttributeList.begin(); itAttr != node.mAttributeList.end(); itAttr ++ )
		{
			//pAttributes = *it;
			if(!node.GetQName((*itAttr)->Key(), strQName))
				strQName = (*itAttr)->Key();
			outStream << L" "  << strQName << L"=\"" << (*itAttr)->Value() << L"\"";
		}
	}
	else
	{
		//Output all the namespaces.
		list<XMPDataNameSpace*>::iterator itNameSpace;
		for (itNameSpace = node.mNameSpaceList.begin(); itNameSpace != node.mNameSpaceList.end(); itNameSpace ++)
		{
			outStream << endl << strSubSpace << **itNameSpace ;
		}

		wstring strQName;
		list<XMPDataAttribute*>::iterator itAttr;
		for (itAttr = node.mAttributeList.begin(); itAttr != node.mAttributeList.end(); itAttr ++ )
		{
			//pAttributes = *it;
			if(!node.GetQName((*itAttr)->Key(), strQName))
				strQName = (*itAttr)->Key();
			outStream << endl << strSubSpace << strQName << L"=\"" << (*itAttr)->Value() << L"\"";
		}
	}
	
	if((node.mSubNodeList.size() > 0) || node.content.size() > 0)
	{
		outStream << L">";
		if(node.mSubNodeList.size() > 0)
		{
			list<XMPDataNode*>::iterator itSub;
			for (itSub = node.mSubNodeList.begin(); itSub != node.mSubNodeList.end(); itSub ++)
			{
				outStream << endl << **itSub;
			}
			outStream << endl << strSpace << L"</" << strNodeQName << ">";
		}
		else //It is assumed that an element can not have both sub element and content.
			outStream << CXMLEncoder::Encode(node.content) << L"</" << strNodeQName << ">";
			//outStream << node.content << L"</" << strNodeQName << ">";
	}
	else
	{
		outStream << L"/>";
	}
	
	return outStream;
}