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
 * Creation date: January 1999
 *
 * Original Author: 
 * Chris Lin    ti.lin@kodak.com
 * Copyright (C) 2001 Eastman Kodak Company. All Rights Reserved.
 * 
 * Contributor(s): 
 *  George Sotak  <george.sotak@kodak.com>
 */

#include "EkCompiler.h"
#include "PmtAttribute.h"

PmtAttrFactories PmtAttribute::msAttrFactoryMap;

PmtAttrFactory *PmtAttribute::getFactory(const SPmtString & nameSpace)
{
    return msAttrFactoryMap.getAttrFactory(nameSpace);
}

PmtAttrFactories::~PmtAttrFactories()
{
    clear();
}


PmtAttrFactory *PmtAttrFactories::
createAttrFactory(const SPmtString & nameSpace)
{
    PmtAttrFactory *factory = new PmtAttrFactory();
    pair < AttrFactMap::iterator, bool > ret =
        mFactoryMap.insert(AttrFactMap::value_type(nameSpace, factory));
    if (ret.second)
        return factory;
    else
    {
        delete factory;
        return 0;
    }
}


PmtAttrFactory *PmtAttrFactories::getAttrFactory(const SPmtString & nameSpace)
{
    AttrFactMap::iterator s = mFactoryMap.find(nameSpace);
    if (s != mFactoryMap.end())
        return (*s).second;
    else
        return createAttrFactory(nameSpace);
}


void PmtAttrFactories::clear()
{
    AttrFactMap::iterator s = mFactoryMap.begin();
    AttrFactMap::iterator end = mFactoryMap.end();
    while (s != end)
    {
        delete(*s).second;
        s++;
    }
    mFactoryMap.erase(mFactoryMap.begin(), end);
}


void PmtAttrFactories::clear(const SPmtString & nameSpace)
{

    AttrFactMap::iterator s = mFactoryMap.find(nameSpace);
    if (s != mFactoryMap.end())
    {
        delete(*s).second;
        mFactoryMap.erase(s);
    }

    return;
}

