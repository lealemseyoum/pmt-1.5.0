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
 * Creation Date: 7/08/03
 *
 * Original Author: 
 * Sam Fryer <samuel.fryer@kodak.com> 
 * 
 * Contributor(s):
 *
 */


#ifndef PMT_SAX2_H
#define PMT_SAX2_H

// See if we're using XERCES
#if defined ( PMT_USE_XERCES )

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// XERCES
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// required includes
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/BinInputStream.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

// These files have some other relevant definitions in them.  As we proceed
// to a truly SAX2 interface, we'll have to incorporate those definitions.
#include "PmtDomString.h"

// For XERCES, the DomString and the SAXString are the same
typedef PmtDomString PmtSAXString;

// avoid namespace unpleasantries
XERCES_CPP_NAMESPACE_USE ;

// new typedef in Xerces 3
#if defined ( XERCES_VERSION_MAJOR ) && ( XERCES_VERSION_MAJOR > 2 )
#define PMTXMLSize XMLSize_t
#else
#define PMTXMLSize unsigned int
#define XMLFilePos PMTXMLSize
#endif

//----------------------------------------------------------------------------
// SAX2 Parser definitions
//----------------------------------------------------------------------------
#define PmtCreateSAX2XMLReader XMLReaderFactory::createXMLReader
typedef SAX2XMLReader PmtSAX2XMLReader;

#define PMT_SAX2_PARSEFILE(parser,filename)  parser->parse(filename);

#define PMT_DELETE_SAX2_PARSER(parser) delete parser;

//----------------------------------------------------------------------------
// SAX2 Attribute definitions
//----------------------------------------------------------------------------
typedef Attributes PmtSAX2Attributes;

#define PMT_GETSAX2ATTR_LEN(attributes,len) len=attributes.getLength();

// Routine to return the PmtSAXString representation of the localName.
inline PmtSAXString * PmtGetSAX2AttributeLocalName(
                                   const PmtSAX2Attributes * attributes,
                                   int index)
{
    const XMLCh *name = attributes->getLocalName(index);
    PmtSAXString * localName = new PmtSAXString(name);
    return localName;
};

// Routine to return the PmtSAXString representation of the localName.
inline PmtSAXString * PmtGetSAX2AttributeURI(
	const PmtSAX2Attributes * attributes,
	int index)
{
	const XMLCh *name = attributes->getURI(index);
	PmtSAXString * localName = new PmtSAXString(name);
	return localName;
};

#define PMT_GETSAX2ATTR_VALUE(attributes,index,value) \
    value=attributes.getValue((PMTXMLSize)index);

//----------------------------------------------------------------------------
// Content Handler definitions
//----------------------------------------------------------------------------

class PmtSAX2DefaultHandler : public DefaultHandler
{
    public:
        virtual void xcharacters(const XMLCh* const chars,
                                 const unsigned int length) 
		{
		};
#if defined ( XERCES_VERSION_MAJOR ) && ( XERCES_VERSION_MAJOR > 2 )
		virtual void characters(const XMLCh* const chars,
								const XMLSize_t length)
		{
			xcharacters(chars,length);
		};
#else
		virtual void characters(const XMLCh* const chars,
			const unsigned int length)
		{
			xcharacters(chars,length);
		};
#endif
		virtual void xendDocument()
		{
		}
		virtual void endDocument(void)
		{
			xendDocument();
		}
};

//----------------------------------------------------------------------------
// Input definitions
//----------------------------------------------------------------------------

#define PMT_TRANS_TABLE_ID XERCES_TRANS_TABLE_ID

// Since we're using the XERCES paradigm, this really doesn't do much
inline MemBufInputSource * PmtGetSAX2MemorySource(
                               const unsigned char * src,
                               unsigned int len,
                               const char * id)
{
    return new MemBufInputSource(src,len,id);
}

// This is how we delete the source, delete the source, delete the source
#define PMT_DELETE_SAX2_MEMSRC(x) delete x;

// These are the stream input classes written for Xerces 
// ('Cause Xerces doesn't provide what it should...)
class PmtBinInputStream:public BinInputStream
{
  public:
    PmtBinInputStream(istream & aIn):in(aIn)
    {
    }

    virtual ~ PmtBinInputStream()
    {
    }

    virtual XMLFilePos curPos() const
    {
        return in.tellg();
    }

    virtual PMTXMLSize readBytes(XMLByte * const toFill,
                                   const PMTXMLSize maxToRead)
    {
        in.read((char *) toFill, maxToRead);
        return in.gcount();
    }

    virtual const XMLCh* getContentType() const
    {
        return NULL;
    }

  private:
    istream & in;
};


class PmtStreamInputSource:public InputSource
{
  public:
    PmtStreamInputSource(istream & aStream):in(aStream)
    {
    }

    virtual BinInputStream *makeStream() const
    {
        if (in.good())
        {
            BinInputStream *inputStream = new PmtBinInputStream(in);
             return inputStream;
        }
        else
             return 0;
    }

  private:
    istream & in;
};

#define PmtSAX2CreateStreamInputSource(inStream) \
              new PmtStreamInputSource(*inStream)
#define PMT_DELETE_STREAM_SOURCE(x) delete x;


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// END OF XERCES
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

#else

#if defined ( PMT_USE_MSXML )

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// MSXML
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// required includes
#include <string.h>
#include "msxml2.h"

// Use XERCES type character definitions
#define XMLCh wchar_t

// define our new PmtSAXString for MSXML
// This is basically a copy/replacement for PmtDomString
// that uses Windows type wchar calls for translations
//class PMTCORE_DECL PmtSAXString
class PmtSAXString
{
  public:

    //! This is a constructor which takes a const XMLCh * that someone else owns.
    inline PmtSAXString(const XMLCh* SAXString, bool ownSAXString = false)
               :mSAXString((XMLCh*)SAXString),mAsciiString(NULL),
                mOwnSAXString(ownSAXString),mOwnAsciiString(false)
    {
    }

    //! This is a constructor which takes a const char * that someone else owns.
    inline PmtSAXString(const char *utf8Str)
               :mSAXString(NULL),mAsciiString((char*)utf8Str),
                mOwnSAXString(false),mOwnAsciiString(false)
    {
    }
    
    
    //! This is a constructor which takes a const SPmtString that someone else owns.
    inline PmtSAXString(const SPmtString utf8Str)
               :mSAXString(NULL),mAsciiString((char*)utf8Str.c_str()),
                mOwnSAXString(false),mOwnAsciiString(false)
    {
    }


    //! This is the destructor
    ~PmtSAXString()
    {
       if ((mOwnAsciiString)&&(mAsciiString != NULL))
           delete[] mAsciiString;
       if ((mOwnSAXString)&&(mSAXString != NULL))
           delete[] mSAXString;
    }

    //! This method returns the SPmtString representation
    inline SPmtString getString()
    {
        SPmtString theString(this->getAsciiString());
        return theString;
    }

    //! This method returns an ascii representation of the XMLCh*
    inline const char *getAsciiString()
    {
        if (mAsciiString == NULL)
        {
            unsigned int length = wcslen(mSAXString);
            mAsciiString = new char[length+4];
            wcstombs(mAsciiString,mSAXString,length);
            mAsciiString[length]=0;
            mOwnAsciiString = true;
        }
        return mAsciiString;
    }

    //! This method returns the XMLCh * representation
    inline XMLCh* getXMLCh()
    {
        if (mSAXString == NULL)
        {
            unsigned int length = strlen(mAsciiString);
            mSAXString = new XMLCh[length+4];
            mbstowcs(mSAXString,mAsciiString,length);
            mSAXString[length]=0;
            mOwnSAXString = true;
        }
        return mSAXString;
    }

  private:
    XMLCh *mSAXString;
    char *mAsciiString;
    bool mOwnSAXString;
    bool mOwnAsciiString;

    //! This is a private constructor
    inline PmtSAXString()
               :mSAXString(NULL),mAsciiString(NULL),
                mOwnSAXString(false),mOwnAsciiString(false)
    {
    }

};


//----------------------------------------------------------------------------
// SAX2 Parser definitions
//----------------------------------------------------------------------------
typedef ISAXXMLReader PmtSAX2XMLReader;

inline void PMT_SAX2_PARSEFILE(PmtSAX2XMLReader *parser, const char *filename) 
{
    wchar_t PMT_SAX2_PARSEFILE_filename[256]; 
    mbstowcs(PMT_SAX2_PARSEFILE_filename,filename,255); 
    parser->parseURL(PMT_SAX2_PARSEFILE_filename);
};

// Shouldn't we be able to add this function somehow?
#define setContentHandler putContentHandler

// routine to create the parser
// Note: CoInitialize may be called several times ... maybe we could
// COM better and come up with a better way
inline PmtSAX2XMLReader * PmtCreateSAX2XMLReader()
{
    PmtSAX2XMLReader * parser = NULL;
    CoInitialize(NULL); 
    HRESULT hr = CoCreateInstance(
                                __uuidof(SAXXMLReader), 
                                NULL, 
                                CLSCTX_ALL, 
                                __uuidof(ISAXXMLReader), 
                                (LPVOID *)& parser);
    return parser;
};

#define PMT_DELETE_SAX2_PARSER(parser) parser->Release();

#define PMT_TRANS_TABLE_ID 0

//----------------------------------------------------------------------------
// SAX2 Attribute definitions
//----------------------------------------------------------------------------
typedef ISAXAttributes PmtSAX2Attributes ;

#define PMT_GETSAX2ATTR_LEN(attributes,len) \
    ((PmtSAX2Attributes*)&attributes)->getLength(&len);

// Routine to convert the attribute name into a PmtSAXString
// (MSXML simply returns a pointer to the place in the file, and 
// the length of the name.)
inline PmtSAXString * PmtGetSAX2AttributeLocalName(
                      const PmtSAX2Attributes * attributes,
                      int index)
{
    const XMLCh * name = NULL;
    int count = 256;
    ((PmtSAX2Attributes*)attributes)->getLocalName(index,&name,&count);
    XMLCh * permanentName = new XMLCh[count+1];
    wcsncpy(permanentName,name,count);
    permanentName[count] = 0;
    PmtSAXString *localName = new PmtSAXString(permanentName,true);
    return localName;
}

inline PmtSAXString * PmtGetSAX2AttributeURI(
	const PmtSAX2Attributes * attributes,
	int index)
{
	const XMLCh * name = NULL;
	int count = 256;
	((PmtSAX2Attributes*)attributes)->getURI(index,&name,&count);
	XMLCh * permanentName = new XMLCh[count+1];
	wcsncpy(permanentName,name,count);
	permanentName[count] = 0;
	PmtSAXString *localName = new PmtSAXString(permanentName,true);
	return localName;
}


// MSXML does return the value properly, though.
#define PMT_GETSAX2ATTR_VALUE(attributes,index,value) \
    int PMT_GETSAX2ATTR_VALUE_count = 256; \
    ((PmtSAX2Attributes*)&attributes)->getValue(index,&value, \
                            &PMT_GETSAX2ATTR_VALUE_count);

//----------------------------------------------------------------------------
// Content Handler definitions
//----------------------------------------------------------------------------

// Our own DefaultHandler class that simply does nothing for all the functions
// because the MSXML handler has everything as pure virtual.
// We also set the MSXML style handlers to call the XERCES style handlers.
class PmtSAX2DefaultHandler : public ISAXContentHandler
{
    public:

        // Constructor
        PmtSAX2DefaultHandler(){};

        // Standard Deconstructor
        virtual ~PmtSAX2DefaultHandler(){};

        long __stdcall QueryInterface(const struct _GUID &,void ** ){return 0;};

        unsigned long __stdcall AddRef(void){return 0;};

        unsigned long __stdcall Release(void){return 0;};

        virtual HRESULT STDMETHODCALLTYPE putDocumentLocator( 
        /* [in] */ ISAXLocator *pLocator) {return S_OK;};
        
        virtual HRESULT STDMETHODCALLTYPE startDocument( void) {return S_OK;};
        
		virtual void xendDocument(void) {};
        virtual HRESULT STDMETHODCALLTYPE endDocument( void) 
		{
			xendDocument();
			return S_OK;
		};
        
		virtual void startPrefixMapping(const XMLCh* const prefix,const XMLCh* const uri){};
        virtual HRESULT STDMETHODCALLTYPE startPrefixMapping( 
            /* [in] */ const wchar_t *pwchPrefix,
            /* [in] */ int cchPrefix,
            /* [in] */ const wchar_t *pwchUri,
            /* [in] */ int cchUri) 
		{
			startPrefixMapping(pwchPrefix, pwchUri);
			return S_OK;
		};
        
        virtual HRESULT STDMETHODCALLTYPE endPrefixMapping( 
            /* [in] */ const wchar_t *pwchPrefix,
            /* [in] */ int cchPrefix) {return S_OK;};
        
        // The callback function for the start of a new element.
        virtual void startElement(    
                            const   XMLCh* const    uri,
                            const   XMLCh* const    localname,
                            const   XMLCh* const    qname,
                            const   PmtSAX2Attributes&  attributes){};

        HRESULT STDMETHODCALLTYPE startElement( 
            /* [in] */ const wchar_t __RPC_FAR *pwchNamespaceUri,
            /* [in] */ int cchNamespaceUri,
            /* [in] */ const wchar_t __RPC_FAR *pwchLocalName,
            /* [in] */ int cchLocalName,
            /* [in] */ const wchar_t __RPC_FAR *pwchRawName,
            /* [in] */ int cchRawName,
            /* [in] */ ISAXAttributes __RPC_FAR *pAttributes)
        {  
            startElement(pwchNamespaceUri,pwchLocalName,
                         pwchRawName,*pAttributes);
            return S_OK;
        };

        virtual void endElement( 
                                const XMLCh* const namespaceUri,
                                const XMLCh* const localName,
                                const XMLCh* const qname
                               ) {};
        HRESULT STDMETHODCALLTYPE endElement( 
            /* [in] */ const wchar_t *pwchNamespaceUri,
            /* [in] */ int cchNamespaceUri,
            /* [in] */ const wchar_t *pwchLocalName,
            /* [in] */ int cchLocalName,
            /* [in] */ const wchar_t *pwchQName,
            /* [in] */ int cchQName)
        {  
            endElement(pwchNamespaceUri,pwchLocalName,pwchQName);
            return S_OK;
        };
        
        virtual void xcharacters(const XMLCh* const chars,
                                const unsigned int length) 
		{
		};
        virtual HRESULT STDMETHODCALLTYPE characters( 
            /* [in] */ const wchar_t *pwchChars,
            /* [in] */ int cchChars) 
        {
            xcharacters(pwchChars,cchChars);
            return S_OK;
        };
        
        virtual HRESULT STDMETHODCALLTYPE ignorableWhitespace( 
            /* [in] */ const wchar_t *pwchChars,
            /* [in] */ int cchChars) {return S_OK;};
        
        virtual HRESULT STDMETHODCALLTYPE processingInstruction( 
            /* [in] */ const wchar_t *pwchTarget,
            /* [in] */ int cchTarget,
            /* [in] */ const wchar_t *pwchData,
            /* [in] */ int cchData) {return S_OK;};
        
        virtual HRESULT STDMETHODCALLTYPE skippedEntity( 
            /* [in] */ const wchar_t *pwchName,
            /* [in] */ int cchName) {return S_OK;};
        
};

//----------------------------------------------------------------------------
// Input definitions
//----------------------------------------------------------------------------

// MSXML uses the VARIANT type to input raw data
#define MemBufInputSource VARIANT
#define PmtStreamInputSource VARIANT

// This creates a VARIANT and places the data into the BSTR section of it.
inline MemBufInputSource * PmtGetSAX2MemorySource(const unsigned char * src,
                                                  unsigned int len,
                                                  const char * id)
{
    VARIANT * memSrc = new VARIANT;
    XMLCh * wSrc = new XMLCh[len+1];
    mbstowcs(wSrc,(const char *)src,len);
    wSrc[len]=0;
    memSrc->pcVal = (char*)wSrc;
    memSrc->vt = VT_BSTR;
    return memSrc;
}

// This creates a VARIANT and places the data into the BSTR section of it.
inline PmtStreamInputSource * PmtSAX2CreateStreamInputSource(istream * inStream)
{
    inStream->seekg(0,ios::end);
    unsigned long len = inStream->tellg();
    inStream->seekg(0,ios::beg);
    VARIANT * memSrc = new VARIANT;
    char * src = new char[len+1];
    inStream->read(src,len);
    XMLCh * wSrc = new XMLCh[len+1];
    mbstowcs(wSrc,(const char *)src,len);
    wSrc[len]=0;
    memSrc->pcVal = (char*)wSrc;
    memSrc->vt = VT_BSTR;
    delete [] src;
    return memSrc;
}

// This is how we delete the source, delete the source, delete the source
#define PMT_DELETE_SAX2_MEMSRC(x) delete x->pcVal; delete x;
#define PMT_DELETE_STREAM_SOURCE(x) delete x->pcVal; delete x;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// END OF MSXML
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// end if PMT_USE_MSXML
#endif 

// end if PMT_USE_XERCES
#endif 

// end of PMT_SAX2_H
#endif 
