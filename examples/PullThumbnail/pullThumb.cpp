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
 * Copyright (C) 2003 Eastman Kodak Company. All
 * Rights Reserved.
 *
 * Original Author:
 * Sam Fryer  samuel.fryer@kodak.com
 *
 * Contributor(s):
 *
 * Date: May 10, 2005
*/

/*
 * Program Usage:
 *   pmtPullThumbnail <original filename> <new thumbnail filename>
*/

// The core PMT includes ...
#include "PmtMetadataT.h"
#include "PmtDefinitionInterpreter.h"

// Each accessor that we might need has to be included so that it gets 
// set up correctly in the factory.
#include "PmtAllAccessors.h"

//Procedure to extract a thumbnail image into it's own file
// (if one exists ... otherwise create a thumbnail)
int pullAndCreateThumbnail(std::string thumbname, PmtAccessorPtr inAcc)
{
    int returnValue = 0;
    unsigned int thumbWidth = 0;
    unsigned int thumbHeight = 0;
    PmtMetadataPtr mdRoot = inAcc->readMetadata();
    char * thumbBuf = NULL;
    PmtImageRect * thumbImage = NULL;
    PmtImageAccessor * img = inAcc->getImageAccessor();
    
    img->getThumbDimensions(thumbHeight,thumbWidth);
    
    if ( (thumbWidth>0) && (thumbHeight>0) )
    {
        thumbBuf = (char*)malloc(thumbWidth*thumbHeight*3);
        thumbImage = new PmtImageRect(0,thumbWidth-1,0,thumbHeight-1,thumbBuf);
        if (!img->getThumbPixels(*thumbImage))
            returnValue = 1;
    }
    else 
        returnValue = 1;
    
    // we need to create the thumbnail data ....
    if (returnValue == 1)
    {
    }
    
    // If we've worked through all our problems, create the new Exif File
    if (returnValue == 0)
    {
        PmtAccessorPtr outAcc = new PmtExifAccessor();

        if (outAcc->open(thumbname.c_str(), PMT_CREATE) == false )
            returnValue = 2;
        else
        {
            PmtMetadataPtr software = 
                               mdRoot->getMetadatum("CaptureDevice.Software");
            software->setValueStr("pmtPullThumbnail");
            outAcc->writeMetadata(mdRoot);
            PmtImageAccessor * newImg = outAcc->getImageAccessor();
            newImg->setPixels(*thumbImage);
            outAcc->close();
        }
    }
    
    // A little cleanup for things that don't do it automagically...
    if (thumbBuf != NULL) free(thumbBuf);
    if (thumbImage != NULL) delete thumbImage;

    return returnValue;
}

int doIt(std::string filename, std::string thumbname)
{
    int return_value = 1;
    
    // The PMT may throw exceptions.  We need to be able to handle them
    // to exit a little more gracefully.
    try
    {
        // load the default metadata definition
        PmtLogicalDefinitionInterpreter interpreter;
        interpreter.load();

        // Create the appropriate file accessor
        PmtAccessorPtr inAcc = PmtAccessor::getAccessor(filename.c_str());
        
        if (inAcc)
            return_value = pullAndCreateThumbnail(thumbname,inAcc);

    }

    // Catch exceptions explicitely thrown by PMT
    catch(PmtError & e)
    {
        cout << e << endl;
        return_value = 1;
    }

    // Catch any other exceptions
    catch(...)
    {
        cout << "**** Some Exception caught." << endl;
        return_value = 1;
    }
    
    return return_value;
}

int main(int argc, char * argv[])
{
    int status = 3;
    
    // Read the input arguements
    if (argc == 3)
    {
        std::string filename,thumbname;
        
        filename = argv[1];
        thumbname = argv[2];

        // if we have a filename, dump the metadata from the file
        if ((filename.length() > 0) && (thumbname.length() > 0))
            status = doIt(filename,thumbname);

    }
    else
        printf("Usage: %s <input image filename> <new thumbnail filename>\n",
                argv[0]);

    return status;
}

