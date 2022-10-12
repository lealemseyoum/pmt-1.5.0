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
 * George Sotak    george.sotak@kodak.com
 * 
 * Contributor(s): 
 * Ricardo Rosario  ricardo.rosario@kodak.com
*/

#ifndef PMT_ACCESSOR_H
#define PMT_ACCESSOR_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#if (defined _MSC_VER) && (!defined _WINDOWS_) && (!defined __AFXWIN_H__)
#include "windows.h"
#endif
#include <list>
#include "EkCompiler.h"
#include "EkSmartPtr.h"
#include "EkRefCount.h"
#include "EkRational.h"
#if !defined(EK_USE_LOCKS) && !defined(EK_NOUSE_LOCKS)
#define EK_USE_LOCKS
#endif
#include "EkLocking.h"
#include "EkStrings.h"
#include "PmtAccrImplDefs.h"
#include "PmtCommon.h"
#include "PmtError.h"
#include "PmtMetadata.h"
#include "PmtTranslationTable.h"

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED
#include "PmtImageAccessor.h"
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED
#ifdef PMT_SOUND_ACCESSOR_REQUIRED
#include "PmtSoundAccessor.h"
#endif                          // PMT_SOUND_ACCESSOR_REQUIRED

// Put some forward class references
class PmtAccessor;

// Smart pointers
typedef EkSmartPtr < PmtAccessor > PmtAccessorPtr;
typedef EkConstSmartPtr < PmtAccessor > PmtConstAccessorPtr;

#ifdef PMTCORE_EXPLICIT
PMTACCR_IMPL_TMPL_EXT template class PMTACCR_IMPL_DECL EkConstSmartPtr <
    PmtAccessor >;
PMTACCR_IMPL_TMPL_EXT template class PMTACCR_IMPL_DECL EkSmartPtr <
    PmtAccessor >;
// Both of these are for the EkFactory class
PMTACCR_IMPL_TMPL_EXT template class PMTACCR_IMPL_DECL
EK_DLL_MAP(PmtImageFileFormatName, PmtAccessorPtr);
PMTACCR_IMPL_TMPL_EXT template class PMTACCR_IMPL_DECL EkFactoryT <
    PmtImageFileFormatName, PmtAccessorPtr >;
#endif


//! The PmtAccessor is the base class for accessor implementations
/*! An Accessor's primary responsibility is to provide applications
with read / write access to stored metadata. The metadata may be
stored in an image file, in a file system through the metadata
persistence mechanism, in a database (relational or object),
etc. Initial considerations have been limited to image file
access. PmtAccessor is the base class from which all other accessors
will be derived. Prior to using an accessor, the application must
ensure that the metadata definitions have been loaded via the
LogicalDefinitionInterpreter.
 
The PmtAccessor class keeps a factory object where all derived
accessors are registered. It provides a static factory method
getAccessor() for creating a specific accessor to access the given
file. All derived accessors must use the EkFactory
mechanism (EK_PROVIDE, EK_REQUIRE) to register itself in the factory
object.
*/
class PMTACCR_IMPL_DECL PmtAccessor:public EkRefCount < EK_DEFAULT_MUTEX >
{
  public:
    inline virtual ~ PmtAccessor()
    {
    }

    //! This function returns the throwError flag.
    inline const bool & throwErrors() const
    {
        return mThrowErrors;
    }

    //! Provides the application control over how metadata access errors are handled
    /*! If throwErrors is set true, then an error will be thrown if
       there is any  problem accessing the requested metadata. If set false,
       no errors will be thrown. The \b default setting is \b false.
     */
    inline bool & throwErrors()
    {
        return mThrowErrors;
    }

    //! Check if any error is thrown and suppressed.
    inline bool isErrorThrown() const
    {
        return mErrors.empty();
    }

    //! Print the messages of the suppressed errors
    void printErrorsSuppressed(ostream & c = cout) const;

    //! Return the format name of the file to be accessed.
    inline const PmtImageFileFormatName & formatName() const
    {
        return mFormatName;
    }

    //! create an instance of the specified accessor.
    /*! Allows the instantiation of a specific accessor implementation
       without needing the include the implementations header file.
     */
    static PmtAccessorPtr create(const PmtImageFileFormatName & accrId);

    //! create an instance of the specified accessor.
    /*! Allows the instantiation of a specific accessor implementation
       without needing the include the implementations header file.
     */
    static PmtImageFileFormatName recognizeFilename(const SPmtString fileName);

    //!Read all existing metadata and return a root metadata.
    // Note that the subclass can override it to provide a more efficient
    // implementation. 
    virtual PmtMetadataPtr readMetadata(const SPmtString nsURI = "");

    //! Read the specified metadata from the storage format.
    virtual PmtMetadataPtr readMetadata(PmtMetadataPtr & md);

    //! Read the specified metadata from the storage format.
    PmtMetadataIterator & readMetadata(PmtMetadataIterator & mdIter);


    //@{
    /** Request the accessor to write the given metadata to the storage media.

        The argument is a pointer to an instance of either an
        PmtMetadata (for a single metadata item) or an
        PmtCompositeMetadata (for multiple metadata items).  If
        throwErrors() is set to false and an error occurs, one of two
        things will happen. First, if the write is for a single
        metadata, then the write will just be aborted. Second, if the
        write is for a composite, then the write of the current
        metadatum is aborted and the writing of the composite contiues
        with the next element. If throwErrors() is set to true, then
        the write operation is always aborted by throwing the error.
        If bWriteValid is false then only the given metadata
        (including its childern) will be written; otherwise, the whole
        metadata tree starting from the out-most parent will be
        written.
        */

    virtual void writeMetadata(const PmtMetadataPtr & md, bool bWriteValid =
                               false);

    void writeMetadata(PmtMetadataIterator & md, bool bWriteValid = false);
    //@}

    /*! Remove metadata from a file


     */
    virtual void removeMetadata(const PmtMetadataPtr & md);
 
    /*! Open a file.
    
        This is a pure virtual function that is to be implemented by
        the derived classes.
    */
    virtual bool open(const char *name, PmtOpenMode mode = PMT_READONLY) = 0;

    //! Close the file. 
    /*!This is a pure virtual function that is to be implemented by
       the derived classes. */
    virtual void close()
    {
        mErrors.clear();
    }

    //! This method creates and returns the proper accessor implementation for the given file.
    static PmtAccessorPtr getAccessor(const char *fileName, PmtOpenMode mode =
                                      PMT_READONLY);

    //! This method returns a copy of the accessor. 
    virtual PmtAccessorPtr clone() const = 0;

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED
    //! This method returns the image accessor if it is implemented in the
    // child class. Otherwise, it returns NULL.
    virtual PmtImageAccessor *getImageAccessor()
    {
        return NULL;
    }
#endif                          // PMT_IMAGE_ACCESSOR_REQUIRED

#ifdef PMT_SOUND_ACCESSOR_REQUIRED
    //! This method returns the sound accessor if it is implemented in the
    // child class. Otherwise, it returns NULL.
    virtual PmtSoundAccessorPtr getSoundAccessor()
    {
        return NULL;
    }
#endif                          // PMT_SOUND_ACCESSOR_REQUIRED

	static inline PmtTranslationTablePtr getTranslationTable()
	{ return msTranslationTable; }

  protected:
  
    //! The translation table
    static PmtTranslationTablePtr msTranslationTable;

    PmtImageFileFormatName mFormatName;

    SPmtString mFileName;

    bool mThrowErrors;

    vector < PmtError > mErrors;

    //! The constructor is protected to ensure that only derived classes can call it.
    inline PmtAccessor(const PmtImageFileFormatName &
                       fName):mFormatName(fName), mThrowErrors(false)
    {
    }

    //! Initializes the translation table from the compiled in default.
    void load();

    //! Initializes the translation table from the supplied file.
    void load(const SPmtString & filename);

    //! Initialized the translation table from the supplied in-memory argument.
    void loadMemory(const char *translationTable);

    static void erase();

    /*! 
        Get the value from the underlying representation in the accessor
        implementation (ie, the image file) and set it to the Metadata object
        passed in
     */
    virtual bool getMdValue(PmtMetadataPtr & md) = 0;

    /*! 
        Set the value in the Metadata object to the underlying representation
        in the accessor implementation (ie, the image file)
     */
    virtual bool setMdValue(const PmtMetadataPtr & md) = 0;

    /*! Remove the metadata specified by md from the file.

     */
    virtual void removeMdValue(const PmtMetadataPtr & md)
    {
    }

    /*! Used to load in the translation tables if needed.
    
     */
    void loadIfAny();

  private:

    static bool msTranslationTableLoaded;

    inline PmtAccessor()
    {
    }
    inline PmtAccessor(const PmtAccessor &)
    {
    }
    inline PmtAccessor & operator=(PmtAccessor &)
    {
        return *this;
    }
};


#endif                          // PMT_ACCESSOR_H

