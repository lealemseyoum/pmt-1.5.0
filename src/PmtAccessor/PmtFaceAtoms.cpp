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
 * Creation Date: 1/16/09
 *
 * Original Author: 
 * Sam Fryer <samuel.fryer@kodak.com> 
 * 
 * Contributor(s): 
 * 
 */

#include "PmtConfig.h"
#include "EkCompiler.h"


#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <climits>
#include <cfloat>
#include <vector>

#include "PmtIoFunctions.h"
#include "PmtFaceAtoms.h"

// note: ffob is a container atom.
//       obID is a string atom.
//       obNa is a string atom.
void PmtInitializeFaceAtoms()
{
	static bool factoryLoaded = false;
    if(!factoryLoaded)
    {
        factoryLoaded = true;
        PmtAtomFactoryT * theFactory = PmtAtom::getFactory();

		// First, the face container atom
		theFactory->addEntry("ffob",new PmtContainerAtom("ffob"));

		// Now, the individual atoms
        theFactory->addEntry("obID",new PmtAsciiStringAtom("obID"));
        theFactory->addEntry("obNa",new PmtUnicodeAtom("obNa"));
        theFactory->addEntry("fpdt",new PmtFaceFpdtAtom("fpdt"));
		theFactory->addEntry("leye",new PmtFacePtAtom("leye"));
        theFactory->addEntry("reye",new PmtFacePtAtom("reye"));
        theFactory->addEntry("LOEC",new PmtFacePtAtom("LOEC"));
        theFactory->addEntry("LIEC",new PmtFacePtAtom("LIEC"));
        theFactory->addEntry("ROEC",new PmtFacePtAtom("ROEC"));
        theFactory->addEntry("RIEC",new PmtFacePtAtom("RIEC"));
        theFactory->addEntry("LMCP",new PmtFacePtAtom("LMCP"));
        theFactory->addEntry("RMCP",new PmtFacePtAtom("RMCP"));
        theFactory->addEntry("obEv",new PmtFaceFloatVecAtom("obEv"));
        theFactory->addEntry("fout",new PmtFacePtVecAtom("fout"));
        theFactory->addEntry("uCON",new PmtFaceUCONAtom("uCON"));
        theFactory->addEntry("FPPA",new PmtSimpleTypeAtom<int>("FPPA"));
        theFactory->addEntry("FPYA",new PmtSimpleTypeAtom<int>("FPYA"));
        theFactory->addEntry("FPRA",new PmtSimpleTypeAtom<int>("FPRA"));
        theFactory->addEntry("FEAD",new PmtBinaryAtom("FEAD"));
        theFactory->addEntry("FPCL",new PmtSimpleTypeAtom<unsigned short>("FPCL"));
	}
}

PmtFaceFloatPtr PmtFaceReadFloat(unsigned char * buf, int bufsize, 
								 FILE * theFile, unsigned short rep)
{
	PmtFaceFloatPtr pt;
	unsigned char * mybuf;
	unsigned char bufcache[9];
	if (buf != NULL) mybuf = buf;
	else mybuf = bufcache;
	if (theFile != NULL)
	{
		fread(mybuf,1,8,theFile);
	}
	if (rep == 1)
	{
		int numerator = *mybuf; mybuf++;
		numerator = numerator << 8; numerator += *mybuf; mybuf++;
		numerator = numerator << 8; numerator += *mybuf; mybuf++;
		numerator = numerator << 8; numerator += *mybuf; mybuf++;
		int denominator = *mybuf; mybuf++;
		denominator = denominator << 8; denominator += *mybuf; mybuf++;
		denominator = denominator << 8; denominator += *mybuf; mybuf++;
		denominator = denominator << 8; denominator += *mybuf; mybuf++;
		pt = new PmtFaceFloat(numerator,denominator);
	}
	else
	{
		char a[8];
		a[7]=buf[0];a[6]=buf[1];a[5]=buf[2];a[4]=buf[3];
		a[3]=buf[4];a[2]=buf[5];a[1]=buf[6];a[0]=buf[7];
		pt = new PmtFaceFloat(*(double *)a);
	}
	return pt;
}


//================= PmtFaceFpdtAtom ========================================

unsigned long long PmtFaceFpdtAtom::readAtom() 
{
	floatingPointRepresentation = 0; 
	if (theFile != NULL)
	{
		unsigned char buf[3];
		if (fread(buf,1,2,theFile)==2)
			floatingPointRepresentation = (unsigned long long)buf[1];
	}
	else
		if ((theReadBuffer != NULL)&&(theReadBufferSize > 9))
			floatingPointRepresentation = (unsigned long long)theReadBuffer[9];

	return atomSize;
}
unsigned long long PmtFaceFpdtAtom::writeAtom(ostream & o)
{
    o << (char)0 << (char)0 << (char)0 << (char)10;
    o << atomType[0] << atomType[1] << atomType[2] << atomType[3];
    o << (char)0 << (char)floatingPointRepresentation;
    return 10;
}

PmtAtom * PmtFaceFpdtAtom::clone()
{
    PmtFaceFpdtAtom * a = new PmtFaceFpdtAtom(this->atomType);
	a->floatingPointRepresentation = this->floatingPointRepresentation;
    return a;
}

SPmtString PmtFaceFpdtAtom::getMetadata(SPmtString key, SPmtString tag)
{
	SPmtString result;
	if (key == atomType)
	{
		if (floatingPointRepresentation == 1)
			result = "RATIONAL";
		else if (floatingPointRepresentation == 2)
			result = "DOUBLE PRECISION";
	}
	return result;
}

void PmtFaceFpdtAtom::setFromString(SPmtString val)
{
    if (val == "RATIONAL") floatingPointRepresentation = 1;
    else if (val == "DOUBLE PRECISION") floatingPointRepresentation = 2;
    else
    {
        floatingPointRepresentation = 1; //default to 1
        int f = atoi(val.c_str());
        if (f == 2) floatingPointRepresentation = 2;
    }
}

//================= PmtFaceFloatVecAtom ========================================

unsigned long long PmtFaceFloatVecAtom::readAtom() 
{
	std::vector<PmtAtomPtr> floatRepAtoms = this->parent->getAtoms("ffob.fpdt");
	unsigned short floatingPointRepresentation = 1; 
	if (floatRepAtoms.size() > 0) 
		floatingPointRepresentation = ((PmtFaceFpdtAtom*)(floatRepAtoms[0].ptr()))->floatingPointRepresentation;

	unsigned long long sizeRead = 0;
	while (sizeRead < atomSize-7)
	{
		PmtFaceFloatPtr pt = PmtFaceReadFloat(theReadBuffer,atomSize,theFile,floatingPointRepresentation);
		floats.push_back(pt);
		sizeRead += 8;
	}

	return atomSize;
}
unsigned long long PmtFaceFloatVecAtom::writeAtom(ostream & o)
{
	unsigned int workingAtomSize = floats.size()*8 + 8;
 	std::vector<PmtAtomPtr> floatRepAtoms = this->parent->getAtoms("ffob.fpdt");
	unsigned short floatingPointRepresentation = 1; 
	if (floatRepAtoms.size() > 0) 
		floatingPointRepresentation = ((PmtFaceFpdtAtom*)(floatRepAtoms[0].ptr()))->floatingPointRepresentation;

	char * a = (char*)&workingAtomSize;
	o << a[3] << a[2] << a[1] << a[0];
    o << atomType[0] << atomType[1] << atomType[2] << atomType[3];
	std::vector<PmtFaceFloatPtr>::iterator iter = floats.begin();
	while (iter != floats.end())
	{
		(*iter)->write(o,floatingPointRepresentation);
		iter++;
	}
    return workingAtomSize;
}

PmtAtom * PmtFaceFloatVecAtom::clone()
{
    PmtFaceFloatVecAtom * a = new PmtFaceFloatVecAtom(this->atomType);
	a->floats = this->floats;
    return a;
}

SPmtString PmtFaceFloatVecAtom::getMetadata(SPmtString key, SPmtString tag)
{
	SPmtString result;
	if (key == atomType)
	{
        std::ostringstream os;
        PmtPrintValue(os,floats);
        result = os.str().c_str();
	}
	return result;
}

void PmtFaceFloatVecAtom::setFromString(SPmtString val)
{
    floats.clear();
    std::istringstream is(val);
    std::vector<double> d;
    PmtReadValue<double>(is,d);
    std::vector<double>::iterator i = d.begin();
    while (i != d.end())
    {
        floats.push_back(new PmtFaceFloat((*i)));
        i++;
    }
}

//================= PmtFacePtAtom ========================================

unsigned long long PmtFacePtAtom::readAtom() 
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
    
    if ((numread >= atomSize) && (atomSize>15))
    {
	    std::vector<PmtAtomPtr> floatRepAtoms = this->parent->getAtoms("ffob.fpdt");
	    unsigned short floatingPointRepresentation = 1; 
	    if (floatRepAtoms.size() > 0) 
		    floatingPointRepresentation = ((PmtFaceFpdtAtom*)(floatRepAtoms[0].ptr()))->floatingPointRepresentation;

		PmtFaceFloatPtr x = PmtFaceReadFloat(c,atomSize,theFile,floatingPointRepresentation);
		PmtFaceFloatPtr y = PmtFaceReadFloat(&c[8],atomSize,theFile,floatingPointRepresentation);
		pt.first = x;
		pt.second = y;
	}

	return atomSize;
}
unsigned long long PmtFacePtAtom::writeAtom(ostream & o)
{
 	std::vector<PmtAtomPtr> floatRepAtoms = this->parent->getAtoms("ffob.fpdt");
	unsigned short floatingPointRepresentation = 1; 
	if (floatRepAtoms.size() > 0) 
		floatingPointRepresentation = ((PmtFaceFpdtAtom*)(floatRepAtoms[0].ptr()))->floatingPointRepresentation;

	o << (char)0 << (char)0 << (char)0 << (char)24;
    o << atomType[0] << atomType[1] << atomType[2] << atomType[3];
	pt.first->write(o,floatingPointRepresentation);
	pt.second->write(o,floatingPointRepresentation);
    return 24;
}

PmtAtom * PmtFacePtAtom::clone()
{
    PmtFacePtAtom * a = new PmtFacePtAtom(this->atomType);
	a->pt = this->pt;
    return a;
}

SPmtString PmtFacePtAtom::getMetadata(SPmtString key, SPmtString tag)
{
	SPmtString result;
	if (key == atomType)
	{
		char buf[32];
		sprintf(buf,"%f",(double)(*(pt.first.ptr())));
		SPmtString r(buf);
		r += " ";
		sprintf(buf,"%f",(double)(*(pt.second.ptr())));
		r += buf;
        result = r;
	}
	return result;
}

SPmtString::size_type PmtFacePtAtom::readPoint(SPmtString val, unsigned short fpr)
{
    SPmtString::size_type s = SPmtString::npos;
    if (val.length()>2)
    {
	    unsigned short floatingPointRepresentation = 2;
        if (fpr == 0)
        { 
            if (parent != NULL)
            {
	            std::vector<PmtAtomPtr> floatRepAtoms = this->parent->getAtoms("ffob.fpdt");
	            if (floatRepAtoms.size() > 0) 
		            floatingPointRepresentation = ((PmtFaceFpdtAtom*)(floatRepAtoms[0].ptr()))->floatingPointRepresentation;
            }
        }
        else
            floatingPointRepresentation = fpr;
        
		const char * c = val.c_str();
		SPmtString::size_type ts = val.length();
		while ((ts > 0 ) && ((*c == ' ')||(*c == '\t')))
		{ ts--; c++; }
        if (floatingPointRepresentation == 2)
        {
            pt.first = new PmtFaceFloat(atof(c));
			while ((ts > 0 ) && ((*c != ' ')&&(*c != '\t')))
			{ ts--; c++; }
			while ((ts > 0 ) && ((*c == ' ')||(*c == '\t')))
			{ ts--; c++; }
            if (ts > 0)
                pt.second = new PmtFaceFloat(atof(c));
			while ((ts > 0 ) && ((*c != ' ')&&(*c != '\t')))
			{ ts--; c++; }
        }
        else
        {
			const char * c2 = c;
			while ((ts > 0 ) && ((*c2 != ' ')&&(*c2 != '\t')))
			{ ts--; c2++; }
			while ((ts > 0 ) && ((*c2 == ' ')||(*c2 == '\t')))
			{ ts--; c2++; }
            if (ts > 0)
                pt.first = new PmtFaceFloat(atoi(c),atoi(c2));
			c = c2;
			while ((ts > 0 ) && ((*c != ' ')&&(*c != '\t')))
			{ ts--; c++; }
			while ((ts > 0 ) && ((*c == ' ')||(*c == '\t')))
			{ ts--; c++; }
			c2 = c;
			while ((ts > 0 ) && ((*c2 != ' ')&&(*c2 != '\t')))
			{ ts--; c2++; }
			while ((ts > 0 ) && ((*c2 == ' ')||(*c2 == '\t')))
			{ ts--; c2++; }
            if (ts > 0)
                pt.second = new PmtFaceFloat(atoi(c),atoi(c2));
        }
		s = val.length() - ts;
    }
    return s;
}

//================= PmtFacePtVecAtom ========================================

unsigned long long PmtFacePtVecAtom::readAtom() 
{
	std::vector<PmtAtomPtr> floatRepAtoms = this->parent->getAtoms("ffob.fpdt");
	unsigned short floatingPointRepresentation = 1; 
	if (floatRepAtoms.size() > 0) 
		floatingPointRepresentation = ((PmtFaceFpdtAtom*)(floatRepAtoms[0].ptr()))->floatingPointRepresentation;

	unsigned long long sizeRead = 8;

	// note: atomSize == size of atom's data not including 8 byte header
	while (sizeRead < atomSize-7)
	{
		unsigned char * buf = theReadBuffer+sizeRead;
		PmtFaceFloatPtr x = PmtFaceReadFloat(buf,atomSize,theFile,floatingPointRepresentation);
		PmtFaceFloatPtr y = PmtFaceReadFloat(&buf[8],atomSize,theFile,floatingPointRepresentation);
		points.push_back(PmtFacePoint(x,y));
		sizeRead += 16;
	}

	return sizeRead-8;
}
unsigned long long PmtFacePtVecAtom::writeAtom(ostream & o)
{
	unsigned int workingAtomSize = points.size()*16 + 8;
 	std::vector<PmtAtomPtr> floatRepAtoms = this->parent->getAtoms("ffob.fpdt");
	unsigned short floatingPointRepresentation = 1; 
	if (floatRepAtoms.size() > 0) 
		floatingPointRepresentation = ((PmtFaceFpdtAtom*)(floatRepAtoms[0].ptr()))->floatingPointRepresentation;

	char * a = (char*)&workingAtomSize;
	o << a[3] << a[2] << a[1] << a[0];
    o << atomType[0] << atomType[1] << atomType[2] << atomType[3];
	std::vector<PmtFacePoint>::iterator iter = points.begin();
	while (iter != points.end())
	{
		(*iter).first->write(o,floatingPointRepresentation);
		(*iter).second->write(o,floatingPointRepresentation);
		iter++;
	}
    return workingAtomSize;
}

PmtAtom * PmtFacePtVecAtom::clone()
{
    PmtFacePtVecAtom * a = new PmtFacePtVecAtom(this->atomType);
	a->points = this->points;
    return a;
}

SPmtString PmtFacePtVecAtom::getMetadata(SPmtString key, SPmtString tag)
{
	SPmtString result;
	if (key == atomType)
	{
		char buf[32];
		std::vector<PmtFacePoint>::iterator iter = points.begin();
		while(iter != points.end())
		{
			if (iter != points.begin()) result+=" ";
			sprintf(buf,"%f",(double)(*(*iter).first.ptr()));
			result += buf;
			result += " ";
			sprintf(buf,"%f",(double)(*(*iter).second.ptr()));
			result += buf;
			iter++;
		}
	}
	return result;
}

void PmtFacePtVecAtom::setFromString(SPmtString val)
{
	unsigned short floatingPointRepresentation = 2;
    if (parent  != NULL)
    {
	    std::vector<PmtAtomPtr> floatRepAtoms = this->parent->getAtoms("ffob.fpdt");
	    if (floatRepAtoms.size() > 0) 
		    floatingPointRepresentation = ((PmtFaceFpdtAtom*)(floatRepAtoms[0].ptr()))->floatingPointRepresentation;
    }
    
    SPmtString a = val;
    SPmtString::size_type s = 0;
	points.clear();
    while (s != SPmtString::npos)
    {
        a = a.substr(s);
        s = readPoint(a,floatingPointRepresentation);
		if (s != SPmtString::npos)
			points.push_back(pt);
    }
}

