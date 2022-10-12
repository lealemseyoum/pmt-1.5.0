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
#ifndef XMP_TYPE_TRANSLATER_H
#define XMP_TYPE_TRANSLATER_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#include "EkCompiler.h"
#include "PmtSAX2.h"
#if defined(_UNICODE)

#define XMP_EKSTRING2STR(str) str
#define XMP_EKSTRING2WIDE(str) str

#else
#define XMP_EKSTRING2STR(str) EkWToA( str )
#define XMP_EKSTRING2WIDE(str) EkAToW( str )
#endif

#define XMP_PMTSTRING2WIDE(str) EkAToW( str )
//! Convert a XMLCh string to wstring value
wstring XMLChToWideString(const XMLCh* const xmlStr);


//! The XMPTypeRational offer some methods to convert XMP rational and PMT ration value
class XMPTypeRational
{
public:
	//! Denominator
	int nDenominator;

	//! Numerator
	int nNumerator;

	//! The default constructor
	XMPTypeRational(){}

	//! Constructor, with the numerator and denominator of the rational
	XMPTypeRational(int nNumerator, int nDenominator)
	{
		this->nDenominator = nDenominator;
		this->nNumerator = nNumerator;
	}

	//! Copy Constructor
	XMPTypeRational(const XMPTypeRational& rational)
	{
		nDenominator = rational.nDenominator;
		nNumerator = rational.nNumerator;
	}

	//! Constructor, initialize with a float value
	XMPTypeRational(float fValue);

	//! Assignment Constructor
	XMPTypeRational& operator = (const XMPTypeRational& rational) 
	{
		nDenominator = rational.nDenominator;
		nNumerator = rational.nNumerator;
		return *this;
	}

	//! This method returns the float value
	float ToFloat() const
	{
		if(nDenominator == 0)
			throw exception();
		return ((float)nNumerator) / nDenominator; 
	}

	//! This method returns the string of the float value, as format "3.14"
	wstring ToFloatString() const;

	//! This method convert the rational value to a bytes vector, the bytes are stored as little endian
	void ToVUint8(vector<unsigned char>& vValues) const;

	//! This method initialize the object with a bytes vector(4 bytes), the bytes are supposed  to stored as little endian
	void FromVUint8(vector<unsigned char>::const_iterator itBeg);

	//! This method return the PMT string value of the rational value, as format "13/65"
	wstring ToXMPString() const;

	//! This method initialize the object with the PMT string, if the string is a valid PMT rational string, it will return true, otherwise false
	bool FromXMPString(const wstring& strValue);
};

//! XMPTypeDate offers some methods to maintenance the date value of PMT string and XMP string.
class XMPTypeDate
{
public:
	//! The default constructor
	XMPTypeDate();

	/*! Initialize with XMP string.
	A date-time value which is represented using a subset of ISO RFC 8601 formatting, as described in
	http://www.w3.org/TR/Note-datetime.html. The following formats are supported:
	YYYY
	YYYY-MM
	YYYY-MM-DD
	YYYY-MM-DDThh:mmTZD
	YYYY-MM-DDThh:mm:ssTZD
	YYYY-MM-DDThh:mm:ss.sTZD
	YYYY = four-digit year
	MM = two-digit month (01=January)
	DD = two-digit day of month (01 through 31)
	hh = two digits of hour (00 through 23)
	mm = two digits of minute (00 through 59)
	ss = two digits of second (00 through 59)
	s = one or more digits representing a decimal fraction of a second
	TZD = time zone designator (Z or +hh:mm or -hh:mm)
	The time zone designator is optional in XMP. When not present, the time zone is unknown, and software
	should not assume anything about the missing time zone.
	It is recommended, when working with local times, that you use a time zone designator of +hh:mm or
	-hh:mm instead of Z, to aid human readability. For example, if you know a file was saved at noon on
	October 23 a timestamp of 2004-10-23T12:00:00-06:00 is more understandable than
	2004-10-23T18:00:00Z.
	*/
	bool FromXMPString(const wstring &strValue);

	/*! Initialize with PMT string
	The valid PMT date format is "YYYY:MM:DD HH:MM:SS"
	*/
	bool FromPMTString(const wstring &strValue);

	//! This method returns the XMP format string
	wstring ToXMPTypeString() const;

	//! This method returns the PMT format string
	wstring ToPMTTypeString() const;

	//! This method returns the hours
	int& Hours() { return nHour;}

	//! This method returns the minutes
	int& Minutes() { return nMinute; }

	//! This method returns the seconds
	int& Seconds() { return nSecond; }
	
private:
	int nYear;
	int nMonth;
	int nDay;
	int nHour;
	int nMinute;
	int nSecond;
	int nFraction;
	int nPrecision;
	bool bAbsolute;
	bool bAhead;
	int nOffsetHours;
	int nOffsetMinutes;

	bool ParseTZD(const wstring& TZD, bool& bAbsolute, bool& bAhead, int& nHours, int& nMinutes );

	//! Not implemented copy constructor
	XMPTypeDate(const XMPTypeDate& rhs);

	//! Not implemented assignment constructor
	XMPTypeDate& operator=(const XMPTypeDate& rhs);
};

//! XMPTypeOECF maintenances the OECF/SFR value
/*
A structure describing the OECF/SFR.
 The field namespace URI is http://ns.adobe.com/exif/1.0/
 The preferred field namespace prefix is exif
*/
class XMPTypeOECF
{
public:
	//! Columns of the OECF/SFR
	unsigned int nColumns;

	//! Rows of the OECF/SFR
	unsigned int nRows;

	//! Column names
	vector<wstring> strColumnsNames; // nCloumns.

	//! OECF/SFR values
	vector<XMPTypeRational> vRationals; // nColumns * nRows

	//! This method append the object itself to the byte vector as little endian
	void ToVUint8(vector<unsigned char>& vValues) const;

	//! This method initialize with a byte vector
	bool FromVUint8(vector<unsigned char>& vValues);
};


//! The XMPTypeFlash maintenance the status of flash when the image was shot
class XMPTypeFlash
{
public:
	//! True if flash fired
	bool bFired;

	//! True if flash function is not present.
	bool bFunction;

	/*! The flash mode. One of:
		0 = unknown
		1 = compulsory flash firing
		2 = compulsory flash suppression
		3 = auto mode
	*/
	int nMode;

	//! True if red-eye reduction is supported
	bool bRedEyeMode;

	/*! Whether strobe return is supported and if supported, detected.		One of:
		0 = no strobe return detection
		2 = strobe return light not detected
		3 = strobe return light detected
	*/
	int nReturn;

	/*! This method return a PMT byte value of the flash information

		Bit 0 indicates the flash firing status, bits 1 and 2 indicate the flash return status
		bits 3 and 4 indicate the flash mode, bit 5 indicates whether the flash function is present
		and bit 6 indicates "red eye" mode. 
	*/
	unsigned char ToByte() const;

	//! Initialize from a PMT byte value
	void FromByte(unsigned char chValue);
};

//! The XMPTypeCFAPattern maintenance the CFAPattern value
class XMPTypeCFAPattern
{
public:
	//! Columns of the CFAPattern
	unsigned int nColumns;

	//! Rows of the CFAPattern
	unsigned int nRows;

	//! CFAPattern values
	vector<unsigned char> nValues;

	//! This methods append the object itself to a byte vector with little endian
	void ToVUint8(vector<unsigned char>& vValues);

	//! This method initializes with a byte vector with little endian
	bool FromVUint8(vector<unsigned char>& vValues);
};

//! The XMPTypeDeviceSetting maintenance the device settings
class XMPTypeDeviceSetting
{
public:
	//! Columns of the vector.
	unsigned int nColumns;

	//! Rows of the vector
	unsigned int nRows;

	//! Device settings
	vector<wstring> strValues;

	//! This method appends the object itself to a byte vector with little endian
	void ToVUint8(vector<unsigned char>& vValues);

	//! This method initializes with a byte vector with little endian
	bool FromVUint8(vector<unsigned char>& vValues);
};

//! The XMPTypeTime maintenance the time value
/*
A representation of a time value in seconds. This is similar to After Effect¡¯s TDB, or QuickTime¡¯s
representation of time. They each have a value, and the scale of the value. For example, if the scale is the
rational 1/25 (PAL 25fps), and the value is 50, the time is 2 seconds.
*/
class XMPTypeTime
{
public:
	/*! The scale for the time value.
		For NTSC, use 1001/30000, or the less accurate 100/2997.
		For PAL, use 1/25.
	*/
	XMPTypeRational rScale;

	//! The time value in the specified scale
	int nValue;

	//! This method returns the float value of seconds
	float ToFloat() const
	{
		if(rScale.nDenominator == 0)
			throw exception();
		return (float)nValue * rScale.nNumerator / rScale.nDenominator;
	}

	//! This method initialize with a float value, the scale is initialized as PAL
	void FromFloat(float fValue);
};
/*
A Text value in the form ¡°DDD,MM,SSk¡± or ¡°DDD,MM.mmk¡±, where:
DDD is a number of degrees
MM is a number of minutes
SS is a number of seconds
mm is a fraction of minutes
k is a single character N, S, E, or W indicating a direction (north, south, east, west)
*/

//! The XMPTypeGPSCoordinate maintenance the gps coordinate value
class XMPTypeGPSCoordinate
{
public:
	//! Number of degrees
	int nDegrees;

	//! Number of minutes
	int nMinutes;

	//! Number of seconds
	float fSeconds;

	//! A single character N, S, E, or W indicating a direction (north, south, east, west)
	wchar_t cDirection;

	//! The default constructor
	XMPTypeGPSCoordinate();

	//! This method converts the values to a float vector as PMT metadata needed 
	void ToFloatVector(vector<float>& fValues) const;

	//! This method initialize with a float vector from PMT metadata value
	void FromFloatVector(const vector<float>& fValues);

	/*! Initialize from a XMP string value

		A Text value in the form ¡°DDD,MM,SSk¡± or ¡°DDD,MM.mmk¡±, where:
		DDD is a number of degrees
		MM is a number of minutes
		SS is a number of seconds
		mm is a fraction of minutes
		k is a single character N, S, E, or W indicating a direction (north, south, east, west)
		Leading zeros are not necessary for the for DDD, MM, and SS values. The DDD,MM.mmk form should be used
		when any of the native EXIF component rational values has a denominator other than 1. There can be any
		number of fractional digits.
	*/
	bool FromString(const wstring& strValue);

	//! This method returns the string value as XMP needed
	wstring ToString() const;
};

// The XMPTypeTranslater offers some methods to maintenance types of PMT and XMP metadataums
class XMPTypeTranslater
{
public:
	//! For bool value, only "True" and "False" are valid.
	//! This method convert the boolean value to a XMP string, "True" for true and "False" for false
	static wstring ToXMPTypeBool(bool realValue);

	//! This method convert a XMP string to boolean value, true for "True" and false for "False", others will lead to exception
	static bool FromXMPTypeBool(const wstring& strValue);

	//! This method converts a int value to a string
	static wstring ToXMPTypeInt(int realValue);

	//! this method parses the int value from a string
	static int FromXMPTypeInt(const wstring& strValue); 

	//! This method converts a int vector to a string vector
	static void ToXMPTypeSeqInt(const vector<int>& realValues, vector<wstring>& strValues);

	//! this method parses a int vector from a string vector
	static void FromXMPTypeSeqInt(const vector<wstring>& strValues, vector<int>& realValues );
	
	//! This method converts a rational value to a string as PMT needed
	static wstring ToXMPTypeRational(const XMPTypeRational& realValue);

	//! This method parse a rational value from a string as PMT defined
	static XMPTypeRational FromXMPTypeRational(const wstring& strValue); 

	//! This method converts a rational vector to a string vector
	static void ToXMPTypeSeqRational(const vector<XMPTypeRational>& realValues, vector<wstring>& strValues);

	//! This method parses a rational vector from a string vector
	static void FromXMPTypeSeqRational(const vector<wstring>& strValues, vector<XMPTypeRational>& realValues );
	
	//! This method converts a float value to a string
	static wstring FloatToString(float fValue);

	//! This method converts a wide string to a string
	static string WideStringToString(const wstring& strWideString);

	//! This method convert a string to a byte vector
	static void StringToVUint8(const wstring& strValue, vector<unsigned char>& vValues, bool bWithNull = true);

	//! This method convert a string to a byte vector, the string is considered as unicode
	static void StringToVUint8W(const wstring& strValue, vector<unsigned char>& vValues, bool bWithNull = true);

	//! This method parses a string from a byte vector
	static wstring VUint8ToString(const vector<unsigned char> vValues, bool bWithNull = true);

	//! This method parses a unicode string from a byte vector
	static wstring VUint8WToString(const vector<unsigned char> vValues, bool bWithNull = true);
	
protected:
	XMPTypeTranslater(void);
	~XMPTypeTranslater(void);
};


#endif