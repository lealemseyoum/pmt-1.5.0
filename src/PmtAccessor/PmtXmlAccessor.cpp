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
 * Creation Date: 7/18/00
 *
 * Original Author: 
 * Chris Lin <ti.lin@kodak.com> 
 * 
 * Contributor(s): 
 */
#include <cstdio>

#include "EkCompiler.h"
#include "PmtXmlAccessor.h"

PmtXmlAccessor::~PmtXmlAccessor()
{
    close();
}

void PmtXmlAccessor::close()
{
    if (!mClosing)
    {
        mClosing = true;
        PmtAccessor::close();
        interpreter.close();
    }
    mClosing = false;
}


bool PmtXmlAccessor::open(const char *name, PmtOpenMode mode)
{
    return (interpreter.open(name, mode));
}


// support only UTF-8 currently
bool PmtXmlAccessor::recognize(const SPmtString & name)
{
    const int len = 5;
    char xmlDecl[] = "<?xml";
    char buf[len];

    FILE *f = fopen(name.c_str(), "r");
    if (f)
    {
        rewind(f);
        int m = fread(buf, sizeof(char), len, f);
        fclose(f);

        if (m != len)
            return false;
        for (int i = 0; i < len; i++)
        {
            if (buf[i] != xmlDecl[i])
                return false;
        }
        // get to be true if get here.
        return true;
    }
    return false;
}


PmtMetadataPtr PmtXmlAccessor::readMetadata(const SPmtString nsURI)
{
    PmtMetadataPtr rootMd = NULL;
    if (nsURI != interpreter.getNamespaceURI())
        return rootMd;

    try
    {
        rootMd = interpreter.readMetadata();
    }
    catch(PmtError & e)
    {
        if (mThrowErrors)
            throw;
        else
            mErrors.push_back(e);
    }

    return rootMd;
}


PmtMetadataPtr PmtXmlAccessor::readMetadata(PmtMetadataPtr & md)
{
    try
    {
        interpreter.readMetadata(md);
    }
    catch(PmtError & e)
    {
        if (mThrowErrors)
            throw;
        else
            mErrors.push_back(e);
    }

    return md;
}

void PmtXmlAccessor::writeMetadata(const PmtMetadataPtr & md, bool bWriteValid)
{
    try
    {
        PmtMetadataPtr pmd = md;

        if (bWriteValid)
        {
            while (pmd->parent())
            {
                pmd = (PmtMetadata *) (pmd->parent());
            }
        }
        interpreter.writeMetadata(pmd);
    }
    catch(PmtError & e)
    {
        if (mThrowErrors)
            throw;
        else
            mErrors.push_back(e);
    }
}

void PmtXmlAccessor::removeMetadata(const PmtMetadataPtr & md)
{
    interpreter.removeMetadata(md);
}

bool PmtXmlAccessor::setNSPrefix(const SPmtString & nsPrefix)
{
    return interpreter.setNSPrefix(nsPrefix);
}

