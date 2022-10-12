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


/*! A structure for holding an IFD entry

 */
struct IFDEntry {
	unsigned short tag; // the tag number
	unsigned short type; // the type of the value
	unsigned short count; // the number of values
	int valoff; // can contain the value(s) or an offset to them
};

/*! Determines whether the buffer passed in contains an IFD

    \param candidate the buffer we are going to look at
    \param size the size of the buffer passed in
    \return -1 if it isn't an IFD, or the number of entries if it is an IFD
 */
int isIFD(char * candidate, int size)
{
    int numOfEntries = (candidate[0]<<8) | candidate[1]; // our return value
    bool endianness = true;
    if ( numOfEntries*12 > size )
    {
        // wrong endianness
        numOfEntries = candidate[0] | (candidate[1]<<8);
        endianness = false;
        if ( numOfEntries*12 > size )
        {
            // this isn't an IFD or it has a bad count...
            // Nothing else we can do...
            numOfEntries = -1;
        }
    }

    if (numOfEntries >= 0)
    {
        // Lets do one more check to see if the type of the first entry is
        // valid.
        int typeOfFirstEntry = 0;
        if (endianness)
            typeOfFirstEntry = (candidate[4]<<8) | candidate[5];
        else
            typeOfFirstEntry = candidate[4] | (candidate[5]<<8);

        if (typeOfFirstEntry < 1 || typeOfFirstEntry > 10)
        {
            // Unknown type value in first entry
            numOfEntries = -1;
        }
    }
	
    return numOfEntries;
}

/*! Get the IFD Entries

    The application should call isIFD before calling this function to make sure
    that we are dealing with an IFD. Also, the application is responsible for
    deleting the IFDEntries and the vector created within this function.
    
    \param ifd the IFD in memory
    \param size the size of the buffer passed in
    \returns 

 */
vector<IFDEntry*>* getIFDEntries(char * ifd, int size)
{
    // lets figure out how many entries and learn about the endianness
    int numOfEntries = (ifd[0]<<8) | ifd[1]; // our return value
    bool endianness = true;
    if ( numOfEntries*12 > size )
    {
        // wrong endianness
        numOfEntries = ifd[0] | (ifd[1]<<8);
        endianness = false;
        if ( numOfEntries*12 > size )
        {
            // this isn't an IFD or it has a bad count... 
            // Nothing else we can do..
            return NULL;
        }
    }
    
    // create the vector for holding the entries
    vector<IFDEntry*> * returnValue = new vector<IFDEntry*>();
    for (int i=0; i<numOfEntries; i++)
    {
        // Create the new entry
        IFDEntry* newEntry = new IFDEntry();
        
        // Now lets do some magic based on the endianness.
        if (endianness)
        {
            newEntry->tag = (int)((ifd[i*12+2]<<8) | ifd[i*12+3]);
            newEntry->type =(int)((ifd[i*12+4]<<8) | ifd[i*12+5]);
            newEntry->count = (int)((ifd[i*12+6]<<24) | 
                (ifd[i*12+7]<<16) | (ifd[i*12+8]<<8) | ifd[i*12+9]);
            newEntry->valoff =(int)((ifd[i*12+10]<<24) | 
                (ifd[i*12+11]<<16) | (ifd[i*12+12]<<8) | ifd[i*12+13]);
        }
        else
        {
            newEntry->tag = (int)(ifd[i*12+2] | (ifd[i*12+3]<<8));
            newEntry->type = (int)(ifd[i*12+4] | (ifd[i*12+5]<<8));
            newEntry->count = (int)(ifd[i*12+6] | (ifd[i*12+7]<<8) |
					(ifd[i*12+8]<<16) | (ifd[i*12+9]<<24));
            newEntry->valoff = (int)(ifd[i*12+10] | (ifd[i*12+11]<<8) |
					(ifd[i*12+12]<<16) | (ifd[i*12+13]<<24));
        }
        returnValue->push_back(newEntry);
    }

	return returnValue;
}

