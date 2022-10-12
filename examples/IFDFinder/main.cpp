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
 * Date: February 2004
*/

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include "PmtCommon.h"

#include "PmtDefinitionInterpreter.h"
#include "PmtAllAccessors.h"
#include "PmtMetadataT.h"

// Include the file with the IFD related functions
#include "IFDFinder.h"

int main(int argc, char *argv[])
{
	// load metadata definition
    PmtLogicalDefinitionInterpreter interpreter;
    interpreter.load();

	// make sure we have at least one command line argument. take the first one
	// as the filename
	if ( argc < 3 )
	{
		cout << "Usage: IFDFinder <filename> <offset-to-IFD>" << endl;
		return 1;
	}
    
    cout << "Looking for IFD at an offset of " << argv[2] << " in the " <<
        "MakeNotes of " << argv[1] << "." <<endl;

	// The name of the file to retrieve the MakerNotes data from
	char * fileName = argv[1];
    int offsetToIFD = atoi(argv[2]);

	// Get an Accessor for the file and check that it is not null
	PmtAccessorPtr acc = PmtAccessor::getAccessor(fileName);
	if (acc.ptr() != NULL)
	{
        // We have an accessor for the file. Now, lets get the MakerNotes data
        // and after that we can close the accessor.
		PmtMetadataPtr rootMd = PmtMetadata::create();
		PmtMetadataPtr md = rootMd->getMetadatum("CaptureDevice.MakerNotes");
		acc->readMetadata(md);
		acc->close();

        // Get the value in MakerNotes
		PmtMetadataT< vector<unsigned char> >* mdT = (PmtMetadataT< vector<unsigned char> >*)md.ptr();
		vector<unsigned char> val = mdT->value();
		int count = val.size();
		cout << "Bytes in MakerNotes: " << count << endl;

		// copy the Maker Notes data to a char*
		char * MNDataOrig = new char[count];
		for ( int i = 0; i < count; i++ )
		{
			MNDataOrig[i] = (char)val[i];
		}
		
        // A new pointer to the offset specified
        char * MNData = MNDataOrig+offsetToIFD;
        
		int numOfEntries = isIFD(MNData,count-offsetToIFD);

        // if we get a -1, then there was no IFD
        if (numOfEntries >= 0)
        {
            cout << "IFD Found!!!" << endl;
		    cout << "Number of IFD entries: " << numOfEntries << endl;
        }
        else
        {
            cout << "MakerNotes doesn't contain an IFD at the specified offset"
                << endl;
            return 1;
        }

		// Now, lets try to read the entries...
        vector<IFDEntry*>* entries = getIFDEntries(MNData, count-offsetToIFD);
		if (entries==NULL)
        {
            cout << "Error encountered in getIFDEntries()" << endl;
        }
        else
        {
            // Display the entries
            cout << "tag\ttype\tcount\toffset/value" << endl;
		    for (unsigned int i=0; i<entries->size(); i++)
		    {
                cout << (*entries)[i]->tag << "\t" << (*entries)[i]->type <<
                    "\t" << (*entries)[i]->count << "\t" << 
                    (*entries)[i]->valoff << endl;
                delete (*entries)[i];
		    }
            delete entries;
		}
        delete MNDataOrig;
	}
	return 0;
}
