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


#ifndef PMT_FACE_ATOMS__H
#define PMT_FACE_ATOMS__H

#include "PmtAtom.h"

void PmtInitializeFaceAtoms();

class PmtFaceFloat:public EkRefCount < EK_DEFAULT_MUTEX >
{
    public:
    
        PmtFaceFloat()
         : useDouble(false),numerator(0),denominator(1),myDouble(0.0)
        {}
        
        PmtFaceFloat(double x)
         : useDouble(true),numerator(0),denominator(1),myDouble(x)
        {}

        PmtFaceFloat(int n, int d)
         : useDouble(false),myDouble(0.0)
        {
            numerator = n;
            if (d != 0) denominator = d;
            else
            {
                // if denominator is 0, force return of 0!
                numerator = 0; 
                denominator = 1;
            }
        }

        inline operator double () const
        {
            double returnValue = myDouble;
            if (!useDouble)
                returnValue = (double)numerator/(double)denominator;
            return returnValue;
        }
		inline void write(ostream &o, unsigned short represent)
		{
			char * a;
			if (represent == 1)
			{
				if (this->useDouble)
				{
					denominator = 100000000;
					if (myDouble > 1.0)
					{
						double t = myDouble;
						while (t > 1.0)
						{
							t /= 10.0; 
							denominator /= 10;
						}
					}
					numerator = (int)(myDouble*(double)denominator);
				}
				a = (char*)&numerator;
				o << a[3] << a[2] << a[1] << a[0];
				a = (char*)&denominator;
				o << a[3] << a[2] << a[1] << a[0];
			}
			else
			{
				double t = myDouble;
				if (!useDouble)
					t = (double)numerator/(double)denominator;
				a = (char*)&t;
				o << a[7] << a[6] << a[5] << a[4] << a[3] << a[2] << a[1] << a[0];
			}
		}

		inline double operator = (double rhs)
		{
			myDouble = rhs;
			useDouble = true;
			return myDouble;
		}

    private:
        bool useDouble;
        int numerator;
        int denominator;
        double myDouble;
};

typedef EkSmartPtr < PmtFaceFloat > PmtFaceFloatPtr;
typedef std::pair < PmtFaceFloatPtr, PmtFaceFloatPtr > PmtFacePoint;

class PMTACCR_IMPL_DECL PmtFaceFpdtAtom : public PmtAtom
{
    public:
    
        // default to numerator/denominator representation
        PmtFaceFpdtAtom(SPmtString name)
         : PmtAtom (name), floatingPointRepresentation(1)
        {}
        virtual ~PmtFaceFpdtAtom() {};
        virtual unsigned long long readAtom();
        virtual unsigned long long writeAtom(ostream & o);
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();
        virtual void setFromString(SPmtString val);

		unsigned short floatingPointRepresentation;
};

class PMTACCR_IMPL_DECL PmtFaceFloatVecAtom : public PmtAtom
{
    public:
    
        // default to numerator/denominator representation
        PmtFaceFloatVecAtom(SPmtString name) : PmtAtom (name)
        {}
        virtual ~PmtFaceFloatVecAtom() {};
        virtual unsigned long long readAtom();
        virtual unsigned long long writeAtom(ostream & o);
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();
        virtual void setFromString(SPmtString val);

    private:
		std::vector<PmtFaceFloatPtr> floats;
};

// This covers leye and reye atoms, as well as other point atoms.
class PMTACCR_IMPL_DECL PmtFacePtAtom : public PmtAtom
{
    public:
    
        // default to numerator/denominator representation
        PmtFacePtAtom(SPmtString name) : PmtAtom (name)
        {}
        virtual ~PmtFacePtAtom() {};
        virtual unsigned long long readAtom();
        virtual unsigned long long writeAtom(ostream & o);
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();
        virtual inline void setFromString(SPmtString val)
        { readPoint(val); }

    protected:
		PmtFacePoint pt;
        SPmtString::size_type readPoint(SPmtString val, unsigned short fpr = 0);
};

// This covers face outline atoms, as well as other line type atoms
class PMTACCR_IMPL_DECL PmtFacePtVecAtom : public PmtFacePtAtom
{
    public:
    
        // default to numerator/denominator representation
        PmtFacePtVecAtom(SPmtString name) : PmtFacePtAtom (name)
        {}
        virtual ~PmtFacePtVecAtom() {};
        virtual unsigned long long readAtom();
        virtual unsigned long long writeAtom(ostream & o);
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();
        virtual void setFromString(SPmtString val);

		virtual inline void setPoints(std::vector<PmtFacePoint> p)
		{ points = p; }

    private:
        std::vector<PmtFacePoint> points;
};

//This covers the user confirmation of face/name
class PMTACCR_IMPL_DECL PmtFaceUCONAtom : public PmtSimpleTypeAtom<unsigned char>
{
    public:
        PmtFaceUCONAtom(SPmtString name) :  PmtSimpleTypeAtom<unsigned char>(name)
        {}

        ~PmtFaceUCONAtom(){};

        inline virtual SPmtString getMetadata(SPmtString key, SPmtString tag)
        {
	        SPmtString result;
	        if (key == atomType)
            {
                if (value==0) result = "Unknown";
                else if (value == 1) result = "Implicit";
                else if (value == 2) result = "Explicit";
            }
	        return result;
        }
        
        inline virtual PmtAtom * clone()
        {return new PmtFaceUCONAtom(atomType); };

        inline virtual void setFromString(SPmtString val)
        {
            if (val == "Unknown") value=0;
            else if (val == "Implicit") value = 1;
            else if (val == "Explicit") value = 2;
        }
};

#endif

