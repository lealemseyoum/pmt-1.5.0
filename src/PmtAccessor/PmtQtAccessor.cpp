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
 * Creation Date: 7/17/06
 *
 * Original Author: 
 * Sam Fryer <samuel.fryer@kodak.com> 
 * 
 * Contributor(s): 
 * 
 */

#include "EkCompiler.h"
#include "PmtConfig.h"

#ifdef PMT_QT_ACCESSOR

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <climits>
#include <cfloat>

#include "PmtCommon.h"
#include "PmtQtAccessor.h"
#include "PmtTranslator.h"
#include "PmtAtom.h"
#include "PmtQtAtoms.h"

#define PMT_QUICKTIME_ELEMENT_NAME "QT"

//! Default constructor
PmtQTAccessor::PmtQTAccessor():PmtAccessor(PMT_FORMAT_QUICKTIME), mQTFile(NULL),
#ifdef PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
mImgAccessor(NULL),
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
#ifdef PMT_SOUND_ACCESSOR_REQUIRED
mSndAccessor(NULL),
#endif                          // PMT_SOUND_ACCESSOR_REQUIRED
mClosing(false)
{
}

//! The copy constructor
PmtQTAccessor::
PmtQTAccessor(const PmtQTAccessor & src):PmtAccessor(PMT_FORMAT_QUICKTIME),
mQTFile(src.mQTFile),
#ifdef PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
mImgAccessor(NULL),
#endif
       // PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
mClosing(false)
{
}

PmtQTAccessor::~PmtQTAccessor()
{
    close();
}


void PmtQTAccessor::close()
{
    if (!mClosing && mQTFile)
    {
        mClosing = true;
        fclose( mQTFile );
        mQTFile = NULL;
    }

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
    // if we have an image accessor, delete it
    if (mImgAccessor)
    {
        delete mImgAccessor;
        mImgAccessor = NULL;
    }
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED

    mClosing = false;
}


bool PmtQTAccessor::open(const char *name, PmtOpenMode mode)
{
    bool bRet = false;
    close();

    char openMode[10];
    if (mode == PMT_READONLY)
    {
		openMode[0] = 'r';
		openMode[1] = 'b';
		openMode[2] = 0;
    }
    else 
    {
        SPmtString m = "PmtQTAccessor can only read files.";
        throw PmtError(PMT_ACCESSOR_FILE_READ_ONLY, m, WHERE);
    }


    mQTFile = fopen(name, openMode);
    if (mQTFile)
        bRet = true;

    return bRet;
}


// Recognize whether this might be a QT file or not by
// reading in the first four bytes and checking against a magic number.
bool PmtQTAccessor::recognize(const SPmtString & name)
{
    bool bRet = false;

    FILE *f = fopen(name.c_str(), "rb");
    if (f)
    {
        rewind(f);
        char n[8];
        int m = fread(&n, 8, 1, f);
        if (m > 0)
            bRet = ((n[4]=='s')&&(n[5]=='k')&&(n[6]=='i')&&(n[7]=='p'))||
			       ((n[4]=='m')&&(n[5]=='o')&&(n[6]=='o')&&(n[7]=='v'))||
				   ((n[4]=='m')&&(n[5]=='d')&&(n[6]=='a')&&(n[7]=='t'))||
				   ((n[4]=='p')&&(n[5]=='n')&&(n[6]=='o')&&(n[7]=='t'))||
				   ((n[4]=='f')&&(n[5]=='t')&&(n[6]=='y')&&(n[7]=='p'))||
				   ((n[4]=='w')&&(n[5]=='i')&&(n[6]=='d')&&(n[7]=='e'))||
				   ((n[4]=='f')&&(n[5]=='r')&&(n[6]=='e')&&(n[7]=='e'));
        fclose(f);
    }

    return bRet;
}


PmtMetadataPtr PmtQTAccessor::readMetadata(const SPmtString nsURI)
{
    loadIfAny();
    loadAtomFactory();
    PmtMetadataPtr rootMd = PmtMetadata::create(nsURI);
//    EK_GUARD(EK_DEFAULT_MUTEX, msLock);

    // Get the translation table column for this accessor
    PmtTranslationTableElements * theTable = 
                  msTranslationTable->getSubTable(PMT_QUICKTIME_ELEMENT_NAME);

    if (theTable != NULL)
	{
		parseQuickTimeFile(mQTFile,rootMd,theTable);
//		PmtSoundAccessorPtr s = getSoundAccessor();
//		s->writeSoundToFile("C:\\temp\\test.au");
	}
    return rootMd;
}

// We probably could put some type of configuration mechanism here
void PmtQTAccessor::loadAtomFactory()
{
    static bool factoryLoaded = false;
    if(!factoryLoaded)
    {
        factoryLoaded = true;
        PmtAtomFactoryT * theFactory = PmtAtom::getFactory();
        theFactory->addEntry("moov",new PmtContainerAtom("moov"));
        theFactory->addEntry("udta",new PmtContainerAtom("udta"));
        theFactory->addEntry("trak",new PmtContainerAtom("trak"));
        theFactory->addEntry("mdia",new PmtContainerAtom("mdia"));
        theFactory->addEntry("stbl",new PmtContainerAtom("stbl"));
        theFactory->addEntry("DcMD",new PmtContainerAtom("DcMD"));
        theFactory->addEntry("GPSi",new PmtContainerAtom("GPSi"));
        theFactory->addEntry("DcME",new PmtContainerAtom("DcME"));
        theFactory->addEntry("DcEM",new PmtContainerAtom("DcEM"));
        theFactory->addEntry("DcEK",new PmtContainerAtom("DcEK"));
//        theFactory->addEntry("vmhd",new PmtContainerAtom("vmhd"));
//        theFactory->addEntry("smhd",new PmtContainerAtom("smhd"));
        theFactory->addEntry("mvhd",new PmtQtMvhdAtom("mvhd"));
        theFactory->addEntry("tkhd",new PmtQtTkhdAtom("tkhd"));
        theFactory->addEntry("stts",new PmtQtSttsAtom("stts"));
        theFactory->addEntry("©inf",new PmtStringAtom("©inf"));
        theFactory->addEntry("©req",new PmtStringAtom("©req"));
        theFactory->addEntry("©fmt",new PmtStringAtom("©fmt"));
        theFactory->addEntry("©cpy",new PmtStringAtom("©cpy"));
        theFactory->addEntry("©day",new PmtStringAtom("©day"));
        theFactory->addEntry("XMP_",new PmtAsciiStringAtom("XMP_"));
        theFactory->addEntry("Titl",new PmtUnicodeAtom("Titl"));
        theFactory->addEntry("Comm",new PmtUnicodeAtom("Comm"));
        theFactory->addEntry("Keyw",new PmtUnicodeAtom("Keyw"));
        theFactory->addEntry("Rate",new PmtSimpleTypeAtom<unsigned short>("Rate"));
        theFactory->addEntry("minf",new PmtQtMinfAtom("minf"));
        theFactory->addEntry("hdlr",new PmtQtHdlrAtom("hdlr"));
        theFactory->addEntry("stsd",new PmtQtStsdAtom("stsd"));
#ifdef PMT_SOUND_ACCESSOR_REQUIRED
        theFactory->addEntry("stsz",new PmtQtStszAtom("stsz"));
        theFactory->addEntry("stsc",new PmtQtStscAtom("stsc"));
        theFactory->addEntry("stco",new PmtQtStcoAtom("stco"));
#endif
	}
}

bool PmtQTAccessor::getMdValue(PmtMetadataPtr & md)
{
    // We're not supporting reading individual values at this time
    // use "readMetadata" instead
    return false;
}


bool PmtQTAccessor::setMdValue(const PmtMetadataPtr & md)
{
    // right now, we're a read-only operation!
    return false;
}



#ifdef PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
// This method checks to see if we have an open file and if so returns an
// image accessor for it. Otherwise, it returns NULL
PmtImageAccessor *PmtQTAccessor::getImageAccessor()
{
    // Check to see if we have an open file
    if (mQTFile)
    {
        // Check if we already have an image accessor for this file. Create it if
        // necessary.
        if (!mImgAccessor)
            mImgAccessor = new PmtQuickTimeImageAccessor(mQTFile);
        else
            mImgAccessor->setImageFile(mQTFile);
    }

    return mImgAccessor;
}
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED

#ifdef PMT_SOUND_ACCESSOR_REQUIRED
// This method checks to see if we have an open file and if so returns an
// image accessor for it. Otherwise, it returns NULL
PmtSoundAccessorPtr PmtQTAccessor::getSoundAccessor()
{
    // Check to see if we have an open file
    if (mQTFile)
    {
        // Check if we already have an image accessor for this file. Create it if
        // necessary.
        if (!mSndAccessor)
		{
            std::vector < PmtAtomPtr > :: iterator iter = mAtoms.begin();
			std::vector < PmtAtomPtr > minfs;
 			PmtAtomPtr minf;
			PmtAtomPtr mdat;
            while ((iter != mAtoms.end()) && (!minf))
            {
                minfs = (*iter)->getAtoms("moov.trak.mdia.minf");
 				if (minfs.size() > 0)
				{
					for (unsigned int i=0; i<minfs.size() && !minf ; i++)
					{
						if (((PmtQtMinfAtom*)(minfs[i].ptr()))->getMinfType() == PmtQtMinfAtom::MINF_AUDIO)
							minf = minfs[i];
					}
				}
               iter++;
            }
            
			iter = mAtoms.begin();
            while ((iter != mAtoms.end()) && (!mdat))
            {
                minfs = (*iter)->getAtoms("mdat");
 				if (minfs.size() > 0)
					mdat = minfs[0];
               iter++;
            }
			if ((minf)&&(mdat))
			{
				PmtQtSoundAccessor * snd = new PmtQtSoundAccessor();
				snd->setMinfAtom(minf);
				snd->setMdatAtom(mdat);
				snd->setFile(mQTFile);
				mSndAccessor = snd;
			}
		}
    }

    return mSndAccessor;
}
#endif                          // PMT_SOUND_ACCESSOR_REQUIRED

// This method starts to parse the file
bool PmtQTAccessor::parseQuickTimeFile(FILE *f, PmtMetadataPtr rootMd, 
									   PmtTranslationTableElements * theTTable)
{
    bool noErrors = true;
    if (f)
    {
        fseek(f,0,SEEK_END);
        unsigned long filesize = ftell(f);
        fseek(f,0,SEEK_SET);
        unsigned long currPos = ftell(f);

        while ((!feof(f))&&(currPos < filesize)&&(noErrors))
        {
            PmtAtomPtr atom = PmtAtom::GetNextAtom(f,filesize-currPos);
            if (atom)
            {
                mAtoms.push_back(atom);
                currPos += atom->readAtom();
            }
            atom = NULL;
        }
        
        // Now that we've read the atoms, let's get the metadata
       	int len = theTTable->size();
	    int i = 0;
	    PmtMetadataPtr md;
	    for (i = 0; i<len; i++)
	    {
		    PmtTranslationTableElement * telement = theTTable->operator[](i);
		    if (telement != NULL)
		    {
                std::vector < PmtAtomPtr > :: iterator iter = mAtoms.begin();
                SPmtString value;
                while ((iter != mAtoms.end()) && (value.empty()))
                {
                    value = (*iter)->getMetadata(telement->mLocation,telement->mTag);
                    iter++;
                }
                if (!value.empty())
                {
					md = rootMd->getMetadatum(telement->mKey,true);
					if (md)
						md->setValueStr(EK_STR2WIDE(value));
				}
		    }
	    }
	}
#ifdef PMT_XMP_ACCESSOR
	//Get metadatas from XMP block.
	std::vector<PmtAtomPtr>::iterator itAtom;
	SPmtString value;
	for (itAtom = mAtoms.begin(); itAtom != mAtoms.end(); itAtom ++)
	{
		value = (*itAtom)->getMetadata("moov.udta.XMP_", "XMP_");
		if(!value.empty())
        {
			if(mXmpAccessor.open((const unsigned char*)(value.c_str()),value.size(), PMT_READONLY))
		        mXmpAccessor.readMetadata(rootMd, true);
        }
	}
	
#endif
    return noErrors;
}


#endif // PMT_QT_ACCESSOR

