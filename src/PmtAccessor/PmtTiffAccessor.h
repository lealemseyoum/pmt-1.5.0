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
 * Ricardo Rosario  <ricardo.rosario@kodak.com>
 */

#ifndef PMT_TIFF_ACCESSOR__H
#define PMT_TIFF_ACCESSOR__H

#include "PmtCommon.h"
#include "PmtAccrImplDefs.h"
#include "PmtAccessor.h"

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED
#include "PmtTiffImageAccessor.h"
#endif // PMT_IMAGE_ACCESSORS_REQUIRED

#include "TiffImageFile.h"
#include "TiffTagEntry.h"

#ifdef HAVE_EKTIFF
#define PMTTIFF_INT32 long
#define PMTTIFF_UINT32 unsigned long
#else 
#ifdef HAVE_OPENTIFF
#define PMTTIFF_INT32 tiff_int32
#define PMTTIFF_UINT32 tiff_uint32

#endif
#endif


class PmtTiffAccessor;

typedef EkSmartPtr < PmtTiffAccessor > PmtTiffAccessorPtr;
typedef EkConstSmartPtr < PmtTiffAccessor > ConstPmtTiffAccessorPtr;


/*! Provides access to metadata stored in TIFF formatted image files. 

  One should never have to include this header file, nor use this 
  interface directly. All access to this class' functionality should
  be through the PmtAccessor base class interface. 
*/
class PMTACCR_IMPL_DECL PmtTiffAccessor:public PmtAccessor
{
  public:

    //! Default constructor
    inline PmtTiffAccessor():PmtAccessor(PMT_FORMAT_TIFF), mImgFile(NULL),
#ifdef PMT_IMAGE_ACCESSOR_REQUIRED
        mImgAccessor(NULL),
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED
        mOwnImgFile(false), mClosing(false)
    {
    }

    //! The constructor which takes the image handle from the Open Tiff Toolkit. 
    inline PmtTiffAccessor(TiffImageFile * img):PmtAccessor(PMT_FORMAT_TIFF),
        mImgFile(img),
#ifdef PMT_IMAGE_ACCESSOR_REQUIRED
     mImgAccessor(NULL),
#endif
        // PMT_IMAGE_ACCESSOR_REQUIRED
     mOwnImgFile(false), mClosing(false)
    {
    }

    //! The copy constructor
    inline PmtTiffAccessor(const PmtTiffAccessor &
                           src):PmtAccessor(PMT_FORMAT_TIFF),
        mImgFile(src.mImgFile),
#ifdef PMT_IMAGE_ACCESSOR_REQUIRED
     mImgAccessor(NULL),
#endif
        // PMT_IMAGE_ACCESSOR_REQUIRED
     mOwnImgFile(false), mClosing(false)
    {
    }

    //! Destructor
    virtual ~ PmtTiffAccessor(void);

    //! Opens a TIFF image file for reading / writing (depending on the mode)
    virtual bool open(const char *name, PmtOpenMode mode = PMT_READONLY);

    //! Close an Tiff image file.
    virtual void close();

    //! Return a copy of the PmtTiffAccessor.
    inline virtual PmtAccessorPtr clone() const
    {
        return new PmtTiffAccessor();
    }

    virtual PmtMetadataPtr readMetadata(const SPmtString nsURI = "");

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED
    /*! Returns a PmtTiffImageAccessor for the image. The image must have
        been opened before calling this function.
     */
    virtual PmtImageAccessor *getImageAccessor();
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED

    //! Check if the given file is an Tiff file.
    static bool recognize(const SPmtString & name);

  protected:
    
    /*! Get the value from the TiffImageFile and set it to the Metadata object
        passed in.
        
        \return true if it was successful, otherwise false
     */
    virtual bool getMdValue(PmtMetadataPtr & md);

    /*! Set the value in the Metadata object to the TiffImageFile
    
        \return true if it was successful, otherwise false
     */
    virtual bool setMdValue(const PmtMetadataPtr & md);

    //! Remove the metadata specified by md from the file.
    virtual void removeMdValue(const PmtMetadataPtr & md);

  private:

    TiffTagEntry * getTagValue(const SPmtString & loc,
                               unsigned short tagNumber) const;

    bool setTagValue(const SPmtString & loc, const TiffTagEntry & tagEntry);

    /*! Read the audio
    
        \return true if it was successful, otherwise false
     */
    bool readAudio(const SPmtString & loc, PmtMetadataPtr md) const;

    /*! Write the audio
    
        \return true if it was successful, otherwise false
     */
    bool writeAudio(const SPmtString & loc, PmtMetadataPtr md) const;

    TiffImageFile::DirPath getDirPath(const SPmtString & loc) const;

    bool getFormatInfo(PmtMetadataPtr md, SPmtString & loc, SPmtString & tag,
                       SPmtString &, SPmtString & translator);

    unsigned short getTag(const SPmtString & stag) const;
    unsigned short getType(const SPmtString & stype) const;

    TiffImageFile *mImgFile;

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED
    // An image accessor of the open image file
    PmtTiffImageAccessor *mImgAccessor;
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED

    bool mOwnImgFile;

    bool mClosing;

    // The data buffer alocated here will be deleted in PmtFormatType::up/downAssignToMd
    template < class T >
    inline void
    getEntryData(const TiffTagEntry * entry, T * &data, int &count) const
    {
        count = entry->getCount();
        data = new T[count];
        if (count == 1)
        {
            *data = (( const TiffTagEntryT < T > *)(entry))->getValue();
        }
        else if (count > 1)
        {
            const vector < T > &vals =
                (( const TiffTagEntryT < vector < T > >*)(entry))->getValue();
            copy(vals.begin(), vals.end(), data);
        }
    }


    template < class T >
    inline void
    setEntryData(TiffTagEntry * entry, T * data, int count) const
    {
        entry->setCount(count);
        if (count == 1)
        {
            (( TiffTagEntryT < T > *)(entry))->setValue(*data);
            delete data;
        }
        else if (count > 1)
        {
            vector < T > &vals =
                (( TiffTagEntryT < vector < T > >*)(entry))->getValue();
            copy(data, data + count,
                 back_insert_iterator < vector < T > >(vals));
            delete [] data;
        }
    }

};

//PMTACCR_IMPL_REQUIRE(PmtTiffAccessor)
#endif                          // PMT_TIFF_ACCESSOR__H

