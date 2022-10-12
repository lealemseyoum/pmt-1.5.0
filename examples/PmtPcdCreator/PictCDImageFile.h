#if !defined(AFX_PICTCDIMAGEFILE_H__3398AE42_9666_4EE0_95DD_69B5A873DABD__INCLUDED_)
#define AFX_PICTCDIMAGEFILE_H__3398AE42_9666_4EE0_95DD_69B5A873DABD__INCLUDED_

/* 
 * The contents of this file are subject to the IBM Common Public
 * License Version 0.5 (the "License"); you may not use this file 
 * except in compliance with the License. You may obtain a copy of 
 * the License at http://oss.software.ibm.com/developerworks/
 * opensource/license-cpl.html
 * 
 * The Original Code is the Picture Metadata Toolkit,
 * released November 13, 2000.
 *
 * The Initial Developer of the Original Code is Eastman 
 * Kodak Company. Portions created by Kodak are 
 * Copyright (C) 1999-2000 Eastman Kodak Company. All 
 * Rights Reserved. 
 * 
 * Creation Date: 
 *
 * Original Author: 
 * Thomas L. Hosie    thomas.hosie@kodak.com
 * 
 * Contributor(s): 
*/

/////////////////////////////////////////////////////////////////////////////////////
// Copyright 2003, Eastman Kodak Company. All rights reserved
//
// File:       PictCDImageFile.h 
//
// Description
//	Interface code for the CPictCDImageFile class. 
//	This class is part of a PMT sample application. 
//  This class demonstrates the 
//	use of PMT to get metadata from EXIF, JIF and TIFF image files. 
//	Along with setting metadata in an EXIF image file. This class provides
//  get and set methods for PMT Picture CD metadata. 
//  This class does NOT provide 
//	methods for all EXIF image file metadata. 
//	The image metadata is stored in classes that represent the Application 
//	Segments and Image File Descriptors (IFD) of a Picture CD (PCD) 
//  image file. 
//
// Change History:
//	Created:	
///////////////////////////////////////////////////////////////////////////////////// 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
using namespace std;

#include "PmtMetadata.h"

// Enums for some of the values for metadata items.
// This is not a complete list

enum ImageSourceEnum
{
	PCD_IS_UNIDENTIFIED = 0,
	PCD_IS_FILM_SCAN = 1,
	PCD_IS_PRINT_SCAN = 2,
	PCD_IS_DIGITAL_CAMERA = 3,
	PCD_IS_STILL_VIDEO = 4,
	PCD_IS_COMP_GRAPHICS = 5,
	PCD_IS_UNKNOWN_PEDIGREE = 6
};

typedef enum ParPrintAreaEnum
{
	PCD_PPA_COMPLETE_IMAGE = 0,
	PCD_PPA_APS_H = 1,
	PCD_PPA_APS_P = 2,
	PCD_PPA_APS_C = 3,
};

typedef enum FlimCategoryEnum
{
	PCD_FC_UNKNOWN = 0,
	PCD_FC_NEG_BW = 1,
	PCD_FC_NEG_COLOR = 2,
	PCD_FC_REV_BW = 3,
	PCD_FC_REV_COLOR = 4
};


typedef enum ImageColorSpaceEnum
{
	PCD_ICS_SRGB = 1,
	PCD_ICS_UNKNOWN = 65535
};

typedef enum SbaImageColorSpaceEnum
{
	PCD_SICS_UNKNOWN = 0,
	PCD_SICS_RIM = 1,
	PCD_SICS_ROM = 2,
	PCD_SICS_RPD = 3,
	PCD_SICS_RLSE = 4
};

typedef enum ImageRotationEnum
{
	PCD_IR_NOT_ROTATED = 0,
	PCD_IR_USER_INPUT = 1,
	PCD_IR_AS_DIRECTED = 2,
	PCD_IR_ALGORITHM_INPUT = 3,
	PCD_IR_UNKNOWN_REASON = 255
};

enum YcbCrPositioningEnum
{
	PCD_YP_UNKNOWN = 0,
	PCD_YP_CENTERED = 1,
	PCD_YP_COSITED = 2
};

typedef enum ResolutionUnitEnum
{
	PCD_RU_UNKNOWN = 0,
	PCD_RU_NOTUSED = 1,
	PCD_RU_INCHES = 2,
	PCD_RU_CENTIMETERS = 3
};

typedef enum SubjectDistanceRangeEnum
{
	PCD_SDR_UNKNOWN = 0,
	PCD_SDR_MACRO = 1,
	PCD_SDR_CLOSEVIEW = 2,
	PCD_SDR_DISTANTVIEW = 3
};

typedef enum FilmSizeEnum
{
	PCD_FS_UNKNOWN = -1,
	PCD_FS_35MM = 0,
	PCD_FS_APS = 1
};

// Helper functions
static void saveAsUnicode(vector<unsigned char> &src, vector<unsigned char> &dest);
static void unicodeVec2AsciiVec(vector<unsigned char> &src, vector<unsigned char> &dest);

/********************************************************************
	Class:	CPcdIFApp1IFD0
	Description: App 1, IFD 0 PCD metadata.
*********************************************************************/
class CPcdIFApp1IFD0
{
public:


	CPcdIFApp1IFD0();
	virtual ~CPcdIFApp1IFD0();

	CPcdIFApp1IFD0(const CPcdIFApp1IFD0& obj);
    const CPcdIFApp1IFD0& operator= (const CPcdIFApp1IFD0& obj);

	// Load the mdRoot object with current member variables values.
	void load(PmtMetadataPtr& mdRoot);

	// Set member variables values with the values in the mdRoot object..
	void save(PmtMetadataPtr& mdRoot);

	// Gets
	const _TCHAR* getMake() {return mMake.c_str();}
	const _TCHAR* getModel() {return mModel.c_str();}
	unsigned short getOrientation() {return mOrientation;}
	float getXresolution() {return mXresolution;}
	float getYresolution() {return mYresolution;}
	unsigned short getResolutionUnit() {return mResolutionUnit;}
	ResolutionUnitEnum getResolutionUnitEnumType();
	const _TCHAR* getSoftware() {return mSoftware.c_str();}
	const _TCHAR* getDateTime() {return mDateTime.c_str();}
	unsigned short getYcbCrPositioning() {return mYcbCrPositioning;}
	YcbCrPositioningEnum getYcbCrPositioningEnumType();
	unsigned long getWidth() {return mWidth;}
	unsigned long getHeight() {return mHeight;}
	
	// Sets
	void setMake(const _TCHAR* make) {mMake = make;}
	void setModel(const _TCHAR* model) {mModel = model;}
	void setOrientation(unsigned short orientation) {mOrientation = orientation;}
	void setXresolution(float xResolution) {mXresolution = xResolution;}
	void setYresolution(float yResolution) {mYresolution = yResolution;}
	void setResolutionUnit(unsigned short resolutionUnit) {mResolutionUnit = resolutionUnit;}
	void setResolutionUnitEnumType(ResolutionUnitEnum type);
	void setSoftware(const _TCHAR* software) {mSoftware = software;}
	void setDateTime(const _TCHAR* dateTime) {mDateTime = dateTime;}
	void setYcbCrPositioning(unsigned short pos) {mYcbCrPositioning = pos;}
	void setYcbCrPositioningEnumType(YcbCrPositioningEnum type);
	void setWidth(unsigned long width) {mWidth = width;}
	void setHeight(unsigned long height) {mHeight = height;}

	// APP1, 0th IFD Tags
	string			mMake;	
	string			mModel;	
	unsigned short	mOrientation;		// 1 - Pixel 0 is at top left conor.
	float			mXresolution;		// Typically 35mm	= 256.0 and APS = 216.0
	float			mYresolution;		// Typically 35mm	= 256.0 and APS = 216.0
	unsigned short	mResolutionUnit;	// 2 = inches. 
	string			mSoftware;	
	string			mDateTime;			// YYYY:MM:DD HH:MM:SS
	unsigned short	mYcbCrPositioning;	// 1 = centered.
	unsigned long	mWidth;				// Used for TIFF file input.
	unsigned long	mHeight;			// Used for TIFF file input.
};


/********************************************************************
	Class:	CPcdIFApp1IFDExif
	Description: APP 1, the EXIF IFD
*********************************************************************/
class CPcdIFApp1IFDExif
{
public:


	CPcdIFApp1IFDExif();
	virtual ~CPcdIFApp1IFDExif();

	CPcdIFApp1IFDExif(const CPcdIFApp1IFDExif& obj);
    const CPcdIFApp1IFDExif& operator= (const CPcdIFApp1IFDExif& obj);

	// Load the mdRoot object with current member variables values.
	void load(PmtMetadataPtr& mdRoot);

	// Set member variables values with the values in the mdRoot object..
	void save(PmtMetadataPtr& mdRoot);

	// Gets
	const vector<unsigned char> getExifVersion() {return mExifVersion;}
	const _TCHAR* getDateTimeOriginal() {return mDateTimeOriginal.c_str();}
	const _TCHAR* getDateTimeDigitized() {return mDateTimeDigitized.c_str();}
	const vector<unsigned char> getComponentsConfig() {return mComponentsConfig;}
	float getBrightness() {return mSceneBrightness;}
	float getSubjectDistance() {return mSubjectDistance;}
	unsigned short getArtificialIlluminant() {return mArtificialIlluminant;}
	unsigned short getFlashFired() {return mFlashFired;}
	float getLensFocalLength() {return mLensFocalLength;}
	const unsigned char* getUserComment();
	const vector<unsigned char> getFlashPixVersion() {return mFlashPixVersion;}
	unsigned short getColorSpace() {return mColorSpace;}
	unsigned long getPixelXDimension() {return mPixelXDimension;}
	unsigned long getPixelYDimension() {return mPixelYDimension;}
	const vector<unsigned char> getSpatialFeqResponse() {return mSpatialFeqResponse;}
	float getFocalPlaneXResolution() {return mFocalPlaneXResolution;}
	float getFocalPlaneYResolution() {return mFocalPlaneYResolution;}
	unsigned short getFocalPlaneUnitResolution() {return mFocalPlaneUnitResolution;}
	const vector<unsigned short> getSubjectArea() {return mSubjectArea;}
	unsigned short getCustomRendered() {return mCustomRendered;}
	unsigned short getWhiteBalance() {return mWhiteBalance;}
	float getDigitalZoomRatio() {return mDigitalZoomRatio;}
	unsigned short getFocalLength35mm() {return mFocalLengthIn35mmFilm;}
	unsigned short getSceneCaptureType() {return mSceneCaptureType;}
	unsigned short getGainControl() {return mGainControl;}
	unsigned short getContrast() {return mContrast;}
	unsigned short getSaturation() {return mSaturation;}
	unsigned short getSharpness() {return mSharpness;}
	unsigned short getSubjectDistanceRange() {return mSubjectDistanceRange;}
	const _TCHAR* getImageUniqueID() {return mImageUniqueID.c_str();}
	
	const _TCHAR* getInterOpIndex() {return mInterOpIndex.c_str();}
	const vector<unsigned char> getInterOpVersion() {return mInterOpVersion;}

	// Sets
	void setExifVersion(const vector<unsigned char> exifVersion) {mExifVersion = exifVersion;}
	void setDateTimeOriginal(const _TCHAR* dateTimeOriginal) {mDateTimeOriginal = dateTimeOriginal;}
	void setDateTimeDigitized(const _TCHAR* dateTimeDigitized) {mDateTimeDigitized = dateTimeDigitized;}
	void setComponentsConfig(const vector<unsigned char> components) {mComponentsConfig = components;}
	void setBrightness(float brightness) {mSceneBrightness = brightness;}
	void setSubjectDistance(float distance) {mSubjectDistance = distance;}
	void setArtificialIlluminant(unsigned short ill) {mArtificialIlluminant = ill;}
	void setFlashFired(unsigned short flashFired) {mFlashFired = flashFired;}
	void setLensFocalLength(float lensFocalLength) {mLensFocalLength = lensFocalLength;}
	void setUserComment(const unsigned char* userComment, int numChars);
	void setFlashPixVersion(const vector<unsigned char> flashPixVersion) {mFlashPixVersion = flashPixVersion;}
	void setColorSpace(unsigned short colorSpace) {mColorSpace = colorSpace;}
	void setPixelXDimension(unsigned long pixelXDimension) {mPixelXDimension = pixelXDimension;}
	void setPixelYDimension(unsigned long pixelYDimension) {mPixelYDimension = pixelYDimension;}
	void setSpatialFeqResponse(const vector<unsigned char> spatialFeqResponse) {mSpatialFeqResponse = spatialFeqResponse;}
	void setFocalPlaneXResolution(float focalPlaneXResolution) {mFocalPlaneXResolution = focalPlaneXResolution;}
	void setFocalPlaneYResolution(float focalPlaneYResolution) {mFocalPlaneYResolution = focalPlaneYResolution;}
	void setFocalPlaneUnitResolution(unsigned short focalPlaneUnitResolution) {mFocalPlaneUnitResolution = focalPlaneUnitResolution;}
	void setSubjectArea(const vector<unsigned short> subjectArea) {mSubjectArea = subjectArea;}
	void setCustomRendered(unsigned short customRendered) {mCustomRendered = customRendered;}
	void setWhiteBalance(unsigned short whiteBalance) {mWhiteBalance = whiteBalance;}
	void setDigitalZoomRatio(float digitalZoomRatio) {mDigitalZoomRatio = digitalZoomRatio;}
	void setFocalLength35mm(unsigned short focalLength) {mFocalLengthIn35mmFilm = focalLength;}
	void setSceneCaptureType(unsigned short type) {mSceneCaptureType = type;}
	void setGainControl(unsigned short gain) {mGainControl = gain;}
	void setContrast(unsigned short contrast) {mContrast = contrast;}
	void setSaturation(unsigned short saturation) {mSaturation = saturation;}
	void setSharpness(unsigned short sharpness) {mSharpness = sharpness;}
	void setSubjectDistanceRange(unsigned short subjectDistanceRange) {mSubjectDistanceRange = subjectDistanceRange;}
	void setImageUniqueID(const _TCHAR* imageUniqueID) {mImageUniqueID = imageUniqueID;}

	void setInterOpIndex(const _TCHAR* interOpIndex) {mInterOpIndex = interOpIndex;}
	void setInterOpVersion(const vector<unsigned char> interOpVersion) {mInterOpVersion = interOpVersion;}
	
	// APP1, Exif IFD Tags
	vector<unsigned char>	mExifVersion;	
	string			mDateTimeOriginal;	
	string			mDateTimeDigitized;	
	vector<unsigned char>	mComponentsConfig;	 
	float			mSceneBrightness;	 
	float			mSubjectDistance;  
	unsigned short	mArtificialIlluminant;	 
	unsigned short	mFlashFired;	 
	float			mLensFocalLength;  
	vector<unsigned short>		mSubjectArea;		 
	unsigned char*	mUserComment;	 
	int				mUserCommentSize;  
	vector<unsigned char>	mFlashPixVersion;	 
	unsigned short	mColorSpace;	 
	unsigned long	mPixelXDimension;	 
	unsigned long	mPixelYDimension;	 
	vector<unsigned char>	mSpatialFeqResponse;	 
	float			mFocalPlaneXResolution;		 
	float			mFocalPlaneYResolution;		 
	unsigned short	mFocalPlaneUnitResolution;	 
	unsigned short	mCustomRendered;	 
	unsigned short	mWhiteBalance;		 
	float			mDigitalZoomRatio;	 
	unsigned short	mFocalLengthIn35mmFilm;	 
	unsigned short	mSceneCaptureType;	 
	unsigned short	mGainControl;	 
	unsigned short	mContrast;	 
	unsigned short	mSaturation;	 
	unsigned short	mSharpness;	 
	unsigned short	mSubjectDistanceRange;  
	string			mImageUniqueID;	 

	// Interoperability
	string			mInterOpIndex;	 
	vector<unsigned char>	mInterOpVersion;	 

};


/********************************************************************
	Class:	CPcdIFApp1IFD1
	Description: APP 1, IFD 1
*********************************************************************/
class CPcdIFApp1IFD1
{
public:
	CPcdIFApp1IFD1();
	virtual ~CPcdIFApp1IFD1();

	CPcdIFApp1IFD1(const CPcdIFApp1IFD1& obj);
    const CPcdIFApp1IFD1& operator= (const CPcdIFApp1IFD1& obj);

	// Load the mdRoot object with current member variables values.
	void load(PmtMetadataPtr& mdRoot);

	// Set member variables values with the values in the mdRoot object..
	void save(PmtMetadataPtr& mdRoot);

	// Gets
	unsigned short getCompression() {return mCompression;}
	unsigned short getOrientation() {return mOrientation;}
	float getXresolution() {return mXresolution;}
	float getYresolution() {return mYresolution;}
	unsigned short getResolutionUnit() {return mResolutionUnit;}
	unsigned long getPEGInterchangeFormat() {return mJPEGInterchangeFormat;}
	unsigned long getJPEGInterchangeFormatLength() {return mJPEGInterchangeFormatLength;}
	unsigned short getYcbCrPositioning() {return mYcbCrPositioning;}

	// Sets
	void setCompression(unsigned short compression) {mCompression = compression;}
	void setOrientation(unsigned short orientation) {mOrientation = orientation;}
	void setXresolution(float xResolution) {mXresolution = xResolution;}
	void setYresolution(float yResolution) {mYresolution = yResolution;}
	void setResolutionUnit(unsigned short resolutionUnit) {mResolutionUnit = resolutionUnit;}
	void setJPEGInterchangeFormat(unsigned long JPEGInterchangeFormat) {mJPEGInterchangeFormat = JPEGInterchangeFormat;}
	void setJPEGInterchangeFormatLength(unsigned long JPEGInterchangeFormatLength) {mJPEGInterchangeFormatLength = JPEGInterchangeFormatLength;}
	void setYcbCrPositioning(unsigned short YcbCrPositioning) {mYcbCrPositioning = YcbCrPositioning;}

	// APP1, 1st IFD Tags
	unsigned short		mCompression;	 
	unsigned short		mOrientation;	 
	float		mXresolution;	 
	float		mYresolution;	 
	unsigned short		mResolutionUnit;	 
	unsigned long		mJPEGInterchangeFormat;			 
	unsigned long		mJPEGInterchangeFormatLength;	 
	unsigned short		mYcbCrPositioning;	 
};


/********************************************************************
	Class:	CPcdIFApp3IFD0
	Description: App 3, IFD 0
*********************************************************************/
class CPcdIFApp3IFD0
{
public:

	CPcdIFApp3IFD0();
	virtual ~CPcdIFApp3IFD0();

	CPcdIFApp3IFD0(const CPcdIFApp3IFD0& obj);
    const CPcdIFApp3IFD0& operator= (const CPcdIFApp3IFD0& obj);

	// Load the mdRoot object with current member variables values.
	void load(PmtMetadataPtr& mdRoot);

	// Set member variables values with the values in the mdRoot object..
	void save(PmtMetadataPtr& mdRoot);

	// Gets
	const _TCHAR* getFilmProductCode() {return mFilmProductCode.c_str();}
	unsigned short getImageSource() {return mImageSource;}
	unsigned short getIntendedPrintArea() {return mIntendedPrintArea;}
	const vector<unsigned char> getCameraOwnerID() {return mCameraOwnerID;}
	const vector<unsigned char> getCameraSerialNum() {return mCameraSerialNum;}
	const _TCHAR* getGroupCaption() {return mGroupCaption.c_str();}
	const vector<unsigned char> getDealerID() {return mDealerID;}
	const vector<unsigned char> getFilmOrderID() {return mFilmOrderID;}
	const vector<unsigned char> getBagNum() {return mBagNum;}
	unsigned short getScanFrameSeqNum() {return mScanFrameSeqNum;}
	unsigned char getFilmCategory() {return mFilmCategory;}
	const _TCHAR* getFilmGenCode() {return mFilmGenCode.c_str();}
	const _TCHAR* getScannerSoftware() {return mScannerSoftware.c_str();}
	unsigned short getFilmSize() {return mFilmSize;}
	const int* getSbaRgbShifts() {return mSbaRgbShifts;}
	unsigned short getSbaInputColorSpace() {return mSbaInputColorSpace;}
	const unsigned short* getSbaInputChannelDepth() {return mSbaInputChannelDepth;}
	const vector<unsigned char> getCompleteExposureRecord() {return mCompleteExposureRecord;}
	const int* getUserAdjSbaRgbShifts() {return mUserAdjSbaRgbShifts;}
	unsigned char getImageRotationStatus() {return mImageRotationStatus;}
	const vector<unsigned char> getRollGuid() {return mRollGuid;}
	const vector<unsigned char> getMetadataVersionNum() {return mMetadataVersionNum;}
	const vector<unsigned short> getEditTagArray() {return mEditTagArray;}
	float getMagnification() {return mMagnification;}
	float getNativePhysicalXResolution() {return mNativePhysicalXResolution;}
	float getNativePhysicalYResolution() {return mNativePhysicalYResolution;}
	unsigned short getNativePhysicalResolutionUnit() {return mNativePhysicalResolutionUnit;}

	// Sets
	void setFilmProductCode(const _TCHAR* filmProductCode) {mFilmProductCode = filmProductCode;}
	void setImageSource(unsigned short imageSource) {mImageSource = imageSource;}
	void setIntendedPrintArea(unsigned short intendedPrintArea) {mIntendedPrintArea = intendedPrintArea;}
	void setCameraOwnerID(const vector<unsigned char> cameraOwnerID) {mCameraOwnerID = cameraOwnerID;}
	void setCameraSerialNum(const vector<unsigned char> cameraSerialNum) {mCameraSerialNum = cameraSerialNum;}
	void setGroupCaption(const _TCHAR* groupCaption) {mGroupCaption = groupCaption;}
	void setDealerID(const vector<unsigned char> dealerID) {mDealerID = dealerID;}
	void setFilmOrderID(const vector<unsigned char> filmOrderID) {mFilmOrderID = filmOrderID;}
	void setBagNum(const vector<unsigned char> bagNum) {mBagNum = bagNum;}
	void setScanFrameSeqNum(unsigned short scanFrameSeqNum) {mScanFrameSeqNum = scanFrameSeqNum;}
	void setFilmCategory(unsigned char filmCategory) {mFilmCategory = filmCategory;}
	void setFilmGenCode(const _TCHAR* filmGenCode) {mFilmGenCode = filmGenCode;}
	void setScannerSoftware(const _TCHAR* scannerSoftware) {mScannerSoftware = scannerSoftware;}
	void setFilmSize(unsigned short filmSize) {mFilmSize = filmSize;}
	void setSbaRgbShifts(int* sbaRgbShifts);
	void setSbaInputColorSpace(unsigned short sbaInputColorSpace) {mSbaInputColorSpace = sbaInputColorSpace;}
	void setSbaInputChannelDepth(unsigned short* sbaInputChannelDepth);
	void setCompleteExposureRecord(const vector<unsigned char> completeExposureRecord) {mCompleteExposureRecord = completeExposureRecord;}
	void setUserAdjSbaRgbShifts(int* userAdjSbaRgbShifts);
	void setImageRotationStatus(unsigned char imageRotationStatus) {mImageRotationStatus = imageRotationStatus;}
	void setRollGuid(const vector<unsigned char> rollGuid) {mRollGuid = rollGuid;}
	void setMetadataVersionNum(const vector<unsigned char> metadataVersionNum) {mMetadataVersionNum = metadataVersionNum;}
	void setEditTagArray(const vector<unsigned short> editTagArray) {mEditTagArray = editTagArray;}
	void setMagnification(float magnification) {mMagnification = magnification;}
	void setNativePhysicalXResolution(float nativePhysicalXResolution) {mNativePhysicalXResolution = nativePhysicalXResolution;}
	void setNativePhysicalYResolution(float nativePhysicalYResolution) {mNativePhysicalYResolution = nativePhysicalYResolution;}
	void setNativePhysicalResolutionUnit(unsigned short nativePhysicalResolutionUnit) {mNativePhysicalResolutionUnit = nativePhysicalResolutionUnit;}


	// APP3, 0th IFD Tags
	string			mFilmProductCode;	 
	unsigned short	mImageSource;	 
									 
	unsigned char	mIntendedPrintArea;	 
	vector<unsigned char>	mCameraOwnerID;	 
	vector<unsigned char>	mCameraSerialNum;	 
	string			mGroupCaption;		 
	vector<unsigned char>	mDealerID;		 
	vector<unsigned char>	mFilmOrderID;	 
	vector<unsigned char>	mBagNum;		 
	unsigned short	mScanFrameSeqNum;	 
	unsigned char	mFilmCategory;		 
	string			mFilmGenCode;		 
	string			mScannerSoftware;	 
	unsigned char	mFilmSize;			 
	int				mSbaRgbShifts[3];	 
	unsigned short	mSbaInputColorSpace;	 
	unsigned short	mSbaInputChannelDepth[3];	 
	vector<unsigned char>	mCompleteExposureRecord;  
	int				mUserAdjSbaRgbShifts[3];	 
	unsigned char	mImageRotationStatus;	 
	vector<unsigned char>	mRollGuid;	 
	vector<unsigned char>	mMetadataVersionNum;	 
	vector<unsigned short>	mEditTagArray;	 
	float			mMagnification;	 
	float			mNativePhysicalXResolution;	 
	float			mNativePhysicalYResolution;	 
	unsigned short	mNativePhysicalResolutionUnit;	 
};



/********************************************************************
	Class:	CPictCDImageFile
	Description: PCD metadata.
*********************************************************************/
class CPictCDImageFile  
{
public:
	CPictCDImageFile();
	virtual ~CPictCDImageFile();

	CPictCDImageFile(const CPictCDImageFile& obj);
    const CPictCDImageFile& operator= (const CPictCDImageFile& obj);
	
	BOOL	load(const _TCHAR* filename, PmtMetadataPtr& mdExisting);
	BOOL	save(const _TCHAR* filename, PmtMetadataPtr& mdExisting);

	const byte*		getImage() {return (mImage);}
	BOOL			setImage(const byte* pImage);
	BOOL			makeThumbnail(void);

	// Gets
	const _TCHAR*	getMake() {return (mPcdIFApp1IFD0.getMake());}
	const _TCHAR*	getModel() {return (mPcdIFApp1IFD0.getModel());}
	unsigned short getOrientation() {return (mPcdIFApp1IFD0.getOrientation());}
	float getXresolution() {return (mPcdIFApp1IFD0.getXresolution());}
	float getYresolution() {return (mPcdIFApp1IFD0.getYresolution());}
	unsigned short getResolutionUnit() {return (mPcdIFApp1IFD0.getResolutionUnit());}
	const _TCHAR* getSoftware() {return (mPcdIFApp1IFD0.getSoftware());}
	const _TCHAR* getDateTime() {return (mPcdIFApp1IFD0.getDateTime());}
	unsigned short getYcbCrPositioning() {return (mPcdIFApp1IFD0.getYcbCrPositioning());}
	unsigned long getWidth() {return (mPcdIFApp1IFD0.getWidth());}
	unsigned long getHeight() {return (mPcdIFApp1IFD0.getHeight());}

	const vector<unsigned char> getExifVersion() {return (mPcdIFApp1IFDExif.getExifVersion());}
	const _TCHAR* getDateTimeOriginal() {return (mPcdIFApp1IFDExif.getDateTimeOriginal());}
	const _TCHAR* getDateTimeDigitized() {return (mPcdIFApp1IFDExif.getDateTimeDigitized());}
	const vector<unsigned char> getComponentsConfig() {return (mPcdIFApp1IFDExif.getComponentsConfig());}
	float getBrightness() {return (mPcdIFApp1IFDExif.getBrightness());}
	float getSubjectDistance() {return (mPcdIFApp1IFDExif.getSubjectDistance());}
	unsigned short getArtificialIlluminant() {return (mPcdIFApp1IFDExif.getArtificialIlluminant());}
	unsigned short getFlashFired() {return (mPcdIFApp1IFDExif.getFlashFired());}
	float getLensFocalLength() {return (mPcdIFApp1IFDExif.getLensFocalLength());}
	const unsigned char* getUserComment(){return (mPcdIFApp1IFDExif.getUserComment());}
	const vector<unsigned char> getFlashPixVersion() {return (mPcdIFApp1IFDExif.getFlashPixVersion());}
	unsigned short getColorSpace() {return (mPcdIFApp1IFDExif.getColorSpace());}
	unsigned long getPixelXDimension() {return (mPcdIFApp1IFDExif.getPixelXDimension());}
	unsigned long getPixelYDimension() {return (mPcdIFApp1IFDExif.getPixelYDimension());}
	const vector<unsigned char> getSpatialFeqResponse() {return (mPcdIFApp1IFDExif.getSpatialFeqResponse());}
	float getFocalPlaneXResolution() {return (mPcdIFApp1IFDExif.getFocalPlaneXResolution());}
	float getFocalPlaneYResolution() {return (mPcdIFApp1IFDExif.getFocalPlaneYResolution());}
	unsigned short getFocalPlaneUnitResolution() {return (mPcdIFApp1IFDExif.getFocalPlaneUnitResolution());}
	const vector<unsigned short> getSubjectArea() {return (mPcdIFApp1IFDExif.getSubjectArea());}
	unsigned short getCustomRendered() {return (mPcdIFApp1IFDExif.getCustomRendered());}
	unsigned short getWhiteBalance() {return (mPcdIFApp1IFDExif.getWhiteBalance());}
	float getDigitalZoomRatio() {return (mPcdIFApp1IFDExif.getDigitalZoomRatio());}
	unsigned short getFocalLength35mm() {return (mPcdIFApp1IFDExif.getFocalLength35mm());}
	unsigned short getSceneCaptureType() {return (mPcdIFApp1IFDExif.getSceneCaptureType());}
	unsigned short getGainControl() {return (mPcdIFApp1IFDExif.getGainControl());}
	unsigned short getContrast() {return (mPcdIFApp1IFDExif.getContrast());}
	unsigned short getSaturation() {return (mPcdIFApp1IFDExif.getSaturation());}
	unsigned short getSharpness() {return (mPcdIFApp1IFDExif.getSharpness());}
	unsigned short getSubjectDistanceRange() {return (mPcdIFApp1IFDExif.getSubjectDistanceRange());}
	const _TCHAR* getImageUniqueID() {return (mPcdIFApp1IFDExif.getImageUniqueID());}

	const _TCHAR* getInterOpIndex() {return mPcdIFApp1IFDExif.getInterOpIndex();}
	const vector<unsigned char> getInterOpVersion() {return mPcdIFApp1IFDExif.getInterOpVersion();}

	unsigned short getTnCompression() {return mPcdIFApp1IFD1.getCompression();}
	unsigned short getTnOrientation() {return mPcdIFApp1IFD1.getOrientation();}
	float getTnXresolution() {return mPcdIFApp1IFD1.getXresolution();}
	float getTnYresolution() {return mPcdIFApp1IFD1.getYresolution();}
	unsigned short getTnResolutionUnit() {return mPcdIFApp1IFD1.getResolutionUnit();}
	unsigned long getTnPEGInterchangeFormat() {return mPcdIFApp1IFD1.getPEGInterchangeFormat();}
	unsigned long getTnJPEGInterchangeFormatLength() {return mPcdIFApp1IFD1.getJPEGInterchangeFormatLength();}
	unsigned short getTnYcbCrPositioning() {return mPcdIFApp1IFD1.getYcbCrPositioning();}

	const _TCHAR* getFilmProductCode() {return mPcdIFApp3IFD0.getFilmProductCode();}
	unsigned short getImageSource() {return mPcdIFApp3IFD0.getImageSource();}
	unsigned short getIntendedPrintArea() {return mPcdIFApp3IFD0.getIntendedPrintArea();}
	const vector<unsigned char> getCameraOwnerID() {return mPcdIFApp3IFD0.getCameraOwnerID();}
	const vector<unsigned char> getCameraSerialNum() {return mPcdIFApp3IFD0.getCameraSerialNum();}
	const _TCHAR* getGroupCaption() {return mPcdIFApp3IFD0.getGroupCaption();}
	const vector<unsigned char> getDealerID() {return mPcdIFApp3IFD0.getDealerID();}
	const vector<unsigned char> getFilmOrderID() {return mPcdIFApp3IFD0.getFilmOrderID();}
	const vector<unsigned char> getBagNum() {return mPcdIFApp3IFD0.getBagNum();}
	unsigned short getScanFrameSeqNum() {return mPcdIFApp3IFD0.getScanFrameSeqNum();}
	unsigned char getFilmCategory() {return mPcdIFApp3IFD0.getFilmCategory();}
	const _TCHAR* getFilmGenCode() {return mPcdIFApp3IFD0.getFilmGenCode();}
	const _TCHAR* getScannerSoftware() {return mPcdIFApp3IFD0.getScannerSoftware();}
	unsigned char getFilmSize() {return mPcdIFApp3IFD0.getFilmSize();}
	const int* getSbaRgbShifts() {return mPcdIFApp3IFD0.getSbaRgbShifts();}
	unsigned short getSbaInputColorSpace() {return mPcdIFApp3IFD0.getSbaInputColorSpace();}
	const unsigned short* getSbaInputChannelDepth() {return mPcdIFApp3IFD0.getSbaInputChannelDepth();}
	const vector<unsigned char> getCompleteExposureRecord() {return mPcdIFApp3IFD0.getCompleteExposureRecord();}
	const int* getUserAdjSbaRgbShifts() {return mPcdIFApp3IFD0.getUserAdjSbaRgbShifts();}
	unsigned char getImageRotationStatus() {return mPcdIFApp3IFD0.getImageRotationStatus();}
	const vector<unsigned char> getRollGuid() {return mPcdIFApp3IFD0.getRollGuid();}
	const vector<unsigned char> getMetadataVersionNum() {return mPcdIFApp3IFD0.getMetadataVersionNum();}
	const vector<unsigned short> getEditTagArray() {return mPcdIFApp3IFD0.getEditTagArray();}
	float getMagnification() {return mPcdIFApp3IFD0.getMagnification();}
	float getNativePhysicalXResolution() {return mPcdIFApp3IFD0.getNativePhysicalXResolution();}
	float getNativePhysicalYResolution() {return mPcdIFApp3IFD0.getNativePhysicalYResolution();}
	unsigned short getNativePhysicalResolutionUnit() {return mPcdIFApp3IFD0.getNativePhysicalResolutionUnit();}


	
	//Sets
	void setMake(const _TCHAR* make) {mPcdIFApp1IFD0.setMake(make);}
	void setModel(const _TCHAR* model) {mPcdIFApp1IFD0.setModel(model);}
	void setOrientation(unsigned short orientation) {mPcdIFApp1IFD0.setOrientation(orientation);}
	void setXresolution(float xResolution) {mPcdIFApp1IFD0.setXresolution(xResolution);}
	void setYresolution(float yResolution) {mPcdIFApp1IFD0.setYresolution(yResolution);}
	void setResolutionUnit(unsigned short resolutionUnit) {mPcdIFApp1IFD0.setResolutionUnit(resolutionUnit);}
	void setResolutionUnitEnumType(ResolutionUnitEnum type) {mPcdIFApp1IFD0.setResolutionUnitEnumType(type);}
	void setSoftware(const _TCHAR* software) {mPcdIFApp1IFD0.setSoftware(software);}
	void setDateTime(const _TCHAR* dateTime) {mPcdIFApp1IFD0.setDateTime(dateTime);}
	void setYcbCrPositioning(unsigned short pos) {mPcdIFApp1IFD0.setYcbCrPositioning(pos);}
	void setYcbCrPositioningEnumType(YcbCrPositioningEnum type) {mPcdIFApp1IFD0.setYcbCrPositioningEnumType(type);}
	void setWidth(unsigned long width) {mPcdIFApp1IFD0.setWidth(width);}
	void setHeight(unsigned long height) {mPcdIFApp1IFD0.setHeight(height);}

	void setExifVersion(const vector<unsigned char> exifVersion) {mPcdIFApp1IFDExif.setExifVersion(exifVersion);}
	void setDateTimeOriginal(const _TCHAR* dateTimeOriginal) {mPcdIFApp1IFDExif.setDateTimeOriginal(dateTimeOriginal);}
	void setDateTimeDigitized(const _TCHAR* dateTimeDigitized) {mPcdIFApp1IFDExif.setDateTimeDigitized(dateTimeDigitized);}
	void setComponentsConfig(const vector<unsigned char> components) {mPcdIFApp1IFDExif.setComponentsConfig(components);}
	void setBrightness(float brightness) {mPcdIFApp1IFDExif.setBrightness(brightness);}
	void setSubjectDistance(float distance) {mPcdIFApp1IFDExif.setSubjectDistance(distance);}
	void setArtificialIlluminant(unsigned short ill) {mPcdIFApp1IFDExif.setArtificialIlluminant(ill);}
	void setFlashFired(unsigned short flashFired) {mPcdIFApp1IFDExif.setFlashFired(flashFired);}
	void setLensFocalLength(float lensFocalLength) {mPcdIFApp1IFDExif.setLensFocalLength(lensFocalLength);}
	void setUserComment(const unsigned char* userComment, int numChars) {mPcdIFApp1IFDExif.setUserComment(userComment, numChars);}
	void setFlashPixVersion(const vector<unsigned char> flashPixVersion) {mPcdIFApp1IFDExif.setFlashPixVersion(flashPixVersion);}
	void setColorSpace(unsigned short colorSpace) {mPcdIFApp1IFDExif.setColorSpace(colorSpace);}
	void setPixelXDimension(unsigned long pixelXDimension) {mPcdIFApp1IFDExif.setPixelXDimension(pixelXDimension);}
	void setPixelYDimension(unsigned long pixelYDimension) {mPcdIFApp1IFDExif.setPixelYDimension(pixelYDimension);}
	void setSpatialFeqResponse(const vector<unsigned char> spatialFeqResponse) {mPcdIFApp1IFDExif.setSpatialFeqResponse(spatialFeqResponse);}
	void setFocalPlaneXResolution(float focalPlaneXResolution) {mPcdIFApp1IFDExif.setFocalPlaneXResolution(focalPlaneXResolution);}
	void setFocalPlaneYResolution(float focalPlaneYResolution) {mPcdIFApp1IFDExif.setFocalPlaneYResolution(focalPlaneYResolution);}
	void setFocalPlaneUnitResolution(unsigned short focalPlaneUnitResolution) {mPcdIFApp1IFDExif.setFocalPlaneUnitResolution(focalPlaneUnitResolution);}
	void setSubjectArea(const vector<unsigned short> subjectArea) {mPcdIFApp1IFDExif.setSubjectArea(subjectArea);}
	void setCustomRendered(unsigned short customRendered) {mPcdIFApp1IFDExif.setCustomRendered(customRendered);}
	void setWhiteBalance(unsigned short whiteBalance) {mPcdIFApp1IFDExif.setWhiteBalance(whiteBalance);}
	void setDigitalZoomRatio(float digitalZoomRatio) {mPcdIFApp1IFDExif.setDigitalZoomRatio(digitalZoomRatio);}
	void setFocalLength35mm(unsigned short focalLength) {mPcdIFApp1IFDExif.setFocalLength35mm(focalLength);}
	void setSceneCaptureType(unsigned short type) {mPcdIFApp1IFDExif.setSceneCaptureType(type);}
	void setGainControl(unsigned short gain) {mPcdIFApp1IFDExif.setGainControl(gain);}
	void setContrast(unsigned short contrast) {mPcdIFApp1IFDExif.setContrast(contrast);}
	void setSaturation(unsigned short saturation) {mPcdIFApp1IFDExif.setSaturation(saturation);}
	void setSharpness(unsigned short sharpness) {mPcdIFApp1IFDExif.setSharpness(sharpness);}
	void setSubjectDistanceRange(unsigned short range) {mPcdIFApp1IFDExif.setSubjectDistanceRange(range);}
	void setImageUniqueID(const _TCHAR* imageUniqueID) {mPcdIFApp1IFDExif.setImageUniqueID(imageUniqueID);}

	void setInterOpIndex(const _TCHAR* interOpIndex) {mPcdIFApp1IFDExif.setInterOpIndex(interOpIndex);}
	void setInterOpVersion(const vector<unsigned char> interOpVersion) {mPcdIFApp1IFDExif.setInterOpVersion(interOpVersion);}

	void setTnCompression(unsigned short compression) {mPcdIFApp1IFD1.setCompression(compression);}
	void setTnOrientation(unsigned short orientation) {mPcdIFApp1IFD1.setOrientation(orientation);}
	void setTnXresolution(float xResolution) {mPcdIFApp1IFD1.setXresolution(xResolution);}
	void setTnYresolution(float yResolution) {mPcdIFApp1IFD1.setYresolution(yResolution);}
	void setTnResolutionUnit(unsigned short resolutionUnit) {mPcdIFApp1IFD1.setResolutionUnit(resolutionUnit);}
	void setTnJPEGInterchangeFormat(unsigned long JPEGInterchangeFormat) {mPcdIFApp1IFD1.setJPEGInterchangeFormat(JPEGInterchangeFormat);}
	void setTnJPEGInterchangeFormatLength(unsigned long JPEGInterchangeFormatLength) {mPcdIFApp1IFD1.setJPEGInterchangeFormatLength(JPEGInterchangeFormatLength);}
	void setTnYcbCrPositioning(unsigned short YcbCrPositioning) {mPcdIFApp1IFD1.setYcbCrPositioning(YcbCrPositioning);}

	// App3IFD0
	void setFilmProductCode(const _TCHAR* filmProductCode) {mPcdIFApp3IFD0.setFilmProductCode(filmProductCode);}
	void setImageSource(unsigned short imageSource) {mPcdIFApp3IFD0.setImageSource(imageSource);}
	void setIntendedPrintArea(unsigned short intendedPrintArea) {mPcdIFApp3IFD0.setIntendedPrintArea(intendedPrintArea);}
	void setCameraOwnerID(const vector<unsigned char> cameraOwnerID) {mPcdIFApp3IFD0.setCameraOwnerID(cameraOwnerID);}
	void setCameraSerialNum(const vector<unsigned char> cameraSerialNum) {mPcdIFApp3IFD0.setCameraSerialNum(cameraSerialNum);}
	void setGroupCaption(const _TCHAR* groupCaption) {mPcdIFApp3IFD0.setGroupCaption(groupCaption);}
	void setDealerID(const vector<unsigned char> dealerID) {mPcdIFApp3IFD0.setDealerID(dealerID);}
	void setFilmOrderID(const vector<unsigned char> filmOrderID) {mPcdIFApp3IFD0.setFilmOrderID(filmOrderID);}
	void setBagNum(const vector<unsigned char> bagNum) {mPcdIFApp3IFD0.setBagNum(bagNum);}
	void setScanFrameSeqNum(unsigned short scanFrameSeqNum) {mPcdIFApp3IFD0.setScanFrameSeqNum(scanFrameSeqNum);}
	void setFilmCategory(unsigned char filmCategory) {mPcdIFApp3IFD0.setFilmCategory(filmCategory);}
	void setFilmGenCode(const _TCHAR* filmGenCode) {mPcdIFApp3IFD0.setFilmGenCode(filmGenCode);}
	void setScannerSoftware(const _TCHAR* scannerSoftware) {mPcdIFApp3IFD0.setScannerSoftware(scannerSoftware);}
	void setFilmSize(unsigned char filmSize) {mPcdIFApp3IFD0.setFilmSize(filmSize);}
	void setSbaRgbShifts(int* sbaRgbShifts) {mPcdIFApp3IFD0.setSbaRgbShifts(sbaRgbShifts);}
	void setSbaInputColorSpace(unsigned short sbaInputColorSpace) {mPcdIFApp3IFD0.setSbaInputColorSpace(sbaInputColorSpace);}
	void setSbaInputChannelDepth(unsigned short* sbaInputChannelDepth) {mPcdIFApp3IFD0.setSbaInputChannelDepth(sbaInputChannelDepth);}
	void setCompleteExposureRecord(const vector<unsigned char> completeExposureRecord) 
		{mPcdIFApp3IFD0.setCompleteExposureRecord(completeExposureRecord);}
	void setUserAdjSbaRgbShifts(int* userAdjSbaRgbShifts) {mPcdIFApp3IFD0.setUserAdjSbaRgbShifts(userAdjSbaRgbShifts);}
	void setImageRotationStatus(unsigned char imageRotationStatus) {mPcdIFApp3IFD0.setImageRotationStatus(imageRotationStatus);}
	void setRollGuid(const vector<unsigned char> rollGuid) {mPcdIFApp3IFD0.setRollGuid(rollGuid);}
	void setMetadataVersionNum(const vector<unsigned char> metadataVersionNum) {mPcdIFApp3IFD0.setMetadataVersionNum(metadataVersionNum);}
	void setEditTagArray(const vector<unsigned short> editTagArray) {mPcdIFApp3IFD0.setEditTagArray(editTagArray);}
	void setMagnification(float magnification) {mPcdIFApp3IFD0.setMagnification(magnification);}
	void setNativePhysicalXResolution(float nativePhysicalXResolution) {mPcdIFApp3IFD0.setNativePhysicalXResolution(nativePhysicalXResolution);}
	void setNativePhysicalYResolution(float nativePhysicalYResolution) {mPcdIFApp3IFD0.setNativePhysicalYResolution(nativePhysicalYResolution);}
	void setNativePhysicalResolutionUnit(unsigned short nativePhysicalResolutionUnit) {mPcdIFApp3IFD0.setNativePhysicalResolutionUnit(nativePhysicalResolutionUnit);}


private:
	string		mFilename;

	// Pixel interleaved
	byte*		mImage; 

	// Pixel interleaved
	byte*		mThumbnail; 

	CPcdIFApp1IFD0		mPcdIFApp1IFD0;
	CPcdIFApp1IFDExif	mPcdIFApp1IFDExif;
	CPcdIFApp1IFD1		mPcdIFApp1IFD1;
	CPcdIFApp3IFD0		mPcdIFApp3IFD0;
};

#endif // !defined(AFX_PICTCDIMAGEFILE_H__3398AE42_9666_4EE0_95DD_69B5A873DABD__INCLUDED_)
