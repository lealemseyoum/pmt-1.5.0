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
 * George Sotak   george.sotak@kodak.com
 * 
 * Contributor(s): 
 *
*/
#include <cstdio>

#include "EkCompiler.h"
#include "PmtIoFunctions.h"
#include "PmtCompositeMetadata.h"
#include "PmtError.h"
#include "PmtMetadata.h"
#include "PmtInstanceInterpreter.h"
#include "PmtRootMetadata.h"
#include "PmtCoreCommon.h"
#include "PmtDefinitionTypes.h"

PmtMetadata & PmtMetadata::operator=(const PmtMetadata & orig)
{
    if (this == &orig)
        return *this;
    mMetadataKey = orig.mMetadataKey;
    mType = orig.mType;
    mParent = 0;
    return *this;
}

const PmtKeyType PmtMetadata::key(bool includeOccurrence) const
{
    // Only when this is a multiple occurrence item, do we potentially output the brackets.
    if (includeOccurrence && mMultipleOccurrence)
    {
        PmtKeyType key;
        const int MAX_UNSIGNED_INT_LEN = 11;
        char tempBuf[MAX_UNSIGNED_INT_LEN];
        key = mMetadataKey + '[';
        snprintf(tempBuf, MAX_UNSIGNED_INT_LEN,"%u", mOccurrence);
        key += tempBuf;
        key += ']';
        return key;
    }
    else
        return mMetadataKey;
}


const PmtKeyType PmtMetadata::fullKey(bool includeOccurrence) const
{
    PmtKeyType fullKey = key(includeOccurrence);
    const PmtMetadata *cmd = parent();
    if (cmd && cmd->parent())
    {
        fullKey = "." + fullKey;
        fullKey = cmd->fullKey(includeOccurrence) + fullKey;
    }
/*
    //!!! - Until namespace support is added, just strip off the root element.
    PmtKeyType::size_type pos;
    // If more than 1 level
    pos = fullKey.find('.');
    if (pos != PmtKeyType::npos)
    {
        PmtKeyType firstSegment = fullKey.substr(0, pos);
        if (firstSegment.find(ROOT_ELEMENT_NAME) != PmtKeyType::npos)
            fullKey = fullKey.substr(pos + 1);
    }
*/

    return fullKey;
}

const SPmtString & PmtMetadata::getNameSpace(void) const
{
    return mParent->getNameSpace();
}

PmtRootMetadata *PmtMetadata::getRootMetadata(void) const
{
    return (mParent!=NULL)?mParent->getRootMetadata():NULL;
}


ostream & PmtMetadata::print(ostream & c, bool bFullKey) const
{
    // Print Format -- {factoryKey,metadataKey,derivedStuff}
    c << "{";
    PmtPrintValue(c, trueType());
    c << ",";
    PmtPrintValue(c, bFullKey ? fullKey(false) : key(false));
    c << ",";
    printDerived(c);
    c << "}";

    return c;
}

#if 0
// Read/write <SPmtString>
ostream & PmtPrintValue(ostream & c, const SPmtString & s)
{
    // To handle parsing of strings with quote characters embedded ...
    // Replace \ with \\ & " with \"
    SPmtString str2 = s;
    SPmtString rc("\\\"");
    for (unsigned int i = 0; i < rc.size(); i++)
    {
        unsigned int pos = 0;
        do
        {
            pos = str2.find(rc[i], pos);
            if (pos != SPmtString::npos)
            {
                str2.insert(pos, "\\");
                pos += 2;
            }
        }
        while (pos != SPmtString::npos);
    }
    return c << "\"" << str2 << "\"";
}

istream & PmtReadValue(istream & c, SPmtString & v)
{
    char ch;
    c.read(&ch, 1);
    v = "\0";
    while (!c.eof())
    {
        if (ch == '\\')         // character prefixed by escape
        {
            c.read(&ch, 1);
            v += ch;
        }
        else
            v += ch;
        c.read(&ch, 1);
    }
    return c;
}

#endif

// Only returns root element of schema.
PmtMetadataPtr PmtMetadata::create(const SPmtString & nameSpace)
{
    PmtMetadataPtr md = NULL;
    PmtMetadataFactory *factory = getFactories().getMdFactory(nameSpace);
    if (factory)
    {
//        md = factory->create(ROOT_ELEMENT_NAME);
        md = factory->create("");
    }
    return md;
}


void PmtMetadata::getKeyList(const PmtKeyType & key,
                             PmtKeyList & returnKeyList)
{
    getAliases().getKeyList(key, returnKeyList);
}

void PmtMetadataIterator::show(bool showObjectMemoryPointers)
{
    PmtMetadataPtr md = start();
    while (md.ptr() != NULL)
    {
        md->show(showObjectMemoryPointers);
        md = next();
    }
}

////////////////////////////////
// iostream operation for PmtMetadata

istream & operator>>(istream & in, PmtMetadataPtr & md)
{
    PmtLogicalInstanceInterpreter interp;
    in >> interp;
    md = md ? interp.readMetadata(md) : interp.readMetadata();
    return in;
}


ostream & operator<<(ostream & out, const PmtMetadataPtr & md)
{
    PmtLogicalInstanceInterpreter interp((PmtMetadataPtr)md);
    out << interp;
    return out;
}


PmtAttributePtr PmtMetadata::getAttribute(PmtKeyType name,
                                          bool createIfNotExists)
{
    PmtAttributePtr returnValue = NULL;
    PmtAttributeMap::iterator iter = mAttributeMap.find(name);

    if (iter != mAttributeMap.end())
    {
        if ((*iter).second)
            return (*iter).second;
        else if (createIfNotExists)
        {
            PmtKeyType key = fullKey(false);
            key += ":";
            key += name;
            returnValue = getAttrFactory()->create(key);
            if (returnValue.ptr() != NULL)
                (*iter).second = returnValue;
        }
    }
    return returnValue;
}

void PmtMetadata::setAttribute(PmtKeyType name, PmtAttributePtr pmtAttrPtr)
{

    PmtAttributeMap::iterator iter = mAttributeMap.find(name);

    // insert only if the map does not already contain it.
    if (iter == mAttributeMap.end())
    {
        mAttributeMap.insert(PmtAttributeMap::value_type(name, pmtAttrPtr));

    }
}


const PmtAttributeMap PmtMetadata::getAttributes(bool createIfNotExists)
{
    if (createIfNotExists)
    {
        PmtKeyType fKey = fullKey(false);

        PmtAttributeMap::iterator s = mAttributeMap.begin();
        while (s != mAttributeMap.end())
        {
            if (!((*s).second))
            {
                PmtKeyType key = fKey;
                key += ":";
                key += (*s).first;
                (*s).second = getAttrFactory()->create(key);
            }
            s++;
        }

        return mAttributeMap;
    }
    else
    {
        PmtAttributeMap attrMap;
        PmtAttributeMap::iterator s = mAttributeMap.begin();
        while (s != mAttributeMap.end())
        {
            if ((*s).second)
            {
                attrMap.
                    insert(PmtAttributeMap::
                           value_type((*s).first, (*s).second));
            }
            s++;
        }

        return attrMap;
    }
}


void PmtMetadata::showAttr(int level) const
{
    PmtAttributeMap::const_iterator s = mAttributeMap.begin();
    while (s != mAttributeMap.end())
    {
        if ((*s).second)
        {
            (*s).second->show(level);
        }
        else
        {
            for (int i = 0; i < level; i++)
                cout << "  ";

            cout << "PmtAttribute(" << (*s).first << ", ";
            cout << "not instantiated)" << endl;
        }
        s++;
    }
}


PmtMetadataFactory *PmtMetadata::getMdFactory() const
{
    if (mParent)
        return mParent->getMdFactory();
    else
        return 0;
}


PmtAttrFactory *PmtMetadata::getAttrFactory() const
{
    if (mParent)
        return mParent->getAttrFactory();
    else
        return 0;
}




void PmtMetadataFactory::clearIt(void)
{

//    PmtMetadata* md ;
//    md = start() ;
//    while( md )
//    {
//        delete md ;
//        md = next() ;
//    }

    clear();
}


PmtMetadataFactory *PmtMetadata::getFactory(const SPmtString & nameSpace)
{
    return getFactories().getMdFactory(nameSpace);
}

PmtMetadataFactories & PmtMetadata::getFactories(void)
{
    static PmtMetadataFactories factoryMap;
    return factoryMap;
}

PmtAliases & PmtMetadata::getAliases(void)
{
    static PmtAliases aliases;
    return aliases;
}

PmtMetadataFactory *PmtMetadataFactories::
createMdFactory(const SPmtString & nameSpace)
{
    PmtMetadataFactory *factory = new PmtMetadataFactory();
    pair < MdFactMap::iterator, bool > ret =
        mFactoryMap.insert(MdFactMap::value_type(nameSpace, factory));
    if (ret.second)
        return factory;
    else
    {
        delete factory;
        return 0;
    }
}


PmtMetadataFactory *PmtMetadataFactories::
getMdFactory(const SPmtString & nameSpace)
{
    MdFactMap::iterator s = mFactoryMap.find(nameSpace);
    if (s != mFactoryMap.end())
        return (*s).second;
    else
        return createMdFactory(nameSpace);
}


void PmtMetadataFactories::clear(void)
{
    MdFactMap::iterator s = mFactoryMap.begin();
    MdFactMap::iterator end = mFactoryMap.end();
    while (s != end)
    {
        delete(*s).second;
        s++;
    }
    mFactoryMap.erase(mFactoryMap.begin(), end);
}


void PmtMetadataFactories::clear(const SPmtString & nameSpace)
{

    MdFactMap::iterator s = mFactoryMap.find(nameSpace);
    if (s != mFactoryMap.end())
    {
        delete(*s).second;
        mFactoryMap.erase(s);
    }

    return;
}

// Convert from a trueType (string) to a PmtMDType
PmtMDType PmtMetadata::getType(SPmtString trueType)
{
    PmtMDType returnValue;
    
    if (trueType == PMT_TRUE_TYPE_INT8)
        returnValue = PMT_MD_TYPE_INT8;
    else if (trueType == PMT_TRUE_TYPE_VINT8)
        returnValue = PMT_MD_TYPE_VINT8;
    else if (trueType == PMT_TRUE_TYPE_UINT8)
        returnValue = PMT_MD_TYPE_UINT8;
    else if (trueType == PMT_TRUE_TYPE_VUINT8)
        returnValue = PMT_MD_TYPE_VUINT8;
    else if (trueType == PMT_TRUE_TYPE_INT16)
        returnValue = PMT_MD_TYPE_INT16;
    else if (trueType == PMT_TRUE_TYPE_VINT16)
        returnValue = PMT_MD_TYPE_VINT16;
    else if (trueType == PMT_TRUE_TYPE_UINT16)
        returnValue = PMT_MD_TYPE_UINT16;
    else if (trueType == PMT_TRUE_TYPE_VUINT16)
        returnValue = PMT_MD_TYPE_VUINT16;
    else if (trueType == PMT_TRUE_TYPE_INT32)
        returnValue = PMT_MD_TYPE_INT32;
    else if (trueType == PMT_TRUE_TYPE_VINT32)
        returnValue = PMT_MD_TYPE_VINT32;
    else if (trueType == PMT_TRUE_TYPE_UINT32)
        returnValue = PMT_MD_TYPE_UINT32;
    else if (trueType == PMT_TRUE_TYPE_VUINT32)
        returnValue = PMT_MD_TYPE_VUINT32;
    else if (trueType == PMT_TRUE_TYPE_FLOAT)
        returnValue = PMT_MD_TYPE_FLOAT;
    else if (trueType == PMT_TRUE_TYPE_VFLOAT)
        returnValue = PMT_MD_TYPE_VFLOAT;
    else if (trueType == PMT_TRUE_TYPE_DOUBLE)
        returnValue = PMT_MD_TYPE_DOUBLE;
    else if (trueType == PMT_TRUE_TYPE_VDOUBLE)
        returnValue = PMT_MD_TYPE_VDOUBLE;
    else if (trueType == PMT_TRUE_TYPE_STRING)
        returnValue = PMT_MD_TYPE_STRING;
    else if (trueType == PMT_TRUE_TYPE_COMPOSITE)
        returnValue = PMT_MD_TYPE_COMPOSITE;
    else if (trueType == PMT_TRUE_TYPE_BOOLEAN)
        returnValue = PMT_MD_TYPE_BOOL;
    else if (trueType == PMT_TRUE_TYPE_VBOOLEAN)
        returnValue = PMT_MD_TYPE_VBOOL;
    else if (trueType == PMT_TRUE_TYPE_VSTRING)
        returnValue = PMT_MD_TYPE_VSTRING;
    else if (trueType == PMT_TRUE_TYPE_WSTRING)
        returnValue = PMT_MD_TYPE_WSTRING;
    else if (trueType == PMT_TRUE_TYPE_VWSTRING)
        returnValue = PMT_MD_TYPE_VWSTRING;
    else if (trueType == PMT_TYPE_INT8)
        returnValue = PMT_MD_TYPE_INT8;
    else if (trueType == PMT_TYPE_UINT8)
        returnValue = PMT_MD_TYPE_UINT8;
    else if (trueType == PMT_TYPE_UINT16)
        returnValue = PMT_MD_TYPE_UINT16;
    else if (trueType == PMT_TYPE_INT32)
        returnValue = PMT_MD_TYPE_INT32;
    else if (trueType == PMT_TYPE_UINT32)
        returnValue = PMT_MD_TYPE_UINT32;
    else if (trueType == PMT_TYPE_BOOLEAN)
        returnValue = PMT_MD_TYPE_BOOL;
    else
        returnValue = PMT_MD_TYPE_UNKNOWN;
    
    return returnValue;
}

// Convert from a PmtMDType to a trueType (string)
SPmtString PmtMetadata::getType(PmtMDType mdType) 
{
    SPmtString returnValue;
    switch (mdType)
    {
    case PMT_MD_TYPE_UINT8:
        returnValue = PMT_TRUE_TYPE_UINT8;
        break;
    case PMT_MD_TYPE_VUINT8:
        returnValue = PMT_TRUE_TYPE_VUINT8;
        break;
      case PMT_MD_TYPE_UINT16:
        returnValue = PMT_TRUE_TYPE_UINT16;
        break;
    case PMT_MD_TYPE_VUINT16:
        returnValue = PMT_TRUE_TYPE_VUINT16;
        break;  
    case PMT_MD_TYPE_UINT32:
        returnValue = PMT_TRUE_TYPE_UINT32;
        break;
    case PMT_MD_TYPE_VUINT32:
        returnValue = PMT_TRUE_TYPE_VUINT32;
        break;
    case PMT_MD_TYPE_FLOAT:
        returnValue = PMT_TRUE_TYPE_FLOAT;
        break;
    case PMT_MD_TYPE_VFLOAT:
        returnValue = PMT_TRUE_TYPE_VFLOAT;
        break;
    case PMT_MD_TYPE_STRING:
        returnValue = PMT_TRUE_TYPE_STRING;
        break;
    case PMT_MD_TYPE_COMPOSITE:
        returnValue = PMT_TRUE_TYPE_COMPOSITE;
        break;
    case PMT_MD_TYPE_INT8:
        returnValue = PMT_TRUE_TYPE_INT8;
        break;
    case PMT_MD_TYPE_VINT8:
        returnValue =  PMT_TRUE_TYPE_VINT8;
        break;
    case PMT_MD_TYPE_INT16:
        returnValue = PMT_TRUE_TYPE_INT16;
        break;
    case PMT_MD_TYPE_VINT16:
        returnValue = PMT_TRUE_TYPE_VINT16;
        break;
    case PMT_MD_TYPE_INT32:
        returnValue = PMT_TRUE_TYPE_INT32;
        break;
    case PMT_MD_TYPE_VINT32:
        returnValue = PMT_TRUE_TYPE_VINT32;
        break;
    case PMT_MD_TYPE_DOUBLE:
        returnValue = PMT_TRUE_TYPE_DOUBLE;
        break;
    case PMT_MD_TYPE_VDOUBLE:
        returnValue = PMT_TRUE_TYPE_VDOUBLE;
        break;
    case PMT_MD_TYPE_BOOL:
        returnValue = PMT_TRUE_TYPE_BOOLEAN;
        break;
    case PMT_MD_TYPE_VBOOL:
        returnValue = PMT_TRUE_TYPE_VBOOLEAN;
        break;
    case PMT_MD_TYPE_VSTRING:
        returnValue = PMT_TRUE_TYPE_VSTRING;
        break;
    case PMT_MD_TYPE_WSTRING:
        returnValue = PMT_TRUE_TYPE_WSTRING;
        break;
    case PMT_MD_TYPE_VWSTRING:
        returnValue = PMT_TRUE_TYPE_VWSTRING;
        break;
    default:
        returnValue = "";
    }
    
    return returnValue;
}
