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
 * George Sotak   george.sotak@kodak.com
 * 
 * Contributor(s):
 * Dan Rupe  rupe@kodak.com
*/

#ifdef _MSC_VER
#pragma warning (disable:4786)
#pragma warning (disable:4503)
#endif

#include "PmtCoreCommon.h"
#include "PmtCompositeMetadata.h"
#include "PmtOccurrence.h"
#include "PmtKeyParser.h"


PmtCompositeMetadata & PmtCompositeMetadata::
operator=(const PmtCompositeMetadata & rhs)
{
    // Test for assignment to self
    if (this != &rhs)
    {
        // Assign base class members
        PmtMetadata::operator=(rhs);

        mModelGroup = rhs.mModelGroup->clone(this);

        // set lhs composite's parent to NULL
        mParent = NULL;
    }
    return *this;
}


ostream & PmtCompositeMetadata::printDerived(ostream & c) const
{
    PmtMdIterListConstIter crnt = mMdIterList.begin();
    while (crnt != mMdIterList.end())
    {
        (*crnt)->second->printDerived(c);
        crnt++;
    }

    return c;
}


void PmtCompositeMetadata::accept(PmtMetadataVisitor * visitor)
{
    PmtMdIterListIter crnt = mMdIterList.begin();
    while (crnt != mMdIterList.end())
    {
        (*crnt)->second->accept(visitor);
        crnt++;
    }

    return;
}


void PmtCompositeMetadata::accept(PmtMdVisitorImpl * visitor)
{
    PmtMdIterListIter crnt = mMdIterList.begin();
    while (crnt != mMdIterList.end())
    {
        (*crnt)->second->accept(visitor);
        crnt++;
    }

    return;
}


PmtMetadataPtr PmtCompositeMetadata::getMetadatum(const PmtKeyType & key,
                                                bool createMetadataIfNotExists)
{
    PmtKeyParser keyParser(key);
    PmtKeyType parsedKey;
    PmtMetadataPtr md;
    if (keyParser.parseKey(parsedKey))
    {
        PmtKeyType curLevel, remainder;
        PmtKeyType::size_type pos;
        bool foundError = false;

        // If more than 1 level
        pos = parsedKey.find('.');
        if (pos == PmtKeyType::npos)
        {
            curLevel = parsedKey;
        }
        else
        {
            // else - multi - level
            curLevel = parsedKey.substr(0, pos);
            remainder = parsedKey.substr(pos + 1);
        }

        // Deal with occurrences for this level of the key path
        unsigned int occurNum;
        PmtOccurrencePtr occurrence = new PmtOccurrence();
        if (isOccurrenceSpecified(curLevel))
        {
            // strips occurrence from curLevel
            if (!occurrence->parseKeyForOccurrences(curLevel))  
                foundError = true;
        }

        // no wildcards or multiple occurrences in getMetadatum()
        if ( (!foundError) && ( (occurrence->isUnspecifiedOccurrence())
            || (occurrence->isExplicitlySpecifiedSingleOccurrence()) ) )
        {            
            occurNum = occurrence->start();

            md = getMetadatumOccurrence(curLevel, createMetadataIfNotExists, 
                                        occurNum);
            // If any more levels
            if (!remainder.empty())
            {
                // Since there are more levels, we must have just created / 
                // retrieved a composite above. Call recursively, passing to 
                // the next level of recursion the md we just created.
                if (md)
                    md = md->getMetadatum(remainder, createMetadataIfNotExists);
            }
        }
    }
    return md;
}


PmtMetadataPtr PmtCompositeMetadata::
getMetadatumOccurrence(const PmtKeyType & curLevel,
                       bool createMetadataIfNotExists, int occurNum)
{
    PmtMetadataPtr md = findMetadata(curLevel, occurNum);

    // If the metadata item has not been previously instantiated, then we need
    // to ask the model group(s) to do the instantiation for us.
    if (!md)
    {
        // If we are allowed to create metadata, then go ahead and do so.
        // If we are not, then we will end up returning NULL below.
        if (createMetadataIfNotExists)
            // instantiates metadata
            md = mModelGroup->getMetadatum(curLevel, occurNum); 
    }

    return md;
}


/*! 
  \param key is of type PmtKeyType and is the qualified key
  of the metadata instance, a wildcard qualified metadata key or
  an alias key.
  \param createMetadataIfNotExists is of type bool and controls the
  behavior of the creation of non-existant metadata. Default
  is true - create if it does not exist. false will not create
  \param  entryToSearch is of type PmtEntryTypeEnum. Allows one to 
  limit the key search to just the metadata keys (\b PMT_METADATA_KEYS_ONLY) 
  or alias keys (\b PMT_ALIAS_KEYS_ONLY). The default is to seach all 
  keys (\b PMT_ALL_KEY).
  \return A PmtMetadataIterator instance containing references to metadata
  instances.

This method will create, if necessary and if createMetadataIfNotExists
is set ture (the default), and return references to multiple metadata
instances through the PmtMetadataIterator instance that is actually
returned. The \b key maybe a qualified metadata key, a wildcard
metadata key, or an alias key. Metadata is created and cached for
quick retrieval and value retention when subsequent getMetadata calls
are made.

If a <b> simple key </b> is supplied, such as \em CaptureConditions, then
the CaptureConditions metadata item will be returned.
        
If a <b> qualified key </b>, such as \em CaptureConditions.Aperture is
specfied, then the \em Aperture metadata instance within CaptureConditions
will be returned. If needed, \em CaptureConditions itself will be created,
although not returned.
        
If \b "*" is specified, then all metadata contained within this composite
are created, if necessary, and returned.
        
If <b> "" </b>, an empty key, is specified, then all pre-existing metadata
instances within this composite will be returned. No new items will be
instantiated regardless as to the value of createMetadataIfNotExists.
        
If a <b> wildcard key </b> such as \em CaptureConditions.* is
specified, then the \em CaptureConditions composite will be
instantiated (if it already hasn't been), and all the items within \em
CaptureConditions will be instantiated, if need be, and returned. The
PmtCompositeMetadata instance for \em CaptureConditions will not be
returned.

If a <b> wildcard key </b> such as \em Capture* is specified, then
all the metadata items that match the wildcard (from the default
schema this includes \em CaptureConditions and \em CaptureDevice)
would be created, if necessary, and any metadata contained within them
will be recursively created and returned.  Also, if any aliases match
the wildcard (assuming an alias file is loaded), and entryToSearch
does not equal PMT_METADATA_KEYS_ONLY, then any aliases beginning with
\em Capture will also be included.

Metadata instances are not actually returned by this method, rather an
instance of PmtMetadataIterator is returned. This iterator contains
pointers (i.e., PmtMetadataPtr) to the metadata instances that
matched the supplied \b key. Pointers to PmtCompositeMetadata instances
are not included in the returned iterator.  Only the leaf metadata
instances (i.e., those of type PmtMetadataT<>) are returned in the
iterator.
        
If \b createMetadataIfNotExists is \em false, then the returned metadata
items as described above may differ. The items returned will only be
returned if they already exist. This means, for example, that if
\em CaptureConditions.Aperture" is requested, and \em Aperture does not exist
already, nothing will be returned in the PmtMetadataIterator. The same
principle applies to all calls made to this method when
\b createMetadataIfNotExists is \em false.
        
The \b entryToSearch parameter specifies if metadata keys, aliases, or
both are searched for the creation / retrieval of metadata
instances. \b PMT_ALL_KEYS, the default, means that if an alias file is
loaded, then both regular metadata keys and aliases (including
wildcards) will be searched.  \b PMT_METADATA_KEYS_ONLY means that only
metadata keys will be searched, whereas \b PMT_ALIAS_KEYS_ONLY means that only
aliases will be searched.

        
Here is example code (assumes the default schema is loaded):
\code
PmtMetadataPtr root; root = PmtMetadata::create();
PmtMetadataIterator iter = root->getMetadata("CaptureConditions.*");
\endcode
*/
PmtMetadataIterator PmtCompositeMetadata::getMetadata(const PmtKeyType & key,
                                          bool createMetadataIfNotExists,
                                          const PmtEntryTypeEnum entryToSearch)
{
    PmtMetadataIterator mdIter;

    if (key.empty())              // Quick check for blank key.
        getMetadataTopLevelPath("*[*]", mdIter, false, PMT_METADATA_KEYS_ONLY);
    else
        getMetadataTopLevelPath(key, mdIter, createMetadataIfNotExists,
                                entryToSearch);

    return mdIter;
}


void PmtCompositeMetadata::getMetadataTopLevelPath(const PmtKeyType & key,
                                          PmtMetadataIterator & mdIter,
                                          bool createMetadataIfNotExists,
                                          const PmtEntryTypeEnum entryToSearch)
{
    // The keyparser only does a simple validation of the key and 
    // removes ALL spaces (is this really what we want?)
    PmtKeyParser keyParser(key);
    PmtKeyType parsedKey;
    if (keyParser.parseKey(parsedKey))
    {
        if ((entryToSearch == PMT_ALL_KEYS)
            || (entryToSearch == PMT_METADATA_KEYS_ONLY))
            getMetadataPath(parsedKey, mdIter, createMetadataIfNotExists);

        if ((entryToSearch == PMT_ALL_KEYS)
            || (entryToSearch == PMT_ALIAS_KEYS_ONLY))
            getMetadataAlias(parsedKey, mdIter, createMetadataIfNotExists);
    }
}


void PmtCompositeMetadata::getMetadataPath(const PmtKeyType & key,
                                           PmtMetadataIterator & mdIter,
                                           bool createMetadataIfNotExists)
{
    if (!key.empty())
    {
      PmtKeyType curLevel;
      PmtKeyType remainder;
      PmtKeyType::size_type pos;
      bool foundError = false;
      
      // If more than 1 level
      pos = key.find('.');
      if (pos == PmtKeyType::npos)
      {
          curLevel = key;
      }
      else
      {
          // else - multi - level
          curLevel = key.substr(0, pos);
          remainder = key.substr(pos + 1);
      }

      PmtKeyType curLevelNoOccurrence = curLevel;
      PmtOccurrencePtr occurrence = new PmtOccurrence();
      if (isOccurrenceSpecified(curLevelNoOccurrence))
      {
          // strips occurrence from key
          if (!occurrence->parseKeyForOccurrences(curLevelNoOccurrence))
              foundError = true;
      }

      if (!foundError)
      {
        if (curLevelNoOccurrence == PMT_MULTI_LEVEL_WILDCARD_CHAR)
        {
            getMetadataAllUnderForOccurrences(mdIter, createMetadataIfNotExists,
                                              occurrence);
        }
        else if (curLevelNoOccurrence[curLevelNoOccurrence.length() - 1] == 
                                PMT_MULTI_LEVEL_WILDCARD_CHAR)  
        {
            // if * at the end
            getMetadataKeyMultiLevelWildcard(curLevelNoOccurrence, mdIter,
                                             createMetadataIfNotExists,
                                             occurrence);
        }
        else
        {
            if (occurrence->isWildcard())
            {
                unsigned int occurNum = 0;
                while (occurNum < getWildcardMax(curLevelNoOccurrence))
                {
                    getMetadataPathOccurrence(curLevelNoOccurrence, remainder,
                                              mdIter, createMetadataIfNotExists,
                                              occurNum);
                    occurNum++;
                }
            }
            else
            {
                int occurNum = occurrence->start();
                while (occurNum != PmtOccurrence::OCCURRENCE_END)
                {
                    getMetadataPathOccurrence(curLevelNoOccurrence, remainder,
                                              mdIter, createMetadataIfNotExists,
                                              occurNum);
                    occurNum = occurrence->next();
                }
            }
        }
      }
    }
}


// Creates or gets all metadata items within this composite, if the 
// occurrence(s) asked for are in the allowable range specified in the schema.
// This means that some of the items within the composite may be returned, 
// whereas others may not.
void PmtCompositeMetadata::
getMetadataAllUnderForOccurrences(PmtMetadataIterator & mdIter,
                                  bool createMetadataIfNotExists,
                                  PmtConstOccurrencePtr occurrence)
{
    PmtMdIterListIter curIter = mMdIterList.begin();
    while (curIter != mMdIterList.end())
    {
        getMetadataKeyAndAllUnderForOccurrences((*curIter)->first, mdIter,
                                                createMetadataIfNotExists,
                                                occurrence);

        curIter++;
    }
}


void PmtCompositeMetadata::
getMetadataKeyMultiLevelWildcard(const PmtKeyType & curLevel,
                                 PmtMetadataIterator & mdIter,
                                 bool createMetadataIfNotExists,
                                 PmtConstOccurrencePtr occurrence)
{
    // We have a single key that's wilcarded - no path.

    // Remove * from end.
    PmtKeyType myKey(curLevel, 0, curLevel.length() - 1);

    PmtMdIterListIter curIter = mMdIterList.begin();
    while (curIter != mMdIterList.end())
    {
        // If the wildcard portion is found at the beginning of the metadata
        // key, then get its children (and children's children, etc.) and put 
        // them inside the iterator.
        if ((*curIter)->first.find(myKey) == 0)
        {
            getMetadataKeyAndAllUnderForOccurrences((*curIter)->first, mdIter,
                                                    createMetadataIfNotExists,
                                                    occurrence);
        }

        curIter++;
    }
}


void PmtCompositeMetadata::
getMetadataPathOccurrence(const PmtKeyType & curLevel,
                          const PmtKeyType & remainder,
                          PmtMetadataIterator & mdIter,
                          bool createMetadataIfNotExists, 
                          int occurNum)
{
    // If the curLevel is a '?'.
    if (curLevel == PMT_SINGLE_LEVEL_WILDCARD_CHAR)
    {
        PmtMdIterListIter curIter = mMdIterList.begin();
        while (curIter != mMdIterList.end())
        {
            getMetadataPathOccurrenceAndRemainder((*curIter)->first, remainder,
                                                  mdIter,
                                                  createMetadataIfNotExists,
                                                  occurNum);

            curIter++;
        }
    }
    else if (curLevel[curLevel.length() - 1] == PMT_SINGLE_LEVEL_WILDCARD_CHAR) // if '?' at end
    {
        // Remove ? from end.
        PmtKeyType myKey(curLevel, 0, curLevel.length() - 1);

        PmtMdIterListIter curIter = mMdIterList.begin();
        while (curIter != mMdIterList.end())
        {
            // If the wildcard portion is found at the beginning of the 
            // metadata key, then get its children and put them inside the 
            // iterator.
            if ((*curIter)->first.find(myKey) == 0)
                getMetadataPathOccurrenceAndRemainder((*curIter)->first,
                                                      remainder, mdIter,
                                                      createMetadataIfNotExists,
                                                      occurNum);

            curIter++;
        }
    }
    else
    {
        getMetadataPathOccurrenceAndRemainder(curLevel, remainder, mdIter,
                                              createMetadataIfNotExists,
                                              occurNum);
    }
}


void PmtCompositeMetadata::
getMetadataPathOccurrenceAndRemainder(const PmtKeyType & curLevel,
                                      const PmtKeyType & remainder,
                                      PmtMetadataIterator & mdIter,
                                      bool createMetadataIfNotExists,
                                      int occurNum)
{
    PmtMetadataPtr md;

    md = getMetadatumOccurrence(curLevel, createMetadataIfNotExists, occurNum);
    if (md)
    {
        if (md->isComposite())
        {
            if (!remainder.empty())
                PMT_CAST_COMPOSITE(md)->getMetadataPath(remainder, mdIter,
                                                    createMetadataIfNotExists);
        }
        else
            mdIter.insert(md);
    }
}


void PmtCompositeMetadata::
getMetadataKeyAndAllUnderForOccurrences(const PmtKeyType & key,
                                        PmtMetadataIterator & mdIter,
                                        bool createMetadataIfNotExists,
                                        PmtConstOccurrencePtr occurrence)
{
    if (occurrence->isWildcard())
    {
        unsigned int occurNum = 0;
        while (occurNum < getWildcardMax(key))
        {
            getMetadataKeyAndAllUnderForOccurrencesRecursive(key, mdIter,
                                                     createMetadataIfNotExists,
                                                     occurNum,
                                                     occurrence);
            occurNum++;
        }
    }
    else
    {
        int occurNum = occurrence->start();
        while (occurNum != PmtOccurrence::OCCURRENCE_END)
        {
            getMetadataKeyAndAllUnderForOccurrencesRecursive(key, mdIter,
                                                     createMetadataIfNotExists,
                                                     occurNum,
                                                     occurrence);
            occurNum = occurrence->next();
        }
    }
}


void PmtCompositeMetadata::
getMetadataKeyAndAllUnderForOccurrencesRecursive(const PmtKeyType & key,
                                               PmtMetadataIterator & mdIter,
                                               bool createMetadataIfNotExists,
                                               int occurNum,
                                               PmtConstOccurrencePtr occurrence)
{
    PmtMetadataPtr md;

    md = getMetadatumOccurrence(key, createMetadataIfNotExists, occurNum);
    if (md)
    {
        if (md->isComposite())
        {
            PMT_CAST_COMPOSITE(md)->getMetadataAllUnderForOccurrences(mdIter,
                                                      createMetadataIfNotExists,
                                                      occurrence);
        }
        else
            mdIter.insert(md);
    }
}


void PmtCompositeMetadata::getMetadataAlias(const PmtKeyType & key,
                                            PmtMetadataIterator & mdIter,
                                            bool createMetadataIfNotExists)
{
    PmtKeyList keyList;
    getKeyList(key, keyList);

    if (!keyList.empty())
    {
        PmtKeyList::iterator keyIter = keyList.begin();
        while (keyIter != keyList.end())
        {
            // Do not allow aliases members to refer to other alias names.
            // Pass PMT_METADATA_KEYS_ONLY in to ensure this.
            getMetadataTopLevelPath((*keyIter), mdIter,
                                    createMetadataIfNotExists,
                                    PMT_METADATA_KEYS_ONLY);
            keyIter++;
        }
    }
}


/*! 
  \param keyList is of type PmtKeylist and is a list of qualified key
  of the metadata instance, a wildcard qualified metadata key, 
  an alias key, or a mixture of any of these.
  \param createMetadataIfNotExists is of type bool and controls the
  behavior of the creation of non-existant metadata. Default
  is true - create if it does not exist. false will not create
  \param  entryToSearch is of type PmtEntryTypeEnum. Allows one to 
  limit the key search to just the metadata keys (\b PMT_METADATA_KEYS_ONLY) 
  or alias keys (\b PMT_ALIAS_KEYS_ONLY). The default is to seach all 
  keys (\b PMT_ALL_KEY).
  \return A PmtMetadataIterator instance containing references to metadata
  instances.

  The behavior of this method is identical to 
  PmtCompositeMetadata::getMetadata(const PmtKeyType& key,
    bool createMetadataIfNotExists, const PmtEntryTypeEnum entryToSearch)
*/
PmtMetadataIterator PmtCompositeMetadata::
getMetadata(const PmtKeyList & keyList, bool createMetadataIfNotExists,
            const PmtEntryTypeEnum entryToSearch)
{
    PmtMetadataIterator mdIter;

    PmtKeyList::const_iterator curIter = keyList.begin();
    while (curIter != keyList.end())
    {
        getMetadataTopLevelPath((*curIter), mdIter, createMetadataIfNotExists,
                                entryToSearch);
        curIter++;
    }

    return mdIter;
}


/*! 
  \param key is of type PmtKeyType and is the qualified key
  of the metadata instance, a wildcard qualified metadata key or
  an alias key.
  \param  entryToSearch is of type PmtEntryTypeEnum. Allows one to 
  limit the key search to just the metadata keys (\b PMT_METADATA_KEYS_ONLY) 
  or alias keys (\b PMT_ALIAS_KEYS_ONLY). The default is to seach all 
  keys (\b PMT_ALL_KEY).

For the examples below, assume that the default schema is what's loaded,
and that the root composite is what's getting called through, like this:

\code
PmtMetadataPtr root;
root = PmtMetadata::create();
// getMetadata() requests not shown
root->deleteMetadata("CaptureConditions");
\endcode

If a <b> simple key </b> is supplied, such as \em CaptureConditions,
then the metadata instance will be deleted. If the specified metadata
is an instance of PmtCompositeMetadata, then all its children will be
recursively deleted.

If a <b> quilified key </b> is supplied, such as \em
CaptureConditions.Aperture, then the metadata instance with the key
matching \em Aperture within \em CaptureConditions will be deleted.

If a <b> wildcard key </b> of the form \em CaptureConditions.* is
supplied, then all of the metadata items contained within \em
CaptureConditions will be deleted.

If a <b> wildcard key </b> of the form \em Capture* is supplied, then
all the metadata items that match the wildcard (from the default
schema this includes CaptureConditions and CaptureDevice) will be
deleted. Also, if any aliases match the wildcard (assuming an alias
file is loaded), and entryToSearch does not equal
PMT_METADATA_KEYS_ONLY, then any aliases beginning with \em Capture
would also be deleted.

If <b> "*" </b> is supplied, then all the metadata instances contained
within the composite are deleted.

If <b> "" </b>, an empty key, is supplied, this method will do nothing.

This method will perform automatic deletions of PmtCompositeMetadata
instances when all of their children have been deleted.  This happens
only when the children have been deleted from their parent
PmtCompositeMetadata (or higher parent). For example, if the key
\em CaptureDevice.Scanner.ModelAndVersion is deleted like this:

\code
PmtMetadataPtr captureDevice = root->getMetadatum("CaptureDevice");
captureDevice->deleteMetadata("Scanner.ModelAndVersion");
\endcode

then the \em Scanner metadata instance will get deleted too, since
it's only child has been deleted. This applies to the situation in
which the delete is issued to the \em root metadata instance:

\code
root->deleteMetadata("CaptureDevice.Scanner.ModelAndVersion");
\endcode

additionally, in this case the PmtCompositeMetadata instance for
\em CaptureDevice is also deleted.

If however, \em ModelAndVersion were deleted like this: 

\code
PmtMetadataPtr scanner = root->getMetadatum("CaptureDevice.Scanner");
scanner->deleteMetadata("ModelAndVersion"); 
\endcode

then \em ModelAndVersion would get deleted. However, because the
application has a handle to the \em Scanner instance, it is not
deleted.

The \b entryToSearch parameter specifies if regular metadata keys,
aliases, or both are searched to determine which metadata instances
are to be deleted. \b PMT_ALL_KEYS, the default, means that if an alias
file is loaded, then both regular metadata keys and aliases (including
wildcards) will be deleted.  \b PMT_METADATA_KEYS_ONLY means that only
regular keys will match, whereas \b PMT_ALIAS_KEYS_ONLY means that only
aliases will match. The key may be either and alias or a metadata key.

*/
void PmtCompositeMetadata::deleteMetadata(const PmtKeyType & key,
                                          const PmtEntryTypeEnum entryToSearch)
{
    if ((key == "*[*]") || (key == ""))
    {
        if ((entryToSearch == PMT_ALL_KEYS)
            || (entryToSearch == PMT_METADATA_KEYS_ONLY))
            deleteMetadataAll();    // Quick delete of everything.
    }
    else
    {
        deleteMetadataTopLevelPath(key, entryToSearch);
    }
}


void PmtCompositeMetadata::deleteMetadataTopLevelPath(const PmtKeyType & key,
                                          const PmtEntryTypeEnum entryToSearch)
{
    // We deal with aliases here, since aliases can only occur in
    // top level keys.
    // Aliases may not have paths  - no periods.
    // but if it does, it won't be valid -- and won't be deleted.

        // Could be non-alias or alias
        if ((entryToSearch == PMT_ALL_KEYS)
            || (entryToSearch == PMT_METADATA_KEYS_ONLY))
            deleteMetadataPath(key);

        if ((entryToSearch == PMT_ALL_KEYS)
            || (entryToSearch == PMT_ALIAS_KEYS_ONLY))
            deleteMetadataAlias(key);
}


void PmtCompositeMetadata::deleteMetadataPath(const PmtKeyType & key)
{
    PmtKeyType toProcessLevels = key;
    PmtKeyType::size_type pos;
    PmtKeyType curLevel;

    // If more than 1 level
    pos = toProcessLevels.find('.');
    if (pos != PmtKeyType::npos)
    {
        curLevel = toProcessLevels.substr(0, pos);
        toProcessLevels = toProcessLevels.substr(pos + 1);
    }
    // else - only 1 level
    else
    {
        curLevel = toProcessLevels;
        toProcessLevels = "";
    }

    PmtKeyType curLevelNoOccurrence = curLevel;
    PmtOccurrencePtr occurrence = new PmtOccurrence();
    if (isOccurrenceSpecified(curLevelNoOccurrence))
    {
        // strips occurrence from key
        if (!occurrence->parseKeyForOccurrences(curLevelNoOccurrence))
            return;
    }

    // Something about deletions that is different than creating/getting 
    // metadata items, is that as soon as we encounter a wildcard, we know we
    // can just blow away everything else underneath, without having to
    // traverse the rest of the path.
    if (curLevelNoOccurrence == PMT_MULTI_LEVEL_WILDCARD_CHAR)
    {
        deleteMetadataAllForOccurrence(occurrence);
    }
    else if (curLevelNoOccurrence == "")
    {
        deleteMetadataAllForOccurrence(occurrence);
    }
    else if (curLevelNoOccurrence[curLevelNoOccurrence.length() - 1] == 
                                 PMT_MULTI_LEVEL_WILDCARD_CHAR)  // if * at end
    {
        deleteMetadataKeyWildcard(curLevelNoOccurrence, occurrence);
    }
    else if (curLevelNoOccurrence == PMT_SINGLE_LEVEL_WILDCARD_CHAR)
    {
        deleteMetadataAllForOccurrence(occurrence);
    }
    else if (curLevelNoOccurrence[curLevelNoOccurrence.length() - 1] == 
                                 PMT_SINGLE_LEVEL_WILDCARD_CHAR) // if ? at end
    {
        deleteMetadataKeyWildcard(curLevelNoOccurrence, occurrence);
    }
    else
    {
        int occurNum = occurrence->start();
        while (occurNum != PmtOccurrence::OCCURRENCE_END)
        {
            PmtMetadataPtr crntMd =
                findMetadata(curLevelNoOccurrence, occurNum);

            // If more than 1 level
            // We cannot compare toProcessLevels == curLevel to determine if 
            // there's only 1 level left, since we allow metadata keys to be
            // the same at different levels.
            if (pos != PmtKeyType::npos)
            {
                if (crntMd && crntMd->isComposite())
                    PMT_CAST_COMPOSITE(crntMd)->
                                           deleteMetadataPath(toProcessLevels);
                else
                    // Should be an error - if the key indicates further levels
                    // to process, but the metadata with the key matching the
                    // current level is not a composite, then the key is
                    // incorrect.
                    return;
            }
            else
            {
                if (crntMd)
                    deleteMetadatum(curLevelNoOccurrence, occurNum);
            }

            occurNum = occurrence->next();
        }
    }
}


void PmtCompositeMetadata::
deleteMetadataAllForOccurrence(PmtConstOccurrencePtr occurrence)
{
    PmtMdIterListIter curIter = mMdIterList.begin();
    while (curIter != mMdIterList.end())
    {
        int occurNum = occurrence->start();
        while (occurNum != PmtOccurrence::OCCURRENCE_END)
        {
            deleteMetadatum((*curIter)->first, occurNum);
            occurNum = occurrence->next();
        }

        curIter++;
    }
}


void PmtCompositeMetadata::deleteMetadataAll()
{
    PmtMdIterListIter crnt = mMdIterList.begin();
    while (crnt != mMdIterList.end())
    {
        (*crnt)->second->deleteMetadataAll();
        crnt++;
    }
}


void PmtCompositeMetadata::deleteMetadataAlias(const PmtKeyType & key)
{
    PmtKeyList keyList;
    getKeyList(key, keyList);

    if (!keyList.empty())
    {
        PmtKeyList::iterator keyIter = keyList.begin();
        while (keyIter != keyList.end())
        {
            // Do not allow aliases members to refer to other alias names.
            // Pass PMT_METADATA_KEYS_ONLY in to ensure this.
            deleteMetadataTopLevelPath((*keyIter), PMT_METADATA_KEYS_ONLY);
            keyIter++;
        }
    }
}


// Handles * and ? wildcards as part of a key segment.
// Does not handle lone * or ? as part of a key segment.
void PmtCompositeMetadata::
deleteMetadataKeyWildcard(const PmtKeyType & wildcardKey,
                          PmtConstOccurrencePtr occurrence)
{
    // We have a single key that's wilcarded - no path.

    // Remove * from end.
    PmtKeyType myKey(wildcardKey, 0, wildcardKey.length() - 1);

    PmtMdIterListIter curIter = mMdIterList.begin();
    while (curIter != mMdIterList.end())
    {
        // If the wildcard portion is found at the beginning of the key,
        // then it matches, so delete it.
        if ((*curIter)->first.find(myKey) == 0)
        {
            int occurNum = occurrence->start();
            while (occurNum != PmtOccurrence::OCCURRENCE_END)
            {
                deleteMetadatum((*curIter)->first, occurNum);
                occurNum = occurrence->next();
            }
        }

        curIter++;
    }
}


/*! 
  \param keyList is of type PmtKeylist and is a list of qualified key
  of the metadata instance, a wildcard qualified metadata key, 
  an alias key, or a mixture of any of these.
  \param  entryToSearch is of type PmtEntryTypeEnum. Allows one to 
  limit the key search to just the metadata keys (\b PMT_METADATA_KEYS_ONLY) 
  or alias keys (\b PMT_ALIAS_KEYS_ONLY). The default is to seach all 
  keys (\b PMT_ALL_KEY).

  The behavior of this method is identical to 
  PmtCompositeMetadata::deleteMetadata(const PmtKeyType& key,
    const PmtEntryTypeEnum entryToSearch)
*/
void PmtCompositeMetadata::deleteMetadata(const PmtKeyList & keyList,
                                          const PmtEntryTypeEnum entryToSearch)
{
    PmtKeyList::const_iterator curIter = keyList.begin();

    while (curIter != keyList.end())
    {
        deleteMetadataTopLevelPath((*curIter), entryToSearch);

        curIter++;
    }
}


PmtMetadataPtr PmtCompositeMetadata::findMetadata(const PmtKeyType & key,
                                                  unsigned int occurNum)
{
    PmtMetadataPtr md;
    PmtMdIterListIter crnt = mMdIterList.begin();
    while ((crnt != mMdIterList.end()) && (!md))
    {
        if ((*(*crnt)).first == key)
            md = (*crnt)->second->getMetadatum(occurNum, false);

        crnt++;
    }

    return md;
}


unsigned int PmtCompositeMetadata::getWildcardMax(const PmtKeyType & key)
{
    PmtMdIterListIter crnt = mMdIterList.begin();
    unsigned int count = 0;
    while ((count == 0) && (crnt != mMdIterList.end()))
    {
        if ((*(*crnt)).first == key)
            count = (*crnt)->second->getWildcardMax();

        crnt++;
    }

    return count;
}


bool PmtCompositeMetadata::isEmpty(void)
{
    PmtMdIterListIter crnt = mMdIterList.begin();
    while (crnt != mMdIterList.end())
    {
        if (!(*crnt)->second->isEmpty())
            return false;

        crnt++;
    }

    return true;
}


void PmtCompositeMetadata::show(bool showObjectMemoryPointers, int level,
                                bool showModelGroups)
{
    SPmtString mdKey;
    SPmtString mdType;
    int i;

    mdKey = fullKey();
    mdType = type();

    for (i = 0; i < level; i++)
        cout << "   ";
    cout << "-PmtMetadataComposite  ";
    cout << mdKey << " " << mdType << endl;

    showAttr(level + 1);

    PmtMdIterListIter curIter = mMdIterList.begin();
    PmtMdIterListIter end = mMdIterList.end();

    while (curIter != end)
    {
        for (i = 0; i < level; i++)
            cout << "  ";
        cout << "  " << (*curIter)->first.c_str();

        (*curIter)->second->show(showObjectMemoryPointers, level + 1);

        curIter++;
    }

    if (showModelGroups)
    {
        if (mModelGroup)
        {
            mModelGroup->show(showObjectMemoryPointers, level + 1,
                              showModelGroups);
        }
        else
            cout << " NULL mModelGroup" << endl;
    }
}

