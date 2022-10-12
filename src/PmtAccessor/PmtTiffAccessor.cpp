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
 * Ricardo Rosario <ricardo.rosario@kodak.com>
 */

#include "EkCompiler.h"

#if defined(HAVE_EKTIFF) || defined(HAVE_OPENTIFF)

#include <stdlib.h>
#include <string>
#include <climits>
#include <cfloat>
#include <iostream>


#include "PmtCommon.h"
#include "PmtTiffAccessor.h"
#include "PmtTranslator.h"
#include "ptstring.h"

#include "TiffDirectory.h"
#include "TiffError.h"

#define PMT_TIFF_AUDIO_TYPE     "audio"
#define PMT_TIFF_ELEMENT_NAME   "TIFF"


inline std::ostream & TiffPrintValue(std::ostream & c,
    const ptstring<char>& v)
{
    return c << v;
}


PmtTiffAccessor::~PmtTiffAccessor()
{
    close();
}


void PmtTiffAccessor::close()
{
    if (!mClosing && mOwnImgFile && mImgFile)
    {
        mClosing = true;
        mImgFile->close();
        delete mImgFile;
        mImgFile = NULL;
        mOwnImgFile = false;
    }

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED
    // if we have an image accessor, delete it
    if (mImgAccessor)
    {
        delete mImgAccessor;
        mImgAccessor = NULL;
    }
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED

    mClosing = false;
}


bool PmtTiffAccessor::open(const char *name, PmtOpenMode mode)
{
    close();

    char openMode[10];
    if (mode == PMT_READONLY)
    {
        strcpy(openMode, "r");
    }
    else if (mode == PMT_CREATE)
    {
        strcpy(openMode, "w");
    }
    else if (mode == PMT_READWRITE)
    {
        strcpy(openMode, "r+");
    }


    mImgFile = new TiffImageFile();

    try
    {
        mImgFile->open(name, openMode);
    }
    catch(TiffError & e)
    {
        cout << e << endl;
        return false;
    }
    catch(...)
    {
        cout << "******Some exception caught" << endl;
        return false;
    }

    mFileName = name;
    mOwnImgFile = true;
    return true;
}


bool PmtTiffAccessor::recognize(const SPmtString & name)
{
    bool bRet = false;

    const u8_t TIFF_MAGIC1 = 0x49;
    const u8_t TIFF_MAGIC2 = 0x4D;
    const u8_t TIFF_MAGIC = 0x2A;

    FILE *f = fopen(name.c_str(), "r");
    if (f)
    {
        rewind(f);
        u8_t n[4];
        int m = fread(&n, sizeof(n), 1, f);
        bool little_endian = n[0] == TIFF_MAGIC1 && n[1] == TIFF_MAGIC1
            && n[2] == TIFF_MAGIC && n[3] == 0;
        bool big_endian = n[0] == TIFF_MAGIC2 && n[1] == TIFF_MAGIC2
            && n[2] == 0 && n[3] == TIFF_MAGIC;

        if (m != 1 || !(little_endian || big_endian))
        {
            bRet = false;
        }
        else
        {
            bRet = true;
        }

        fclose(f);
    }

    return bRet;
}


PmtMetadataPtr PmtTiffAccessor::readMetadata(const SPmtString nsURI)
{
    // Make sure any schema and translation table changes are handled
    loadIfAny();

    PmtMetadataPtr rootMd = PmtMetadata::create();
    
    // Prevent deletion of the translation table while we're using it.
    // (shouldn't this just be some kind of protected counter?)
    EK_GUARD(EK_DEFAULT_MUTEX, PmtTranslationTable::msLock);
    
    // Get the translation table column for this accessor
    PmtTranslationTableElements * theTable = 
                  msTranslationTable->getSubTable(PMT_TIFF_ELEMENT_NAME);

    if (theTable != NULL)
    {
        
        unsigned int i = 0;

        // loop through all the elements in the column and query the underlying
        // toolkit for the appropriate data (shouldn't this be the other way
        // around -- query toolkit and then insert it appropriately?)
        for (i = 0; i<theTable->size(); i++)
        {
            PmtTranslationTableElement * tableElement = (*theTable)[i];

            if (tableElement->mType == PMT_TIFF_AUDIO_TYPE)
            {
                PmtMetadataPtr md = rootMd->getMetadatum(tableElement->mKey);
                if ( !readAudio(tableElement->mLocation, md) )
                    rootMd->deleteMetadata(tableElement->mKey, 
                                           PMT_METADATA_KEYS_ONLY);
            }
            else
            {
                TiffTagEntry *entry = NULL;

                entry = getTagValue(tableElement->mLocation, 
                                    getTag(tableElement->mTag));

                if ( entry != NULL )
                {
                    PmtTranslatorPtr translator =
                        PmtTranslator::create(tableElement->mTranslator);
                    if (translator)
                    {
                        void *value = NULL;
                        int count;
                        switch (entry->getType())
                        {
                        case EKTIFF_ASCII:
                            {
                                SPmtString tmpStr =
                                    dynamic_cast < TiffTagEntryT < string >
                                    *>(entry)->getValue().c_str();
                                int strCnt = entry->getCount();
                                char *tmpPtr = new char[strCnt];
                                strncpy(tmpPtr, tmpStr.c_str(), strCnt);
                                tmpPtr[strCnt - 1] = '\0';
                                value = new SPmtString(tmpPtr);
                                delete[]tmpPtr;
                                break;
                            }
                        case EKTIFF_BYTE:
                        case EKTIFF_UNDEFINED:
                            getEntryData(entry, (char *&) value, count);
                            break;
                        case EKTIFF_UBYTE:
                            getEntryData(entry, (unsigned char *&) value,
                                         count);
                            break;
                        case EKTIFF_SHORT:
                            getEntryData(entry, (short *&) value, count);
                            break;
                        case EKTIFF_USHORT:
                            getEntryData(entry, (unsigned short *&) value,
                                         count);
                            break;
                        case EKTIFF_LONG:
                            getEntryData(entry, (PMTTIFF_INT32 *&) value, count);
                            break;
                        case EKTIFF_ULONG:
                            getEntryData(entry, (PMTTIFF_UINT32 *&) value,
                                         count);
                            break;
                        case EKTIFF_RATIONAL:
                        case EKTIFF_URATIONAL:
                        case EKTIFF_FLOAT:
                            getEntryData(entry, (float *&) value, count);
                            break;
                        case EKTIFF_DOUBLE:
                            getEntryData(entry, (double *&) value, count);
                            break;
                        default:
                            SPmtString m =
                            "PmtTiffAccessor::getMdValue: invalid TIFF type ";
                            m += entry->getType();
                            mErrors.push_back(PmtError(PMT_ERROR, m.c_str(), WHERE));
                        }

                        if ( value != NULL )
                        {
                            PmtMetadataPtr md = rootMd->getMetadatum(
                                                           tableElement->mKey);
                            translator->assign(md.ptr(), value,
                                           (unsigned int &) count, true);
                        }
                    }
                    else
                    {
                        SPmtString m =
                 "PmtTiffAccessor::readMetadata Can't create translator for ";
                        m += tableElement->mTranslator;
                        if (mThrowErrors)
                            throw PmtError(PMT_TRANSLATOR_NOT_FOUND, 
                                           m.c_str(), WHERE);
                        else
                            mErrors.push_back(PmtError
                                (PMT_TRANSLATOR_NOT_FOUND, m.c_str(), WHERE));
                    }
                }
            }
        }
    }

    return rootMd;
}


bool PmtTiffAccessor::getMdValue(PmtMetadataPtr & md)
{
    bool success = true; // the return value
    SPmtString loc, tag, type, translatorName;
    getFormatInfo(md, loc, tag, type, translatorName);

    TiffTagEntry *entry = NULL;
    if (type == PMT_TIFF_AUDIO_TYPE)
    {
        success = readAudio(loc, md);
    }
    else
    {
        entry = getTagValue(loc, getTag(tag));
        
        if ( entry != NULL )
        {
            PmtTranslatorPtr translator = PmtTranslator::create(translatorName);
            if (translator)
            {
                void *value = NULL;
                int count;
                switch (entry->getType())
                {
                    case EKTIFF_ASCII:
                    {
                        SPmtString tmpStr =
                            dynamic_cast < TiffTagEntryT<string>* >
                            (entry)->getValue().c_str();
                        int strCnt = entry->getCount();
                        char *tmpPtr = new char[strCnt];
                        strncpy(tmpPtr, tmpStr.c_str(), strCnt);
                        tmpPtr[strCnt - 1] = '\0';
                        value = new SPmtString(tmpPtr);
                        delete[]tmpPtr;
                        break;
                    }
                    case EKTIFF_BYTE:
                    case EKTIFF_UNDEFINED:
                        getEntryData(entry, (char *&) value, count);
                        break;
                    case EKTIFF_UBYTE:
                        getEntryData(entry, (unsigned char *&) value, count);
                        break;
                    case EKTIFF_SHORT:
                        getEntryData(entry, (short *&) value, count);
                        break;
                    case EKTIFF_USHORT:
                        getEntryData(entry, (unsigned short *&) value, count);
                        break;
                    case EKTIFF_LONG:
                        getEntryData(entry, (PMTTIFF_INT32 *&) value, count);
                        break;
                    case EKTIFF_ULONG:
                        getEntryData(entry, (PMTTIFF_UINT32 *&) value, count);
                        break;
                    case EKTIFF_RATIONAL:
                    case EKTIFF_URATIONAL:
                    case EKTIFF_FLOAT:
                        getEntryData(entry, (float *&) value, count);
                        break;
                    case EKTIFF_DOUBLE:
                        getEntryData(entry, (double *&) value, count);
                        break;
                    default:
                        success = false;
                }
                
                if ( success && (value!=NULL))
                {
                    translator->assign(md.ptr(), value,
                                       (unsigned int &) count, true);
                }
            }
            else
                success = false;
        }
        else
            success = false;
    }
    
    return success;
}


bool PmtTiffAccessor::setMdValue(const PmtMetadataPtr & md)
{
    bool success = true; // the return value
    SPmtString loc, tag, type, translatorName;
    
    if ( getFormatInfo(md, loc, tag, type, translatorName) )
    {
        if (type == PMT_TIFF_AUDIO_TYPE)
        {
            success = writeAudio(loc, md);
        }
        else
        {
            PmtTranslatorPtr translator = PmtTranslator::create(translatorName);
            if (translator)
            {
                void *value = NULL;
                int count;
                translator->assign(md.ptr(), value, (unsigned int &) count,
                                   false);

                unsigned short enumType = 0;
                try
                {
                    enumType = getType(type);
                }
                catch (PmtError e)
                {
                    success = false;
                }
            
                TiffTagEntry *entry = NULL;
            
                if ( success )
                {
                    switch (enumType)
                    {
                    case EKTIFF_ASCII:
                    {
                        entry = new TiffTagEntryT < string > (getTag(tag), enumType);
                    
                        SPmtString *pstr = (SPmtString *) (value);
                        ((TiffTagEntryT<string>*)entry)->
                                                    setValue(pstr->c_str());
                        entry->setCount(pstr->size() + 1);
                        delete pstr;
                        break;
                    }
                    case EKTIFF_BYTE:
                    case EKTIFF_UNDEFINED:
                        if (count > 1)
                            entry = new TiffTagEntryT < vector < char > > 
                                (getTag(tag), enumType);
                        else
                            entry = new TiffTagEntryT < char >(getTag(tag), enumType);
                    
                        setEntryData(entry, (char *) value, count);
                        break;
                    case EKTIFF_UBYTE:
                        if (count > 1)
                            entry = new TiffTagEntryT < vector 
                                < unsigned char > > (getTag(tag), enumType);
                        else
                            entry = new TiffTagEntryT < unsigned char >
                                (getTag(tag), enumType);
        
                        setEntryData(entry, (unsigned char *) value, count);
                        break;
                    case EKTIFF_SHORT:
                        if (count > 1)
                            entry = new TiffTagEntryT < vector < short > >
                                (getTag(tag), enumType);
                        else
                            entry = new TiffTagEntryT < short >
                                (getTag(tag), enumType);
                            
                        setEntryData(entry, (short *) value, count);
                        break;
                    case EKTIFF_USHORT:
                        if (count > 1)
                            entry = new TiffTagEntryT < vector 
                                < unsigned short > > (getTag(tag), enumType);
                        else
                            entry = new TiffTagEntryT < unsigned short >
                                (getTag(tag), enumType);
                    
                        setEntryData(entry, (unsigned short *) value, count);
                        break;
                    case EKTIFF_LONG:
                        if (count > 1)
                            entry = new TiffTagEntryT < vector < PMTTIFF_INT32 > >
                                (getTag(tag), enumType);
                        else
                            entry = new TiffTagEntryT < PMTTIFF_INT32 >
                                (getTag(tag), enumType);
        
                        setEntryData(entry, (PMTTIFF_INT32 *) value, count);
                        break;
                    case EKTIFF_ULONG:
                        if (count > 1)
                            entry = new TiffTagEntryT < vector 
                                < PMTTIFF_UINT32 > > (getTag(tag), enumType);
                        else
                            entry = new TiffTagEntryT < PMTTIFF_UINT32 >
                                (getTag(tag), enumType);
        
                        setEntryData(entry, (PMTTIFF_UINT32 *) value, count);
                        break;
                    case EKTIFF_RATIONAL:
                    case EKTIFF_URATIONAL:
                    case EKTIFF_FLOAT:
                        if (count > 1)
                            entry = new TiffTagEntryT < vector < float > >
                                (getTag(tag), enumType);
                        else
                            entry = new TiffTagEntryT < float >
                                (getTag(tag), enumType);
        
                        setEntryData(entry, (float *) value, count);
                        break;
                    case EKTIFF_DOUBLE:
                        if (count > 1)
                            entry = new TiffTagEntryT < vector < double > >
                                (getTag(tag), enumType);
                        else
                            entry = new TiffTagEntryT < double >
                                (getTag(tag), enumType);
                            
                        setEntryData(entry, (double *) value, count);
                        break;
                    default:
                        success = false;
                        delete(char *) value;
                    }
                }
                else
                    success = false;

                if ( success )
                {
                    success = setTagValue(loc, *entry);
           
                    if (entry)
                        delete entry;
                }
            }
            else
                success = false;
        }
    }
    else
        success = false;
        
    return success;
}

void PmtTiffAccessor::removeMdValue(const PmtMetadataPtr & md)
{
    SPmtString loc, tag, type, translatorName;
    TiffImageFile::DirPath dirPath = getDirPath(loc);
#ifdef HAVE_OPENTIFF
    if ( getFormatInfo(md, loc, tag, type, translatorName) && (mImgFile!=NULL) )
        mImgFile->removeGenericTag( getTag(tag), dirPath );
#endif
}

TiffTagEntry *PmtTiffAccessor::getTagValue(const SPmtString & loc,
                                           unsigned short tagNumber) const
{
    TiffImageFile::DirPath dirPath = getDirPath(loc);

    return  mImgFile->getGenericTag(tagNumber, dirPath);
}

bool PmtTiffAccessor::setTagValue(const SPmtString & loc,
                                  const TiffTagEntry & tagEntry)
{
    bool success = true; // the return value
    TiffImageFile::DirPath dirPath = getDirPath(loc);
    try
    {
        mImgFile->setGenericTag(tagEntry, dirPath);
    }
    catch(...)
    {
        success = false;
    }
    
    return success;
}


bool PmtTiffAccessor::readAudio(const SPmtString & loc, PmtMetadataPtr md) const
{
    bool success = true;
    TiffImageFile::DirPath dirPath = getDirPath(loc);
    TiffDirectory *dir = mImgFile->getDirectory(dirPath);
    if (dir)
    {
        int size = dir->getAudioSize();
        if (size)
        {
            unsigned char *buf = new unsigned char[size];
            try
            {
                success = (dir->readAudio(buf, size) == size);
            }
            catch(...)
            {
                success = false;
            }
            if (success)
            {
                vector < unsigned char >vals;
                copy(buf, buf + size,
                     back_insert_iterator < vector < unsigned char > > (vals));
                PmtMetadataT < vector < unsigned char > > *mdt =
                    dynamic_cast < PmtMetadataT < vector <
                    unsigned char > > *>(md.ptr());
                if ( mdt != NULL )
                    mdt->value() = vals;
                else
                    success = false;
            }
            delete buf;
        }
        else
            success = false;
    }
    else
        success = false;

    return success;
}


bool PmtTiffAccessor::writeAudio(const SPmtString & loc,
                                 PmtMetadataPtr md) const
{
    bool success = true; // the return value
    TiffImageFile::DirPath dirPath = getDirPath(loc);
    TiffDirectory *dir = mImgFile->getDirectory(dirPath);
    if (!dir)
    {
        mImgFile->createDirectory(dirPath);
        dir = mImgFile->getDirectory(dirPath);
    }

    PmtMetadataT< vector<unsigned char> >* temp = dynamic_cast < PmtMetadataT <
        vector < unsigned char > >* >(md.ptr());
    if ( temp )
    {
        const vector < unsigned char >&data = temp->value();
        int size = data.size();
        unsigned char *buf = new unsigned char[size];

        PmtMetadataT < vector < unsigned char > > *mdt =
            dynamic_cast < PmtMetadataT < vector < unsigned char > > *>
            (md.ptr());
        if ( mdt )
        {
            vector < unsigned char > vals = mdt->value();
            copy(vals.begin(), vals.end(), buf);

            try
            {
                dir->writeAudio(buf, size);
            }
            catch(...)
            {
                success = false;
            }
        }
        else
            success = false;
        
        delete buf;
    }
    else
        success = false;

    return success;
}


TiffImageFile::DirPath PmtTiffAccessor::getDirPath(const SPmtString & loc) const
{
    TiffImageFile::DirPath dirPath;
    if (loc == "IFD_MAIN")
    {
        dirPath.push_back(TiffImageFile::DirPath::value_type(0, 0));
    }
    else if (loc == "IFD_SUB")
    {
        dirPath.push_back(TiffImageFile::DirPath::value_type(0, 0));
        dirPath.
            push_back(TiffImageFile::DirPath::value_type(TIFFTAG_SUBIFD, 0));
    }
    else if (loc == "IFD_EXIF")
    {
        dirPath.push_back(TiffImageFile::DirPath::value_type(0, 0));
        dirPath.
            push_back(TiffImageFile::DirPath::value_type(TIFFTAG_EXIFIFD, 0));
    }
    else if (loc == "IFD_SOUND")
    {
        dirPath.push_back(TiffImageFile::DirPath::value_type(0, 0));
        dirPath.
            push_back(TiffImageFile::DirPath::value_type(TIFFTAG_SOUNDIFD, 0));
    }
    else if (loc == "IFD_GPS")
    {
        dirPath.push_back(TiffImageFile::DirPath::value_type(0, 0));
        dirPath.
            push_back(TiffImageFile::DirPath::value_type(TIFFTAG_GPSIFD, 0));
    }
    else if (loc == "IFD_EXIF_INTER")
    {
        dirPath.push_back(TiffImageFile::DirPath::value_type(0, 0));
        dirPath.
            push_back(TiffImageFile::DirPath::value_type(TIFFTAG_EXIFIFD, 0));
        dirPath.
            push_back(TiffImageFile::DirPath::value_type(TIFFTAG_INTERIFD, 0));
    }
    else if (loc == "IFD_KPRO")
    {
        dirPath.push_back(TiffImageFile::DirPath::value_type(0, 0));
        dirPath.push_back(TiffImageFile::DirPath::value_type(33424, 0));
    }
    else
    {
        SPmtString m =
            "PmtTiffAccessor::getDirPath: Unrecognized TIFF location ";
        m += loc;
        throw PmtError(PMT_TIFF_FORMAT_ENTRY_BAD_TAG_LOCATION, m, WHERE);
    }
    return dirPath;
}


bool PmtTiffAccessor::getFormatInfo(PmtMetadataPtr md, SPmtString & loc,
                                    SPmtString & tag, SPmtString & type,
                                    SPmtString & translator)
{
    bool success = true; // the return value
    PmtKeyType key = md->fullKey(false);
    
    // Prevent deletion of the translation table while we're using it.
    // (shouldn't this just be some kind of protected counter?)
    EK_GUARD(EK_DEFAULT_MUTEX, PmtTranslationTable::msLock);
    
    // Get the translation table column for this accessor
    PmtTranslationTableElements * theTable = 
        msTranslationTable->getSubTable(PMT_TIFF_ELEMENT_NAME);

    if (theTable != NULL)
    {
        unsigned int i = 0;
        
        PmtTranslationTableElement * tableElement = NULL;

        // find the appropriate item
        while ((i<theTable->size()) && 
               ((*theTable)[i]->mKey != key )) i++;
 
        // get the values as required
        if ( i<theTable->size() )
        {
            tableElement = (*theTable)[i];
            loc = tableElement->mLocation;
            tag = tableElement->mTag;
            type = tableElement->mType;
            translator = tableElement->mTranslator;
        }
        else
            success = false;
    }
    else
        success = false;
    
    return success;
}


unsigned short PmtTiffAccessor::getTag(const SPmtString & stag) const
{
    return (unsigned short) (atoi(stag.c_str()));
}


unsigned short PmtTiffAccessor::getType(const SPmtString & stype) const
{
    if (stype == "ascii")
        return EKTIFF_ASCII;
    else if (stype == "byte")
        return EKTIFF_BYTE;
    else if (stype == "ubyte")
        return EKTIFF_UBYTE;
    else if (stype == "short")
        return EKTIFF_SHORT;
    else if (stype == "ushort")
        return EKTIFF_USHORT;
    else if (stype == "long")
        return EKTIFF_LONG;
    else if (stype == "ulong")
        return EKTIFF_ULONG;
    else if (stype == "rational")
        return EKTIFF_RATIONAL;
    else if (stype == "urational")
        return EKTIFF_URATIONAL;
    else if (stype == "float")
        return EKTIFF_FLOAT;
    else if (stype == "double")
        return EKTIFF_DOUBLE;
    else if (stype == "undefined")
        return EKTIFF_UNDEFINED;
    else if (stype == "audio")
        return EKTIFF_UNDEFINED;
    else
    {
        SPmtString m = "PmtTiffAccessor::getType: invalid TIFF type for ";
        m += stype;
        throw PmtError(PMT_TIFF_FORMAT_ENTRY_BAD_TAG_TYPE, m, WHERE);
    }
}

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED
// This method checks to see if we have an open file and if so returns an
// image accessor for it. Otherwise, it returns NULL
PmtImageAccessor *PmtTiffAccessor::getImageAccessor()
{
    // Check to see if we have an open file
    if (mImgFile)
    {
        // Check if we already have an image accessor for this file. Create it if
        // necessary.
        if (!mImgAccessor)
            mImgAccessor = new PmtTiffImageAccessor(mImgFile);
        else
            mImgAccessor->setImageFile(mImgFile);
    }

    return mImgAccessor;
}
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED

#endif
