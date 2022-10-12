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


#ifdef PMT_QT_ACCESSOR

#ifndef PMT_QT_ATOMS__H
#define PMT_QT_ATOMS__H

#include "PmtAtom.h"


class PMTACCR_IMPL_DECL PmtQtMvhdAtom : public PmtAtom
{
    public:
        PmtQtMvhdAtom(SPmtString name) : PmtAtom (name), duration(0),timescale(0)
        {}
        virtual ~PmtQtMvhdAtom() {};
        virtual unsigned long long readAtom();
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();

    private:
		SPmtString creationTime;
		SPmtString modificationTime;
		unsigned long duration;
		unsigned long timescale;
};


class PMTACCR_IMPL_DECL PmtQtTkhdAtom : public PmtAtom
{
    public:
        PmtQtTkhdAtom(SPmtString name) : PmtAtom (name), width(0),height(0)
        {}
        virtual ~PmtQtTkhdAtom() {};
        
        virtual unsigned long long readAtom();
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();

    private:
        unsigned long width;
        unsigned long height;
};

class PMTACCR_IMPL_DECL PmtQtSttsAtom : public PmtAtom
{
    public:
        PmtQtSttsAtom(SPmtString name) : PmtAtom (name)
        {}
        virtual ~PmtQtSttsAtom() {};
        
        virtual unsigned long long readAtom();
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();

    private:
        std::vector < std::pair< unsigned int, unsigned int > * > samples;
};

class PMTACCR_IMPL_DECL PmtQtMinfAtom : public PmtContainerAtom
{
    public:
        PmtQtMinfAtom(SPmtString name) : PmtContainerAtom (name), mMinfType(MINF_VIDEO)
        {}
        virtual ~PmtQtMinfAtom() {};
        
        virtual unsigned long long readAtom();
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();

		typedef enum
		{
			MINF_VIDEO = 0,
			MINF_AUDIO,
			MINF_STILL
		} MinfType;

		MinfType getMinfType()
		{ return mMinfType; }

    private:
        MinfType mMinfType;
        
        PmtAtomPtr getStblAtom();
};

class PMTACCR_IMPL_DECL PmtQtHdlrAtom : public PmtAtom
{
    public:
        PmtQtHdlrAtom(SPmtString name) : PmtAtom (name)
		{memset(componentType,0,5);memset(componentSubtype,0,5);}
        virtual ~PmtQtHdlrAtom() {};
        
        virtual unsigned long long readAtom();
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();

    private:
        char componentType[5];
        char componentSubtype[5];
};
class PmtQtStsdAtom : public PmtAtom
{
    public:
        PmtQtStsdAtom(SPmtString name) : PmtAtom (name), data(NULL)
        {}
        virtual ~PmtQtStsdAtom() 
		{ if (data) delete [] data; };
        
        virtual unsigned long long readAtom();
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();

		unsigned int getSampleRate();
		unsigned int getUncompressedSampleSize();
		unsigned int getNChannels();

    private:
		unsigned char * data;
};

#ifdef PMT_SOUND_ACCESSOR_REQUIRED

class PmtQtStszAtom : public PmtAtom
{
    public:
        PmtQtStszAtom(SPmtString name) : PmtAtom (name), sampleSize(0)
        {}
        virtual ~PmtQtStszAtom() {};
        
        virtual unsigned long long readAtom();
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();

		unsigned int getSampleSize(unsigned int sampleNum);

    private:
		unsigned int sampleSize;
        std::vector < unsigned int > sampleSizes;
};

class PmtQtStscAtom : public PmtAtom
{
    public:

		typedef struct { int firstChunk; int samplesPerChunk; int sampleType; } SampleToChunk;
		
		PmtQtStscAtom(SPmtString name) : PmtAtom (name)
        {}
        virtual ~PmtQtStscAtom() {};
        
        virtual unsigned long long readAtom();
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();

		unsigned int getChunk(unsigned int sampleNum);
		unsigned int getSamplesPerChunk(unsigned int chunkNum);

		std::vector < SampleToChunk * > &getTable()
		{ return samplesToChunks; }

    private:

        std::vector < SampleToChunk * > samplesToChunks;
};

class PmtQtStcoAtom : public PmtAtom
{
    public:
        PmtQtStcoAtom(SPmtString name) : PmtAtom (name)
        {}
        virtual ~PmtQtStcoAtom() {};
        
        virtual unsigned long long readAtom();
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();

		unsigned int getChunkOffset(unsigned int chunkNum);

    private:
        std::vector < unsigned int > chunkOffsets;
};

#endif

#endif
#endif

