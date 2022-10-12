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
 * Creation Date: 06/16/2003
 *
 * Original Author: 
 * Ricardo Rosario <ricardo.rosario@kodak.com> 
 * 
 * Contributor(s): 
 */

#include "PmtConfig.h"

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED

#include "PmtExifImageAccessor.h"
#include "ExifImageDescUtils.h"

// Get the pixels from the image and copy them to the passed in PmtImageRect
unsigned int PmtExifImageAccessor::getPixels(PmtImageRect & imgRect)
{
    unsigned int returnValue = 1;

    // Get the coordinates specified in imgRect
    unsigned int x1 = imgRect.getX1();
    unsigned int x2 = imgRect.getX2();
    unsigned int y1 = imgRect.getY1();
    unsigned int y2 = imgRect.getY2();

    // We want x2 to be greater than x1 and y2 to be greater than y1
    if (x1 > x2)
    {
        unsigned int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if (y1 > y2)
    {
        unsigned int temp = y1;
        y1 = y2;
        y2 = temp;
    }
    
    // Check that the photometric interpretation in imgRect is RGB and
    // make sure we have a ExifImageFile
    if ( mImgFile!=NULL && 
        imgRect.getPhotometricInterp()==PmtImageRect::PMT_RGB )
    {
        // Try to read existing image into a buffer.
        ExifImageInfo imgInfo;
        mImgFile->getImageInfo(imgInfo);
        ExifImageDesc existingImgBuf;
        exifAllocImageDesc( imgInfo, existingImgBuf );
        ExifStatus status ;
        exif_uint32 nextLine ;
        exif_uint32 linesDone ;
        status = mImgFile->readScanLines( existingImgBuf,
            existingImgBuf.numberOfRows, nextLine, linesDone ) ;

        if ( status != EXIF_OK )
        {
            // No existing image
            returnValue = 0;
        }
        else
        {
            // copy pixels (one row at a time) from the image to imgRect
            char* copyFrom = (char*)existingImgBuf.components[0].theData;
            char* copyTo = (char*)imgRect.getImageBuffer();
            for (unsigned int i = y1; i <= y2; i++)
            {
                memcpy( (copyTo + (i-y1)*(x2-x1+1)*3), 
                    (copyFrom + i*imgInfo.width*3 + x1*3), (x2-x1+1)*3 );
            }            
        }

        // Delete the image buffer
        exifDeleteImageDesc( existingImgBuf );
    }
    else
        returnValue = 0;
    
    return returnValue;
}

// Set the pixels specified in the passed in PmtImageRect
unsigned int PmtExifImageAccessor::setPixels(PmtImageRect & imgRect)
{
    unsigned int returnValue = 1;

    // Get the coordinates specified in imgRect
    unsigned int x1 = imgRect.getX1();
    unsigned int x2 = imgRect.getX2();
    unsigned int y1 = imgRect.getY1();
    unsigned int y2 = imgRect.getY2();

    // We want x2 to be greater than x1 and y2 to be greater than y1
    if (x1 > x2)
    {
        unsigned int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if (y1 > y2)
    {
        unsigned int temp = y1;
        y1 = y2;
        y2 = temp;
    }
    
    // Check that the photometric interpretation in imgRect is RGB and
    // make sure we have a ExifImageFile
    if ( mImgFile!=NULL && 
        imgRect.getPhotometricInterp()==PmtImageRect::PMT_RGB )
    {
    
        // a temporary image buffer for the image we are going to write out
        ExifImageDesc imageBuf;
        unsigned int imageHeight;
        unsigned int imageWidth;
    
        // Try to read existing image into a buffer.
        ExifImageInfo imgInfo;
        mImgFile->getImageInfo(imgInfo);
        ExifImageDesc existingImgBuf;
        exifAllocImageDesc( imgInfo, existingImgBuf );
        ExifStatus status ;
        exif_uint32 nextLine ;
        exif_uint32 linesDone ;
        status = mImgFile->readScanLines( existingImgBuf,
            existingImgBuf.numberOfRows, nextLine, linesDone ) ;

        if ( status != EXIF_OK )
        {
            // No existing image
            imgInfo.height = y2+1;
            imgInfo.width = x2+1;
        
            // set the image information
            mImgFile->setImageInfo(imgInfo);
        
            exifAllocImageDesc( imgInfo, imageBuf );
        
            imageHeight = imgInfo.height;
            imageWidth = imgInfo.width;
        }
        else
        {
            // There is an existing image
            // Check if we are going to need to create a bigger image
            if ( y2+1 > imgInfo.height )
                imageHeight = y2+1;
            else
                imageHeight = imgInfo.height;
            if ( x2+1 > imgInfo.width )
                imageWidth = x2+1;
            else
                imageWidth = imgInfo.width;
        
            if ( imageHeight!=imgInfo.height || imageWidth!=imgInfo.width )
            {
                // the image has grown
                unsigned int origHeight = imgInfo.height;
                unsigned int origWidth = imgInfo.width;
                imgInfo.height = imageHeight;
                imgInfo.width = imageWidth;
            
                exifAllocImageDesc( imgInfo, imageBuf );
            
                char* origBuf = (char*)existingImgBuf.components[0].theData;
                char* newBuf = (char*)imageBuf.components[0].theData;
            
                // Copy existing image in origBuf to the new larger buffer.
                // This must be done one row at a time, taking into account that
                // the new image may be wider and/or taller than the original.
                for ( unsigned int i = 0; i < origHeight; i++ )
                {
                    memcpy( (newBuf + i*imageWidth*3),
                        (origBuf + i*origWidth*3), origWidth*3  );
                }
            }
            else
            {   
                // the image hasn't changed size so we just copy the existing
                // image to the new image buffer
                exifAllocImageDesc( imgInfo, imageBuf );
                memcpy( imageBuf.components[0].theData,
                    existingImgBuf.components[0].theData,
                    imageHeight*imageWidth*3 );
            }
        }
        
        // clean up
        exifDeleteImageDesc( existingImgBuf ) ;
    
        // copy pixel values from image rect to the image buf to be written to 
        // the file
        char* copyTo = (char*)imageBuf.components[0].theData;
        char* copyFrom = (char*)imgRect.getImageBuffer();
        for (unsigned int i = y1; i <= y2; i++)
        {
            memcpy( (copyTo + i*imageWidth*3 + x1*3), 
                (copyFrom + (i-y1)*(x2-x1+1)*3), (x2-x1+1)*3 );
        }
    
        // do the compression
        status = mImgFile->writeScanLines( imageBuf, imageBuf.numberOfRows,
            nextLine, linesDone ) ;
    
        if ( status != EXIF_OK )
            returnValue = 0;
        
        // clean up
        exifDeleteImageDesc( imageBuf ) ;
    }
    else
        returnValue = 0;
    
    return returnValue;
}

// This method gets the pixels specified by imgRect from the thumbnail image
unsigned int PmtExifImageAccessor::getThumbPixels(PmtImageRect & imgRect)
{
    unsigned int returnValue = 0;
    
    // Get the coordinates specified in imgRect
    unsigned int x1 = imgRect.getX1();
    unsigned int x2 = imgRect.getX2();
    unsigned int y1 = imgRect.getY1();
    unsigned int y2 = imgRect.getY2();

    // We want x2 to be greater than x1 and y2 to be greater than y1
    if (x1 > x2)
    {
        unsigned int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if (y1 > y2)
    {
        unsigned int temp = y1;
        y1 = y2;
        y2 = temp;
    }
    
    // Check that the photometric interpretation in imgRect is RGB and
    // make sure we have a ExifImageFile
    if ( mImgFile!=NULL && 
        imgRect.getPhotometricInterp()==PmtImageRect::PMT_RGB )
    {
        ExifImageDesc* thumbBuf = mImgFile->getThumbnail();
        ExifImageInfo imgInfo ;
        imgInfo.width = thumbBuf->numberOfColumns ;
        imgInfo.height = thumbBuf->numberOfRows ;
        imgInfo.numChannels = thumbBuf->numberOfComponents ;
        
        if (thumbBuf != NULL)
        {
            // copy pixels (one row at a time) from the image to imgRect
            char* copyFrom = (char*)thumbBuf->components[0].theData;
            char* copyTo = (char*)imgRect.getImageBuffer();
            for (unsigned int i = y1; i <= y2; i++)
            {
                memcpy( (copyTo + (i-y1)*(x2-x1+1)*3), 
                    (copyFrom + i*imgInfo.width*3 + x1*3), (x2-x1+1)*3 );
            }
            returnValue = 1; // Success!!!
        }
    }
    
    return returnValue;
}

// This method sets the pixels specified by imgRect to the thumbnail image.
unsigned int PmtExifImageAccessor::setThumbPixels(PmtImageRect & imgRect)
{
    unsigned int returnValue = 0;
    
    // Get the coordinates specified in imgRect
    unsigned int x1 = imgRect.getX1();
    unsigned int x2 = imgRect.getX2();
    unsigned int y1 = imgRect.getY1();
    unsigned int y2 = imgRect.getY2();

    // We want x2 to be greater than x1 and y2 to be greater than y1
    if (x1 > x2)
    {
        unsigned int temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if (y1 > y2)
    {
        unsigned int temp = y1;
        y1 = y2;
        y2 = temp;
    }

    // Check that the photometric interpretation in imgRect is RGB and
    // make sure we have a ExifImageFile
    if ( mImgFile!=NULL && 
        imgRect.getPhotometricInterp()==PmtImageRect::PMT_RGB )
    {
        // an image buffer for the image we are going to write out
        ExifImageDesc* imageBuf = new ExifImageDesc;
        unsigned int imageHeight;
        unsigned int imageWidth;
    
        // Try to read existing thumbnail image into a buffer.
        ExifImageDesc* existingThumb = mImgFile->getThumbnail();
        ExifImageInfo imgInfo;
        
        if (existingThumb == NULL)
        {
            // No existing thumbnail image
            imgInfo.height = y2+1;
            imgInfo.width = x2+1;
            imgInfo.numChannels = 3;
        
            exifAllocImageDesc( imgInfo, *imageBuf );
        
            imageHeight = imgInfo.height;
            imageWidth = imgInfo.width;
            
            // Set the tags related to the thumbnail dimensions
            ExifTagEntry* entry = new ExifTagEntryT<exif_uint32>( 257, 
                EXIF_LONG, 1, imageHeight );
            mImgFile->setGenericTag(*entry,EXIF_APP1_IFD1 );
            delete entry;
            
            entry = new ExifTagEntryT<exif_uint32>( 256, 
                EXIF_LONG, 1, imageWidth );
            mImgFile->setGenericTag(*entry,EXIF_APP1_IFD1);
            delete entry;
        }
        else
        {
            // There is an existing thumbnail image
            imgInfo.width = existingThumb->numberOfColumns;
            imgInfo.height = existingThumb->numberOfRows;
            imgInfo.numChannels = existingThumb->numberOfComponents; 
            
            // Check if we are going to need to create a bigger image
            if ( y2+1 > imgInfo.height )
                imageHeight = y2+1;
            else
                imageHeight = imgInfo.height;
            if ( x2+1 > imgInfo.width )
                imageWidth = x2+1;
            else
                imageWidth = imgInfo.width;
        
            if ( imageHeight!=imgInfo.height || imageWidth!=imgInfo.width )
            {
                // the image has grown
                unsigned int origHeight = imgInfo.height;
                unsigned int origWidth = imgInfo.width;
                imgInfo.height = imageHeight;
                imgInfo.width = imageWidth;
            
                exifAllocImageDesc( imgInfo, *imageBuf );
            
                char* origBuf = (char*)existingThumb->components[0].theData;
                char* newBuf = (char*)imageBuf->components[0].theData;
            
                // Copy existing image in origBuf to the new larger buffer.
                // This must be done one row at a time, taking into account that
                // the new image may be wider and/or taller than the original.
                for ( unsigned int i = 0; i < origHeight; i++ )
                {
                    memcpy( (newBuf + i*imageWidth*3),
                        (origBuf + i*origWidth*3), origWidth*3  );
                }
            }
            else
            {   
                // the image hasn't changed size so we just copy the existing
                // image to the new image buffer
                exifAllocImageDesc( imgInfo, *imageBuf );
                memcpy( imageBuf->components[0].theData,
                    existingThumb->components[0].theData,
                    imageHeight*imageWidth*3 );
            }
        }
        
        // copy pixel values from image rect to the image buf to be written to 
        // the file
        char* copyTo = (char*)imageBuf->components[0].theData;
        char* copyFrom = (char*)imgRect.getImageBuffer();
        for (unsigned int i = y1; i <= y2; i++)
        {
            memcpy( (copyTo + i*imageWidth*3 + x1*3), 
                (copyFrom + (i-y1)*(x2-x1+1)*3), (x2-x1+1)*3 );
        }

        ExifStatus status = mImgFile->setThumbnail( imageBuf, mbCompressThumb);
        if ( status != EXIF_OK )
            returnValue = 0;
    }
    else
        returnValue = 0;

    return returnValue;
}

// Get the dimensions of the thumbnail image
void PmtExifImageAccessor::getThumbDimensions(unsigned int& height, 
                                              unsigned int& width)
{
    // Try to read existing thumbnail if any
    ExifImageDesc* existingThumb = mImgFile->getThumbnail();
    if ( existingThumb==NULL )
    {
        // No thumbnail found. Set dimensions to zero
        height = 0;
        width = 0;
    }
    else
    {
        height = existingThumb->numberOfRows;
        width = existingThumb->numberOfColumns;
    }
}


#endif                          /* PMT_IMAGE_ACCESSOR_REQUIRED */

