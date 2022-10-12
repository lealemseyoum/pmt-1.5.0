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
 * Creation date: 7/18/2000
 *
 * Original Author: 
 * Chris Lin <ti.lin@kodak.com> 
 * 
 * Contributor(s): 
 * Sam Fryer <samuel.fryer@kodak.com>
 *
 */

#ifndef _PMT_TRANSLATOR_H
#define _PMT_TRANSLATOR_H

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif

#if (defined _MSC_VER) && (!defined _WINDOWS_) && (!defined __AFXWIN_H__)
#include "windows.h"
#endif

#include "EkCompiler.h"
#include "EkSmartPtr.h"
#include "EkRefCount.h"
#include "PmtAccrImplDefs.h"
#include "PmtCommon.h"
#include "PmtMetadataT.h"



// The symbols below are used as keys to the translator factory.
// Each of them are defined to a hash function value.
// The function is (char[8]<<1)+char[9], where char = "builtin2bool",
// "builtin2long", etc.
#define PMT_TRANSLATOR_BUILTIN2BOOL 307
#define PMT_TRANSLATOR_BUILTIN2CHAR 302
#define PMT_TRANSLATOR_BUILTIN2UCHAR 333
#define PMT_TRANSLATOR_BUILTIN2SHORT 334
#define PMT_TRANSLATOR_BUILTIN2USHORT 349
#define PMT_TRANSLATOR_BUILTIN2LONG 327
#define PMT_TRANSLATOR_BUILTIN2ULONG 342
#define PMT_TRANSLATOR_BUILTIN2FLOAT 312
#define PMT_TRANSLATOR_BUILTIN2DOUBLE 311
#define PMT_TRANSLATOR_BUILTIN2STRING 346
#define PMT_TRANSLATOR_BUILTIN2ASCII 309
#define PMT_TRANSLATOR_BUILTIN2UNDEFINED 344
#define PMT_TRANSLATOR_BUILTIN2RATIONAL 325
#define PMT_TRANSLATOR_BUILTIN2URATIONAL 348
#define PMT_TRANSLATOR_BUILTIN2FACE 301

class PmtTranslator;

// Smart pointers
typedef EkSmartPtr < PmtTranslator > PmtTranslatorPtr;
typedef EkConstSmartPtr < PmtTranslator > PmtConstTranslatorPtr;

//! Translates between file format metadata type and the common metadata types
/*! A translator is responsible for assigning the value between 
  PmtMetadata and other data type, which includes primitive data types like 
  char, int, float and non-primitive data types such as struct, union. 
  PmtTranslator is the base class that defines the interface. Note that all 
  translator classes are internal and shouldn't be used directly by the users.
*/
class PmtTranslator:public EkRefCount < EK_DEFAULT_MUTEX >
{
  public:
    PmtTranslator()
    {
    };

    virtual ~ PmtTranslator()
    {
    };

    static PmtTranslatorPtr create(const SPmtString & type);

    inline virtual PmtTranslatorPtr clone()
    {
        return this;
    }

    virtual void assign(PmtMetadata * md, void *&value, unsigned int &vCount,
                        bool valueToMd) = 0;

    bool isVector(const PmtMDType & id);


  protected:

    // This method converts the translator type from the string to the
    // corresponding factory key
    static unsigned int stringToFactoryKey(const SPmtString & type);

    template < class MdType, class ValType >
    inline void upAssignToMd(bool bVector, MdType dummy,
                             PmtMetadata * md,
                             unsigned int &valueCount,
                             ValType * &ffValue)
    {
        if (!bVector)
        {
            PmtMetadataT < MdType > *mdt = ( PmtMetadataT < MdType > *)(md);
            mdt->value() = static_cast < MdType > (*ffValue);
            delete[] ffValue;
            ffValue = NULL;
        }
        else
        {
            PmtMetadataT < vector < MdType > >*mdt =
                ( PmtMetadataT < vector < MdType > >*)(md);
            vector < MdType > tmpVec;
            ValType *tmpVal = ffValue;
            for (unsigned int i = 0; i < valueCount; i++, tmpVal++)
                tmpVec.push_back(static_cast < MdType > (*tmpVal));
            mdt->value() = tmpVec;
            delete[] ffValue;
            ffValue = NULL;
        }
    }

    template < class MdType, class ValType >
    inline void upAssignToVal(bool bVector, MdType dummy,
                              PmtMetadata * md,
                              unsigned int &valueCount,
                              ValType * &ffValue)
    {
        if (!bVector)
        {
            PmtMetadataT < MdType > *mdt =
                ( PmtMetadataT < MdType > *)(md);
            // NEED TO DO A RANGE CHECK 
            valueCount = 1;
            ffValue = new ValType[valueCount];
            *ffValue = static_cast < ValType > (mdt->value());
        }
        else
        {
            PmtMetadataT < vector < MdType > >*mdt =
                ( PmtMetadataT < vector < MdType > >*)(md);
            const vector < MdType > &tmpVec = mdt->value();
            valueCount = tmpVec.size();
            ffValue = new ValType[valueCount];
            ValType *valPtr = ffValue;
            EK_TYPENAME vector < MdType >::const_iterator crnt(tmpVec.begin());
            while (crnt != tmpVec.end())
            {
                *valPtr = static_cast < ValType > (*crnt);
                crnt++;
                valPtr++;
            }
//             copy(tmpVec.begin(), tmpVec.end(), ffValue);
        }
    }

    template < class MdType, class ValType >
    inline void downAssignToVal(bool bVector, MdType dummy,
                                PmtMetadata * md,
                                unsigned int &valueCount,
                                ValType * &ffValue, ValType min,
                                ValType max)
    {
        if (!bVector)
        {
            PmtMetadataT < MdType > *mdt =
                ( PmtMetadataT < MdType > *)(md);
            MdType mdValue(mdt->value());

            valueCount = 1;
            ffValue = new ValType[valueCount];
            if (mdValue < min)
                *ffValue = min;
            else if (mdValue > max)
                *ffValue = max;
            else
                *ffValue = static_cast < ValType > (mdValue);
        }
        else
        {
            PmtMetadataT < vector < MdType > >*mdt =
                ( PmtMetadataT < vector < MdType > >*)(md);
            const vector < MdType > &tmpVec = mdt->value();

            valueCount = tmpVec.size();
            ffValue = new ValType[valueCount];
            ValType(*valPtr) (ffValue);

            EK_TYPENAME vector < MdType >::const_iterator crnt(tmpVec.begin());
            while (crnt != tmpVec.end())
            {
                if (*crnt < min)
                    *valPtr = min;
                else if (*crnt > max)
                    *valPtr = max;
                else
                    *valPtr = static_cast < ValType > (*crnt);

                crnt++;
                valPtr++;
            }
        }
    }

    template < class MdType, class ValType >
    inline void downAssignToMd(bool bVector, PmtMetadata * md,
                               MdType min, MdType max,
                               unsigned int &valueCount,
                               ValType * &ffValue)
    {
        if (!bVector)
        {
            PmtMetadataT < MdType > *mdt =
                ( PmtMetadataT < MdType > *)(md);
            if (*ffValue < min)
                mdt->value() = min;
            else if (*ffValue > max)
                mdt->value() = max;
            else
                mdt->value() = static_cast < MdType > (*ffValue);

            delete[]ffValue;
            ffValue = NULL;
        }
        else
        {
            PmtMetadataT < vector < MdType > >*mdt =
                ( PmtMetadataT < vector < MdType > >*)(md);
            vector < MdType > tmpVec;
            ValType *valPtr = ffValue;
            for (unsigned int i = 0; i < valueCount; i++, valPtr++)
            {
                if (*valPtr < min)
                    tmpVec.push_back(min);
                else if (*valPtr > max)
                    tmpVec.push_back(max);
                else
                    tmpVec.push_back(static_cast < MdType > (*valPtr));
            }
            mdt->value() = tmpVec;
            delete[]ffValue;
            ffValue = NULL;
        }
    }

  private:
    //Copy constructor & assignment operator
    PmtTranslator(const PmtTranslator & src);
    PmtTranslator & operator=(PmtTranslator & src);

};


//--------------------------------------------------------------------
// Templated Translation Class
//--------------------------------------------------------------------
template < class TYPE >
class PmtTranslatorT : public PmtTranslator
{
    public:

        //! Constructor
        inline PmtTranslatorT()
        {
        }

        //! Deconstructor
        inline virtual ~ PmtTranslatorT()
        {
        }

        //! Method to actually assign the value to the MD item, or vice versa.
        void assign(PmtMetadata * md, void *&value, unsigned int &vCount,
                    bool valToMd);

};

//--------------------------------------------------------------------
// External Template Instantiations
//--------------------------------------------------------------------
#ifndef PMT_TRANSLATOR_CPP
#ifdef _MSC_VER
// VC6 complains (somewhat erroneously) about extern template instantiations.  
// Disable those complaints here...
#pragma warning( push )
#pragma warning( disable : 4231)
#endif
extern template class PmtTranslatorT < bool > ;
extern template class PmtTranslatorT < char > ;
extern template class PmtTranslatorT < unsigned char > ;
extern template class PmtTranslatorT < short > ;
extern template class PmtTranslatorT < unsigned short > ;
extern template class PmtTranslatorT < int > ;
extern template class PmtTranslatorT < unsigned int > ;
extern template class PmtTranslatorT < float > ;
extern template class PmtTranslatorT < double > ;
extern template class PmtTranslatorT < SPmtString > ;
extern template class PmtTranslatorT < char * > ;
extern template class PmtTranslatorT < void * > ;
#ifdef _MSC_VER
// Reset the warnings as they were
#pragma warning( pop )
#endif
#endif

#endif

