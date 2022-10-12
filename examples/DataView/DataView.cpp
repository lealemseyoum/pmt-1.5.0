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
 * Copyright (C) 2003 Eastman Kodak Company.
 *
 * Original Author:
 * Sam Fryer  samuel.fryer@kodak.com
 *
 * Contributor(s):
 *
 * Date: July, 2003
*/

/*
 * Program Usage:
 *   DataView <filename>
 *     Where <filename> is the name of the file to get the metadata from.
 *     (optional)
*/

//Load the configuration file to see if image accessors were enabled.
#include "PmtConfig.h"

// First, make sure PMT was configured correctly!
#ifndef PMT_IMAGE_ACCESSOR_REQUIRED
int main(){return 0;};
#else

#ifndef PMT_EXAMPLES_USE_X
int main(){return 0;};
#else

// System includes
#include <sstream>

// The GUI windowing code
#include "CViewImage.h"

// The core PMT includes ...
#include "PmtMetadataT.h"
#include "PmtDefinitionInterpreter.h"

// Each accessor that we might need has to be included so that it gets 
// set up correctly in the factory.
#include "PmtAllAccessors.h"

// Routine to set the recommended spacing between key and data
string getSpacing(PmtMetadataPtr md)
{
    string key = md->fullKey();
    string spacing = "";
    if (key.length() > 52) spacing = " ";
    else if (key.length() > 39) spacing = "\t";
    else if (key.length() > 31) spacing = "\t\t";
    else if (key.length() > 22) spacing = "\t\t\t";
    else if (key.length() > 12) spacing = "\t\t\t\t";
    return spacing;
}

// Procedure: readFileAndView
// Purpose:   read an image file and dump the metadata to standard out
// Inputs:    filename -- the name of the file to read
// Returns:   0 if no errors, 1 if an error was encountered.
int readFileAndView(char *filename, CViewImage * viewer)
{
    // signal the caller if we had any problems
    int return_value = 0;

    // The PMT may throw exceptions.  We need to be able to handle them
    // to exit a little more gracefully.
    try
    {
        // load the default metadata definition
        PmtLogicalDefinitionInterpreter interpreter;
        interpreter.load();

        // Create the appropriate file accessor
        PmtAccessorPtr inAcc = PmtAccessor::getAccessor(filename);

        // verify that an accessor was returned.
        if (inAcc.ptr())
        {
           
            // Read in the metadata and close the file
            PmtMetadataPtr mdRoot = inAcc->readMetadata();
            
            // Make sure that we got a valid root pointer
            if (mdRoot.ptr())
            {
                // create an iterator to go through the data
                PmtMetadataIterator mdIter = mdRoot->getMetadata("");

                // pull the first piece of metadata off
                PmtMetadataPtr md = mdIter.start();

                // Loop through all the metadata
                while (md)
                {
                    // Set the text box to use our new data
                    viewer->addMetadataItem( md->fullKey().c_str(),     //Name
                                             md->getValueStr().c_str(), //Data
                                             md->fullDocumentation());  //Tip
                
                    // Get the next piece of metadata
                    md = mdIter.next();
                }

                // Get the Image Accessor and pass it to the viewer
                PmtImageAccessorPtr imageAccessor = inAcc->getImageAccessor();

                // Assuming we got an image accessor, pass it along.
                if (imageAccessor.ptr())
                {
                    unsigned int width = 0;
                    unsigned int height = 0;
                    PmtMetadataPtr mdWidth = mdRoot->getMetadatum
                                            ("ImageContainer.ActualImageWidth");
                    if (mdWidth.ptr()) 
                        width = ((PmtMetadataT<unsigned int>*)
                                                        mdWidth.ptr())->value();
                    PmtMetadataPtr mdHeight = mdRoot->getMetadatum
                                           ("ImageContainer.ActualImageHeight");
                    if (mdHeight.ptr()) 
                        height = ((PmtMetadataT<unsigned int>*)
                                                       mdHeight.ptr())->value();
                    PmtImageRect * buffer = 
                                         new PmtImageRect(0,width-1,0,height-1);

                    if (imageAccessor->getPixels(*buffer) == 1)
                        viewer->setImage(buffer);
                }
            }
            
            // Tell the viewer to display the window
            return_value = viewer->display();

            // Once we're done displaying, close the Accessor
            inAcc->close();
        }
        else
            return_value=-1;
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

// The main entry point.
int main(int argc, char *argv[])
{
    // The name of the file we're dumping the metadata from
    char *filename = NULL;

    // The windowing code
    CViewImage viewer;
    
    // Read the input arguements
    if (argc >= 2)
    {
        // we're going to be stupid and just assume the second parameter is a
        // filename.
        filename = argv[1];
        readFileAndView(filename,&viewer);
    }
    
    viewer.start();
    
    return 0;
}

#endif

// if PMT_IMAGE_ACCESSOR_REQUIRED
#endif
