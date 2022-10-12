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

#ifdef PMT_SOUND_ACCESSOR_REQUIRED
#include "PmtQtAccessor.h"
#include "PmtQtAtoms.h" 
#include "PmtQtSoundAccessor.h"
#include <stdio.h>

int PmtQtSoundAccessor::readSound()
{
	return 0;
}

int PmtQtSoundAccessor::writeSoundToFile(SPmtString filename)
{
	FILE * outfile = fopen(filename.c_str(),"w+");
	if ((inFile)&&(outfile))
	{
		if ((mdat)&&(minf))
		{
			std::vector < PmtAtomPtr > atoms = minf->getAtoms("minf.stbl.stsz");
			if (atoms.size() > 0 )
			{
				PmtQtStszAtom * stsz = (PmtQtStszAtom *) atoms[0].ptr();
				atoms = minf->getAtoms("minf.stbl.stco");
				if (atoms.size() > 0)
				{
					PmtQtStcoAtom * stco = ( PmtQtStcoAtom * ) atoms[0].ptr();
					atoms = minf->getAtoms("minf.stbl.stsd");
					if (atoms.size() > 0)
					{
						PmtQtStsdAtom * stsd = ( PmtQtStsdAtom * ) atoms[0].ptr();
						atoms = minf->getAtoms("minf.stbl.stsc");
						if (atoms.size() > 0)
						{
							PmtQtStscAtom * stsc = ( PmtQtStscAtom * ) atoms[0].ptr();
							unsigned int sample = 0;
							unsigned int j = 0;
							unsigned long long chunkSize = 0;
							unsigned int nChannels = stsd->getNChannels();
							unsigned int sampleRate = stsd->getSampleRate();
							writeFileHeader(outfile,sampleRate,nChannels);
							std::vector < PmtQtStscAtom::SampleToChunk * > table = stsc->getTable();
							std::vector < PmtQtStscAtom::SampleToChunk * > :: iterator iter = 
								table.begin();
							while (iter != table.end())
							{
								unsigned int chunk = (*iter)->firstChunk;
								unsigned int samplesInChunk = (*iter)->samplesPerChunk;
								chunkSize = 0;
								for (j = sample; j< sample+samplesInChunk; j++)
									chunkSize += stsz->getSampleSize(j);
								unsigned long seekOffset = mdat->getOffset();
								seekOffset += stco->getChunkOffset(chunk);
								fseek(inFile,seekOffset,SEEK_SET);
								char * buf = new char[chunkSize];
								if (fread(buf,chunkSize,1,inFile) == chunkSize)
									fwrite(buf,chunkSize,1,outfile);
								sample += samplesInChunk;
								delete [] buf;
								iter++;
							}
						}
					}
				}
			}
		}
		fclose(outfile);
	}
	return 0;
}

int PmtQtSoundAccessor::writeFileHeader(FILE * outFile, 
										unsigned int sampleRate, unsigned int nChannels)
{
	unsigned char buf[] = 
	{
		0x2E,0x73,0x6E,0x64,	//Magic Number
		0,0,0,24,				//Header Size
		0,0,0,0,				//Data Size
		0,0,0,1,				//Encoding Scheme (uLaw 8bit)
		0,0,0,0,				//Samples Per Second
		0,0,0,1					//Number of Channels
	};

	return fwrite(buf,24,1,outFile);
}

#endif
