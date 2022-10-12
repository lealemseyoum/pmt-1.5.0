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
 * Creation Date: January 1999
 *
 * Original Author:
 * George Sotak george.sotak@kodak.com
 * 
 * Contributor(s):
 * Dan Rupe
 * Ricardo Rosario ricardo.rosario@kodak.com
*/


#ifndef PMT_COMPOSITE_METADATA_H
#define PMT_COMPOSITE_METADATA_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#pragma warning (disable:4231)  // as PmtXmlAccessor.cpp get 4231 even have EkCompiler.h included
#endif

#if (defined _MSC_VER) && (!defined _WINDOWS_) && (!defined __AFXWIN_H__)
#include "windows.h"
#endif

#include <map>
#include "PmtMetadata.h"
#include "PmtModelGroup.h"

///////////////////////////////////////////////////////////////////////////////
// Convenience Macros
#define PMT_CAST_COMPOSITE(M) static_cast<PmtCompositeMetadata*>((M).ptr())


class PmtAbstractImage;
class PmtMetadataVisitor;
class PmtCompositeMetadata;

// Smart pointers
typedef EkSmartPtr < PmtCompositeMetadata > PmtCompositeMetadataPtr;
typedef EkConstSmartPtr < PmtCompositeMetadata > ConstPmtCompositeMetadataPtr;

typedef list < PmtMetadataList::iterator > PmtMdIterList;
typedef PmtMdIterList::iterator PmtMdIterListIter;
typedef PmtMdIterList::const_iterator PmtMdIterListConstIter;

#ifdef PMTCORE_EXPLICIT
//PMTCORE_TMPL_EXT template class PMTCORE_DECL EkRefCount<EK_DEFAULT_MUTEX>;
PMTCORE_TMPL_EXT template class PMTCORE_DECL EkConstSmartPtr <
    PmtCompositeMetadata >;
PMTCORE_TMPL_EXT template class PMTCORE_DECL EkSmartPtr <
    PmtCompositeMetadata >;
// Both of these are for the EkFactory class
#endif

/*! The PmtCompositeMetadata class is a container for other metadata

   Since it is derived from the PmtMetadata class, it is-a metadata,
   and has an associated metadata \b key and \b type.  However, there is no
   metadata value associated with this class. PmtCompositeMetadata
   is mapped to the \em \<complexType\> construct of the Metadata Definition
   Language; therefore, its \b type is the value assigned to the \em name
   attribute of the \em \<complexType\>.
*/
class PMTCORE_DECL PmtCompositeMetadata:public PmtMetadata
{
  public:
   
    //! Default Constructor
    inline PmtCompositeMetadata():PmtMetadata(), mModelGroup(NULL)
    {
    }

    //! Constructor. defaultCompositeFactoryKey = "composite"
    inline PmtCompositeMetadata(const PmtKeyType & mdKey,
                                const SPmtString & type,
                                const SPmtString & trueType):PmtMetadata(mdKey,
                                                            type, trueType),
                                                            mModelGroup(NULL)
    {
    }
    
    //! Constructor.
    inline PmtCompositeMetadata(const PmtKeyType & mdKey,
                                const SPmtString & type,
                                PmtMDType mdType):PmtMetadata(mdKey,
                                                            type, mdType),
                                                            mModelGroup(NULL)
    {
    }
    
    //! Copy constructor            
    inline PmtCompositeMetadata(const PmtCompositeMetadata &
                                orig):PmtMetadata(orig), mModelGroup(NULL)
    {
        if (orig.mModelGroup)
            mModelGroup = orig.mModelGroup->clone(this);
    }

    //! Copy constructor
    inline PmtCompositeMetadata(const PmtCompositeMetadata & orig,
                                PmtCompositeMetadata *
                                theParent):PmtMetadata(orig, theParent),
        mModelGroup(NULL)
    {
        if (orig.mModelGroup)
            mModelGroup = orig.mModelGroup->clone(this);
    }

    //! This is the destructor
    inline virtual ~ PmtCompositeMetadata()
    {
    }

    //! Assignment operator
    PmtCompositeMetadata & operator=(const PmtCompositeMetadata & rhs);

    //! Returns true if the metadata is a composite.
    inline virtual bool isComposite() const
    {
        return true;
    }

    //! Returns an reference to a metadata instance whose key matched the specified key.
    /*! 
       \param key is of type PmtKeyType and is the quialified key
       of the metadata instance to be retrieved
       \param createMetadataIfNotExists is of type bool and controls the
       behavior of the creation of non-existant metadata. Default
       is true - create if it does not exist. false will not create
       the instance and a NULL PmtMetadataPtr is returned.
     */
    virtual PmtMetadataPtr getMetadatum(const PmtKeyType & key,
                                        bool createMetadataIfNotExists = true);

    //! Create retrieve metadata instances
    virtual PmtMetadataIterator getMetadata(const PmtKeyType & key,
                                            bool createMetadataIfNotExists =
                                            true,
                                            const PmtEntryTypeEnum
                                            entryToSearch = PMT_ALL_KEYS);

    //! Create retrieve metadata instances
    virtual PmtMetadataIterator getMetadata(const PmtKeyList & keyList,
                                            bool createMetadataIfNotExists =
                                            true,
                                            const PmtEntryTypeEnum
                                            entryToSearch = PMT_ALL_KEYS);

    //! Delete metadata instances.
    virtual void deleteMetadata(const PmtKeyType & key,
                                const PmtEntryTypeEnum entryToSearch =
                                PMT_ALL_KEYS);

    //! Delete metadata instances.
    virtual void deleteMetadata(const PmtKeyList & keyList,
                                const PmtEntryTypeEnum entryToSearch =
                                PMT_ALL_KEYS);

    //! Return a "deep" copy of the instance.
    /*! This is here for the metadata factory to allow it to create
       instances from the prototype metadata instances it contains.
     */
    inline virtual PmtMetadata *clone(void) const
    {
        return new PmtCompositeMetadata(*this);
    }

    //! Return a "deep" copy of the instance.
    /*! This is here for the metadata factory to allow it to create
       instances from the prototype metadata instances it contains.
     */
    inline virtual PmtMetadataPtr clone(PmtCompositeMetadata * theParent) const
    {
        return new PmtCompositeMetadata(*this, theParent);
    }

    //! For VIM visitor concept
    virtual void accept(PmtMetadataVisitor * visitor);
    //! For VIM visitor concept
    virtual void accept(PmtMdVisitorImpl * visitor);

    //! Show metadata object.
    virtual void show(bool showObjectMemoryPointers = false, int level =
                      0, bool showModelGroups = false);

    // Write info specific to this class
    virtual ostream & printDerived(ostream & c) const;

    const PmtMdIterList & getMdIterList(void) const
    {
        return mMdIterList;
    }
    
    const PmtModelGroupPtr & modelGroup(void) const
    {
        return mModelGroup;
    }
    PmtModelGroupPtr & modelGroup(void)
    {
        return mModelGroup;
    }

    void addMetadataKey(const PmtMetadataList::iterator & mdIter)
    {
        mMdIterList.push_back(mdIter);
    }

  private:
    PmtModelGroupPtr mModelGroup;

    // mMdIterList is a list of iterators to the PmtMetadataList in the model group
    // class. mMdIterList can be thought of as (its contained iterators pointing to)
    // a list of keys referring to the metadata objects (via the pair's PmtElement)
    // that this PmtCompositeMetadata contains (regardless as to which PmtModelGroup
    // the metadata object actually resides in). Since occurrences are dealt with
    // in the PmtElement class, the key stored in mMdIterList is the key only.
    // Any occurrence specifications must be dealt with by calling into the
    // PmtElement class.
    PmtMdIterList mMdIterList;

    PmtMetadataPtr getMetadatumOccurrence(const PmtKeyType & curLevel,
                                          bool createMetadataIfNotExists,
                                          int occurNum);

    void getMetadataTopLevelPath(const PmtKeyType & key,
                                 PmtMetadataIterator & mdIter,
                                 bool createMetadataIfNotExists,
                                 const PmtEntryTypeEnum entryToSearch);

    //! This inserts into the iterator the metadata requested in the key path.
    void getMetadataPath(const PmtKeyType & key, PmtMetadataIterator & mdIter,
                         bool createMetadataIfNotExists);

    //! This inserts into the iterator all the metadata contained in this composite.
    void getMetadataAllUnderForOccurrences(PmtMetadataIterator & mdIter,
                                           bool createMetadataIfNotExists,
                                           PmtConstOccurrencePtr occurrence);

    //! This inserts into the iterator the wildcard (single key) metadata requested.
    void getMetadataKeyMultiLevelWildcard(const PmtKeyType & wildcardKey,
                                          PmtMetadataIterator & mdIter,
                                          bool createMetadataIfNotExists,
                                          PmtConstOccurrencePtr occurrence);

    void getMetadataPathOccurrence(const PmtKeyType & curLevel,
                                   const PmtKeyType & remainder,
                                   PmtMetadataIterator & mdIter,
                                   bool createMetadataIfNotExists,
                                   int occurNum);

    void getMetadataPathOccurrenceAndRemainder(const PmtKeyType & curLevel,
                                               const PmtKeyType & remainder,
                                               PmtMetadataIterator & mdIter,
                                               bool createMetadataIfNotExists,
                                               int occurNum);

    void getMetadataKeyAndAllUnderForOccurrences(const PmtKeyType & key,
                                                 PmtMetadataIterator & mdIter,
                                                 bool
                                                 createMetadataIfNotExists,
                                                 PmtConstOccurrencePtr
                                                 occurrence);

    void getMetadataKeyAndAllUnderForOccurrencesRecursive(const PmtKeyType &
                                                          key,
                                                          PmtMetadataIterator &
                                                          mdIter,
                                                          bool
                                                          createMetadataIfNotExists,
                                                          int occurNum,
                                                          PmtConstOccurrencePtr
                                                          occurrence);

    //! This inserts into the iterator the aliased metadata
    void getMetadataAlias(const PmtKeyType & key, PmtMetadataIterator & mdIter,
                          bool createMetadataIfNotExists);

    void deleteMetadataTopLevelPath(const PmtKeyType & key,
                                    const PmtEntryTypeEnum entryToSearch);

    void deleteMetadataPath(const PmtKeyType & key);

    //! This deletes a single piece of metadata based on the key
    inline void deleteMetadatum(const PmtKeyType & key, int occurNum)
    {
        mModelGroup->deleteMetadatum(key, occurNum);
    }

    void deleteMetadataAllForOccurrence(PmtConstOccurrencePtr occurrence);

    //! This deletes all the metadata contained within this composite
    void deleteMetadataAll();

    //! This deletes the metadata associated with an alias.
    void deleteMetadataAlias(const PmtKeyType & key);

    //! This deletes the metadata that matches a wildcard (* or ?) as part of a key segment.
    void deleteMetadataKeyWildcard(const PmtKeyType & wildcardKey,
                                   PmtConstOccurrencePtr occurrence);

    //! This returns the metadata item if it already exists inside this composite
    PmtMetadataPtr findMetadata(const PmtKeyType & key, unsigned int occurNum);

    unsigned int getWildcardMax(const PmtKeyType & key);

    bool isEmpty(void);

    //! Do quick check to see if we have an occurrence specification present with the key
    inline bool isOccurrenceSpecified(const PmtKeyType & key)
    {
        return key.find("[") != PmtKeyType::npos;
    }
};

#endif                          // PMT_COMPOSITE_METADATA_H

