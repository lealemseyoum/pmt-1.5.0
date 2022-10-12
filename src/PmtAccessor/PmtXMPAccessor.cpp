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
* Creation Date: 10/10/2009
*
* Original Author: 
* Philip Wu   hanbing.wu@kodak.com
* 
* Contributor(s):
*/
#include <sstream>
#include <math.h>
#include <stdio.h>

#include "PmtXMPAccessor.h"
#include "XMPCommonTags.h"
#include "XMPTypeTranslater.h"
#include "PmtMetadataT.h"
#include "XMPSpecialMetadatas.h"
#include "PmtRootMetadata.h"

PmtXMPAccessor::PmtXMPAccessor(void):PmtAccessor(PMT_FORMAT_XMP)
{
	mFileHandle = NULL;
	mStartPosition = 0;
	mOpenMode = PMT_READONLY;
	mpXMPNode = NULL;
}

PmtXMPAccessor::~PmtXMPAccessor(void)
{
	close();
}
bool PmtXMPAccessor::open(const char *name, PmtOpenMode mode /*= PMT_READONLY*/)
{
	bool bSuccessed = false;
	close();
	if(Initialize())
	{
		mOpenMode = mode;
		char *szOpenMode = NULL;
		if(mode == PMT_READONLY)
			szOpenMode = "rb";
		else if(mode == PMT_CREATE)
		{
			szOpenMode = "wb";
			CreateEmptyBlock();
		}
		else if(mode == PMT_READWRITE)
			szOpenMode = "rb+";

		mFileHandle = fopen(name, szOpenMode);
		if(mFileHandle > 0)
		{
			mFileName = name;
			if(mode != PMT_CREATE)
			{
				mStartPosition = mBlockManager.FindXMPBlock(mFileHandle, 0);
				if(mStartPosition >= 0)
					bSuccessed = ParseXMPBlock(mBlockManager.XMPMetaData(), mBlockManager.XMPMetaDataSize(), mBlockManager.EncodingType());
				if(bSuccessed)
					bSuccessed = BuildPrivateData();
			}
		}
	}
	return bSuccessed;
}
bool PmtXMPAccessor::append( const char *name )
{
	bool bSuccessed = false;
	FILE* pAppendFile = fopen(name, "rb");
	if(pAppendFile > 0)
	{
		XMPBlockManager manager;
		int nStart = manager.FindXMPBlock(pAppendFile, 0);
		if(nStart >= 0)
			bSuccessed = ParseXMPBlock(manager.XMPMetaData(), manager.XMPMetaDataSize(), manager.EncodingType());
	}
	return bSuccessed;
}
bool PmtXMPAccessor::append( const unsigned char * buf, const unsigned int bufSize )
{
	bool bSuccessed = false;
	XMPBlockManager manager;
	int nStart = manager.FindXMPBlock(buf, bufSize);
	if(nStart >= 0)
		bSuccessed = ParseXMPBlock(manager.XMPMetaData(), manager.XMPMetaDataSize(), manager.EncodingType());
	return bSuccessed;
}
bool PmtXMPAccessor::open(const unsigned char * buf, const unsigned int bufSize, PmtOpenMode mode /*= PMT_READONLY*/)
{
	bool bSuccessed = false;
	close();
	mOpenMode = mode;
	if(Initialize())
	{
		mStartPosition = mBlockManager.FindXMPBlock(buf, bufSize);
		if(mStartPosition >= 0)
			bSuccessed = ParseXMPBlock(mBlockManager.XMPMetaData(), mBlockManager.XMPMetaDataSize(), mBlockManager.EncodingType());
		if(bSuccessed)
			bSuccessed = BuildPrivateData();
	}
	return bSuccessed;	
}
bool PmtXMPAccessor::open()
{
	close();
	return CreateEmptyBlock();
}
bool PmtXMPAccessor::ParseXMPBlock(const unsigned char* pDate, int nLength, CharacterSet charaSet)
{
	bool bRetValue = false;
	XMPDataNode* rootNode = XMPDataNode::Create();
	if(mpXMPNode && rootNode)
	{
		if(mXMPparser.Parse(pDate,nLength,charaSet, rootNode))
		{
			XMPDataNode* pXMPNode = rootNode->GetXMPNode(XMPMETADATA_ROOTTAG);
			if(pXMPNode && mpXMPNode->AppendNode(*pXMPNode))
			{
				bRetValue = ReadMetaData(mRootMetadata,*rootNode);	
			}
		}
	}
	XMPDataNode::Destroy(rootNode);	
	return bRetValue;
}
bool PmtXMPAccessor::readMetadata(PmtMetadataPtr rootMd, bool ignoreIfExist /*= false*/)
{
	bool bSuccessed = false;
	if(mRootMetadata)
	{
		PmtMetadataPtr mdNew;
		PmtMetadataIterator mdIter = mRootMetadata->getMetadata("*[*]", false);
		PmtMetadataPtr md = mdIter.start();

		while(md)
		{
			if(ignoreIfExist)
			{
				mdNew = rootMd->getMetadatum(md->fullKey(), false);
				if(mdNew)
				{
					md = mdIter.next();
					continue;
				}
			}
			mdNew = rootMd->getMetadatum(md->fullKey());
			mdNew->setValueStr(md->getValueStr());
			md = mdIter.next();
		}
		bSuccessed = true;
	}
	return bSuccessed;
}

PmtMetadataPtr PmtXMPAccessor::readMetadata(const SPmtString nsURI /*= ""*/)
{
	PmtMetadataPtr rootMd = PmtMetadata::create(nsURI);
	if(rootMd)
		readMetadata(rootMd);
	return rootMd;
}
bool PmtXMPAccessor::getMdValue(PmtMetadataPtr & md)
{
	bool bSuccessed = false;
	if(mRootMetadata)
	{
		PmtMetadataPtr mdLocal = mRootMetadata->getMetadatum(md->fullKey(), false);
		if(mdLocal)
		{
			md->setValueStr(mdLocal->getValueStr());
			bSuccessed = true;
		}
	}
	return bSuccessed;
}

bool PmtXMPAccessor::setMdValue(const PmtMetadataPtr & md)
{
	bool bRetValue = false;
	if(mRootMetadata && (mOpenMode != PMT_READONLY))
	{
		PmtMetadataPtr mdLocal = mRootMetadata->getMetadatum(md->fullKey(),false);
		if(mdLocal)
		{
			if(mdLocal->getValueStr() == md->getValueStr())
				bRetValue = true;
			else
			{
				mdLocal->setValueStr(md->getValueStr());
				bRetValue = WriteMdValue(md);
			}
		}
		else
		{
			mdLocal = mRootMetadata->getMetadatum(md->fullKey());
			mdLocal->setValueStr(md->getValueStr());
			bRetValue = WriteMdValue(md);
		}
	}
	return bRetValue;
}
bool PmtXMPAccessor::ReadMetaData(PmtMetadataPtr& mdRoot)
{
	bool bSuccessed = false;
	XMPDataNode* pXMPMetadata = mpXMPNode->GetXMPNode(XMPMETADATA_ROOTTAG);
	if(pXMPMetadata)
		bSuccessed = ReadMetaData(mdRoot, *pXMPMetadata);
	return bSuccessed;
}

void PmtXMPAccessor::close()
{
	if(mOpenMode != PMT_READONLY)
	{
		Save();
	}
	Destroy();
}

int PmtXMPAccessor::close( char * buf, unsigned int bufSize )
{
	int nReturn = 0;
	if(Flush())
	{
		if(bufSize < mBlockManager.XMPBlockSize())
			nReturn = 0 - mBlockManager.XMPBlockSize();
		else //If memory is enough, copy buffer and free resources.
		{
			memcpy(buf, mBlockManager.XMPBlockData(), mBlockManager.XMPBlockSize());
			nReturn = mBlockManager.XMPBlockSize();
			Destroy();
		}
	}
	return nReturn;
}

void PmtXMPAccessor::Destroy()
{
	if(mFileHandle)
	{
		fclose(mFileHandle);
		mFileHandle = NULL;
	}
	if(mpXMPNode)
	{
		XMPDataNode::Destroy(mpXMPNode);
		mpXMPNode = NULL;
	}
}
bool PmtXMPAccessor::ReadMetaData(PmtMetadataPtr& mdRoot, XMPDataNode& node)
{
	PmtTranslationTableElements * theTable = msTranslationTable->getSubTable("XMP");

	if(theTable == NULL)
		return false;

	//int nTranslatorIndex;
	PmtMetadataPtr md;
	list<XMPDataAttribute*>::iterator itSub;
	vector<int> vTranslaterIndex;
	vector<int>::iterator indexBeg;
	for(itSub = node.FirstAttribute(); itSub != node.EndAttribute(); itSub ++)
	{
		wstring strKey = (*itSub)->Key();
		FindXMPKeysInTranslationTable(theTable, XMPTypeTranslater::WideStringToString((*itSub)->Key()), vTranslaterIndex);

		for (indexBeg = vTranslaterIndex.begin(); indexBeg != vTranslaterIndex.end(); indexBeg ++)
		{
			//wcout << L"Attribute: " <<(*itSub)->Key() << L"=" << (*itSub)->Value() << endl;
			XMPTypeMode typeMode = GetXMPTypeMode((*theTable)[*indexBeg]->mType);
			md = mdRoot->getMetadatum((*theTable)[*indexBeg]->mKey);
			if((typeMode == XMPTypeModeSimple) && md)
			{
				AssignXMPValueToPMTValue(md, (*itSub)->Value(), (*theTable)[*indexBeg]->mType);
			}
			else
			{
				//There must be some error.
				throw exception();
			}
		}
	}

	//If the key of the metadata is mapped to the PMT metadata, then its context is value or all of its subnodes composite its value.
	FindXMPKeysInTranslationTable(theTable, XMPTypeTranslater::WideStringToString(node.Key()),vTranslaterIndex);
	for(indexBeg = vTranslaterIndex.begin(); indexBeg != vTranslaterIndex.end(); indexBeg ++)
	{
		XMPTypeMode typeMode = GetXMPTypeMode((*theTable)[*indexBeg]->mType);
		if(typeMode == XMPTypeModeNone)
		{
			//There must be some error.
			throw exception();
		}
		bool bMultiPMTValue = IsMultiPMTValue((*theTable)[*indexBeg]->mKey);
		if(!bMultiPMTValue)
			md = mdRoot->getMetadatum((*theTable)[*indexBeg]->mKey);

		if(typeMode == XMPTypeModeSimple)
		{
			//wcout << L"Simple: " <<node.Key() << L"=" << node.Content() << endl;
			AssignXMPValueToPMTValue(md, node.Content(), (*theTable)[*indexBeg]->mType);
		}
		else if(typeMode == XMPTypeModeSeqVector)
		{
			//wcout << L"seq vector: " <<node.Key() << endl;
			vector<wstring> strValues;
			node.GetKeyValueSeq(node.Key(), strValues);
			if(bMultiPMTValue)
				AssignMutiXMPValueToPMTValue(mdRoot, strValues, (*theTable)[*indexBeg]->mKey,(*theTable)[*indexBeg]->mType);
			else
				AssignXMPValueToPMTValue(md, strValues, (*theTable)[*indexBeg]->mType);
		}
		else if(typeMode == XMPTypeModeBagVector) //Only SceneContent.UserComment.MS.Keywords
		{
			//wcout << L"bag vector: " <<node.Key() << endl;
			vector<wstring> strValues;
			node.GetKeyValueBag(node.Key(), strValues);
			if(bMultiPMTValue)
				AssignMutiXMPValueToPMTValue(mdRoot, strValues, (*theTable)[*indexBeg]->mKey,(*theTable)[*indexBeg]->mType);
			else
				AssignXMPValueToPMTValue(md, strValues, (*theTable)[*indexBeg]->mType);
		}
		else if(typeMode == XMPTypeModeComposited)
		{
			//wcout << L"composited: " <<node.Key() << endl;
			AssignCompositedValuetoPMTValue(md, node, (*theTable)[*indexBeg]->mType);
		}
		else if(typeMode == XMPTypeModeComplex)
		{
			//wcout << L"complex: " <<node.Key() << endl;
			AssignCompositedValuetoPMTValue(md, node, (*theTable)[*indexBeg]->mType);
			//Its sub nodes are also metadatas.
			list<XMPDataNode*>::iterator itSub;
			for (itSub = node.FirstSubNode(); itSub != node.EndSubNode(); itSub ++)
			{
				ReadMetaData(mdRoot, **itSub);
			}
		}
	}
	if(vTranslaterIndex.size() <= 0) //If the key is not the metadata, then handle all its child elements.
	{
		list<XMPDataNode*>::iterator itSub;
		for (itSub = node.FirstSubNode(); itSub != node.EndSubNode(); itSub ++)
		{
			//wcout << L"sub node: " <<(*itSub)->Key() << endl;
			ReadMetaData(mdRoot, **itSub);
		}
	}
	return true;
}

bool PmtXMPAccessor::AssignXMPValueToPMTValue(PmtMetadataPtr &md, const wstring& strValue, const SPmtString& strTypeName)
{
	bool bRetValue = false;
	wstring strWideValue = strValue;
	if(strTypeName == "Integer")
	{
		md->setValueStr(XMP_EKSTRING2STR(strWideValue));
		bRetValue = true;
	}
	else if(strTypeName == "Rational") //All the rational values are mapped to float type in current PMT.
	{
		if(md->getMDType() == PMT_MD_TYPE_FLOAT)
		{
			XMPTypeRational rationValue = XMPTypeTranslater::FromXMPTypeRational(strWideValue);
			PmtMetadataT<float>* mdT = (PmtMetadataT<float>*) md.ptr();
			mdT->value() = rationValue.ToFloat();
			bRetValue = true;
		}
	}
	else if(strTypeName == "String")
	{
		if(md->getMDType() == PMT_MD_TYPE_STRING)
		{
			md->setValueStr(XMP_EKSTRING2STR(strWideValue));
			bRetValue = true;
		}
		//ImageContainer.FlashpixVersion, SceneContent.GPS.VersionID, SceneContent.GPS.ProcessingMethod, SceneContent.GPS.AreaInformation, ImageContainer.ExifVersion
		else if(md->getMDType() == PMT_MD_TYPE_VUINT8)
		{
			vector<unsigned char> vValues;
			XMPTypeTranslater::StringToVUint8(strWideValue, vValues);
			PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*) md.ptr();
			mdT->value() = vValues;
			bRetValue = true;
		}
		else if((md->getMDType() == PMT_MD_TYPE_FLOAT) && md->key() == "ImageContainer.Movie.FrameRate")
		{
			bRetValue = true;
			PmtMetadataT<float>* mdT = (PmtMetadataT<float>*) md.ptr();
			if(strWideValue == L"24")
				mdT->value() = 24; 
			else if(strWideValue == L"NTSC") //used in America Japan and Korea etc.
				mdT->value() = 30;
			else if(strWideValue == L"PAL") //used in China and European countries.
				mdT->value() = 25;
			else
				bRetValue = false;
		}
	}
	else if(strTypeName == "Date")
	{
		XMPTypeDate xmpDate;
		if(xmpDate.FromXMPString(strWideValue))
		{
			if(md->getMDType() == PMT_MD_TYPE_STRING) //ImageFileChangeDateTime, ImageCaptureDateTime, ImageFileCreationDateTime
			{
				md->setValueStr(XMP_EKSTRING2STR(xmpDate.ToPMTTypeString()));
				bRetValue = true;
			}
			else if(md->getMDType() == PMT_MD_TYPE_VFLOAT) //SceneContent.GPS.TimeStamp 
			{
				PmtMetadataT< vector<float> >* mdT = (PmtMetadataT< vector<float> >*) md.ptr(); 
				mdT->value().push_back(xmpDate.Hours());
				mdT->value().push_back(xmpDate.Minutes());
				mdT->value().push_back(xmpDate.Seconds());
				bRetValue = true;
			}
		}
	}
	//All of the GPSCoordinate are mapped to vfloat type.
	else if(strTypeName == "GPSCoordinate")
	{
		vector<float> fValues;
		XMPTypeGPSCoordinate gpsCoordinate;
		if(gpsCoordinate.FromString(strWideValue))
		{
			gpsCoordinate.ToFloatVector(fValues);
			PmtMetadataT< vector<float> >* mdT = (PmtMetadataT< vector<float> >*) md.ptr();
			mdT->value() = fValues;
			bRetValue = true;
		}
	}
	else if(strTypeName == "Boolean") // Only CaptureConditions.Flash.Fired
	{
		if(md->getMDType() == PMT_MD_TYPE_BOOL)
		{
			PmtMetadataT<bool>* mdT = (PmtMetadataT<bool>*) md.ptr(); 
			mdT->value() = XMPTypeTranslater::FromXMPTypeBool(strWideValue);
			bRetValue = true;
		}
	}
	return bRetValue;
}

bool PmtXMPAccessor::SetMdValue(const PmtMetadataPtr& md, const wstring& strKey, const SPmtString& strTypeName, XMPDataNode& node)
{
	wstring strValue;
	if(strTypeName == "Integer")
	{
		strValue = XMP_EKSTRING2WIDE(md->getValueStr());
	}
	else if(strTypeName == "Rational")
	{
		if(md->getMDType() == PMT_MD_TYPE_FLOAT)
		{
			PmtMetadataT<float>* mdT = (PmtMetadataT<float>*) md.ptr();
			XMPTypeRational rational(mdT->value());
			strValue = rational.ToXMPString();
		}
	}
	else if(strTypeName == "String")
	{
		if(md->getMDType() == PMT_MD_TYPE_STRING)
		{
			strValue = XMP_EKSTRING2WIDE(md->getValueStr());
		}
		//ImageContainer.FlashpixVersion, SceneContent.GPS.VersionID, SceneContent.GPS.ProcessingMethod, SceneContent.GPS.AreaInformation, ImageContainer.ExifVersion
		else if(md->getMDType() == PMT_MD_TYPE_VUINT8)
		{
			PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*) md.ptr();
			strValue = XMPTypeTranslater::VUint8ToString(mdT->value());
		}
		else if((md->getMDType() == PMT_MD_TYPE_FLOAT) && md->key() == "ImageContainer.Movie.FrameRate")
		{
			PmtMetadataT<float>* mdT = (PmtMetadataT<float>*) md.ptr();
			if(abs(mdT->value() - 24.0) < 0.01)
				strValue = L"24";
			else if(abs(mdT->value() - 25.0) < 0.01)
				strValue = L"PAL";
			else if(abs(mdT->value() - 30.0) < 0.01)
				strValue = L"NTSC";
		}
	}
	else if(strTypeName == "Date")
	{
		XMPTypeDate xmpDate;
		if(md->getMDType() == PMT_MD_TYPE_STRING) //ImageFileChangeDateTime, ImageCaptureDateTime, ImageFileCreationDateTime
		{
			xmpDate.FromPMTString(XMP_EKSTRING2WIDE(md->getValueStr()));
			strValue = xmpDate.ToXMPTypeString();
		}
		else if(md->getMDType() == PMT_MD_TYPE_VFLOAT) //SceneContent.GPS.TimeStamp 
		{
			PmtMetadataT< vector<float> >* mdT = (PmtMetadataT< vector<float> >*) md.ptr(); 
			xmpDate.Hours() = mdT->value()[0];
			xmpDate.Minutes() = mdT->value()[1];
			xmpDate.Seconds() = mdT->value()[2];
			strValue = xmpDate.ToXMPTypeString();
		}
	}
	//All of the GPSCoordinate are mapped to vfloat type.
	else if(strTypeName == "GPSCoordinate")
	{
		if(md->getMDType() == PMT_MD_TYPE_VFLOAT)
		{
			XMPTypeGPSCoordinate gpsCoordinate;
			PmtMetadataT< vector<float> >* mdT = (PmtMetadataT< vector<float> >*) md.ptr();
			gpsCoordinate.FromFloatVector(mdT->value());
			strValue = gpsCoordinate.ToString();
		}
	}
	else if(strTypeName == "Boolean") // Only CaptureConditions.Flash.Fired
	{
		if(md->getMDType() == PMT_MD_TYPE_BOOL)
		{
			PmtMetadataT<bool>* mdT = (PmtMetadataT<bool>*) md.ptr(); 
			strValue = XMPTypeTranslater::ToXMPTypeBool(mdT->value());
		}
	}
	bool bRetValue = false;
	if(strValue.size())
		bRetValue = node.UpdateKeyValue(strKey, strValue, true);
	return bRetValue;
}
bool PmtXMPAccessor::AssignXMPValueToPMTValue(PmtMetadataPtr &md, const vector<wstring>& strValues, const SPmtString& strTypeName)
{	
	bool bRetValue = false;
	if(strTypeName == "SeqInteger")
	{
		vector<int> vValues;
		XMPTypeTranslater::FromXMPTypeSeqInt(strValues, vValues);
		if(md->getMDType() == PMT_MD_TYPE_VUINT8) //ImageContainer.ComponentsConfiguration
		{
			PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*)md.ptr();
			mdT->value().assign(vValues.begin(), vValues.end());
			bRetValue = true;
		}
		else if(md->getMDType() == PMT_MD_TYPE_VUINT16) //ImageContainer.Thumbnail.BitsPerSample, ImageContainer.TransferFunction, ImageContainer.YCbCrSubSampling, CaptureConditions.SubjectArea, SceneContent.SubjectLocation
		{
			PmtMetadataT< vector<unsigned short> >* mdT = (PmtMetadataT< vector<unsigned short> >*)md.ptr();
			mdT->value().assign(vValues.begin(), vValues.end());
			bRetValue = true;
		}
		else if(md->getMDType() == PMT_MD_TYPE_UINT16) //ISOSpeedRating
		{
			PmtMetadataT< unsigned short >* mdT = (PmtMetadataT< unsigned short>*)md.ptr();
			mdT->value() = vValues[0];
			bRetValue = true;
		}
	}	
	//ImageContainer.WhitePoint, ImageContainer.PrimaryChromaticities, ImageContainer.YCbCrCoefficients,ImageContainer.ReferenceBlackWhite
	else if(strTypeName == "SeqRational")
	{
		if(md->getMDType() == PMT_MD_TYPE_VFLOAT)
		{
			vector<XMPTypeRational> vRations;
			XMPTypeTranslater::FromXMPTypeSeqRational(strValues, vRations);
			PmtMetadataT< vector<float> >* mdT = (PmtMetadataT< vector<float> >*)md.ptr();
			mdT->value().clear();
			vector<XMPTypeRational>::const_iterator itRational;
			for (itRational = vRations.begin(); itRational != vRations.end(); itRational ++)
			{
				mdT->value().push_back(itRational->ToFloat());
			}
			bRetValue = true;
		}
	}
	 // They are all moved to add multiple metadatas.
// 	else if((strTypeName == "SeqString") || (strTypeName == "BagString")) //SceneContent.UserComment.MS.Author, SceneContent.UserComment.MS.Keywords
// 	{
// 		return false;
// 		if(md->getMDType() != PMT_MD_TYPE_VUINT8) //All of these two metadatas are in unicode bytes.
// 			return false;
// 		wstring strAllValues;
// 		vector<wstring>::const_iterator itStr;
// 		bool bFistValue = true;
// 		for (itStr = strValues.begin(); itStr != strValues.end(); itStr ++)
// 		{
// 			if(!bFistValue)
// 				strAllValues += L"; "; //Use colon and a blank space to separate each string. just as Adobe Reader does.
// 			bFistValue = false;
// 			strAllValues += *itStr;
// 		}
// 		PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*)md.ptr();
// 		vector<unsigned char> vValues;
// 		XMPTypeTranslater::StringToVUint8W(strAllValues,vValues);
// 		mdT->value() = vValues;
// 	}

	return bRetValue;
}
bool PmtXMPAccessor::IsMultiPMTValue(const SPmtString& strKey)
{
	bool bIsMultiple = false;
	if(strKey == "SceneContent.UserComment.MS.Author")
		bIsMultiple = true;
	else if(strKey == "SceneContent.UserComment.MS.Keywords")
		bIsMultiple = true;
	return bIsMultiple;
}
void PmtXMPAccessor::SplitMetadataFullName(const SPmtString& strFullKey, SPmtString& strPrefix, SPmtString& strName)
{
	size_t nLastDot = strFullKey.find_last_of('.');
	if(nLastDot > 0)
	{
		strPrefix = strFullKey.substr(0, nLastDot);
		if(strPrefix.size() > 0)
			strName = strFullKey.substr(nLastDot + 1, strFullKey.size() - nLastDot -1);
	}
}
bool PmtXMPAccessor::SplitMetadataIndex(const SPmtString& strFullKey, SPmtString& strKey, unsigned int& nIndex)
{
	bool bStatus = false;
	int nFirstBracket = strFullKey.find_first_of('[');
	int nLastBracket = strFullKey.find_last_of(']');
	if(nLastBracket >= 0 && nFirstBracket > 0 && (nLastBracket - nFirstBracket == 2))
	{
		nIndex = strFullKey[nFirstBracket+1] - '0';
		strKey = strFullKey.substr(0,nFirstBracket);
		bStatus = true;
	}
	return bStatus;
}
bool PmtXMPAccessor::AssignMutiXMPValueToPMTValue(PmtMetadataPtr& mdRoot, const vector<wstring>& strValues, const SPmtString& strKey, const SPmtString& strTypeName)
{
	bool bStatus = false;
	SPmtString strPrefix;
	SPmtString strName;
	SplitMetadataFullName(strKey, strPrefix, strName);
	if(strPrefix.size() > 0 && strName.size() >  0)
	{
		PmtCompositeMetadata* pMS = (PmtCompositeMetadata*)(mdRoot->getMetadatum(strPrefix,false).ptr());
		if(pMS)
		{
			vector<wstring>::size_type nOccurNum = 0;
			vector<wstring>::const_iterator itStr;
			for (itStr = strValues.begin(); itStr != strValues.end(); itStr ++)
			{
				PmtMetadataPtr md = pMS->modelGroup()->getMetadatum(strName,nOccurNum ++);
				if(md->getMDType() == PMT_MD_TYPE_VUINT8) //All of these two metadatas are in unicode bytes.
				{
					PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*)md.ptr();
					vector<unsigned char> vValues;
					XMPTypeTranslater::StringToVUint8W(*itStr,vValues);
					mdT->value() = vValues;
				}
				else
					break;				
			}
			bStatus = (itStr == strValues.end());
		}
	}
	return bStatus;
}
bool PmtXMPAccessor::SetMdValueVector(const PmtMetadataPtr& md, const wstring& strKey, const SPmtString& strTypeName, XMPDataNode& node)
{
	bool bHasError = false;
	vector<wstring> strValues;
	if(strTypeName == "SeqInteger")
	{
		vector<int> vValues;
		XMPTypeTranslater::FromXMPTypeSeqInt(strValues, vValues);
		if(md->getMDType() == PMT_MD_TYPE_VUINT8) //ImageContainer.ComponentsConfiguration
		{
			PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*)md.ptr();
			vValues.assign(mdT->value().begin(), mdT->value().end());
		}
		else if(md->getMDType() == PMT_MD_TYPE_VUINT16) //ImageContainer.Thumbnail.BitsPerSample, ImageContainer.TransferFunction, ImageContainer.YCbCrSubSampling, CaptureConditions.SubjectArea, SceneContent.SubjectLocation
		{
			PmtMetadataT< vector<unsigned short> >* mdT = (PmtMetadataT< vector<unsigned short> >*)md.ptr();
			vValues.assign(mdT->value().begin(), mdT->value().end());
		}
		else if(md->getMDType() == PMT_MD_TYPE_UINT16) //ISOSpeedRating
		{
			PmtMetadataT< unsigned short >* mdT = (PmtMetadataT< unsigned short>*)md.ptr();
			vValues.push_back(mdT->value());
		}
		else
			bHasError = true;
		if(!bHasError)
			XMPTypeTranslater::ToXMPTypeSeqInt(vValues, strValues);
	}	
	//ImageContainer.WhitePoint, ImageContainer.PrimaryChromaticities, ImageContainer.YCbCrCoefficients,ImageContainer.ReferenceBlackWhite
	else if(strTypeName == "SeqRational")
	{
		if(md->getMDType() == PMT_MD_TYPE_VFLOAT)
		{
			vector<XMPTypeRational> vRations;

			PmtMetadataT< vector<float> >* mdT = (PmtMetadataT< vector<float> >*)md.ptr();
			vector<float>::const_iterator itFloat;
			for (itFloat = mdT->value().begin(); itFloat != mdT->value().end(); itFloat ++)
			{
				vRations.push_back(XMPTypeRational(*itFloat));
			}
			XMPTypeTranslater::ToXMPTypeSeqRational(vRations, strValues);
		}
		else
			bHasError = true;
	}
	else if((strTypeName == "SeqString")) //SceneContent.UserComment.MS.Author
		bHasError = true;
	else
		bHasError = true;
	if(!bHasError)
		bHasError = !(node.UpdateKeyValueSeq(strKey, strValues, true));
	return !bHasError;
}
//This will not be invoked.
// bool PmtXMPAccessor::SetMdValueBag(const PmtMetadataPtr& md, const wstring& strKey, const SPmtString& strTypeName, XMPDataNode& node)
// {
// 	if(strTypeName != "BagString") // Only SceneContent.UserComment.MS.Keywords
// 		return false;
// 
// 	if(md->getMDType() != PMT_MD_TYPE_VUINT8) //All of these metadatas are in unicode bytes.
// 		return false;
// 	PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*)md.ptr();
// 	wstring strAllValues = XMPTypeTranslater::VUint8WToString(mdT->value());
// 	int nIndex = 0, nStart = 0;
// 	vector<wstring> strValues;
// 	while(true)
// 	{
// 		nIndex = strAllValues.find(L';');
// 		if(nIndex > nStart)
// 		{
// 			strValues.push_back(strAllValues.substr(nStart, nIndex - nStart));
// 			nStart = nIndex + 2;
// 		}
// 		else
// 		{
// 			strValues.push_back(strAllValues.substr(nStart));
// 			break;
// 		}
// 	}
// 	return node.UpdateKeyValueBag(strKey, strValues, true);
//}
bool PmtXMPAccessor::SetMdValueVectorIndex(const PmtMetadataPtr& md, const wstring& strKey, const SPmtString& strTypeName, XMPDataNode& node, unsigned int nIndex)
{
	bool bRetValue = false;
	if(strTypeName == "SeqString") // Only SceneContent.UserComment.MS.Author
	{
		if(md->getMDType() == PMT_MD_TYPE_VUINT8) //All of these metadatas are in unicode bytes.
		{
			PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*)md.ptr();
			wstring strValue = XMPTypeTranslater::VUint8WToString(mdT->value());
			bRetValue = node.UpdateKeyValueSeqIndex(strKey, strValue, nIndex, true);
		}
	}
	return bRetValue;	
}
bool PmtXMPAccessor::SetMdValueBagIndex(const PmtMetadataPtr& md, const wstring& strKey, const SPmtString& strTypeName, XMPDataNode& node, unsigned int nIndex)
{
	bool bRetValue = false;
	if(md->getMDType() == PMT_MD_TYPE_VUINT8) //All of these three metadatas are in unicode bytes.
	{
		PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*)md.ptr();
		wstring strValue = XMPTypeTranslater::VUint8WToString(mdT->value());
		bRetValue = node.UpdateKeyValueBagIndex(strKey, strValue, md->occurrence(), true);
	}
	return bRetValue;
}

bool PmtXMPAccessor::SetMdValueComposited(const PmtMetadataPtr& md, const wstring& strKey, const SPmtString& strTypeName, XMPDataNode& node)
{
	bool bRetValue = false;
	if(strTypeName == "LangAlt")
		bRetValue = SetMdValueLangAlt(md, strKey, node);
	else if(strTypeName == "OECF")
		bRetValue = SetMdValueOECF(md, strKey, node);
	else if(strTypeName == "CFAPattern")
		bRetValue = SetMdValueCFAPattern(md, strKey, node);
	else if(strTypeName == "Time")
		bRetValue = SetMdValueTime(md, strKey, node);
	else if(strTypeName == "Flash")
		bRetValue = SetMdValueFlash(md, strKey, node);
	else if(strTypeName == "DeviceSetting")
		bRetValue = SetMdValueDeviceSetting(md, strKey, node);
	return bRetValue;
}

bool PmtXMPAccessor::AssignCompositedValuetoPMTValue(PmtMetadataPtr& md, XMPDataNode& node, const SPmtString& strTypeName)
{
	bool bRetValue = false;
	if(strTypeName == "LangAlt")
		bRetValue = AssignLangAltToPMTValue(md, node);
	else if(strTypeName == "OECF")
		bRetValue = AssignOECFToPMTValue(md,node);
	else if(strTypeName == "CFAPattern")
		bRetValue = AssignCFAPatternToPMTValue(md,node);
	else if(strTypeName == "Time")
		bRetValue = AssignTimeToPMTValue(md,node);
	else if(strTypeName == "Flash")
		bRetValue = AssignFlashToPMTValue(md, node);
	else if(strTypeName == "DeviceSetting")
		bRetValue = AssignDeviceSettingToPMTValue(md, node);
	return bRetValue;
}
bool PmtXMPAccessor::AssignLangAltToPMTValue(PmtMetadataPtr& md, XMPDataNode& node)
{
	bool bRetValue = false;
	map<wstring, wstring> lang;
	wstring strValue;
	node.GetKeyValueLanguage(node.Key(), lang);
	if(lang.size() >= 1)
	{
		if(lang.count(L"x-default"))
		{
			strValue = lang[L"x-default"];
		}
		else
			strValue = lang.begin()->second;
		if(md->getMDType() == PMT_MD_TYPE_STRING) //SceneContent.ImageDescription, IntellectualProperty.Copyright
		{
			md->setValueStr(XMP_EKSTRING2STR(strValue));
			bRetValue = true;
		}
		else if(md->getMDType() == PMT_MD_TYPE_VUINT8) //SceneContent.UserComment.MS.Title,	SceneContent.UserComment.MS.Subject
		{
			PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*)md.ptr();
			vector<unsigned char> vValues;
			XMPTypeTranslater::StringToVUint8W(strValue,vValues);
			mdT->value() = vValues;
			bRetValue = true;
		}
	}
	return bRetValue;
}

bool PmtXMPAccessor::SetMdValueLangAlt(const PmtMetadataPtr& md, const wstring& strKey, XMPDataNode& node)
{
	bool bRetValue = false;
	wstring strValue;
	if(md->getMDType() == PMT_MD_TYPE_STRING) //SceneContent.ImageDescription, IntellectualProperty.Copyright
	{
		strValue = XMP_EKSTRING2WIDE(md->getValueStr());
	}
	else if(md->getMDType() == PMT_MD_TYPE_VUINT8) //SceneContent.UserComment.MS.Title,	SceneContent.UserComment.MS.Subject
	{
		PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*)md.ptr();
		strValue = XMPTypeTranslater::VUint8WToString(mdT->value());
	}
	if(strValue.size())
	{
		map<wstring, wstring> mapLang;
		mapLang[L"x-default"] = strValue;
		bRetValue =  node.UpdateKeyValueLanguage(strKey, mapLang, true);
	}
	return bRetValue;
}

bool PmtXMPAccessor::AssignOECFToPMTValue(PmtMetadataPtr& md, XMPDataNode& node)
{
	bool bRetValue = false;
	wstring strPrefix = XMPMETADATA_EXIFTAG;
	XMPTypeOECF oecf;
	const wchar_t* szValue;
	szValue = node.GetKeyValue((strPrefix + L"Columns"));
	if(szValue)
	{
		//Get number of columns
		oecf.nColumns = XMPTypeTranslater::FromXMPTypeInt(szValue);

		//Get number of rows
		szValue = node.GetKeyValue((strPrefix + L"Rows"));
		if(szValue)
			oecf.nRows = XMPTypeTranslater::FromXMPTypeInt(szValue);
	}
	if(szValue)
	{
		//Get OECF/SFR column names
		vector<wstring> strValues;
		node.GetKeyValueSeq((strPrefix + L"Names"), strValues);
		if(strValues.size() == oecf.nColumns)
		{
			strValues.swap(oecf.strColumnsNames);

			//Get OECF/SFR values
			strValues.clear();
			node.GetKeyValueSeq((strPrefix + L"Values"), strValues);
			if(strValues.size() == oecf.nColumns * oecf.nRows)
			{
				XMPTypeTranslater::FromXMPTypeSeqRational(strValues,oecf.vRationals);

				PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*)md.ptr();
				mdT->value().clear();
				oecf.ToVUint8(mdT->value());
				bRetValue = true;
			}
		}
	}
	return bRetValue;
}

bool PmtXMPAccessor::SetMdValueOECF(const PmtMetadataPtr& md, const wstring& strKey, XMPDataNode& node)
{
	bool bRetValue = false;
	XMPDataNode* pOECFNode = node.GetXMPNode(strKey, true);
	if(pOECFNode != NULL)
	{
		wstring strPrefix = XMPMETADATA_EXIFTAG;
		XMPTypeOECF oecf;

		if(md->getMDType() == PMT_MD_TYPE_VUINT8)
		{
			PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*)md.ptr();
			if(oecf.FromVUint8(mdT->value()))
			{
				wstring strValue;

				strValue = XMPTypeTranslater::ToXMPTypeInt(oecf.nColumns);
				pOECFNode->UpdateSubKeyValue(strPrefix + L"Columns", strValue, true);

				strValue = XMPTypeTranslater::ToXMPTypeInt(oecf.nRows);
				pOECFNode->UpdateSubKeyValue(strPrefix + L"Rows", strValue, true);

				pOECFNode->UpdateKeyValueSeq(strPrefix + L"Names", oecf.strColumnsNames, true);
				vector<wstring> strValues;
				XMPTypeTranslater::ToXMPTypeSeqRational(oecf.vRationals, strValues);
				bRetValue = pOECFNode->UpdateKeyValueSeq(strPrefix + L"Values", strValues, true);
			}
		}
	}
	return bRetValue;
}

bool PmtXMPAccessor::AssignFlashToPMTValue(PmtMetadataPtr& md, XMPDataNode& node)
{
	bool bRetValue = false;
	XMPTypeFlash flash;
	wstring strPrefix = XMPMETADATA_EXIFTAG;

	const wchar_t* szValue;
	szValue = node.GetKeyValue((strPrefix + L"Fired"));
	if(szValue > 0)
		flash.bFired = XMPTypeTranslater::FromXMPTypeBool(szValue);
	szValue = node.GetKeyValue((strPrefix + L"Function"));
	if(szValue)
		flash.bFunction = XMPTypeTranslater::FromXMPTypeBool(szValue);
	szValue = node.GetKeyValue((strPrefix + L"Mode"));
	if(szValue)
		flash.nMode = XMPTypeTranslater::FromXMPTypeInt(szValue);
	szValue = node.GetKeyValue((strPrefix + L"RedEyeMode"));
	if(szValue)
		flash.bRedEyeMode = XMPTypeTranslater::FromXMPTypeBool(szValue);
	szValue = node.GetKeyValue((strPrefix + L"Return"));
	if(szValue)
		flash.nReturn = XMPTypeTranslater::FromXMPTypeInt(szValue);
	
	if(md->getMDType() == PMT_MD_TYPE_UINT8)
	{
		PmtMetadataT< unsigned char >* mdT = (PmtMetadataT< unsigned char >*)md.ptr();
		mdT->value() = flash.ToByte();
		bRetValue = true;
	}
	return bRetValue;
}

bool PmtXMPAccessor::SetMdValueFlash(const PmtMetadataPtr& md, const wstring& strKey, XMPDataNode& node)
{
	bool bRetValue = false;
	XMPDataNode* pFlashNode = NULL;
	if((md->getMDType() == PMT_MD_TYPE_UINT8) && (pFlashNode = node.GetXMPNode(strKey, true)))
	{
		PmtMetadataT< unsigned char >* mdT = (PmtMetadataT< unsigned char >*)md.ptr();
		XMPTypeFlash flash;
		flash.FromByte(mdT->value());

		wstring strPrefix = XMPMETADATA_EXIFTAG;
		bRetValue = pFlashNode->UpdateSubKeyValue(strPrefix +  L"Fired", XMPTypeTranslater::ToXMPTypeBool(flash.bFired),true);

		if(bRetValue)
			bRetValue = pFlashNode->UpdateSubKeyValue(strPrefix +  L"Function", XMPTypeTranslater::ToXMPTypeBool(flash.bFunction),true);
		if(bRetValue)
			bRetValue = pFlashNode->UpdateSubKeyValue(strPrefix +  L"Mode", XMPTypeTranslater::ToXMPTypeInt(flash.nMode),true);
		if(bRetValue)
			bRetValue = pFlashNode->UpdateSubKeyValue(strPrefix +  L"RedEyeMode", XMPTypeTranslater::ToXMPTypeBool(flash.bRedEyeMode),true);
		if(bRetValue)
			bRetValue = pFlashNode->UpdateSubKeyValue(strPrefix +  L"Return", XMPTypeTranslater::ToXMPTypeInt(flash.nReturn),true);
	}
	return bRetValue;	
}
bool PmtXMPAccessor::AssignCFAPatternToPMTValue(PmtMetadataPtr& md, XMPDataNode& node)
{
	bool bRetValue = false;
	wstring strPrefix = XMPMETADATA_EXIFTAG;

	XMPTypeCFAPattern cfa;
	const wchar_t* szValue;
	szValue = node.GetKeyValue((strPrefix + L"Columns"));
	if(szValue)
	{
		cfa.nColumns = XMPTypeTranslater::FromXMPTypeInt(szValue);
		szValue = node.GetKeyValue((strPrefix + L"Rows"));
	}
	if(szValue)
	{
		cfa.nRows = XMPTypeTranslater::FromXMPTypeInt(szValue);
		vector<wstring> strValues;
		node.GetKeyValueSeq((strPrefix + L"Values"), strValues);
		if(strValues.size() == cfa.nColumns * cfa.nRows)
		{
			vector<int> intValues;
			XMPTypeTranslater::FromXMPTypeSeqInt(strValues, intValues);
			cfa.nValues.assign(intValues.begin(), intValues.end());

			PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*)md.ptr();
			mdT->value().clear();
			cfa.ToVUint8(mdT->value());

			bRetValue = true;
		}
	}
	return bRetValue;
}

bool PmtXMPAccessor::SetMdValueCFAPattern(const PmtMetadataPtr& md, const wstring& strKey, XMPDataNode& node)
{
	bool bRetValue = false;
	if(md->getMDType() == PMT_MD_TYPE_VUINT8)
	{
		PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*)md.ptr();
		XMPTypeCFAPattern cfa;
		XMPDataNode* pCFAPattern = NULL;
		if(mdT && cfa.FromVUint8(mdT->value()) && (pCFAPattern = node.GetXMPNode(strKey, true)))
		{
			wstring strPrefix = XMPMETADATA_EXIFTAG;
			wstring strValue;

			bRetValue = pCFAPattern->UpdateSubKeyValue(strPrefix +  L"Columns", XMPTypeTranslater::ToXMPTypeBool(cfa.nColumns),true);
			if(bRetValue)
				bRetValue = pCFAPattern->UpdateSubKeyValue(strPrefix +  L"Rows", XMPTypeTranslater::ToXMPTypeBool(cfa.nRows),true);
			if(bRetValue)
			{
				vector<wstring> strValues;
				vector<int> nValues;
				nValues.assign(cfa.nValues.begin(), cfa.nValues.end());
				XMPTypeTranslater::ToXMPTypeSeqInt(nValues, strValues);
				bRetValue = pCFAPattern->UpdateKeyValueSeq(strPrefix + L"Values", strValues, true);
			}
		}
	}
	return bRetValue;
}

bool PmtXMPAccessor::AssignTimeToPMTValue(PmtMetadataPtr& md, XMPDataNode& node)
{
	bool bRetValue = false;
	XMPTypeTime xmpTime;
	wstring strPrefix = XMPMETADATA_DMTAG;
	const wchar_t* szValue = node.GetKeyValue((strPrefix + L"scale"));
	if(szValue)
	{
		xmpTime.rScale = XMPTypeTranslater::FromXMPTypeRational(szValue);

		szValue = node.GetKeyValue((strPrefix + L"value"));
		if(szValue && (md->getMDType() == PMT_MD_TYPE_FLOAT)) // Only ImageContainer.Movie.Duration.
		{
			xmpTime.nValue = XMPTypeTranslater::FromXMPTypeInt(szValue);

			PmtMetadataT< float >* mdT = (PmtMetadataT< float >*)md.ptr();
			mdT->value() = xmpTime.ToFloat();

			bRetValue = true;
		}
	}
	return bRetValue;
}

bool PmtXMPAccessor::SetMdValueTime(const PmtMetadataPtr& md, const wstring& strKey, XMPDataNode& node)
{
	bool bRetValue = false;
	XMPDataNode* pTimeNode = NULL;
	if((md->getMDType() == PMT_MD_TYPE_FLOAT) && (pTimeNode = node.GetXMPNode(strKey, true))) // Only ImageContainer.Movie.Duration.
	{
		XMPTypeTime xmpTime;
		PmtMetadataT< float >* mdT = (PmtMetadataT< float >*)md.ptr();
		mdT->value() = xmpTime.ToFloat();

		xmpTime.FromFloat(mdT->value());

		wstring strPrefix = XMPMETADATA_DMTAG;
		bRetValue = pTimeNode->UpdateSubKeyValue(strPrefix + L"scale", XMPTypeTranslater::ToXMPTypeRational(xmpTime.rScale));
		if(bRetValue)
			bRetValue = pTimeNode->UpdateSubKeyValue(strPrefix + L"value", XMPTypeTranslater::ToXMPTypeInt(xmpTime.nValue));	
	}
	return bRetValue;
}

bool PmtXMPAccessor::AssignDeviceSettingToPMTValue(PmtMetadataPtr& md, XMPDataNode& node)
{
	bool bRetValue = false;
	wstring strPrefix = XMPMETADATA_EXIFTAG;
	XMPTypeDeviceSetting devSetting;
	
	const wchar_t* szColumns, *szRows;
	szColumns = node.GetKeyValue((strPrefix + L"Columns"));
	szRows = node.GetKeyValue((strPrefix + L"Rows"));
	if(szColumns && szRows && (md->getMDType() == PMT_MD_TYPE_VUINT8))
	{
		devSetting.nColumns = XMPTypeTranslater::FromXMPTypeInt(szColumns);
		devSetting.nRows = XMPTypeTranslater::FromXMPTypeInt(szRows);

		vector<wstring> strValues;
		node.GetKeyValueSeq((strPrefix + L"Values"), devSetting.strValues);
		if((devSetting.strValues.size() == devSetting.nColumns * devSetting.nRows))
		{
			PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*)md.ptr();
			mdT->value().clear();
			devSetting.ToVUint8(mdT->value());
			bRetValue = true;
		}
	}
	return bRetValue;
}
bool PmtXMPAccessor::SetMdValueDeviceSetting(const PmtMetadataPtr& md, const wstring& strKey, XMPDataNode& node)
{
	bool bRetValue = false;
	if(md->getMDType() == PMT_MD_TYPE_VUINT8)
	{
		PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*)md.ptr();

		XMPTypeDeviceSetting devSetting;
		XMPDataNode* pDeviceSetting = NULL;
		if(devSetting.FromVUint8(mdT->value()) && (pDeviceSetting = node.GetXMPNode(strKey, true)))
		{
			wstring strPrefix = XMPMETADATA_EXIFTAG;
			bRetValue = pDeviceSetting->UpdateSubKeyValue(strPrefix + L"Columns", XMPTypeTranslater::ToXMPTypeInt(devSetting.nColumns),true);
			if(bRetValue)
				bRetValue = pDeviceSetting->UpdateSubKeyValue(strPrefix + L"Rows", XMPTypeTranslater::ToXMPTypeInt(devSetting.nRows),true);

			if(bRetValue)
				bRetValue = pDeviceSetting->UpdateKeyValueSeq(strPrefix + L"Values", devSetting.strValues, true);
		}
	}
	return bRetValue;
}
// int PmtXMPAccessor::FindXMPKeyInTranslationTable(PmtTranslationTableElements * theTable, const SPmtString& strXMPKey)
// {
// 	int nKeyIndex;
// 	for (nKeyIndex = 0; nKeyIndex != theTable->size(); nKeyIndex ++)
// 	{
// 		if((*theTable)[nKeyIndex]->mTag == strXMPKey)
// 			break;
// 	}
// 	return (nKeyIndex == theTable->size()) ? -1 : nKeyIndex;
// }

void PmtXMPAccessor::FindXMPKeysInTranslationTable(PmtTranslationTableElements * theTable, const SPmtString& strXMPKey, vector<int>& vIndexes)
{
	unsigned int nKeyIndex;
	vIndexes.clear();
	for (nKeyIndex = 0; nKeyIndex != theTable->size(); nKeyIndex ++)
	{
		if((*theTable)[nKeyIndex]->mTag == strXMPKey)
			vIndexes.push_back(nKeyIndex);
	}
}
int PmtXMPAccessor::FindPMTKeyInTranslationTable(PmtTranslationTableElements * theTable, const SPmtString& strPMTKey)
{
	unsigned int nKeyIndex;
	for (nKeyIndex = 0; nKeyIndex != theTable->size(); nKeyIndex++)
	{
		if((*theTable)[nKeyIndex]->mKey == strPMTKey)
			break;
	}
	return (nKeyIndex == theTable->size()) ? -1 : nKeyIndex;
}
XMPTypeMode PmtXMPAccessor::GetXMPTypeMode(const SPmtString& strTypeName)
{
	XMPTypeMode typeMode = XMPTypeModeNone;
	if(strTypeName == "Integer")
		typeMode = XMPTypeModeSimple;
	else if(strTypeName == "Rational")
		typeMode = XMPTypeModeSimple;
	else if(strTypeName == "String")
		typeMode = XMPTypeModeSimple;
	else if(strTypeName == "ascii")
		typeMode = XMPTypeModeSimple;
	else if(strTypeName == "GPSCoordinate")
		typeMode = XMPTypeModeSimple;
	else if(strTypeName == "Boolean")
		typeMode = XMPTypeModeSimple;
	else if(strTypeName == "Date")
		typeMode = XMPTypeModeSimple;
	else if(strTypeName == "SeqInteger")
		typeMode = XMPTypeModeSeqVector;	
	else if(strTypeName == "SeqRational")
		typeMode =  XMPTypeModeSeqVector;
	else if(strTypeName == "SeqString")
		typeMode =  XMPTypeModeSeqVector;
	else if(strTypeName == "BagString")
		typeMode =  XMPTypeModeBagVector;
	else if(strTypeName == "LangAlt")
		typeMode =  XMPTypeModeComposited;	
	else if(strTypeName == "OECF")
		typeMode =  XMPTypeModeComposited;
	else if(strTypeName == "CFAPattern")
		typeMode =  XMPTypeModeComposited;
	else if(strTypeName == "Time")
		typeMode =  XMPTypeModeComposited;
	else if(strTypeName == "DeviceSetting")
		typeMode =  XMPTypeModeComposited;
	else if(strTypeName == "Flash")
		typeMode =  XMPTypeModeComplex;
	return typeMode;
}
bool PmtXMPAccessor::SaveXMPBlockToXMLFile(const char* szFilePathName)
{
	bool bRetValue = false;
	FILE* fOutput = fopen(szFilePathName, "wb");

	if(fOutput > 0)
	{
		if(Flush())
		{
			fwrite(XMPBlockData(),1,XMPBlockSize(),fOutput);
		}
		fclose(fOutput);
		bRetValue = true;
	}
	return bRetValue;
}

bool PmtXMPAccessor::SaveXMPMetadataToXMLFile(const char* szFilePathName)
{
	bool bRetValue = false;
	FILE* fOutput = fopen(szFilePathName, "wb");
	if(fOutput > 0)
	{
		if(Flush())
		{
			fwrite(XMPMetadata(),1,XMPMetadataSize(),fOutput);
		}
		fclose(fOutput);
		bRetValue = true;
	}
	return bRetValue;
}

//! Check if the given file is has XMP block.
bool PmtXMPAccessor::recognize(const SPmtString & name)
{
	XMPBlockManager manager;
	return (manager.FindXMPBlock(name.c_str()) >= 0);
}

bool PmtXMPAccessor::WriteMdValue(const PmtMetadataPtr & md)
{
	PmtTranslationTableElements * theTable = msTranslationTable->getSubTable("XMP");
	SPmtString strFullKey = md->fullKey();
	bool bMultipleOccurrence = md->multipleOccurrence();
	unsigned int nIndex;
	if(bMultipleOccurrence)
	{
		SPmtString strKey;
		if(SplitMetadataIndex(strFullKey, strKey, nIndex))
			strFullKey = strKey;
		else
			return false;
	}
	int nKeyIndex = FindPMTKeyInTranslationTable(theTable, strFullKey);
	if(nKeyIndex < 0)
		return false;
	XMPDataNode* pParentNode = mpRDFDescNode; //parent node to hold this value.
	XMPTypeMode mode = GetXMPTypeMode((*theTable)[nKeyIndex]->mType);

	if(bMultipleOccurrence)
	{
		if(mode == XMPTypeModeSeqVector)
			return SetMdValueVectorIndex(md, EkAToW((*theTable)[nKeyIndex]->mTag), (*theTable)[nKeyIndex]->mType, *pParentNode, nIndex);
		else if(mode == XMPTypeModeBagVector)
			return SetMdValueBagIndex(md, EkAToW((*theTable)[nKeyIndex]->mTag), (*theTable)[nKeyIndex]->mType, *pParentNode, nIndex);
		else
			return false;
	}
	XMPSpecialMetadataElement* pSpecialEle = NULL;
	if((pSpecialEle = XMPSpecialMetadataManager::FindElementByKey(strFullKey)) != NULL)
	{
		pParentNode = mpRDFNode->GetXMPNode(XMP_PMTSTRING2WIDE(pSpecialEle->mTag),true);
		if(!pParentNode)
			return false;
	}
	if(mode == XMPTypeModeSimple)
		return SetMdValue(md, EkAToW((*theTable)[nKeyIndex]->mTag), (*theTable)[nKeyIndex]->mType, *pParentNode);
	else if(mode == XMPTypeModeSeqVector)
		return SetMdValueVector(md, EkAToW((*theTable)[nKeyIndex]->mTag), (*theTable)[nKeyIndex]->mType,*pParentNode);
	else if((mode == XMPTypeModeComposited) || (mode == XMPTypeModeComplex))
		return SetMdValueComposited(md, EkAToW((*theTable)[nKeyIndex]->mTag), (*theTable)[nKeyIndex]->mType,*pParentNode);
// 	else if(mode == XMPTypeModeBagVector)
// 		return SetMdValueBag(md, EkAToW((*theTable)[nKeyIndex]->mTag), (*theTable)[nKeyIndex]->mType,*pParentNode);
	else
		return false;
}

// Remove the metadata specified by md from the file.
void PmtXMPAccessor::removeMdValue(const PmtMetadataPtr & md)
{
	SPmtString strFullKey = md->fullKey();
	PmtMetadataPtr mdLocal = mRootMetadata->getMetadatum(strFullKey, false);
	if(mdLocal)
	{
		//How to get the root metadata another way?
		PmtRootMetadata* pRoot = (PmtRootMetadata*)(mRootMetadata.ptr());
		pRoot->deleteMetadata(md->fullKey());
		//mdLocal->getRootMetadata()->deleteMetadata(md->fullKey());

		XMPDataNode* pParentNode = mpRDFNode; //parent node to hold this value.
		PmtTranslationTableElements * theTable = msTranslationTable->getSubTable("XMP");
		if(md->multipleOccurrence())
		{
			SPmtString strKey;
			unsigned int nIndex;
			if(SplitMetadataIndex(strFullKey,strKey, nIndex))
			{
				int nKeyIndex = FindPMTKeyInTranslationTable(theTable, strKey);
				if(nKeyIndex >= 0)
					pParentNode->RemoveKeyVectorIndex(EkAToW((*theTable)[nKeyIndex]->mTag), nIndex);
				return ;
			}
		}
		int nKeyIndex = FindPMTKeyInTranslationTable(theTable, strFullKey);
		if(nKeyIndex >= 0)
		{
			XMPSpecialMetadataElement* pSpecialEle = NULL;
			if((pSpecialEle = XMPSpecialMetadataManager::FindElementByKey(strFullKey)) != NULL)
			{
				pParentNode = mpRDFNode->GetXMPNode(XMP_PMTSTRING2WIDE(pSpecialEle->mTag),false);
			}
			if(pParentNode)
				pParentNode->RemoveKey(EkAToW((*theTable)[nKeyIndex]->mTag));
		}		
	}
}

bool PmtXMPAccessor::Flush()
{
	bool bRetValue = false;
	//Output the node to a string buffer.
	if(mpXMPNode && mpXMPNode->ValidateNameSpace())
	{
		std::wostringstream oString;
		oString << *(mpXMPNode->GetXMPNode(L"adobe:ns:meta/xmpmeta"));

		//Use the string buffer to updata the XMPBlock in XMPBlockManager. the string is assumed to begin with "<x:xmpmeta" and end with "</x:xmpmeta>", if there is not enough space, it 		will failed.
		bRetValue = mBlockManager.UpdataMetaData(oString.str().c_str(), oString.str().length());
	}
	return bRetValue;
}

bool PmtXMPAccessor::Save()
{
	bool bRetValue = false;
	if((mOpenMode != PMT_READONLY) && Flush())
	{
		if(mFileHandle > 0)
		{
			bRetValue = (fseek(mFileHandle, mStartPosition, SEEK_SET) == 0);
			if(bRetValue)
			{
				size_t blockSize = XMPBlockSize();
				bRetValue = (fwrite(XMPBlockData(),1,blockSize,mFileHandle) == blockSize);
			}
		}
		else //Opened by memory
			bRetValue = true;
	}
	return bRetValue;
}
bool PmtXMPAccessor::Initialize()
{
	loadIfAny();
	mpXMPNode = XMPDataNode::Create();
	mRootMetadata = PmtMetadata::create();
	if(mpXMPNode)
		mpXMPNode->AddSubNode(XMPMETADATA_ROOTTAG);
	return (mpXMPNode && mRootMetadata);
}
bool PmtXMPAccessor::BuildPrivateData()
{
	mpRDFNode = mpXMPNode->GetXMPNode(XMPMETADATA_RDFTAG);
	mpRDFDescNode = mpRDFNode->GetXMPNode(XMPMETADATA_RDFDESCRIPTIONTAG);
	return mpRDFNode && mpRDFDescNode;
}
bool PmtXMPAccessor::CreateEmptyBlock()
{
	bool bRetValue = false;
	
	
	XMPDataNode* pXMPMetadata = mpXMPNode->GetXMPNode(XMPMETADATA_ROOTTAG);
	if(pXMPMetadata)
	{
		pXMPMetadata->AddNameSpace(L"x", L"adobe:ns:meta/");
		pXMPMetadata->AddAttribute(L"adobe:ns:meta/xmptk", L"3.1.2-113");
		pXMPMetadata->AddSubNode(XMPMETADATA_RDFTAG);

		mpRDFNode = pXMPMetadata->GetXMPNode(XMPMETADATA_RDFTAG);
		if(mpRDFNode)
		{
			mpRDFNode->AddNameSpace(L"rdf", L"http://www.w3.org/1999/02/22-rdf-syntax-ns#");

			mpRDFDescNode = mpRDFNode->AddSubNode(XMPMETADATA_RDFDESCRIPTIONTAG);
			if(mpRDFDescNode)
			{
				mpRDFDescNode->AddAttribute(L"http://www.w3.org/1999/02/22-rdf-syntax-ns#about", L"");
				bRetValue = true;
			}
		}
	}
	
	return bRetValue;
}

// #include <fstream>
// bool PmtXMPAccessor::TestWrite(PmtMetadataPtr mdRoot)
// {
// 	if(!mRootMetadata)
// 		return false;
// 
// 	PmtMetadataIterator mdIter = mdRoot->getMetadata("*[*]", false);
// 	PmtMetadataPtr md = mdIter.start();
// 
// 	bool bRet;
// 	while(md)
// 	{
// 		bRet = setMdValue(md);
// 		md = mdIter.next();
// 	}
// 	removeMdValue(mdRoot->getMetadatum("CaptureConditions.Aperture"));
// 	mpXMPNode->ValidateNameSpace();
// 
// 	wofstream osT(L"c:\\outaaaa.xml", wios::out);
// 	osT << *(mpXMPNode->GetXMPNode(XMPMETADATA_ROOTTAG));
// 	return true;
// }
