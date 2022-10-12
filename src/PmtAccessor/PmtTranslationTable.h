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
 * Creation Date: June 2003
 *
 * Original Author: 
 * Sam Fryer <samuel.fryer@kodak.com>
 *
 * Contributor(s):
 *
*/

// This is all based on something like the following
// sample XML entry:
/* 
<TRANSLATION> 
<ENTRY Key="SceneContent.ImageDescription" uri="http://metadata.kodak.com/SceneContent/ImageDescription" >
 <EXIF Tag="270" Type="ascii" Location="APP1_IFD0" Translator="builtin2ascii"/>
 <TIFF Tag="270" Type="ascii" Location="IFD_MAIN" Translator="builtin2string"/>
</ENTRY>
</TRANSLATION>
*/

#ifndef PMT_TRANSLATION_TABLE_H
#define PMT_TRANSLATION_TABLE_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#if (defined _MSC_VER) && (!defined _WINDOWS_) && (!defined __AFXWIN_H__)
#include "windows.h"
#endif

#include "EkCompiler.h"
#include "EkSmartPtr.h"
#include "EkRefCount.h"

//! The structure of an element in the translation table.  
class PmtTranslationTableElement
{
    public:
    
        //! The constructor.
        PmtTranslationTableElement(const char *key,
                                   const char *uri, 
                                   const char *tag, 
                                   const char *type, 
                                   const char *location, 
                                   const char *translator)
        :mKey(key),mUri(uri),mTag(tag),mType(type),mLocation(location),
         mTranslator(translator) 
        {};
        
        //! A default deconstructor
        ~PmtTranslationTableElement(){};

        //! The MDL key that this pertains to.
        SPmtString mKey;
 
		//! The MDL URI that this pertains to.
        SPmtString mUri;
        
        //! The tag, type and location values are accessor dependent
        // (i.e., the accessor determines what to use them for...)
        SPmtString mTag;
        SPmtString mType;
        SPmtString mLocation;
        
        //! What translator to use for translating from accessing type to MDL type
        SPmtString mTranslator;
        
    private:
    
        //! A private constructor.
        PmtTranslationTableElement();
};

//! A vector of elements is a column in the table.
typedef vector < PmtTranslationTableElement * > PmtTranslationTableElements;

//! Mapping a type to vector finishes the creation of a complete translation table.
typedef EK_DLL_MAP(SPmtString, PmtTranslationTableElements *) 
                                                       PmtTranslationTables;

// This is the structure that define how the default 
// translation table is stored after being pre-processed
// by the perl script.  Any change here needs to be 
// actually made in that script.
struct CharPmtTT
{
    const char * key;       // The metadata key
    const char * uri;       // The metadata key URI
    const char ** elements; // The TT elements.

    // A default constructor for the array below
    CharPmtTT( const char * theKey = NULL,
               const char * theURI = NULL,
               const char ** theElements = NULL ) 
    : key(theKey),uri(theURI),elements(theElements)
    {};
} ;

//! The general translation table class interface.
class PMTCORE_DECL PmtTranslationTable : public EkRefCount < EK_DEFAULT_MUTEX >
{
    public:
        
        //! Constructors
        PmtTranslationTable()
        {
        };
        
        //! Deconstructors
        ~PmtTranslationTable()
        {
            this->erase(false);
        }

        //! Initializes the translation table from the compiled in default.
        void load();

        //! Initializes the translation table from the supplied file.
        void load(const SPmtString & filename);

        //! Initializes the translation table from the supplied in-memory table.
        void loadMemory(const char *translationTable);

        //! Erase the table
        inline void erase(bool doLock = true)
        {
			// Due to MS deleting the lock before deleting this, when
			// this is deleted, we assume that we don't have anyone waiting
			// for us and ignore the lock.  This is not a problem until this
			// this class is instantiated as anything but a static.
			if (doLock)
			{
                EK_GUARD(EK_DEFAULT_MUTEX, msLock);
                eraseTables();
			}
			else
			{
                eraseTables();
			}
        }
        
        //! Get the sub-table
        inline PmtTranslationTableElements * getSubTable(SPmtString formatType)
        {
            PmtTranslationTableElements * theTable = NULL;
            PmtTranslationTables::iterator theTableIterator = 
                                             mTranslationTable.find(formatType);
            if (theTableIterator != mTranslationTable.end())
                theTable = (*theTableIterator).second;
            return theTable;            
        }
 
		static const CharPmtTT * getDefaultTranslationTable();
 
 //These should be private!!!!!  
 // But they aren't do to compatibility issues with current code.
 // TBD: Change this!
 
        //! The table lock (this should NOT be static!)
        static EK_DEFAULT_MUTEX msLock;
    
        //! The translation tables
        PmtTranslationTables mTranslationTable;
    
    private:
        
        inline void eraseTables()
		{
            PmtTranslationTables::iterator theTableIterator = 
                                              mTranslationTable.begin();
            while (theTableIterator != mTranslationTable.end())
            {
                PmtTranslationTableElements * theTable = 
                                                 (*theTableIterator).second;
                unsigned int i = 0;
                for (i = 0;i<theTable->size();i++)
                    delete (*theTable)[i];
                delete theTable;
                theTableIterator++;
            }
            mTranslationTable.clear();
		}

 
};


//! Smart pointers
typedef EkSmartPtr < PmtTranslationTable > PmtTranslationTablePtr;
typedef EkConstSmartPtr < PmtTranslationTable > PmtConstTranslationTablePtr;

#endif
