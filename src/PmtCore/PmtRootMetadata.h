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
 * Creation Date: Aug 2001
 *
 * Original Author:
 * Chris Lin ti.lin@kodak.com
 * 
 * Contributor(s):
*/

#ifndef PMT_ROOT_METADATA_H
#define PMT_ROOT_METADATA_H

#include "PmtCompositeMetadata.h"

class PmtRootMetadata;

// Smart pointers
typedef EkSmartPtr < PmtRootMetadata > PmtRootMetadataPtr;
typedef EkConstSmartPtr < PmtRootMetadata > ConstPmtRootMetadataPtr;

#ifdef PMTCORE_EXPLICIT
PMTCORE_TMPL_EXT template class PMTCORE_DECL EkConstSmartPtr <
    PmtRootMetadata >;
PMTCORE_TMPL_EXT template class PMTCORE_DECL EkSmartPtr < PmtRootMetadata >;
#endif

//! The PmtRootMetadata class is the root container of all other metadata.
class PMTCORE_DECL PmtRootMetadata:public PmtCompositeMetadata
{
  public:
    inline PmtRootMetadata():PmtCompositeMetadata(), mFactory(0),
        mAttrFactory(0), mNameSpace("")
    {
    }

    inline PmtRootMetadata(const PmtKeyType & mdKey, const SPmtString & type,
                           const SPmtString & trueType,
                           PmtMetadataFactory * factory,
                           PmtAttrFactory * attrFact,
                           const SPmtString &
                           nameSpace):PmtCompositeMetadata(mdKey, type,
                                                           trueType),
        mFactory(factory), mAttrFactory(attrFact), mNameSpace(nameSpace)
    {
    }
    
    inline PmtRootMetadata(const PmtKeyType & mdKey, const SPmtString & type,
                           PmtMDType mdType,
                           PmtMetadataFactory * factory,
                           PmtAttrFactory * attrFact,
                           const SPmtString &
                           nameSpace):PmtCompositeMetadata(mdKey, type,
                                                           mdType),
        mFactory(factory), mAttrFactory(attrFact), mNameSpace(nameSpace)
    {
    }    

    inline PmtRootMetadata(const PmtRootMetadata &
                           orig):PmtCompositeMetadata(orig),
        mFactory(orig.mFactory), mAttrFactory(orig.mAttrFactory),
        mNameSpace(orig.mNameSpace)
    {
    }

    virtual inline ~ PmtRootMetadata()
    {
    }

    inline virtual PmtMetadata *clone(void) const
    {
        return new PmtRootMetadata(*this);
    }

    inline virtual PmtMetadataFactory *getMdFactory() const
    {
        return mFactory;
    }
    inline virtual PmtAttrFactory *getAttrFactory() const
    {
        return mAttrFactory;
    }

    inline virtual const SPmtString & getNameSpace(void) const
    {
        return mNameSpace;
    }

    inline virtual PmtRootMetadata *getRootMetadata(void)
    {
        return this;
    }

  private:
    PmtMetadataFactory * mFactory;
    PmtAttrFactory *mAttrFactory;
    SPmtString mNameSpace;

    inline PmtRootMetadata & operator=(PmtRootMetadata &)
    {
        return *this;
    }
};

#endif

