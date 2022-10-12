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

#include "PmtConfig.h"

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED

#include "PmtImageRect.h"

// Constructor
PmtImageRect::PmtImageRect(unsigned int X1, unsigned int X2, unsigned int Y1,
                           unsigned int Y2, void *imgBuf,
                           unsigned char bitsPerSample,
                           PhotometricInterp photoInterp,
                           unsigned char numberOfPlanes,
                           PixelFormat
                           pixelFormat):mBitsPerSample(bitsPerSample),
mPhotometricInterp(photoInterp), mNumOfPlanes(numberOfPlanes),
mPixFormat(pixelFormat), mImgBuf(imgBuf), mIOwnImgBuf(false),
mSizeChange(false), mBufSize(0)
{
    // We want mX2 >= mX1 and mY2 >= mY1
    if (X1 > X2)
    {
        mX2 = X1;
        mX1 = X2;
    }
    else
    {
        mX1 = X1;
        mX2 = X2;
    }

    if (Y1 > Y2)
    {
        mY2 = Y1;
        mY1 = Y2;
    }
    else
    {
        mY1 = Y1;
        mY2 = Y2;
    }
}

// Destructor
PmtImageRect::~PmtImageRect()
{
    // Delete the image buffer if we own it
    if (mIOwnImgBuf && mImgBuf)
        free(mImgBuf);
}

// This method is for setting the coordinates
void PmtImageRect::setCoordinates(unsigned int X1, unsigned int X2,
                                  unsigned int Y1, unsigned int Y2)
{
    // We want X2 >= X1 and Y2 >= Y1
    if (X1 > X2)
    {
        unsigned int temp = X1;
        X1 = X2;
        X2 = temp;
    }
    if (Y1 > Y2)
    {
        unsigned int temp = Y1;
        Y1 = Y2;
        Y2 = temp;
    }

    // Check if the size of the image has changed
    if ((mY2 - mY1) * (mX2 - mX1) != (Y2 - Y1) * (X2 - X1))
        mSizeChange = true;

    // Set the new coordinates
    mX1 = X1;
    mX2 = X2;
    mY1 = Y1;
    mY2 = Y2;
}

// This method sets the bits per sample
void PmtImageRect::setBitsPerSample(unsigned char bitsPerSample)
{
    if (bitsPerSample != mBitsPerSample)
    {
        mSizeChange = true;
        mBitsPerSample = bitsPerSample;
    }
}

// This method sets the number of planes
void PmtImageRect::setNumberOfPlanes(unsigned char numberOfPlanes)
{
    if (numberOfPlanes != mNumOfPlanes)
    {
        mSizeChange = true;
        mNumOfPlanes = numberOfPlanes;
    }
}

// This method returns a pointer to the buffer that contains or will contain
// the image data
void *PmtImageRect::getImageBuffer()
{
    if (!mImgBuf)
    {
        // if we don't have a buffer yet, we need to create it
        // mBitsPerSample required to be multiple of 8!!!
        unsigned int requiredBufSize =
            (mX2-mX1+1)*(mY2-mY1+1) * mNumOfPlanes * mBitsPerSample / 8;
        mImgBuf = malloc(requiredBufSize);
        mBufSize = requiredBufSize;
        mIOwnImgBuf = true;
        mSizeChange = false;
    }
    else if (mSizeChange && mIOwnImgBuf)
    {
        // if we do have a buffer, and the required size has changed
        // mBitsPerSample required to be multiple of 8!!!
        unsigned int requiredBufSize =
            (mX2-mX1+1) * (mY2-mY1+1) * mNumOfPlanes * mBitsPerSample / 8;

        // if we now need a bigger buffer, delete the one we have and create a
        // new one of the required size
        if (requiredBufSize > mBufSize)
        {
            free(mImgBuf);
            mImgBuf = malloc(requiredBufSize);
            mSizeChange = false;
            mBufSize = requiredBufSize;
        }
    }

    return mImgBuf;
}

// This method returns a pointer to the buffer that contains or will
// contain the image data
void PmtImageRect::setImageBuffer(void *imgBuf)
{
    if (mImgBuf && mIOwnImgBuf)
    {
        free(mImgBuf);
        mIOwnImgBuf = false;
        mSizeChange = false;
        mBufSize = 0;
    }
    mImgBuf = imgBuf;
}

#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED

