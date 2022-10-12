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
 * Creation date: May, 2001
 *
 * Original Author: 
 * Chris Lin    ti.lin@kodak.com
 * Copyright (C) 2001 Eastman Kodak Company. All Rights Reserved.
 * 
 * Contributor(s): 
 */

#ifndef PMT_ATTRIBUTE_H
#define PMT_ATTRIBUTE_H

#if (defined _MSC_VER) && (!defined _WINDOWS_) && (!defined __AFXWIN_H__)
#include "windows.h"
#endif

#include "EkCompiler.h"
#include "EkSmartPtr.h"
#include "EkRefCount.h"
#include "EkStrings.h"
#include "EkFactoryT.h"

#include "PmtCommon.h"
#include "PmtCoreDefs.h"

// Put some forward class references
class PmtAttribute;

// Smart pointers
typedef EkSmartPtr < PmtAttribute > PmtAttributePtr;
typedef EkConstSmartPtr < PmtAttribute > PmtConstAttributePtr;

#ifdef PMTCORE_EXPLICIT
PMTCORE_TMPL_EXT template class PMTCORE_DECL EkConstSmartPtr < PmtAttribute >;
PMTCORE_TMPL_EXT template class PMTCORE_DECL EkSmartPtr < PmtAttribute >;
PMTCORE_TMPL_EXT template class PMTCORE_DECL EK_DLL_MAP( PmtKeyType, PmtAttributePtr );
PMTCORE_TMPL_EXT template class PMTCORE_DECL EkFactoryT < PmtKeyType,
    PmtAttributePtr >;
#endif

typedef EK_DLL_MAP(PmtKeyType, PmtAttributePtr) PmtAttributeMap;


//! PmtMdFactoryMap is a wrapper class of factory map
typedef EkFactoryT < PmtKeyType, PmtAttributePtr > PmtAttrFactory;


class PMTCORE_DECL PmtAttrFactories
{

  public:
    PmtAttrFactories()
    {
    };
    ~PmtAttrFactories();

    PmtAttrFactory *getAttrFactory(const SPmtString & uri);
    void clear();
    void clear(const SPmtString & nameSpace);

  private:

    typedef map < SPmtString, PmtAttrFactory * >AttrFactMap;
    AttrFactMap mFactoryMap;

    PmtAttrFactory *createAttrFactory(const SPmtString & uri);
};


//! PmtAttribute is the base metadata attribute class. 
/*! PmtAttribute serves as a base class for all potential atribute specialization.
 It provides a common interface for accessing, manipulating metadata attributes. 
 Analogous to XML attributes, an attribute object contains two piece of information: a name and a
 value.
*/
class PMTCORE_DECL PmtAttribute:public EkRefCount < EK_DEFAULT_MUTEX >
{
  public:
    //! Default constructor
    inline PmtAttribute()
    {
        mIsChanged = false;
    }

    //! Constructor
    inline PmtAttribute(const PmtKeyType & name, const PmtKeyType & type,
                        const PmtKeyType & trueType):mName(name),
        mTrueType(trueType), mType(type), mIsChanged(false)
    {
    }

    //! Copy constructor
    inline PmtAttribute(const PmtAttribute & copy):mName(copy.mName),
        mTrueType(copy.mTrueType), mType(copy.mType),
        mIsChanged(copy.mIsChanged)
    {
    }

    //! Destructor
    inline virtual ~ PmtAttribute()
    {
    }

    //! Returns a const reference to the attribute's name.
    inline const PmtKeyType & name() const
    {
        return mName;
    }

    //! Returns a const reference to the attribute's value type.
    inline const PmtKeyType & type() const
    {
        return mType;
    }

    inline const PmtKeyType & trueType() const
    {
        return mTrueType;
    }

    //! This method returns the value of the attribute
    virtual EkString getValueStr() const = 0;

    //! This method sets the value of the attribute
    virtual void setValueStr(const EkString & valStr) = 0;

    inline virtual bool isChanged()
    {
        return mIsChanged;
    };
    //! This method returns the value of the attribute
    virtual EkString getFixedStr() const = 0;

    //! This method sets the value of the attribute
    virtual void setFixedStr(const EkString & valStr) = 0;

    //! This method returns the value of the attribute
    virtual EkString getUseStr() const = 0;

    //! This method sets the value of the attribute
    virtual void setUseStr(const EkString & valStr) = 0;

    //! Returns true if the attribute value is valid
    virtual bool validate() const = 0;

    //! This method returns a copy of the accessor. 
    virtual PmtAttributePtr clone() const = 0;

    virtual void show(int level = 0) const = 0;

    static PmtAttrFactory *getFactory(const SPmtString & nameSpace);
    inline static PmtAttrFactories & getFactories(void)
    {
        return msAttrFactoryMap;
    }

    inline virtual void addFacet(const PmtKeyType & facetName,
                                 const vector < SPmtString > &value,
                                 bool fixed)
    {
    }

  protected:
    static PmtAttrFactories msAttrFactoryMap;
    PmtKeyType mName;
    PmtKeyType mTrueType;
    PmtKeyType mType;

    // flag to check if the setValueStr() or the value() was called on this attribute..
    // we check for this flag when writting out the instance.
    bool mIsChanged;

};

#endif

