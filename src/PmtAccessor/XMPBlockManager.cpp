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
#include <string.h>
#include <stdio.h>

#include "XMPBlockManager.h"

#define XMP_BLOCK_HEADER_UTF8 "<?xpacket begin=\"\xEF\xBB\xBF\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?>\n"
#define XMP_BLOCK_TRAILER_UTF8 "\n<?xpacket end=\"w\"?>"

MemoryStreamAccessor::MemoryStreamAccessor(const unsigned char* pData, int nLength)
:pMemoryData(pData),nMemoryLength(nLength),nCurrentPosition(0)
{

}
int MemoryStreamAccessor::CurrentPosition()
{
	return nCurrentPosition;
}
bool MemoryStreamAccessor::GetNextValidChar(unsigned char &ch)
{
	bool bRetValue = false;
	while(nCurrentPosition < nMemoryLength)
	{
		//escape the null bytes.
		if(pMemoryData[nCurrentPosition] != 0)
		{
			ch = pMemoryData[nCurrentPosition];
			nCurrentPosition ++;
			bRetValue = true;
			break;
		}
		nCurrentPosition ++;
	}
	return bRetValue;
}
int MemoryStreamAccessor::SeekToPositon(int pos)
{
	nCurrentPosition = pos;
	return nCurrentPosition;
}
int MemoryStreamAccessor::Read(unsigned char* pBuffer, int nBytes)
{
	int nBytesCanRead = nBytes;
	//Make sure we will not read bytes exceed the boundary.
	if(nBytes + nCurrentPosition > nMemoryLength)
		nBytesCanRead = nMemoryLength - nCurrentPosition;
	memcpy(pBuffer, pMemoryData + nCurrentPosition, nBytesCanRead);
	return nBytesCanRead;
}

FileStreamAccessor::FileStreamAccessor(FILE* handle, int nStartPos /*= 0*/):mHandle(handle),nStartPosition(nStartPos)
{

}
int FileStreamAccessor::CurrentPosition()
{
	return ftell(mHandle) - nStartPosition;
}
bool FileStreamAccessor::GetNextValidChar(unsigned char &ch)
{
	bool bRetValue = false;
	while(!bRetValue && (fread(&ch,1,1,mHandle) == 1))
	{
		if(ch != '\0')
			bRetValue = true;
	}
	return bRetValue;
}
int FileStreamAccessor::SeekToPositon(int pos)
{
	return fseek(mHandle, pos + nStartPosition, SEEK_SET) - nStartPosition;
}
int FileStreamAccessor::Read(unsigned char* pBuffer, int nBytes)
{
	return fread(pBuffer, 1, nBytes, mHandle);
}

XMPBlockManager::XMPBlockManager(void)
{
	mBlockSize = 0;
	mMetadataSize = 0;
	mMetadataOffset = 0;
	mpDataBytes = NULL;
	mbValid = false;
	bReadOnly = true;
	mCharacterSet = CHARACTERSET_UTF_NONE;
}

XMPBlockManager::~XMPBlockManager(void)
{
	if(mpDataBytes)
		delete[] mpDataBytes;
}
int XMPBlockManager::FindXMPBlock(const wchar_t* filePath)
{
	wstring strFilePath = filePath;
	SPmtString pmtStr = EkWToA(strFilePath);
	return FindXMPBlock(pmtStr.c_str());
}
int XMPBlockManager::FindXMPBlock(const char* filePath)
{
	int nBlockPosition = BAD_POSITION;
	FILE* handle = fopen(filePath, "rb");
	if(handle > 0)
	{
		FileStreamAccessor* pAccessor = new FileStreamAccessor(handle);
		nBlockPosition =  FindXMPBlock(pAccessor);
		fclose(handle);
		delete pAccessor;
	}	
	return nBlockPosition;
}
int XMPBlockManager::FindXMPBlock(FILE* handle, const int pos)
{
	FileStreamAccessor* pAccessor = new FileStreamAccessor(handle,pos);
	int nBlockPosition = BAD_POSITION;
	nBlockPosition =  FindXMPBlock(pAccessor);
	delete pAccessor;
	return nBlockPosition;
}
int XMPBlockManager::FindXMPBlock(const unsigned char* pData, int nMaxLength)
{
	int nBlockPosition = BAD_POSITION;
	MemoryStreamAccessor *pAccessor = new MemoryStreamAccessor(pData,nMaxLength);
	nBlockPosition = FindXMPBlock(pAccessor);
	delete pAccessor;
	return nBlockPosition;
}

//This function is too complex, it is not easy to revise it to have only one return statement.
int XMPBlockManager::FindXMPBlock(StreamAccessor* pAccessor)
{
	int nBlockPosition = BAD_POSITION;

	static char startString[] = "<?xpacket begin=";
	static char endString[] = "<?xpacket end=";
	//Some systems do not use x as default.
	static char startMetadata[] = "<";//x:xmpmeta";
	static char endMetadata[] = "xmpmeta>";

	int nIndexFirstCharacter = 0;
	int nIndexLastCharacter = 0;
	nIndexFirstCharacter = KMPFinder(pAccessor,0,startString);
	if(nIndexFirstCharacter < 0)
		return nBlockPosition;
	
	unsigned char ch;
	if(!RecognizeCharacterSet(pAccessor))
		return nBlockPosition;
	//Find "\"" from the end of character.
	if(!pAccessor->GetNextValidChar(ch))
		return nBlockPosition;
	if(ch != '\"' && ch != '\'')
		return nBlockPosition;
	if(!CheckID(pAccessor))
		return nBlockPosition;

	//Find "<x:xmpmeta".
	int nIndexFirstMetadata = KMPFinder(pAccessor,nIndexFirstCharacter,startMetadata);
	if(nIndexFirstMetadata < 0)
		return nBlockPosition;

	//Find "</x:xmpmeta>".
	int nIndexLastMetadata = KMPFinder(pAccessor,nIndexFirstMetadata,endMetadata);
	if(nIndexLastMetadata < 0)
		return nBlockPosition;
	
	nIndexLastCharacter = KMPFinder(pAccessor,nIndexFirstCharacter, endString);
	if(nIndexLastCharacter < 0)
		return nBlockPosition;

	//Get the read write mode.
	if(!pAccessor->GetNextValidChar(ch))
		return nBlockPosition;
	if((ch != '\"') && (ch != '\''))
		return nBlockPosition;
	if(!pAccessor->GetNextValidChar(ch))
		return nBlockPosition;
	if(ch == 'r')
		bReadOnly = true;
	else if(ch == 'w')
		bReadOnly = false;
	else //Error mode.
		return nBlockPosition;
	//Find the last '>'
	while(pAccessor->GetNextValidChar(ch))
	{
		if(ch == '>')
			break;
	}
	nIndexLastCharacter = pAccessor->CurrentPosition();
	switch(mCharacterSet)
	{
	case CHARACTERSET_UTF_NONE:
	case CHARACTERSET_UTF_8:
		nIndexFirstCharacter -= strlen(startString);
		nIndexLastCharacter -= 1;
		nIndexFirstMetadata -= strlen(startMetadata);
		nIndexLastMetadata -= 1;
		break;
	case CHARACTERSET_UTF_16_L:
		nIndexFirstCharacter -= strlen(startString) * 2 - 1;
		//nIndexLastCharacter -= 0;
		nIndexFirstMetadata -= strlen(startMetadata) * 2 - 1;
		//nIndexLastMetadata -= 0;
		break;
	case CHARACTERSET_UTF_16_B:
		nIndexFirstCharacter -= strlen(startString) * 2;
		nIndexLastCharacter -= 1;
		nIndexFirstMetadata -= strlen(startMetadata) * 2;
		nIndexLastMetadata -= 1;
		break;
	case CHARACTERSET_UTF_32_L:
		nIndexFirstCharacter -= strlen(startString) * 4 - 3;
		nIndexLastCharacter += 2;
		nIndexFirstMetadata -= strlen(startMetadata) * 4 - 3;
		nIndexLastMetadata += 2;
		break;
	case CHARACTERSET_UTF_32_B:
		nIndexFirstCharacter -= strlen(startString) * 4;
		nIndexLastCharacter -= 1;
		nIndexFirstMetadata -= strlen(startMetadata) * 4;
		nIndexLastMetadata -= 1;
	}
	
	mBlockSize = nIndexLastCharacter - nIndexFirstCharacter + 1;
	mMetadataOffset = nIndexFirstMetadata - nIndexFirstCharacter;
	mMetadataSize = nIndexLastMetadata - nIndexFirstMetadata + 1;
	mpDataBytes = new unsigned char[mBlockSize];

	pAccessor->SeekToPositon(nIndexFirstCharacter);
	pAccessor->Read(mpDataBytes,mBlockSize);

	nBlockPosition = nIndexFirstCharacter;
	return nBlockPosition;
}
void XMPBlockManager::PaddingSpace(unsigned char* pPaddingBytes, int nLength, CharacterSet characterSet /*= CHARACTERSET_UTF_8*/)
{
	if(nLength <= 0)
		return;
	if(mCharacterSet == CHARACTERSET_UTF_8)
	{
		int nPaddingIndex = 1;
		while(nPaddingIndex <= nLength)
		{
			//Every 100 space character with one newline character.
			if((nPaddingIndex % 101) != 0)
				*(pPaddingBytes++) = 0x20;
			else
				*(pPaddingBytes++) = '\n';
			nPaddingIndex ++;
		}
		//Set last character as newline character.
		*(pPaddingBytes - 1) = '\n';			
	}
	else if(mCharacterSet == CHARACTERSET_UTF_16_L)
	{
		int nPaddingIndex = 1;
		while(nPaddingIndex <= nLength - 1)
		{
			//Every 100 space character with one newline character.
			if((nPaddingIndex % 101) != 0)
			{
				*(pPaddingBytes++) = 0x20;
				*(pPaddingBytes++) = 0;
			}
			else
			{
				*(pPaddingBytes++) = '\n';
				*(pPaddingBytes++) = 0;
			}
			nPaddingIndex += 2;
		}
		//Set last character as newline character.
		*(pPaddingBytes - 2) = '\n';	
		*(pPaddingBytes - 1) = 0;
	}
	else if(mCharacterSet == CHARACTERSET_UTF_16_B)
	{
		int nPaddingIndex = 1;
		while(nPaddingIndex <= nLength - 1)
		{
			//Every 100 space character with one newline character.
			if((nPaddingIndex % 101) != 0)
			{
				*(pPaddingBytes++) = 0;
				*(pPaddingBytes++) = 0x20;
			}
			else
			{
				*(pPaddingBytes++) = 0;
				*(pPaddingBytes++) = '\n';
			}
			nPaddingIndex += 2;
		}
		//Set last character as newline character.
		*(pPaddingBytes - 2) = 0;
		*(pPaddingBytes - 1) = '\n';	
	}
}
bool XMPBlockManager::ExtendBlockSize(int nSize)
{
	bool bRetValue = false;
	if(!bReadOnly)
	{
		int nExtendedSize = (nSize + 1023) / 1024 * 1024;
		unsigned char* pNewDataBytes = new unsigned char[mBlockSize + nExtendedSize];
		int nBlankStart =  mMetadataSize + mMetadataOffset;
		memcpy(pNewDataBytes, mpDataBytes,nBlankStart);

		//padding the extended bytes.
		if(mCharacterSet == CHARACTERSET_UTF_8 || mCharacterSet == CHARACTERSET_UTF_16_L || mCharacterSet == CHARACTERSET_UTF_16_B)
		{
			PaddingSpace(pNewDataBytes + nBlankStart, nExtendedSize, mCharacterSet);
			memcpy(pNewDataBytes + nBlankStart + nExtendedSize, mpDataBytes + nBlankStart, mBlockSize - nBlankStart); 
			mBlockSize = mBlockSize + nExtendedSize;
			delete[] mpDataBytes;
			mpDataBytes = pNewDataBytes;
			bRetValue = true;
		}
		//CharacterSet more than 16 bits are not supported.
	}
	return bRetValue;
}
void XMPBlockManager::InitializeEmptyBlock()
{
	mCharacterSet = CHARACTERSET_UTF_8;
	mMetadataOffset = sizeof(XMP_BLOCK_HEADER_UTF8) - 1;
	mBlockSize = sizeof(XMP_BLOCK_HEADER_UTF8) - 1 + sizeof(XMP_BLOCK_TRAILER_UTF8) - 1;
	mMetadataSize = 0;
	mbValid = true;
	bReadOnly = false;
	mpDataBytes = new unsigned char[mBlockSize];
	memcpy(mpDataBytes,XMP_BLOCK_HEADER_UTF8, sizeof(XMP_BLOCK_HEADER_UTF8) - 1);
	memcpy(mpDataBytes + sizeof(XMP_BLOCK_HEADER_UTF8) - 1, XMP_BLOCK_TRAILER_UTF8, sizeof(XMP_BLOCK_TRAILER_UTF8) - 1 );
}
bool XMPBlockManager::UpdataMetaData(const wchar_t* const pMetaData, int nLength)
{
	bool bRetValue = false;
	if(nLength <= 0)
		return bRetValue;
	if(mBlockSize <= 0)
		InitializeEmptyBlock();
	int nBlankSize = mBlockSize - GetXMPHeaderTrailerSize();
	switch(mCharacterSet)
	{
	case CHARACTERSET_UTF_NONE:
	case CHARACTERSET_UTF_8:
		{
			SPmtString pmtStr = EkWToA(pMetaData);
			string metadata = pmtStr.c_str();
			metadata = metadata.substr(0,nLength);
			int nSize = metadata.size();
			if((nSize <= nBlankSize) || ((nSize > nBlankSize) && ExtendBlockSize(nSize - nBlankSize)))
			{
				memcpy(mpDataBytes + mMetadataOffset, metadata.c_str(), nSize);
				if(nSize < mMetadataSize)
					PaddingSpace(mpDataBytes + mMetadataOffset + nSize,mMetadataSize - nSize, mCharacterSet);
				mMetadataSize = nSize;
				bRetValue = true;
			}
		}
		break;
	case CHARACTERSET_UTF_16_L:
		{
			int nSize = nLength * 2;
			if((nSize <= nBlankSize) || ((nSize > nBlankSize) && ExtendBlockSize(nSize - nBlankSize)))
			{
				memcpy(mpDataBytes + mMetadataOffset, pMetaData, nSize);
				if(nLength * 2 < mMetadataSize)
					PaddingSpace(mpDataBytes + mMetadataOffset + nSize,mMetadataSize - nSize, mCharacterSet);
				mMetadataSize = nLength;
				bRetValue = true;
			}
		}
		break;
	case CHARACTERSET_UTF_16_B:
	case CHARACTERSET_UTF_32_L:
	case CHARACTERSET_UTF_32_B:
		{
			//Those character set is not supported yet.
		}
	}
	return bRetValue;
}
#define MAX_XMP_HEADER_TRAILER_SIZE 200
int XMPBlockManager::GetXMPHeaderTrailerSize()
{
	static char* strEnd = "<?xpacket end=\"w\"?>";

	int nHeaderTrailerSize = MAX_XMP_HEADER_TRAILER_SIZE;
	switch(mCharacterSet)
	{
	case CHARACTERSET_UTF_NONE:
	case CHARACTERSET_UTF_8:
		nHeaderTrailerSize = strlen(strEnd) + mMetadataOffset;
		break;
	case CHARACTERSET_UTF_16_L:
	case CHARACTERSET_UTF_16_B:
		nHeaderTrailerSize = strlen(strEnd) * 2 + mMetadataOffset;
		break;
	case CHARACTERSET_UTF_32_L:
	case CHARACTERSET_UTF_32_B:
		nHeaderTrailerSize = strlen(strEnd) * 4 + mMetadataOffset;
		break;
	}
	return nHeaderTrailerSize;
}
bool XMPBlockManager::RecognizeCharacterSet(StreamAccessor* pAccessor)
{
	unsigned char ch;
	bool return_value = false;
	mCharacterSet = CHARACTERSET_UTF_NONE;
	
	if(pAccessor->GetNextValidChar(ch))
	{
		//Get the ' or " first
		if(ch == '\'' || ch == '\"')
		{
			pAccessor->GetNextValidChar(ch);
			int nStartPosition = pAccessor->CurrentPosition();
			if(ch == 239) //EF BB BF is UTF-8
			{
				if((pAccessor->GetNextValidChar(ch)) && (ch == 187))
					if((pAccessor->GetNextValidChar(ch))&&(ch == 191))
						mCharacterSet = CHARACTERSET_UTF_8;
			}
			else if(ch == 254) //FE FF is UTF-16  big endian
			{
				if((pAccessor->GetNextValidChar(ch)) && (ch == 255))
					mCharacterSet = CHARACTERSET_UTF_16_B;
			}
			else if(ch == 255) //FF FE is UTF-16 little endian
			{
				if((pAccessor->GetNextValidChar(ch)) && (ch == 254))
					mCharacterSet = CHARACTERSET_UTF_16_L;
			}
			if (mCharacterSet != CHARACTERSET_UTF_NONE)
			{
				int nEndPosition = pAccessor->CurrentPosition();
				if(nEndPosition - nStartPosition > 2) //32 bits encoding.
				{
					if(mCharacterSet == CHARACTERSET_UTF_16_L)
						mCharacterSet = CHARACTERSET_UTF_32_L;
					else
						mCharacterSet = CHARACTERSET_UTF_32_B;
				}
				return_value = true;
			}
		}
	}
	return return_value;
}
bool XMPBlockManager::CheckID(StreamAccessor* pAccessor)
{
	static char xmpID[] ="id=\"W5M0MpCehiHzreSzNTczkc9d\"";
	static char xmpID2[] ="id=\'W5M0MpCehiHzreSzNTczkc9d\'";

	bool bRetValue = false;
	unsigned int nIDIndex = 0;
	unsigned char ch;
	bool bPrefix = true;

	int nSavedPosition = pAccessor->CurrentPosition();
	while(!bRetValue && pAccessor->GetNextValidChar(ch))
	{
		//Escape the first blank characters.
		if( ( !bPrefix || (ch != ' ' && ch != '\t')))
		{
			bPrefix = false;
			if(ch == xmpID[nIDIndex])
				nIDIndex ++;
			else
				break;
			if(nIDIndex >= strlen(xmpID))
				bRetValue = true;
		}
	}
	//Some system use ' instead of ".
	if(!bRetValue)
	{
		bool bPrefix = true;
		nIDIndex = 0;
		pAccessor->SeekToPositon(nSavedPosition);
		while(!bRetValue && pAccessor->GetNextValidChar(ch))
		{
			//Escape the first blank characters.
			if( ( !bPrefix || (ch != ' ' && ch != '\t')))
			{
				bPrefix = false;
				if(ch == xmpID2[nIDIndex])
					nIDIndex ++;
				else
					break;
				if(nIDIndex >= strlen(xmpID))
					bRetValue = true;
			}
		}
	}
	return bRetValue;
}
const XMLCh* XMPBlockManager::EncodingTypeName(CharacterSet characterSet) 
{
	const XMLCh* szName = NULL;
	switch(characterSet)
	{
#ifdef PMT_USE_XERCES

 	case CHARACTERSET_UTF_8:
 		szName = XMLUni::fgUTF8EncodingString;
		break;
	case CHARACTERSET_UTF_16_L:
		szName = XMLUni::fgUTF16LEncodingString;
		break;
	case CHARACTERSET_UTF_16_B:
		szName = XMLUni::fgUTF16BEncodingString;
		break;
	//32 bits encoding seems not supported by SAX.
#endif
	default:;
	}
	return szName;
}


void XMPBlockManager::KMPGetNextPos(const char* const str, int next[])
{
	int nNextPos = -1;
	unsigned int nIndex = 0;
	next[0] = nNextPos;
	while(nIndex < strlen(str) - 1)
	{
		if((nNextPos == -1) || (str[nIndex] == str[nNextPos]))
		{
			nIndex++;
			nNextPos++;
			next[nIndex] = nNextPos;
		}
		else
			nNextPos = next[nNextPos];
	}
}

int XMPBlockManager::KMPFinder(StreamAccessor* pAccessor, const int pos, char *subStr)
{
	int nRetEndPosition = BAD_POSITION;
	int nLengthOfSubStr = strlen(subStr);
	int *aryNextPos = new int[nLengthOfSubStr + 1];
	KMPGetNextPos(subStr, aryNextPos);

	pAccessor->SeekToPositon(pos);
	
	unsigned char ch;
	bool bNeedRead = true;
	
	int nMatchedLength = 0;
	while(nMatchedLength < nLengthOfSubStr)
	{
		if(!bNeedRead || (bNeedRead && pAccessor->GetNextValidChar(ch)))
		{
			if((nMatchedLength == -1) || (ch == subStr[nMatchedLength]))
			{
				bNeedRead = true;
				nMatchedLength++;
			}
			else
			{
				bNeedRead = false;
				nMatchedLength = aryNextPos[nMatchedLength];
			} 
		}
		else
			break;
	}
	delete[] aryNextPos;
	if(nMatchedLength == nLengthOfSubStr)
		nRetEndPosition = pAccessor->CurrentPosition();
	return nRetEndPosition;
}
