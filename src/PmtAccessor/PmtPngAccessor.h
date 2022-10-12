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

#ifndef PMT_PNG_ACCESSOR__H
#define PMT_PNG_ACCESSOR__H

#include "PmtCommon.h"
#include "PmtAccrImplDefs.h"
#include "PmtAccessor.h"

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
#include "PmtPngImageAccessor.h"
#endif                          // PMT_IMAGE_ACCESSORS_REQUIRED


class PmtPngAccessor;
class PmtPngAccessorData;

typedef EkSmartPtr < PmtPngAccessor > PmtPngAccessorPtr;
typedef EkConstSmartPtr < PmtPngAccessor > ConstPmtPngAccessorPtr;


//! Provides access to metadata stored in PNG formatted image files. 
/*! One should never have to include this header file, nor use this 
  interface directly. All access to this class' functionality should
  be through the PmtAccessor base class interface. 
*/
class PMTACCR_IMPL_DECL PmtPngAccessor:public PmtAccessor
{
  public:

    //! Default constructor
    PmtPngAccessor();

    //! The constructor which takes the image FILE handle.
//        PmtPngAccessor( FILE* img );

    //! The copy constructor
    PmtPngAccessor(const PmtPngAccessor & src);

    //! Destructor
     virtual ~ PmtPngAccessor(void);

    //! Opens a PNG image file for reading / writing (depending on the mode)
    virtual bool open(const char *name, PmtOpenMode mode = PMT_READONLY);

    //! Close an PNG image file.
    virtual void close();

    //! Return a copy of the PmtPngAccessor.
    inline virtual PmtAccessorPtr clone() const
    {
        return new PmtPngAccessor();
    }

    virtual PmtMetadataPtr readMetadata(const SPmtString nsURI = "");

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
    //! Returns a PmtPngImageAccessor for the image. The image must have
    // been opened before calling this function.
    virtual PmtImageAccessor *getImageAccessor();
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET

  protected:

    // Check if the given file is an PNG file.
     virtual bool recognize(const SPmtString & name) const;

    virtual void getMdValue(PmtMetadataPtr & md);

    virtual void setMdValue(const PmtMetadataPtr & md);

  private:


     PmtPngAccessorData * mPngData;

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
    // An image accessor of the open image file
    PmtPngImageAccessor *mImgAccessor;
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET

    bool mClosing;

};

//PMTACCR_IMPL_REQUIRE(PmtPngAccessor)
#endif                          // PMT_PNG_ACCESSOR__H

