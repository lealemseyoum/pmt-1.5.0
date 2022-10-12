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
 * Creation Date: 01/29/2001
 *
 * Original Author: 
 * Lin, Chris  ti.lin@kodak.com
 * Dan Rupe  rupe@kodak.com
 * Copyright (C) 2001 Eastman Kodak Company. All Rights Reserved. 
 * 
 * Contributor(s): 
*/
#ifndef PMT_FACET_T_H
#define PMT_FACET_T_H

#include <string>
#include <sstream>
#include <algorithm>

#include "PmtFacet.h"
#include "PmtIoFunctions.h"
#include "PmtError.h"

template < class TYPE > class PmtFacetEnumeration:public PmtFacet
{
  public:
  inline PmtFacetEnumeration():PmtFacet(FACET_ENUMERATION)
    {
    }

    inline PmtFacetEnumeration(const PmtFacetEnumeration < TYPE >
                               &src):PmtFacet(src),
        mEnumeration(src.mEnumeration)
    {
    }

    inline virtual bool validate(const void *value) const;

    virtual void init(const vector < SPmtString > &value, bool fixed);
    virtual void setFacetValue(const vector < SPmtString > &value, bool fixed);
    virtual PmtFacetPtr clone() const
    {
        return new PmtFacetEnumeration < TYPE > (*this);
    }
    inline virtual void show(int level = 0) const;

  private:
    typedef vector < TYPE > FacetEnumerationType;
    FacetEnumerationType mEnumeration;

    inline PmtFacetEnumeration & operator=(const PmtFacetEnumeration & rhs);
};


template < class TYPE >
    inline bool PmtFacetEnumeration < TYPE >::validate(const void *value) const
{
    if (mEnumeration.empty())
        return false;
    else
    {
        for (unsigned int i = 0; i < mEnumeration.size(); i++)
        {
            const TYPE *tmp = static_cast < const TYPE * >(value);
            if (mEnumeration[i] == *tmp)
                return true;
        }
    }

    return false;
}


template < class TYPE >
    inline void PmtFacetEnumeration < TYPE >::init(const vector < SPmtString >
                                                   &value, bool fixed)
{
    mEnumeration.clear();
    for (unsigned int i = 0; i < value.size(); i++)
    {
        TYPE tmpValue;
        istringstream c(value[i]);
        PmtReadValue(c, tmpValue);
        mEnumeration.push_back(tmpValue);
    }
    mFixed = fixed;
}


template < class TYPE >
    inline void PmtFacetEnumeration < TYPE >::setFacetValue(const vector <
                                                            SPmtString >
                                                            &value, bool fixed)
{
    vector < TYPE > tmpVec;
    unsigned int i;
    for (i = 0; i < value.size(); i++)
    {
        TYPE tmpValue;
        istringstream c(value[i]);
        PmtReadValue(c, tmpValue);
        if (find(mEnumeration.begin(), mEnumeration.end(), tmpValue) ==
            mEnumeration.end())
            throw PmtError(PMT_ERROR,
                           "Restriction on PmtFacetEnumeration failed - check Metadata definition.",
                           WHERE);
        tmpVec.push_back(tmpValue);
    }
    mEnumeration.clear();
    for (i = 0; i < tmpVec.size(); i++)
        mEnumeration.push_back(tmpVec[i]);
}


template < class TYPE >
    inline void PmtFacetEnumeration < TYPE >::show(int level) const
{
    for (int i = 0; i < level; i++)
        cout << "  ";

    cout << "Facet(" << mFacetName << ", ";
    if (mFixed)
        cout << "true, ";
    else
        cout << "false, [";
    PmtPrintValue(cout, mEnumeration);
    cout << "])" << endl;
}


template < class TYPE > class PmtFacetMinInclusive:public PmtFacet
{
  public:
  inline PmtFacetMinInclusive():PmtFacet(FACET_MIN_INCLUSIVE)
    {
    }

    inline PmtFacetMinInclusive(const PmtFacetMinInclusive < TYPE >
                                &src):PmtFacet(src),
        mFacetValue(src.mFacetValue)
    {
    }

    inline virtual bool validate(const void *value) const
    {
        const TYPE *tmp = static_cast < const TYPE * >(value);
         return mFacetValue <= *tmp;
    }
    virtual void init(const vector < SPmtString > &value, bool fixed);
    virtual void setFacetValue(const vector < SPmtString > &value, bool fixed);
    virtual PmtFacetPtr clone() const
    {
        return new PmtFacetMinInclusive < TYPE > (*this);
    }
    inline virtual void show(int level = 0) const;

  private:
    inline PmtFacetMinInclusive & operator=(const PmtFacetMinInclusive & rhs);

    TYPE mFacetValue;
};


template < class TYPE >
    inline void PmtFacetMinInclusive < TYPE >::init(const vector < SPmtString >
                                                    &value, bool fixed)
{
    istringstream c(value[0]);
    PmtReadValue(c, mFacetValue);
    mFixed = fixed;
}


template < class TYPE >
    inline void PmtFacetMinInclusive < TYPE >::setFacetValue(const vector <
                                                             SPmtString >
                                                             &value,
                                                             bool fixed)
{
    if (mFixed)
        throw PmtError(PMT_ERROR,
                       "Can't derive from a facet with a fixed attribute being true",
                       WHERE);

    TYPE tmpValue;
    istringstream c(value[0]);
    PmtReadValue(c, tmpValue);
    if (tmpValue < mFacetValue)
        throw PmtError(PMT_ERROR,
                       "Restriction on PmtFacetMinInclusive failed - check Metadata definition.",
                       WHERE);

    mFacetValue = tmpValue;
    mFixed = fixed;
}


template < class TYPE >
    inline void PmtFacetMinInclusive < TYPE >::show(int level) const
{
    for (int i = 0; i < level; i++)
        cout << "  ";

    cout << "Facet(" << mFacetName << ", ";
    if (mFixed)
        cout << "true, ";
    else
        cout << "false, [";
    PmtPrintValue(cout, mFacetValue);
    cout << "])" << endl;
}


template < class TYPE > class PmtFacetMaxInclusive:public PmtFacet
{
  public:
  inline PmtFacetMaxInclusive():PmtFacet(FACET_MAX_INCLUSIVE)
    {
    }

    inline PmtFacetMaxInclusive(const PmtFacetMaxInclusive < TYPE >
                                &src):PmtFacet(src),
        mFacetValue(src.mFacetValue)
    {
    }

    inline virtual bool validate(const void *value) const
    {
        const TYPE *tmp = static_cast < const TYPE * >(value);
         return mFacetValue >= *tmp;
    }

    virtual void init(const vector < SPmtString > &value, bool fixed);
    virtual void setFacetValue(const vector < SPmtString > &value, bool fixed);
    virtual PmtFacetPtr clone() const
    {
        return new PmtFacetMaxInclusive < TYPE > (*this);
    }
    inline virtual void show(int level = 0) const;

  private:
    inline PmtFacetMaxInclusive & operator=(const PmtFacetMaxInclusive & rhs)
    {
    }

    TYPE mFacetValue;
};


template < class TYPE >
    inline void PmtFacetMaxInclusive < TYPE >::init(const vector < SPmtString >
                                                    &value, bool fixed)
{
    istringstream c(value[0]);
    PmtReadValue(c, mFacetValue);
    mFixed = fixed;
}


template < class TYPE >
    inline void PmtFacetMaxInclusive < TYPE >::setFacetValue(const vector <
                                                             SPmtString >
                                                             &value,
                                                             bool fixed)
{
    if (mFixed)
        throw PmtError(PMT_ERROR,
                       "Can't derive from a facet with a fixed attribute being true",
                       WHERE);

    TYPE tmpValue;
    istringstream c(value[0]);
    PmtReadValue(c, tmpValue);
    if (tmpValue > mFacetValue)
        throw PmtError(PMT_ERROR,
                       "Restriction on PmtFacetMinInclusive failed - check Metadata definition.",
                       WHERE);

    mFacetValue = tmpValue;
    mFixed = fixed;
}


template < class TYPE >
    inline void PmtFacetMaxInclusive < TYPE >::show(int level) const
{
    for (int i = 0; i < level; i++)
        cout << "  ";

    cout << "Facet(" << mFacetName << ", ";
    if (mFixed)
        cout << "true, [";
    else
        cout << "false, [";
    PmtPrintValue(cout, mFacetValue);
    cout << "])" << endl;
}

#endif

