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



#ifndef PMT_XMP_ACCESSOR__H

#define PMT_XMP_ACCESSOR__H



#if ((defined _MSC_VER) && (_MSC_VER > 1000))

#pragma once

#endif



#include "PmtCommon.h"

#include "PmtAccrImplDefs.h"

#include "PmtAccessor.h"



#include "XMPBlockManager.h"

#include "PmtXMPSAXParser.h"



enum XMPTypeMode

{

	XMPTypeModeNone = 0,

	XMPTypeModeSimple = 1,

	XMPTypeModeSeqVector = 2,

	XMPTypeModeBagVector = 3,

	XMPTypeModeComposited = 4,  //Structure

	XMPTypeModeComplex = 5 //Structure, but itself and some of its elements are all metadatas.

};



class PmtExifAccessor;

class PmtQTAccessor;

class PmtXMPAccessor;

typedef EkSmartPtr < PmtXMPAccessor > PmtXMPAccessorPtr;

typedef EkConstSmartPtr < PmtXMPAccessor > ConstPmtXMPAccessorPtr;

class PMTACCR_IMPL_DECL PmtXMPAccessor:public PmtAccessor

{

public:

	friend class PmtExifAccessor;

	friend class PmtQTAccessor;



	//! The constructor

	PmtXMPAccessor(void);



	//! The Destructor

	~PmtXMPAccessor(void);



	/*! Opens a XMP text file for reading/writing (depending on the mode)



	\param name The name of the file to open

	\param mode The open mode

	\return true if successful, false otherwise   

	*/

	virtual bool open(const char *name, PmtOpenMode mode = PMT_READONLY);



	/*! Opens a XMP document from memory



	\param buf The buffer with the document to open

	\param bufSize The size of the buffer

	\param mode The open mode

	\return true if successful, false otherwise

	*/

	bool open(const unsigned char * buf, const unsigned int bufSize, PmtOpenMode mode = PMT_READONLY);



	/*! Creates a new XMP document



	\return true if successful, false otherwise

	*/

	bool open();



	/*! Appends/merges a XMP text file to an already open document



	\param name The name of the file to append

	\return true if successful, false otherwise  

	*/

	bool append( const char *name );



	/*! Appends/merges a XMP document from memory to an already open document



	\param buf The buffer with the document to open

	\param bufSize The size of the buffer

	\return true if successful, false otherwise

	*/

	bool append( const unsigned char * buf, const unsigned int bufSize );



	//! Close, and save the metadatas to file if mode is not PMT_READONLY.

	virtual void close() ;





	/*! Close to Memory



	The caller is responsible for creating and deleting the passed in

	buffer.

	\param buf The buffer to use for writing the document

	\param bufSize The size of the buffer

	\return positive int equal to the size of the document, if successful;

	negative int with absolute value equal to the required buffer

	size, if passed in buffer isn't big enough;

	0 for any other error.

	*/

	int close( char * buf, unsigned int bufSize );



	

	//! Return a copy of the PmtExifAccessor.

	inline virtual PmtAccessorPtr clone() const

	{

		return new PmtXMPAccessor();

	}



	virtual PmtMetadataPtr readMetadata(const SPmtString nsURI = "");

	virtual bool readMetadata(PmtMetadataPtr rootMd, bool ignoreIfExist = false);



	//! Check if the given file is has XMP block.

	static bool recognize(const SPmtString & name);



	//Get the start position of the block in file or memory.

	const int& StartPosition() const { return mStartPosition; }



	//Write the XMP block to file, it will begin with <?xpacket begin="??" id="W5M0MpCehiHzreSzNTczkc9d"?>", and end with  "<?xpacket end="w"?>"

	bool SaveXMPBlockToXMLFile(const char* szFilePathName);

	const unsigned char* XMPBlockData() const { return mBlockManager.XMPBlockData(); }

	int XMPBlockSize() const { return mBlockManager.XMPBlockSize(); }



	//Write the XMP block to file, it will begin with "<x:xmpmeta", and end with  "</x:xmpmeta>"

	bool SaveXMPMetadataToXMLFile(const char* szFilePathName);

	const unsigned char* XMPMetadata() const { return mBlockManager.XMPMetaData(); }

	int XMPMetadataSize() const { return mBlockManager.XMPMetaDataSize(); }



	CharacterSet XMPBlockCharacterSet() const { return mBlockManager.EncodingType(); }



	//Write the changed node to block manager. That is, write the XMP block to memory.

	bool Flush();



	//Write the XMP block to file if needed.

	bool Save();



	//bool TestWrite(PmtMetadataPtr mdRoot);

protected:



	virtual bool getMdValue(PmtMetadataPtr& md);



	virtual bool setMdValue(const PmtMetadataPtr& md);



	// Remove the metadata specified by md from the file.

	virtual void removeMdValue(const PmtMetadataPtr& md);



private:

	bool Initialize();

	bool BuildPrivateData();

	bool CreateEmptyBlock();



	//! Read metadatas from XMP block to mdRoot.

	bool ReadMetaData(PmtMetadataPtr& mdRoot);



	//! Read metadatas from node to mdRoot.

	bool ReadMetaData(PmtMetadataPtr& mdRoot, XMPDataNode& node);



	bool AssignXMPValueToPMTValue(PmtMetadataPtr &md, const wstring& strValue, const SPmtString& strTypeName);

	bool AssignXMPValueToPMTValue(PmtMetadataPtr &md, const vector<wstring>& strValues, const SPmtString& strTypeName);

	bool AssignCompositedValuetoPMTValue(PmtMetadataPtr& md, XMPDataNode& node, const SPmtString& strTypeName);

	bool AssignLangAltToPMTValue(PmtMetadataPtr& md, XMPDataNode& node);

	bool AssignOECFToPMTValue(PmtMetadataPtr& md, XMPDataNode& node);

	bool AssignFlashToPMTValue(PmtMetadataPtr& md, XMPDataNode& node);

	bool AssignCFAPatternToPMTValue(PmtMetadataPtr& md, XMPDataNode& node);

	bool AssignTimeToPMTValue(PmtMetadataPtr& md, XMPDataNode& node);

	bool AssignDeviceSettingToPMTValue(PmtMetadataPtr& md, XMPDataNode& node);



	bool IsMultiPMTValue(const SPmtString& strKey);

	void SplitMetadataFullName(const SPmtString& strFullKey, SPmtString& strPrefix, SPmtString& strName);

	bool SplitMetadataIndex(const SPmtString& strFullKey, SPmtString& strKey, unsigned int& nIndex);

	bool SetMdValueVectorIndex(const PmtMetadataPtr& md, const wstring& strKey, const SPmtString& strTypeName, XMPDataNode& node, unsigned int nIndex);

	bool SetMdValueBagIndex(const PmtMetadataPtr& md, const wstring& strKey, const SPmtString& strTypeName, XMPDataNode& node, unsigned int nIndex);

	bool AssignMutiXMPValueToPMTValue(PmtMetadataPtr& mdRoot, const vector<wstring>& strValues, const SPmtString& strKey, const SPmtString& strTypeName);



	bool WriteMdValue(const PmtMetadataPtr& md);

	bool SetMdValue(const PmtMetadataPtr& md, const wstring& strKey, const SPmtString& strTypeName, XMPDataNode& node);

	bool SetMdValueVector(const PmtMetadataPtr& md, const wstring& strKey, const SPmtString& strTypeName, XMPDataNode& node);

	//bool SetMdValueBag(const PmtMetadataPtr& md, const wstring& strKey, const SPmtString& strTypeName, XMPDataNode& node);

	bool SetMdValueComposited(const PmtMetadataPtr& md, const wstring& strKey, const SPmtString& strTypeName, XMPDataNode& node);

	bool SetMdValueLangAlt(const PmtMetadataPtr& md, const wstring& strKey, XMPDataNode& node);

	bool SetMdValueOECF(const PmtMetadataPtr& md, const wstring& strKey, XMPDataNode& node);

	bool SetMdValueFlash(const PmtMetadataPtr& md, const wstring& strKey, XMPDataNode& node);

	bool SetMdValueCFAPattern(const PmtMetadataPtr& md, const wstring& strKey, XMPDataNode& node);

	bool SetMdValueTime(const PmtMetadataPtr& md, const wstring& strKey, XMPDataNode& node);

	bool SetMdValueDeviceSetting(const PmtMetadataPtr& md, const wstring& strKey, XMPDataNode& node);



	// Because one XMP metadata may refer to multiple PMT element, so we should find all the PMT elements.

	//int FindXMPKeyInTranslationTable(PmtTranslationTableElements * theTable, const SPmtString& strXMPKey);

	void FindXMPKeysInTranslationTable(PmtTranslationTableElements * theTable, const SPmtString& strXMPKey, vector<int>& vIndexes);

	int FindPMTKeyInTranslationTable(PmtTranslationTableElements * theTable, const SPmtString& strPMTKey);

	XMPTypeMode GetXMPTypeMode(const SPmtString& strTypeName);



	bool ParseXMPBlock(const unsigned char* pDate, int nLength, CharacterSet charaSet);

	void Destroy();



	//! Block manager

	XMPBlockManager mBlockManager;



	//! XML parser

	PmtXMPSAXParser mXMPparser;



	//! File handle

	FILE* mFileHandle;



	//! Start position of the XMP block in file

	int mStartPosition;



	//! Open mode

	PmtOpenMode mOpenMode;



	//! Root XMP node

	XMPDataNode* mpXMPNode;



	//! RDF node

	XMPDataNode* mpRDFNode;



	//! RDF description node

	XMPDataNode* mpRDFDescNode;



	//! Root metadata pointer

	PmtMetadataPtr mRootMetadata;

};



#endif