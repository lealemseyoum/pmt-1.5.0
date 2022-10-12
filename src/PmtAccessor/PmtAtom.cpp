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
 * Creation Date: 3/29/07
 *
 * Original Author: 
 * Sam Fryer <samuel.fryer@kodak.com> 
 * 
 * Contributor(s): 
 * 
 */

#include "EkCompiler.h"


#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <climits>
#include <cfloat>
#include <strstream>

#include "PmtCommon.h"
#include "PmtQtAccessor.h"
#include "PmtTranslator.h"
#include "PmtAtom.h"
#include "PmtIoFunctions.h"

// Read in atoms from a file
PmtAtom * PmtAtom::GetNextAtom(FILE *f, unsigned long long readsize, PmtAtom * theParent)
{
    PmtAtom * atom = NULL;
    if (f)
    {
        unsigned char atomheader[10];
        unsigned long long atomsize = 0;
        bool noErrors = true;
        
        if ((readsize >= 8 ) && (fread(atomheader,1,8,f) == 8))
        {
            atomsize = atomheader[0];
            atomsize = (unsigned int)(atomsize<<8) + (unsigned int)atomheader[1];
            atomsize = (unsigned int)(atomsize<<8) + (unsigned int)atomheader[2];
            atomsize = (unsigned int)(atomsize<<8) + (unsigned int)atomheader[3];

			// if the atom size is larger than 32 bits, 
			// there's no metadata we want to read!
			// So, we're simply going to skip it!
			if (atomsize == 1)
			{
				// This method really needs to be tested!
				if(fread(&atomsize,8,1,f)!=1)
				{
                    noErrors = false;
				}
			}
            
            // Note:  This "+8" garbage is needed for poorly constructed
            // files.  It could cause real problems in the future.
            if ((atomsize > readsize+8)||((atomsize<8)&&(atomsize!=1)))
                noErrors = false;
                
            if (noErrors)
            {
                SPmtString atomname = atomheader[4];
                atomname += atomheader[5];
                atomname += atomheader[6];
                atomname += atomheader[7];

                // get the correct atom type
                atom = PmtAtom::getFactory()->create(atomname);
                
                // If no atom type was found, use a null atom to skip it
                if (!atom)
                    atom = new PmtNullAtom(atomname);
                    
                atom->init(atomsize-8,f,theParent);
            }
        }
	}
    return atom;
}

// Read in atoms from a block of memory
PmtAtom * PmtAtom::GetNextAtom(unsigned char * c, unsigned long long unreadsize, PmtAtom * theParent)
{
    PmtAtom * atom = NULL;
	unsigned char * tmpbuf = c;
    if ((tmpbuf != NULL)&&(unreadsize > 8))
    {
        unsigned int atomsize = 0;
        bool noErrors = true;
        
        atomsize = (unsigned int)(*tmpbuf);tmpbuf++;
        atomsize = (unsigned int)(atomsize<<8) + (unsigned int)(*tmpbuf);tmpbuf++;
        atomsize = (unsigned int)(atomsize<<8) + (unsigned int)(*tmpbuf);tmpbuf++;
        atomsize = (unsigned int)(atomsize<<8) + (unsigned int)(*tmpbuf);tmpbuf++;

		// if the atom size is larger than 32 bits, 
		// there's no metadata we want to read!
		// So, we're simply going to skip it!
		if (atomsize == 1)
            noErrors = false;

        if (noErrors)
        {
            SPmtString atomname = (*tmpbuf);tmpbuf++;
            atomname += (*tmpbuf);tmpbuf++;
            atomname += (*tmpbuf);tmpbuf++;
            atomname += (*tmpbuf);tmpbuf++;

            // get the correct atom type
            atom = PmtAtom::getFactory()->create(atomname);

            // If no atom type was found, use a null atom to skip it
            if (!atom)
                atom = new PmtNullAtom(atomname);

            atom->init(atomsize-8,c,unreadsize,theParent);
        }
	}
    return atom;
}

PmtAtomFactoryT * PmtAtom::getFactory()
{
    static PmtAtomFactoryT theFactory;
    return &theFactory;
}

// Is recursion the best thing here?
PmtAtom * PmtAtom::getTopParentAtom()
{
    PmtAtom * top = this;
    if (parent != NULL)
        return parent->getTopParentAtom();
    return top;
}

std::vector < PmtAtomPtr > PmtAtom::getAtoms(SPmtString key)
{
	std::vector < PmtAtomPtr >  ret;
	if (key == atomType)
		ret.push_back(this);
	return ret;
}

//================= PmtContainerAtom ========================================

unsigned long long PmtContainerAtom::readAtom()
{
    unsigned long long count = 0;
    if (atomSize > 8)
    {
		// First, determing where the atom is
		if (theFile != NULL)
			offset = ftell(theFile);

        bool noErrors = true;
        unsigned long long two = atomSize;
		unsigned char * tmpbuf = theReadBuffer+8;
		unsigned long long tmpbufsize = theReadBufferSize-8;
        while ((count < two) && (noErrors))
        {
            PmtAtomPtr ap;
			if (theFile != NULL) 
				ap = PmtAtom::GetNextAtom(theFile,two-count,this);
			else
				ap = PmtAtom::GetNextAtom(tmpbuf,tmpbufsize,this);

            // The atoms should return their full size.
		    count += 8;
            if (ap)
            {
                atoms.push_back(ap);
                count += ap->readAtom();
            }
            else
                noErrors = false;
            ap = NULL;
			if (theFile == NULL)
			{
				tmpbuf = theReadBuffer+8+count;
				if (count < theReadBufferSize-8)
					tmpbufsize = theReadBufferSize-8-count;
				else
					tmpbufsize = 0;
			}
        }
    }
    
    if (count < atomSize-8)
    {
		if (theFile != NULL)
			fseek(theFile,atomSize-count-8,SEEK_CUR);
        count = atomSize;
    }
    return count;
}

unsigned long long PmtContainerAtom::writeAtom(ostream & o)
{
    std::stringstream tmpStream(ios_base::in |ios_base::out | ios_base::binary);
    unsigned long long atomSize = 8;
    
    std:: vector < PmtAtomPtr > ::iterator iter = atoms.begin();
    while(iter != atoms.end())
    {
        atomSize += (*iter)->writeAtom(tmpStream);
        iter++;
    }
    
    if ( atomSize < 0x00000000FFFFFFFF )
    {
        char * a = (char *)& atomSize;
        o << a[3] << a[2] << a[1] << a[0];
        o << atomType[0] << atomType[1] << atomType[2] << atomType[3];
		a = (char *) malloc(atomSize);
		if (a != NULL)
		{
			tmpStream.read(a,atomSize-8);
			o.write(a,atomSize-8);
			free(a);
		}
    }
    else
    {
        // I forget what we do in this case.  Need to look it up!
    }
    
    return atomSize;
}

SPmtString PmtContainerAtom::getMetadata(SPmtString key, SPmtString tag)
{
    SPmtString result;
    size_t pos = key.find_first_of('.');
    if (pos != SPmtString::npos)
    {
        SPmtString top = key.substr(0,pos);
        if (atomType == top)
        {
            SPmtString subkey = key.substr(pos+1);
            SPmtString res;
            std:: vector < PmtAtomPtr > ::iterator iter = atoms.begin();
            while((iter != atoms.end())&&(res.empty()))
            {
                res = (*iter)->getMetadata(subkey,tag);
				iter++;
            }
            result = res;
        }
    }
    return result;
}

PmtAtom * PmtContainerAtom::clone()
{
    PmtAtom * a = new PmtContainerAtom(this->atomType);
    return a;
}

std::vector < PmtAtomPtr > PmtContainerAtom::getAtoms(SPmtString key)
{
	std::vector < PmtAtomPtr >  ret;
	if (key == atomType)
		ret.push_back(this);
	else
    {
        size_t pos = key.find_first_of('.');
		if (pos != SPmtString::npos)
		{
			SPmtString top = key.substr(0,pos);
			if (atomType == top)
			{
				SPmtString subkey = key.substr(pos+1);
				std:: vector < PmtAtomPtr > res;
				std:: vector < PmtAtomPtr > ::iterator iter = atoms.begin();
				while(iter != atoms.end())
				{
					res = (*iter)->getAtoms(subkey);
					if (res.size() > 0)
					{
						std:: vector < PmtAtomPtr > ::iterator resIter = res.begin();
						while (resIter != res.end())
						{
							ret.push_back((*resIter));
							resIter++;
						}
					}
					iter++;
				}
			}
		}
	}
	return ret;
}

bool PmtContainerAtom::addAtom(PmtAtom * newAtom)
{
	bool retval = false;
	if (newAtom != NULL)
	{
		atoms.push_back(newAtom);
		newAtom->setParent(this);
		atomSize += newAtom->getAtomSize();
		retval = true;
	}
	return retval;
}

//================= PmtNullAtom ========================================

unsigned long long PmtNullAtom::readAtom()
{
	if (theFile != NULL)
	{
		// First, determing where the atom is
		offset = ftell(theFile);

		// fseek only takes a 32-bit offset, and so we must take this
		// into account as we process 64-bit offsets
		unsigned long long size64 = atomSize;
		unsigned int maxint = 0x0FFFFFFF;
		while (size64 > maxint)
		{
			fseek(theFile,maxint,SEEK_CUR);
			size64 -= maxint;
		}
		fseek(theFile,(int)size64,SEEK_CUR);
	}
    return atomSize;
}

PmtAtom * PmtNullAtom::clone()
{
    PmtAtom * a = new PmtNullAtom(this->atomType);
    return a;
}

//================= PmtStringAtom ========================================

unsigned long long PmtStringAtom::readAtom()
{
	unsigned char * c = theReadBuffer+8;
	unsigned long long numread = theReadBufferSize;
	
	if (theFile != NULL)
	{
		// First, determing where the atom is
		offset = ftell(theFile);

		c = new unsigned char[atomSize];
		numread = fread(c,1,atomSize,theFile);
	}

    if (numread >= atomSize)
    {
		unsigned int pos = 0;
		// we're only going to read character encodings of "0000" for now
		while (pos<numread-4)
		{
			unsigned int numChars = pos + 4 +((unsigned int)(c[pos])<<8) + (unsigned int)c[pos+1];
			if ((c[pos+2] == 0) && (c[pos+3] == 0)) // found good ol' ascii!
			{
				pos += 4;
				for(; pos < numChars; pos++)
					data += c[pos];
			}
			else
				pos = numChars;
		}
    }
	if (theFile != NULL)
		delete [] c;
    return numread;
}
unsigned long long PmtStringAtom::writeAtom(ostream & o)
{
    unsigned long long atomSize = data.length()+8;
    char * a = (char*)&atomSize;
    o << a[3] << a[2] << a[1] << a[0];
    o << atomType[0] << atomType[1] << atomType[2] << atomType[3];
    o << data;
    return atomSize;
}

PmtAtom * PmtStringAtom::clone()
{
    PmtAtom * a = new PmtStringAtom(this->atomType);
    return a;
}

SPmtString PmtStringAtom::getMetadata(SPmtString key, SPmtString tag)
{
	SPmtString result;
	if (key == atomType)
		result = data;
	return result;
}

//================= PmtUnicodeAtom ========================================

unsigned long long PmtUnicodeAtom::readAtom()
{
	unsigned char * c = theReadBuffer+8;
	unsigned long long numread = theReadBufferSize;
	
	if ((theFile != NULL) && (atomSize > 6))
	{
		// First, determing where the atom is
		offset = ftell(theFile);

		c = new unsigned char[atomSize];
		numread = fread(c,1,atomSize,theFile);
    }
    
    if (numread >= atomSize)
    {
        length = atomSize - 12;
		unsigned int pos = 0;
		numChars = ((unsigned int)(c[pos])<<8) + (unsigned int)c[pos+1];
        lang = ((unsigned int)(c[pos+2])<<8) + (unsigned int)c[pos+3];
		pos += 4;
        unsigned char * a = &c[5];
        unsigned char * b = &c[4];
        while(pos < atomSize)
    	{
            data.push_back(*a);
            data.push_back(*b);
            a++;a++;b++;b++;pos+=2;
        }
    }
        
    if (theFile != NULL)
        delete[] c;

    return atomSize;
}
unsigned long long PmtUnicodeAtom::writeAtom(ostream & o)
{
    unsigned long long atomSize = (length*2)+12;
    char * a = (char*)&atomSize;
    o << a[3] << a[2] << a[1] << a[0];
    o << atomType[0] << atomType[1] << atomType[2] << atomType[3];
    a = (char *)&numChars;
    o << a[1] << a[0];
    a = (char *)&lang;
    o << a[1] << a[0]; 
    std::vector<unsigned char>::iterator i = data.begin();
    unsigned char b;
    while(i != data.end()) 
    {    
        b = (*i);
        i++;
        if (i != data.end())
        {
            o << (*i) << b ;
            i++;
        } 
    }
    return atomSize;
}

PmtAtom * PmtUnicodeAtom::clone()
{
    PmtUnicodeAtom * a = new PmtUnicodeAtom(this->atomType);
    a->data = this->data;
    a->lang = this->lang;
    a->numChars = this->numChars;
    a->length = this->length;
    return a;
}

SPmtString PmtUnicodeAtom::getMetadata(SPmtString key, SPmtString tag)
{
	SPmtString result;
	if (key == atomType)
    {
        std::ostringstream os;
        PmtPrintValue(os,data);
		result = os.str().c_str();
    }
	return result;
}

void PmtUnicodeAtom::setFromString(SPmtString val)
{
    if (val.length() > 0)
    {
        std::istringstream is(val);
        PmtReadValue(is,data);
        numChars = length = data.size() / 2;
    }
}

//================= PmtAsciiStringAtom ========================================

unsigned long long PmtAsciiStringAtom::readAtom()
{
	unsigned char * c = theReadBuffer+8;
	unsigned long long numread = theReadBufferSize;
	unsigned int pos = 0;
	
	if (theFile != NULL)
	{
		// First, determing where the atom is
		offset = ftell(theFile);

		c = new unsigned char[atomSize];
		numread = fread(c,1,atomSize,theFile);
	}

    if (numread >= atomSize)
    {
		for(; ((pos < numread)&&(pos < atomSize)); pos++)
			myData += c[pos];
    }
	if (theFile != NULL)
		delete [] c;
    return pos;
}
unsigned long long PmtAsciiStringAtom::writeAtom(ostream & o)
{
    unsigned long long atomSize = myData.length()+8;
    char * a = (char*)&atomSize;
    o << a[3] << a[2] << a[1] << a[0];
    o << atomType[0] << atomType[1] << atomType[2] << atomType[3];
    o << myData;
    return atomSize;
}

PmtAtom * PmtAsciiStringAtom::clone()
{
    PmtAtom * a = new PmtAsciiStringAtom(this->atomType);
    return a;
}

SPmtString PmtAsciiStringAtom::getMetadata(SPmtString key, SPmtString tag)
{
	SPmtString result;
	if (key == atomType)
		result = myData;
	return result;
}

//================= PmtBinaryAtom ========================================

// Right now, we only convert to 2K sizes.
void PmtBinaryAtom::setFromString(SPmtString val)
{
    unsigned char buf[2048];
    if (val.length() > 0)
    {
        unsigned long long length = 0;
        SPmtString::size_type t = val.length();
        const char * c = val.c_str();
        while (t > 0)
        {
            buf[length] = atoi(c);
            length++;
            while ( (t>0) && ((*c) != ' ') && ((*c) != '\t') )
            { c++; t--; }
            while ( (t>0) && ( ((*c) == ' ') || ((*c) == '\t') ) )
            { c++; t--; }
        }
        if (length > 0)
        {
            value = malloc(length);
            memcpy(value,buf,length);
            atomSize = length;
        }
    }
}



