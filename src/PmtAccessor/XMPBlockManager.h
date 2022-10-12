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

#ifndef XMP_BLOCK_MANAGER_H_

#define XMP_BLOCK_MANAGER_H_



#if ((defined _MSC_VER) && (_MSC_VER > 1000))

#pragma once

#endif



#include "EkCompiler.h"

#include "PmtSAX2.h"

enum CharacterSet

{

	CHARACTERSET_UTF_NONE = 0,

	CHARACTERSET_UTF_8 = 1,

	CHARACTERSET_UTF_16_B = 2,

	CHARACTERSET_UTF_16_L = 3,

	CHARACTERSET_UTF_32_B = 4,

	CHARACTERSET_UTF_32_L = 5

};



//! StreamAccessor defines the interfaces for streams

class StreamAccessor

{

public:

	//! Current position

	virtual int CurrentPosition() = 0;



	//! Gets next valid character from the stream

	virtual bool GetNextValidChar(unsigned char &ch) = 0;



	//! Seeks to the given position

	virtual int SeekToPositon(int pos) = 0;



	//! Reads nBytes from the stream, return the number of bytes actually read

	virtual int Read(unsigned char* pBuffer, int nBytes) = 0;

	virtual ~StreamAccessor(){};

};



//! Stream whose buffer is stored in memory

class MemoryStreamAccessor:public StreamAccessor

{

public:

	//! Constructor, nLength of pData

	MemoryStreamAccessor(const unsigned char* pData, int nLength);

	virtual int CurrentPosition();

	virtual bool GetNextValidChar(unsigned char &ch);

	virtual int SeekToPositon(int pos);

	virtual int Read(unsigned char* pBuffer, int nBytes);

	virtual ~MemoryStreamAccessor() {}

private:

	//! Pointer to the memory buffer

	const unsigned char* pMemoryData;



	//! Length of the buffer

	int nMemoryLength;



	//! Current position

	int nCurrentPosition;

};



//! Stream whose data is stored in file

class FileStreamAccessor:public StreamAccessor

{

public:

	//! Constructor

	FileStreamAccessor(FILE* handle, int nStartPos = 0);

	virtual int CurrentPosition();

	virtual bool GetNextValidChar(unsigned char &ch);

	virtual int SeekToPositon(int pos);

	virtual int Read(unsigned char* pBuffer, int nBytes);

	virtual ~FileStreamAccessor(){}



private:

	//! File handle

	FILE* mHandle;



	//! Start position of the stream in file

	int nStartPosition;

};



//! XMPBlockManager offers a series of methods to find a XMP block and maintenance the content of the block

class XMPBlockManager

{

public:

	//! Constructor

	XMPBlockManager(void);



	//! Destructor

	~XMPBlockManager(void);

	

	/*! Block finders

		Return the index of first character of <?xpacket begin=, BAD_POSITION if not find.

		The found XMP block is stored in mpDataBytes, and the mBlockSize all the characters from "<?xpacket begin=" to <?xpacket end=\ 

		mMetadataOffset stores the offset of "<x:xmpmeta" in mpDataBytes, and mMetadataSize is the length of the metadatas.

	*/

	int FindXMPBlock(const wchar_t* filePath);

	int FindXMPBlock(const char* filePath);

	int FindXMPBlock(FILE* handle, const int pos);

	int FindXMPBlock(const unsigned char* pData, int nMaxLength);



	//! Returns the status of the block

	const bool& IsValid() const { return mbValid; }

	

	//! Returns the block data pointer in memory, whose content begin with <?xpacket begin="

	unsigned char* const XMPBlockData() const { return mpDataBytes; }



	//! Returns the size of the block

	int XMPBlockSize() const { return mBlockSize; }



	//! Returns the metadata pointer in memory, whose content begin with "<x:xmpmeta"

	unsigned char* const XMPMetaData() const { return mpDataBytes + mMetadataOffset; }



	//! Returns the size of metadata content

	int XMPMetaDataSize() const { return mMetadataSize; }

	const CharacterSet& EncodingType() const { return mCharacterSet; }



	//! Updates metadata, if the space is not enough, it will extend automatically.

	bool UpdataMetaData(const wchar_t* const pMetaData, int nLength);



	//! Returns the related name of the character set defined in XMLUni class.

	static const XMLCh* EncodingTypeName(CharacterSet characterSet) ;

	

	//! Whether the block is readonly, if "<?xpacket end="r"?>" is found at the last of the block, then return true, false if <?xpacket end="w"?>

	const bool& IsReadOnly() const { return bReadOnly; }



	//! Value of bad position 

	static const int BAD_POSITION = -1;

private:

	//! Find XMP block in a stream

	int FindXMPBlock(StreamAccessor* pAccessor);



	//! Recognizes the character set from current position of the stream.

	bool RecognizeCharacterSet(StreamAccessor* pAccessor);



	//! Checks the ID of the XMP block

	bool CheckID(StreamAccessor* pAccessor);



	//! Gets the size of the header and trailer of the block, it will differs upon the character set

	int GetXMPHeaderTrailerSize();



	//! Uses KMP algorithm to build the position array.

	void KMPGetNextPos(const char* const str, int next[]);



	//! Find the subStr from the stream, pos is the first position of the stream to find, if exists, return current position of the handle, otherwise -1.

	int KMPFinder(StreamAccessor* pAccessor, const int pos, char *subStr);



	//! Extend the space of the block, nSize is the lest size should be extend, and multiple of 1K bytes are extended acutually

	bool ExtendBlockSize(int nSize);



	//! Pads the space with blanks

	void PaddingSpace(unsigned char* pPaddingBytes, int nLength, CharacterSet characterSet = CHARACTERSET_UTF_8);



	//! Character set

	CharacterSet mCharacterSet;



	//! Size of the XMP block

	int mBlockSize;



	//! Pointer to the block buffer

	unsigned char* mpDataBytes;



	//! Offset of the metadata in the block buffer

	int mMetadataOffset;



	//! Size of the metadata

	int mMetadataSize;



	//! Whether the block is valid

	bool mbValid;



	//! Whether the block is readonly

	bool bReadOnly;



	//! Initialize a empty block, only with header and trailer

	void InitializeEmptyBlock();

};



#endif



