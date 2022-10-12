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
#include "XMPTypeTranslater.h"
#include <cstdio>
#include <sstream>
#include <cmath>
#include "PmtSAX2.h"
#include <algorithm>
#include <cstdlib>
#include <wchar.h>
wstring XMLChToWideString(const XMLCh* const xmlStr)
{
	PmtSAXString saxString(xmlStr);
	SPmtString pmtStr = saxString.getString();
	return EkAToW(pmtStr);
}
XMPTypeRational::XMPTypeRational(float fValue)
{
	int nValue = fValue;
	if(abs(fValue - (double)nValue) < 0.0001)
	{
		nNumerator = nValue;
		nDenominator = 1;
	}
	else
	{
		double fTemp;
		for(nDenominator = 1; nDenominator < 100000; nDenominator ++)
		{
			float fNumerator = fValue * nDenominator;
			nNumerator = fValue * nDenominator;
			if(fNumerator - nNumerator > 0.5)
				nNumerator ++;
			fTemp = (float)nNumerator / (float)nDenominator;
			if(abs(fTemp - fValue) < 0.0001)
				break;
		}
	}
}
wstring XMPTypeRational::ToXMPString() const
{
	wostringstream outStringStream;
	outStringStream << nNumerator << L"/" << nDenominator;
	return outStringStream.str();
}
wstring XMPTypeRational::ToFloatString() const
{
	wostringstream outStringStream;
	outStringStream << ToFloat();
	return outStringStream.str();
}
bool XMPTypeRational::FromXMPString(const wstring& strValue)
{
	bool bSuccessed = false;
	int nSeperator = strValue.find(L'/');
	if(nSeperator >= 1)
	{
		wstring strNumerator = strValue.substr(0,nSeperator);
		nNumerator = XMPTypeTranslater::FromXMPTypeInt(strNumerator);
		wstring strDenominator = strValue.substr(nSeperator + 1, strValue.size() - 1 - nSeperator);
		nDenominator = XMPTypeTranslater::FromXMPTypeInt(strDenominator);
		bSuccessed = true;
	}
	return bSuccessed;
}

void XMPTypeRational::ToVUint8(vector<unsigned char>& vValues) const
{
	unsigned char* pInt = (unsigned char*)(&nNumerator);
	int i;
	for (i = 0; i < 4; i ++)
		vValues.push_back(pInt[i]);
	pInt = (unsigned char*)(&nDenominator);
	for (i = 0; i < 4; i ++)
		vValues.push_back(pInt[i]);
}
void XMPTypeRational::FromVUint8(vector<unsigned char>::const_iterator itBeg)
{
	int i;
	unsigned char* pInt = (unsigned char*)(&nNumerator);
	for (i = 0; i < 4 ; i ++)
	{
		pInt[i] = *(itBeg ++);
	}
	pInt = (unsigned char*)(&nDenominator);
	for (i = 0; i < 4 ; i ++)
	{
		pInt[i] = *(itBeg ++);
	}
}
XMPTypeDate::XMPTypeDate()
{
	nYear = 0;
	nMonth = 0 ;
	nDay = 0;
	nHour = 0;  
	nMinute = 0;
	nSecond = 0;
	nFraction = 0;
	nPrecision = 0;
	bAbsolute = false;
	bAhead = false;
	nOffsetHours = 0;
	nOffsetMinutes = 0;
}
bool XMPTypeDate::FromXMPString(const wstring &strValue)
{
	wstring strTemp;
	bool bHasError = false;
	if(strValue.size() >= 4) //YYYY
	{
		strTemp = strValue.substr(0,4);
		nYear = XMPTypeTranslater::FromXMPTypeInt(strTemp);
	}
	else
		bHasError = true;
	if(!bHasError && (strValue.size() >= 4 + 1 + 2)) //YYYY-MM
	{
		if(strValue[4] == L'-')
		{
			strTemp = strValue.substr(5, 2);
			nMonth = XMPTypeTranslater::FromXMPTypeInt(strTemp);
		}
		else
			bHasError = true;
	}
	if(!bHasError && (strValue.size() >= 7 + 1 + 2)) //YYYY-MM-DD
	{
		if(strValue[7] == L'-')
		{
			strTemp = strValue.substr(8, 2);
			nDay = XMPTypeTranslater::FromXMPTypeInt(strTemp);
		}
		else
			bHasError = true;		
	}
	if(!bHasError && (strValue.size() >= 10 + 1 + 2)) //YYYY-MM-DDThh
	{
		if(strValue[10] == L'T')
		{
			strTemp = strValue.substr(11, 2);
			nHour = XMPTypeTranslater::FromXMPTypeInt(strTemp);
		}
		else
			bHasError = true;		
	}
	if(!bHasError && (strValue.size() >= 13 + 1 + 2))  //YYYY-MM-DDThh:mm
	{
		if(strValue[13] == L':')
		{
			strTemp = strValue.substr(14, 2);
			nMinute = XMPTypeTranslater::FromXMPTypeInt(strTemp);
		}
		else
			bHasError = true;
	}
	if(!bHasError && (strValue.size() > 16))
	{
		if(strValue[16] == L':')
		{
			if(strValue.size() >= 16 + 1 + 2) //YYYY-MM-DDThh:mm:ss
			{
				strTemp = strValue.substr(17, 2);
				nSecond = XMPTypeTranslater::FromXMPTypeInt(strTemp);
			}
			else
				bHasError = true;
			if(!bHasError && (strValue.size() > 19))
			{
				if((strValue[19] == L'.')) //YYYY-MM-DDThh:mm:ss.s
				{
					wchar_t temp[20];
					unsigned int nIndex;
					for (nIndex = 20; nIndex < strValue.length(); nIndex ++)
					{
						if(strValue[nIndex] >= L'0' && strValue[nIndex] <= L'9')
						{
							temp[nPrecision] = strValue[nIndex];
							nPrecision ++;
						}
						temp[nPrecision] = L'\0';
						nPrecision --;
					}
					strTemp = temp;
					nFraction = XMPTypeTranslater::FromXMPTypeInt(strTemp);
				}
				else//TZD(time zone designator)
				{
					wstring strTZD = strValue.substr(19, strValue.size() - 19);
					if(!ParseTZD(strTZD, bAbsolute, bAhead, nOffsetHours, nOffsetMinutes ))
						bHasError = true;
				}
			}
		}
		else if(!bHasError)//TZD(time zone designator)
		{
			wstring strTZD = strValue.substr(16, strValue.size() - 16);
			if(!ParseTZD(strTZD, bAbsolute, bAhead, nOffsetHours, nOffsetMinutes ))
				bHasError = true;
		}
	}
	return !bHasError;
}
bool XMPTypeDate::ParseTZD(const wstring& TZD, bool& bAbsolute, bool& bAhead, int& nHours, int& nMinutes )
{
	bAbsolute = false;
	bAhead = false;
	nHours = 0;
	nMinutes = 0;
	if(TZD.size() < 1)
		return false;
	if(TZD[0] == L'Z')
	{
		bAbsolute = true;
		return true;
	}
	else if(TZD[0] == L'+')
		bAhead = true;
	else if(TZD[0] == L'-')
		bAhead = false;
	else
		return false;
	//Parse hh:mm
	bool bRetValue = false;
	if(TZD.size() >= 6)
	{
		wstring strTemp;
		strTemp = TZD.substr(1,2);
		nHours = XMPTypeTranslater::FromXMPTypeInt(strTemp);
		if(TZD[3] == L':')
		{
			strTemp = TZD.substr(4,2);
			nMinutes = XMPTypeTranslater::FromXMPTypeInt(strTemp);
			bRetValue = true;
		}		
	}	
	return bRetValue;
}

//Format is YYYY:MM:DD HH:MM:SS
bool XMPTypeDate::FromPMTString(const wstring &strValue)
{
	bool bRetValue = false;
	if((strValue[4] == L':') && (strValue[7] == L':') &&
		(strValue[10] == L' ') && (strValue[13] == L':') && (strValue[16] == L':'))
	{
		wstring strTemp;
		strTemp = strValue.substr(0,4);
		nYear = XMPTypeTranslater::FromXMPTypeInt(strTemp);
		strTemp = strValue.substr(5, 2);
		nMonth = XMPTypeTranslater::FromXMPTypeInt(strTemp);
		strTemp = strValue.substr(8,2);
		nDay = XMPTypeTranslater::FromXMPTypeInt(strTemp);
		strTemp = strValue.substr(11,2);
		nHour = XMPTypeTranslater::FromXMPTypeInt(strTemp);
		strTemp = strValue.substr(14,2);
		nMinute = XMPTypeTranslater::FromXMPTypeInt(strTemp);
		strTemp = strValue.substr(17,2);
		nSecond = XMPTypeTranslater::FromXMPTypeInt(strTemp);
		bRetValue = true;
	}
	return bRetValue;
}
/*YYYY
YYYY-MM
YYYY-MM-DD
YYYY-MM-DDThh:mmTZD
YYYY-MM-DDThh:mm:ssTZD
YYYY-MM-DDThh:mm:ss.sTZD*/
wstring XMPTypeDate::ToXMPTypeString() const
{
	wchar_t buffer[20]; 
	memset(buffer, 0, 10 * sizeof(wchar_t));
	wostringstream oStream;
	oStream << nYear ;

	if(nMonth > 0 && nMonth <= 12)
	{
		swprintf(buffer, 10,L"-%02d", nMonth);
		oStream << buffer;
		if(nDay > 0 && nDay <= 31)
		{
			swprintf(buffer,10, L"%02d", nDay);
			oStream << L"-" << buffer;
			swprintf(buffer,20, L"T%02d:%02d:%02d", nHour, nMinute, nSecond);
			oStream << buffer << L"Z";
		}
	}
	return oStream.str();
}
wstring XMPTypeDate::ToPMTTypeString() const
{
	wchar_t buffer[40]; //The length is fixed 20 character, but in case it overflow.
	memset(buffer, 0, 40 * sizeof(wchar_t));
	swprintf(buffer, 40,L"%4d:%02d:%02d %02d:%02d:%02d", nYear, nMonth, nDay, nHour, nMinute, nSecond);
	wstring strValue = buffer;
	if(strValue.size() >= 20)
		strValue = L"";
	return strValue;
}
void XMPTypeOECF::ToVUint8(vector<unsigned char>& vValues) const
{
	unsigned char nLow, nHigh;
	nLow = nColumns & 0xff;
	nHigh = (nColumns & 0xff00) >> 8;
	vValues.push_back(nLow);
	vValues.push_back(nHigh);
	nLow = nRows & 0xff;
	nHigh = (nRows & 0xff00) >> 8;
	vValues.push_back(nLow);
	vValues.push_back(nHigh);

	//Save column names.
	vector<wstring>::const_iterator itStr;
	for (itStr = strColumnsNames.begin(); itStr != strColumnsNames.end(); itStr ++)
	{
		XMPTypeTranslater::StringToVUint8(*itStr, vValues, true);
	}
	
	//Save OECF values.
	vector<XMPTypeRational>::const_iterator itRational;
	for (itRational = vRationals.begin(); itRational != vRationals.end(); itRational ++)
	{
		itRational->ToVUint8(vValues);
	}

}
bool XMPTypeOECF::FromVUint8(vector<unsigned char>& vValues)
{
	bool bRetValue = false;

	//Get columns and rows
	nColumns = vValues[0] + (vValues[1] * (2L << 8));
	nRows = vValues[2] + (vValues[3] * (2L << 8));

	//Get column names.
	strColumnsNames.clear();
	vector<unsigned char> nStrValue;
	vector<unsigned char>::iterator itBeg;
	vector<unsigned char>::iterator itEnd;
	itBeg = vValues.begin() + 4;
	unsigned int nColIndex;
	vector<unsigned char>::value_type chNull = 0;
	for (nColIndex = 0; nColIndex != nColumns; nColIndex ++)
	{
		itEnd = find(itBeg, vValues.end(), chNull);
		if(itEnd != vValues.end())
		{
			nStrValue.assign(itBeg, itEnd);
			itBeg = itEnd + 1;
			strColumnsNames.push_back(XMPTypeTranslater::VUint8ToString(nStrValue));
		}
		else
			break;		
	}
	
	//Get values.	
	if(nColIndex == nColumns)
	{
		unsigned int nRationIndex;
		vRationals.clear();
		XMPTypeRational rational;
		if(vValues.size() >= itBeg - vValues.begin() + 8 * nRows * nColumns)
		{
			for (nRationIndex = 0; nRationIndex != nRows * nColumns; nRationIndex ++)
			{
				rational.FromVUint8(itBeg);
				vRationals.push_back(rational);
				itBeg += 8;
			}
			bRetValue = true;
		}
	}
	return bRetValue;
}
//This tag indicates the status of flash when the image was shot.
//Bit 0 indicates the flash firing status, bits 1 and 2 indicate the flash return status
//bits 3 and 4 indicate the flash mode, bit 5 indicates whether the flash function is present
//and bit 6 indicates \"red eye\" mode. 

unsigned char XMPTypeFlash::ToByte() const
{
	unsigned char nFlash;
	nFlash = bFired ? 1 : 0;

	nFlash += nReturn << 1;
	nFlash += nMode << 3;
	nFlash += bFunction ? 1 << 5 : 0;
	nFlash += bRedEyeMode ? 1 << 6 : 0;
	return nFlash;
}
void XMPTypeFlash::FromByte(unsigned char chValue)
{
	bFired = chValue & 0x1;
	bFunction = chValue & 0x10;
	bRedEyeMode = chValue & 0x20;

	nReturn = (chValue & 0x06) >> 1;
	nMode = (chValue & 0x14) >> 3;
}
void XMPTypeCFAPattern::ToVUint8(vector<unsigned char>& vValues)
{
	unsigned char nLow, nHigh;
	nLow = nColumns & 0xff;
	nHigh = (nColumns & 0xff00) >> 8;
	vValues.push_back(nLow);
	vValues.push_back(nHigh);
	nLow = nRows & 0xff;
	nHigh = (nRows & 0xff00) >> 8;
	vValues.push_back(nLow);
	vValues.push_back(nHigh);

	vValues.insert(vValues.end(),nValues.begin(), nValues.end());
}

bool XMPTypeCFAPattern::FromVUint8(vector<unsigned char>& vValues)
{
	bool bRetValue = false;
	unsigned int nLow, nHigh;
	nLow = vValues[0];
	nHigh = vValues[1];
	nColumns = nLow + (nHigh << 8);

	nLow = vValues[2];
	nHigh = vValues[3];
	nRows = nLow + (nHigh << 8);

	if(vValues.size() == nColumns * nRows + 4)
	{
		nValues.insert(nValues.begin(), vValues.begin() + 4, vValues.end());
		bRetValue = true;
	}
	return bRetValue;
}

void XMPTypeDeviceSetting::ToVUint8(vector<unsigned char>& vValues)
{
	unsigned char nLow, nHigh;
	nLow = nColumns & 0xff;
	nHigh = (nColumns & 0xff00) >> 8;
	vValues.push_back(nLow);
	vValues.push_back(nHigh);
	nLow = nRows & 0xff;
	nHigh = (nRows & 0xff00) >> 8;
	vValues.push_back(nLow);
	vValues.push_back(nHigh);

	vector<unsigned char> nValues;
	vector<wstring>::const_iterator itStr;
	for (itStr = strValues.begin(); itStr != strValues.end(); itStr ++)
	{
		nValues.clear();
		XMPTypeTranslater::StringToVUint8W(*itStr, nValues, true);
		vValues.insert(vValues.end(),nValues.begin(), nValues.end());
	}	
}
bool XMPTypeDeviceSetting::FromVUint8(vector<unsigned char>& vValues)
{
	bool bRetValue = false;

	strValues.clear();
	if(vValues.size() >= 4)
	{
		unsigned char nLow, nHigh;
		nLow = vValues[0];
		nHigh = vValues[1];
		nColumns = nLow + (nHigh << 8);

		nLow = vValues[2];
		nHigh = vValues[3];
		nRows = nLow + (nHigh << 8);

		wstring strValue;
		wstring::value_type ch;
		vector<unsigned char>::size_type nSize = vValues.size();
		vector<unsigned char>::size_type nIndex;
		for (nIndex = 4; nIndex < nSize; nIndex += 2)
		{
			nLow = vValues[nIndex];
			nHigh = vValues[nIndex + 1];
			ch = nLow + (nHigh << 8);
			if(ch != 0)
				strValue.push_back(ch);
			else
			{
				strValues.push_back(strValue);
				strValue.clear();
			}
		}
		bRetValue = (strValues.size() == nRows * nColumns);
	}
	return bRetValue;
}
XMPTypeGPSCoordinate::XMPTypeGPSCoordinate()
{
	nDegrees = 0;
	nMinutes = 0;
	fSeconds = 0;
	cDirection = L'N';
}
void XMPTypeTime::FromFloat(float fValue)
{
	rScale.nNumerator = 1;
	rScale.nDenominator = 25;
	nValue = fValue * rScale.nDenominator / rScale.nNumerator;
}
void XMPTypeGPSCoordinate::ToFloatVector(vector<float>& fValues) const
{
	fValues.clear();
	fValues.push_back(nDegrees);
	fValues.push_back(nMinutes);
	fValues.push_back(fSeconds);
}
void XMPTypeGPSCoordinate::FromFloatVector(const vector<float>& fValues)
{
	nDegrees = fValues[0];
	nMinutes = fValues[1];
	fSeconds = fValues[2];
}

bool XMPTypeGPSCoordinate::FromString(const wstring& strValue)
{
	bool bRetValue = false;

	wstring strTempValue = strValue;

	//Parse DDD
	int nIndex = strValue.find(L',');
	if(nIndex > 0)
	{
		wstring strDDD = strTempValue.substr(0,nIndex);
		nDegrees = XMPTypeTranslater::FromXMPTypeInt(strDDD);
		strTempValue = strTempValue.substr(nIndex + 1);

		//Parse MM
		nIndex = strTempValue.find_first_of(L",.");
		if(nIndex > 0)
		{
			wstring strMM = strTempValue.substr(0,nIndex);
			nMinutes = XMPTypeTranslater::FromXMPTypeInt(strMM);

			int nKIndex = strTempValue.find_first_of(L"NSEW");
			if(nKIndex >= 0)
			{
				//Get direction.
				cDirection = strTempValue[nKIndex];

				wstring strSS = strTempValue.substr(nIndex + 1, nKIndex - nIndex - 1);
				int nSeconds = XMPTypeTranslater::FromXMPTypeInt(strSS);
				if(strTempValue[nIndex] == L',') //DDD,MM,SSk
				{
					fSeconds = nSeconds;
					bRetValue = true;
				}
				else if(strTempValue[nIndex] == L'.') //DDD,MM.mmk
				{
					unsigned int power;
					int nDenominator = 10;
					for (power = 1; power < strSS.size(); power ++)
					{
						nDenominator *= 10;
					}
					fSeconds = (float)nSeconds * 60 / nDenominator;
					bRetValue = true;
				}
			}
		}		
	}
	return bRetValue;	
}
wstring XMPTypeGPSCoordinate::ToString() const
{
	wostringstream outStringStream;
	int nSecond = fSeconds;
	outStringStream << nDegrees << L"," << nMinutes << L"," << nSecond << cDirection;
	return outStringStream.str();
}
XMPTypeTranslater::XMPTypeTranslater(void)
{
}

XMPTypeTranslater::~XMPTypeTranslater(void)
{
}

wstring XMPTypeTranslater::ToXMPTypeBool(bool realValue)
{
	wstring strValue = L"";
	if(realValue)
		strValue = L"True";
	else
		strValue = L"False";
	return strValue;
}
bool XMPTypeTranslater::FromXMPTypeBool(const wstring& strValue)
{
	bool bRetValue = false;
	if(strValue == L"True")
		bRetValue = true;
	else if(strValue == L"False")
		bRetValue = false;
	else
		throw exception();
	return bRetValue;
}

wstring XMPTypeTranslater::ToXMPTypeInt(int realValue)
{
	wostringstream outStringStream;
	outStringStream << realValue;
	return outStringStream.str();
}
int XMPTypeTranslater::FromXMPTypeInt(const wstring& strValue)
{
	wistringstream inStringStream(strValue);
	int nValue;
	inStringStream >> nValue;
	return nValue;
}
void XMPTypeTranslater::FromXMPTypeSeqInt(const vector<wstring>& strValues, vector<int>& realValues )
{
	realValues.clear();
	vector<wstring>::const_iterator itStr;
	for (itStr = strValues.begin(); itStr != strValues.end(); itStr ++)
	{
		realValues.push_back(FromXMPTypeInt(*itStr));
	}
}
void XMPTypeTranslater::ToXMPTypeSeqInt(const vector<int>& realValues, vector<wstring>& strValues)
{
	strValues.clear();
	vector<int>::const_iterator itInt;
	for (itInt = realValues.begin(); itInt != realValues.end(); itInt ++)
	{
		strValues.push_back(ToXMPTypeInt(*itInt));
	}
}

void XMPTypeTranslater::FromXMPTypeSeqRational(const vector<wstring>& strValues, vector<XMPTypeRational>& realValues )
{
	realValues.clear();
	vector<wstring>::const_iterator itStr;
	for (itStr = strValues.begin(); itStr != strValues.end(); itStr ++)
	{
		realValues.push_back(FromXMPTypeRational(*itStr));
	}
}
void XMPTypeTranslater::ToXMPTypeSeqRational(const vector<XMPTypeRational>& realValues, vector<wstring>& strValues)
{
	strValues.clear();
	vector<XMPTypeRational>::const_iterator itRational;
	for (itRational = realValues.begin(); itRational != realValues.end(); itRational ++)
	{
		strValues.push_back(ToXMPTypeRational(*itRational));
	}
}

wstring XMPTypeTranslater::ToXMPTypeRational(const XMPTypeRational& realValue)
{
	return realValue.ToXMPString();
}
XMPTypeRational XMPTypeTranslater::FromXMPTypeRational(const wstring& strValue)
{
	XMPTypeRational rational;
	rational.FromXMPString(strValue);
	return rational;
}
wstring XMPTypeTranslater::FloatToString(float fValue)
{
	wostringstream outString;
	outString << fValue;
	return outString.str();
}
string XMPTypeTranslater::WideStringToString(const wstring& strWideString)
{
	SPmtString pmtString = EkWToA(strWideString);
	return pmtString.c_str();
}

void XMPTypeTranslater::StringToVUint8(const wstring& strValue, vector<unsigned char>& vValues,bool bWithNull)
{
	string str = WideStringToString(strValue);
	const unsigned char* pStr = (const unsigned char*)(str.c_str());
	while(*pStr != 0)
		vValues.push_back(*(pStr ++));
	if(bWithNull)
		vValues.push_back(0);
}
void XMPTypeTranslater::StringToVUint8W(const wstring& strValue, vector<unsigned char>& vValues,bool bWithNull)
{
	unsigned char nLow, nHigh;
	wstring::value_type nValue;
	unsigned int i = 0;
	for (i = 0; i < strValue.size(); i ++)
	{
		nValue = strValue[i];
		nLow = nValue & 0xff;
		nHigh = (nValue & 0xff00) >> 8;
		vValues.push_back(nLow);
		vValues.push_back(nHigh);
	}
	if(bWithNull)
	{
		vValues.push_back(0);
		vValues.push_back(0);
	}
}
wstring XMPTypeTranslater::VUint8ToString(const vector<unsigned char> vValues,bool bWithNull)
{
	unsigned char* pBytes = new unsigned char[vValues.size() + 1];
	wstring strRetValue;
	unsigned int i;
	for (i = 0; i < vValues.size(); i ++)
	{
		pBytes[i] = vValues[i];
	}
	pBytes[i] = 0;
	SPmtString strValue = (const char*)pBytes;
	wstring strRe = EkAToW(strValue);
	delete[] pBytes;
	strRetValue = strRe;
	return strRetValue;
}
wstring XMPTypeTranslater::VUint8WToString(const vector<unsigned char> vValues,bool bWithNull)
{
	wstring::value_type ch;
	wstring strValue;
	
	unsigned char nLow, nHigh;
	int nSize = vValues.size();
	if(bWithNull)
		nSize -=2;
	int i;
	for (i = 0; i < nSize; i += 2)
	{
		nLow = vValues[i];
		nHigh = vValues[i+ 1];
		ch = (nHigh << 8) + nLow;
		strValue.push_back(ch);
	}
	return strValue;
}
