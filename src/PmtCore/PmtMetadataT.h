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
 * Creation date: January 1999
 *
 * Original Author: 
 * George Sotak   george.sotak@kodak.com
 * 
 * Contributor(s): 
 */

#ifndef PMT_METADATA_T_H
#define PMT_METADATA_T_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#if (defined _MSC_VER) && (!defined _WINDOWS_) && (!defined __AFXWIN_H__)
#include "windows.h"
#endif

#include <sstream>
#include "PmtIoFunctions.h"
#include "PmtMdVisitorImpl.h"
#include "PmtMetadata.h"
#include "PmtFacet.h"

#define PMT_CAST_TYPE(MD, TYPE) dynamic_cast<PmtMetadataT<TYPE > *>((MD).ptr())

// Forward references
class PmtCompositeMetadata;
template < class T > class PmtMetadataT;

#ifdef PMTCORE_EXPLICIT
#endif


//! The PmtMetadataT<TYPE> class is typically referred to as the leaf metadata and has a value.

/*! PmtMetadataT<TYPE> is a templated class that is derived from
PmtMetadata. It contains the metadata value (of the data type TYPE)
and contains facets associated with the metadata value. Instances of
the PmtMetadataT<TYPE> class maps to <em>\<element\></em> declarations
with <em>\<simpleType\></em>s. The facets restrict the range of the
domain of the TYPE. Facets are specified in the metadata definition
language as <em>\<restriction\></em> or <em>\<extention\></em> on the
<em>\<simpleType\></em> construct.
*/

template < class TYPE > class PMTCORE_DECL PmtMetadataT:public PmtMetadata
{
  public:
    //! This is the default constructor
  inline PmtMetadataT():PmtMetadata()
    {;
    }

    //! This is another constructor
    inline PmtMetadataT(const PmtKeyType & mdKey, const SPmtString & type,
                        const SPmtString & trueType):PmtMetadata(mdKey, type,
                                                                 trueType)
    {
    }

    //! This is another constructor
    inline PmtMetadataT(const PmtKeyType & mdKey, const SPmtString & type,
                        const SPmtString & trueType,
                        const SPmtString & nillable):PmtMetadata(mdKey, type,
                                                                 trueType,
                                                                 nillable)
    {
    }

    //! This is another constructor
    inline PmtMetadataT(const PmtKeyType & mdKey, const SPmtString & type,
                        const SPmtString & trueType, const TYPE & val,
                        const SPmtString & nillable):PmtMetadata(mdKey, type,
                                                                 trueType,
                                                                 nillable),
        mValue(val)
    {
    }

    //! This is another constructor
    inline PmtMetadataT(const PmtKeyType & mdKey, const SPmtString & type,
                        PmtMDType mdType):PmtMetadata(mdKey, type, mdType)
    {
    }

    //! This is another constructor
    inline PmtMetadataT(const PmtKeyType & mdKey, const SPmtString & type,
                        PmtMDType mdType,
                        const SPmtString & nillable):PmtMetadata(mdKey, type,
                                                                 mdType,
                                                                 nillable)
    {
    }

    //! This is another constructor
    inline PmtMetadataT(const PmtKeyType & mdKey, const SPmtString & type,
                        PmtMDType mdType, const TYPE & val,
                        const SPmtString & nillable):PmtMetadata(mdKey, type,
                                                                 mdType,
                                                                 nillable),
        mValue(val)
    {
    }

    //! This is yet another constructor
    inline PmtMetadataT(const PmtMetadataT < TYPE > &theSrc,
                        PmtCompositeMetadata * theParent):PmtMetadata(theSrc,
                                                                      theParent),
        mValue(theSrc.mValue), mFacets(theSrc.mFacets)
    {
    }

    //! Copy constructor
    inline PmtMetadataT(const PmtMetadataT < TYPE >
                        &theSrc):PmtMetadata(theSrc), mValue(theSrc.mValue),
        mFacets(theSrc.mFacets)
    {
    }
    //! This is the destructor
    inline virtual ~ PmtMetadataT()
    {
    };

    //! Returns a const reference to the metadata's value
    inline const TYPE & value() const
    {
        return mValue;
    }

    //! Returns a reference to the metadata's value
    inline TYPE & value()
    {
        mIsChanged = true;
        return mValue;
    }

    //! Returns the metadata's value in ASCII encoding
    inline virtual EkString getValueStr() const;

    //! Sets the metadata's value from an ASCII encoding
    inline virtual void setValueStr(const EkString & valStr);

    //! Return a "deep" copy of the metadata instance
    inline virtual PmtMetadata *clone(void) const
    {
        return new PmtMetadataT < TYPE > (*this);
    }

    //! Return a "deep" copy of the metadata instance
    inline virtual PmtMetadataPtr clone(PmtCompositeMetadata * theParent) const
    {
        return new PmtMetadataT < TYPE > (*this, theParent);
    }

    //! Support for Visitor Pattern
    inline virtual void accept(PmtMetadataVisitor * visitor);
    //! Support for Visitor Pattern
    inline virtual void accept(PmtMdVisitorImpl * visitor);

    //! Returns true if metadata's value is valid.
    inline virtual bool validate() const;

    //! Show metadata object.
    inline virtual void show(bool showObjectMemoryPointers = false, int level =
                             0, bool showModelGroups = false);

  protected:
    //! Write info unique to class
    inline virtual ostream & printDerived(ostream & c) const
    {
        PmtPrintValue(c, mValue);
        return c;
    }

    inline virtual void addFacet(const PmtKeyType & facetName,
                                 const vector < SPmtString > &value,
                                 bool fixed);

    TYPE mValue;                //mUse, mFixed;

  private:
    vector < PmtFacetPtr > mFacets;

    //! Disable the assignment operator
    PmtMetadataT < TYPE > &operator=(PmtMetadataT < TYPE > &src);
};


template < class TYPE > inline EkString PmtMetadataT < TYPE >::getValueStr() const
{
    ostringstream c;
    PmtPrintValue(c, mValue);   // performs c << value
#ifdef _WINVER
	c << endl;
#endif
#if defined(_UNICODE)
    return EkAToW(c.str());
#else
    return c.str();
#endif
}

template < class TYPE >
    inline void PmtMetadataT < TYPE >::setValueStr(const EkString & valStr)
{
    SPmtString tmp;
#if defined(_UNICODE)
    tmp = EkWToA(valStr);
#else
    tmp = valStr;
#endif
    istringstream c(tmp);
    PmtReadValue(c, mValue);    // performs c >> value
    mIsChanged = true;
    return;
}


template < class TYPE >
    inline void PmtMetadataT < TYPE >::accept(PmtMetadataVisitor * visitor)
{
    PmtMdVisitorImpl *visImpl;
    if ((visImpl = dynamic_cast < PmtMdVisitorImpl * >(visitor)))
        visImpl->visit(*this);
    return;
}

template < class TYPE >
    inline void PmtMetadataT < TYPE >::accept(PmtMdVisitorImpl * visitor)
{
    visitor->visit(*this);
    return;
}


template < class TYPE > inline bool PmtMetadataT < TYPE >::validate() const
{
    for (unsigned int i = 0; i < mFacets.size(); i++)
    {
        if (!mFacets[i]->validate(&mValue))
            return false;
    }
    return true;
}


template < class TYPE >
    inline void PmtMetadataT < TYPE >::addFacet(const PmtKeyType & facetName,
                                                const vector < SPmtString >
                                                &value, bool fixed)
{
    PmtFacetPtr facet = 0;
    for (unsigned int i = 0; i < mFacets.size(); i++)
    {
        if (mFacets[i]->getFacetName() == facetName)
        {
            facet = mFacets[i];
            break;
        }
    }

    if (facet)
    {
        facet->setFacetValue(value, fixed);
    }
    else
    {
        SPmtString fKey = facetName;
        fKey += "-";
        fKey += trueType();
        facet = PmtFacet::getFactory().create(fKey);
        if (facet)
        {
            facet->init(value, fixed);
            mFacets.push_back(facet);
        }
    }
}


template < class TYPE >
    inline void PmtMetadataT < TYPE >::show(bool showObjectMemoryPointers,
                                            int level, bool showModelGroups)
{
    SPmtString mdKey = fullKey();

    int i;
    for (i = 0; i < level; i++)
        cout << "  ";
    cout << "-PmtMetadataT  " << mdKey << " ";
    print(cout);
    if (showObjectMemoryPointers)
        cout << "  " << this;
    cout << endl;

    showAttr(level + 1);

    for (unsigned int j = 0; j < mFacets.size(); j++)
        mFacets[j]->show(level + 1);
}


#if defined(_MSC_VER) && defined(INTERPRETER_CPP)
extern template class PmtMetadataT < vector < char > >;
extern template class PmtMetadataT < char >;
extern template class PmtMetadataT < vector < unsigned char > >;
extern template class PmtMetadataT < unsigned char >;
extern template class PmtMetadataT < vector < short > >;
extern template class PmtMetadataT < short >;
extern template class PmtMetadataT < vector < unsigned short > >;
extern template class PmtMetadataT < unsigned short >;
extern template class PmtMetadataT < vector < long > >;
extern template class PmtMetadataT < long >;
extern template class PmtMetadataT < vector < unsigned long > >;
extern template class PmtMetadataT < unsigned long >;
extern template class PmtMetadataT < vector < bool > >;
extern template class PmtMetadataT < bool >;
extern template class PmtMetadataT < vector < float > >;
extern template class PmtMetadataT < float >;
extern template class PmtMetadataT < vector < double > >;
extern template class PmtMetadataT < double >;
extern template class PmtMetadataT < vector < string > >;
extern template class PmtMetadataT < string >;
#endif


#endif                          // PMT_METADATA_T_H

