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

#ifndef PMT_ATOM__H
#define PMT_ATOM__H

#include "PmtCommon.h"

#include "EkCompiler.h"
#include "EkSmartPtr.h"
#include "EkRefCount.h"
#include "EkFactoryT.h"

#include <stdlib.h>
#include <stdio.h>
#include <sstream>

#include "PmtAccrImplDefs.h"

class PmtAtom;

// Smart pointers
typedef EkSmartPtr < PmtAtom > PmtAtomPtr;
typedef EkConstSmartPtr < PmtAtom > PmtConstAtomPtr;
typedef EkFactoryT < SPmtString , PmtAtom * > PmtAtomFactoryT;

/*! The PmtAtomParser is the base class for atom parsing implementations.
*/
class PMTACCR_IMPL_DECL PmtAtom:public EkRefCount < EK_DEFAULT_MUTEX >
{
  public:
    static PmtAtom * GetNextAtom(FILE *f, unsigned long long readsize, PmtAtom * parent = NULL);
    static PmtAtom * GetNextAtom(unsigned char * c, unsigned long long unreadsize, 
                                 PmtAtom * parent = NULL);
    static PmtAtomFactoryT * getFactory();
 
    //! Destructor
    virtual ~PmtAtom(){}

    /*! 
        Read the atom.
     */
    virtual unsigned long long readAtom() = 0;
    
    /*! 
        Write the atom.
     */
    virtual unsigned long long writeAtom(ostream & o) 
    {return 0;}
    
    /*! 
        Return metadata values.
     */
    virtual SPmtString getMetadata(SPmtString key, SPmtString tag) = 0;

    /*! 
        Return the raw atom contents.
     */
	virtual char * getRawMetadata(SPmtString key)
	{ return NULL; }
    
    /*! 
        Clone the atom.
     */
    virtual PmtAtom * clone() = 0;
    
    /*! 
        Get the size of the atom.
     */
    unsigned long long getAtomSize()
    { return atomSize; }
    
    /*! 
        Get the type (name) of the atom.
     */
    SPmtString getAtomType()
    { return atomType; }
    
    /*! 
        Get the offset to the atom.
     */
    unsigned long long getOffset()
    { return offset; }

	/*! 
        Get atoms whose type matches the key.
     */
	virtual std::vector < PmtAtomPtr > getAtoms(SPmtString key);

    /*! 
        Set the parent to the atom.
     */
    void setParent(PmtAtom * p)
    { parent = p; }

    /*! 
        Set from a string representation
     */
    virtual void setFromString(SPmtString val)
    { }

  protected:
  
    SPmtString atomType;
    unsigned long long atomSize;
    FILE * theFile;
	unsigned long long offset;  // offset from beginning to start of atom
    unsigned char * theReadBuffer;
    unsigned long long theReadBufferSize;
    
    PmtAtom * parent;
        
    //! Constructor
    PmtAtom(SPmtString name) : 
        atomType(name),atomSize(0),theFile(NULL),offset(0),
        theReadBuffer(NULL),theReadBufferSize(0),parent(NULL)
    {}
    
    inline void init(unsigned long long length,FILE * file, PmtAtom * theParent)
    { atomSize=length;theFile=file; parent=theParent;}
    
    inline void init(unsigned long long length,unsigned char * c, 
                        unsigned long long s, PmtAtom * theParent)
    { atomSize=length;theReadBuffer=c; theReadBufferSize=s; parent=theParent;}
    
    PmtAtom * getTopParentAtom();
    
  private:
};


//================= PmtNullAtom ========================================


class PMTACCR_IMPL_DECL PmtNullAtom : public PmtAtom
{
    public:
        PmtNullAtom(SPmtString name) : PmtAtom (name)
        {}
        virtual ~PmtNullAtom() {};
        
        virtual unsigned long long readAtom();
        
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag)
        { SPmtString S; return S; }
    
        virtual PmtAtom * clone();
    private:
};

//================= PmtContainerAtom ========================================


class PMTACCR_IMPL_DECL PmtContainerAtom : public PmtAtom
{
    public:
        PmtContainerAtom(SPmtString name) : PmtAtom (name)
        {}
        virtual ~PmtContainerAtom() {};
        
        virtual unsigned long long readAtom();
        virtual unsigned long long writeAtom(ostream & o);
        
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();

		virtual std::vector < PmtAtomPtr > getAtoms(SPmtString key);

		virtual bool addAtom(PmtAtom * newAtom);

    protected:
        std::vector < PmtAtomPtr > atoms;
};
typedef EkSmartPtr < PmtContainerAtom > PmtContainerAtomPtr;


//================= PmtStringAtom ========================================

class PMTACCR_IMPL_DECL PmtStringAtom : public PmtAtom
{
    public:
        PmtStringAtom(SPmtString name) : PmtAtom (name)
        {}
        virtual ~PmtStringAtom() {};
        
        virtual unsigned long long readAtom();
        virtual unsigned long long writeAtom(ostream & o);
        
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();

        virtual void setFromString(SPmtString val)
        { data = val; atomSize = data.length(); }
        
    private:
        SPmtString data;
};


//================= PmtUnicodeAtom =======================================

class PMTACCR_IMPL_DECL PmtUnicodeAtom : public PmtAtom
{
    public:
        PmtUnicodeAtom(SPmtString name) 
        : PmtAtom(name),lang(0),numChars(0),length(0)
        {}
        virtual ~PmtUnicodeAtom() {};
        
        virtual unsigned long long readAtom();
        virtual unsigned long long writeAtom(ostream & o);
        
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();

        virtual void setFromString(SPmtString val);
        
    private:
        std::vector<unsigned char> data;// the actual unicode string
        unsigned short lang;            // language
        unsigned int numChars;          // number of characters
        unsigned long long length;      // actual length of data
};


//================= PmtAsciiStringAtom ========================================

class PMTACCR_IMPL_DECL PmtAsciiStringAtom : public PmtAtom
{
    public:
        PmtAsciiStringAtom(SPmtString name) : PmtAtom (name)
        {}
        virtual ~PmtAsciiStringAtom() {};
        
        virtual unsigned long long readAtom();
        virtual unsigned long long writeAtom(ostream & o);
        
        virtual SPmtString getMetadata(SPmtString key, SPmtString tag);
    
        virtual PmtAtom * clone();

        virtual void setFromString(SPmtString val)
        { myData = val; atomSize = myData.length(); }
        
    private:
        SPmtString myData;
};

//================= PmtBinaryAtom ==================================

class PMTACCR_IMPL_DECL PmtBinaryAtom : public PmtAtom
{
    public:
        PmtBinaryAtom(SPmtString name) : PmtAtom (name),value(0)
        {}
        virtual ~PmtBinaryAtom() {};
        
        inline virtual unsigned long long readAtom()
        {
            value = malloc((size_t)atomSize);

	        if (theFile != NULL)
	        {
		        // First, determing where the atom is
		        offset = ftell(theFile);
		        fread(value,1,(size_t)atomSize,theFile);
	        }
            else
            {
    	        unsigned char * c = theReadBuffer+8;
                memcpy(value,c,(size_t)atomSize);
            }
            return atomSize;
        }
        
        inline virtual unsigned long long writeAtom(ostream & o)
        {
            unsigned long long theAtomSize = atomSize+8;
            const char * a = (char*)&theAtomSize;
            o << a[3] << a[2] << a[1] << a[0];
            o << atomType[0] << atomType[1] << atomType[2] << atomType[3];
            a = (const char *)value;
			unsigned long long i = 0;
            for (; i<atomSize;i++, a++) 
                o << *a;
            return theAtomSize;
        }

        
        inline virtual SPmtString getMetadata(SPmtString key, SPmtString tag)
        {
	        SPmtString result;
	        if (key == atomType)
            {
                std::ostringstream os;
                unsigned char * a = (unsigned char *)value;
                for (unsigned long long i = 0; i< atomSize; i++,a++)
                    os << (unsigned short) (*a) << " ";
		        result = os.str().c_str();
            }
	        return result;
        }
    
        inline virtual PmtAtom * clone()
        {return new PmtBinaryAtom(atomType); };

        virtual void setFromString(SPmtString val);
        
    protected:
        void * value;
};



//================= PmtSimpleTypeAtom ==================================

template <class TYPE>
class PMTACCR_IMPL_DECL PmtSimpleTypeAtom : public PmtAtom
{
    public:
        PmtSimpleTypeAtom(SPmtString name) : PmtAtom (name),value(0)
        {}
        virtual ~PmtSimpleTypeAtom() {};
        
        inline virtual unsigned long long readAtom()
        {
            unsigned long length = sizeof(TYPE);
	        unsigned char * c = theReadBuffer+8;
	        unsigned long long numread = theReadBufferSize;

	        if ((theFile != NULL) && (atomSize >= length))
	        {
		        // First, determing where the atom is
		        offset = ftell(theFile);

		        c = new unsigned char[(unsigned int)atomSize];
		        numread = fread(c,1,(size_t)atomSize,theFile);
	        }

            if ((numread >= atomSize)&&(length <= atomSize))
            {
                unsigned long i=0,p=length-1;
                unsigned char * a = (unsigned char *)&value;
                for (i=0;i<length;i++,p--)
                    a[i] = c[p];                        
            }
	        if ((theFile != NULL) && (atomSize >= length))
                delete c;
            return atomSize;
        }
        
        inline virtual unsigned long long writeAtom(ostream & o)
        {
            unsigned long length = sizeof(TYPE);
            unsigned long long atomSize = length+8;
            char * a = (char*)&atomSize;
            o << a[3] << a[2] << a[1] << a[0];
            o << atomType[0] << atomType[1] << atomType[2] << atomType[3];
            a = (char *)&value;
            for (int i = length-1; i>=0;i--) 
                o << a[i];
            return atomSize;
        }

        
        inline virtual SPmtString getMetadata(SPmtString key, SPmtString tag)
        {
	        SPmtString result;
	        if (key == atomType)
            {
                std::ostringstream os;
                os << value;
		        result = os.str().c_str();
            }
	        return result;
        }
    
        inline virtual PmtAtom * clone()
        {return new PmtSimpleTypeAtom<TYPE>(atomType); };

        inline virtual void setFromString(SPmtString val)
        {
	        if (val.length() > 0)
            {
                std::istringstream is(val);
                is >> value;
            }
        }
        
    protected:
        TYPE value;
};


//=======================================================================
// Streaming the atoms out:
inline ostream & operator << (ostream & lhs, PmtAtom * rhs)
{ if (rhs != NULL) rhs->writeAtom(lhs); return lhs; }

#endif                          // PMT_ATOM__H
