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
 * Creation Date: 7/18/00
 *
 * Original Author: 
 * Chris Lin <ti.lin@kodak.com> 
 * 
 * Contributor(s): 
 * Ricardo Rosario  ricardo.rosario@kodak.com
 * Kenneth Arnold   kenneth.arnold@kodak.com
 * Sam Fryer        samuel.fryer@kodak.com
 */

#include "EkCompiler.h"

#include "PmtCommon.h"
#include "PmtExifAccessor.h"

#if defined(HAVE_KIESEXIF) || defined(HAVE_OPENEXIF)

#include "PmtTranslator.h"

#include "ExifRawAppSeg.h"

#include "PmtCompositeMetadata.h"
#include "PmtRootMetadata.h"

#include <cstdio>

// These following define metadata identifiers located outside the APPx segments
#define PMT_ADOBE_XMP_IDENTIFIER               "http://ns.adobe.com/xap/1.0/"
#define PMT_EXIF_XMP_LOC                       "XMP"
#define PMT_EXIF_AUDIO_TYPE                    "audio"
#define NON_TAGGED_METADATA		                70000
#define DIGITALPROCESS_JPEG_ALPHAQTABLE        "70001"
#define DIGITALPROCESS_JPEG_CHROMA1QTABLE      "70002"
#define DIGITALPROCESS_JPEG_CHROMA2QTABLE      "70003"
#define DIGITALPROCESS_JPEG_LUMINQTABLE        "70004"
#define PMT_EXIF_AUDIO_TAG                      70005

#define DIGITALPROCESS_JPEG_CHROMINANCESAMPLING 70006
#define DIGITALPROCESS_JPEG_COMPONENTS          70007
#define DIGITALPROCESS_JPEG_HORIZONTALSAMPLING  70008
#define DIGITALPROCESS_JPEG_VERTICALSAMPLING    70009
#define DIGITALPROCESS_JPEG_HEIGHT              70010
#define DIGITALPROCESS_JPEG_PRECISION           70011
#define DIGITALPROCESS_JPEG_WIDTH               70012
#define DIGITALPROCESS_JPEG_COMMSEG             70013

#define PMT_EXIF_ELEMENT_NAME   "EXIF"


PmtExifAccessor::~PmtExifAccessor()
{
    close();
}

void PmtExifAccessor::close()
{
    if (!mClosing && mOwnImgHandle && mImageHandle)
    {
        mClosing = true;

#if defined HAVE_KIESEXIF
        NIFEXIF_Close(mImageHandle);
#endif                          // HAVE_KIESEXIF

#if defined HAVE_OPENEXIF

#ifdef PMT_XMP_ACCESSOR
		if(openMode != PMT_READONLY)
			SaveXMPValues();
#endif //end of PMT_XMP_ACCESSOR

        mImageHandle->close();
#endif                          //HAVE_OPENEXIF


        delete mImageHandle;
        mImageHandle = NULL;
        mOwnImgHandle = false;
    }
    mClosing = false;
}

bool PmtExifAccessor::open(const char *name, PmtOpenMode mode)
{
    close();
    bool returnValue = false;
    
#if defined HAVE_KIESEXIF
    NIFStatus status;
    NIFExifImageInfo imageInfo;
    NIFExifFileType fileType;
    NIFOpenMode openMode = NIF_OPEN_READONLY;

    NIFExifImageHandle *imgHandle = new NIFExifImageHandle;
    if (mode != PMT_CREATE)
    {
        if (mode == PMT_READWRITE)
            openMode = NIF_OPEN_READWRITE;

        status = NIFEXIF_OpenByFileName((char *) name, &fileType,
                                        &imageInfo, openMode, imgHandle);
    }
    else
    {
        fileType = NIF_FILE_EXIF;
        status =
            NIFEXIF_CreateByFileName((char *) name, fileType, &imageInfo,
                                     imgHandle);
    }

    if (status == NIF_OK)
    {
        mFileName = name;
        mImageHandle = imgHandle;
        mImageInfo = imageInfo;
        mOwnImgHandle = true;
        returnValue = true;
    }
    else
    {
        delete imgHandle;
    }
#endif                          // HAVE_KIESEXIF

#if defined HAVE_OPENEXIF
    char openMode[] = {0,0,0};
    if (mode == PMT_READONLY)
    {
        openMode[0] = 'r';
    }
    else if (mode == PMT_CREATE)
    {
        openMode[0] = 'w';
    }
    else //if (mode == PMT_READWRITE)
    {
        openMode[0] = 'r';
        openMode[1] = '+';
    }

    mImageHandle = new ExifImageFile();

    if (mImageHandle->open(name,openMode) != EXIF_OK)
    {
        delete mImageHandle;
        mImageHandle = NULL;
    }
    else
    {
        mFileName = name;
        mOwnImgHandle = true;
        returnValue = true;
    }

#ifdef PMT_XMP_ACCESSOR
	this->openMode = mode;
#endif  //PMT_XMP_ACCESSOR

#endif                          // HAVE_OPENEXIF

    return returnValue;
}

bool PmtExifAccessor::recognize(const SPmtString & name)
{
#if defined HAVE_KIESEXIF
    if (NIFEXIF_IsExifFile((char *) (name.c_str())))
#endif                          // HAVE_KIESEXIF
#if defined HAVE_OPENEXIF
    // All EXIF files are, by definition, JPEG files as well.
    if ( ExifJpegImage::isJpegFile((char *) name.c_str()) == EXIF_OK )
#endif                          // HAVE_OPENEXIF
        return true;
    else
        return false;
}

#ifdef HAVE_KIESEXIF
bool PmtExifAccessor::readNonTaggedMetadata(
        SPmtString type, SPmtString tag,
        ExifTagEntry & tagEntry)
{
    bool success = false;

    if (type == PMT_EXIF_AUDIO_TYPE)
    {
            success = readAudio(tagEntry);
    }
    else if  (  tag == DIGITALPROCESS_JPEG_ALPHAQTABLE
            || tag == DIGITALPROCESS_JPEG_CHROMA1QTABLE
            || tag == DIGITALPROCESS_JPEG_CHROMA2QTABLE
            || tag == DIGITALPROCESS_JPEG_LUMINQTABLE)
    {
        success = getQTable(tag, tagEntry);
    }
    else
    {
        int tagNum = atoi(tag.c_str());
        if( tagNum == DIGITALPROCESS_JPEG_CHROMINANCESAMPLING )
        {
            // Implement this!!! Does KIES support this???
        }
        else if( tagNum == DIGITALPROCESS_JPEG_COMPONENTS )
        {
            tagEntry.count = 1;
            tagEntry.tagtype = EXIF_SHORT;
            tagEntry.tagLocation = UNDEFINED_LOCATION;
            tagEntry.tagValue.ShortValue = new unsigned short[1];
            tagEntry.tagValue.ShortValue[0] = mImageInfo.numChannels;
            success = true;
        }
        else if( tagNum == DIGITALPROCESS_JPEG_HORIZONTALSAMPLING )
        {
            // Implement this!!! Does KIES support this???
        }
        else if( tagNum == DIGITALPROCESS_JPEG_VERTICALSAMPLING )
        {
            // Implement this!!! Does KIES support this???
        }
        else if( tagNum == DIGITALPROCESS_JPEG_HEIGHT )
        {
            tagEntry.count = 1;
            tagEntry.tagtype = EXIF_LONG;
            tagEntry.tagLocation = UNDEFINED_LOCATION;
            tagEntry.tagValue.LongValue = new unsigned int[1];
            tagEntry.tagValue.LongValue[0] = mImageInfo.imageHeight;
            success = true;
        }
        else if( tagNum == DIGITALPROCESS_JPEG_PRECISION )
        {
            tagEntry.count = 1;
            tagEntry.tagtype = EXIF_SHORT;
            tagEntry.tagLocation = UNDEFINED_LOCATION;
            tagEntry.tagValue.ShortValue = new unsigned short[1];
            tagEntry.tagValue.ShortValue[0] = mImageInfo.precision;
            success = true;
        }
        else if( tagNum == DIGITALPROCESS_JPEG_WIDTH )
        {
            tagEntry.count = 1;
            tagEntry.tagtype = EXIF_LONG;
            tagEntry.tagLocation = UNDEFINED_LOCATION;
            tagEntry.tagValue.LongValue = new unsigned int[1];
            tagEntry.tagValue.LongValue[0] = mImageInfo.imageWidth;
            success = true;
        }
    }

    return success;
    
}
#endif

#ifdef HAVE_OPENEXIF
void PmtExifAccessor::readTags(
                        unsigned short appKey, 
                        SPmtString appKeyString,
                        SPmtString appLocation,
                        PmtMetadataPtr rootMd,
                        PmtTranslationTableElements * theTable)
{
    ExifPathsTags pathsTags;
    mImageHandle->getAllTags( appKey, appKeyString, pathsTags );

    // Get an iterator for the IFDs in App1
    ExifPathsTagsIter crntpathsTags = pathsTags.begin();
    ExifPathsTagsIter endpathsTags = pathsTags.end();

    // Process the IFDs
    int ifdTag;
    int ifdIndex;
    while( crntpathsTags != endpathsTags )
    {
        SPmtString location = "";
        ExifIFDPathIter crntPath = (*crntpathsTags).first.begin();
        ExifIFDPathIter endPath = (*crntpathsTags).first.end();
        while( crntPath != endPath )
        {
            ifdTag = (*crntPath).first;
            ifdIndex = (*crntPath).second;
            if (ifdTag==0)
            {
                location += appLocation;
                location += (char)(48+ifdIndex); // append the IFD #
            }
            else if (ifdTag==EXIFTAG_EXIFIFDPOINTER && ifdIndex==0)
                location += ".EXIF_IFD";
            else if (ifdTag==EXIFTAG_EXIFINTEROPIFDPOINTER && ifdIndex==0)
                location += ".INTEROP_IFD";
            else if (ifdTag==EXIFTAG_GPSINFO && ifdIndex==0)
                location += ".GPSINFO_IFD";
            else if (ifdTag==EXIFTAG_FACESINFO && ifdIndex==0)
                location += ".FACES_IFD";
            else
            {
                location = "";
                break;
            }

            crntPath++;
        }

        processEntries(location, (*crntpathsTags).second, theTable,
            rootMd);

        crntpathsTags++;
    }
}

static EkString convertRawDataToString(char * data, int size)
{
	EkString dataString;
	char * dtmp = data;
	int i;

	// The raw data contains the identifier, so we need to skip past it...
	for (i=0;(i<size)&&(*dtmp != 0);i++,dtmp++);

	// Now convert the data...
	for (; i<size; i++,dtmp++)
	{
		if ((*dtmp>0x1F)&&(*dtmp<0x7F)) 
			dataString += *dtmp;
	}
	return dataString;
}

void PmtExifAccessor::getXMPDocs(
                        PmtMetadataPtr rootMd,
                        PmtTranslationTableElements * theTable)
{
    // Get a vector with all the application segments in the file
    vector<ExifAppSegment*> appSegs = mImageHandle->getAllAppSegs();

	vector<ExifAppSegment*>::iterator appIter = appSegs.begin();
	while (appIter != appSegs.end())
	{
		if (!strcmp((*appIter)->getAppIdent(),PMT_ADOBE_XMP_IDENTIFIER))
		{
			if((*appIter)->isRaw())
			{
				char * data = ((ExifRawAppSeg *)(*appIter))->getRawData();

				#ifdef PMT_XMP_ACCESSOR
				
				if(mnXMPOffset < 0 || mnXMPLastBytes <= 0)
				{
					if(mXMPAccessor.open((unsigned char*)data,
						((ExifRawAppSeg *)(*appIter))->getLength(),openMode))
					{
						mnXMPOffset = mXMPAccessor.StartPosition();
						mnXMPLastBytes = mnXMPOffset + mXMPAccessor.XMPBlockSize();
					}
				}
				else
				{
					mXMPAccessor.append((unsigned char*)data,
						((ExifRawAppSeg *)(*appIter))->getLength());
				}
				#endif

				EkString dataString = convertRawDataToString(data,(*appIter)->getLength());
				PmtTranslationTableElements::iterator ttIter = theTable->begin();
				while(ttIter != theTable->end())
				{
					if(!strcmp((*ttIter)->mLocation.c_str(),PMT_EXIF_XMP_LOC))
					{
						PmtMetadataPtr md = rootMd->getMetadatum((*ttIter)->mKey);
						if (md)
							md->setValueStr(dataString);
					}
					ttIter++;
				}
			}
		}
		appIter++;
	}
}

#ifdef PMT_XMP_ACCESSOR

bool PmtExifAccessor::SaveXMPValues()
{
	//mImageHandle->
	if(!mXMPAccessor.Flush())
		return false;
	
	vector<ExifAppSegment*> appSegs = mImageHandle->getAllAppSegs();

	//Remove all the XMP segments
	vector<ExifAppSegment*>::iterator appIter = appSegs.begin();
	uint16 nFirstMarker = 0;
	//Remove all the XMP packet and insert the new XMP packet.
	while (appIter != appSegs.end())
	{
		if (!strcmp((*appIter)->getAppIdent(),PMT_ADOBE_XMP_IDENTIFIER))
		{
			uint16 nMarker = (*appIter)->getAppSegmentMarker();
			//Write the XMP packet to the first XMP segment.
			if((nFirstMarker == 0) && (mnXMPOffset >=0) && (mnXMPLastBytes > 0))
			{
				nFirstMarker = nMarker;
				ExifRawAppSeg* pRawSeg = (ExifRawAppSeg *)(*appIter);
				char * data = pRawSeg->getRawData();

				int nLength = mXMPAccessor.XMPBlockSize() +  pRawSeg->getLength() - (mnXMPLastBytes - mnXMPOffset);
				char *pWriteBytes = new char[nLength];
				memcpy(pWriteBytes, data, mnXMPOffset);
				memcpy(pWriteBytes + mnXMPOffset, mXMPAccessor.XMPBlockData(), mXMPAccessor.XMPBlockSize());
				memcpy(pWriteBytes + mnXMPOffset + mXMPAccessor.XMPBlockSize(), data + mnXMPLastBytes, pRawSeg->getLength() - mnXMPLastBytes);

				//Update length of the xmp segment
				pWriteBytes[0] = nLength >> 8;
				pWriteBytes[1] = nLength & 0xff;
				ExifRawAppSeg xmpRawData(nFirstMarker, PMT_ADOBE_XMP_IDENTIFIER);
				xmpRawData.setRawData(pWriteBytes,nLength);

				mImageHandle->removeAppSeg(nFirstMarker, PMT_ADOBE_XMP_IDENTIFIER);
				mImageHandle->insertAppSeg(&xmpRawData);
				delete[] pWriteBytes;
			}
			else
				mImageHandle->removeAppSeg(nMarker, PMT_ADOBE_XMP_IDENTIFIER);
		}
		appIter++;
	}

	
	return true;
}
#endif //End of PMT_XMP_ACCESSOR

#endif


PmtMetadataPtr PmtExifAccessor::readMetadata(const SPmtString nsURI)
{
    // Make sure any schema and translation table changes are handled
    loadIfAny();

    PmtMetadataPtr rootMd = PmtMetadata::create(nsURI);
    
    // Prevent deletion of the translation table while we're using it.
    // (shouldn't this just be some kind of protected counter?)
    EK_GUARD(EK_DEFAULT_MUTEX, PmtTranslationTable::msLock);
    
    // Get the translation table column for this accessor
    PmtTranslationTableElements * theTable = 
                         msTranslationTable->getSubTable(PMT_EXIF_ELEMENT_NAME);

    if (theTable != NULL)
    {
#if defined HAVE_KIESEXIF        
        unsigned int size = theTable->size();
        unsigned int i = 0;

        // loop through all the elements in the column and query the underlying
        // toolkit for the appropriate data (shouldn't this be the other way
        // around -- query toolkit and then insert it appropriately?)
        for (i = 0; i<size; i++)
        {
            PmtTranslationTableElement * tableElement = (*theTable)[i];

            bool success = true;
            
            ExifTagEntry tagEntry;
            unsigned int tagEntryCount;


            if (atoi(tableElement->mTag.c_str()) >= NON_TAGGED_METADATA)
            {
                //Non-tagged metadata has Entry Tag >= NON_TAGGED_METADATA
                success = readNonTaggedMetadata(tableElement->mType,
                                                tableElement->mTag,tagEntry);
            }
            else
            {
                success = getTagValue(
                        (ExifTagLocation) getLocation(tableElement->mLocation),
                            getTag(tableElement->mTag), tagEntry);
            }

            if (success)
            {
                PmtTranslatorPtr translator =
                    PmtTranslator::create(tableElement->mTranslator);
                if (translator)
                {
                    tagEntryCount = tagEntry.count;
                    PmtMetadataPtr md = rootMd->getMetadatum(tableElement->mKey);
                    if (md)
                    {
                        //Some keys can be SHORT or LONG in the spec but are 
                        //defined as ulong in the translation table. Therefore,
                        //we have to check if we are reading a SHORT that we 
                        //expect to be LONG
                        if ((tagEntry.tagtype == 3) && 
                            (tableElement->mType == "ulong"))
                        {
                            unsigned int *temp =
                                new unsigned int (*(unsigned short *)tagEntry.tagValue.sShortValue);
                            translator->assign(md.ptr(), (void *&) (temp),
                                               tagEntryCount, true);
                            delete[]tagEntry.tagValue.StringValue;
                        }
                        else
                        {
                            translator->assign(md.ptr(),
                                        (void *&) (tagEntry.tagValue.StringValue),
                                        tagEntryCount, true);
                        }
                    }
                    else
                    {
                        SPmtString m =
                            "PmtExifAccessor::readMetadata Could not get metadata object: ";
                        m += tableElement->mKey;

                        if (mThrowErrors)
                            throw PmtError(PMT_TRANSLATION_MD_OBJECT_NOT_FOUND,
                                           m, WHERE);
                        else
                        mErrors.
                            push_back(PmtError
                                      (PMT_TRANSLATION_MD_OBJECT_NOT_FOUND, m,
                                       WHERE));
                    }
                    tagEntry.count = static_cast<unsigned int>(tagEntryCount);
                }
                else
                {
                    SPmtString m = 
                 "PmtExifAccessor::readMetadata Can't create translator for ";
                    m += tableElement->mTranslator;
                    if (mThrowErrors)
                        throw PmtError(PMT_TRANSLATOR_NOT_FOUND, m, WHERE);
                    else
                    mErrors.
                        push_back(PmtError
                                  (PMT_TRANSLATOR_NOT_FOUND, m, WHERE));
                }
            }
        }
        
        // Check if Width and Height got filled in. If they didn't, then
        // fill them in with the value in the JPEG marker.
        PmtMetadataPtr mdHandle = rootMd->getMetadatum("ImageContainer.Height",
            false);
        if (mdHandle.ptr() == NULL)
        {
            mdHandle = rootMd->getMetadatum("ImageContainer.Height", true);
            PmtMetadataT<unsigned long>* mdT = (PmtMetadataT<unsigned long>*)
                mdHandle.ptr();
            mdT->value() = mImageInfo.imageHeight;
            
        }
        mdHandle = rootMd->getMetadatum("ImageContainer.Width", false);
        if (mdHandle.ptr() == NULL)
        {
            mdHandle = rootMd->getMetadatum("ImageContainer.Width", true);
            PmtMetadataT<unsigned long>* mdT = (PmtMetadataT<unsigned long>*)
                mdHandle.ptr();
            mdT->value() = mImageInfo.imageWidth;
        }
#endif                          // HAVE_KIESEXIF

#if defined HAVE_OPENEXIF
        // Get all the App0 (JFIF) metadata entries in the file
        getJFIFValues(rootMd);
        
        // Get all the App1 metadata entries in the file
        readTags(0xFFE1, "Exif","APP1_IFD",rootMd,theTable);

        // Get all the App3 metadata entries in the file
        readTags(0xFFE3, "Meta","APP3_IFD",rootMd,theTable);
        
        // get the Q tables
        getQTables(rootMd);
        
        // Fill in SOF metadata values
        getSOFValues(rootMd);
        
		// Read any Adobe XMP segments
		getXMPDocs(rootMd,theTable);

        // Check if Width and Height got filled in. If they didn't, then
        // fill them in with the value in the JPEG marker.
        ExifImageInfo imgInfo;
        if( mImageHandle->getImageInfo(imgInfo) == EXIF_OK )
		{
			PmtMetadataPtr mdHandle = rootMd->getMetadatum("ImageContainer.Height", true);
			PmtMetadataT<unsigned long>* mdT = (PmtMetadataT<unsigned long>*) mdHandle.ptr();
			if (mdT != NULL)
			{
				//If the height was stored incorrectly in the EXIF metadata,
				// reset it to what's in the SOF
				if ((mdT->value() > imgInfo.height+16)||(mdT->value() < imgInfo.height-16))
					mdT->value() = imgInfo.height;
			}

			mdHandle = rootMd->getMetadatum("ImageContainer.Width", true);
			mdT = (PmtMetadataT<unsigned long>*) mdHandle.ptr();
			if (mdT != NULL)
			{
				//If the width was stored incorrectly in the EXIF metadata,
				// reset it to what's in the SOF
				if ((mdT->value() > imgInfo.width+16)||(mdT->value() < imgInfo.width-16))
					mdT->value() = imgInfo.width;
	            
			}
		}

#ifdef PMT_XMP_ACCESSOR
		mXMPAccessor.readMetadata(rootMd,true);
#endif
#endif                          // HAVE_OPENEXIF

    }
    return rootMd;
}

#if defined HAVE_OPENEXIF
// Get the SOF related values and set them in the rootMd
void PmtExifAccessor::getSOFValues(PmtMetadataPtr rootMd)
{
    ExifImageInfo imgInfo;
    if( mImageHandle->getImageInfo(imgInfo) == EXIF_OK )
    {
        PmtMetadataPtr md;
        
        md = rootMd->getMetadatum("DigitalProcess.Jpeg.SOF.Height");
        if( md.ptr() != NULL )
        {
            PmtMetadataT<unsigned long>* mdT = 
                (PmtMetadataT<unsigned long>*) md.ptr();
            mdT->value() = imgInfo.height;
        }
        
        md = rootMd->getMetadatum("DigitalProcess.Jpeg.SOF.Width");
        if( md.ptr() != NULL )
        {
            PmtMetadataT<unsigned long>* mdT = 
                (PmtMetadataT<unsigned long>*) md.ptr();
            mdT->value() = imgInfo.width;
        }
    
        md = rootMd->getMetadatum("DigitalProcess.Jpeg.SOF.Components");
        if( md.ptr() != NULL )
        {
            PmtMetadataT<unsigned short>* mdT = 
                (PmtMetadataT<unsigned short>*) md.ptr();
            mdT->value() = imgInfo.numChannels;
        }
        
        md = rootMd->getMetadatum("DigitalProcess.Jpeg.SOF.Precision");
        if( md.ptr() != NULL )
        {
            PmtMetadataT<unsigned short>* mdT = 
                (PmtMetadataT<unsigned short>*) md.ptr();
            mdT->value() = imgInfo.precision;
        }
        
        md = rootMd->getMetadatum("DigitalProcess.Jpeg.SOF.HorizontalSampling");
        if( md.ptr() != NULL )
        {
            int numComponents = imgInfo.numChannels;
            vector<uint8> value;
            for( int i = 1; i <= numComponents; i++ )
                value.push_back(mImageHandle->getHSamplingFactor(i));
            
            PmtMetadataT<vector<uint8> >* mdT = 
                (PmtMetadataT<vector<uint8> >*) md.ptr();
            mdT->value() = value;
        }
        
        md = rootMd->getMetadatum("DigitalProcess.Jpeg.SOF.VerticalSampling");
        if( md.ptr() != NULL )
        {
            int numComponents = imgInfo.numChannels;
            vector<uint8> value;
            for( int i = 1; i <= numComponents; i++ )
                value.push_back(mImageHandle->getVSamplingFactor(i));
            
            PmtMetadataT<vector<uint8> >* mdT = 
                (PmtMetadataT<vector<uint8> >*) md.ptr();
            mdT->value() = value;
        }
        
        md = rootMd->getMetadatum("DigitalProcess.Jpeg.SOF.ChrominanceSampling");
        if( md.ptr() != NULL )
        {
            EkString value = EK_L("UNKNOWN");
            uint8 h = mImageHandle->getHSamplingFactor(1);
            uint8 v = mImageHandle->getVSamplingFactor(1);
            
            if(h==2)
            {
                if(v==2)
                    value = EK_L("4:2:0");
                else if(v==1)
                    value = EK_L("4:2:2");
            }
            else if(h==1)
            {
                if(v==1)
                    value = EK_L("4:4:4");
            }
            
            md->setValueStr(value);
        }
    }
    
    
}

// Get all the Q Tables in the image file and set them in the rootMd
void PmtExifAccessor::getQTables(PmtMetadataPtr rootMd)
{
    ExifJpegQuantTable* qTable = new ExifJpegQuantTable;

    ExifTagEntry* entry;
    vector<unsigned char> qTableValue;

	// I really don't want to know why we're creating a translation table 
	// element here.  Maybe we can make it magically go away soon....
    PmtTranslationTableElement* tableElement = 
        new PmtTranslationTableElement( "DigitalProcess.Jpeg.LuminQTable",
		"",
        DIGITALPROCESS_JPEG_LUMINQTABLE, "uchar", "UNDEFINED_LOCATION",
        "builtin2uchar" );

    if ( mImageHandle->getJpegQTable(0, qTable) == EXIF_OK )
    {
        // load Q0 into table and set appropriate metadata
        for(int i=0;i<64;i++)
            qTableValue.push_back( (const unsigned char)qTable->quantizer[i] );
        
        entry = new ExifTagEntryT< vector<unsigned char> >(
            atoi(DIGITALPROCESS_JPEG_ALPHAQTABLE), EXIF_BYTE, 64, qTableValue );
        getValueFromEntry(tableElement, entry, rootMd);
        delete entry;
        qTableValue.clear();
    }
    
    if ( mImageHandle->getJpegQTable(1, qTable) == EXIF_OK )
    {
        // load Q1 into table and set appropriate metadata
        for(int i=0;i<64;i++)
            qTableValue.push_back( (const unsigned char)qTable->quantizer[i] );
        
        entry = new ExifTagEntryT< vector<unsigned char> >(
            atoi(DIGITALPROCESS_JPEG_CHROMA1QTABLE), EXIF_BYTE,64,qTableValue);
        tableElement->mKey = "DigitalProcess.Jpeg.Chroma1QTable";
        tableElement->mTag = DIGITALPROCESS_JPEG_CHROMA1QTABLE;
        getValueFromEntry(tableElement, entry, rootMd);
        delete entry;
        qTableValue.clear();
    }
    
    if ( mImageHandle->getJpegQTable(2, qTable) == EXIF_OK )
    {
        // load Q2 into table and set appropriate metadata
        for(int i=0;i<64;i++)
            qTableValue.push_back( (const unsigned char)qTable->quantizer[i] );
        
        entry = new ExifTagEntryT< vector<unsigned char> >(
            atoi(DIGITALPROCESS_JPEG_CHROMA2QTABLE),EXIF_BYTE,64,qTableValue);
        tableElement->mKey = "DigitalProcess.Jpeg.Chroma2QTable";
        tableElement->mTag = DIGITALPROCESS_JPEG_CHROMA2QTABLE;
        getValueFromEntry(tableElement, entry, rootMd);
        delete entry;
        qTableValue.clear();
    }

    if ( mImageHandle->getJpegQTable(3, qTable) == EXIF_OK )
    {
        // load Q3 into table and set appropriate metadata
        for(int i=0;i<64;i++)
            qTableValue.push_back( (const unsigned char)qTable->quantizer[i] );
        
        entry = new ExifTagEntryT< vector<unsigned char> >(
            atoi(DIGITALPROCESS_JPEG_LUMINQTABLE), EXIF_BYTE, 64, qTableValue );
        tableElement->mKey = "DigitalProcess.Jpeg.AlphaQTable";
        tableElement->mTag = DIGITALPROCESS_JPEG_ALPHAQTABLE;
        getValueFromEntry(tableElement, entry, rootMd);
        delete entry;
        qTableValue.clear();
    }

    delete tableElement;
    delete qTable;
}

// Get the specified SOF entry and return it
ExifTagEntry* PmtExifAccessor::getSOFEntry( const char * tag )
{
    ExifTagEntry * returnValue = NULL;
    int tagNum = atoi(tag);

    if( tagNum == DIGITALPROCESS_JPEG_CHROMINANCESAMPLING )
    {
        string value = "UNKNOWN";
        uint8 h = mImageHandle->getHSamplingFactor(1);
        uint8 v = mImageHandle->getVSamplingFactor(1);

        if(h==2)
        {
            if(v==2)
                value = "4:2:0";
            else if(v==1)
                value = "4:2:2";
        }
        else if(h==1)
        {
            if(v==1)
                value = "4:4:4";
        }

        returnValue = new ExifTagEntryT<string>( NON_TAGGED_METADATA,
                EXIF_ASCII, 6, value );
    }
    else if( tagNum == DIGITALPROCESS_JPEG_COMPONENTS )
    {
        ExifImageInfo imgInfo;
        if( mImageHandle->getImageInfo(imgInfo) == EXIF_OK )
        {
            returnValue = new ExifTagEntryT<uint16>( NON_TAGGED_METADATA,
                EXIF_SHORT, 1, imgInfo.numChannels );
        }
    }
    else if( tagNum == DIGITALPROCESS_JPEG_HORIZONTALSAMPLING )
    {
        ExifImageInfo imgInfo;
        if( mImageHandle->getImageInfo(imgInfo) == EXIF_OK )
        {
            vector<unsigned char> value;
            int numComponents = imgInfo.numChannels;
            for( int i = 1; i <= numComponents; i++ )
                value.push_back(mImageHandle->getHSamplingFactor(i));
            returnValue = new ExifTagEntryT<vector<uint8> >(
                NON_TAGGED_METADATA, EXIF_BYTE, numComponents, value);
        }
    }
    else if( tagNum == DIGITALPROCESS_JPEG_VERTICALSAMPLING )
    {
        ExifImageInfo imgInfo;
        if( mImageHandle->getImageInfo(imgInfo) == EXIF_OK )
        {
            vector<unsigned char> value;
            int numComponents = imgInfo.numChannels;
            for( int i = 1; i <= numComponents; i++ )
                value.push_back(mImageHandle->getVSamplingFactor(i));
            returnValue = new ExifTagEntryT<vector<uint8> >(
                NON_TAGGED_METADATA, EXIF_BYTE, numComponents, value);
        }
    }
    else if( tagNum == DIGITALPROCESS_JPEG_HEIGHT )
    {
        ExifImageInfo imgInfo;
        if( mImageHandle->getImageInfo(imgInfo) == EXIF_OK )
        {
            returnValue = new ExifTagEntryT<exif_uint32>( 
                NON_TAGGED_METADATA, EXIF_LONG, 1, imgInfo.height );
        }
    }
    else if( tagNum == DIGITALPROCESS_JPEG_PRECISION )
    {
        ExifImageInfo imgInfo;
        if( mImageHandle->getImageInfo(imgInfo) == EXIF_OK )
        {
            returnValue = new ExifTagEntryT<uint16>( NON_TAGGED_METADATA,
                EXIF_SHORT, 1, imgInfo.precision );
        }
    }
    else if( tagNum == DIGITALPROCESS_JPEG_WIDTH )
    {
        ExifImageInfo imgInfo;
        if( mImageHandle->getImageInfo(imgInfo) == EXIF_OK )
        {
            returnValue = new ExifTagEntryT<exif_uint32>( 
                NON_TAGGED_METADATA, EXIF_LONG, 1, imgInfo.width );
        }
    }
    
    return returnValue;
}

// Get the specified Q Table and return it within an ExifTagEntry
ExifTagEntry* PmtExifAccessor::getQTableEntry( const char * tag )
{
    ExifTagEntry * returnValue = NULL;

    ExifJpegQuantTable* qTable = new ExifJpegQuantTable;

    vector<unsigned char> qTableValue;
    
    int tagNum = atoi(tag);
    if ( tagNum == atoi(DIGITALPROCESS_JPEG_ALPHAQTABLE) )
    {
        // We want Q0
        if ( mImageHandle->getJpegQTable(0,qTable) == EXIF_OK )
        {
            for(int i=0;i<64;i++)
              qTableValue.push_back((const unsigned char)qTable->quantizer[i]);
            returnValue = new ExifTagEntryT< vector<uint8> >(
                NON_TAGGED_METADATA,EXIF_BYTE,64,qTableValue );
        }
    }
    else if ( tagNum == atoi(DIGITALPROCESS_JPEG_CHROMA1QTABLE) )
    {
        // We want Q1
        if ( mImageHandle->getJpegQTable(1,qTable) == EXIF_OK )
        {
            for(int i=0;i<64;i++)
               qTableValue.push_back((const unsigned char)qTable->quantizer[i]);
            returnValue = new ExifTagEntryT< vector<uint8> >(
                NON_TAGGED_METADATA,EXIF_BYTE,64,qTableValue);   
        }
    }
    else if ( tagNum == atoi(DIGITALPROCESS_JPEG_CHROMA2QTABLE) )
    {
        // We want Q2
        if ( mImageHandle->getJpegQTable(2,qTable) == EXIF_OK )
        {
            for(int i=0;i<64;i++)
               qTableValue.push_back((const unsigned char)qTable->quantizer[i]);
            returnValue = new ExifTagEntryT< vector<uint8> >(
                NON_TAGGED_METADATA,EXIF_BYTE,64,qTableValue);
        }
    }
    else if ( tagNum == atoi(DIGITALPROCESS_JPEG_LUMINQTABLE) )
    {
        // We want Q3
        if ( mImageHandle->getJpegQTable(3,qTable) == EXIF_OK )
        {
            for(int i=0;i<64;i++)
              qTableValue.push_back((const unsigned char)qTable->quantizer[i]);
            returnValue = new ExifTagEntryT< vector<uint8> >(
                NON_TAGGED_METADATA,EXIF_BYTE,64,qTableValue );
        }
    }
    
    delete qTable;
    
    return returnValue;
}

void PmtExifAccessor::getJFIFValues(PmtMetadataPtr rootMd)
{
    ExifAppSegment* appSeg = mImageHandle->getAppSeg( 0xFFE0, "JFIF", true );
    if (appSeg != NULL)
    {
        ExifApp0Seg * app0Seg = dynamic_cast<ExifApp0Seg *>(appSeg);
        if (app0Seg != NULL)
        {
            PmtMetadataPtr md = rootMd->getMetadatum(
                                    "ImageContainer.DisplayResolutionUnit");
            if( md.ptr() != NULL )
            {
                PmtMetadataT<unsigned char>* mdT = 
                    (PmtMetadataT<unsigned char>*) md.ptr();
                mdT->value() = app0Seg->unitsId();
            }
            
            md = rootMd->getMetadatum("ImageContainer.DisplayXResolution");
            if( md.ptr() != NULL )
            {
                PmtMetadataT<float>* mdT = 
                    (PmtMetadataT<float>*) md.ptr();
                mdT->value() = app0Seg->xDensity();
            }
            
            md = rootMd->getMetadatum("ImageContainer.DisplayYResolution");
            if( md.ptr() != NULL )
            {
                PmtMetadataT<float>* mdT = 
                    (PmtMetadataT<float>*) md.ptr();
                mdT->value() = app0Seg->yDensity();
            }

            // Not sure if I like this ... this won't report a metadata
            // value of 0 if it actually does exist in the file.
            if (app0Seg->yThumbnail() > 0)
            {
                md = rootMd->getMetadatum("ImageContainer.Thumbnail.Height");
                if( md.ptr() != NULL )
                {
                    PmtMetadataT<float>* mdT = 
                        (PmtMetadataT<float>*) md.ptr();
                    mdT->value() = app0Seg->yThumbnail();
                }
            }

            // Not sure if I like this ... this won't report a metadata
            // value of 0 if it actually does exist in the file.
            if (app0Seg->xThumbnail() > 0)
            {
                md = rootMd->getMetadatum("ImageContainer.Thumbnail.Width");
                if( md.ptr() != NULL )
                {
                    PmtMetadataT<float>* mdT = 
                        (PmtMetadataT<float>*) md.ptr();
                    mdT->value() = app0Seg->xThumbnail();
                }
            }
        }
    }
}

// process all the entries within an IFD
void PmtExifAccessor::processEntries(SPmtString location,
    ExifTags ifdTags, PmtTranslationTableElements* theTable,
    PmtMetadataPtr rootMd)
{
    if (location != "")
    {
        unsigned int size = theTable->size();
        int tagNumber;
        unsigned int counter;

        ExifTagsIter crnt = ifdTags.begin();
        ExifTagsIter end = ifdTags.end();
                
        ExifTagEntry* entry = NULL;

        while( crnt != end )
        {
            entry = *(crnt);
            tagNumber = entry->getTagNum();

            // find the appropriate item
            counter = 0; // reset the counter
            while ((counter<size) && 
                !((atoi((*theTable)[mTtPosition]->mTag.c_str()) == tagNumber) &&
                ((*theTable)[mTtPosition]->mLocation == location)))
            {
                mTtPosition++;
                counter++;
                if ( mTtPosition == size )
                    mTtPosition = 0;
            }
                    
            PmtTranslationTableElement* tableElement = NULL;
                    
            // get the values as required
            if ( counter<theTable->size() )
            {
                tableElement = (*theTable)[mTtPosition];
                        
                getValueFromEntry(tableElement,entry,rootMd);
                
                mTtPosition++;
				if ( mTtPosition == size )
					mTtPosition = 0;
				while( (atoi((*theTable)[mTtPosition]->mTag.c_str()) == tagNumber)
                    && ((*theTable)[mTtPosition]->mLocation == location))
                {
                    tableElement = (*theTable)[mTtPosition];
                        
                    getValueFromEntry(tableElement,entry,rootMd);
                    
                    mTtPosition++;
					if ( mTtPosition == size )
						mTtPosition = 0;
                }
            }

            crnt++;
        }
    }
}

// process an entry (get its value from the entry and set it to the md object)
void PmtExifAccessor::getValueFromEntry(PmtTranslationTableElement* tableElement,
    ExifTagEntry* entry, PmtMetadataPtr rootMd)
{
    string translatorName = tableElement->mTranslator;
    if ( translatorName == "builtin2ascii")
        translatorName = "builtin2string";
    PmtTranslatorPtr translator =
        PmtTranslator::create(translatorName);
    if (translator)
    {
        void *value = NULL;
        int count = 0;
        switch (entry->getType())
        {
        case EXIF_ASCII:
            {
            const char * c = dynamic_cast<ExifTagEntryT<string> *>(entry)->
                                                            getValue().c_str();
            PmtMetadataPtr md = rootMd->getMetadatum(tableElement->mKey);
            if (md->getMDType() == PMT_MD_TYPE_STRING)
            {
                md->setValueStr(EK_STR2WIDE(c));
            }
            else
            {
                count = entry->getCount();
                value = new char[count];
                memcpy(value,c,count);
            }
				md->setURI(tableElement->mUri);
            }
            break;
        case EXIF_SBYTE:
        case EXIF_UNDEFINED:
            getEntryData(entry, (char *&) value, count);
            break;
        case EXIF_BYTE:
            getEntryData(entry, (unsigned char *&) value, count);
            break;
        case EXIF_SSHORT:
            getEntryData(entry, (short *&) value, count);
            break;
        case EXIF_SHORT:
            getEntryData(entry, (unsigned short *&) value, count);
            break;
        case EXIF_SLONG:
            getEntryData(entry, (int *&) value, count);
            break;
        case EXIF_LONG:
            getEntryData(entry, (unsigned int *&) value, count);
            break;
        case EXIF_SRATIONAL:
        case EXIF_RATIONAL:
        case EXIF_FLOAT:
            getEntryData(entry, (float *&) value, count);
            break;
        case EXIF_DOUBLE:
            getEntryData(entry, (double *&) value, count);
            break;
        default:
            SPmtString m = 
                "PmtExifAccessor::getValueFromEntry: invalid Exif type ";
            m += entry->getType();
            mErrors.push_back(PmtError(PMT_ERROR, m, WHERE));
        }
        
        if ( value != NULL )
        {
            PmtMetadataPtr md = rootMd->getMetadatum(tableElement->mKey);
            if( md.ptr() != NULL )
            {
                //Some keys can be SHORT or LONG in the spec but are defined
                //as ulong in the translation table. Therefore, we have to 
                //check if we are reading a SHORT that we expect to be LONG
                if ((entry->getType() == EXIF_SHORT) && 
                    (tableElement->mType == "ulong"))
                {
                    unsigned int *temp;
                    temp = new unsigned int (*(unsigned short *) value);
                    translator->assign(md.ptr(), (void *&) (temp), 
                                    (unsigned int &) count, true);
                    delete[] (short *)value;
                }
                else
                {
                    translator->assign(md.ptr(), value, (unsigned int &) count,
                                                                         true);
                }
				md->setURI(tableElement->mUri);
            }
        }
    }
    else
    {
        SPmtString m =
            "PmtExifAccessor::readMetadata Can't create translator for ";
        m += tableElement->mTranslator;
        mErrors.push_back(PmtError(PMT_TRANSLATOR_NOT_FOUND, m, WHERE));
    }
}
#endif

// Get the value from the ExifTagEntry in the underlying EXIF toolkit and set
// it to md
bool PmtExifAccessor::getMdValue(PmtMetadataPtr & md)
{
    bool success = true; // the return value
    SPmtString loc, tag, type, translatorName;
    if ( getFormatInfo(md, loc, tag, type, translatorName) )
    {
#if defined HAVE_KIESEXIF
    bool done = false;  // indicates that we already set value
    ExifTagEntry tagEntry;
    
    if (atoi(tag.c_str()) >= NON_TAGGED_METADATA)
    {
        //Non-tagged metadata has Entry Tag >= NON_TAGGED_METADATA
        success = readNonTaggedMetadata(type, tag, tagEntry);
    }
    else if (md->fullKey() == "ImageContainer.Height")
    {
        // For ImageContainer.Height, we first look at tag 257, if it is
        // not present then we look at 40963, and if this one isn't present
        // either, then we look at the JPEG marker that contains the value
        
        success = getTagValue((ExifTagLocation)getLocation("APP1_IFD0"), 257,
                                tagEntry);
        if (!success)
        {
            success = getTagValue((ExifTagLocation)getLocation(
                "APP1_IFD0.EXIF_IFD"), 40963, tagEntry);
                
            if(!success)
            {
                PmtMetadataT<unsigned long>* mdT = 
                    (PmtMetadataT<unsigned long>*) md.ptr();
                mdT->value() = mImageInfo.imageHeight;
                done = true;
            }
        }
    }
    else if  (md->fullKey() == "ImageContainer.Width") 
    {
        // For ImageContainer.Width, we first look at tag 256, if it is
        // not present then we look at 40962, and if this one isn't present
        // either, then we look at the JPEG marker that contains the value
        
        success = getTagValue((ExifTagLocation)getLocation("APP1_IFD0"), 256,
                                tagEntry);
        if (!success)
        {
            success = getTagValue((ExifTagLocation)getLocation(
                "APP1_IFD0.EXIF_IFD"), 40962, tagEntry);
            
            if(!success)
            {
                PmtMetadataT<unsigned long>* mdT = 
                    (PmtMetadataT<unsigned long>*) md.ptr();
                mdT->value() = mImageInfo.imageWidth;
                done = true;
            }
        }
       
    }
    else
    {
        success = getTagValue((ExifTagLocation) getLocation(loc),
                              getTag(tag), tagEntry);
    }

    if ( success && (!done) )
    {
        PmtTranslatorPtr translator = PmtTranslator::create(translatorName);
        if (translator)
        {
            unsigned int tagEntryCount = tagEntry.count;
            
            //Some keys can be SHORT or LONG in the spec but are defined
            //as ulong in the translation table. Therefore, we have to 
            //check if we are reading a SHORT that we expect to be LONG
            if (tagEntry.tagtype == 3 && type == "ulong")
            {
                unsigned int *temp = new unsigned int (*(unsigned short *)tagEntry.tagValue.sShortValue);
                translator->assign(md.ptr(), (void *&) (temp), tagEntryCount,
                                   true);
                delete[] tagEntry.tagValue.StringValue;
            }
            else
            {
                translator->assign(md.ptr(),
                                   (void *&) (tagEntry.tagValue.StringValue),
                                    tagEntryCount, true);
            }
            tagEntry.count = static_cast<unsigned int>(tagEntryCount);
        }
        else
            success = false;
    }
#endif                          // HAVE_KIESEXIF

#if defined HAVE_OPENEXIF
    // KIES uses the builtin2ascii translator, but OpenExif uses the 
    // builtin2string
    if (translatorName == "builtin2ascii")
        translatorName = "builtin2string";

    ExifTagEntry *entry = NULL;
    bool iOwnEntry = false;
    if (type == PMT_EXIF_AUDIO_TYPE)
    {
        success = readAudio(*entry);
    }
    else
    {
        int exifTagNumber = atoi(tag.c_str());
        if ( exifTagNumber >= NON_TAGGED_METADATA )
        {
            if (exifTagNumber < PMT_EXIF_AUDIO_TAG)
                // need to create a Q Table or SOF entry
                entry = getQTableEntry( tag.c_str() );
            
            else if (exifTagNumber == DIGITALPROCESS_JPEG_COMMSEG)
                entry = getCommSegments();
                
            else 
                entry = getSOFEntry(tag.c_str());
            
            iOwnEntry = true;
            
        }
        else if (md->fullKey() == "ImageContainer.Height")
        {
            // For ImageContainer.Height, we first look at tag 257, if it is
            // not present then we look at 40963, and if this one isn't present
            // either, then we look at the JPEG marker that contains the value
            
            entry = getTagValue((ExifTagLocation)getLocation("APP1_IFD0"), 257);
            if (entry == NULL)
            {
                entry = getTagValue((ExifTagLocation)getLocation(
                    "APP1_IFD0.EXIF_IFD"), 40963);
                    
                if(entry == NULL)
                {
                    ExifImageInfo imgInfo;
                    if ( mImageHandle->getImageInfo(imgInfo) == EXIF_OK )
                    {
                        entry = new ExifTagEntryT<unsigned int>(0,EXIF_LONG,
                            1, imgInfo.height);
                    }
                }
            }
        }
        else if  (md->fullKey() == "ImageContainer.Width") 
        {
            // For ImageContainer.Width, we first look at tag 256, if it is
            // not present then we look at 40962, and if this one isn't present
            // either, then we look at the JPEG marker that contains the value
            
            entry = getTagValue((ExifTagLocation)getLocation("APP1_IFD0"), 256);
            if (entry == NULL)
            {
                entry = getTagValue((ExifTagLocation)getLocation(
                    "APP1_IFD0.EXIF_IFD"), 40962);
                
                if(entry == NULL)
                {
                    ExifImageInfo imgInfo;
                    if ( mImageHandle->getImageInfo(imgInfo) == EXIF_OK )
                    {
                        entry = new ExifTagEntryT<unsigned int>(0,EXIF_LONG,
                            1, imgInfo.width);
                    }
                }
            }
           
        }
        else
        {
            entry = getTagValue((ExifTagLocation) getLocation(loc), 
                getTag(tag));
        }

        if ( entry != NULL )
        {
            PmtTranslatorPtr translator = PmtTranslator::create(translatorName);
            if (translator)
            {
                void *value = NULL;
                int count;
                switch (entry->getType())
                {
                    case EXIF_ASCII:
                    {
                        value =
                            new SPmtString(dynamic_cast < ExifTagEntryT < 
                                string > *>(entry)->getValue().c_str());
                        break;
                    }
                    case EXIF_SBYTE:
                    case EXIF_UNDEFINED:
                        getEntryData(entry, (char *&) value, count);
                        break;
                    case EXIF_BYTE:
                        getEntryData(entry, (unsigned char *&) value, count);
                        break;
                    case EXIF_SSHORT:
                        getEntryData(entry, (short *&) value, count);
                        break;
                    case EXIF_SHORT:
                        getEntryData(entry, (unsigned short *&) value, count);
                        break;
                    case EXIF_SLONG:
                        getEntryData(entry, (int *&) value, count);
                        break;
                    case EXIF_LONG:
                        getEntryData(entry, (unsigned int *&) value, count);
                        break;
                    case EXIF_SRATIONAL:
                    case EXIF_RATIONAL:
                    case EXIF_FLOAT:
                        getEntryData(entry, (float *&) value, count);
                        break;
                    case EXIF_DOUBLE:
                        getEntryData(entry, (double *&) value, count);
                        break;
                    default:
                        success = false;
                }
                
                if ( success && (value!=NULL))
                {
                    //Some keys can be SHORT or LONG in the spec but are defined
                    //as ulong in the translation table. Therefore, we have to 
                    //check if we are reading a SHORT that we expect to be LONG
                    if ((entry->getType() == 3) && (type == "ulong"))
                    {
                        unsigned int *temp;
                        temp = new unsigned int (*(unsigned short *) value);
                        translator->assign(md.ptr(), (void *&) (temp),
                                           (unsigned int &) count, true);
                        delete[] (short *)value;
                    }
                    else
                    {
                        translator->assign(md.ptr(), value, 
                                           (unsigned int &) count, true);
                    }
                }
            }
            else
                success = false;
            
            if ( iOwnEntry && entry!=NULL )
                delete entry;
        }
        else
            success = false;
    }

#ifdef PMT_XMP_ACCESSOR
	if(success == false)
		success = mXMPAccessor.getMdValue(md);
#endif

#endif                          //HAVE_OPENEXIF
    }
    else
        success = false;


    return success;
}

// Set the value in md to an ExifTagEntry and pass it to the underlying
// EXIF toolkit
bool PmtExifAccessor::setMdValue(const PmtMetadataPtr & md)
{
    bool success = true; // the return value
    SPmtString loc, tag, type, translatorName;
    if ( getFormatInfo(md, loc, tag, type, translatorName) )
    {

#if defined HAVE_KIESEXIF
    ExifTagEntry tagEntry;
    PmtTranslatorPtr translator = PmtTranslator::create(translatorName);
    if (translator && (getTag(tag) < NON_TAGGED_METADATA))
    {
        unsigned int tagEntryCount = tagEntry.count;
        translator->assign(md.ptr(), (void *&) (tagEntry.tagValue.StringValue),
                           tagEntryCount, false);
        tagEntry.count = static_cast<unsigned int>(tagEntryCount);

        if (type == PMT_EXIF_AUDIO_TYPE)
        {
            success = writeAudio(tagEntry);
        }
        else
        {
            tagEntry.tagnum = getTag(tag);
            tagEntry.tagtype = getType(type);
            tagEntry.tagLocation = (ExifTagLocation) getLocation(loc);
            success = setTagValue(tagEntry);
        }

        // delete the mem allocated by translator->assign
        delete tagEntry.tagValue.StringValue;
    }
    else
        success = false;
#endif                          // HAVE_KIESEXIF

#if defined HAVE_OPENEXIF
    // KIES uses the builtin2ascii translator, but OpenExif uses the 
    // builtin2string
    if (translatorName == "builtin2ascii")
        translatorName = "builtin2string";

#ifdef PMT_XMP_ACCESSOR
	mXMPAccessor.setMdValue(md);
#endif
	

    PmtTranslatorPtr translator = PmtTranslator::create(translatorName);
    unsigned int tagNum = getTag(tag);
    if(tagNum >= NON_TAGGED_METADATA)
    {
        ExifJpegQuantTable* qTable = new ExifJpegQuantTable;
        
        if (tagNum == (unsigned int) atoi(DIGITALPROCESS_JPEG_ALPHAQTABLE))
        {
            PmtMetadataT< vector<unsigned char> >* mdT = 
                (PmtMetadataT< vector<unsigned char> >*)md.ptr();
            vector<unsigned char> val = mdT->value();
            if( val.size() >= 64 )
            {
                for (int i=0; i<64; i++)
                {
                    qTable->quantizer[i] = val[i];
                }
                mImageHandle->setJpegQTable( 0, qTable );
            }
        }
        else if (tagNum == (unsigned int) atoi(DIGITALPROCESS_JPEG_CHROMA1QTABLE))
        {
            PmtMetadataT< vector<unsigned char> >* mdT = 
                (PmtMetadataT< vector<unsigned char> >*)md.ptr();
            vector<unsigned char> val = mdT->value();
            if( val.size() >= 64 )
            {
                for (int i=0; i<64; i++)
                {
                    qTable->quantizer[i] = val[i];
                }
                mImageHandle->setJpegQTable( 1, qTable );
            }
        }
        else if (tagNum == (unsigned int) atoi(DIGITALPROCESS_JPEG_CHROMA2QTABLE))
        {
            PmtMetadataT< vector<unsigned char> >* mdT = 
                (PmtMetadataT< vector<unsigned char> >*)md.ptr();
            vector<unsigned char> val = mdT->value();
            if( val.size() >= 64 )
            {
                for (int i=0; i<64; i++)
                {
                    qTable->quantizer[i] = val[i];
                }
                mImageHandle->setJpegQTable( 2, qTable );
            }
        }
        else if (tagNum == (unsigned int) atoi(DIGITALPROCESS_JPEG_LUMINQTABLE))
        {
            PmtMetadataT< vector<unsigned char> >* mdT = 
                (PmtMetadataT< vector<unsigned char> >*)md.ptr();
            vector<unsigned char> val = mdT->value();
            if( val.size() >= 64 )
            {
                for (int i=0; i<64; i++)
                {
                    qTable->quantizer[i] = val[i];
                }
                mImageHandle->setJpegQTable( 3, qTable );
            }
        }
        
        delete qTable;
    }
    else if (translator)
    {
        void *value = NULL;
        unsigned int count;
        translator->assign(md.ptr(), value, count, false);
        
        unsigned short enumType = 0;
        try
        {
            enumType = getType(type);
        }
        catch (PmtError e)
        {
            success = false;
        }

        ExifTagEntry *entry = NULL;
        
        if ( success )
        {
            if (type == PMT_EXIF_AUDIO_TYPE)
            {
                success = writeAudio(*entry);
                delete (char *) value;
            }
            else
            {
                switch (enumType)
                {
                    case EXIF_ASCII:
                    {
                        entry = new ExifTagEntryT < string > 
                            (tagNum, enumType);
                        
                        SPmtString *pstr = (SPmtString *) (value);
                        ((ExifTagEntryT<string>*)entry)->
                                                    setValue(pstr->c_str());
                        entry->setCount(pstr->size() + 1);
                        delete pstr;
                        break;
                    }
                    case EXIF_SBYTE:
                    case EXIF_UNDEFINED:
                        if (count > 1)
                            entry = new ExifTagEntryT < vector < char > > 
                                (tagNum, enumType);
                        else
                            entry = new ExifTagEntryT < char >
                                (tagNum, enumType);
                        setEntryData(entry, (char *) value, count);
                        break;
                    case EXIF_BYTE:
                        if (count > 1)
                            entry = new ExifTagEntryT < vector < unsigned char >
                                > (tagNum, enumType);
                        else
                            entry = new ExifTagEntryT < unsigned char >
                                (tagNum, enumType);
                                
                        setEntryData(entry, (unsigned char *) value, count);
                        break;
                    case EXIF_SSHORT:
                        if (count > 1)
                            entry = new ExifTagEntryT < vector < short > >
                                (tagNum, enumType);
                        else
                            entry = new ExifTagEntryT < short >(tagNum,
                                enumType);
                                
                        setEntryData(entry, (short *) value, count);
                        break;
                    case EXIF_SHORT:
                        if (count > 1)
                            entry = new ExifTagEntryT < vector < 
                                unsigned short > > (tagNum, enumType);
                        else
                            entry = new ExifTagEntryT < unsigned short >
                                (tagNum, enumType);
                                
                        setEntryData(entry, (unsigned short *) value, count);
                        break;
                    case EXIF_SLONG:
                        if (count > 1)
                            entry = new ExifTagEntryT < vector < int > >
                                (tagNum, enumType);
                        else
                            entry = new ExifTagEntryT < int >
                                (tagNum, enumType);
                                
                        setEntryData(entry, (int *) value, count);
                        break;
                    case EXIF_LONG:
                        if (count > 1)
                            entry = new ExifTagEntryT < vector < 
                                unsigned int > > (tagNum, enumType);
                        else
                            entry = new ExifTagEntryT < unsigned int >
                                (tagNum, enumType);
                                
                        setEntryData(entry, (unsigned int *) value, count);
                        break;
                    case EXIF_SRATIONAL:
                    case EXIF_RATIONAL:
                    case EXIF_FLOAT:
                        if (count > 1)
                            entry = new ExifTagEntryT < vector < float > >
                                (tagNum, enumType);
                        else
                            entry = new ExifTagEntryT < float >
                                (tagNum, enumType);
                                
                        setEntryData(entry, (float *) value, count);
                        break;
                    case EXIF_DOUBLE:
                        if (count > 1)
                            entry = new ExifTagEntryT < vector < double > >
                                (tagNum, enumType);
                        else
                            entry = new ExifTagEntryT < double >
                                (tagNum, enumType);
                            
                        setEntryData(entry, (double *) value, count);
                        break;
                    default:
                        success = false;
                        delete (char *) value;
                }
                
                setTagValue((ExifTagLocation) getLocation(loc), *entry);
            }
            
        }

        if (entry)
            delete entry;
    }
    else
        success = false;
#endif                          // HAVE_OPENEXIF
    }
    else
	{
		// We shouldn't hit this case unless someone entered a bad
		// metadata key that we don't know about.  SceneContent.Faces.x could be
		// one of the keys, so we'll explicitely check for that before
		// returning an error.
		const PmtCompositeMetadata * faces = md->parent();
		while ((faces!=NULL)&&(!faces->fullKey().compare("SceneContent.Faces")))
			faces = faces->parent();

		if (faces == NULL)
			success = false;
		else
		{
			PmtMetadataPtr f = (PmtMetadata *)faces;
			success = setMdValue(f);

			// Faces may require App3 creation (if it's not
			// already there.) so Let's add the manadatory tags
			PmtMetadataPtr m = f->getRootMetadata()->getMetadatum("ImageContainer.MetadataNumber",false);
			if (!m)
			{
				m = f->getRootMetadata()->getMetadatum("ImageContainer.MetadataNumber",false);
				m->setValueStr(EK_L("0110"));
				setMdValue(m);
			}
		}
	}   
    return success;
}


void PmtExifAccessor::removeMdValue(const PmtMetadataPtr & md)
{
    SPmtString loc, tag, type, translatorName;
    if ( getFormatInfo(md, loc, tag, type, translatorName) )
    {
#ifdef HAVE_OPENEXIF

#ifdef PMT_XMP_ACCESSOR
		mXMPAccessor.removeMdValue(md);
#endif  //end of PMT_XMP_ACCESSOR
		
        if( mImageHandle!=NULL )
            mImageHandle->removeGenericTag( getTag(tag), 
                                        (ExifTagLocation)getLocation(loc));
#endif

#ifdef HAVE_KIESEXIF
        // NIFEXIF_RemoveGenericTag(mImageHandle, 
        //          (ExifTagLocation)getLocation(loc), getTag(tag) );
#endif
    }
}

#ifdef HAVE_KIESEXIF
bool PmtExifAccessor::getTagValue(ExifTagLocation tagLocation,
                                  unsigned short tagNumber,
                                  ExifTagEntry & tagEntry) const
{
    bool success = true; // the return value
    NIFStatus status =
        NIFEXIF_GetGenericTag(mImageHandle, tagLocation, tagNumber, &tagEntry);

    if (status != NIF_OK)
        success = false;
        
    return success;
}
#endif                          // HAVE_KIESEXIF

#if defined HAVE_OPENEXIF
ExifTagEntry *PmtExifAccessor::getTagValue(ExifTagLocation loc,
                                           unsigned short tagNumber) const
{
    ExifStatus status;
    ExifTagEntry* returnValue = NULL;
    if ( mImageHandle != NULL )
        returnValue = mImageHandle->getGenericTag(tagNumber, loc, status);
    
    return returnValue;
}
#endif                          //HAVE_OPENEXIF

#if defined HAVE_KIESEXIF
bool PmtExifAccessor::setTagValue(ExifTagEntry & tagEntry) const
{
    bool success = true; // the return value
    tagEntry.tagAction = EXIF_TAG_MODIFY;

    NIFStatus status = NIFEXIF_SetGenericTag(mImageHandle, &tagEntry);

    if (status != NIF_OK)
        success = false;
        
    return success;
}
#endif                          // HAVE_KIESEXIF

#if defined HAVE_OPENEXIF
bool PmtExifAccessor::setTagValue(ExifTagLocation loc,
                                  const ExifTagEntry & tagEntry)
{
    bool success = true; // the return value
    if (mImageHandle->setGenericTag(tagEntry, loc) != EXIF_OK)
        success = false;
        
    return success;
}
#endif                          // HAVE_OPENEXIF


bool PmtExifAccessor::readAudio(ExifTagEntry & tagEntry) const
{
    bool success = true; // the return value
#if defined HAVE_KIESEXIF
    NIFbool audioExists;
    NIFStatus status = NIFEXIF_DoesAudioExist(mImageHandle, &audioExists);

    if ((status == NIF_OK) && audioExists)
    {
        status = NIFEXIF_GetAudioStreamLength(mImageHandle, &tagEntry.count);
        if (status == NIF_OK)
        {
            tagEntry.tagValue.StringValue = new unsigned char[tagEntry.count];

            UInt32 amountRead;
            UInt32 amountToRead(tagEntry.count);
            unsigned char *startPtr = tagEntry.tagValue.StringValue;
            do
            {
                // Note: cast of amountRead to ULng32 is invalid for 
                // recent versions of libexif_io.a
                status =
                    NIFEXIF_ReadAudioStream(mImageHandle, startPtr,
                                            amountToRead, (UInt32*)&amountRead);
                amountToRead -= amountRead;
                startPtr += amountRead;
            }
            while ((status == NIF_OK) && amountToRead);
        }
        else
            success = false;
    }
    else
        success = false;
#endif

#if defined HAVE_OPENEXIF
    success = false;
#endif                          // HAVE_OPENEXIF
    return success;
}


bool PmtExifAccessor::writeAudio(ExifTagEntry & tagEntry) const
{
    bool success = true; // the return value
#if defined HAVE_KIESEXIF
    UInt32 amountWritten;
    UInt32 amountToWrite(tagEntry.count);
    unsigned char *startPtr = tagEntry.tagValue.StringValue;
    NIFStatus status;
    do
    {
        // Note: cast of amountWritten to ULng32 is invalid for 
        // recent versions of libexif_io.a
        status =
            NIFEXIF_WriteAudioStream(mImageHandle, startPtr, amountToWrite,
                                     (UInt32*)&amountWritten);
        amountToWrite -= amountWritten;
        startPtr += amountWritten;
    }
    while ((status == NIF_OK) && amountToWrite);
    
    if ( status != NIF_OK )
        success = false;
#endif                          // HAVE_KIESEXIF

#if defined HAVE_OPENEXIF
    success = false;
#endif                          // HAVE_OPENEXIF

    return success;
}

#ifdef HAVE_KIESEXIF
/**************************************************************************
* getQTable reads and returns the quantization tables used to compress the 
* image.  There are four quantization tables defined in the PMT MDL, 
* DIGITALPROCESS_JPEG_ALPHAQTABLE, DIGITALPROCESS_JPEG_CHROMA1QTABLE,
* DIGITALPROCESS_JPEG_CHROMA2QTABLE, and DIGITALPROCESS_JPEG_LUMINQTABLE.
*
* getQTable reads the tables from the image using the underlying 
* toolkits. The requested Q table data is returned to the calling program 
* using the ExifTagEntry.tagValue.StringValue and .count fields
***************************************************************************/
bool PmtExifAccessor::getQTable( SPmtString tag,
                                  ExifTagEntry & tagEntry ) const
{
    bool success = false;

    NIFJPEGQuantTable * Q0= NULL;
    NIFJPEGQuantTable * Q1= NULL;
    NIFJPEGQuantTable * Q2= NULL;
    NIFJPEGQuantTable * Q3= NULL;

    //KIES reads all tables at once, only returns the table requested
    if (NIFEXIF_GetJPEGTables(mImageHandle, Q0, Q1, Q2, Q3) == NIF_OK)
    {
        success = true;
        tagEntry.count = 64;
        tagEntry.tagValue.StringValue = new unsigned char[tagEntry.count];

        // 
        if ((tag == DIGITALPROCESS_JPEG_ALPHAQTABLE) 
             && (Q3 != NULL))
        {
            //Load Q0 data into tag Entry
            for (int i=0; i<64; i++)
                tagEntry.tagValue.StringValue[i] = Q3->quantizer[i];
        }
        else if ((tag == DIGITALPROCESS_JPEG_CHROMA1QTABLE) 
                  && (Q1 != NULL))
        {
            //Load Q1 data into tag Entry
            for (int i=0; i<64; i++)
                tagEntry.tagValue.StringValue[i] = Q1->quantizer[i];
        }
        else if ((tag == DIGITALPROCESS_JPEG_CHROMA2QTABLE) 
                  && (Q2 != NULL))
        {
            //Load Q2 data into tag Entry
            for (int i=0; i<64; i++)
                tagEntry.tagValue.StringValue[i] = Q2->quantizer[i];
        }
        else if ((tag == DIGITALPROCESS_JPEG_LUMINQTABLE) 
                  && (Q0 != NULL))
        {
            //Load Q3 data into tag Entry
            for (int i=0; i<64; i++)
                tagEntry.tagValue.StringValue[i] = Q0->quantizer[i];
        }
        else {
			delete tagEntry.tagValue.StringValue;
            success = false;
		}

        if(Q0)
	        delete [] Q0;
        if(Q1)
	        delete [] Q1;
        if(Q2)
	        delete [] Q2;
        if(Q3)
	        delete [] Q3;

    }

    return success;
}
#endif

bool PmtExifAccessor::getFormatInfo(PmtMetadataPtr md, SPmtString & loc,
                                    SPmtString & tag, SPmtString & type,
                                    SPmtString & translator)
{
    bool success = false; // the return value
    if( md.ptr() != NULL )
    {
        PmtKeyType key = md->fullKey(false);

        // Prevent deletion of the translation table while we're using it.
        // (shouldn't this just be some kind of protected counter?)
        EK_GUARD(EK_DEFAULT_MUTEX, PmtTranslationTable::msLock);

        // Get the translation table column for this accessor
        PmtTranslationTableElements * theTable = 
            msTranslationTable->getSubTable(PMT_EXIF_ELEMENT_NAME);

        if (theTable != NULL)
        {
            unsigned int i = 0;

            PmtTranslationTableElement * tableElement = NULL;

            // find the appropriate item
            while ((i<theTable->size()) && 
                   ((*theTable)[i]->mKey != key )) i++;

            // get the values as required
            if ( i<theTable->size() )
            {
                tableElement = (*theTable)[i];
                loc = tableElement->mLocation;
                tag = tableElement->mTag;
                type = tableElement->mType;
                translator = tableElement->mTranslator;
				md->setURI(tableElement->mUri);
				success = true;
            }
        }
    }
    return success;
}


unsigned int PmtExifAccessor::getTag(const SPmtString & stag) const
{
    return (unsigned int) (atoi(stag.c_str()));
}


unsigned short PmtExifAccessor::getType(const SPmtString & stype) const
{
    if (stype == "ascii")
        return EXIF_ASCII;
    else if (stype == "byte")
        return EXIF_SBYTE;
    else if (stype == "ubyte")
        return EXIF_BYTE;
    else if (stype == "short")
        return EXIF_SSHORT;
    else if (stype == "ushort")
        return EXIF_SHORT;
    else if (stype == "long")
        return EXIF_SLONG;
    else if (stype == "ulong")
        return EXIF_LONG;
    else if (stype == "rational")
        return EXIF_SRATIONAL;
    else if (stype == "urational")
        return EXIF_RATIONAL;
    else if (stype == "float")
        return EXIF_FLOAT;
    else if (stype == "double")
        return EXIF_DOUBLE;
    else if (stype == "undefined")
        return EXIF_UNDEFINED;
    else if (stype == "audio")
        return EXIF_UNDEFINED;
    else if (stype == "face")
        return EXIF_UNDEFINED;
    else
    {
        SPmtString m = "PmtExifAccessor::getType: invalid exif type for ";
        m += stype;
        throw PmtError(PMT_EXIF_FORMAT_ENTRY_BAD_TAG_TYPE, m, WHERE);
    }
}


unsigned short PmtExifAccessor::getLocation(const SPmtString & sloc) const
{
	unsigned short ret = 0;
    if (sloc == "APP1_IFD0")
        ret = EXIF_APP1_IFD0;
    else if (sloc == "APP1_IFD0.EXIF_IFD")
        ret = EXIF_APP1_EXIFIFD;
    else if (sloc == "APP1_IFD1")
        ret = EXIF_APP1_IFD1;
    else if (sloc == "APP1_IFD0.GPSINFO_IFD")
        ret = EXIF_APP1_IFD0_GPSINFO_IFD;
    else if (sloc == "APP1_IFD0.EXIF_IFD.INTEROP_IFD")
        ret = EXIF_APP1_EXIFINTEROP_IFD;
    else if (sloc == "APP3_IFD0")
        ret = EXIF_APP3_IFD0;
    else if (sloc == "APP4_IFD0")
        ret = EXIF_APP4_IFD0;
    else if (sloc == "APP3_IFD0:ERIMM")
        ret = EXIF_APP3_DELTAIMAGE_ERIMM_IFD;
    else if (sloc == "APP3_IFD0:ROMM8")
        ret = EXIF_APP3_DELTAIMAGE_ROMM8_IFD;
    else if (sloc == "APP3_IFD0:ROMM12")
        ret = EXIF_APP3_DELTAIMAGE_ROMM12_IFD;
    else if (sloc == "APP4_IFD0:ERIMM")
        ret = EXIF_APP4_DELTAIMAGE_ERIMM_IFD;
    else if (sloc == "APP4_IFD0:ROMM8")
        ret = EXIF_APP4_DELTAIMAGE_ROMM8_IFD;
    else if (sloc == "APP4_IFD0:ROMM12" || sloc == "APP3_AUTHENTICATIONIFD")
        ret = EXIF_APP4_DELTAIMAGE_ROMM12_IFD;
    else if (sloc == "APP5_1_IFD0")
        ret = EXIF_APP5_KPRO1_IFD0;
    else if (sloc == "APP5_2_IFD0")
        ret = EXIF_APP5_KPRO1_IFD0;
    else if (sloc == "APP3_IFD0.FACES_IFD")
        ret = EXIF_APP3_IFD0_FACE_IFD;
    else
    {
        SPmtString m =
            "PmtExifAccessor::getLocation: invalid exif location for ";
        m += sloc;
        throw PmtError(PMT_EXIF_FORMAT_ENTRY_BAD_TAG_LOCATION, m, WHERE);
    }
	return ret;
}

#if ((defined PMT_IMAGE_ACCESSOR_REQUIRED) && (defined HAVE_OPENEXIF))
// This method checks to see if we have an open file and if so returns an
// image accessor for it. Otherwise, it returns NULL
PmtImageAccessor *PmtExifAccessor::getImageAccessor()
{
    // Check to see if we have an open file
    if (mImageHandle!=NULL)
    {
        // Check if we already have an image accessor for this file. 
        // Create it if necessary.
        if (!mImgAccessor)
            mImgAccessor = new PmtExifImageAccessor(mImageHandle);
        else
            mImgAccessor->setImageFile(mImageHandle);
    }
    else
        mImgAccessor = NULL;

    return mImgAccessor;
}

#endif

#endif  // have either kies or open exif libraries.

