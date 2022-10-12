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
 * Sam Fryer <samuel.fryer@kodak.com> 
 */

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED

#ifndef PMT_IMAGE_ACCESSOR__H
#define PMT_IMAGE_ACCESSOR__H

#include "PmtCommon.h"
#include "PmtImageRect.h"

#include "EkCompiler.h"
#include "EkSmartPtr.h"
#include "EkRefCount.h"


// Define some supported image types here to prevent confusion.
// This should really only be used for accessing detailed imaging
// that only one type of accessor can provide.  Only advanced users 
// (or picky image scientists) should use these.
#define PMTIMAGEACCESSOR_IMAGEUNKNOWN       0
#define PMTIMAGEACCESSOR_EXIF               1
#define PMTIMAGEACCESSOR_TIFF               2


/*! The PmtImageAccessor is the base class for image accessor implementations.

    An image accessor's primary responsibility is to provide applications
    with read / write access to the main image and thumbnail image data.
*/
class PMTACCR_IMPL_DECL PmtImageAccessor:public EkRefCount < EK_DEFAULT_MUTEX >
{
  public:
    //! Constructor
    PmtImageAccessor(unsigned char imageType = PMTIMAGEACCESSOR_IMAGEUNKNOWN)
      : mcImageType(imageType)
    {
    }

    //! Destructor
    virtual ~ PmtImageAccessor()
    {
    }

    /*! 
        This method gets the pixels specified by imgRect from the main
        image and copies them to the buffer in imgRect. The method
        returns 1 if it succeeds and 0 if it fails.
    */
    virtual unsigned int getPixels(PmtImageRect & imgRect) = 0;

    /*! 
        This method sets the pixels specified by imgRect and contained
        in its buffer to the main image. The method returns 1 if it
        succeeds and 0 if it fails.
    */
    virtual unsigned int setPixels(PmtImageRect & imgRect) = 0;
    
    /*!
        This method gets the pixels specified by imgRect from the thumbnail
        image and copies them to the buffer in imgRect. The method
        returns 1 if it succeeds and 0 if it fails.
    */
    virtual unsigned int getThumbPixels(PmtImageRect & imgRect) = 0;

    /*! 
        This method sets the pixels specified by imgRect and contained
        in its buffer to the thumbnail image. The method returns 1 if it
        succeeds and 0 if it fails.
    */
    virtual unsigned int setThumbPixels(PmtImageRect & imgRect) = 0;  

    /*! Get the dimensions of the thumbnail image
    
     */
    virtual void getThumbDimensions(unsigned int& height,
                                    unsigned int& width) = 0;

    /*! 
        Get the declared type of the image class.
     */
    unsigned char getImageType()
    {
        return mcImageType;
    }
    
  protected:
    unsigned char mcImageType;
    
  private:
};

// Smart pointers
typedef EkSmartPtr < PmtImageAccessor > PmtImageAccessorPtr;
typedef EkConstSmartPtr < PmtImageAccessor > PmtConstImageAccessorPtr;

#endif                          // PMT_IMAGE_ACCESSOR__H

#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED

