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
 * Creation date: May 2001
 *
 * Original Author: 
 * Chris Lin    ti.lin@kodak.com
 * Copyright (C) 2001 Eastman Kodak Company. All Rights Reserved. 
 * 
 * Contributor(s): 
 */

#ifndef PMT_ATTRIBUTE_T_H
#define PMT_ATTRIBUTE_T_H

#include <sstream>
#include "PmtIoFunctions.h"
#include "PmtFacet.h"

//! PmtAttributeT is a template metadata attribute class.
/*! PmtAttributeT is a template class derived from PmtAttribute. It contains the
 attribute value and facets associated with the value.
*/
template < class TYPE > class PMTCORE_DECL PmtAttributeT:public PmtAttribute
{
  public:
    //! Constructor
  inline PmtAttributeT():PmtAttribute()
    {
    }

    //! Constructor
    inline PmtAttributeT(const PmtKeyType & name, const PmtKeyType & type,
                         const PmtKeyType & trueType, const TYPE & value,
                         const TYPE & fixed,
                         const SPmtString & use):PmtAttribute(name, type,
                                                              trueType),
        mValue(value), mFixed(fixed), mUse(use)
    {
    }

    //! Copy Constructor
    inline PmtAttributeT(const PmtAttributeT < TYPE > &src):PmtAttribute(src),
        mValue(src.mValue), mFixed(src.mFixed), mUse(src.mUse),
        mFacets(src.mFacets)
    {
    }

    //! This method returns a const reference to the attribute value.
    inline const TYPE & value() const
    {
        return mValue;
    }

    //! This method returns a reference to the attribute value.
    inline TYPE & value()
    {
        mIsChanged = true;
        return mValue;
    }

    //! This method returns the value of the attribute
    inline virtual EkString getValueStr() const;

    //! This method sets the value of the attribute
    inline virtual void setValueStr(const EkString & valStr);

    //! This method returns the value fixed of the attribute
    inline virtual EkString getFixedStr() const;

    //! This method sets the value fixed of the attribute
    inline virtual void setFixedStr(const EkString & valStr);

    //! This method returns the value use of the attribute
    inline virtual EkString getUseStr() const;

    //! This method sets the value use of the attribute
    inline virtual void setUseStr(const EkString & valStr);

    //! Returns true if attribute value is valid.
    inline virtual bool validate() const;

    //! This method returns a copy of the accessor. 
    inline virtual PmtAttributePtr clone() const
    {
        return new PmtAttributeT < TYPE > (*this);
    }

    inline virtual void show(int level = 0) const;

  protected:
    inline virtual void addFacet(const PmtKeyType & facetName,
                                 const vector < SPmtString > &value,
                                 bool fixed);

    TYPE mValue;
    TYPE mFixed;
    SPmtString mUse;
    vector < PmtFacetPtr > mFacets;
};


template < class TYPE > inline EkString PmtAttributeT < TYPE >::getValueStr() const
{
    ostringstream c;
    PmtPrintValue(c, mValue);   // performs c << value
#if defined(_UNICODE)
    return EkAToW(c.str());
#else
    return c.str();
#endif
}


template < class TYPE >
    inline void PmtAttributeT < TYPE >::setValueStr(const EkString & valStr)
{
    string tmp;
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


template < class TYPE > inline EkString PmtAttributeT < TYPE >::getFixedStr() const
{
    ostringstream c;
    PmtPrintValue(c, mFixed);   // performs c << value
#if defined(_UNICODE)
    return EkAToW(c.str());
#else
    return c.str();
#endif
}


template < class TYPE >
    inline void PmtAttributeT < TYPE >::setFixedStr(const EkString & valStr)
{
    string tmp;
#if defined(_UNICODE)
    tmp = EkWToA(valStr);
#else
    tmp = valStr;
#endif
    istringstream c(tmp);
    PmtReadValue(c, mFixed);    // performs c >> value
    return;
}


template < class TYPE > inline EkString PmtAttributeT < TYPE >::getUseStr() const
{
    string tmp;
    ostringstream c(tmp);
    PmtPrintValue(c, mUse);     // performs c << value

    c.flush();
#if defined(_UNICODE)
    return EkAToW(tmp);
#else
    return tmp;
#endif
}


template < class TYPE >
    inline void PmtAttributeT < TYPE >::setUseStr(const EkString & valStr)
{

    string tmp;
#if defined(_UNICODE)
    tmp = EkWToA(valStr);
#else
    tmp = valStr;
#endif
    istringstream c(tmp);
    PmtReadValue(c, mUse);      // performs c >> value
    return;
}


template < class TYPE > inline bool PmtAttributeT < TYPE >::validate() const
{
    for (unsigned int i = 0; i < mFacets.size(); i++)
    {
        if (!mFacets[i]->validate(&mValue))
            return false;
    }
    return true;
}


template < class TYPE > inline void PmtAttributeT < TYPE >::show(int level) const
{
    int i;
    for (i = 0; i < level; i++)
        cout << "  ";

    cout << "PmtAttribute(" << mName << ", " << mType << ", [";
    PmtPrintValue(cout, mValue);

    cout << ", ";
    PmtPrintValue(cout, mUse);
    cout << ", ";
    PmtPrintValue(cout, mFixed);

    cout << "])" << endl;

    for (unsigned int j = 0; j < mFacets.size(); j++)
        mFacets[j]->show(level + 1);
}


template < class TYPE >
    inline void PmtAttributeT < TYPE >::addFacet(const PmtKeyType & facetName,
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


#if defined(_MSC_VER) && defined(INTERPRETER_CPP)
extern template class PmtAttributeT < vector < char > >;
extern template class PmtAttributeT < char >;
extern template class PmtAttributeT < vector < unsigned char > >;
extern template class PmtAttributeT < unsigned char >;
extern template class PmtAttributeT < vector < short > >;
extern template class PmtAttributeT < short >;
extern template class PmtAttributeT < vector < unsigned short > >;
extern template class PmtAttributeT < unsigned short >;
extern template class PmtAttributeT < vector < long > >;
extern template class PmtAttributeT < long >;
extern template class PmtAttributeT < vector < unsigned long > >;
extern template class PmtAttributeT < unsigned long >;
extern template class PmtAttributeT < vector < bool > >;
extern template class PmtAttributeT < bool >;
extern template class PmtAttributeT < vector < float > >;
extern template class PmtAttributeT < float >;
extern template class PmtAttributeT < vector < double > >;
extern template class PmtAttributeT < double >;
extern template class PmtAttributeT < vector < SPmtString > >;
extern template class PmtAttributeT < SPmtString >;
#endif


#endif

