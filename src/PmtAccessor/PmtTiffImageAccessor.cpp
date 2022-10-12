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

#include "PmtTiffImageAccessor.h"
#include "PmtTiffAccessor.h"
#include "TiffDirectory.h"
#include "TiffStripImage.h"

unsigned int PmtTiffImageAccessor::getPixels(PmtImageRect & imgRect)
{
    // The main image is in IFD0
    return getPixelsFromIFD( imgRect, 0 );
}
 
unsigned int PmtTiffImageAccessor::setPixels(PmtImageRect & imgRect)
{
    // The main image is in IFD0
    return setPixelsInIFD( imgRect, 0 );
}

// This method gets the pixels specified by imgRect from the thumbnail image
unsigned int PmtTiffImageAccessor::getThumbPixels(PmtImageRect & imgRect)
{   
    // The main image is in IFD1
    return getPixelsFromIFD( imgRect, 1 );
}

// This method sets the pixels specified by imgRect to the thumbnail image.
unsigned int PmtTiffImageAccessor::setThumbPixels(PmtImageRect & imgRect)
{
    // The main image is in IFD1
    return setPixelsInIFD( imgRect, 1 );
}

// Get the dimensions of the thumbnail image
void PmtTiffImageAccessor::getThumbDimensions(unsigned int& height, 
                                              unsigned int& width)
{
    if (mImgFile==NULL)
    {
        height = 0;
        width = 0;    
    }
    else
    {
        TiffDirectory* thumbIFD = mImgFile->getDirectory(1);
        TiffTagEntry* entry;
        
        if ( thumbIFD != NULL )
        {
            entry = thumbIFD->getGenericTag(TIFFTAG_IMAGELENGTH);
            if (entry)
            {
                if (entry->getType() == EKTIFF_USHORT)
                {
                    unsigned int val = dynamic_cast < TiffTagEntryT < uint16 > *>
                        (entry)->getValue();
                    
                    height = val;
                }
                else if (entry->getType() == EKTIFF_ULONG)
                {
                    unsigned int val = dynamic_cast < TiffTagEntryT<PMTTIFF_UINT32> *>
                        (entry)->getValue();
                    
                    height = val;
                }
                else
                    height = 0;
            }
            else
                height = 0;

            entry = thumbIFD->getGenericTag(TIFFTAG_IMAGEWIDTH);
            if (entry)
            {
                if (entry->getType() == EKTIFF_USHORT)
                {
                    unsigned int val = dynamic_cast < TiffTagEntryT<uint16> *>
                        (entry)->getValue();

                    width = val;
                }
                else if (entry->getType() == EKTIFF_ULONG)
                {
                    unsigned int val = dynamic_cast < TiffTagEntryT<PMTTIFF_UINT32> *>
                        (entry)->getValue();
                    
                    width = val;
                }
                else
                    width = 0;
            }
            else
                width = 0;

        }

    }
    
    
}

// get pixels from the specified IFD
unsigned int PmtTiffImageAccessor::getPixelsFromIFD(PmtImageRect & imgRect,
                                                    int ifdNum)
{
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

    // Make sure we have a TiffImageFile
    if (mImgFile==NULL)
        return 0;

    // Check to see if the specified main IFD is present. If not, there is no
    // image to get
    TiffDirectory *mainIFD = mImgFile->getDirectory(ifdNum);
    if (mainIFD == NULL)
        return 0;

    // Check to make sure there is an image present in the IFD.
    // (ImageWidth and ImageHeight tags must be present)
    if (mainIFD->getGenericTag(TIFFTAG_IMAGEWIDTH) == NULL ||
        mainIFD->getGenericTag(TIFFTAG_IMAGELENGTH) == NULL)
        return 0;

    // Check to see if the image contains the PhotometricInterpretation entry
    TiffTagEntry *entry;
    entry = mainIFD->getGenericTag(TIFFTAG_PHOTOMETRIC);
    if (!entry)
        return 0;               // PhotometricInterpretation is a mandatory tag

    // Check the value of the photometric interpretation, to see if it is RGB
    if (dynamic_cast < TiffTagEntryT < uint16 > *>(entry)->getValue() ==
        PHOTOMETRIC_RGB)
    {
        // Check for a PlanarConfiguration entry. This tag is not mandatory.
        entry = mainIFD->getGenericTag(TIFFTAG_PLANARCONFIG);
        if (entry)
        {
            if (dynamic_cast < TiffTagEntryT < uint16 >
                *>(entry)->getValue() != PLANARCONFIG_CONTIG)
                return 0;       // We only support Chunky format for now ( RGBRGBRGB... )
        }

        // Check for a Compression entry.
        entry = mainIFD->getGenericTag(TIFFTAG_COMPRESSION);
        if (!entry)
            return 0;           // Compression is a mandatory tag
        if (dynamic_cast < TiffTagEntryT < uint16 > *>(entry)->getValue() !=
            COMPRESSION_NONE)
            return 0;           // Compressed images not supported for now

        // Check for a BitsPerSample entry
        entry = mainIFD->getGenericTag(TIFFTAG_BITSPERSAMPLE);
        if (!entry)
            return 0;
        if (dynamic_cast < TiffTagEntryT < vector < uint16 > >*>
            (entry)->getValue() != vector < uint16 > (3, 8))
            return 0;           // Only <8,8,8> bits per sample supported for now

        // Make sure image is not tiled.
        if (mainIFD->getImage()->isTiled())
            return 0;           // Tiled images not supported for now

        // Check type of ImageLength entry. This entry can have USHORT
        // or ULONG type. If it is USHORT, go ahead and change it to 
        // ULONG.
        entry = mainIFD->getGenericTag(TIFFTAG_IMAGELENGTH);
        if (entry->getType() == EKTIFF_USHORT)
        {
            unsigned int val = dynamic_cast < TiffTagEntryT < uint16 > *>
                (entry)->getValue();

            entry = new TiffTagEntryT < PMTTIFF_UINT32 > (TIFFTAG_IMAGELENGTH,
                                                  EKTIFF_ULONG, 1, val);

            mainIFD->setGenericTag(*entry);
            delete entry;
        }

        // Check type of ImageWidth entry. This entry can have USHORT
        // or ULONG type. If it is USHORT, go ahead and change it to 
        // ULONG.
        entry = mainIFD->getGenericTag(TIFFTAG_IMAGEWIDTH);
        if (entry->getType() == EKTIFF_USHORT)
        {
            unsigned int val = dynamic_cast < TiffTagEntryT < uint16 > *>
                (entry)->getValue();

            entry = new TiffTagEntryT < PMTTIFF_UINT32 > (TIFFTAG_IMAGEWIDTH,
                                                  EKTIFF_ULONG, 1, val);

            mainIFD->setGenericTag(*entry);
            delete entry;
        }

        // Get ImageLength and ImageWidth values
        const unsigned int height = dynamic_cast < TiffTagEntryT < PMTTIFF_UINT32 > *>
            (mainIFD->getGenericTag(TIFFTAG_IMAGELENGTH))->getValue();
        const unsigned int width = dynamic_cast < TiffTagEntryT < PMTTIFF_UINT32 > *>
            (mainIFD->getGenericTag(TIFFTAG_IMAGEWIDTH))->getValue();

        // Get the image, read it to mainImage one strip at a time
        TiffStripImage *tiffStripImg = (TiffStripImage *) mainIFD->getImage();
        unsigned int numOfStrips = mainIFD->numOfStrips();
        tsize_t stripSize = tiffStripImg->stripSize();
        tsize_t offset = 0;
        
        // Allocate memory to place the main image
        int mainImageBufferSize = numOfStrips * stripSize;
        char *mainImage = (char *) malloc(mainImageBufferSize);
        // Get the pointer to the buffer where the image data is to be copied
        char *imageBuffer = (char *) imgRect.getImageBuffer();        
        
        for (tstrip_t s = 0; s < numOfStrips; s++)
        {
            offset +=
                tiffStripImg->readRawStrip(s, mainImage + offset, stripSize);
        }

        // Copy the specified image data into imageBuffer (the buffer provided
        // by the app to get the image data to)
        // Start at y1 and go all the way to y2 in the mainImage. But in the
        // image buffer, we need to subtract y1 from i because (x1,y1) of the
        // mainImage needs to be copied to (0,0) of the imageBuffer and so on.
        for (unsigned int i = y1; i <= y2; i++)
        {
            // Start at x1*3 and go all the way to x2*3+3-1 (need to multiply
            // by 3 because every pixel contains 3 bytes of data - RGB) in the
            // mainImage. In the image buffer, we need to subtract x1*3 from j.
            for (unsigned int j = x1 * 3; j < (x2*3+3); j++)
            {
                *(imageBuffer + (i - y1) * (x2 - x1 + 1)*3 + (j - x1*3)) =
                    *(mainImage + i * width * 3 + j);
            }
        }

        free(mainImage);
    }
    else
    {
        return 0;               // We only support RGB for now
    }


    return 1;
}

// set pixels in the specified IFD
unsigned int PmtTiffImageAccessor::setPixelsInIFD(PmtImageRect & imgRect,
                                                    int ifdNum)
{
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

    // Make sure we have a TiffImageFile
    if (mImgFile==NULL)
        return 0;

    // Check to see if the main IFD is present. If not, create it.
    while (mImgFile->getDirectory(ifdNum) == NULL)
        mImgFile->createDirectory();

    // Get IFD0
    TiffDirectory *mainIFD = mImgFile->getDirectory(ifdNum);

    // Check to see if there is an image present in the main IFD. 
    // (ImageWidth and ImageHeight tags must be present)
    if (mainIFD->getGenericTag(TIFFTAG_IMAGEWIDTH) == NULL ||
        mainIFD->getGenericTag(TIFFTAG_IMAGELENGTH) == NULL)
    {
        // There is no image. Therefore we need to create and set all the IFD
        // entries (metadata) required by the TIFF specification
        TiffTagEntry *et;
        if (imgRect.getPhotometricInterp() == PmtImageRect::PMT_RGB)
        {
            et = new TiffTagEntryT < PMTTIFF_UINT32 > (TIFFTAG_IMAGEWIDTH,
                                               EKTIFF_ULONG, 1, x2+1);
            mainIFD->setGenericTag(*et);
            delete et;

            et = new TiffTagEntryT < PMTTIFF_UINT32 > (TIFFTAG_IMAGELENGTH,
                                               EKTIFF_ULONG, 1, y2+1);
            mainIFD->setGenericTag(*et);
            delete et;

            et = new TiffTagEntryT < uint16 > (TIFFTAG_PHOTOMETRIC,
                                               EKTIFF_USHORT, 1,
                                               PHOTOMETRIC_RGB);
            mainIFD->setGenericTag(*et);
            delete et;

            et = new TiffTagEntryT < uint16 > (TIFFTAG_PLANARCONFIG,
                                               EKTIFF_USHORT, 1,
                                               PLANARCONFIG_CONTIG);
            mainIFD->setGenericTag(*et);
            delete et;

            et = new TiffTagEntryT < uint16 > (TIFFTAG_SAMPLESPERPIXEL,
                                               EKTIFF_USHORT, 1, 3);
            mainIFD->setGenericTag(*et);
            delete et;

            et = new TiffTagEntryT < vector < uint16 > >(TIFFTAG_BITSPERSAMPLE,
                                                         EKTIFF_USHORT, 3,
                                                         vector < uint16 > (3,
                                                                            8));
            mainIFD->setGenericTag(*et);
            delete et;

            et = new TiffTagEntryT < PMTTIFF_UINT32 > (TIFFTAG_ROWSPERSTRIP,
                                               EKTIFF_ULONG, 1, y2+1);
            mainIFD->setGenericTag(*et);
            delete et;

            et = new TiffTagEntryT < uint16 > (TIFFTAG_COMPRESSION,
                                               EKTIFF_USHORT, 1,
                                               COMPRESSION_NONE);
            mainIFD->setGenericTag(*et);
            delete et;

            et = new TiffTagEntryT < uint16 > (TIFFTAG_RESOLUTIONUNIT,
                                               EKTIFF_USHORT, 1, RESUNIT_INCH);
            mainIFD->setGenericTag(*et);
            delete et;

            et = new TiffTagEntryT < float >(TIFFTAG_XRESOLUTION,
                                             EKTIFF_URATIONAL, 1, 72);
            mainIFD->setGenericTag(*et);
            delete et;

            et = new TiffTagEntryT < float >(TIFFTAG_YRESOLUTION,
                                             EKTIFF_URATIONAL, 1, 72);
            mainIFD->setGenericTag(*et);
            delete et;
        }
        else
        {
            return 0;           // Only RGB is supported for now.
        }
    }
    else
    {
        return 0;               // Only creating new images is supported for now.
    }

    // get value of ImageLength and ImageWidth
    const unsigned int height = dynamic_cast < TiffTagEntryT < PMTTIFF_UINT32 > *>
        (mainIFD->getGenericTag(TIFFTAG_IMAGELENGTH))->getValue();
    const unsigned int width = dynamic_cast < TiffTagEntryT < PMTTIFF_UINT32 > *>
        (mainIFD->getGenericTag(TIFFTAG_IMAGEWIDTH))->getValue();

    // Allocate memory to place the main image
    i32_t mainImageBufferSize = width * height * 3;
    char *mainImage = (char *) malloc(mainImageBufferSize);
    // Get the pointer to the buffer with the image data to be set
    char *imageBuffer = (char *) imgRect.getImageBuffer();

    // Copy the specified image data from imageBuffer (the buffer provided
    // by the app to get the image data from) to mainImage
    for (unsigned int i = y1; i <= y2; i++)
    {
        for (unsigned int j = x1*3; j < (x2*3+3); j++)
        {
            *(mainImage + i*width*3 + j) =
                *(imageBuffer + (i - y1)*(x2 - x1 + 1)*3 + (j - x1*3));
        }
    }

    // write mainImage to the file
    TiffStripImage *img = (TiffStripImage *) (mainIFD->getImage());
    img->writeRawStrip(0, mainImage, mainImageBufferSize);
    img->flush();

    free(mainImage);
    return 1;
}

#endif                          /* PMT_IMAGE_ACCESSOR_REQUIRED */

