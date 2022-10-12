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

#ifndef PMT_FACET_H
#define PMT_FACET_H

#if (defined _MSC_VER) && (!defined _WINDOWS_) && (!defined __AFXWIN_H__)
#include "windows.h"
#endif

#include "EkRefCount.h"
#include "EkSmartPtr.h"
#include "EkFactoryT.h"
#include "PmtCommon.h"

const char *const FACET_MIN_INCLUSIVE = "xsd:minInclusive";
const char *const FACET_MAX_INCLUSIVE = "xsd:maxInclusive";
const char *const FACET_ENUMERATION = "xsd:enumeration";

class PmtFacet;

typedef EkSmartPtr < PmtFacet > PmtFacetPtr;
typedef EkConstSmartPtr < PmtFacet > PmtConstFacetPtr;

typedef EkFactoryT < PmtKeyType, PmtFacetPtr > PmtFacetFactoryType;

// Note that this is only defined for DLL builds with MS compilers
#ifdef PMTCORE_EXPLICIT

// VC6 complains (somewhat erroneously) about extern template instantiations.  
// Disable those complaints here...
#pragma warning( push )
#pragma warning( disable : 4231)

PMTCORE_TMPL_EXT template class PMTCORE_DECL EkConstSmartPtr < PmtFacet >;
PMTCORE_TMPL_EXT template class PMTCORE_DECL EkSmartPtr < PmtFacet >;
PMTCORE_TMPL_EXT template class PMTCORE_DECL EK_DLL_MAP(PmtKeyType,
                                                        PmtFacetPtr);
PMTCORE_TMPL_EXT template class PMTCORE_DECL EkFactoryT < PmtKeyType,
    PmtFacetPtr >;
    
// Reset the warnings as they were
#pragma warning( pop )

#endif


//! The PmtFacet class is the base class for all facet specializations
/*! Facets are range restrictions on the domain of a metadata's value. The
  PmtFacet class provides the base class definition for the variety of
  facets that can be associated with a value.
*/
class PmtFacet:public EkRefCount < EK_DEFAULT_MUTEX >
{
  public:
    virtual ~ PmtFacet()
    {
    }
    virtual bool validate(const void *value) const = 0;
    virtual void init(const vector < SPmtString > &value, bool fixed) = 0;
    virtual void setFacetValue(const vector < SPmtString > &value,
                               bool fixed) = 0;
    inline const PmtKeyType & getFacetName()
    {
        return mFacetName;
    }
    inline static PmtFacetFactoryType & getFactory()
    {
        return msFactory;
    }
    //inline static PmtFacetFactoryType* getFactory() { return msFactory; }
    virtual PmtFacetPtr clone() const = 0;
    virtual void show(int level = 0) const = 0;

  protected:
    inline PmtFacet(const PmtKeyType & facetName):mFixed(false),
        mFacetName(facetName)
    {
    }

    inline PmtFacet(const PmtFacet & src):mFixed(src.mFixed),
        mFacetName(src.mFacetName)
    {
    }

  protected:
    bool mFixed;
    PmtKeyType mFacetName;

    static PmtFacetFactoryType msFactory;
    //static EkGPtr<PmtFacetFactoryType> msFactory;
};


#endif                          // PMT_FACET_H

