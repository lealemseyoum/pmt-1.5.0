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

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma warning (disable:4503)
#endif

#include "PmtModelGroup.h"
#include "PmtCompositeMetadata.h"


PmtModelGroup::PmtModelGroup(const PmtModelGroup & orig,
                             PmtCompositeMetadata * parentMd)
                             :mParentMd(parentMd)
{
    PmtMdListConstIter crntIter = orig.mMdList.begin();
    PmtMdListConstIter endIter = orig.mMdList.end();
    while (crntIter != endIter)
    {
        mParentMd->addMetadataKey(mMdList.insert(mMdList.end(),
                         PmtMetadataList::value_type((*crntIter).first,
                                      (*crntIter).second->clone(mParentMd))));

        crntIter++;
    }

    PmtModelGroupListConstIter mgIter = orig.mChildren.begin();
    PmtModelGroupListConstIter endMgIter = orig.mChildren.end();
    while (mgIter != endMgIter)
    {
        mChildren.push_back((*mgIter)->clone(mParentMd));
        mgIter++;
    }

    return;
}


void PmtModelGroup::addChildModelGroup(PmtModelGroupPtr childModelGroup)
{
    childModelGroup->parentMd() = mParentMd;
    mChildren.push_back(childModelGroup);
}


void PmtModelGroup::addMetadataKey(const PmtKeyType & key,
                                   const PmtKeyType & fullKey,
                                   const SPmtString & minOccurs,
                                   const SPmtString & maxOccurs,
                                   const SPmtString & nillable)
{
    // Add the key to our list in this model group and to the list in the 
    // composite. As part of the pair added, also add a PmtElement object.
    PmtElementPtr element =
        PmtElement::create(fullKey, minOccurs, maxOccurs, nillable);
    mParentMd->addMetadataKey(mMdList.insert(mMdList.end(),
                                 PmtMetadataList::value_type(key,element)));
}
void PmtModelGroup::addMetadataKey(const PmtKeyType & key,
                                   const PmtKeyType & fullKey,
                                   const unsigned int & minOccurs,
                                   const unsigned int & maxOccurs,
                                   const SPmtString & nillable)
{
    // Add the key to our list in this model group and to the list in the 
    // composite. As part of the pair added, also add a PmtElement object.
    PmtElementPtr element = PmtElement::create(fullKey, minOccurs, 
                                               maxOccurs, nillable);
    mParentMd->addMetadataKey(mMdList.insert(mMdList.end(),
                                     PmtMetadataList::value_type(key,element)));
}


PmtMdListIter PmtModelGroup::checkKey(const PmtKeyType & key)
{
    PmtMdListIter crntIter = mMdList.begin();
    PmtMdListIter endIter = mMdList.end();
    while (crntIter != endIter)
    {
        if ((*crntIter).first == key)
            return crntIter;

        crntIter++;
    }

    return endIter;
}


bool PmtModelGroup::deleteMetadatum(const PmtKeyType & key, int occurNum)
{
    PmtMdListIter mdListIter = checkKey(key);
    // if the key is not found, dispatch this to a
    // child model group
    if (mdListIter == mMdList.end())
    {
        PmtModelGroupListIter crnt = mChildren.begin();
        PmtModelGroupListIter end = mChildren.end();
        while (crnt != end)
        {
            if ((*crnt)->deleteMetadatum(key, occurNum))
            {
                break;
            }

            crnt++;
        }
        // if we did not break out of the loop, the
        // instance to be deleted was not found, so
        // return false.
        if (crnt == end)
            return false;
    }
    else
    {
        (*mdListIter).second->deleteMetadatum(occurNum);
    }

    return true;
}


PmtMetadataPtr PmtModelGroup::getMetadatum(const PmtKeyType & key,
                                           int occurNum)
{
    if (!okToCreate(key))
        return NULL;

    PmtMetadataPtr rtnMd = NULL;

    // check if I am responsible for this key by search
    // through the mdList for the key
    PmtMdListIter mdListIter = checkKey(key);

    // if the key is not found, dispatch this to a
    // child model group
    if (mdListIter == mMdList.end())
    {
        PmtModelGroupListIter crnt = mChildren.begin();
        PmtModelGroupListIter end = mChildren.end();
        while (crnt != end)
        {
            rtnMd = (*crnt)->getMetadatum(key, occurNum);
            if (rtnMd)
                break;
            crnt++;
        }
    }
    else
    {
        rtnMd = (*mdListIter).second->getMetadatum(occurNum, true, mParentMd);
    }

    return rtnMd;
}


int PmtModelGroup::getItemIndex(const PmtKeyType & key)
{
    int index = 0;
    PmtMdListIter curIter = mMdList.begin();
    while (curIter != mMdList.end())
    {
        if ((*curIter).first == key)
            return index;

        index++;
        curIter++;
    }

    // Now look through all the child model groups.
    PmtModelGroupListIter curChildIter = mChildren.begin();
    while (curChildIter != mChildren.end())
    {
        if ((*curChildIter)->getItemIndex(key) != -1)
            return index;

        index++;
        curChildIter++;
    }

    // If we've gotten this far, nothing has been found.
    return -1;
}


bool PmtSequenceModelGroup::okToCreate(const PmtKeyType & key)
{
    // If the key resides directly in this model group, return true.
    if (doesDirectlyReside(key))
        return true;

    int requestedIndex = getItemIndex(key);

    if (requestedIndex == -1)
        return false;           // should not happen

    // Continue into index where requested is, recursively.
    PmtModelGroupPtr mg = getModelGroupFromIndex(requestedIndex);
    if (mg)
        return mg->okToCreate(key);
    else
        return false;
}


bool PmtChoiceModelGroup::okToCreate(const PmtKeyType & key)
{
    int requestedIndex = getItemIndex(key);

    if (requestedIndex == -1)
        return false;           // should not happen

    if (anyCreated(requestedIndex))
        return false;

    // If the key resides directly in this model group, return true.
    if (doesDirectlyReside(key))
        return true;

    // Continue into index where requested is, recursively.
    PmtModelGroupPtr mg = getModelGroupFromIndex(requestedIndex);

    if (mg)
        return mg->okToCreate(key);
    else
        return false;
}


// If any items are created at any level in the model group nestings, skipping over the indexToSkip item.
bool PmtModelGroup::anyCreated(int indexToSkip)
{
    int counter = 0;
    PmtMdListIter curIter = mMdList.begin();
    while (curIter != mMdList.end())
    {
        if ((indexToSkip == -1) || (counter != indexToSkip))
            if ((*curIter).second->getMetadatum(0, false))
                return true;

        counter++;
        curIter++;
    }

    // Now look through all the child model groups.
    PmtModelGroupListIter curChildIter = mChildren.begin();
    while (curChildIter != mChildren.end())
    {
        if ((indexToSkip == -1) || (counter != indexToSkip))
            if ((*curChildIter)->anyCreated(-1))
                return true;

        counter++;
        curChildIter++;
    }

    return false;
}


bool PmtModelGroup::doesDirectlyReside(const PmtKeyType & key)
{
    PmtMetadataPtr md;
    PmtMdListIter mdListIter = checkKey(key);

    if (mdListIter != mMdList.end())
        return true;
    else
        return false;
}


PmtModelGroupPtr PmtModelGroup::getModelGroupFromIndex(int index)
{
    unsigned int counter = 0;
    PmtModelGroupListIter curChildIter = mChildren.begin();
    while (curChildIter != mChildren.end())
    {
        if (counter == (index - mMdList.size()))
            return (*curChildIter);

        counter++;
        curChildIter++;
    }

    return NULL;
}


void PmtModelGroup::show(bool showObjectMemoryPointers, int level,
                         bool showModelGroups)
{
    SPmtString mdKey;
    SPmtString mdType;

    mdKey = mParentMd->fullKey();
    mdType = mParentMd->type();

    int i;
    for (i = 0; i < level; i++)
        cout << "  ";
    cout << "-PmtModelGroup - " << type().c_str() << endl;

    PmtMdListIter curIter = mMdList.begin();
    PmtMdListIter end = mMdList.end();
    while (curIter != end)
    {
        for (i = 0; i < level; i++)
            cout << "  ";
        cout << "  " << (*curIter).first.c_str();

        (*curIter).second->show(showObjectMemoryPointers, level + 1);

        curIter++;
    }

    PmtModelGroupListIter mgCurIter = mChildren.begin();
    PmtModelGroupListIter mgEnd = mChildren.end();
    while (mgCurIter != mgEnd)
    {
        (*mgCurIter)->show(showObjectMemoryPointers, level + 1,
                           showModelGroups);
        mgCurIter++;
    }
}

