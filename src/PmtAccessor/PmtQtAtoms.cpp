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

#include "PmtConfig.h"

#ifdef PMT_QT_ACCESSOR
#include "EkCompiler.h"


#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <climits>
#include <cfloat>
#include <vector>

#include "PmtCommon.h"
#include "PmtQtAccessor.h"
#include "PmtTranslator.h"
#include "PmtAtom.h"
#include "PmtQtAtoms.h"

#if defined (unix) || defined(__unix)
// playing dangerous games here ....
// Other libraries may have defined itoa, so we'll 
// add pmt_ to it just in case.
#define itoa pmt_itoa
void pmt_itoa(const int i, char * buf, int bufSize)
{
    unsigned char * t = new unsigned char [bufSize];
    int j = 0;
    int n = i;
    int k = 0;
    char * mybuf = buf;
    int myBufSize = bufSize-1; // leave 1 for ending null
    if (n < 0) 
    {
        buf[0]='-';
        myBufSize--;
        n = -1*n;
        mybuf++;
    }
    
    while((n>0) && (j < myBufSize))
    {
        k = n/10;
        t[j++]=n-(k*10);
        n = k;
    }
    j--;
    for(k=j;k>=0;k--) mybuf[j-k]= 48 + t[k];
    mybuf[j+1]=0;
}
#endif
    
// We're going to cheat here and assume that all the videos we
// see were created AFTER Jan 1, 1970.  This may actually work
// for movies created before then, but that's for the user to test!
// Question ... is gmtime thread-safe? ... didn't think so ...
SPmtString getTimeStringFromQTTime(unsigned long t)
{
#define SECONDS_FROM_19040101_TO_19700101 2082844801
    time_t t1 = t - SECONDS_FROM_19040101_TO_19700101;
    char timestring[30];
    strftime(timestring,29,"%Y:%m:%d %H:%M:%S",gmtime(&t1));
    return timestring;
}

//================= PmtMvhdAtom ========================================

unsigned long long PmtQtMvhdAtom::readAtom()
{
    unsigned char * mvhd = new unsigned char[atomSize+1];
    unsigned int numread = fread(mvhd,1,atomSize,theFile);
    if (numread > 20)
    {
	    unsigned long c = mvhd[4];
	    c = (c<<8) + (unsigned int)mvhd[5];
	    c = (c<<8) + (unsigned int)mvhd[6];
	    c = (c<<8) + (unsigned int)mvhd[7];
	    creationTime = getTimeStringFromQTTime(c);

	    c = mvhd[8];
	    c = (c<<8) + (unsigned int)mvhd[9];
	    c = (c<<8) + (unsigned int)mvhd[10];
	    c = (c<<8) + (unsigned int)mvhd[11];
	    modificationTime = getTimeStringFromQTTime(c);

	    c = mvhd[12];
	    c = (c<<8) + (unsigned int)mvhd[13];
	    c = (c<<8) + (unsigned int)mvhd[14];
	    c = (c<<8) + (unsigned int)mvhd[15];
	    timescale = c;

	    c = mvhd[16];
	    c = (c<<8) + (unsigned int)mvhd[17];
	    c = (c<<8) + (unsigned int)mvhd[18];
	    c = (c<<8) + (unsigned int)mvhd[19];
	    duration = c;
    }

    delete [] mvhd;
    return numread;
}

SPmtString PmtQtMvhdAtom::getMetadata(SPmtString key, SPmtString tag)
{
    SPmtString result;
    char buf[100];
    if (key == atomType)
    {
	    if (tag == "CreationTime")
	    {
            result = creationTime;
	    }
	    else if (tag == "ModificationTime")
	    {
		    result = modificationTime;
	    }
	    else if (tag == "TimeScale")
	    {
            if (timescale != 0)
            {
			    itoa(timescale,buf,10);
                result = buf;
            }
	    }
	    else if (tag == "Duration")
	    {
		    if ((timescale != 0) && (duration != 0))
		    {
			    double seconds = (double)duration/(double)timescale;

				// Metadata is defined to only be # of seconds.
				// ignore other computations for now.
				//unsigned int hours = (unsigned int)(seconds / 3600);
				//seconds = seconds - (hours*3600);
				//unsigned int minutes = (unsigned int)(seconds / 60);
				//seconds = seconds - (minutes * 60);
				//sprintf(buf,"%02u:%02u:%07.4f",hours,minutes,seconds);
				sprintf(buf,"%09.4f",seconds);
                result = buf;
		    }
	    }
    }
    return result;
}

PmtAtom * PmtQtMvhdAtom::clone()
{
    PmtAtom * a = new PmtQtMvhdAtom(this->atomType);
    return a;
}


//================= PmtTkhdAtom ========================================

unsigned long long PmtQtTkhdAtom::readAtom()
{
    unsigned char * data = new unsigned char[atomSize+1];
    unsigned int numread = fread(data,1,atomSize,theFile);
    if (numread > 83)
    {
		unsigned long tw = data[74];
		tw = (tw<<8) + (unsigned int)data[75];
		tw = (tw<<8) + (unsigned int)data[76];
		tw = (tw<<8) + (unsigned int)data[77];
		if (width < tw) width = tw;
		unsigned long th = data[78];
		th = (th<<8) + (unsigned int)data[79];
		th = (th<<8) + (unsigned int)data[80];
		th = (th<<8) + (unsigned int)data[81];
		if (height < th) height = th;
    }
    delete [] data;
    return numread;
}

SPmtString PmtQtTkhdAtom::getMetadata(SPmtString key, SPmtString tag)
{
    SPmtString result;
    char buf[32];
    if (key == "tkhd")
    {
	    if (tag == "TrackWidth")
	    {
		    if (width > 0)
		    {
			    itoa(width,buf,10);
			    result = buf;
		    }
	    }
	    else if (tag == "TrackHeight")
	    {
		    if (height > 0)
		    {
			    itoa(height,buf,10);
			    result = buf;
		    }
	    }
    }
    return result;
}

PmtAtom * PmtQtTkhdAtom::clone()
{
    PmtAtom * a = new PmtQtTkhdAtom(this->atomType);
    return a;
}


//================= PmtSttsAtom ========================================

unsigned long long PmtQtSttsAtom::readAtom()
{
    unsigned char * data = new unsigned char[atomSize+1];
    unsigned int numread = fread(data,1,atomSize,theFile);
    if (numread > 15)
    {
		unsigned int numEntries = ((((((unsigned int)data[4]<<8) + 
					                 (unsigned int)data[5])<<8) +
					                 (unsigned int)data[6])<<8) +
					                 (unsigned int)data[7];
		numEntries = numEntries*8+8;
		if (numEntries <= atomSize)
		{
			unsigned int sampleCount = 0;
			unsigned int sampleDuration = 0;
			for (unsigned int i=8;(i<numEntries);i=i+8) 
			{
				sampleCount+= ((((((unsigned int)data[i]<<8) + 
								 (unsigned int)data[i+1])<<8) +
								 (unsigned int)data[i+2])<<8) +
								 (unsigned int)data[i+3];
				sampleDuration = ((((((unsigned int)data[i+4]<<8) + 
								 (unsigned int)data[i+5])<<8) +
								 (unsigned int)data[i+6])<<8) +
								 (unsigned int)data[i+7];
                samples.push_back(new std::pair < unsigned int, unsigned int > (sampleCount,sampleDuration) );
			}
		}
    }
    delete [] data;
    return numread;
}

SPmtString PmtQtSttsAtom::getMetadata(SPmtString key, SPmtString tag)
{
    SPmtString result;
    char buf[32];
    if (key == this->atomType)
    {
        if (tag == "sampleCount")
        {
	        if (samples.size() > 0)
	        {
		        itoa(samples[0]->first,buf,10);
		        result = buf;
	        }
        }
        else if (tag == "sampleDuration")
        {
	        if (samples.size() > 0)
	        {
		        itoa(samples[0]->second,buf,10);
		        result = buf;
	        }
        }
    }
    return result;
}

PmtAtom * PmtQtSttsAtom::clone()
{
    PmtAtom * a = new PmtQtSttsAtom(this->atomType);
    return a;
}

//================= PmtQtHdlrAtom ========================================

unsigned long long PmtQtHdlrAtom::readAtom()
{
    unsigned char * data = new unsigned char[atomSize+1];
    unsigned int numread = fread(data,1,atomSize,theFile);
    if (numread > 11)
    {
		componentType[0] = data[4];
		componentType[1] = data[5];
		componentType[2] = data[6];
		componentType[3] = data[7];

		componentSubtype[0] = data[8];
		componentSubtype[1] = data[9];
		componentSubtype[2] = data[10];
		componentSubtype[3] = data[11];
    }
    delete [] data;
    return numread;
}

SPmtString PmtQtHdlrAtom::getMetadata(SPmtString key, SPmtString tag)
{
    SPmtString result;
    if (key == "tkhd")
    {
	    if (tag == "ComponentType")
	    {
			result = componentType;
	    }
	    else if (tag == "ComponentSubtype")
		{
			result = componentSubtype;
	    }
    }
    return result;
}

PmtAtom * PmtQtHdlrAtom::clone()
{
    PmtAtom * a = new PmtQtHdlrAtom(this->atomType);
    return a;
}

//================= PmtQtStsdAtom ========================================

unsigned long long PmtQtStsdAtom::readAtom()
{
    data = new unsigned char[atomSize+1];
    unsigned int numread = fread(data,1,atomSize,theFile);
    if (numread != atomSize)
    {
		delete [] data;
		data = NULL;
    }
    return numread;
}

SPmtString PmtQtStsdAtom::getMetadata(SPmtString key, SPmtString tag)
{
    SPmtString result;
    if ((key == "stsd")&&(data)&&(atomSize > 17))
    {
	    if (tag == "Format")
	    {
			result += data[12];
			result += data[13];
			result += data[14];
			result += data[15];
	    }
		else if (tag == "SampleRate")
		{
			unsigned int sampleRate = this->getSampleRate();
			if (sampleRate>0)
			{
				char buf[32];
				itoa(sampleRate,buf,10);
				result = buf;
			}
		}
    }
    return result;
}

unsigned int PmtQtStsdAtom::getSampleRate()
{
	unsigned int sampleRate = 0;
	if ((data)&&(atomSize>=44))
	{
		sampleRate=((((
			       (((unsigned int)data[40])<<8)+
			       ((unsigned int)data[41]))<<8) +
			       ((unsigned int)data[42]))<<8) +
			       ((unsigned int)data[43]);
	}
	return sampleRate;
}

unsigned int PmtQtStsdAtom::getUncompressedSampleSize()
{
	unsigned int sampleSize = 0;
	if ((data)&&(atomSize>=44))
	{
		sampleSize=(((unsigned int)data[30])<<8)+
			       ((unsigned int)data[31]);
	}
	return sampleSize;
}

unsigned int PmtQtStsdAtom::getNChannels()
{
	unsigned int nChannels = 0;
	if ((data)&&(atomSize>=44))
	{
		nChannels=(((unsigned int)data[28])<<8)+
			       ((unsigned int)data[29]);
	}
	return nChannels;
}

PmtAtom * PmtQtStsdAtom::clone()
{
    PmtAtom * a = new PmtQtStsdAtom(this->atomType);
    return a;
}


#ifdef PMT_SOUND_ACCESSOR_REQUIRED
//================= PmtStszAtom ========================================

unsigned long long PmtQtStszAtom::readAtom()
{
    unsigned char * data = new unsigned char[atomSize+1];
    unsigned int numread = fread(data,1,atomSize,theFile);
    if (numread > 11)
    {
		sampleSize = ((((((unsigned int)data[4]<<8) + 
		                  (unsigned int)data[5])<<8) +
		                  (unsigned int)data[6])<<8) +
		                  (unsigned int)data[7];
		unsigned int numEntries = ((((((unsigned int)data[8]<<8) + 
					                 (unsigned int)data[9])<<8) +
					                 (unsigned int)data[10])<<8) +
					                 (unsigned int)data[11];
		numEntries = numEntries*4+12;
		if (numEntries <= atomSize)
		{
			unsigned int theSize = 0;
			for (unsigned int i=12;(i<numEntries);i=i+4) 
			{
				theSize = ((((((unsigned int)data[i]<<8) + 
								 (unsigned int)data[i+1])<<8) +
								 (unsigned int)data[i+2])<<8) +
								 (unsigned int)data[i+3];
                sampleSizes.push_back( theSize );
			}
		}
    }
    delete [] data;
    return numread;
}

SPmtString PmtQtStszAtom::getMetadata(SPmtString key, SPmtString tag)
{
    SPmtString result;
	if ((key == atomType) && (tag == "SampleSize"))
	{
		char buf[32];
		itoa(sampleSize,buf,10);
		result = buf;
	}
    return result;
}

PmtAtom * PmtQtStszAtom::clone()
{
    PmtAtom * a = new PmtQtStszAtom(this->atomType);
    return a;
}

unsigned int PmtQtStszAtom::getSampleSize(unsigned int sampleNum)
{
	unsigned int result = 0;
	if (sampleSize > 0)
		result = sampleSize;
	else
		if (sampleNum < sampleSizes.size())
			result = sampleSizes[sampleNum];
    return result;
}

//================= PmtStscAtom ========================================

unsigned long long PmtQtStscAtom::readAtom()
{
    unsigned char * data = new unsigned char[atomSize+1];
    unsigned int numread = fread(data,1,atomSize,theFile);
    if (numread > 7)
    {
		unsigned int numEntries = ((((((unsigned int)data[4]<<8) + 
					                 (unsigned int)data[5])<<8) +
					                 (unsigned int)data[6])<<8) +
					                 (unsigned int)data[7];
		numEntries = numEntries*4+8;
		if (numEntries <= atomSize)
		{
			SampleToChunk * stc = NULL;
			for (unsigned int i=8;(i<numEntries);i=i+4) 
			{
				stc = new SampleToChunk();
				stc->firstChunk = ((((((unsigned int)data[i]<<8) + 
								 (unsigned int)data[i+1])<<8) +
								 (unsigned int)data[i+2])<<8) +
								 (unsigned int)data[i+3];
				stc->samplesPerChunk = ((((((unsigned int)data[i+4]<<8) + 
								 (unsigned int)data[i+5])<<8) +
								 (unsigned int)data[i+6])<<8) +
								 (unsigned int)data[i+7];
				stc->sampleType = ((((((unsigned int)data[i+8]<<8) + 
								 (unsigned int)data[i+9])<<8) +
								 (unsigned int)data[i+10])<<8) +
								 (unsigned int)data[i+11];
                samplesToChunks.push_back( stc );
			}
		}
    }
    delete [] data;
    return numread;
}

SPmtString PmtQtStscAtom::getMetadata(SPmtString key, SPmtString tag)
{
    SPmtString result;
    return result;
}

PmtAtom * PmtQtStscAtom::clone()
{
    PmtAtom * a = new PmtQtStscAtom(this->atomType);
    return a;
}

unsigned int PmtQtStscAtom::getChunk(unsigned int sampleNum)
{
	unsigned int result = 0;
	std::vector < SampleToChunk * > :: iterator iter = samplesToChunks.begin();
	unsigned int currSample = 0;
	while ((iter != samplesToChunks.end()) &&
		(sampleNum > currSample + (*iter)->samplesPerChunk))
	{
		currSample += (*iter)->samplesPerChunk;
		iter++;
	}

	if (iter != samplesToChunks.end())
		result = (*iter)->firstChunk;
    return result;
}

unsigned int PmtQtStscAtom::getSamplesPerChunk(unsigned int chunkNum)
{
	unsigned int result = 0;
	std::vector < SampleToChunk * > :: iterator iter = samplesToChunks.begin();
	while ((iter != samplesToChunks.end()) &&
		(chunkNum != (unsigned int)(*iter)->firstChunk))
		iter++;

	if (iter != samplesToChunks.end())
		result = (*iter)->samplesPerChunk;
    return result;
}

//================= PmtStcoAtom ========================================

unsigned long long PmtQtStcoAtom::readAtom()
{
    unsigned char * data = new unsigned char[atomSize+1];
    unsigned int numread = fread(data,1,atomSize,theFile);
    if (numread > 15)
    {
		unsigned int numEntries = ((((((unsigned int)data[4]<<8) + 
					                 (unsigned int)data[5])<<8) +
					                 (unsigned int)data[6])<<8) +
					                 (unsigned int)data[7];
		numEntries = numEntries*4+8;
		if (numEntries <= atomSize)
		{
			unsigned int offset = 0;
			for (unsigned int i=8;(i<numEntries);i=i+4) 
			{
				offset+= ((((((unsigned int)data[i]<<8) + 
								 (unsigned int)data[i+1])<<8) +
								 (unsigned int)data[i+2])<<8) +
								 (unsigned int)data[i+3];
                chunkOffsets.push_back( offset );
			}
		}
    }
    delete [] data;
    return numread;
}

SPmtString PmtQtStcoAtom::getMetadata(SPmtString key, SPmtString tag)
{
    SPmtString result;
    return result;
}

PmtAtom * PmtQtStcoAtom::clone()
{
    PmtAtom * a = new PmtQtStcoAtom(this->atomType);
    return a;
}

unsigned int PmtQtStcoAtom::getChunkOffset(unsigned int chunkNum)
{
	unsigned int result = 0;
	if (chunkNum < chunkOffsets.size())
		result = chunkOffsets[chunkNum];
    return result;
}
#endif

//================= PmtQtMinfAtom ========================================

unsigned long long PmtQtMinfAtom::readAtom()
{
    unsigned long long count = 0;
    bool noErrors = true;
    while ((count < atomSize) && (noErrors))
    {
        PmtAtomPtr ap = PmtAtom::GetNextAtom(theFile,atomSize-count,this);
		count += 8;
        if (ap)
        {
            atoms.push_back(ap);
            count += ap->readAtom();
            if (ap->getAtomType() == "smhd")
                this->mMinfType = MINF_AUDIO;
        }
        else
            noErrors = false;
        ap = NULL;
    }
    return count;
}

SPmtString PmtQtMinfAtom::getMetadata(SPmtString key, SPmtString tag)
{
    SPmtString result;
    char buf[100];
    if (key == atomType)
    {
	    if ((tag == "FrameCount") && (mMinfType == MINF_VIDEO))
	    {
            PmtAtomPtr stbl = this->getStblAtom();
            if (stbl)
                result = stbl->getMetadata("stbl.stts","sampleCount");
	    }
	    else if ((tag == "FrameRate") && (mMinfType == MINF_VIDEO))
	    {
            PmtAtom * moov = this->getTopParentAtom();
            PmtAtomPtr stbl = this->getStblAtom();
            if ((stbl)&&(moov))
            {
                SPmtString tmp = stbl->getMetadata("stbl.stts","sampleDuration");
                if (!tmp.empty())
                {
                    unsigned long sd = atoi(tmp.c_str());
                    tmp = moov->getMetadata("moov.mvhd","TimeScale");
                    if ((!tmp.empty()) && (sd != 0))
                    {
                        unsigned long ts = atoi(tmp.c_str());
                        sprintf(buf,"%07.4f",(double)ts/(double)sd);
                        result = buf;
                    }
                }
                
            }
	    }
	    else if ((tag == "SoundSampleRate") && (mMinfType == MINF_AUDIO))
	    {
            PmtAtom * moov = this->getTopParentAtom();
            PmtAtomPtr stbl = this->getStblAtom();
            if ((stbl)&&(moov))
            {
                result = stbl->getMetadata("stbl.stsd","SampleRate");
            }
	    }
    }
    return result;
}

PmtAtom * PmtQtMinfAtom::clone()
{
    PmtAtom * a = new PmtQtMinfAtom(this->atomType);
    return a;
}

PmtAtomPtr PmtQtMinfAtom::getStblAtom()
{
    PmtAtomPtr atom;
    std:: vector < PmtAtomPtr > ::iterator iter = atoms.begin();
    while((iter != atoms.end())&&(!atom))
    {
        if ((*iter)->getAtomType() == "stbl")
            atom = (*iter);
		iter++;
    }
    return atom;
}

#endif
