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
 * Creation Date: 3/12/03
 *
 * Original Author: 
 * Sam Fryer <samuel.fryer@kodak.com> 
 * 
 * Contributor(s): 
 * 
 */

#ifndef PMT_QT_ACCESSOR__H
#define PMT_QT_ACCESSOR__H

#include "PmtConfig.h"
#include "PmtCommon.h"
#include "PmtAccrImplDefs.h"
#include "PmtAccessor.h"
#include "PmtQtAtoms.h"

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
#include "PmtQtImageAccessor.h"
#endif                          // PMT_IMAGE_ACCESSORS_REQUIRED

#ifdef PMT_SOUND_ACCESSOR_REQUIRED
#include "PmtQtSoundAccessor.h"
#endif                          // PMT_SOUND_ACCESSORS_REQUIRED

#ifdef PMT_XMP_ACCESSOR
#include "PmtXMPAccessor.h"
#endif

class PmtQTAccessor;
class PmtQTAccessorData;

typedef EkSmartPtr < PmtQTAccessor > PmtQTAccessorPtr;
typedef EkConstSmartPtr < PmtQTAccessor > ConstPmtQTAccessorPtr;


//! Provides access to metadata stored in QT formatted image files. 
/*! One should never have to include this header file, nor use this 
  interface directly. All access to this class' functionality should
  be through the PmtAccessor base class interface. 
*/
class PMTACCR_IMPL_DECL PmtQTAccessor:public PmtAccessor
{
  public:

    //! Default constructor
    PmtQTAccessor();

    //! The constructor which takes the image FILE handle.
//        PmtQTAccessor( FILE* img );

    //! The copy constructor
    PmtQTAccessor(const PmtQTAccessor & src);

    //! Destructor
     virtual ~ PmtQTAccessor(void);

    //! Opens a QT image file for reading / writing (depending on the mode)
    virtual bool open(const char *name, PmtOpenMode mode = PMT_READONLY);

    //! Close an QT image file.
    virtual void close();

    //! Return a copy of the PmtQTAccessor.
    inline virtual PmtAccessorPtr clone() const
    {
        return new PmtQTAccessor();
    }

    virtual PmtMetadataPtr readMetadata(const SPmtString nsURI = "");

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
    //! Returns a PmtQTImageAccessor for the image. The image must have
    // been opened before calling this function.
    virtual PmtImageAccessor *getImageAccessor();
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
#ifdef PMT_SOUND_ACCESSOR_REQUIRED
    //! Returns a PmtQTImageAccessor for the image. The image must have
    // been opened before calling this function.
    virtual PmtSoundAccessorPtr getSoundAccessor();
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED

    // Check if the given file is an QT file.
    static bool recognize(const SPmtString & name);

  protected:
  
    virtual bool getMdValue(PmtMetadataPtr & md);

    virtual bool setMdValue(const PmtMetadataPtr & md);

  private:


    FILE * mQTFile;

    std::vector < PmtAtomPtr > mAtoms;


#ifdef PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
    // An image accessor of the open image file
    PmtQTImageAccessor *mImgAccessor;
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET

#ifdef PMT_SOUND_ACCESSOR_REQUIRED
    // An image accessor of the open image file
    PmtSoundAccessorPtr mSndAccessor;
#endif                          // PMT_SOUND_ACCESSOR_REQUIRED

    bool mClosing;
    
#ifdef PMT_XMP_ACCESSOR
	PmtXMPAccessor mXmpAccessor;
#endif

    
    bool parseQuickTimeFile(FILE *f, PmtMetadataPtr rootMd,
						    PmtTranslationTableElements * theTTable);
    
    static void loadAtomFactory();
};

//PMTACCR_IMPL_REQUIRE(PmtQTAccessor)
#endif                          // PMT_QT_ACCESSOR__H

