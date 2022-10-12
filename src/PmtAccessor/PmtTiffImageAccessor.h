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
 * Creation Date: 1/10/03
 *
 * Original Author: 
 * Ricardo Rosario <ricardo.rosario@kodak.com> 
 * 
 * Contributor(s): 
 */

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED

#ifndef PMT_TIFF_IMAGE_ACCESSOR__H
#define PMT_TIFF_IMAGE_ACCESSOR__H

#include "PmtCommon.h"
#include "PmtAccrImplDefs.h"
#include "PmtImageAccessor.h"
#include "PmtImageRect.h"
#include "TiffImageFile.h"

//! Provides access to image data stored in TIFF formatted image files. 
/*! One should never have to include this header file, nor use this 
  interface directly. All access to this class' functionality should
  be through the PmtImageAccessor base class interface. 
*/
class PMTACCR_IMPL_DECL PmtTiffImageAccessor:public PmtImageAccessor
{
  public:
    //! Default Constructor
    PmtTiffImageAccessor()
      :  PmtImageAccessor(PMTIMAGEACCESSOR_TIFF), mImgFile(NULL)
    {
    }

    //! Constructor
    PmtTiffImageAccessor(TiffImageFile * imgFile)
      :  PmtImageAccessor(PMTIMAGEACCESSOR_TIFF), mImgFile(imgFile)
    {
    }

    //! Destructor
    virtual ~ PmtTiffImageAccessor()
    {
    }

    /*! 
        This method gets the pixels specified by imgRect from the main
        image and copies them to the buffer in imgRect. The method
        returns 1 if it succeeds and 0 if it fails.
    */
    virtual unsigned int getPixels(PmtImageRect & imgRect);

    /*! 
        This method sets the pixels specified by imgRect and contained
        in its buffer to the main image. The method returns 1 if it
        succeeds and 0 if it fails.
    */
    virtual unsigned int setPixels(PmtImageRect & imgRect);

    /*!
        This method gets the pixels specified by imgRect from the thumbnail
        image and copies them to the buffer in imgRect. The method
        returns 1 if it succeeds and 0 if it fails.
    */
    virtual unsigned int getThumbPixels(PmtImageRect & imgRect);

    /*! 
        This method sets the pixels specified by imgRect and contained
        in its buffer to the thumbnail image. The method returns 1 if it
        succeeds and 0 if it fails.
    */
    virtual unsigned int setThumbPixels(PmtImageRect & imgRect);

    /*! Get the dimensions of the thumbnail image
    
     */
    virtual void getThumbDimensions(unsigned int& height, unsigned int& width);

    //! This method sets the TiffImageFile we are working with
    void setImageFile(TiffImageFile * imgFile)
    {
        mImgFile = imgFile;
    }

  private:
    // A pointer to the TiffImageFile
    TiffImageFile * mImgFile;
    
    // get pixels from the specified IFD
    unsigned int getPixelsFromIFD(PmtImageRect & imgRect, int ifdNum);
    
    // set pixels in the specified IFD
    unsigned int setPixelsInIFD(PmtImageRect & imgRect, int ifdNum);

};

#endif                          // PMT_TIFF_IMAGE_ACCESSOR__H

#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED

