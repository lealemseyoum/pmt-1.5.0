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

/* 
// Description
//	Interface code for the CPictCDImageFile class. 
//	This class is part of a PMT sample application. This class demonstrates the 
//	use of PMT to get metadata from EXIF, JIF and TIFF image files. 
//	Along with setting metadata in an EXIF image file. This class provides get 
//	and set methods for PMT Picture CD metadata. This class does NOT provide 
//	methods for all EXIF image file metadata. 
//	The image metadata is stored in classes that represent the Application 
//	Segments and Image File Descriptors (IFD) of a Picture CD (PCD) image file. 
//
//
//	NOTE: This program uses the PmtImageAccessor. Therefore, PMT must be built
//	with the PMT_IMAGE_ACCESSOR_REQUIRED preprocessor definition for this
//	program.
*/

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PictCDImageFile.h"


#undef FAR // avoid warning



// The core PMT includes ...

#include "PmtMetadataT.h"

#include "PmtDefinitionInterpreter.h"



// Each accessor that we might need has to be included so that it gets 

// set up correctly in the factory.

#include "PmtAllAccessors.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Dimension of the thumbnail image
#define THUMBWIDTH 160
#define THUMBHEIGHT 120
#define THUMBSIZE 576000


#define LOCAL_PMT_CAST_TYPE(MD, TYPE) ((PmtMetadataT<TYPE > *)((MD).ptr()))


// ******************************************************************
//	Class:	CPcdIFApp1IFD0 Methods
// ******************************************************************

/* -------------------------------------------------------------------
	Method:	Constructor
	Description:
------------------------------------------------------------------- */
CPcdIFApp1IFD0::CPcdIFApp1IFD0()
{
	mMake = EK_L("");
	mModel = EK_L("");	
	mOrientation = 0; // 1 - Pixel 0 is at top left conor.	
	mXresolution = 0.0f; // Typically 35mm	= 256.0 and APS = 216.0
	mYresolution = 0.0f; // Typically 35mm	= 256.0 and APS = 216.0
	mResolutionUnit = (unsigned short) PCD_RU_INCHES; 	
	mSoftware = EK_L("");	
	mDateTime = EK_L("");	
	mYcbCrPositioning = (unsigned short) PCD_YP_CENTERED;
	mWidth = 0;
	mHeight = 0;
}

/* -------------------------------------------------------------------
	Method:	Destructor
	Description:
------------------------------------------------------------------- */
CPcdIFApp1IFD0::~CPcdIFApp1IFD0()
{
}

/* -------------------------------------------------------------------
	Method:	Copy
	Description:
------------------------------------------------------------------- */
CPcdIFApp1IFD0::CPcdIFApp1IFD0(const CPcdIFApp1IFD0& obj)
{
	*this = obj;
}

/* -------------------------------------------------------------------
	Method:	operator=
	Description:
------------------------------------------------------------------- */
const
CPcdIFApp1IFD0& CPcdIFApp1IFD0::operator= (const CPcdIFApp1IFD0& obj)
{
    if (this == &obj)
    {
        return *this;
    }

	mMake = obj.mMake;
	mModel = obj.mModel;
	mOrientation = obj.mOrientation;
	mXresolution = obj.mXresolution;
	mYresolution = obj.mYresolution;
	mResolutionUnit = obj.mResolutionUnit;
	mSoftware = obj.mSoftware;
	mDateTime = obj.mDateTime;
	mYcbCrPositioning = obj.mYcbCrPositioning;
	mWidth = obj.mWidth;
	mHeight = obj.mHeight;

    return *this;
}

/* -------------------------------------------------------------------
	Method:	getResolutionUnitEnumType
	Description:
------------------------------------------------------------------- */
ResolutionUnitEnum
CPcdIFApp1IFD0::getResolutionUnitEnumType() 
{
	if (PCD_RU_CENTIMETERS == mResolutionUnit) 
	{
		return PCD_RU_CENTIMETERS;
	}
	else
	{
		// Default is inches.
		return PCD_RU_INCHES;
	}
}

/* -------------------------------------------------------------------
	Method:	setResolutionUnitEnumType
	Description:
------------------------------------------------------------------- */
void
CPcdIFApp1IFD0::setResolutionUnitEnumType(ResolutionUnitEnum type) 
{
	if (PCD_RU_CENTIMETERS == type) 
	{
		mResolutionUnit = (unsigned short) PCD_RU_CENTIMETERS;
	}
	else
	{
		// Default is inches.
		mResolutionUnit = (unsigned short) PCD_RU_INCHES;
	}
}

/* -------------------------------------------------------------------
	Method:	getYcbCrPositioningType
	Description:
------------------------------------------------------------------- */
YcbCrPositioningEnum 
CPcdIFApp1IFD0::getYcbCrPositioningEnumType() 
{
	if (PCD_YP_COSITED == mYcbCrPositioning) 
	{
		return PCD_YP_COSITED;
	}
	else
	{
		// Default is CENTERED.
		return PCD_YP_CENTERED;
	}
}
/* -------------------------------------------------------------------
	Method:	setYcbCrPositioningType
	Description:
------------------------------------------------------------------- */
void 
CPcdIFApp1IFD0::setYcbCrPositioningEnumType(YcbCrPositioningEnum type) 
{
	if (PCD_YP_COSITED == type) 
	{
		mYcbCrPositioning = (unsigned short) PCD_YP_COSITED;
	}
	else
	{
		// Default is CENTERED.
		mYcbCrPositioning = (unsigned short) PCD_YP_CENTERED;
	}
}

/* -------------------------------------------------------------------
	Method:	load(PmtMetadataPtr& mdRoot)
	Description: Set member variables from values in passed in metadata object.
------------------------------------------------------------------- */
void
CPcdIFApp1IFD0::load(PmtMetadataPtr& mdRoot)
{
	PmtMetadataPtr md;
	
	md = mdRoot->getMetadatum("CaptureDevice.Make");
	if (md)
	{
		mMake = md->getValueStr();
	}

	md = mdRoot->getMetadatum("CaptureDevice.Model");
	if (md)
	{
		mModel = md->getValueStr();
	}

	md = mdRoot->getMetadatum("ImageContainer.Orientation");
	if (md)
	{
		mOrientation = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());

	}

	md = mdRoot->getMetadatum("ImageContainer.DisplayXResolution");
	if (md)
	{
		mXresolution = (LOCAL_PMT_CAST_TYPE(md, float)->value());
	}

	md = mdRoot->getMetadatum("ImageContainer.DisplayYResolution");
	if (md)
	{
		mYresolution = (LOCAL_PMT_CAST_TYPE(md, float)->value());
	}

	md = mdRoot->getMetadatum("ImageContainer.DisplayResolutionUnit");
	if (md)
	{
		mResolutionUnit = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("CaptureDevice.Software");
	if (md)
	{
		mSoftware = md->getValueStr();
	}

	md = mdRoot->getMetadatum("DigitalProcess.ImageFileChangeDateTime");
	if (md)
	{
		mDateTime = md->getValueStr();
	}

	md = mdRoot->getMetadatum("ImageContainer.YCbCrPositioning");
	if (md)
	{
		mYcbCrPositioning = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("ImageContainer.Width");
	if (md)
	{
		mWidth = (LOCAL_PMT_CAST_TYPE(md, unsigned long)->value());
	}

	md = mdRoot->getMetadatum("ImageContainer.Height");
	if (md)
	{
		mHeight = (LOCAL_PMT_CAST_TYPE(md, unsigned long)->value());
	}
}

/* -------------------------------------------------------------------
	Method:	save(PmtMetadataPtr& mdRoot)
	Description: Save member variables values into passed in metadata object.
		All metadata items are mandatory. So, have PMT create them if they do
		not exist in mdRoot.
------------------------------------------------------------------- */
void
CPcdIFApp1IFD0::save(PmtMetadataPtr& mdRoot)
{
	PmtMetadataPtr md;
	
	md = mdRoot->getMetadatum("CaptureDevice.Make", true);
	if (md)
	{
		md->setValueStr(mMake);
	}

	md = mdRoot->getMetadatum("CaptureDevice.Model", true);
	if (md)
	{
		md->setValueStr(mModel);
	}

	md = mdRoot->getMetadatum("ImageContainer.Orientation", true);
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mOrientation;
	}

	md = mdRoot->getMetadatum("ImageContainer.DisplayXResolution", true);
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, float)->value() = mXresolution;

	}

	md = mdRoot->getMetadatum("ImageContainer.DisplayYResolution", true);
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, float)->value() = mYresolution;
	}

	md = mdRoot->getMetadatum("ImageContainer.DisplayResolutionUnit", true);
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mResolutionUnit;
	}

	md = mdRoot->getMetadatum("CaptureDevice.Software", true);
	if (md)
	{
		md->setValueStr(mSoftware);
	}

	md = mdRoot->getMetadatum("DigitalProcess.ImageFileChangeDateTime", true);
	if (md)
	{
		md->setValueStr(mDateTime);
	}

	md = mdRoot->getMetadatum("ImageContainer.YCbCrPositioning", true);
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mYcbCrPositioning;
	}
}


// ******************************************************************
//	Class:	CPcdIFApp1IFDExif Methods
// ******************************************************************

/* -------------------------------------------------------------------
	Method:	Constructor
	Description:
------------------------------------------------------------------- */
CPcdIFApp1IFDExif::CPcdIFApp1IFDExif()
{
	mExifVersion.clear();
	mDateTimeOriginal = EK_L("");
	mDateTimeDigitized = EK_L("");
	mComponentsConfig.clear();
	mSceneBrightness = 0.0f;
	mSubjectDistance = 0.0f;
	mArtificialIlluminant = 0;
	mFlashFired = 0;
	mLensFocalLength = 0.0f;
	mUserComment = NULL;
	mUserCommentSize = 0;
	mFlashPixVersion.clear();
	mColorSpace = (unsigned short) PCD_ICS_SRGB;
	mPixelXDimension = 0;
	mPixelYDimension = 0;
	mSpatialFeqResponse.clear();
	mFocalPlaneXResolution = 0.0f;
	mFocalPlaneYResolution = 0.0f;
	mFocalPlaneUnitResolution = (unsigned short) PCD_RU_UNKNOWN;
	mSubjectArea.clear();
	mCustomRendered = 0;
	mWhiteBalance = 0;
	mDigitalZoomRatio = 0.0f;
	mFocalLengthIn35mmFilm = 0;
	mSceneCaptureType = 0;
	mGainControl = 0;
	mContrast = 0;
	mSaturation = 0;
	mSharpness = 0;
	mSubjectDistanceRange = 0;
	mImageUniqueID = EK_L("");

	mInterOpIndex = EK_L("");
	mInterOpVersion.clear();
}

/* -------------------------------------------------------------------
	Method:	Destructor
	Description:
------------------------------------------------------------------- */
CPcdIFApp1IFDExif::~CPcdIFApp1IFDExif()
{
	if (mUserComment)
	{
		delete mUserComment;
		mUserComment = NULL;
		mUserCommentSize = 0;
	}
}

/* -------------------------------------------------------------------
	Method:	Copy
	Description:
------------------------------------------------------------------- */
CPcdIFApp1IFDExif::CPcdIFApp1IFDExif(const CPcdIFApp1IFDExif& obj)
{
	*this = obj;
}

/* -------------------------------------------------------------------
	Method:	operator=
	Description:
------------------------------------------------------------------- */
const
CPcdIFApp1IFDExif& CPcdIFApp1IFDExif::operator= (const CPcdIFApp1IFDExif& obj)
{
    if (this == &obj)
    {
        return *this;
    }
	mExifVersion = obj.mExifVersion;
	mDateTimeOriginal = obj.mDateTimeOriginal;
	mDateTimeDigitized = obj.mDateTimeDigitized;
	mComponentsConfig = obj.mComponentsConfig;
	mSceneBrightness = obj.mSceneBrightness;
	mSubjectDistance = obj.mSubjectDistance;
	mArtificialIlluminant = obj.mArtificialIlluminant;
	mFlashFired = obj.mFlashFired;
	mLensFocalLength = obj.mLensFocalLength;
	mUserComment = NULL;
	mUserCommentSize = 0;
	if (obj.mUserComment && (0 < obj.mUserCommentSize) )
	{
		mUserComment = new unsigned char[obj.mUserCommentSize];
		if (mUserComment)
		{
			memcpy(mUserComment,obj.mUserComment,obj.mUserCommentSize);
			mUserCommentSize = obj.mUserCommentSize;
		}
	}
	mFlashPixVersion = obj.mFlashPixVersion;
	mColorSpace = obj.mColorSpace;
	mPixelXDimension = obj.mPixelXDimension;
	mPixelYDimension = obj.mPixelYDimension;
	mSpatialFeqResponse = obj.mSpatialFeqResponse;
	mFocalPlaneXResolution = obj.mFocalPlaneXResolution;
	mFocalPlaneYResolution = obj.mFocalPlaneYResolution;
	mFocalPlaneUnitResolution = obj.mFocalPlaneUnitResolution;
	mSubjectArea = obj.mSubjectArea;
	mCustomRendered = obj.mCustomRendered;
	mWhiteBalance = obj.mWhiteBalance;
	mDigitalZoomRatio = obj.mDigitalZoomRatio;
	mFocalLengthIn35mmFilm = obj.mFocalLengthIn35mmFilm;
	mSceneCaptureType = obj.mSceneCaptureType;
	mGainControl = obj.mGainControl;
	mContrast = obj.mContrast;
	mSaturation = obj.mSaturation;
	mSharpness = obj.mSharpness;
	mSubjectDistanceRange = obj.mSubjectDistanceRange;
	mImageUniqueID = obj.mImageUniqueID;

	mInterOpIndex = obj.mInterOpIndex;
	mInterOpVersion = obj.mInterOpVersion;

    return *this;
}

/* -------------------------------------------------------------------
	Method:	load(PmtMetadataPtr& mdRoot)
	Description: Set member variables from values in passed in metadata object.
------------------------------------------------------------------- */
void
CPcdIFApp1IFDExif::load(PmtMetadataPtr& mdRoot)
{
	PmtMetadataPtr md;
	
	string strVal;

	md = mdRoot->getMetadatum("ImageContainer.ExifVersion");
	if (md)
	{
		mExifVersion = LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value();
	}

	md = mdRoot->getMetadatum("SceneContent.ImageCaptureDateTime.ImageCaptureDateTime");
	if (md)
	{
		mDateTimeOriginal = md->getValueStr();
	}

	md = mdRoot->getMetadatum("DigitalProcess.ImageFileCreationDateTime");
	if (md)
	{
		mDateTimeDigitized = md->getValueStr();
	}

	md = mdRoot->getMetadatum("ImageContainer.ComponentsConfiguration");
	if (md)
	{
		mComponentsConfig = LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value();
	}

	md = mdRoot->getMetadatum("CaptureConditions.Brightness");
	if (md)
	{
		mSceneBrightness = (LOCAL_PMT_CAST_TYPE(md, float)->value());
	}

	md = mdRoot->getMetadatum("CaptureConditions.SubjectDistance");
	if (md)
	{
		mSubjectDistance = (LOCAL_PMT_CAST_TYPE(md, float)->value());
	}

	md = mdRoot->getMetadatum("CaptureConditions.LightSource");
	if (md)
	{
		mArtificialIlluminant = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("CaptureConditions.Flash.Flash");
	if (md)
	{
		mFlashFired = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("CaptureConditions.FocalLength");
	if (md)
	{
		mLensFocalLength = (LOCAL_PMT_CAST_TYPE(md, float)->value());
	}

	md = mdRoot->getMetadatum("CaptureConditions.SubjectArea");
	if (md)
	{
		mSubjectArea = LOCAL_PMT_CAST_TYPE(md, vector<unsigned short>)->value();
	}

	md = mdRoot->getMetadatum("SceneContent.UserComment.UserComment");
	if (md)
	{
		// Convert vector of one or 2 byte chars to an array of one byte chars.
		vector<unsigned char>	vec;
		vec = LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value();
	
		int i = 0;

		if (mUserComment)
		{
			delete [] mUserComment;
			mUserComment = NULL;
		}
		mUserCommentSize = 0;

		// First 8 bytes define datatype.
		if (8 < vec.size())
		{
			// Save comments as ASCII strings.
			unsigned char* pComment = new unsigned char[vec.size() + 2];
			
			for (i = 0 ; i < vec.size() ; i++)
			{
				pComment[i] = vec[i];
			}

			pComment[i] = NULL;
			pComment[i + 1] = NULL;
			 
			// Check if comment is in UNICODE, by looking at the first bytes, if not UNICODE
			// default to ASCII.
			if ( (('U' == pComment[0]) || ('u' == pComment[0])) &&
				(('N' == pComment[1]) || ('n' == pComment[1])) &&
				(('I' == pComment[2]) || ('i' == pComment[2])) ) // assume rest is "code"
			{
				mUserCommentSize = ((vec.size() - 8) / 2) + 1;
				// Save comments as ASCII string.
				mUserComment = new unsigned char[mUserCommentSize];
				for (i = 0 ; i < mUserCommentSize ; i++)
				{
					mUserComment[i] = pComment[8 + (i*2) +1];
				}

			}
			else
			{
				mUserCommentSize = (vec.size() - 8) + 1;
				mUserComment = new unsigned char[mUserCommentSize];
				for (i = 0 ; i < mUserCommentSize ; i++)
				{
					mUserComment[i] = pComment[8 + i];
				}
			}

			delete [] pComment;
		}
	}


	md = mdRoot->getMetadatum("ImageContainer.FlashpixVersion");
	if (md)
	{
		mFlashPixVersion = LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value();
	}

	md = mdRoot->getMetadatum("CaptureConditions.ColorSpaceInformation");
	if (md)
	{
		mColorSpace = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("ImageContainer.ActualImageWidth");
	if (md)
	{
		mPixelXDimension = (LOCAL_PMT_CAST_TYPE(md, unsigned long)->value());
	}

	md = mdRoot->getMetadatum("ImageContainer.ActualImageHeight");
	if (md)
	{
		mPixelYDimension = (LOCAL_PMT_CAST_TYPE(md, unsigned long)->value());
	}

	md = mdRoot->getMetadatum("CaptureDevice.SpatialFrequencyResponse");
	if (md)
	{
		mSpatialFeqResponse = LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value();
	}

	md = mdRoot->getMetadatum("CaptureDevice.FocalPlaneXResolution");
	if (md)
	{
		mFocalPlaneXResolution = (LOCAL_PMT_CAST_TYPE(md, float)->value());
	}

	md = mdRoot->getMetadatum("CaptureDevice.FocalPlaneYResolution");
	if (md)
	{
		mFocalPlaneYResolution = (LOCAL_PMT_CAST_TYPE(md, float)->value());
	}

	md = mdRoot->getMetadatum("CaptureDevice.FocalPlaneResolutionUnit");
	if (md)
	{
		mFocalPlaneUnitResolution = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("CaptureConditions.CustomRendered");
	if (md)
	{
		mCustomRendered = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("CaptureConditions.WhiteBalance");
	if (md)
	{
		mWhiteBalance = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("CaptureConditions.DigitalZoomRatio");
	if (md)
	{
		mDigitalZoomRatio = (LOCAL_PMT_CAST_TYPE(md, float)->value());
	}

	md = mdRoot->getMetadatum("CaptureConditions.FocalLengthIn35mmFilm");
	if (md)
	{
		mFocalLengthIn35mmFilm = (LOCAL_PMT_CAST_TYPE(md, unsigned short)->value());
	}

	md = mdRoot->getMetadatum("CaptureConditions.SceneCaptureType");
	if (md)
	{
		mSceneCaptureType = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("CaptureConditions.GainControl");
	if (md)
	{
		mGainControl = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("CaptureConditions.Contrast");
	if (md)
	{
		mContrast = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("CaptureConditions.Saturation");
	if (md)
	{
		mSaturation = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("CaptureConditions.Sharpness");
	if (md)
	{
		mSharpness = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("CaptureConditions.SubjectDistanceRange");
	if (md)
	{
		mSubjectDistanceRange = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}


	md = mdRoot->getMetadatum("ImageContainer.ImageUniqueID");
	if (md)
	{
		mImageUniqueID = md->getValueStr();
	}

	md = mdRoot->getMetadatum("ImageContainer.Interoperability.Index");
	if (md)
	{
		mInterOpIndex = md->getValueStr();
	}

	md = mdRoot->getMetadatum("ImageContainer.Interoperability.Version");
	if (md)
	{
		mInterOpVersion = LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value();
	}
}

/* -------------------------------------------------------------------
	Method:	save(PmtMetadataPtr& mdRoot)
	Description: Save member variables values into passed in metadata object.
------------------------------------------------------------------- */
void
CPcdIFApp1IFDExif::save(PmtMetadataPtr& mdRoot)
{
	PmtMetadataPtr md;
	
	// Mandatory items, so use default of createMetadataIfNotExists = true.
	
	md = mdRoot->getMetadatum("ImageContainer.ExifVersion");
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value() = mExifVersion;
	}

	md = mdRoot->getMetadatum("ImageContainer.ComponentsConfiguration");
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, vector <unsigned char >)->value() = mComponentsConfig;
	}


	md = mdRoot->getMetadatum("ImageContainer.FlashpixVersion");
	if (md)
	{
        LOCAL_PMT_CAST_TYPE(md, vector <unsigned char >)->value() = mFlashPixVersion;
	}

	md = mdRoot->getMetadatum("CaptureConditions.ColorSpaceInformation");
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mColorSpace;
	}

	md = mdRoot->getMetadatum("ImageContainer.ActualImageWidth");
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned long)->value() = mPixelXDimension;
	}

	md = mdRoot->getMetadatum("ImageContainer.ActualImageHeight");
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned long)->value() = mPixelYDimension;
	}

	md = mdRoot->getMetadatum("ImageContainer.Interoperability.Index");
	if (md)
	{
		md->setValueStr(mInterOpIndex);
	}

	md = mdRoot->getMetadatum("ImageContainer.Interoperability.Version");
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, vector <unsigned char >)->value() = mInterOpVersion;
	}
	
	// Optional, so save if already exists or if value changed from default.

	md = mdRoot->getMetadatum("SceneContent.ImageCaptureDateTime.ImageCaptureDateTime",
		((19 == mDateTimeOriginal.size()) ? true : false ));
	if (md)
	{
		if (19 != mDateTimeOriginal.size())
		{
			// If existing value was bad delete it.
			mdRoot->deleteMetadata("SceneContent.ImageCaptureDateTime.ImageCaptureDateTime");
		}
		else
		{
			md->setValueStr(mDateTimeOriginal);
		}
	}

	md = mdRoot->getMetadatum("DigitalProcess.ImageFileCreationDateTime",
		((0 != mDateTimeDigitized.size()) ? true : false ));
	if (md)
	{
		md->setValueStr(mDateTimeDigitized);
	}

	md = mdRoot->getMetadatum("CaptureConditions.Brightness",
		((0.0f < mSceneBrightness) ? true : false ));
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, float)->value() = mSceneBrightness;
	}

	md = mdRoot->getMetadatum("CaptureConditions.SubjectDistance",
		((0.0f < mSubjectDistance) ? true : false ));
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, float)->value() = mSubjectDistance;
	}

	md = mdRoot->getMetadatum("CaptureConditions.LightSource",
		((0 != mArtificialIlluminant) ? true : false ));
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mArtificialIlluminant;
	}

	md = mdRoot->getMetadatum("CaptureConditions.Flash.Flash",
		((0 != mFlashFired) ? true : false ));
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mFlashFired;
	}

	md = mdRoot->getMetadatum("CaptureConditions.FocalLength",
		((0.0f < mLensFocalLength) ? true : false ));
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, float)->value() = mLensFocalLength;
	}

	md = mdRoot->getMetadatum("CaptureConditions.SubjectArea",
		((0 != mSubjectArea.size()) ? true : false ));
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, vector <unsigned short >)->value() = mSubjectArea;
	}

	md = mdRoot->getMetadatum("SceneContent.UserComment.UserComment",
		((NULL != mUserComment) ? true : false ));
	if (md)
	{
		if (0 < mUserCommentSize)
		{
			vector<unsigned char>	vec;

			vec.push_back('U');
			vec.push_back('N');
			vec.push_back('I');
			vec.push_back('C');
			vec.push_back('O');
			vec.push_back('D');
			vec.push_back('E');
			vec.push_back(NULL);
			
			for (int i = 0 ; i < mUserCommentSize ; i++)
			{
				vec.push_back(NULL);
				vec.push_back(mUserComment[i]);
			}

			LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value() = vec;
		}
	}

	md = mdRoot->getMetadatum("CaptureDevice.SpatialFrequencyResponse",
		((0 != mSpatialFeqResponse.size()) ? true : false ));
	if (md)
	{
        LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value() = mSpatialFeqResponse;
	}

	// If we write one than write them all.
	bool bWrite = false;
	if ((0.0f < mFocalPlaneXResolution) || (0.0f < mFocalPlaneYResolution) || (0 < mFocalPlaneUnitResolution))
	{
		bWrite = true;
	}

	md = mdRoot->getMetadatum("CaptureDevice.FocalPlaneXResolution",
		bWrite);
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, float)->value() = mFocalPlaneXResolution;
	}

	md = mdRoot->getMetadatum("CaptureDevice.FocalPlaneYResolution",
		bWrite);
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, float)->value() = mFocalPlaneYResolution;
	}

	md = mdRoot->getMetadatum("CaptureDevice.FocalPlaneResolutionUnit",
		bWrite);
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mFocalPlaneUnitResolution;
	}

	md = mdRoot->getMetadatum("CaptureConditions.CustomRendered",
		((0 != mCustomRendered) ? true : false ));
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mCustomRendered;
	}

	md = mdRoot->getMetadatum("CaptureConditions.WhiteBalance",
		((0 != mWhiteBalance) ? true : false ));
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mWhiteBalance;
	}

	md = mdRoot->getMetadatum("CaptureConditions.DigitalZoomRatio",
		((0.0f < mDigitalZoomRatio) ? true : false ));
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, float)->value() = mDigitalZoomRatio;
	}

	md = mdRoot->getMetadatum("CaptureConditions.FocalLengthIn35mmFilm",
		((0 != mFocalLengthIn35mmFilm) ? true : false ));
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned short)->value() = mFocalLengthIn35mmFilm;
	}

	md = mdRoot->getMetadatum("CaptureConditions.SceneCaptureType",
		((0 != mSceneCaptureType) ? true : false ));
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mSceneCaptureType;
	}

	md = mdRoot->getMetadatum("CaptureConditions.GainControl",
		((0 != mGainControl) ? true : false ));
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mGainControl;
	}

	md = mdRoot->getMetadatum("CaptureConditions.Contrast",
		((0 != mContrast) ? true : false ));
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mContrast;
	}

	md = mdRoot->getMetadatum("CaptureConditions.Saturation",
		((0 != mSaturation) ? true : false ));
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mSaturation;
	}

	md = mdRoot->getMetadatum("CaptureConditions.Sharpness",
		((0 != mSharpness) ? true : false ));
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mSharpness;
	}

	md = mdRoot->getMetadatum("CaptureConditions.SubjectDistanceRange",
		((0 != mSubjectDistanceRange) ? true : false ));
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mSubjectDistanceRange;
	}

	// only save Image ID if 32 bytes long
	if (32 == mImageUniqueID.size())
	{
		md = mdRoot->getMetadatum("ImageContainer.ImageUniqueID");
		if (md)
		{
			md->setValueStr(mImageUniqueID);
		}
	}
}

/* -------------------------------------------------------------------
	Method:	getUserComment
	Always returns a ASCII string pointer.
------------------------------------------------------------------- */
const unsigned char* 
CPcdIFApp1IFDExif::getUserComment()
{
	// size has to be atleast 9 bytes.
	if (mUserCommentSize < 1)
	{
		return(NULL);
	}

	return (mUserComment);
}

/* -------------------------------------------------------------------
	Method:	setUserComment
	Size does NOT include the ending NULL.
------------------------------------------------------------------- */
void 
CPcdIFApp1IFDExif::setUserComment(const unsigned char* userComment, int numChars)
{
	if (numChars < 1)
	{
		return;
	}

	if (mUserComment)
	{
		delete [] mUserComment;
		mUserComment = NULL;
	}

	mUserCommentSize = numChars + 1;
	mUserComment = new unsigned char[mUserCommentSize];

	memcpy(mUserComment,userComment,numChars);
	mUserComment[numChars] = NULL;
}
	


// ******************************************************************
//	Class:	CPcdIFApp1IFD1 Methods
// ******************************************************************

/* -------------------------------------------------------------------
	Method:	Constructor
	Description:
------------------------------------------------------------------- */

CPcdIFApp1IFD1::CPcdIFApp1IFD1()
{
	mCompression = 6;  // Always a 6 for compress thumbnail
	mOrientation = 1;	// Always set to 1.	
	mXresolution = 0.0f;	
	mYresolution = 0.0f;	
	mResolutionUnit =(unsigned short) PCD_RU_INCHES;
	mJPEGInterchangeFormat = 0;
	mJPEGInterchangeFormatLength = 0;
	mYcbCrPositioning = (unsigned short) PCD_YP_CENTERED;	
}

/* -------------------------------------------------------------------
	Method:	Destructor
	Description:
------------------------------------------------------------------- */
CPcdIFApp1IFD1::~CPcdIFApp1IFD1()
{
}

/* -------------------------------------------------------------------
	Method:	Copy
	Description:
------------------------------------------------------------------- */
CPcdIFApp1IFD1::CPcdIFApp1IFD1(const CPcdIFApp1IFD1& obj)
{
	*this = obj;
}

/* -------------------------------------------------------------------
	Method:	operator=
	Description:
------------------------------------------------------------------- */
const
CPcdIFApp1IFD1& CPcdIFApp1IFD1::operator= (const CPcdIFApp1IFD1& obj)
{
    if (this == &obj)
    {
        return *this;
    }

	mCompression = obj.mCompression;
	mOrientation = obj.mOrientation;
	mXresolution = obj.mXresolution;
	mYresolution = obj.mYresolution;
	mResolutionUnit = obj.mResolutionUnit;
	mJPEGInterchangeFormat = obj.mJPEGInterchangeFormat;
	mJPEGInterchangeFormatLength = obj.mJPEGInterchangeFormatLength;
	mYcbCrPositioning = obj.mYcbCrPositioning;

    return *this;
}

/* -------------------------------------------------------------------
	Method:	load(PmtMetadataPtr& mdRoot)
	Description: Set member variables from values in passed in metadata object.
------------------------------------------------------------------- */
void
CPcdIFApp1IFD1::load(PmtMetadataPtr& mdRoot)
{
	PmtMetadataPtr md;
	
	md = mdRoot->getMetadatum("ImageContainer.Thumbnail.Compression");
	if (md)
	{
		mCompression = (LOCAL_PMT_CAST_TYPE(md, unsigned short)->value());
	}

	md = mdRoot->getMetadatum("ImageContainer.Thumbnail.Orientation");
	if (md)
	{
		mOrientation = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("ImageContainer.Thumbnail.DisplayXResolution");
	if (md)
	{
		mXresolution = (LOCAL_PMT_CAST_TYPE(md, float)->value());
	}

	md = mdRoot->getMetadatum("ImageContainer.Thumbnail.DisplayYResolution");
	if (md)
	{
		mYresolution = (LOCAL_PMT_CAST_TYPE(md, float)->value());
	}

	md = mdRoot->getMetadatum("ImageContainer.Thumbnail.DisplayResolutionUnit");
	if (md)
	{
		mResolutionUnit = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("ImageContainer.Thumbnail.OffsetToJPEGSOI");
	if (md)
	{
		mJPEGInterchangeFormat = (LOCAL_PMT_CAST_TYPE(md, unsigned long)->value());
	}

	md = mdRoot->getMetadatum("ImageContainer.Thumbnail.BytesOfJPEGData");
	if (md)
	{
		mJPEGInterchangeFormatLength = (LOCAL_PMT_CAST_TYPE(md, unsigned long)->value());
	}

	md = mdRoot->getMetadatum("ImageContainer.Thumbnail.YCbCrPositioning");
	if (md)
	{
		mYcbCrPositioning = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}
}

/* -------------------------------------------------------------------
	Method:	save(PmtMetadataPtr& mdRoot)
	Description: Save member variables values into passed in metadata object.
------------------------------------------------------------------- */
void
CPcdIFApp1IFD1::save(PmtMetadataPtr& mdRoot)
{
	PmtMetadataPtr md;

	md = mdRoot->getMetadatum("ImageContainer.Thumbnail.Compression");
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned short)->value() = mCompression;
	}

	md = mdRoot->getMetadatum("ImageContainer.Thumbnail.Orientation");
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mOrientation;
	}

	md = mdRoot->getMetadatum("ImageContainer.Thumbnail.DisplayXResolution");
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, float)->value() = mXresolution;
	}

	md = mdRoot->getMetadatum("ImageContainer.Thumbnail.DisplayYResolution");
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, float)->value() = mYresolution;
	}

	md = mdRoot->getMetadatum("ImageContainer.Thumbnail.DisplayResolutionUnit");
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mResolutionUnit;
	}

	md = mdRoot->getMetadatum("ImageContainer.Thumbnail.OffsetToJPEGSOI");
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned long)->value() = mJPEGInterchangeFormat;
	}

	md = mdRoot->getMetadatum("ImageContainer.Thumbnail.BytesOfJPEGData");
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned long)->value() = mJPEGInterchangeFormatLength;
	}

	md = mdRoot->getMetadatum("ImageContainer.Thumbnail.YCbCrPositioning");
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned char)->value() = mYcbCrPositioning;
	}

}

// ******************************************************************
//	Class:	CPcdIFApp3IFD0 Methods
// ******************************************************************

/* -------------------------------------------------------------------
	Method:	Constructor
	Description:
------------------------------------------------------------------- */
CPcdIFApp3IFD0::CPcdIFApp3IFD0()
{
	mFilmProductCode = EK_L(""); 
	mImageSource = PCD_IS_UNIDENTIFIED; 
	mIntendedPrintArea = 0;
	mCameraOwnerID.clear();
	mCameraSerialNum.clear();
	mGroupCaption = EK_L("");
	mDealerID.clear();
	mFilmOrderID.clear();
	mBagNum.clear();
	mScanFrameSeqNum = 0;
	mFilmCategory = 0;
	mFilmGenCode = EK_L("");
	mScannerSoftware = EK_L("");
	mFilmSize = 0;
	mSbaRgbShifts[0] = 0;
	mSbaRgbShifts[1] = 0;
	mSbaRgbShifts[2] = 0;
	mSbaInputColorSpace = 0;
	mSbaInputChannelDepth[0] = 0;
	mSbaInputChannelDepth[1] = 0;
	mSbaInputChannelDepth[2] = 0;
	mCompleteExposureRecord.clear();
	mUserAdjSbaRgbShifts[0] = 0;
	mUserAdjSbaRgbShifts[1] = 0;
	mUserAdjSbaRgbShifts[2] = 0;
	mImageRotationStatus = 0;
	mRollGuid.clear();
	mMetadataVersionNum.push_back(0x30);
	mMetadataVersionNum.push_back(0x31);
	mMetadataVersionNum.push_back(0x31);
	mMetadataVersionNum.push_back(0x30);
	mEditTagArray.clear();
	mMagnification = 0.0f;
	mNativePhysicalXResolution = 0.0f;
	mNativePhysicalYResolution = 0.0f;
	mNativePhysicalResolutionUnit = (unsigned short) PCD_RU_INCHES;
}

/* -------------------------------------------------------------------
	Method:	Destructor
	Description:
------------------------------------------------------------------- */
CPcdIFApp3IFD0::~CPcdIFApp3IFD0()
{
}

/* -------------------------------------------------------------------
	Method:	Copy
	Description:
------------------------------------------------------------------- */
CPcdIFApp3IFD0::CPcdIFApp3IFD0(const CPcdIFApp3IFD0& obj)
{
	*this = obj;
}

/* -------------------------------------------------------------------
	Method:	operator=
	Description:
------------------------------------------------------------------- */
const
CPcdIFApp3IFD0& CPcdIFApp3IFD0::operator= (const CPcdIFApp3IFD0& obj)
{
    if (this == &obj)
    {
        return *this;
    }

	mFilmProductCode = obj.mFilmProductCode;
	mImageSource = obj.mImageSource;
	mIntendedPrintArea = obj.mIntendedPrintArea;
	mCameraOwnerID = obj.mCameraOwnerID;
	mCameraSerialNum = obj.mCameraSerialNum;
	mGroupCaption = obj.mGroupCaption;
	mDealerID = obj.mDealerID;
	mFilmOrderID = obj.mFilmOrderID;
	mBagNum = obj.mBagNum;
	mScanFrameSeqNum = obj.mScanFrameSeqNum;
	mFilmCategory = obj.mFilmCategory;
	mFilmGenCode = obj.mFilmGenCode;
	mScannerSoftware = obj.mScannerSoftware;
	mFilmSize = obj.mFilmSize;
	mSbaRgbShifts[0] = obj.mSbaRgbShifts[0];
	mSbaRgbShifts[1] = obj.mSbaRgbShifts[1];
	mSbaRgbShifts[2] = obj.mSbaRgbShifts[2];
	mSbaInputColorSpace = obj.mSbaInputColorSpace;
	mSbaInputChannelDepth[0] = obj.mSbaInputChannelDepth[0];
	mSbaInputChannelDepth[1] = obj.mSbaInputChannelDepth[1];
	mSbaInputChannelDepth[2] = obj.mSbaInputChannelDepth[2];
	mCompleteExposureRecord = obj.mCompleteExposureRecord;
	mUserAdjSbaRgbShifts[0] = obj.mUserAdjSbaRgbShifts[0];
	mUserAdjSbaRgbShifts[1] = obj.mUserAdjSbaRgbShifts[1];
	mUserAdjSbaRgbShifts[2] = obj.mUserAdjSbaRgbShifts[2];
	mImageRotationStatus = obj.mImageRotationStatus;
	mRollGuid = obj.mRollGuid;
	mMetadataVersionNum = obj.mMetadataVersionNum;
	mEditTagArray = obj.mEditTagArray;
	mMagnification = obj.mMagnification;
	mNativePhysicalXResolution = obj.mNativePhysicalXResolution;
	mNativePhysicalYResolution = obj.mNativePhysicalYResolution;
	mNativePhysicalResolutionUnit = obj.mNativePhysicalResolutionUnit;

    return *this;
}


/* -------------------------------------------------------------------
	Method:	load(PmtMetadataPtr& mdRoot)
	Description: Set member variables from values in passed in metadata object.
------------------------------------------------------------------- */
void
CPcdIFApp3IFD0::load(PmtMetadataPtr& mdRoot)
{
	PmtMetadataPtr md;
	
	string strVal;
	
	md = mdRoot->getMetadatum("CaptureDevice.FilmSize");
	if (md)
	{
		mFilmSize = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("CaptureDevice.FilmProductCode");
	if (md)
	{
		mFilmProductCode = md->getValueStr();
	}

	md = mdRoot->getMetadatum("DigitalProcess.ImageSourceEK");
	if (md)
	{
		mImageSource = (LOCAL_PMT_CAST_TYPE(md, unsigned short)->value());
	}
	
	md = mdRoot->getMetadatum("CaptureConditions.PAR");
	if (md)
	{
		mIntendedPrintArea = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("CaptureDevice.CameraOwner.EK");
	if (md)
	{
		vector<unsigned char>	vec;
		vec = LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value();

		mCameraOwnerID.clear();

		saveAsUnicode(vec, mCameraOwnerID);
	}

	md = mdRoot->getMetadatum("CaptureDevice.SerialNumber.Camera");
	if (md)
	{
		vector<unsigned char>	vec;
		vec = LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value();

		mCameraSerialNum.clear();

		saveAsUnicode(vec, mCameraSerialNum);
	}

	/*
	// Not handle for now, spec may change.
	md = mdRoot->getMetadatum("SceneContent.GroupCaption.UserSelectGroupTitle");
	if (md)
	{

	}
	*/
	md = mdRoot->getMetadatum("OutputOrder.Information.DealerIDNumber");
	if (md)
	{
		vector<unsigned char>	vec;
		vec = LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value();

		mDealerID.clear();

		saveAsUnicode(vec, mDealerID);
	}


	md = mdRoot->getMetadatum("OutputOrder.Information.VendorOrderNumber");
	if (md)
	{
		vector<unsigned char>	vec;
		vec = LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value();

		mFilmOrderID.clear();

		saveAsUnicode(vec, mFilmOrderID);
	}

	md = mdRoot->getMetadatum("OutputOrder.Information.EnvelopeNumber");
	if (md)
	{
		vector<unsigned char>	vec;
		vec = LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value();

		mBagNum.clear();

		saveAsUnicode(vec, mBagNum);
	}

	md = mdRoot->getMetadatum("DigitalProcess.ScanFrameSequenceNumber");
	if (md)
	{
		mScanFrameSeqNum = (LOCAL_PMT_CAST_TYPE(md, unsigned short)->value());
	}


	md = mdRoot->getMetadatum("CaptureDevice.FilmCategory");
	if (md)
	{
		mFilmCategory = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}

	md = mdRoot->getMetadatum("CaptureDevice.FilmGencode");
	if (md)
	{
		mFilmGenCode = md->getValueStr();
	}

	md = mdRoot->getMetadatum("CaptureDevice.Scanner.ModelAndVersion");
	if (md)
	{
		mScannerSoftware = md->getValueStr();
	}

	md = mdRoot->getMetadatum("DigitalProcess.History.SBARGBShifts");
	if (md)
	{

		vector<signed long>	vec;
		vec = LOCAL_PMT_CAST_TYPE(md, vector<signed long>)->value();
		if (2 < vec.size())
		{
			mSbaRgbShifts[0] = vec[0];
			mSbaRgbShifts[1] = vec[1];
			mSbaRgbShifts[2] = vec[2];
		}
	}

	md = mdRoot->getMetadatum("DigitalProcess.History.SBAInputImageColorspace");
	if (md)
	{
		mSbaInputColorSpace = (LOCAL_PMT_CAST_TYPE(md, unsigned short)->value());
	}

	md = mdRoot->getMetadatum("DigitalProcess.History.SBAInputImageBitDepth");
	if (md)
	{
		vector<unsigned short>	vec;
		vec = LOCAL_PMT_CAST_TYPE(md, vector<unsigned short>)->value();
		if (2 < vec.size())
		{
			mSbaInputChannelDepth[0] = vec[0];
			mSbaInputChannelDepth[1] = vec[1];
			mSbaInputChannelDepth[2] = vec[2];
		}
	}

	
	md = mdRoot->getMetadatum("DigitalProcess.History.SBAExposureRecord",false);
	if (md)
	{
		mCompleteExposureRecord = LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value();
	}
	else
	{
		mCompleteExposureRecord.clear();
		mdRoot->getMetadatum("DigitalProcess.History.SBAExposureRecord",true);
	}

	md = mdRoot->getMetadatum("DigitalProcess.History.UserAdjSBARGBShifts");
	if (md)
	{

		vector<signed long>	vec;
		vec = LOCAL_PMT_CAST_TYPE(md, vector<signed long>)->value();
		if (2 < vec.size())
		{
			mUserAdjSbaRgbShifts[0] = vec[0];
			mUserAdjSbaRgbShifts[1] = vec[1];
			mUserAdjSbaRgbShifts[2] = vec[2];
		}
	}

	md = mdRoot->getMetadatum("DigitalProcess.ImageRotationStatus");
	if (md)
	{
		mImageRotationStatus = (LOCAL_PMT_CAST_TYPE(md, unsigned char)->value());
	}
	
	md = mdRoot->getMetadatum("DigitalProcess.RollGuid");
	if (md)
	{
		mRollGuid = LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value();
	}	
	
	md = mdRoot->getMetadatum("ImageContainer.MetadataNumber");
	if (md)
	{
		mMetadataVersionNum = LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value();
	}

	md = mdRoot->getMetadatum("DigitalProcess.History.EditTagArray");
	if (md)
	{
		mEditTagArray = (LOCAL_PMT_CAST_TYPE(md, vector<unsigned short>)->value());
		if ((1 == mEditTagArray.size()) && (1 != mEditTagArray[0]))
		{
			// The only value Must be a 1 for digital image created
			mEditTagArray.clear();
		}
	}
	
	md = mdRoot->getMetadatum("CaptureConditions.Magnification");
	if (md)
	{
		mMagnification = (LOCAL_PMT_CAST_TYPE(md, float)->value());
	}

	md = mdRoot->getMetadatum("CaptureDevice.NativePhysicalXResolution");
	if (md)
	{
		mNativePhysicalXResolution = (LOCAL_PMT_CAST_TYPE(md, float)->value());
	}

	md = mdRoot->getMetadatum("CaptureDevice.NativePhysicalYResolution");
	if (md)
	{
		mNativePhysicalYResolution = (LOCAL_PMT_CAST_TYPE(md, float)->value());
	}
	
	md = mdRoot->getMetadatum("CaptureDevice.NativePhysicalResolutionUnit");
	if (md)
	{
		mNativePhysicalResolutionUnit = (LOCAL_PMT_CAST_TYPE(md, unsigned short)->value());
	}


}
/* -------------------------------------------------------------------
	Method:	save(PmtMetadataPtr& mdRoot)
	Description: Save member variables values into passed in metadata object.
------------------------------------------------------------------- */
void
CPcdIFApp3IFD0::save(PmtMetadataPtr& mdRoot)
{
	PmtMetadataPtr md;
	
	// Mandatory
	
	md = mdRoot->getMetadatum("ImageContainer.MetadataNumber");
	if (md)
	{
		(LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value()) = mMetadataVersionNum;
	}

	md = mdRoot->getMetadatum("DigitalProcess.ImageSourceEK");
	if (md)
	{
		(LOCAL_PMT_CAST_TYPE(md, unsigned short)->value()) = mImageSource;
	}

	// Save if already exists or if value changed from default.

	md = mdRoot->getMetadatum("CaptureDevice.FilmProductCode",
		((0 != mFilmProductCode.size()) ? true : false ));
	if (md)
	{
		md->setValueStr(mFilmProductCode);
	}

	
	md = mdRoot->getMetadatum("CaptureConditions.PAR",
		((0 != mIntendedPrintArea) ? true : false ));
	if (md)
	{
		(LOCAL_PMT_CAST_TYPE(md, unsigned char)->value()) = mIntendedPrintArea;
	}

	md = mdRoot->getMetadatum("CaptureDevice.CameraOwner.EK",
		((0 != mCameraOwnerID.size()) ? true : false ));
	if (md && mCameraOwnerID.size())
	{
		(LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value()) = mCameraOwnerID;
	}

	md = mdRoot->getMetadatum("CaptureDevice.SerialNumber.Camera",
		((0 != mCameraSerialNum.size()) ? true : false ));
	if (md && mCameraSerialNum.size())
	{
		(LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value()) = mCameraSerialNum;
	}

	/*
	// Not handle for now, spec may change.
	md = mdRoot->getMetadatum("SceneContent.GroupCaption.UserSelectGroupTitle",
		((0 != mGroupCaption.size()) ? true : false ));
	if (md && mGroupCaption.size())
	{
		// Need to add "UNICODE"

		string temp;

		temp[0] = 'U';
		temp[1] = 'N';
		temp[2] = 'I';
		temp[3] = 'C';
		temp[4] = 'O';
		temp[5] = 'D';
		temp[6] = 'E';
		temp[7] = NULL;

		for ( int i = 8 ; i < (2 * mGroupCaption.size()); i = i + 2 )
		{
			temp[i] = NULL;
			temp[i+1] = mGroupCaption[i/2];
		}
		temp[i] = NULL;
		temp[i+1] = NULL;
		
		md->setValueStr(temp);		
	}
	*/

	md = mdRoot->getMetadatum("OutputOrder.Information.DealerIDNumber",
		((0 != mDealerID.size()) ? true : false ));
	if (md && mDealerID.size())
	{
		(LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value()) = mDealerID;
	}

	md = mdRoot->getMetadatum("OutputOrder.Information.VendorOrderNumber",
		((0 != mFilmOrderID.size()) ? true : false ));
	if (md && mFilmOrderID.size())
	{
		(LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value()) = mFilmOrderID;
	}

	md = mdRoot->getMetadatum("OutputOrder.Information.EnvelopeNumber",
		((0 != mBagNum.size()) ? true : false ));
	if (md && mBagNum.size())
	{
		LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value() = mBagNum;
	}

	md = mdRoot->getMetadatum("DigitalProcess.ScanFrameSequenceNumber",
		((0 != mScanFrameSeqNum) ? true : false ));
	if (md)
	{
		(LOCAL_PMT_CAST_TYPE(md, unsigned short)->value()) = mScanFrameSeqNum;
	}

	md = mdRoot->getMetadatum("CaptureDevice.FilmCategory",
		((0 != mFilmCategory) ? true : false ));
	if (md)
	{
		(LOCAL_PMT_CAST_TYPE(md, unsigned char)->value()) = mFilmCategory;
	}

	md = mdRoot->getMetadatum("CaptureDevice.FilmGencode",
		((0 != mFilmGenCode.size()) ? true : false ));
	if (md)
	{
		 md->setValueStr(mFilmGenCode);
	}

	md = mdRoot->getMetadatum("CaptureDevice.Scanner.ModelAndVersion",
		((0 < mScannerSoftware.size()) ? true : false ));
	if (md && mScannerSoftware.size())
	{
		md->setValueStr(mScannerSoftware);
	}

	// Write Film Size if source image was from a scan.
	md = mdRoot->getMetadatum("CaptureDevice.FilmSize",
		((1 == mImageSource) ? true : false ));
	if (md)
	{
		(LOCAL_PMT_CAST_TYPE(md, unsigned char)->value()) = mFilmSize;
	}

	md = mdRoot->getMetadatum("DigitalProcess.History.SBARGBShifts",
		(((0 != mSbaRgbShifts[0]) || (0 != mSbaRgbShifts[1]) || (0 != mSbaRgbShifts[2])) ? true : false ));
	if (md)
	{
		vector<signed long>	vec;
		vec.push_back(mSbaRgbShifts[0]);
		vec.push_back(mSbaRgbShifts[1]);
		vec.push_back(mSbaRgbShifts[2]);
		LOCAL_PMT_CAST_TYPE(md, vector<signed long>)->value() = vec;
	}

	md = mdRoot->getMetadatum("DigitalProcess.History.SBAInputImageColorspace",
		((0 != mSbaInputColorSpace) ? true : false ));
	if (md)
	{
		(LOCAL_PMT_CAST_TYPE(md, unsigned short)->value()) = mSbaInputColorSpace;
	}

	md = mdRoot->getMetadatum("DigitalProcess.History.SBAInputImageBitDepth",
		(((0 != mSbaInputChannelDepth[0]) || (0 != mSbaInputChannelDepth[1]) || (0 != mSbaInputChannelDepth[2])) ? true : false ));
	if (md)
	{
		vector<unsigned short>	vec;
		vec.push_back(mSbaInputChannelDepth[0]);
		vec.push_back(mSbaInputChannelDepth[1]);
		vec.push_back(mSbaInputChannelDepth[2]);
		(LOCAL_PMT_CAST_TYPE(md, vector<unsigned short>)->value()) = vec;
	}

	md = mdRoot->getMetadatum("DigitalProcess.History.SBAExposureRecord",
		((0 != mCompleteExposureRecord.size()) ? true : false ));
	if (md && mCompleteExposureRecord.size())
	{
		(LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value()) = mCompleteExposureRecord;
	}

	md = mdRoot->getMetadatum("DigitalProcess.History.UserAdjSBARGBShifts",
		(((0 != mUserAdjSbaRgbShifts[0]) || (0 != mUserAdjSbaRgbShifts[1]) || (0 != mUserAdjSbaRgbShifts[2])) ? true : false ));
	if (md)
	{

		vector<signed long>	vec;
		vec.push_back(mUserAdjSbaRgbShifts[0]);
		vec.push_back(mUserAdjSbaRgbShifts[1]);
		vec.push_back(mUserAdjSbaRgbShifts[2]);
		(LOCAL_PMT_CAST_TYPE(md, vector<signed long>)->value()) = vec;
	}

	md = mdRoot->getMetadatum("DigitalProcess.ImageRotationStatus",
		((0 != mImageRotationStatus) ? true : false ));
	if (md)
	{
		(LOCAL_PMT_CAST_TYPE(md, unsigned char)->value()) = mImageRotationStatus;
	}
	
	md = mdRoot->getMetadatum("DigitalProcess.RollGuid",
		((0 != mRollGuid.size()) ? true : false ));
	if (md && mRollGuid.size())
	{
		 (LOCAL_PMT_CAST_TYPE(md, vector<unsigned char>)->value()) = mRollGuid;
	}	

	md = mdRoot->getMetadatum("DigitalProcess.History.EditTagArray",
		((0 != mEditTagArray.size()) ? true : false ));
	if (md)
	{
		(LOCAL_PMT_CAST_TYPE(md, vector<unsigned short>)->value()) = mEditTagArray;
	}
	
	md = mdRoot->getMetadatum("CaptureConditions.Magnification",
		((0.0f < mMagnification) ? true : false ));
	if (md)
	{
		 (LOCAL_PMT_CAST_TYPE(md, float)->value()) = mMagnification;
	}

	// If we write one than write them all.
	bool bWrite = false;
	if ((0.0f < mNativePhysicalXResolution) || (0.0f < mNativePhysicalYResolution) 
		|| (0 < mNativePhysicalResolutionUnit))
	{
		bWrite = true;
	}

	md = mdRoot->getMetadatum("CaptureDevice.NativePhysicalXResolution",
		bWrite);
	if (md)
	{
		 (LOCAL_PMT_CAST_TYPE(md, float)->value()) = mNativePhysicalXResolution;
	}
	
	md = mdRoot->getMetadatum("CaptureDevice.NativePhysicalYResolution",
		bWrite);
	if (md)
	{
		 (LOCAL_PMT_CAST_TYPE(md, float)->value()) = mNativePhysicalYResolution;
	}

	md = mdRoot->getMetadatum("CaptureDevice.NativePhysicalResolutionUnit",
		bWrite);
	if (md)
	{
		LOCAL_PMT_CAST_TYPE(md, unsigned short)->value() = mNativePhysicalResolutionUnit;
	}

}

/* -------------------------------------------------------------------
	Method:	setSbaRgbShifts()
	Description:
------------------------------------------------------------------- */
void
CPcdIFApp3IFD0::setSbaRgbShifts(int* sbaRgbShifts)
{
	if (sbaRgbShifts)
	{
		mSbaRgbShifts[0] = sbaRgbShifts[0];
		mSbaRgbShifts[1] = sbaRgbShifts[1];
		mSbaRgbShifts[2] = sbaRgbShifts[2];
	}
}

/* -------------------------------------------------------------------
	Method:	setSbaRgbShifts()
	Description:
------------------------------------------------------------------- */
void
CPcdIFApp3IFD0::setUserAdjSbaRgbShifts(int* sbaRgbShifts)
{
	if (sbaRgbShifts)
	{
		mUserAdjSbaRgbShifts[0] = sbaRgbShifts[0];
		mUserAdjSbaRgbShifts[1] = sbaRgbShifts[1];
		mUserAdjSbaRgbShifts[2] = sbaRgbShifts[2];
	}
}

/* -------------------------------------------------------------------
	Method:	setSbaInputChannelDepth()
	Description:
------------------------------------------------------------------- */
void 
CPcdIFApp3IFD0::setSbaInputChannelDepth(unsigned short* sbaInputChannelDepth)
{
	if (sbaInputChannelDepth)
	{
		mSbaInputChannelDepth[0] = sbaInputChannelDepth[0];
		mSbaInputChannelDepth[1] = sbaInputChannelDepth[1];
		mSbaInputChannelDepth[2] = sbaInputChannelDepth[2];
	}
}

// ******************************************************************
//	Class:	CPictCDImageFile Methods
// ******************************************************************

/* -------------------------------------------------------------------
	Method:	Constructor
	Description:
------------------------------------------------------------------- */
CPictCDImageFile::CPictCDImageFile()
{
	mFilename = EK_L("");
	mImage = NULL;
	mThumbnail = NULL;

	// Load the default schema
	PmtLogicalDefinitionInterpreter *definitionInterpreter =
		new PmtLogicalDefinitionInterpreter;
	definitionInterpreter->load();
	delete definitionInterpreter;
}

/* -------------------------------------------------------------------
	Method:	Destructor
	Description:
------------------------------------------------------------------- */
CPictCDImageFile::~CPictCDImageFile()
{
	if (mImage)
	{
		delete mImage;
		mImage = NULL;
	}

	if (mThumbnail)
	{
		delete mThumbnail;
		mThumbnail = NULL;
	}
}

/* -------------------------------------------------------------------
	Method:	Copy
	Description:
------------------------------------------------------------------- */
CPictCDImageFile::CPictCDImageFile(const CPictCDImageFile& obj)
{
	*this = obj;
}

/* -------------------------------------------------------------------
	Method:	operator=
	Description:
------------------------------------------------------------------- */
const
CPictCDImageFile& CPictCDImageFile::operator= (const CPictCDImageFile& obj)
{
    if (this == &obj)
    {
        return *this;
    }

	mPcdIFApp1IFD0 = obj.mPcdIFApp1IFD0;
	mPcdIFApp1IFDExif = obj.mPcdIFApp1IFDExif;
	mPcdIFApp1IFD1 = obj.mPcdIFApp1IFD1;
	mPcdIFApp3IFD0 = obj.mPcdIFApp3IFD0;

	// Also copy image
	if (mImage)
	{
		delete mImage;
		mImage = NULL;
	}
	unsigned long imageSize = (mPcdIFApp1IFDExif.mPixelXDimension *
								mPcdIFApp1IFDExif.mPixelYDimension * 3);
	if (0 < imageSize)
	{
		mImage = new byte[imageSize];
		if (!mImage)
		{
			return *this;
		}
		memcpy(mImage,obj.mImage,imageSize);
	}

	// Copy thumbnail
	if (mThumbnail)
	{
		delete mThumbnail;
		mThumbnail = NULL;
	}
	if (obj.mThumbnail)
	{
		mThumbnail = new byte[THUMBSIZE];
		if (!mThumbnail)
		{
			return *this;
		}
		memcpy(mThumbnail,obj.mThumbnail,THUMBSIZE);
	}
    return *this;
}

/* -------------------------------------------------------------------
	Method:	load()
	Description: The load method for JPG files.
------------------------------------------------------------------- */
BOOL CPictCDImageFile::load(const _TCHAR* filename, PmtMetadataPtr& mdExisting)
{

	BOOL rtn = TRUE;

	PmtAccessorPtr inAcc = NULL;


	try
	{
		mFilename = filename;
	
        // Create the appropriate file accessor

        inAcc = PmtAccessor::getAccessor(filename);



        // verify that an accessor was returned.

        if (!(inAcc.ptr()))

        {

			throw -1; // error

		}

           

        // Read in the metadata and close the file

        PmtMetadataPtr mdRoot = inAcc->readMetadata();

        

        // Make sure that we got a valid root pointer

		if (!(mdRoot.ptr()))

		{

			throw -1; // error

		}



		mPcdIFApp1IFD0.load(mdRoot);
		mPcdIFApp1IFDExif.load(mdRoot);
		mPcdIFApp1IFD1.load(mdRoot);
		mPcdIFApp3IFD0.load(mdRoot);

		mdExisting = inAcc->readMetadata();  // reads in all metadata

		// Load Image data
		if (mImage)
		{
			delete mImage;
			mImage = NULL;
		}



		// Check to make sure width and height are set

		if ((0 < mPcdIFApp1IFD0.mWidth) || (0 < mPcdIFApp1IFD0.mHeight))

		{

			mPcdIFApp1IFDExif.mPixelXDimension = mPcdIFApp1IFD0.mWidth;

			mPcdIFApp1IFDExif.mPixelYDimension = mPcdIFApp1IFD0.mHeight;

		}

		else

		{

			mPcdIFApp1IFD0.mWidth = mPcdIFApp1IFDExif.mPixelXDimension;

			mPcdIFApp1IFD0.mHeight = mPcdIFApp1IFDExif.mPixelYDimension;

		} 
		unsigned long imageSize = (mPcdIFApp1IFD0.mWidth *
									mPcdIFApp1IFD0.mHeight * 3);
		if (0 < imageSize)
		{
			// Get the image accessor from the accessor and get the pixels.

			PmtImageAccessor* imgAcc = inAcc->getImageAccessor();

			if (!imgAcc)

			{

				throw -1; // error

			}



			mImage = new byte[imageSize];
			if (!mImage)
			{
				throw -1; // error

			}
			memset(mImage,NULL,imageSize);

			// Create a PmtImageRect to pass into the getPixels method.
			PmtImageRect imgRectIn(0, mPcdIFApp1IFD0.mWidth - 1, 
				0, mPcdIFApp1IFD0.mHeight - 1, mImage, 
				8, PmtImageRect::PMT_RGB, 3, PmtImageRect::PMT_INTERLEAVED);

			
			if (!(imgAcc->getPixels(imgRectIn)))
			{
				throw -1; // error

			}
		}
	
		mdRoot->deleteMetadata("*");

	}
	catch(...)
	{
		rtn = FALSE;
	}


	if (inAcc.ptr())

	{

		inAcc->close();

	}


	return rtn;
}



/* -------------------------------------------------------------------
	Method:	makeThumbnail
	Description: Subsample main image to build TN image.
------------------------------------------------------------------- */
BOOL 
CPictCDImageFile::makeThumbnail(void)
{
	if (NULL == mImage)
	{
		return FALSE;
	}

	if (NULL != mThumbnail)
	{
		delete mThumbnail;
		mThumbnail = NULL;
	}

	int width = getPixelXDimension();
	int height = getPixelYDimension();

	// Columns and rows to skip over in TN image to center in image.
	int xSkip = 0;
	int ySkip = 0;

	// Subsmaple factor, and how times in each direction.
	int sample = 0;
	int xTimes = 0;
	int yTimes = 0;
	float imageRatio = (float) height / width;
	float tnRatio = (float) THUMBHEIGHT / THUMBWIDTH;

	mThumbnail = new byte[THUMBSIZE];
	if (NULL == mThumbnail)
	{
		return FALSE;
	}

	memset(mThumbnail, NULL, THUMBSIZE);

	byte* pTNPixel = mThumbnail;
	byte* pImagePixel = mImage;

	if (imageRatio <= 1)
	{
		sample = width / THUMBWIDTH;
		xTimes = THUMBWIDTH;
		
		yTimes = xTimes * imageRatio;
		if (THUMBHEIGHT < yTimes)
		{
			yTimes = THUMBHEIGHT;
		}

		ySkip = (THUMBHEIGHT - yTimes)/2;
		xSkip = 0;

		pTNPixel = mThumbnail + (ySkip * THUMBWIDTH * 3);
	}
	else
	{
		sample = height / THUMBHEIGHT;
		yTimes = THUMBHEIGHT;
		
		xTimes = yTimes / imageRatio;
		if (THUMBWIDTH < xTimes)
		{
			xTimes = THUMBWIDTH;
		}
		
		xSkip = (THUMBWIDTH - xTimes)/2;
		ySkip = 0;
		pTNPixel = mThumbnail + (xSkip * 3);
	}

	int xi=0;
	int yi=0;
	int x=0;
	int y=0;
	long index = 0;

	for (y=0 ; y < yTimes ; y++)
	{
		xi = 0;
		for (x=0 ; x < xTimes ; x++)
		{
			index = (y * sample * width * 3) + (x * sample * 3);
			*pTNPixel = pImagePixel[index];
			pTNPixel++;
			*pTNPixel = pImagePixel[index + 1];
			pTNPixel++;
			*pTNPixel = pImagePixel[index + 2];
			pTNPixel++;
			xi++;
		}
		yi++;
		if (0 < xSkip)
		{
			pTNPixel = mThumbnail + ( ((y + 1) * THUMBWIDTH * 3) + (xSkip * 3));
		}
	}



	return TRUE;
}

/* -------------------------------------------------------------------
	Method:	save(filename, mdExisting)
	Description: Save metadata to Picture CD JPEG image file.
	mdExisting is used to preserve Metadata that was in the orginal image file.
------------------------------------------------------------------- */
BOOL CPictCDImageFile::save(const _TCHAR* filename, PmtMetadataPtr& mdExisting)
{

	BOOL rtn = FALSE;

#if defined(HAVE_KIESEXIF) || defined(HAVE_OPENEXIF)

	if (mImage)

	{
		try
		{
			mFilename = filename;

			// Create a Exif Accessor. PmtAccessorPtr is a Smart Pointer and doesn't
			// require deleting (trying to will cause a compile error).
			PmtAccessorPtr exifAcc = new PmtExifAccessor();

			// Open the file we are creating
			if ( exifAcc->open(filename, PMT_CREATE) == false )
			{
				throw -1; //error
			}

			// Create a PmtImageRect to pass into the setPixels method.
			PmtImageRect imgRectOut(0, mPcdIFApp1IFDExif.mPixelXDimension - 1, 
				0, mPcdIFApp1IFDExif.mPixelYDimension - 1, mImage, 
				8, PmtImageRect::PMT_RGB, 3, PmtImageRect::PMT_INTERLEAVED);

			// Get the image accessor from the Exif accessor and set the pixels.
			PmtImageAccessor* imgAcc = exifAcc->getImageAccessor();
			if ( imgAcc != NULL )
			{
				imgAcc->setPixels(imgRectOut);
			}

			
			if (FALSE == makeThumbnail())
			{
				throw -1; //error

			}


			// Create the PmtImageRect for the Thumbnail
			PmtImageRect thumbImgRect(0,THUMBWIDTH-1,0,THUMBHEIGHT-1,mThumbnail);
			
			if ( imgAcc != NULL )
			{
				// Set the thumbnail pixels
				imgAcc->setThumbPixels(thumbImgRect);
			}

			mPcdIFApp1IFD0.save(mdExisting);
			mPcdIFApp1IFDExif.save(mdExisting);
			mPcdIFApp1IFD1.save(mdExisting);
			mPcdIFApp3IFD0.save(mdExisting);

			exifAcc->writeMetadata(mdExisting);

			exifAcc->close();



			rtn = TRUE;

		}
		catch(...)
		{
			rtn = FALSE;
		}

	}
#endif

	return rtn;
}

/* -------------------------------------------------------------------
	Method:	setImage(pImage)
	Description: Set pixel data for main image.
		pImage must be pixel interleaved.
------------------------------------------------------------------- */
BOOL 
CPictCDImageFile::setImage(const byte* pImage)
{
	if (mImage)
	{
		delete mImage;
		mImage = NULL;
	}
	unsigned long imageSize = (mPcdIFApp1IFDExif.mPixelXDimension *
								mPcdIFApp1IFDExif.mPixelYDimension * 3);
	if (0 < imageSize)
	{
		mImage = new byte[imageSize];
		if (!mImage)
		{
			return FALSE;
		}
		memcpy(mImage,pImage,imageSize);
	}

	return TRUE;
}


/* -------------------------------------------------------------------
	Method:	saveAsUnicode
	Description: Converts src vector to a UNCODE formated vector.
------------------------------------------------------------------- */
void
saveAsUnicode(vector <unsigned char>& vec, vector <unsigned char>& dest )
{
	// vec start with 8 bytes that define datatype, ASCII or UNICODE
	if (8 < vec.size())
	{
		if ( (('U' == vec[0]) || ('u' == vec[0])) &&
			(('N' == vec[1]) || ('n' == vec[1])) &&
			(('I' == vec[2]) || ('i' == vec[2])) ) // assume rest is code
		{
			for (int i = 0 ; i < vec.size(); i++ )
			{
				dest.push_back(vec[i]);
			}
		}
		else
		{
			dest.push_back('U');
			dest.push_back('N');
			dest.push_back('I');
			dest.push_back('C');
			dest.push_back('O');
			dest.push_back('D');
			dest.push_back('E');
			dest.push_back(NULL);
			
			for ( int i = 8 ; i < vec.size(); i++ )
			{
				dest.push_back(NULL);
				dest.push_back(vec[i]);
			}

		}
	}
}

/* -------------------------------------------------------------------
	Method:	unicodeVec2AsciiVec
	Description: Converts src vector to an ASCII formated vector.
------------------------------------------------------------------- */
void
unicodeVec2AsciiVec(vector<unsigned char> &src, vector<unsigned char> &dest )
{
	// vec start with 8 bytes that define datatype, ASCII or UNICODE
	// Kodak will always use UniCode.
	if (8 < src.size())
	{
		if ( (('U' == src[0]) || ('u' == src[0])) &&
			(('N' == src[1]) || ('n' == src[1])) &&
			(('I' == src[2]) || ('i' == src[2])) ) // assume rest is code
		{
			dest.clear();
			dest.push_back('A');
			dest.push_back('S');
			dest.push_back('C');
			dest.push_back('I');
			dest.push_back('I');
			dest.push_back(NULL);
			dest.push_back(NULL);
			dest.push_back(NULL);

			for (int i = 9 ; i < src.size(); i=i+2 )
			{
				dest.push_back(src[i]);
			}
			dest.push_back(NULL);
		}
	}
}

