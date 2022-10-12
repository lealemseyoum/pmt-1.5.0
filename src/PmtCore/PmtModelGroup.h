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
 * Creation Date: Feb. 2, 2001
 *
 * Original Author: 
 * George Sotak    george.sotak@kodak.com
 * Copyright (C) 2001 Eastman Kodak Company. All Rights Reserved. 
 * 
 * Contributor(s): 
 * Dan Rupe  rupe@kodak.com
*/


#ifndef PMT_MODEL_GROUP_H
#define PMT_MODEL_GROUP_H

#if defined( _MSC_VER)
#pragma once
#pragma warning (disable:4231)
#pragma warning (disable:4786)
#endif

#if (defined _MSC_VER) && (!defined _WINDOWS_) && (!defined __AFXWIN_H__)
#include "windows.h"
#endif

#include "PmtCommon.h"
#include "PmtCoreDefs.h"
#include "PmtMetadata.h"
#include "PmtElement.h"

class PmtModelGroup;
class PmtMetadataVisitor;
class PmtCompositeMetadata;

typedef EkSmartPtr < PmtModelGroup > PmtModelGroupPtr;
typedef EkConstSmartPtr < PmtModelGroup > PmtModelGroupPtrConst;

typedef list < pair < PmtKeyType, PmtElementPtr > >PmtMetadataList;
typedef PmtMetadataList::iterator PmtMdListIter;
typedef PmtMetadataList::const_iterator PmtMdListConstIter;

typedef list < PmtModelGroupPtr > PmtModelGroupList;
typedef PmtModelGroupList::iterator PmtModelGroupListIter;
typedef PmtModelGroupList::const_iterator PmtModelGroupListConstIter;


class PMTCORE_DECL PmtModelGroup:public EkRefCount < EK_DEFAULT_MUTEX >
{
  public:
  
    inline PmtModelGroup(PmtCompositeMetadata * parentMd):mParentMd(parentMd)
    {
    }
    
    inline virtual ~ PmtModelGroup()
    {
    }

    virtual PmtModelGroup *clone(PmtCompositeMetadata * parentMd) const = 0;

    virtual bool okToCreate(const PmtKeyType & key) = 0;

    virtual SPmtString type() = 0;

    PmtMetadataPtr getMetadatum(const PmtKeyType & key, int occurNum);

    bool deleteMetadatum(const PmtKeyType & key, int occurNum);

    void addChildModelGroup(PmtModelGroupPtr childModelGroup);

    const PmtCompositeMetadata *parentMd() const
    {
        return mParentMd;
    }
    
    PmtCompositeMetadata *&parentMd()
    {
        return mParentMd;
    }

    void addMetadataKey(const PmtKeyType & key, const PmtKeyType & fullKey,
                        const SPmtString & minOccurs,
                        const SPmtString & maxOccurs,
                        const SPmtString & nillable);
                        
    void addMetadataKey(const PmtKeyType & key, const PmtKeyType & fullKey,
                        const unsigned int & minOccurs,
                        const unsigned int & maxOccurs,
                        const SPmtString & nillable);

    void show(bool showObjectMemoryPointers = false, int level =
              0, bool showModelGroups = false);

  protected:
  
    PmtCompositeMetadata * mParentMd;
    PmtModelGroupList mChildren;
    PmtMetadataList mMdList;

    inline PmtModelGroup()
    {
    }
    
    PmtModelGroup(const PmtModelGroup & orig, PmtCompositeMetadata * parentMd);

    PmtMdListIter checkKey(const PmtKeyType & key);
    
    int getItemIndex(const PmtKeyType & key);
    
    inline int numItems()
    {
        return mMdList.size() + mChildren.size();
    }
    
    bool anyCreated(int indexToSkip);
    
    bool doesDirectlyReside(const PmtKeyType & key);
    
    PmtModelGroupPtr getModelGroupFromIndex(int index);
};


class PmtSequenceModelGroup:public PmtModelGroup
{
  public:
  
    inline PmtSequenceModelGroup(PmtCompositeMetadata *
                                 parentMd):PmtModelGroup(parentMd)
    {
    }
    
    inline ~ PmtSequenceModelGroup()
    {
    }

    inline virtual PmtModelGroup *clone(PmtCompositeMetadata * parentMd) const
    {
        return new PmtSequenceModelGroup(*this, parentMd);
    }

    virtual bool okToCreate(const PmtKeyType & key);

    inline virtual SPmtString type()
    {
        return "sequence";
    }

  private:
  
    inline PmtSequenceModelGroup()
    {
    }

    inline PmtSequenceModelGroup(const PmtSequenceModelGroup & orig,
                                 PmtCompositeMetadata *
                                 parentMd):PmtModelGroup(orig, parentMd)
    {
    }
};


class PmtChoiceModelGroup:public PmtModelGroup
{
  public:
  
    inline PmtChoiceModelGroup(PmtCompositeMetadata *
                               parentMd):PmtModelGroup(parentMd)
    {
    }
    
    inline ~ PmtChoiceModelGroup()
    {
    }

    inline virtual PmtModelGroup *clone(PmtCompositeMetadata * parentMd) const
    {
        return new PmtChoiceModelGroup(*this, parentMd);
    }

    virtual bool okToCreate(const PmtKeyType & key);

    inline virtual SPmtString type()
    {
        return "choice";
    }

  private:
  
    inline PmtChoiceModelGroup()
    {
    }

    inline PmtChoiceModelGroup(const PmtChoiceModelGroup & orig,
                               PmtCompositeMetadata *
                               parentMd):PmtModelGroup(orig, parentMd)
    {
    }
};


#endif                          // PMT_MODEL_GROUP_H

