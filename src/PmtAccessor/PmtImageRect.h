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

#ifndef PMT_IMAGE_RECT__H
#define PMT_IMAGE_RECT__H

#include "PmtCommon.h"
#include "PmtAccrImplDefs.h"

/*! 
    \author Ricardo Rosario <ricardo.rosario@kodak.com>
    \date Jan 10 2003
    
    \brief A common image data container for the PmtImageAccessors.
    
It is passed to the getPixels() and setPixels() methods and contains the buffer
with the image data to be set (in the case of setPixels()) or the buffer where
the image data is to be stored (in the case of getPixels()). It also specifies
which pixels to get or set. This is done by specifying the coordinates of two
opposite corners (X1,Y1) and (X2,Y2). (0,0) is the upper left corner of the
image and the coordinates increase towards the right for X and towards the
bottom for Y. So, if we want to get or set the whole image, the coordinates
would be (0,0) and (image_width-1,image_height-1).
*/
class PMTACCR_IMPL_DECL PmtImageRect
{
  public:
    /*! PmtPixelFormat describes how the pixel data is organized.
       For example, for an 2x4 RGB image:
       PMT_INTERLEAVED - RGBRGBRGBRGB
       RGBRGBRGBRGB
       PMT_LINEAR      - RRRR
       GGGG
       BBBB
       RRRR
       GGGG
       BBBB
       PMT_PLANAR      - RRRR
       RRRR
       GGGG
       GGGG
       BBBB
       BBBB
     */
    typedef enum
    {
        PMT_INTERLEAVED,
        PMT_LINEAR,
        PMT_PLANAR
    }
    PixelFormat;

    /*! PmtPhotometricInterp describes how the pixels should be interpreted.
       For example: RGB, CMYK, etc.
     */
    typedef enum
    {
        PMT_RGB,
        PMT_CMYK
    }
    PhotometricInterp;


    //! Default Constructor
     PmtImageRect():mX1(0), mX2(0), mY1(0), mY2(0), mBitsPerSample(8),
        mPhotometricInterp(PMT_RGB), mNumOfPlanes(3),
        mPixFormat(PMT_INTERLEAVED), mImgBuf(NULL),
        mIOwnImgBuf(false), mSizeChange(false), mBufSize(0)
    {
    }

    //! Constructor
    PmtImageRect(unsigned int X1, unsigned int X2, unsigned int Y1,
                 unsigned int Y2, void *imgBuf = NULL,
                 unsigned char bitsPerSample = 8,
                 PhotometricInterp photoInterp = PMT_RGB,
                 unsigned char numberOfPlanes = 3,
                 PixelFormat pixelFormat = PMT_INTERLEAVED);

    //! Destructor
    ~PmtImageRect();

    //! This method returns the value of X1
    inline unsigned int getX1()
    {
        return mX1;
    }

    //! This method returns the value of X2
    inline unsigned int getX2()
    {
        return mX2;
    }

    //! This method returns the value of Y1
    inline unsigned int getY1()
    {
        return mY1;
    }

    //! This method returns the value of Y2
    inline unsigned int getY2()
    {
        return mY2;
    }

    //! This method returns the width
    inline unsigned int width()
    {
        return mX2 - mX1 + 1;
    }
    //! This method returns the height
    inline unsigned int height()
    {
        return mY2 - mY1 + 1;
    }

    //! This method is for setting the coordinates
    /*!
       \param X1 - x coordinate for corner 1
       \param X2 - x coordinate for corner 2
       \param Y1 - y coordinate for corner 1
       \param Y2 - y coordinate for corner 2

       Note: Corner 1 must be diagonally opposite to corner 2.
     */
    void setCoordinates(unsigned int X1, unsigned int X2, unsigned int Y1,
                        unsigned int Y2);

    //! This method returns the bits per sample
    inline unsigned char getBitsPerSample()
    {
        return mBitsPerSample;
    }

    //! This method sets the bits per sample. 
    /*!          
       \param bitsPerSample - new bits per sample value

       NOTE: This value must be a multiple of 8, for now...
     */
    void setBitsPerSample(unsigned char bitsPerSample);

    //! This method returns the photometric interpretation of the image
    //  data
    inline PhotometricInterp getPhotometricInterp()
    {
        return mPhotometricInterp;
    }

    //! This method sets the photometric interpretation of the image data
    /*!
       \param photoInterp - photometric interpretation
     */
    inline void setPhotometricInterp(PhotometricInterp photoInterp)
    {
        mPhotometricInterp = photoInterp;
    }

    //! This method returns the number of planes
    inline unsigned char getNumberOfPlanes()
    {
        return mNumOfPlanes;
    }

    //! This method sets the number of planes
    /*!
       \param numberOfPlanes - number of planes (ie colors)
     */
    void setNumberOfPlanes(unsigned char numberOfPlanes);

    //! This method returns the pixel format
    inline PixelFormat getPixelFormat()
    {
        return mPixFormat;
    }

    //! This method sets the pixel format
    /*!
       \param pixFormat - pixel format
     */
    inline void setPixelFormat(PixelFormat pixFormat)
    {
        mPixFormat = pixFormat;
    }

    //! This method returns a pointer to the image buffer.
    /*!  
       This buffer contains or will contain the image data
     */
    void *getImageBuffer();

    //! This method sets the image buffer
    /*!
       \param imgBuf - image buffer
     */
    void setImageBuffer(void *imgBuf);

  private:
    // These variables contain the coordinate values
    unsigned int mX1;
    unsigned int mX2;
    unsigned int mY1;
    unsigned int mY2;

    // The number of bits per each sample in the pixel
    unsigned char mBitsPerSample;

    // The photometric interpretation of the image data
    PhotometricInterp mPhotometricInterp;

    // The number of planes (colors)
    unsigned char mNumOfPlanes;

    // The pixel format (organization of pixel data)
    PixelFormat mPixFormat;

    // Pointer to the image data buffer
    void *mImgBuf;

    // An indicator of who owns the image buffer (true if PmtImageRect
    // created the buffer)
    bool mIOwnImgBuf;

    // An indicator of any changes in image size (coordinate changes that
    // change the buffer size required) since the last image buffer
    // allocation
    bool mSizeChange;

    unsigned int mBufSize;
};

#endif                          // PMT_IMAGE_RECT__H

#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED

