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
 * 
 * Contributor(s):
 * Dan Rupe  rupe@kodak.com
 *
*/

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma warning (disable:4786)
#pragma warning (disable:4503)
#endif

#include <climits>

#include "PmtElement.h"
#include "PmtOccurrence.h"
#include "PmtMdVisitorImpl.h"
#include "PmtCompositeMetadata.h"

const unsigned int PMT_OCCURS_UNBOUNDED = INT_MAX;  // Set to largest positive number.


PmtElementPtr PmtElement::create(const PmtKeyType & fullKeyPath,
                                 const SPmtString & minOccursAttr,
                                 const SPmtString & maxOccursAttr,
                                 const SPmtString & nillable)
{
    const unsigned int PMT_OCCURS_MIN_DEFAULT = 1;
    const unsigned int PMT_OCCURS_MAX_DEFAULT = 1;
    const char *const PMT_OCCURS_UNBOUNDED_STRING = "unbounded";

    PmtElementPtr rtn = NULL;

    unsigned int minOccurs;
    unsigned int maxOccurs;

    if ((minOccursAttr.empty())
        || (minOccursAttr == PMT_OCCURS_UNBOUNDED_STRING))
        minOccurs = PMT_OCCURS_MIN_DEFAULT; // XML Spy treats "unbounded" as 1 for the minOccurs.
    else
        minOccurs = atoi(minOccursAttr.c_str());

    if (maxOccursAttr.empty())
        maxOccurs = PMT_OCCURS_MAX_DEFAULT;
    else if (maxOccursAttr == PMT_OCCURS_UNBOUNDED_STRING)
        maxOccurs = PMT_OCCURS_UNBOUNDED;
    else
        maxOccurs = atoi(maxOccursAttr.c_str());


    if ((minOccurs == 0 || minOccurs == 1) && maxOccurs == 1)
        rtn =
            new PmtOptionalOrSingularElement(fullKeyPath, minOccurs, maxOccurs,
                                             nillable);
    else
        rtn =
            new PmtVectorElement(fullKeyPath, minOccurs, maxOccurs, nillable);

    return rtn;
}
PmtElementPtr PmtElement::create(const PmtKeyType & fullKeyPath,
                                 const unsigned int & minOccurs,
                                 const unsigned int & maxOccurs,
                                 const SPmtString & nillable)
{
    PmtElementPtr rtn = NULL;

    if ((minOccurs == 0 || minOccurs == 1) && maxOccurs == 1)
        rtn =
            new PmtOptionalOrSingularElement(fullKeyPath, minOccurs, maxOccurs,
                                             nillable);
    else
        rtn =
            new PmtVectorElement(fullKeyPath, minOccurs, maxOccurs, nillable);

    return rtn;
}


PmtKeyType PmtElement::allButRootPath()
{
//!!! - just for now, do not take 1st path portion out, since we don't
// have the root included in the path.
    return mFullKeyPath;
#if 0
    PmtKeyType::size_type pos;

    pos = mFullKeyPath.find(".");
    if (pos == PmtKeyType::npos)
        return mFullKeyPath;
    else
    {
        return mFullKeyPath.substr(pos + 1);
    }
#endif
}


PmtMetadataPtr PmtElement::
createFromFactoryAndSetParentAndOccurrence(const PmtCompositeMetadata * parent,
                                           int occurNum)
{
    PmtMetadataFactory *fact = parent->getMdFactory();
    PmtMetadataPtr md = fact->create(allButRootPath());
    if (md)
    {
        md->setParent(parent);
        md->occurrence() = occurNum;
        md->multipleOccurrence() = mMaxOccurs > 1;
    }

    return md;
}


void PmtVectorElement::show(bool showObjectMemoryPointers, int level) const
{
    PmtMdVecConstIter curIter = mMdVec.begin();
    PmtMdVecConstIter end = mMdVec.end();

    if (curIter != end)
    {
        cout << "  - multiple occurrence element -";

        while (curIter != end)
        {
            if (*curIter)
            {
                cout << endl;
                for (int i = 0; i < level; i++)
                    cout << "  ";
                cout << "  occurrence " << (*curIter)->occurrence();
                if (showObjectMemoryPointers)
                    cout << " it's pointer is: " << (*curIter).ptr();
                cout << "   it's value is: ";
                (*curIter)->printDerived(cout);
            }
            else
                cout << "  it's pointer is NULL";

            curIter++;
        }
    }
    else
        cout << "  all occurrences are NULL";

    cout << endl;
}


PmtVectorElement::PmtVectorElement(const PmtVectorElement & orig,
                                   PmtCompositeMetadata *
                                   parentMd):PmtElement(orig)
{
    PmtMdVecConstIter curIter = orig.mMdVec.begin();
    while (curIter != orig.mMdVec.end())
    {
        mMdVec.push_back((*curIter)->clone(parentMd));
        curIter++;
    }
}


PmtMetadataPtr PmtVectorElement::getMetadatum(unsigned int occurNum,
                                              bool createMetadataIfNotExists,
                                              const PmtCompositeMetadata *
                                              parent)
{
    PmtMetadataPtr md = NULL;

    if ((occurNum + 1) <= mMdVec.size())
        md = mMdVec[occurNum];
    else if (createMetadataIfNotExists)
        md = getMetadatumCheckingForMinOccurrences(occurNum, parent);

    return md;
}


PmtMetadataPtr PmtVectorElement::
getMetadatumCheckingForMinOccurrences(unsigned int occurNum,
                                      const PmtCompositeMetadata * parent)
{
    PmtMetadataPtr md = NULL;
    // Only if the requested occurrence is less than or equal to the maximum allowed
    if ((occurNum + 1) <= mMaxOccurs)
    {
        // If there have not been enough occurrences created yet, to meet the
        // minOccurs required in the schema, then create them now.
        if (mMinOccurs > mMdVec.size())
        {
            if (!createOccurrenceMetadata(mMinOccurs, parent))
                return NULL;
        }

        // If the requested occurrence is more than the number that have already
        // been created, then create the previous occurrences first.
        if ((occurNum + 1) > mMdVec.size())
        {
            if (!createOccurrenceMetadata(occurNum + 1, parent))
                return NULL;
        }

        if (mMdVec.size() >= (occurNum + 1))
        {
            md = getMetadatumOrCreateAndSetParent(occurNum, parent);
        }
    }

    return md;
}


// numToCreate is 1's based, not 0's based.
bool PmtVectorElement::createOccurrenceMetadata(int numToCreate,
                                                const PmtCompositeMetadata *
                                                parent)
{
    for (int counter = 0; counter < numToCreate; counter++)
    {
        PmtMetadataPtr md;
        md = getMetadatum(counter, false, NULL);
        // If occurrence has not been created yet
        if (!md)
        {
            md = getMetadatumOrCreateAndSetParent(counter, parent);
            if (!md)
                return false;
        }
    }

    return true;
}


PmtMetadataPtr PmtVectorElement::
getMetadatumOrCreateAndSetParent(unsigned int occurNum,
                                 const PmtCompositeMetadata * parent)
{
    // We may have already created the needed occurrence, so try to
    // get it first, before unconditionally creating it.
    PmtMetadataPtr md = getMetadatum(occurNum, false, NULL);
    if (!md)
    {
        md = createFromFactoryAndSetParentAndOccurrence(parent, occurNum);
        if (md)
        {
            mMdVec.push_back(md);
        }
    }

    return md;
}


void PmtVectorElement::deleteMetadatum(int occurNum)
{
    // If there are other occurrences after this object, then collapse the vector.

    int origSize = mMdVec.size();

    if (origSize == 0)
        return;                 // Do nothing, since already empty.

    if ((occurNum == 0) && (origSize == 1))
    {
        mMdVec.clear();
    }
    else
    {
        for (int counter = occurNum; counter < origSize - 1; counter++)
        {
            mMdVec[counter] = mMdVec[counter + 1];
            mMdVec[counter]->occurrence() = counter;
        }

        mMdVec.pop_back();
    }
}


unsigned int PmtVectorElement::getWildcardMax()
{
    if (mMaxOccurs == PMT_OCCURS_UNBOUNDED)
    {
        // If none exist yet
        if (mMdVec.size() == 0)
            return 1;
        else
            return mMdVec.size();
    }
    else
        return mMaxOccurs;
}


bool PmtVectorElement::isEmpty()
{
    PmtMdVecIter curIter = mMdVec.begin();

    while (curIter != mMdVec.end())
    {
        if (*curIter)
            return false;

        curIter++;
    }

    return true;
}


ostream & PmtVectorElement::printDerived(ostream & c) const
{
    PmtMdVecConstIter curIter = mMdVec.begin();
    while (curIter != mMdVec.end())
    {
        if (*curIter)
            (*curIter)->printDerived(c);

        curIter++;
    }

    return c;
}


void PmtVectorElement::accept(PmtMetadataVisitor * visitor)
{
    PmtMdVecConstIter curIter = mMdVec.begin();
    while (curIter != mMdVec.end())
    {
        if (*curIter)
            (*curIter)->accept(visitor);
        curIter++;
    }
}


void PmtVectorElement::accept(PmtMdVisitorImpl * visitor)
{
    PmtMdVecConstIter curIter = mMdVec.begin();
    while (curIter != mMdVec.end())
    {
        if (*curIter)
            (*curIter)->accept(visitor);
        curIter++;
    }
}


PmtMetadataPtr PmtOptionalOrSingularElement::
getMetadatum(unsigned int occurNum, bool createMetadataIfNotExists,
             const PmtCompositeMetadata * parent)
{
    // In case someone calls for a non-0 occurrence on this object.
    // (PmtOptionalOrSingularElement objects always have a maxOccurs of 1).
    if (occurNum != 0)
        return NULL;

    if (!mMd)
        if (createMetadataIfNotExists)
            mMd = createMetadatumAndSetParent(parent);

    return mMd;
}


PmtMetadataPtr PmtOptionalOrSingularElement::
createMetadatumAndSetParent(const PmtCompositeMetadata * parent)
{
    PmtMetadataPtr md;

    md = createFromFactoryAndSetParentAndOccurrence(parent, 0);

    return md;
}


void PmtOptionalOrSingularElement::accept(PmtMetadataVisitor * visitor)
{
    if (mMd)
        mMd->accept(visitor);
}

void PmtOptionalOrSingularElement::accept(PmtMdVisitorImpl * visitor)
{
    if (mMd)
        mMd->accept(visitor);
}


ostream & PmtOptionalOrSingularElement::printDerived(ostream & c) const
{
    if (mMd)
        mMd->printDerived(c);

    return c;
}


void PmtOptionalOrSingularElement::show(bool showObjectMemoryPointers,
                                        int level) const
{
    cout << "  - optional or singular element - ";
    if (mMd)
    {
        if (!mMd->isComposite())
        {
            if (showObjectMemoryPointers)
                cout << "it's pointer is: " << mMd.ptr();
            cout << "   it's value is: ";
            mMd->printDerived(cout);
            cout << endl;
        }
        else                    // composite
        {
            cout << endl;
            mMd->show(showObjectMemoryPointers, level);
        }
    }
    else
        cout << "it's pointer is NULL" << endl;
}

