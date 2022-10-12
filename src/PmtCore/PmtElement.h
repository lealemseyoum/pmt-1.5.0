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
 * Creation date: March 6, 2001
 *
 * Original Author: 
 * George Sotak   george.sotak@kodak.com
 * Copyright (C) 2001 Eastman Kodak Company. All Rights Reserved.
 * 
 * Contributor(s):
 * Dan Rupe  rupe@kodak.com
 *
*/

#ifndef PMT_ELEMENT_H
#define PMT_ELEMENT_H

#ifdef _MSC_VER
#pragma once
#pragma warning (disable:4231)
#endif

#if (defined _MSC_VER) && (!defined _WINDOWS_) && (!defined __AFXWIN_H__)
#include "windows.h"
#endif

#include <vector>

#include "EkCompiler.h"
#include "EkRefCount.h"
#include "EkSmartPtr.h"

#include "PmtCoreDefs.h"
#include "PmtMetadata.h"
#include "PmtOccurrence.h"

class PmtElement;
class PmtMetadataVisitor;
class PmtMdVisitorImpl;

typedef vector < PmtMetadataPtr > PmtMdVec;
typedef PmtMdVec::iterator PmtMdVecIter;
typedef PmtMdVec::const_iterator PmtMdVecConstIter;

typedef EkSmartPtr < PmtElement > PmtElementPtr;
typedef EkConstSmartPtr < PmtElement > PmtConstElementPtr;


#ifdef PMTCORE_EXPLICIT
PMTCORE_TMPL_EXT template class PMTCORE_DECL EkConstSmartPtr < PmtElement >;
PMTCORE_TMPL_EXT template class PMTCORE_DECL EkSmartPtr < PmtElement >;
#endif

//! The PmtElement class is the base class for handling multiple occurance of a metadata instance.
/*! The metadata declaration construct, <em>\<element\></em>, allows
for the specification of the expected occurance of a metadata
instance. This is specificed through its \em minOccurs and \em maxOccurs
attributes. The PmtElement class and its derivitives provide the
run-time functionality for this specification.  These classes are
intended to be used by the PmtModelGroup and PmtCompositeMetadata
classes in the repesective metadata containers. 
*/

/* INTENDED USAGE: For example, PmtModelGroup currently has
list<pair<PmtKeyType, PmtMetadataPtr> > for metadata containment. The
PmtElement will replace the parameter of the list, e.g.,
list<PmtElementPtr >. The PmtCompositeMetadata will continue to have a
list of iterators from the PmtModelGroup list. However, the access to
the metadata instances will change since the access will need to be
done through the PmtElement interface.
*/
class PmtElement:public EkRefCount < EK_DEFAULT_MUTEX >
{
  public:
    PmtElement(const PmtKeyType & fullKey, int minOccurs, int maxOccurs,
               SPmtString nillable):mFullKeyPath(fullKey),
        mMinOccurs(minOccurs), mMaxOccurs(maxOccurs), mNillable(nillable)
    {
    }
    // Copy constructor
    PmtElement(const PmtElement & orig):mFullKeyPath(orig.mFullKeyPath),
        mMinOccurs(orig.mMinOccurs), mMaxOccurs(orig.mMaxOccurs),
        mNillable(orig.mNillable)
    {
    }
    virtual ~ PmtElement()
    {
    }

    // Analyze min and max and instantiate the appropriate implementation
    static PmtElementPtr create(const PmtKeyType & fullKeyPath,
                                const SPmtString & minOccurs,
                                const SPmtString & maxOccurs,
                                const SPmtString & nillable);
    static PmtElementPtr create(const PmtKeyType & fullKeyPath,
                                const unsigned int & minOccurs,
                                const unsigned int & maxOccurs,
                                const SPmtString & nillable);

    inline const unsigned int &minOccurs() const
    {
        return mMinOccurs;
    }
    inline unsigned int &minOccurs()
    {
        return mMinOccurs;
    }
    inline const unsigned int &maxOccurs() const
    {
        return mMaxOccurs;
    }
    inline unsigned int &maxOccurs()
    {
        return mMaxOccurs;
    }

    inline SPmtString & getNillable()
    {
        return mNillable;
    }

    virtual PmtMetadataPtr getMetadatum(unsigned int occurNum,
                                        bool createMetadataIfNotExists,
                                        const PmtCompositeMetadata * parent =
                                        NULL) = 0;
    virtual void deleteMetadatum(int occurNum) = 0;
    virtual void deleteMetadataAll() = 0;
    virtual unsigned int getWildcardMax() = 0;
    virtual bool isEmpty() = 0;
    virtual PmtElementPtr clone(PmtCompositeMetadata * parentMd) const = 0;
    virtual ostream & printDerived(ostream & c) const = 0;
    virtual void accept(PmtMetadataVisitor * visitor) = 0;
    virtual void accept(PmtMdVisitorImpl * visitor) = 0;
    virtual void show(bool showObjectMemoryPointers = false, int level =
                      0) const = 0;

  protected:
    PmtKeyType mFullKeyPath;

    // mMinOccurs will be used in a makeValid method
    // to make sure the minimum number of metadata
    // are present
    unsigned int mMinOccurs;

    // mMaxOccurs with a value less than zero means
    // there is no Maximum.
    unsigned int mMaxOccurs;
    SPmtString mNillable;

    PmtKeyType allButRootPath();
    PmtMetadataPtr createFromFactoryAndSetParentAndOccurrence(const
                                                              PmtCompositeMetadata
                                                              * parent,
                                                              int occurNum);

    inline PmtElement()
    {
    }
    inline PmtElement & operator=(const PmtElement & orig)
    {
        return *this;
    }
};


//! Handles the general case of minOccurs and maxOccurs being any value beside 0 and 1, respectively.
class PmtVectorElement:public PmtElement
{
  public:
    PmtVectorElement(const SPmtString & name, int minOccurs, int maxOccurs,
                     SPmtString nillable):PmtElement(name, minOccurs,
                                                     maxOccurs, nillable)
    {
    }
    // Copy Constructor
    PmtVectorElement(const PmtVectorElement & orig,
                     PmtCompositeMetadata * parentMd);
    ~PmtVectorElement()
    {
    }

    virtual PmtMetadataPtr getMetadatum(unsigned int occurNum,
                                        bool createMetadataIfNotExists,
                                        const PmtCompositeMetadata * parent =
                                        NULL);
    virtual void deleteMetadatum(int occurNum);
    virtual void deleteMetadataAll()
    {
        mMdVec.clear();
    }
    virtual unsigned int getWildcardMax();
    virtual bool isEmpty();
    virtual PmtElementPtr clone(PmtCompositeMetadata * parentMd) const
    {
        return new PmtVectorElement(*this, parentMd);
    }
    virtual ostream & printDerived(ostream & c) const;
    virtual void accept(PmtMetadataVisitor * visitor);
    virtual void accept(PmtMdVisitorImpl * visitor);
    virtual void show(bool showObjectMemoryPointers = false, int level =
                      0) const;

  private:
    PmtMdVec mMdVec;

    PmtMetadataPtr getMetadatumCheckingForMinOccurrences(unsigned int occurNum,
                                                         const
                                                         PmtCompositeMetadata *
                                                         parent);
    bool createOccurrenceMetadata(int numToCreate,
                                  const PmtCompositeMetadata * parent);
    PmtMetadataPtr getMetadatumOrCreateAndSetParent(unsigned int occurNum,
                                                    const PmtCompositeMetadata
                                                    * parent);
    void removeFromVector(int occurNum);
    inline PmtVectorElement()
    {
    }
    inline PmtVectorElement & operator=(const PmtVectorElement & orig)
    {
        return *this;
    }
};


//! Handles special case of minOccurs = 0 or 1 (and maxOccurs = 1)
class PmtOptionalOrSingularElement:public PmtElement
{
  public:
    PmtOptionalOrSingularElement(const SPmtString & name, int minOccurs,
                                 int maxOccurs,
                                 SPmtString nillable):PmtElement(name,
                                                                 minOccurs,
                                                                 maxOccurs,
                                                                 nillable),
        mMd(NULL)
    {
    }
    // Copy Constructor
    PmtOptionalOrSingularElement(const PmtOptionalOrSingularElement & orig,
                                 PmtCompositeMetadata *
                                 parentMd):PmtElement(orig), mMd(orig.mMd)
    {
        if (mMd)
            mMd->clone(parentMd);
    }
    ~PmtOptionalOrSingularElement()
    {
    }

    virtual PmtMetadataPtr getMetadatum(unsigned int occurNum,
                                        bool createMetadataIfNotExists,
                                        const PmtCompositeMetadata * parent =
                                        NULL);
    virtual void deleteMetadatum(int occurNum)
    {
        mMd = NULL;
    }
    virtual void deleteMetadataAll()
    {
        mMd = NULL;
    }
    virtual unsigned int getWildcardMax()
    {
        return mMaxOccurs;
    }
    virtual bool isEmpty()
    {
        return !mMd;
    }
    virtual PmtElementPtr clone(PmtCompositeMetadata * parentMd) const
    {
        return new PmtOptionalOrSingularElement(*this, parentMd);
    }
    virtual ostream & printDerived(ostream & c) const;
    virtual void accept(PmtMetadataVisitor * visitor);
    virtual void accept(PmtMdVisitorImpl * visitor);
    virtual void show(bool showObjectMemoryPointers = false, int level =
                      0) const;

  private:
    PmtMetadataPtr mMd;

    PmtMetadataPtr createMetadatumAndSetParent(const PmtCompositeMetadata *
                                               parent);
    inline PmtOptionalOrSingularElement()
    {
    }
    inline PmtOptionalOrSingularElement & operator=(const
                                                    PmtOptionalOrSingularElement
                                                    & orig)
    {
        return *this;
    }
};


#endif                          // PMT_ELEMENT_H

