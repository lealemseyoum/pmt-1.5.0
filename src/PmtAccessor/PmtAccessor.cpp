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
 * Ricardo Rosario  ricardo.rosario@kodak.com
*/

#include "EkCompiler.h"

#include <stdlib.h>
#include <map>
#include <vector>

#include "PmtDefinitionInterpreter.h"
#include "PmtCompositeMetadata.h"
#include "PmtAccessor.h"
#include "PmtMetadata.h"
#include "PmtTranslationTable.h"
#include "PmtMisc.h"
#include "PmtFaceAtoms.h"

PmtTranslationTablePtr PmtAccessor::msTranslationTable = new PmtTranslationTable();
bool PmtAccessor::msTranslationTableLoaded = false;

void PmtAccessor::loadIfAny()
{
    EK_GUARD(EK_DEFAULT_MUTEX, PmtLogicalDefinitionInterpreter::loadLock());

    if (PmtLogicalDefinitionInterpreter::getErased())
    {
        erase();
        PmtLogicalDefinitionInterpreter::setErased(false);
    }

	// see if we've already loaded everything
	if (!msTranslationTableLoaded || !PmtLogicalDefinitionInterpreter::ttbFilesChanged())
	{
		// If we haven't loaded everything, clear for a re-load!
		if (msTranslationTable->mTranslationTable.size() > 0)
			erase();

		if (!PmtLogicalDefinitionInterpreter::ttbFiles().empty())
		{
			for (unsigned int i = 0;
				 i < PmtLogicalDefinitionInterpreter::ttbFiles().size(); i++)
			{
				// The ttbFiles()[i] is an in-memory translation table if
				// the .second of the pair is set to true.
				if (PmtLogicalDefinitionInterpreter::ttbFiles()[i].second)
				{
					loadMemory(PmtLogicalDefinitionInterpreter::ttbFiles()[i].
							   first.first.c_str());
				}
				else
				{
					if (PmtLogicalDefinitionInterpreter::ttbFiles()[i].first.
						first == DEFAULT_TRANSLATION_TABLE)
						load();
					else
						load(PmtLogicalDefinitionInterpreter::ttbFiles()[i].first.first);
				}
			}

			// Show that we've loaded the most recent items...
			PmtLogicalDefinitionInterpreter::ttbFilesChanged(true);
			msTranslationTableLoaded = true;
		}
	}

	// Make sure the face atoms are loaded (these may appear in multiple asset types
	PmtInitializeFaceAtoms();
}


void PmtAccessor::load()
{
	msTranslationTable->load();
}


void PmtAccessor::load(const SPmtString & filename)
{
	msTranslationTable->load(filename);
}


void PmtAccessor::loadMemory(const char *translationTable)
{
	msTranslationTable->loadMemory(translationTable);
}


void PmtAccessor::erase()
{
    msTranslationTable->erase();
}



////////////////////////////////////////////////////////////////////////////////

PmtAccessorPtr PmtAccessor::getAccessor(const char *fileName, PmtOpenMode mode)
{
    PmtAccessorPtr acc;
    
    PmtImageFileFormatName format = PmtAccessor::recognizeFilename(fileName);
    if (format != PMT_FORMAT_UNKNOWN)
    {
        acc = PmtAccessor::create(format);
        if (!((acc) && (acc->open(fileName, mode))))
            acc = NULL;
    }

    return acc;
}


PmtMetadataPtr PmtAccessor::readMetadata(const SPmtString nsURI)
{
    bool throwErrors = mThrowErrors;
    mThrowErrors = true;

    PmtMetadataPtr rootMd = PmtMetadata::create(nsURI);
    PmtMetadataIterator iter =
        rootMd->getMetadata("*", true, PMT_METADATA_KEYS_ONLY);
    PmtMetadataPtr md = iter.start();
    while (md)
    {
        try
        {
            readMetadata(md);
        }
        catch(PmtError)
        {
            rootMd->deleteMetadata(md->fullKey(false));
        }
        catch(...)
        {
            rootMd->deleteMetadata(md->fullKey(false));
        }

        md = iter.next();
    }

    mThrowErrors = throwErrors;

    return rootMd;
}


PmtMetadataPtr PmtAccessor::readMetadata(PmtMetadataPtr & md)
{
    if (!md)
        return NULL;

    // not need to load in translation table for XML accessor
    if (mFormatName != PMT_FORMAT_XML)
        loadIfAny();

    if (md->isComposite())
    {
        PmtMetadataIterator mdIter = md->getMetadata("");
        PmtMetadataPtr child = mdIter.start();
        while (child)
        {
            readMetadata(child);
            child = mdIter.next();
        }
    }
    else
    {
        try
        {
            if ( !getMdValue(md) )
            {
                PmtError e(PMT_ERROR, "Error in getMdValue()",
                    WHERE);
                if (mThrowErrors)
                    throw e;
                else
                    mErrors.push_back(e);
            }
        }
        catch(PmtError & e)
        {
            if (mThrowErrors)
                throw e;
            else
                mErrors.push_back(e);

        }
        catch(...)
        {
            PmtError e(PMT_INTERNAL_ERROR, "Generic Exception Caught", WHERE);
            if (mThrowErrors)
                throw e;
            else
                mErrors.push_back(e);

        }
    }

    return md;
}


PmtMetadataIterator & PmtAccessor::readMetadata(PmtMetadataIterator & mdIter)
{
    PmtMetadataPtr md = mdIter.start();
    while (md)
    {
        readMetadata(md);
        md = mdIter.next();
    }

    return mdIter;
}

//////////////////////////////////////////////////////////////////////////////////////


void PmtAccessor::writeMetadata(const PmtMetadataPtr & md, bool bWriteValid)
{
    // not need to load in translation table for XML accessor
    if (mFormatName != PMT_FORMAT_XML)
        loadIfAny();

    if (!md)
        return;

    PmtMetadataPtr pmd = md;

    if (bWriteValid)
    {
        while (pmd->parent())
        {
            pmd = (PmtMetadata *) (pmd->parent());
        }
    }

    if (pmd->isComposite())
    {
        PmtMetadataIterator mdIter = pmd->getMetadata("");
        PmtMetadataPtr child = mdIter.start();
        while (child)
        {
            writeMetadata(child);
            child = mdIter.next();
        }
    }
    else
    {
        try
        {
            setMdValue(pmd);
        }
        catch(PmtError & e)
        {
            if (mThrowErrors)
                throw;
            else
                mErrors.push_back(e);
        }
    }
}


void PmtAccessor::writeMetadata(PmtMetadataIterator & mdIter, bool bWriteValid)
{
    PmtMetadataPtr md = mdIter.start();
    if (!bWriteValid)
    {
        while (md)
        {
            writeMetadata(md, false);
            md = mdIter.next();
        }
    }
    else
    {
        writeMetadata(md, true);
    }
}

void PmtAccessor::removeMetadata(const PmtMetadataPtr & md)
{
    // no need to load in translation table for XML accessor
    if (mFormatName != PMT_FORMAT_XML)
        loadIfAny();

    if (md)
    {
        PmtMetadataPtr pmd = md;

        if (pmd->isComposite())
        {
            PmtMetadataIterator mdIter = pmd->getMetadata("");
            PmtMetadataPtr child = mdIter.start();
            while (child)
            {
                removeMetadata(child);
                child = mdIter.next();
            }
        }
        else
            removeMdValue(pmd);
    }
}

void PmtAccessor::printErrorsSuppressed(ostream & c) const
{
    for (unsigned int i = 0; i < mErrors.size(); i++)
        mErrors[i].print(c);
}


// End PmtAccessor.cpp

