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
 * Creation Date: 7/18/00
 *
 * Original Author: 
 * Chris Lin <ti.lin@kodak.com> 
 * 
 * Contributor(s):
 * Ricardo Rosario  ricardo.rosario@kodak.com
 */

#ifndef PMT_EXIF_ACCESSOR__H
#define PMT_EXIF_ACCESSOR__H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#include "PmtCommon.h"
#if defined(HAVE_KIESEXIF) || defined(HAVE_OPENEXIF)

#ifdef HAVE_KIESEXIF
#include "ExifTags.h"
#include "NIFJpegExifIO.h"
#endif

#ifdef HAVE_OPENEXIF

#include "ExifImageFile.h"
#include "ExifTagEntry.h"
#include "ExifTags.h"

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED
#include "PmtExifImageAccessor.h"
#endif // PMT_IMAGE_ACCESSORS_REQUIRED

#endif // HAVE_OPENEXIF

#include "PmtAccrImplDefs.h"
#include "PmtAccessor.h"

#include "PmtAtom.h"

#ifdef PMT_XMP_ACCESSOR
#include "PmtXMPAccessor.h"
#endif
class PmtExifAccessor;

typedef EkSmartPtr < PmtExifAccessor > PmtExifAccessorPtr;
typedef EkConstSmartPtr < PmtExifAccessor > ConstPmtExifAccessorPtr;


/*!Provides access to metadata stored in Exif formatted image files.

  One should never have to include this header file, nor use this 
  interface directly. All access to this class' functionality should
  be through the PmtAccessor base class interface. 
*/
class PMTACCR_IMPL_DECL PmtExifAccessor:public PmtAccessor
{
  public:

    //! Default constructor
    inline PmtExifAccessor():PmtAccessor(PMT_FORMAT_EXIF), mImageHandle(NULL),
#if ((defined PMT_IMAGE_ACCESSOR_REQUIRED) && (defined HAVE_OPENEXIF))
        mImgAccessor(NULL),
#endif
        mOwnImgHandle(false), mClosing(false), mTtPosition(0),mnXMPOffset(-1),mnXMPLastBytes(-1)
    {
    }

#if defined HAVE_KIESEXIF
    /*! 
        The constructor which takes the image handle returned by
        NIFEXIF_OpenByFileName().
     */
    inline PmtExifAccessor(NIFExifImageHandle *
                           imgHndl):PmtAccessor(PMT_FORMAT_EXIF),
        mImageHandle(imgHndl), mOwnImgHandle(false), mClosing(false), 
        mTtPosition(0)
    {
    }
#endif

    //! The copy constructor
    inline PmtExifAccessor(const PmtExifAccessor &
                           src):PmtAccessor(PMT_FORMAT_EXIF),
                           mImageHandle(src.mImageHandle), 
#if ((defined PMT_IMAGE_ACCESSOR_REQUIRED) && (defined HAVE_OPENEXIF))
        mImgAccessor(NULL),
#endif                
        mOwnImgHandle(false), mClosing(false), mTtPosition(0)
    {
    }

    //! Destructor
    virtual ~ PmtExifAccessor(void);

    //! Opens an Exif image file for reading / writing (depending on the mode)
    virtual bool open(const char *name, PmtOpenMode mode = PMT_READONLY);

    //! Close the Exif file.
    virtual void close();

    //! Return a copy of the PmtExifAccessor.
    inline virtual PmtAccessorPtr clone() const
    {
        return new PmtExifAccessor();
    }

    virtual PmtMetadataPtr readMetadata(const SPmtString nsURI = "");

#if ((defined PMT_IMAGE_ACCESSOR_REQUIRED) && (defined HAVE_OPENEXIF))
    /*! Returns a PmtExifImageAccessor for the image.
    
        The image must have been opened before calling this function.
     */
    virtual PmtImageAccessor *getImageAccessor();
#endif

#ifdef HAVE_KIESEXIF
    //! This method reads metadata stored in non-standard locations
    bool PmtExifAccessor::readNonTaggedMetadata(
        SPmtString type, SPmtString tag,
        ExifTagEntry & tagEntry);
#endif

    //! Check if the given file is an Exif file.
    static bool recognize(const SPmtString & name);


  protected:

    virtual bool getMdValue(PmtMetadataPtr & md);

    virtual bool setMdValue(const PmtMetadataPtr & md);

    // Remove the metadata specified by md from the file.
    virtual void removeMdValue(const PmtMetadataPtr & md);

  private:

#if defined HAVE_KIESEXIF
    bool getTagValue(ExifTagLocation tagLocation, unsigned short tagNumber,
                     ExifTagEntry & tagEntry) const;

    bool setTagValue(ExifTagEntry & tagEntry) const;

    bool getQTable(SPmtString tag, ExifTagEntry & tagEntry) const;
#endif


#if defined HAVE_OPENEXIF
    ExifTagEntry *getTagValue(ExifTagLocation loc,
                              unsigned short tagNumber) const;
    bool setTagValue(ExifTagLocation loc, const ExifTagEntry & tagEntry);
    
    //! Helper function used by readMetadata() for reading all Q tables
    void getQTables(PmtMetadataPtr rootMd);
    
    //! Helper function used by readMetadata() for reading SOF data
    void getSOFValues(PmtMetadataPtr rootMd);
    
    //! Helper method used by getMdValue() for reading a specific Q table
    ExifTagEntry* getQTableEntry( const char * tag );
    
    //! Helper method used by getMdValue() for reading a SOF entry
    ExifTagEntry* getSOFEntry( const char * tag );

	//! Helper method used by getMdValue() for reading any Adobe XMP segments
	void getXMPDocs(PmtMetadataPtr rootMd, 
		PmtTranslationTableElements* theTable);

	//! Helper method used by getMdValue() for reading comm segments
    ExifTagEntry* getCommSegments(  ) { return NULL; };

    //! Helper function used by readMetadata() for getting the JFIF data
    void getJFIFValues(PmtMetadataPtr rootMd);

    /*! Process all the entries within an IFD
        
        This is a helper method for readMetadata()
     */
    void processEntries( SPmtString location, ExifTags ifdTags,
        PmtTranslationTableElements* theTable, PmtMetadataPtr rootMd );
    
    /*! Get the value from an entry and set in the md object.
    
        This is a helper method for readMetadata()
     */
    void getValueFromEntry(PmtTranslationTableElement* tableElement,
        ExifTagEntry* entry, PmtMetadataPtr rootMd);

    void readTags(unsigned short appKey, 
                  SPmtString appKeyString,
                  SPmtString appLocation,
                  PmtMetadataPtr rootMd,
                  PmtTranslationTableElements * theTable);
#endif                          // HAVE_OPENEXIF

    bool readAudio(ExifTagEntry & tagEntry) const;

    bool writeAudio(ExifTagEntry & tagEntry) const;

    bool getFormatInfo(PmtMetadataPtr md, SPmtString & loc, SPmtString & tag,
                       SPmtString &, SPmtString & translator);

    unsigned int   getTag(const SPmtString & stag) const;
    unsigned short getType(const SPmtString & stype) const;
    unsigned short getLocation(const SPmtString & sloc) const;

#if defined HAVE_KIESEXIF
    NIFExifImageHandle *mImageHandle;
    NIFExifImageInfo mImageInfo;
#endif

#if defined HAVE_OPENEXIF
    ExifImageFile *mImageHandle;
#endif

#if ((defined PMT_IMAGE_ACCESSOR_REQUIRED) && (defined HAVE_OPENEXIF))
    // An image accessor of the open image file
    PmtExifImageAccessor *mImgAccessor;
#endif

    bool mOwnImgHandle;

    bool mClosing;

    unsigned int mTtPosition;
    
#if defined HAVE_OPENEXIF
    // The data buffer alocated here will be deleted in
    // PmtFormatType::up/downAssignToMd
    template < class T >
        inline void
        getEntryData(const ExifTagEntry * entry, T * &data, int &count) const
    {
        count = entry->getCount();
        data = new T[count];
        if (count == 1)
        {
            *data = (( const ExifTagEntryT < T > *)(entry))->getValue();
        }
        else if (count > 1)
        {
            const vector < T > &vals =
                (( const ExifTagEntryT < vector < T > > *)(entry))->getValue();
            copy(vals.begin(), vals.end(), data);
        }
    }


    template < class T >
        inline void
        setEntryData(ExifTagEntry * entry, T * data, int count) const
    {
        entry->setCount(count);
        if (count == 1)
        {
            (( ExifTagEntryT < T > *)(entry))->setValue(*data);
            delete data;
        }
        else if (count > 1)
        {
            vector < T > &vals =
                (( ExifTagEntryT < vector < T > > *)(entry))->getValue();
            copy(data, data + count,
                 back_insert_iterator < vector < T > >(vals));
            delete [] data;
        }
    }

#if defined PMT_XMP_ACCESSOR
		PmtXMPAccessor mXMPAccessor;
		int mnXMPOffset;
		int mnXMPLastBytes;
		PmtOpenMode openMode;
		bool SaveXMPValues();
#endif // PMT_XMP_ACCESSOR

#endif                          // HAVE_OPENEXIF
};


//PMTACCR_IMPL_REQUIRE(PmtExifAccessor)
#endif  // have either kies or open exif libraries.

#endif                          // PMT_EXIF_ACCESSOR__H

