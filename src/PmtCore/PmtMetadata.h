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
 * Sam Fryer      samuel.fryer@kodak.com
 *
 */

#ifndef PMT_METADATA_H
#define PMT_METADATA_H

#ifdef _MSC_VER
#pragma once
#pragma warning (disable:4231)
#endif

#include <list>
#include <typeinfo>

#include "EkCompiler.h"
#include "EkRational.h"
#include "EkRefCount.h"
#include "EkSmartPtr.h"
#include "EkStrings.h"

#if !defined(EK_USE_LOCKS) && !defined(EK_NOUSE_LOCKS)
#define EK_USE_LOCKS
#endif

#include "EkLocking.h"

#include "PmtCommon.h"
#include "PmtCoreDefs.h"
#include "PmtError.h"
#include "PmtAliases.h"
#include "PmtAttribute.h"

// This is the default root of all metadata URIs.
// They can be changed via the Translation Table
// for specific URIs not using our names.
#define PMT_ROOT_URI "http://ns.kodak.com/pmt/1.0/"

/*
 * Convenience Macros: Used by PmtMedata implementation to register themselves
 * with the PmtMetadata factory. See also the EK_PROVIDE and EK_REQUIRE macros.
 */

// #define PMT_METADATA_TYPE(facKey, mdType) PmtMetadata::getFactory().addEntry( facKey, new mdType("", facKey) )
// #define PMT_METADATA_TYPE_INIT(facKey, mdType, initVal) PmtMetadata::getFactory().addEntry( facKey, new mdType("", facKey, initVal) )

class PmtCompositeMetadata;
class PmtMetadata;
class PmtMetadataVisitor;
class PmtMdVisitorImpl;
class PmtRootMetadata;
class PmtMetadataFactory;
class PmtMetadataFactories;
class PmtMetadataIterator;

typedef EkSmartPtr < PmtMetadata > PmtMetadataPtr;
typedef EkConstSmartPtr < PmtMetadata > PmtConstMetadataPtr;

#ifdef PMTCORE_EXPLICIT
PMTCORE_TMPL_EXT template class PMTCORE_DECL EkConstSmartPtr < PmtMetadata >;
PMTCORE_TMPL_EXT template class PMTCORE_DECL EkSmartPtr < PmtMetadata >;
// Both of these are for the EkFactory class
PMTCORE_TMPL_EXT template class PMTCORE_DECL EK_DLL_MAP(PmtKeyType,
                                                        PmtMetadataPtr);
PMTCORE_TMPL_EXT template class PMTCORE_DECL EkFactoryT < PmtKeyType,
    PmtMetadataPtr >;
#endif



//! PmtMetadataIterator is provided for iterating over multiple metadata instances
/*! PmtMetadataIterator instances are returned from methods that need to return
  a reference to metadata instances. This is a fairly basic interator class. Internal
  pointers are reset with the start() method, which also return a reference to the
  first metadata instance. Additional references to metadata instances can be had
  by calling the next() method. The next() will return a NULL reference when the
  end of the list is reached.

  Example usage:

  \code
  PmtMetadataIterator iter = rootMd->getMetadata( "*" ) ;
  PmtMetadataPtr child = iter.start() ;
  while( child != NULL )
  {
    // do something with child..

    // now get the next metadata instance
    child = iter.next() ;
  }
  \endcode
*/
class PMTCORE_DECL PmtMetadataIterator
{
  public:
    typedef list < PmtMetadataPtr > PmtMetadataList;
    typedef PmtMetadataList::iterator PmtMdIter;

    //! The default constructor
    inline PmtMetadataIterator()
    {
    }

    //! The copy constructor
    inline PmtMetadataIterator(const PmtMetadataIterator & src):mMdList(src.
                                                                        mMdList),
        mCrntIter(src.mCrntIter)
    {
    }

//     //! The destructor
//     inline ~ PmtMetadataIterator()
//     {
//     }

    //! The assignment operator
    inline PmtMetadataIterator & operator=(const PmtMetadataIterator & rhs);

    //! This returns the beginning element of the iterator
    inline PmtMetadataPtr start();
    //! This returns the next element in the iterator
    inline PmtMetadataPtr next();

    //! This returns the number of elements in the iterator
    inline int size()
    {
        return mMdList.size();
    }

    //! This clears the interator
    inline void clear();

    //! This shows the metadata in the iterator
    void show(bool showObjectMemoryPointers = false);

    //! This checks the iterator
    inline PmtMetadataPtr checkIter(PmtMdIter & iter);

    //! This inserts a new metadata ptr into the metadata list
    inline void insert(PmtMetadataPtr & mdPtr);

  private:
    PmtMetadataList mMdList;
    PmtMdIter mCrntIter;
};


// This is an enumeration of the true types. It is used for determining
// how to cast a PmtMetadata to a PmtMetadataT< true type >.
// Note: vector types MUST IMMEDIATELY follow the corresponding simple types.
enum PmtMDType
{
    PMT_MD_TYPE_UNKNOWN,
    PMT_MD_TYPE_COMPOSITE,
    PMT_MD_TYPE_BOOL,
    PMT_MD_TYPE_VBOOL,
    PMT_MD_TYPE_INT8,
    PMT_MD_TYPE_VINT8,
    PMT_MD_TYPE_UINT8,
    PMT_MD_TYPE_VUINT8,
    PMT_MD_TYPE_INT16,
    PMT_MD_TYPE_VINT16,
    PMT_MD_TYPE_UINT16,
    PMT_MD_TYPE_VUINT16,
    PMT_MD_TYPE_INT32,
    PMT_MD_TYPE_VINT32,
    PMT_MD_TYPE_UINT32,
    PMT_MD_TYPE_VUINT32,
    PMT_MD_TYPE_FLOAT,
    PMT_MD_TYPE_VFLOAT,
    PMT_MD_TYPE_DOUBLE,
    PMT_MD_TYPE_VDOUBLE,
    PMT_MD_TYPE_STRING,
    PMT_MD_TYPE_VSTRING,
    PMT_MD_TYPE_WSTRING,
    PMT_MD_TYPE_VWSTRING
};

//! PmtMetadata is the base class for all potental metadata specializations.
/*! PmtMetadata is the base metadata representation class. The common 
 metadata functionality is contained in this class. A metadata object
 primarily contains two pieces of information: a key and a value.

 The metadata key is an ASCII, case sensitive character string that provides
 unique identification of the metadata. When instantiating a metadata object,
 an application may choose a metadata key that is defined in the
 PmtDefaultDefinitions.xsd file int the DefaultDefintions directory. 

 The metadata value is stored in a derived metadata class (PmtMetadataT),
 but may be accessed from the base class (with some limitations).
*/
class PMTCORE_DECL PmtMetadata:public EkRefCount < EK_DEFAULT_MUTEX >
{
  public:
    //! This is the default constructor
    inline PmtMetadata():mParent(NULL), mMetadataKey(""), mMetadataURI(""), mType(""),
        mMDType(PMT_MD_TYPE_UNKNOWN),mNillable("false"), mIsChanged(false),
        mOccurrence(1), mMultipleOccurrence(false),
        mBriefDocumentation(NULL),mFullDocumentation(NULL)
    {
    }

    //! Constructor ... the class is abstract so these are protected
    inline PmtMetadata(const PmtKeyType & mdKey, const SPmtString & _type,
                       const SPmtString & trueType)
       :mParent(NULL),
        mMetadataKey(mdKey), mMetadataURI(""), mType(_type), mNillable("false"),
        mIsChanged(false), mOccurrence(1), mMultipleOccurrence(false),
        mBriefDocumentation(NULL),mFullDocumentation(NULL)
    {
        mMDType = getType(trueType);
    }

    //! Constructor ... the class is abstract so these are protected
    inline PmtMetadata(const PmtKeyType & mdKey, const SPmtString & _type,
                       const SPmtString & trueType, const SPmtString & nillable)
       :mParent(NULL),
        mMetadataKey(mdKey), mMetadataURI(""), mType(_type), mNillable(nillable),
        mIsChanged(false), mOccurrence(1), mMultipleOccurrence(false),
        mBriefDocumentation(NULL),mFullDocumentation(NULL)
    {
        mMDType = getType(trueType);
    }

    //! Constructor ... the class is abstract so these are protected
    inline PmtMetadata(const PmtKeyType & mdKey, const SPmtString & _type,
                       PmtMDType mdType)
       :mParent(NULL),
        mMetadataKey(mdKey), mMetadataURI(""), mType(_type), mMDType(mdType),
        mNillable("false"), mIsChanged(false), mOccurrence(1),
        mMultipleOccurrence(false),
        mBriefDocumentation(NULL),mFullDocumentation(NULL)
    {
    }

    //! Constructor ... the class is abstract so these are protected
    inline PmtMetadata(const PmtKeyType & mdKey, const SPmtString & _type,
                       PmtMDType mdType, const SPmtString & nillable)
       :mParent(NULL),
        mMetadataKey(mdKey), mMetadataURI(""),
		mType(_type), mMDType(mdType),
        mNillable(nillable), mIsChanged(false), mOccurrence(1),
        mMultipleOccurrence(false),
        mBriefDocumentation(NULL),mFullDocumentation(NULL)
    {
    }

    //! Copy Constructor ... don't copy parent
    inline PmtMetadata(const PmtMetadata & orig):mParent(0),
        mMetadataKey(orig.mMetadataKey), mMetadataURI(orig.mMetadataURI), 
		mType(orig.mType),
        mMDType(orig.mMDType), mNillable(orig.mNillable),
        mIsChanged(orig.mIsChanged), mAttributeMap(orig.mAttributeMap),
        mOccurrence(orig.mOccurrence),
        mMultipleOccurrence(orig.mMultipleOccurrence),
        mBriefDocumentation(orig.mBriefDocumentation),
        mFullDocumentation(orig.mFullDocumentation)
    {
    }

    //! Copy Constructor
    inline PmtMetadata(const PmtMetadata & orig,
                       PmtCompositeMetadata * theParent):mParent(theParent),
        mMetadataKey(orig.mMetadataKey), mMetadataURI(orig.mMetadataURI), 
		mType(orig.mType),
        mMDType(orig.mMDType), mNillable(orig.mNillable),
        mIsChanged(orig.mIsChanged), mAttributeMap(orig.mAttributeMap),
        mOccurrence(orig.mOccurrence),
        mMultipleOccurrence(orig.mMultipleOccurrence),
        mBriefDocumentation(orig.mBriefDocumentation),
        mFullDocumentation(orig.mFullDocumentation)
    {
    }

    //! Base class virtual destructor, does nothing
    inline virtual ~ PmtMetadata()
    {
    }

    //! This is an assignment operator
    PmtMetadata & operator=(const PmtMetadata & orig);

    //! Returns the metadata's key.
    const PmtKeyType key(bool includeOccurrence = true) const;

    //! Returns the metadata's fully qualified key.
    const PmtKeyType fullKey(bool includeOccurrence = true) const;

	inline SPmtString getURI()
    {
		if (mMetadataURI.empty())
		{
			mMetadataURI = PMT_ROOT_URI;
			SPmtString::size_type pos = this->fullKey().find_first_of(".");
			SPmtString::size_type last = 0;
			while (pos != SPmtString::npos)
			{
				mMetadataURI += this->fullKey().substr(last,pos-last);
				last = pos+1;
				pos = this->fullKey().find_first_of(".",last);
			}
			mMetadataURI += this->fullKey().substr(last);
		}		
        return mMetadataURI;
    }
	inline void setURI(SPmtString uri) 
    {
        mMetadataURI = uri;
    }

    inline const unsigned int &occurrence() const
    {
        return mOccurrence;
    }

    //! Returns the metadata's value type as specified in the MDL file
    inline PmtKeyType type() const
    {
        return mType;
    }

    //! Returns the string representation of the C++ type (double, int, etc.).
    inline SPmtString trueType() const
    {
        return getType(mMDType);
    }

    //! Returns the PmtMDType enum representation of the MD type
    inline PmtMDType getMDType() const
    {
        return mMDType;
    }

    virtual const SPmtString & getNameSpace(void) const;

    //! Return a pointer to the parent composite metadata
    inline const PmtCompositeMetadata *parent() const
    {
        return mParent;
    }

    virtual PmtMetadataFactory *getMdFactory() const;
    virtual PmtAttrFactory *getAttrFactory() const;

    static PmtMetadataFactory *getFactory(const SPmtString & nameSpace);
    static PmtMetadataFactories & getFactories(void);

    //! Get a handle to the currently available aliases
    static PmtAliases & getAliases(void);

    //! Get the metadata attribute with the attribute name.
    virtual PmtAttributePtr getAttribute(PmtKeyType name,
                                         bool createIfNotExists = true);

    //! set the attribute value, if not already there in the attribute map.
    virtual void setAttribute(PmtKeyType name, PmtAttributePtr pmtAttrPtr);

    //! Get the metadata attributes.
    virtual const PmtAttributeMap getAttributes(bool createIfNotExists = true);

    //! Delete the specified attribute
    inline virtual void deleteAttribute(PmtKeyType name)
    {
        mAttributeMap.erase(name);
    }

    //! Delete all attribute
    inline virtual void deleteAllAttributes(void)
    {
        mAttributeMap.clear();
    }

    /*! Get a handle to a single metadatum instance with the given key

        The base class implementation just returns a NULL. The
        PmtCompositeMetadata class provides the real implementation.
     */
    inline virtual PmtMetadataPtr getMetadatum(const PmtKeyType & key,
                                         bool createMetadataIfNotExists = true)
    {
        return NULL;
    }

    /*! Get a PmtIterator instance referencing the requested metadata.
    
        The base class implementation just returns an empty iterator. The
        PmtCompositeMetadata class provides the real implementation.
     */
    inline virtual PmtMetadataIterator getMetadata(const PmtKeyType & key,
                           bool createMetadataIfNotExists = true,
                           const PmtEntryTypeEnum entryToSearch = PMT_ALL_KEYS)
    {
        return PmtMetadataIterator();
    }

    /*! Get a PmtIterator instance referencing the requested metadata.
    
        The base class implementation just returns an empty iterator. The
        PmtCompositeMetadata class provides the real implementation.
     */
    inline virtual PmtMetadataIterator getMetadata(const PmtKeyList & keyList,
                           bool createMetadataIfNotExists = true,
                           const PmtEntryTypeEnum entryToSearch = PMT_ALL_KEYS)
    {
        return PmtMetadataIterator();
    }

    //! Delete the metadata instances with the keys comprising the alias
    inline virtual void deleteMetadata(const PmtKeyType & key,
                            const PmtEntryTypeEnum entryToSearch = PMT_ALL_KEYS)
    {
        return;
    }

    //! Delete the metadata instances with the keys comprising the list
    inline virtual void deleteMetadata(const PmtKeyList & keyList,
                            const PmtEntryTypeEnum entryToSearch = PMT_ALL_KEYS)
    {
        return;
    }

    //! This method returns the value of the metadata
    virtual EkString getValueStr() const
    {
        return EK_L("");
    }

    //! This method sets the value of the metadata
    virtual void setValueStr(const EkString & valStr)
    {
        mIsChanged = true;
    }

    //! return: A "deep" copy of the object.
    virtual PmtMetadata *clone(void) const = 0;

    //! This method clones a PMTMetadataPtr
    virtual PmtMetadataPtr clone(PmtCompositeMetadata * parent) const = 0;

    //! This method creates a PmtMetadataPtr
    static PmtMetadataPtr create(const SPmtString & nameSpace = "");

    // Persistance Functions

    //! DEPRECIATED METHOD - Write metadata variables to a stream
    virtual ostream & print(ostream & c = cout, bool bFullKey = false) const;

    //! Returns true if the current class is a composite.
    // Always returns false in the PmtMetadata base class.
    inline virtual bool isComposite() const
    {
        return false;
    }

    //! Support for visitor pattern
    inline virtual void accept(PmtMetadataVisitor *)
    {
        return;
    }
    inline virtual void accept(PmtMdVisitorImpl *)
    {
        return;
    }

    inline virtual void show(bool showObjectMemoryPointers = false, 
                             int level = 0, bool showModelGroups = false)
    {
        cout << "Default show() for metadata item called." << endl;
    }

    inline virtual bool validate() const
    {
        return true;
    }

    virtual PmtRootMetadata *getRootMetadata(void) const;

    //! This method sets the parent
    inline void setParent(const PmtCompositeMetadata * p)
    {
        mParent = p;
    }

    inline unsigned int &occurrence()
    {
        return mOccurrence;
    }

    inline bool & multipleOccurrence()
    {
        return mMultipleOccurrence;
    }

    inline virtual void addFacet(const PmtKeyType & facetName,
                                 const vector < SPmtString > &value,
                                 bool fixed)
    {
    }

    inline const SPmtString & getNillable()
    {
        return mNillable;
    }

    inline virtual bool isChanged()
    {
        return mIsChanged;
    };

    //! DEPRECIATED - Write derived class values. 
    // Pure virtual, implemented in the derived class.
    virtual ostream & printDerived(ostream & c) const = 0;

    // Convert from a trueType (string) to a PmtMDType
    static PmtMDType getType(SPmtString trueType);
    // Convert from a PmtMDType to a trueType (string)
    static SPmtString getType(PmtMDType mdType);

    //! Return the brief documentation associated with this node
    inline const char * briefDocumentation()
    {
        return mBriefDocumentation;
    };

    //! Return the detailed documentation associated with this node
    inline const char * fullDocumentation()
    {
        return mFullDocumentation;
    };

    //! Set the brief documentation associated with this node
    inline void setBriefDocumentation(const char * doc)
    {
        mBriefDocumentation = doc;
    };

    //! Set the detailed documentation associated with this node
    inline void setFullDocumentation(const char * doc)
    {
        mFullDocumentation = doc;
    };

  protected:
    const PmtCompositeMetadata *mParent;

    PmtKeyType mMetadataKey;

	SPmtString mMetadataURI;

    SPmtString mType;

    // Enum representation of the MD type
    PmtMDType mMDType;

    SPmtString mNillable;

    // flag to check if the setValueStr() or the value() was called on 
    // this element. we check for this flag when writting out the instance.
    bool mIsChanged;

    PmtAttributeMap mAttributeMap;

    //! This method gets a list of keys
    static void getKeyList(const PmtKeyType & key, PmtKeyList & returnKeyList);

    void showAttr(int level) const;

    inline void setNillable(const SPmtString & nillable)
    {
        mNillable = nillable;
    }

  private:
    unsigned int mOccurrence;
    
    //! Can this object have more than 1 occurrence
    bool mMultipleOccurrence;   
    
    //! Brief documentation about what this contains
    const char * mBriefDocumentation;
    
    //! Detailed documentation about what this contains
    const char * mFullDocumentation;

};


////////////////////////////////
// iostream operation for PmtMetadata

PMTCORE_DECL istream & operator>>(istream & in, PmtMetadataPtr & md);
PMTCORE_DECL ostream & operator<<(ostream & out, const PmtMetadataPtr & md);


/////////////////////////////////////////////////////////////////
// Metadata Factories and Associated Items. 
/////////////////////////////////////////////////////////////////



class PMTCORE_DECL PmtMetadataFactory:public EkFactoryT < PmtKeyType,
    PmtMetadataPtr >
{
  public:
    inline PmtMetadataFactory():EkFactoryT < PmtKeyType, PmtMetadataPtr > (),
        mElemQual(false), mAttrQual(false)
    {
    }
    inline ~ PmtMetadataFactory()
    {
        clear();
    }                           //{ clearIt() ; }

    const bool & elementQualified() const
    {
        return mElemQual;
    }
    bool & elementQualified()
    {
        return mElemQual;
    }
    const bool & attributeQualified() const
    {
        return mAttrQual;
    }
    bool & attributeQualified()
    {
        return mAttrQual;
    }

  private:
    bool mElemQual;
    bool mAttrQual;

    void clearIt(void);
};


//! PmtMdFactoryMap is a wrapper class of factory map
class PmtMetadataFactories
{

  public:
    PmtMetadataFactories()
    {
    };
    ~PmtMetadataFactories()
    {
        clear();
    }

    PmtMetadataFactory *getMdFactory(const SPmtString & nameSpace);
    void clear();
    void clear(const SPmtString & nameSpace);

  private:
    typedef map < SPmtString, PmtMetadataFactory * >MdFactMap;
    MdFactMap mFactoryMap;

    PmtMetadataFactory *createMdFactory(const SPmtString & nameSpace);
};


/////////////////////////////////////////////////////////////////
// PmtMetadataIterator class inline functions 
// (moved here as they reference items below class definition,
//  As discovered by GCC 3.3.2)
/////////////////////////////////////////////////////////////////

inline void PmtMetadataIterator::clear()
{
    mMdList.erase(mMdList.begin(), mMdList.end());
    return;
}


inline PmtMetadataPtr PmtMetadataIterator::start()
{
    mCrntIter = mMdList.begin();
    return checkIter(mCrntIter);
}


inline PmtMetadataPtr PmtMetadataIterator::next()
{
    return checkIter(++mCrntIter);
}


inline void PmtMetadataIterator::insert(PmtMetadataPtr & mdPtr)
{
    mMdList.push_back(mdPtr);
}


inline PmtMetadataPtr PmtMetadataIterator::checkIter(PmtMdIter & iter)
{
    if (mCrntIter != mMdList.end())
        return (*mCrntIter);
    else
        return NULL;
}

//! The assignment operator
inline PmtMetadataIterator & PmtMetadataIterator::operator=(
                                                const PmtMetadataIterator & rhs)
{
    mMdList = rhs.mMdList;
    mCrntIter = rhs.mCrntIter;
    return *this;
}



#endif

