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
 * Creation Date: 3/12/03
 *
 * Original Author: 
 * Sam Fryer <samuel.fryer@kodak.com> 
 * 
 * Contributor(s): 
 * 
 */

#include "EkCompiler.h"

#ifdef PMT_PMG_ACCESSOR

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <climits>
#include <cfloat>

#include "png.h"

#include "PmtCommon.h"
#include "PmtPngAccessor.h"
#include "PmtTranslator.h"

#include "PmtDomString.h"


// #define PMT_XML_ATTR_LOC        "Location"
// #define PMT_XML_ATTR_TAG        "Tag"
// #define PMT_XML_ATTR_TYPE       "Type"
// #define PMT_XML_ATTR_TRANSLATOR "Translator"

// #if defined ( __GNUC__ ) && ( __GNUC__ > 2 )
// // SMF 2/11/2002 -- This is a work-around for a bug (ICE) in the GCC 3.0.3 compiler.
// int *PmtPngAccessor_tmp =
//     (int *) PMT_ACCESSOR_TYPE_INIT(PMT_FORMAT_PNG, PmtPngAccessor);
// const int *PmtPngAccessor_tmp2[] = { PmtPngAccessor_tmp };
// const int *PmtPngAccessor_FILE[1];
// void *PmtPngAccessor_FILE_Addr = &PmtPngAccessor_FILE;
// bool PmtPngAccessor_tmp3 = (PmtPngAccessor_FILE_Addr = &PmtPngAccessor_tmp2);
// #else
// EK_PROVIDE(PmtPngAccessor,
//            PMT_ACCESSOR_TYPE_INIT(PMT_FORMAT_PNG, PmtPngAccessor))
// #endif

// A class to hold all the PNG information.  This is defined here as it should
// only be used in this file.  It is NOT an open interface.
// Is this a thread safe class?
class PmtPngAccessorData
{
  public:
    inline PmtPngAccessorData(FILE * p = NULL,
                              const char *name = NULL,
                              bool own = false):mFile(p), mOwnImgFile(own)
    {
        mFileName = name;
    }

    // A copy constructor -- watch out for closed files!
    inline PmtPngAccessorData(const PmtPngAccessorData *
                              src):mFile(src->mFile), mOwnImgFile(false)
    {
        mFileName = src->mFileName;
    }

    ~PmtPngAccessorData()
    {
        if (mFile && mOwnImgFile)
            fclose(mFile);

        // clean up after the read, and free any memory allocated
        if (png_ptr && info_ptr)
            png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
    }

    FILE *mFile;
    SPmtString mFileName;
    bool mOwnImgFile;

    // LIBPNG information
    png_structp png_ptr;
    png_infop info_ptr;
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;

};

//! Default constructor
PmtPngAccessor::PmtPngAccessor():PmtAccessor(PMT_FORMAT_PNG), mPngData(NULL),
#ifdef PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
mImgAccessor(NULL),
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
mClosing(false)
{
}

// //! The constructor which takes the image FILE handle.
// PmtPngAccessor::PmtPngAccessor( FILE* img )
//           : PmtAccessor(PMT_FORMAT_PNG),
// #ifdef PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
//             mImgAccessor(NULL),
// #endif // PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
//             mClosing(false)
//         { mPngData = new PmtPngAccessorData(img,false);}

//! The copy constructor
PmtPngAccessor::
PmtPngAccessor(const PmtPngAccessor & src):PmtAccessor(PMT_FORMAT_PNG),
mPngData(src.mPngData),
#ifdef PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
mImgAccessor(NULL),
#endif
       // PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
mClosing(false)
{
}

PmtPngAccessor::~PmtPngAccessor()
{
    close();
}


void PmtPngAccessor::close()
{
    if (!mClosing && mPngData)
    {
        mClosing = true;
        delete mPngData;
        mPngData = NULL;
    }

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
    // if we have an image accessor, delete it
    if (mImgAccessor)
    {
        delete mImgAccessor;
        mImgAccessor = NULL;
    }
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED

    mClosing = false;
}


bool PmtPngAccessor::open(const char *name, PmtOpenMode mode)
{
    bool bRet = true;
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
        strcpy(openMode, "rw");
    }


    FILE *p = fopen(name, openMode);
    if (p)
        mPngData = new PmtPngAccessorData(p, name, true);
    else
        bRet = false;

    return bRet;
}


// Recognize whether this might be a PNG file or not by
// reading in the first four bytes and checking against a magic number.
bool PmtPngAccessor::recognize(const SPmtString & name) const
{
    bool bRet = false;

    FILE *f = fopen(name.c_str(), "r");
    if (f)
    {
        rewind(f);
        png_byte n[4];
        int m = fread(&n, 4, 1, f);
        if (m > 0)
            bRet = !png_sig_cmp(n, 0, 4);
        fclose(f);
    }

    return bRet;
}


PmtMetadataPtr PmtPngAccessor::readMetadata(const SPmtString nsURI)
{
    loadIfAny();

    PmtMetadataPtr rootMd = PmtMetadata::create();
    EK_GUARD(EK_DEFAULT_MUTEX, msLock);

// The PNG library can be compiled without text block support.
// Therefore, if it doesn't exist, we can't use it!
#if defined(PNG_TEXT_SUPPORTED)
    // set up for reading 
    mPngData->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                               NULL, NULL, NULL);
//      if (mPngData->png_ptr == NULL)
//      {
//        fclose(fp);
//        return (ERROR);
//      }

    // Allocate/initialize the memory for image information.
    mPngData->info_ptr = png_create_info_struct(mPngData->png_ptr);
//      if (mPngData->info_ptr == NULL)
//      {
//        fclose(fp);
//        png_destroy_read_struct(&mPngData->png_ptr, png_infopp_NULL, png_infopp_NULL);
//        return (ERROR);
//      }

    /* Set up the input control if you are using standard C streams */
    png_init_io(mPngData->png_ptr, mPngData->mFile);

    /* If we have already read some of the signature */
    png_set_sig_bytes(mPngData->png_ptr, 0);

    /* The call to png_read_info() gives us all of the information from the
     * PNG file before the first IDAT (image data chunk).
     */
    png_read_info(mPngData->png_ptr, mPngData->info_ptr);

    int i = 0;
    for (i = 0; i < mPngData->info_ptr->num_text; i++)
    {
        cout << mPngData->info_ptr->text[i].key << endl;
        cout << mPngData->info_ptr->text[i].text << endl;
    }
//     PmtTranslationTableMap::const_iterator s = msTranslationTable.begin();
//     while (s != msTranslationTable.end())
//     {
//         PmtKeyType key = (*s).first;
//         DOM_Node entryNode = (*s).second;
// 
//         DOM_NodeList nodeList = entryNode.getChildNodes();
//         DOM_Node tiffNode;
//         for (unsigned int i=0; i<nodeList.getLength(); i++)
//         {
//             DOM_Node p = nodeList.item(i);
//             PmtDomString pmtStr(p.getNodeName());
//             if (strcmp(pmtStr.getAsciiString(), PMT_TIFF_ELEMENT_NAME) == 0)
//             {
//                 tiffNode = p;
//                 break;
//             }
//         }
//     
//         if (!tiffNode.isNull())
//         {
//             DOM_NamedNodeMap attrMap = tiffNode.getAttributes();
//             PmtDomString pLoc(attrMap.getNamedItem(PMT_XML_ATTR_LOC).getNodeValue());
//             PmtDomString pTag(attrMap.getNamedItem(PMT_XML_ATTR_TAG).getNodeValue());
//             PmtDomString pType(attrMap.getNamedItem(PMT_XML_ATTR_TYPE).getNodeValue());
//             PmtDomString pTran(attrMap.getNamedItem(PMT_XML_ATTR_TRANSLATOR).getNodeValue());
//             SPmtString loc = pLoc.getString();
//             SPmtString tag = pTag.getString();
//             SPmtString type = pType.getString();
//             SPmtString translatorName = pTran.getString();
// 
//             if (type == PMT_TIFF_AUDIO_TYPE)
//             {
//                 PmtMetadataPtr md = rootMd->getMetadatum(key);
//                 try
//                 {
//                     readAudio(loc, md);
//                 }
//                 catch (PmtError) // can't read audio
//                 {
//                     rootMd->deleteMetadata(key, PMT_METADATA_KEYS_ONLY);
//                 }
//             }
//             else
//             {
//                 bool entryRead = true;
//                 TiffTagEntry *entry = 0;
//             
//                 try
//                 {
//                     entry = getTagValue(loc, getTag(tag));
//                 }
//                 catch (PmtError)
//                 {
//                     entryRead =false;
//                 }
// 
//                 if (entryRead)
//                 {
//                     PmtTranslatorPtr translator = PmtTranslator::create(translatorName);
//                     if (translator)
//                     {   
//                         void* value;
//                         int count;
//                         switch (entry->getType())
//                         {
//                             case EKTIFF_ASCII:
//                             {
//                                 SPmtString tmpStr = dynamic_cast<TiffTagEntryT<string>*>(entry)->getValue().c_str();
//                                 int strCnt = entry->getCount();
//                                 char * tmpPtr = new char[strCnt];
//                                 strncpy(tmpPtr,tmpStr.c_str(), strCnt);
//                                 tmpPtr[strCnt-1] = '\0';
//                                 value = new SPmtString(tmpPtr);
//                                 delete [] tmpPtr;
//                                 break;
//                             }
//                             case EKTIFF_BYTE:
//                             case EKTIFF_UNDEFINED:
//                                 getEntryData(entry, (char*&)value, count);
//                                 break;
//                             case EKTIFF_UBYTE:
//                                 getEntryData(entry, (unsigned char*&)value, count);
//                                 break;
//                             case EKTIFF_SHORT:
//                                 getEntryData(entry, (short*&)value, count);
//                                 break;
//                             case EKTIFF_USHORT:
//                                 getEntryData(entry, (unsigned short*&)value, count);
//                                 break;
//                             case EKTIFF_LONG:
//                                 getEntryData(entry, (long*&)value, count);
//                                 break;
//                             case EKTIFF_ULONG:
//                                 getEntryData(entry, (unsigned long*&)value, count);
//                                 break;
//                             case EKTIFF_RATIONAL:
//                             case EKTIFF_URATIONAL:
//                             case EKTIFF_FLOAT:
//                                 getEntryData(entry, (float*&)value, count);
//                                 break;
//                             case EKTIFF_DOUBLE:
//                                 getEntryData(entry, (double*&)value, count);
//                                 break;
//                             default:
//                                 char msg[100];
//                                 sprintf(msg, "PmtPngAccessor::getMdValue: invalid TIFF type %d", entry->getType());
//                                 throw PmtError(PMT_ERROR, msg, WHERE);
//                         }
// 
//                         PmtMetadataPtr md = rootMd->getMetadatum(key);
//                         translator->assign(md.ptr(), value, (unsigned long&)count, true);
//                     }
//                     else
//                     {
//                         char m[100];
//                         sprintf(m, "PmtPngAccessor::readMetadata Can't create translator for %s", 
//                             translatorName.c_str());
//                         if (mThrowErrors)
//                             throw PmtError(PMT_TRANSLATOR_NOT_FOUND, m, WHERE);
//                         else
//                             mErrors.push_back(PmtError(PMT_TRANSLATOR_NOT_FOUND, m, WHERE));
//                     }
//                 }
//             }
//         }
// 
//         s++;
//     }
#endif                          // PNG_TEXT_SUPPORTED

    return rootMd;
}


void PmtPngAccessor::getMdValue(PmtMetadataPtr & md)
{
    SPmtString loc, tag, type, translatorName;
//     getFormatInfo(md, loc, tag, type, translatorName);
// 
//     TiffTagEntry *entry = 0;
//     if (type == PMT_TIFF_AUDIO_TYPE)
//     {
//         readAudio(loc, md);
//         return;
//     }
//     else
//     {
//         entry = getTagValue(loc, getTag(tag));
//     }
// 
//     PmtTranslatorPtr translator = PmtTranslator::create(translatorName);
//     if (translator)
//     {   
//         void* value;
//         int count;
//         switch (entry->getType())
//         {
//             case EKTIFF_ASCII: {
//                 SPmtString tmpStr = dynamic_cast<TiffTagEntryT<string>*>(entry)->getValue().c_str();
//                 int strCnt = entry->getCount();
//                 char * tmpPtr = new char[strCnt];
//                 strncpy(tmpPtr,tmpStr.c_str(), strCnt);
//                 tmpPtr[strCnt-1] = '\0';
//                 value = new SPmtString(tmpPtr);
//                 delete [] tmpPtr;
//                 break;
//                 }
//             case EKTIFF_BYTE:
//             case EKTIFF_UNDEFINED:
//                 getEntryData(entry, (char*&)value, count);
//                 break;
//             case EKTIFF_UBYTE:
//                 getEntryData(entry, (unsigned char*&)value, count);
//                 break;
//             case EKTIFF_SHORT:
//                 getEntryData(entry, (short*&)value, count);
//                 break;
//             case EKTIFF_USHORT:
//                 getEntryData(entry, (unsigned short*&)value, count);
//                 break;
//             case EKTIFF_LONG:
//                 getEntryData(entry, (long*&)value, count);
//                 break;
//             case EKTIFF_ULONG:
//                 getEntryData(entry, (unsigned long*&)value, count);
//                 break;
//             case EKTIFF_RATIONAL:
//             case EKTIFF_URATIONAL:
//             case EKTIFF_FLOAT:
//                 getEntryData(entry, (float*&)value, count);
//                 break;
//             case EKTIFF_DOUBLE:
//                 getEntryData(entry, (double*&)value, count);
//                 break;
//             default:
//                 char msg[100];
//                 sprintf(msg, "PmtPngAccessor::getMdValue: invalid TIFF type %d", entry->getType());
//                 throw PmtError(PMT_ERROR, msg, WHERE);
//         }
// 
//         translator->assign(md.ptr(), value, (unsigned long&)count, true);
//     }
//     else
//     {
//         char m[100];
//         sprintf(m, "PmtPngAccessor::getMdValue Can't create translator for %s", 
//             translatorName.c_str());
//         throw PmtError(PMT_TRANSLATOR_NOT_FOUND, m, WHERE);
//     }
}


void PmtPngAccessor::setMdValue(const PmtMetadataPtr & md)
{
    SPmtString loc, tag, type, translatorName;
//     getFormatInfo(md, loc, tag, type, translatorName);
// 
//     if (type == PMT_TIFF_AUDIO_TYPE)
//     {
//         writeAudio(loc, md);
//         return;
//     }
//     else
//     {
//         PmtTranslatorPtr translator = PmtTranslator::create(translatorName);
//         if (translator)
//         {
//             void* value = 0;
//             int count;
//             translator->assign(md.ptr(), value, (unsigned long&)count, false);
// 
//             bool bVector = PmtTranslator::isVector(PmtTranslator::typeID(md->trueType()));
// 
//             // DG added this check for count = 1, because EkTiff will throw an exception
//             // if you create entry as vector and later try to assign as non vector
//             if(count == 1)
//                 bVector = false;
// 
//             TiffTagEntry* entry = createTiffTagEntry(getTag(tag), getType(type), bVector);
// 
//             switch (entry->getType())
//             {
//                 case EKTIFF_ASCII:
//                 {
//                     SPmtString* pstr = (SPmtString*)(value);
//                     dynamic_cast<TiffTagEntryT<string>*>(entry)->setValue(pstr->c_str());
//                     dynamic_cast<TiffTagEntryT<string>*>(entry)->setCount(pstr->size()+1);
//                     break;
//                 }
//                 case EKTIFF_BYTE:
//                 case EKTIFF_UNDEFINED:
//                     setEntryData(entry, (char*)value, count);
//                     break;
//                 case EKTIFF_UBYTE:
//                     setEntryData(entry, (unsigned char*)value, count);
//                     break;
//                 case EKTIFF_SHORT:
//                     setEntryData(entry, (short*)value, count);
//                     break;
//                 case EKTIFF_USHORT:
//                     setEntryData(entry, (unsigned short*)value, count);
//                     break;
//                 case EKTIFF_LONG:
//                     setEntryData(entry, (long*)value, count);
//                     break;
//                 case EKTIFF_ULONG:
//                     setEntryData(entry, (unsigned long*)value, count);
//                     break;
//                 case EKTIFF_RATIONAL:
//                 case EKTIFF_URATIONAL:
//                 case EKTIFF_FLOAT:
//                     setEntryData(entry, (float*)value, count);
//                     break;
//                 case EKTIFF_DOUBLE:
//                     setEntryData(entry, (double*)value, count);
//                     break;
//                 default:
//                     SPmtString msg = "PmtPngAccessor::setMdValue: invalid TIFF type ";
//                     msg += type;
//                     throw PmtError(PMT_ERROR, msg, WHERE);
//             }
// 
//             setTagValue(loc, *entry);
//             // cast to (char*) to get rid of g++ warning
//             delete (char*)value;
//             if (entry)
//                 delete entry;
//         }
//         else
//         {
//             char m[100];
//             sprintf(m, "PmtExifAccessor::setMdValue Can't create translator for %s", 
//                 translatorName.c_str());
//             throw PmtError(PMT_TRANSLATOR_NOT_FOUND, m, WHERE);
//         }
// 
//     }
}


// TiffTagEntry* PmtPngAccessor::getTagValue( const SPmtString& loc, unsigned short tagNumber) const
// {
//     TiffImageFile::DirPath dirPath = getDirPath(loc);
//     TiffTagEntry* tte = mImgFile->getGenericTag(tagNumber, dirPath);
//     if (!tte)
//     {
//         char msg[100];
//         sprintf(msg, "PmtEkTiffAccessor::getTagValue Error getting tag %d in IFD %s from tiff file", tagNumber, loc.c_str());
//         throw PmtError(PMT_TIFF_FORMAT_ENTRY_NO_TAG_IN_FILE, msg, WHERE);
//     }
//     
//     return tte;
// }
// 
// void PmtPngAccessor::setTagValue( const SPmtString& loc, const TiffTagEntry& tagEntry )
// {
//     TiffImageFile::DirPath dirPath = getDirPath(loc);
//     try
//     {
//         mImgFile->setGenericTag(tagEntry, dirPath);
//     }
//     catch (TiffError& e)
//     {
//         char msg[300];
//         sprintf(msg, "PmtEkTiffAccessor::setTagValue - can not set tag %d in IFD %s in TIFF file : %s",
//             tagEntry.getTagNum(), loc.c_str(), e.getMsg());
//         throw PmtError(PMT_TIFF_FORMAT_ENTRY_NO_TAG_IN_FILE, msg, WHERE);
//     }
// }
// 
// 
// void PmtPngAccessor::readAudio(const SPmtString& loc, PmtMetadataPtr md) const
// {
//     bool status = true;
//     TiffImageFile::DirPath dirPath = getDirPath(loc);
//     TiffDirectory* dir = mImgFile->getDirectory(dirPath);
//     if (dir)
//     {
//         unsigned int size = dir->getAudioSize();
//         if (size)
//         {
//             unsigned char * buf = new unsigned char[size];
//             try
//             {
//                 status = dir->readAudio(buf, size) == size;
//             }
//             catch (TiffError& e)
//             {
//                 delete buf;
//                 throw e;
//             }
//             if (status)
//             {
//                 vector<unsigned char> vals;
//                  copy(buf, buf+size,
//                               back_insert_iterator< vector<unsigned char> >(vals));
//                 PmtMetadataT<vector<unsigned char> > *mdt = 
//                     dynamic_cast<PmtMetadataT<vector<unsigned char> >*>(md.ptr()) ;
//                 mdt->value() = vals;
//             }
//             delete buf;
//         }
//         else
//             status = false;
//     }
//     else
//         status = false;
// 
//     if (status == false)
//     {
//         throw PmtError(PMT_ERROR, "PmtEkTiffAccessor::readAudio - failed to read audio stream", WHERE);
//     }
// }
// 
// 
// void PmtPngAccessor::writeAudio(const SPmtString& loc, PmtMetadataPtr md) const
// {
//     TiffImageFile::DirPath dirPath = getDirPath(loc);
//     TiffDirectory* dir = mImgFile->getDirectory(dirPath);
//     if (!dir)
//     {
//         mImgFile->createDirectory(dirPath);
//         dir = mImgFile->getDirectory(dirPath);
//     }
// 
//     const vector<unsigned char>& data = (dynamic_cast<PmtMetadataT<vector<unsigned char> >*>(md.ptr()))->value();
//     int size = data.size();
//     unsigned char *buf = new unsigned char[size];
//     
//     PmtMetadataT<vector<unsigned char> > *mdt = 
//         dynamic_cast<PmtMetadataT<vector<unsigned char> >*>(md.ptr()) ;
//     vector<unsigned char> vals = mdt->value();
//      copy(vals.begin(), vals.end(), buf);
// 
//     try
//     {
//         dir->writeAudio(buf, size);
//     }
//     catch (TiffError& e)
//     {
//         delete buf;
//         throw e;
//     }
// 
//     delete buf;
// }
// 
// TiffTagEntry* PmtPngAccessor::createTiffTagEntry(unsigned short tag, unsigned short type, bool bVector)
// {
//     TiffTagEntry* entry = NULL;
//     switch (type)
//     {
//         case EKTIFF_UBYTE:
//             if (bVector)
//                 entry = new TiffTagEntryT<vector<unsigned char> >(tag, type);
//             else
//                 entry = new TiffTagEntryT<unsigned char>(tag, type);
//             break;
//         case EKTIFF_ASCII:
//             entry = new TiffTagEntryT<string>(tag, type);
//             break;
//         case EKTIFF_USHORT:
//             if (bVector)
//                 entry = new TiffTagEntryT<vector<unsigned short> >(tag, type);
//             else
//                 entry = new TiffTagEntryT<unsigned short>(tag, type);
//             break;
//         case EKTIFF_ULONG:
//             if (bVector)
//                 entry = new TiffTagEntryT<vector<unsigned long> >(tag, type);
//             else
//                 entry = new TiffTagEntryT<unsigned long>(tag, type);
//             break;
//         case EKTIFF_URATIONAL:
//             if (bVector)
//                 entry = new TiffTagEntryT<vector<float> >(tag, type);
//             else
//                 entry = new TiffTagEntryT<float>(tag, type);
//             break;
//         case EKTIFF_BYTE:
//         case EKTIFF_UNDEFINED:
//             if (bVector)
//                 entry = new TiffTagEntryT<vector<char> >(tag, type);
//             else
//                 entry = new TiffTagEntryT<char>(tag, type);
//             break;
//         case EKTIFF_SHORT:
//             if (bVector)
//                 entry = new TiffTagEntryT<vector<short> >(tag, type);
//             else
//                 entry = new TiffTagEntryT<short>(tag, type);
//             break;
//         case EKTIFF_LONG:
//             if (bVector)
//                 entry = new TiffTagEntryT<vector<long> >(tag, type);
//             else
//                 entry = new TiffTagEntryT<long>(tag, type);
//             break;
//         case EKTIFF_RATIONAL:
//         case EKTIFF_FLOAT:
//             if (bVector)
//                 entry = new TiffTagEntryT<vector<float> >(tag, type);
//             else
//                 entry = new TiffTagEntryT<float>(tag, type);
//             break;
//         case EKTIFF_DOUBLE:
//             if (bVector)
//                 entry = new TiffTagEntryT<vector<double> >(tag, type);
//             else
//                 entry = new TiffTagEntryT<double>(tag, type);
//             break;
//     }
//     return entry;
// }
// 
// 
// TiffImageFile::DirPath PmtPngAccessor::getDirPath(const SPmtString& loc) const
// {
//     TiffImageFile::DirPath dirPath;
//     if (loc == "IFD_MAIN")
//     {
//         dirPath.push_back(TiffImageFile::DirPath::value_type(0, 0));
//     }
//     else if (loc == "IFD_SUB")
//     {
//         dirPath.push_back(TiffImageFile::DirPath::value_type(0, 0));
//         dirPath.push_back(TiffImageFile::DirPath::value_type(TIFFTAG_SUBIFD, 0));
//     }
//     else if (loc == "IFD_EXIF")
//     {
//         dirPath.push_back(TiffImageFile::DirPath::value_type(0, 0));
//         dirPath.push_back(TiffImageFile::DirPath::value_type(TIFFTAG_EXIFIFD, 0));
//     }
//     else if (loc == "IFD_SOUND")
//     {
//         dirPath.push_back(TiffImageFile::DirPath::value_type(0, 0));
//         dirPath.push_back(TiffImageFile::DirPath::value_type(TIFFTAG_SOUNDIFD, 0));
//     }
//     else if (loc == "IFD_GPS")
//     {
//         dirPath.push_back(TiffImageFile::DirPath::value_type(0, 0));
//         dirPath.push_back(TiffImageFile::DirPath::value_type(TIFFTAG_GPSIFD, 0));
//     }
//     else if (loc == "IFD_EXIF_INTER")
//     {
//         dirPath.push_back(TiffImageFile::DirPath::value_type(0, 0));
//         dirPath.push_back(TiffImageFile::DirPath::value_type(TIFFTAG_EXIFIFD, 0));
//         dirPath.push_back(TiffImageFile::DirPath::value_type(TIFFTAG_INTERIFD, 0));
//     }
//     else if (loc == "IFD_KPRO")
//     {
//         dirPath.push_back(TiffImageFile::DirPath::value_type(0, 0));
//         dirPath.push_back(TiffImageFile::DirPath::value_type(33424, 0));
//     }
//     else
//     {
//         SPmtString m = "PmtPngAccessor::getDirPath: Unrecognized TIFF location ";
//         m += loc;
//         throw PmtError(PMT_TIFF_FORMAT_ENTRY_BAD_TAG_LOCATION, m, WHERE);
//     }
//     return dirPath;
// }
// 
// 
// void PmtPngAccessor::getFormatInfo(PmtMetadataPtr md, SPmtString& loc, SPmtString& tag, SPmtString& type, SPmtString& translator)
// {
//     PmtKeyType key = md->fullKey(false);
//     EK_GUARD(EK_DEFAULT_MUTEX, msLock);
//     PmtTranslationTableMap::const_iterator te = msTranslationTable.find(key);
//     if (te == msTranslationTable.end())
//     {
//         SPmtString msg = "PmtPngAccessor::getFormatInfo: no translation info for key ";
//         msg += key;
//         throw PmtError(PMT_TRANSLATION_INFO_NOT_FOUND, msg, WHERE);
//     }
//     DOM_Node entryNode = (*te).second;
//     DOM_NodeList nodeList = entryNode.getChildNodes();
//     DOM_Node tiffNode;
//     for (unsigned int i=0; i<nodeList.getLength(); i++)
//     {
//         DOM_Node p = nodeList.item(i);
//         PmtDomString pmtStr(p.getNodeName());
//         if (strcmp(pmtStr.getAsciiString(), PMT_TIFF_ELEMENT_NAME) == 0)
//         {
//             tiffNode = p;
//             break;
//         }
//     }
//     
//     if (tiffNode.isNull())
//     {
//         SPmtString m = "PmtPngAccessor::getFormatInfo: no TIFF format info found for key ";
//         m += key;
//         PmtError err(PMT_TIFF_FORMAT_ENTRY_NOT_FOUND, m, WHERE);
//         if ( mThrowErrors )
//             throw err ;
//         else
//             mErrors.push_back( err ) ;
//     }
//     else
//     {
//         DOM_NamedNodeMap attrMap = tiffNode.getAttributes();
//         PmtDomString pLoc(attrMap.getNamedItem(PMT_XML_ATTR_LOC).getNodeValue());
//         PmtDomString pTag(attrMap.getNamedItem(PMT_XML_ATTR_TAG).getNodeValue());
//         PmtDomString pType(attrMap.getNamedItem(PMT_XML_ATTR_TYPE).getNodeValue());
//         PmtDomString pTran(attrMap.getNamedItem(PMT_XML_ATTR_TRANSLATOR).getNodeValue());
//         loc = pLoc.getString();
//         tag = pTag.getString();
//         type = pType.getString();
//         translator = pTran.getString();
//     }
// }
// 
// 
// unsigned short PmtPngAccessor::getTag(const SPmtString& stag) const
// {
//     return (unsigned short)(atoi(stag.c_str()));
// }
// 
// 
// unsigned short PmtPngAccessor::getType(const SPmtString& stype) const
// {
//     if (stype == "ascii")
//         return EKTIFF_ASCII;
//     else if (stype == "byte")
//         return EKTIFF_BYTE;
//     else if (stype == "ubyte")
//         return EKTIFF_UBYTE;
//     else if (stype == "short")
//         return EKTIFF_SHORT;
//     else if (stype == "ushort")
//         return EKTIFF_USHORT;
//     else if (stype == "long")
//         return EKTIFF_LONG;
//     else if (stype == "ulong")
//         return EKTIFF_ULONG;
//     else if (stype == "rational")
//         return EKTIFF_RATIONAL;
//     else if (stype == "urational")
//         return EKTIFF_URATIONAL;
//     else if (stype == "float")
//         return EKTIFF_FLOAT;
//     else if (stype == "double")
//         return EKTIFF_DOUBLE;
//     else if (stype == "undefined")
//         return EKTIFF_UNDEFINED;
//     else if (stype == "audio")
//         return EKTIFF_UNDEFINED;
//     else
//     {
//         SPmtString m = "PmtPngAccessor::getType: invalid TIFF type for ";
//         m += stype;
//         throw PmtError(PMT_TIFF_FORMAT_ENTRY_BAD_TAG_TYPE, m, WHERE);
//     }
// }

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED_NOTREADYYET
// This method checks to see if we have an open file and if so returns an
// image accessor for it. Otherwise, it returns NULL
PmtImageAccessor *PmtPngAccessor::getImageAccessor()
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

#endif // PMT_PNG_ACCESSOR

