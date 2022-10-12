/* 
 * The contents of this file are subject to the IBM Common Public
 * License Version 0.5 (the "License"); you may not use this file 
 * except in compliance with the License. You may obtain a copy of 
 * the License at http://oss.software.ibm.com/developerworks/
 * opensource/license-cpl.html
*
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Initial Developer of the Original Code is Eastman
 * Kodak Company. Portions created by Kodak are
 * Copyright (C) 1999-2000 Eastman Kodak Company. All
 * Rights Reserved.
 *
 * Original Author:
 * Ricardo Rosario ricardo.rosario@kodak.com
 *
 * Contributor(s):
 *
 * Date: February 1, 2003
*/

/* 
 * createTiff.cpp Description:
 * This example program demonstrates how to create a Tiff 8-bit RGB image from
 * scratch using the Picture Metadata Toolkit (PMT). It also demonstrates
 * how to set the thumbnail image. PMT will automatically create and write
 * the mandatory tags that must go with the image.
 *
 * NOTE: This program uses the PmtImageAccessor. Therefore, PMT must be built
 * with the PMT_IMAGE_ACCESSOR_REQUIRED preprocessor definition for this
 * program.
 */

#include <iostream>

#include "PmtDefinitionInterpreter.h"
#include "PmtTiffAccessor.h"

// Dimensions of the image we are going to create
#define WIDTH   600
#define HEIGHT  400
// Dimension of the thumbnail image
#define THUMBWIDTH 60
#define THUMBHEIGHT 40

void initializePixels(char *buf);
void initializeThumbPixels(char *buf);

int main()
{
    // Load the default schema
    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load();
    delete definitionInterpreter;

    // name of file we are creating
    char *file = "testCreate.tif";

    // Create a Tiff Accessor. PmtAccessorPtr is a Smart Pointer and doesn't
    // require deleting (trying to will cause a compile error).
    PmtAccessorPtr tiffAcc = new PmtTiffAccessor();

    // Open the file we are creating
    tiffAcc->open(file, PMT_CREATE);

    // Allocate the buffer for the image data and initialize it.
    // For an 8-bit RGB image, we need height*width*3 bytes. The RGB data is
    // interleaveed (RGBRGBRGB....)
    char *buf = (char *) malloc(HEIGHT * WIDTH * 3);
    initializePixels(buf);

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED
    // Create a PmtImageRect to pass into the setPixels method.
    PmtImageRect imgRectWrite(0, WIDTH - 1, 0, HEIGHT - 1, buf);

    // Get the image accessor from the tiff accessor and set the pixels.
    PmtImageAccessor* imgAcc = tiffAcc->getImageAccessor();
    if ( imgAcc != NULL )
        imgAcc->setPixels(imgRectWrite);
    else
        cout << "Error getting the Image Accessor!!!" << endl;
    
    //--- Set the thumbnail ---
    
    // Allocate the buffer for the Thumbnail image data and initialize it
    char * thumbBuf = (char *) malloc(THUMBHEIGHT*THUMBWIDTH*3);
    initializeThumbPixels(thumbBuf);
    
    // Create the PmtImageRect for the Thumbnail
    PmtImageRect thumbImgRect(0,THUMBWIDTH-1,0,THUMBHEIGHT-1,thumbBuf);
    
    if ( imgAcc != NULL )
    {
        // Set the thumbnail pixels
        imgAcc->setThumbPixels(thumbImgRect);
    }
    
    free(thumbBuf);
    
#else
    cout <<
        "Error!!! You need to build with PMT_IMAGE_ACCESSOR_REQUIRED defined"
        << endl;
#endif
    // Close the file (tiff accessor) and delete the pixel buffer.
    tiffAcc->close();
    free(buf);

    return 0;
}

// This function initializes the pixels to form a checkerboard type pattern of
// red, green and blue boxes.
void initializePixels(char *buf)
{
    // Initialize the values in all of the pixels
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < (WIDTH * 3); j += 3)
        {
            // Set R, G, B values
            if (i % 300 < 100)
            {
                if (j % 900 < 300)
                {
                    // Set to RED
                    *(buf + i * WIDTH * 3 + j) = (char) 255;    //R
                    *(buf + i * WIDTH * 3 + j + 1) = (char) 0;  //G
                    *(buf + i * WIDTH * 3 + j + 2) = (char) 0;  //B
                }
                else if (j % 900 < 600)
                {
                    // Set to GREEN
                    *(buf + i * WIDTH * 3 + j) = (char) 0;  //R
                    *(buf + i * WIDTH * 3 + j + 1) = (char) 255;    //G
                    *(buf + i * WIDTH * 3 + j + 2) = (char) 0;  //B
                }
                else
                {
                    // Set to BLUE
                    *(buf + i * WIDTH * 3 + j) = (char) 0;  //R
                    *(buf + i * WIDTH * 3 + j + 1) = (char) 0;  //G
                    *(buf + i * WIDTH * 3 + j + 2) = (char) 255;    //B
                }
            }
            else if (i % 300 < 200)
            {
                if (j % 900 < 300)
                {
                    // Set to GREEN
                    *(buf + i * WIDTH * 3 + j) = (char) 0;  //R
                    *(buf + i * WIDTH * 3 + j + 1) = (char) 255;    //G
                    *(buf + i * WIDTH * 3 + j + 2) = (char) 0;  //B
                }
                else if (j % 900 < 600)
                {
                    // Set to BLUE
                    *(buf + i * WIDTH * 3 + j) = (char) 0;  //R
                    *(buf + i * WIDTH * 3 + j + 1) = (char) 0;  //G
                    *(buf + i * WIDTH * 3 + j + 2) = (char) 255;    //B
                }
                else
                {
                    // Set to RED
                    *(buf + i * WIDTH * 3 + j) = (char) 255;    //R
                    *(buf + i * WIDTH * 3 + j + 1) = (char) 0;  //G
                    *(buf + i * WIDTH * 3 + j + 2) = (char) 0;  //B
                }
            }
            else if (i % 300 < 300)
            {
                if (j % 900 < 300)
                {
                    // Set to BLUE
                    *(buf + i * WIDTH * 3 + j) = (char) 0;  //R
                    *(buf + i * WIDTH * 3 + j + 1) = (char) 0;  //G
                    *(buf + i * WIDTH * 3 + j + 2) = (char) 255;    //B
                }
                else if (j % 900 < 600)
                {
                    // Set to RED
                    *(buf + i * WIDTH * 3 + j) = (char) 255;    //R
                    *(buf + i * WIDTH * 3 + j + 1) = (char) 0;  //G
                    *(buf + i * WIDTH * 3 + j + 2) = (char) 0;  //B
                }
                else
                {
                    // Set to GREEN
                    *(buf + i * WIDTH * 3 + j) = (char) 0;  //R
                    *(buf + i * WIDTH * 3 + j + 1) = (char) 255;    //G
                    *(buf + i * WIDTH * 3 + j + 2) = (char) 0;  //B
                }
            }
        }
    }
}

// This function initializes the Thumbnail pixels to form a checkerboard type
// pattern of red, green and blue boxes.
void initializeThumbPixels(char *buf)
{
    // Initialize the values in all of the pixels
    for (int i = 0; i < THUMBHEIGHT; i++)
    {
        for (int j = 0; j < (THUMBWIDTH * 3); j += 3)
        {
            // Set R, G, B values
            if (i % 30 < 10)
            {
                if (j % 90 < 30)
                {
                    // Set to RED
                    *(buf + i * THUMBWIDTH * 3 + j) = (char) 255;    //R
                    *(buf + i * THUMBWIDTH * 3 + j + 1) = (char) 0;  //G
                    *(buf + i * THUMBWIDTH * 3 + j + 2) = (char) 0;  //B
                }
                else if (j % 90 < 60)
                {
                    // Set to GREEN
                    *(buf + i * THUMBWIDTH * 3 + j) = (char) 0;  //R
                    *(buf + i * THUMBWIDTH * 3 + j + 1) = (char) 255;    //G
                    *(buf + i * THUMBWIDTH * 3 + j + 2) = (char) 0;  //B
                }
                else
                {
                    // Set to BLUE
                    *(buf + i * THUMBWIDTH * 3 + j) = (char) 0;  //R
                    *(buf + i * THUMBWIDTH * 3 + j + 1) = (char) 0;  //G
                    *(buf + i * THUMBWIDTH * 3 + j + 2) = (char) 255;    //B
                }
            }
            else if (i % 30 < 20)
            {
                if (j % 90 < 30)
                {
                    // Set to GREEN
                    *(buf + i * THUMBWIDTH * 3 + j) = (char) 0;  //R
                    *(buf + i * THUMBWIDTH * 3 + j + 1) = (char) 255;    //G
                    *(buf + i * THUMBWIDTH * 3 + j + 2) = (char) 0;  //B
                }
                else if (j % 90 < 60)
                {
                    // Set to BLUE
                    *(buf + i * THUMBWIDTH * 3 + j) = (char) 0;  //R
                    *(buf + i * THUMBWIDTH * 3 + j + 1) = (char) 0;  //G
                    *(buf + i * THUMBWIDTH * 3 + j + 2) = (char) 255;    //B
                }
                else
                {
                    // Set to RED
                    *(buf + i * THUMBWIDTH * 3 + j) = (char) 255;    //R
                    *(buf + i * THUMBWIDTH * 3 + j + 1) = (char) 0;  //G
                    *(buf + i * THUMBWIDTH * 3 + j + 2) = (char) 0;  //B
                }
            }
            else if (i % 30 < 30)
            {
                if (j % 90 < 30)
                {
                    // Set to BLUE
                    *(buf + i * THUMBWIDTH * 3 + j) = (char) 0;  //R
                    *(buf + i * THUMBWIDTH * 3 + j + 1) = (char) 0;  //G
                    *(buf + i * THUMBWIDTH * 3 + j + 2) = (char) 255;    //B
                }
                else if (j % 90 < 60)
                {
                    // Set to RED
                    *(buf + i * THUMBWIDTH * 3 + j) = (char) 255;    //R
                    *(buf + i * THUMBWIDTH * 3 + j + 1) = (char) 0;  //G
                    *(buf + i * THUMBWIDTH * 3 + j + 2) = (char) 0;  //B
                }
                else
                {
                    // Set to GREEN
                    *(buf + i * THUMBWIDTH * 3 + j) = (char) 0;  //R
                    *(buf + i * THUMBWIDTH * 3 + j + 1) = (char) 255;    //G
                    *(buf + i * THUMBWIDTH * 3 + j + 2) = (char) 0;  //B
                }
            }
        }
    }
}
