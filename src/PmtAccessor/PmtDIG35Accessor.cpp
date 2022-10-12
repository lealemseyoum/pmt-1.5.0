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
 * Creation Date: March 2003
 *
 * Original Author: 
 * Ricardo Rosario ricardo.rosario@kodak.com
 * 
 * Contributor(s): 
 */ 

#include "EkCompiler.h"

#if (defined PMT_DIG35) && ((defined PMT_USE_XERCES_2) || (defined PMT_USE_MSXML))

#include "EkStrings.h"
#include "PmtDIG35Accessor.h"

#include <string.h>

// We need this for PmtSAXString
#include "PmtSAX2.h"

// All the Xerces includes
#ifdef PMT_USE_XERCES_2
#include "xercesc/dom/DOM.hpp"
#include "xercesc/dom/DOMImplementation.hpp" 
#include "xercesc/dom/DOMImplementationLS.hpp"
#include "xercesc/dom/DOMWriter.hpp"
#include "xercesc/dom/DOMNodeList.hpp"
#include "xercesc/dom/DOMElement.hpp"
#include "xercesc/util/XMLString.hpp"
#include "xercesc/framework/MemBufInputSource.hpp"
#include "xercesc/framework/MemBufFormatTarget.hpp"
#include "xercesc/framework/LocalFileFormatTarget.hpp"
#include "PmtDomString.h"
#endif

// #if _MSC_VER < 1300
//     /*
//      * If this is missing on VC6, the EK_PROVIDE statement (below) will 
//      * generate compile-time errors relating to EkFactoryT.  VC7 compiles 
//      * the code properly.  I'm not sure why adding a dummy, unused, factory 
//      * object allows VC6 to compile this, but it works, so don't knock it!
//      */
//     static PmtAccessorFactoryType gVc6IsStupid_DoNotUse;
// #endif
// 
// #if defined ( __GNUC__ ) && ( __GNUC__ > 2 )
// // SMF 2/11/2002 -- This is a work-around for a bug (ICE) in the GCC 3.0.3 compiler.
// int *PmtDIG35Accessor_tmp = (int*)PMT_ACCESSOR_TYPE_INIT(PMT_FORMAT_DIG35, PmtDIG35Accessor); 
// const int *PmtDIG35Accessor_tmp2[] = { PmtDIG35Accessor_tmp};
// const int *PmtDIG35Accessor_FILE[1];
// void * PmtDIG35Accessor_FILE_Addr = &PmtDIG35Accessor_FILE;
// bool PmtDIG35Accessor_tmp3 = (PmtDIG35Accessor_FILE_Addr = &PmtDIG35Accessor_tmp2);
// #else
// EK_PROVIDE(PmtDIG35Accessor, PMT_ACCESSOR_TYPE_INIT(PMT_FORMAT_DIG35, PmtDIG35Accessor))
// #endif

char * DIG35_KEY_ATTR = "key";

// Key to root element of DIG35 document
char * DIG35_ROOT_ELEMENT = "METADATA";

// Keys to DIG35 Metadata block elements
char * DIG35_BASIC_IMAGE_PARAM = "BASIC_IMAGE_PARAM";
char * DIG35_IMAGE_CREATION = "IMAGE_CREATION";
char * DIG35_CONTENT_DESCRIPTION = "CONTENT_DESCRIPTION";
char * DIG35_HISTORY = "HISTORY";
char * DIG35_IPR = "IPR";
char * DIG35_METADATA = "METADATA";

PmtSAXString dig35KeyAttr( DIG35_KEY_ATTR );
PmtSAXString dig35RootElement( DIG35_ROOT_ELEMENT );
PmtSAXString dig35BasicImageParam( DIG35_BASIC_IMAGE_PARAM );
PmtSAXString dig35ImageCreation( DIG35_IMAGE_CREATION );
PmtSAXString dig35ContentDescription( DIG35_CONTENT_DESCRIPTION );
PmtSAXString dig35History( DIG35_HISTORY );
PmtSAXString dig35IPR( DIG35_IPR );
PmtSAXString dig35Metadata( DIG35_METADATA );

// This is the key used for DIG35 entries in the translation table
#define PMT_DIG35_ELEMENT_NAME "DIG35"

// Constructor
PmtDIG35Accessor::PmtDIG35Accessor()
  : PmtAccessor(PMT_FORMAT_DIG35), mDoc(NULL)
{}

// Destructor
PmtDIG35Accessor::~PmtDIG35Accessor() 
{
    close() ;
}

// close the accessor
void PmtDIG35Accessor::close() 
{
    // if we have a filename, a document, and we opened in READWRITE or CREATE
    // mode, then we need to write out the DOM tree as XML to the file...
    if ( (mFileName.length()!=0) && (mMode!=PMT_READONLY) && mDoc)
    {
#ifdef PMT_USE_XERCES_2
        // get a serializer, an instance of DOMWriter
        XMLCh tempStr[100];
        XERCES_CPP_NAMESPACE_QUALIFIER XMLString::transcode("LS", tempStr, 99);
        XERCES_CPP_NAMESPACE_QUALIFIER DOMImplementation *impl = 
            XERCES_CPP_NAMESPACE_QUALIFIER 
            DOMImplementationRegistry::getDOMImplementation(tempStr);
        XERCES_CPP_NAMESPACE_QUALIFIER DOMWriter *theSerializer = 
            ((XERCES_CPP_NAMESPACE_QUALIFIER DOMImplementationLS*)
            impl)->createDOMWriter();
        
        // Plug in a format target to receive the resultant
        // XML stream from the serializer.
        XERCES_CPP_NAMESPACE_QUALIFIER LocalFileFormatTarget *myFormTarget = 
            new XERCES_CPP_NAMESPACE_QUALIFIER LocalFileFormatTarget(
            mFileName.c_str() );
        
        // do the serialization through DOMWriter::writeNode();
        theSerializer->writeNode(myFormTarget, *mDoc);
        delete theSerializer;
        delete myFormTarget;
#endif

#ifdef PMT_USE_MSXML
        // We get the xml as a string, then write it out to the file.
        BSTR xml;
        if(!FAILED(mDoc->get_xml(&xml)))
        {
            // convert to wchar_t
            wchar_t* temp = xml;
            // now put into PmtSAXString
            PmtSAXString temp2(temp);
            // and finally get the ascii string
            const char * outXML = temp2.getAsciiString();
            // open the file, write to it, and close
            FILE * file = fopen( mFileName.c_str(), "wb" );
            fwrite( outXML, strlen(outXML)*sizeof(char),1, file);
            fclose(file);
        }
#endif
    }

    PmtAccessor::close();
    
    // delete the DOM Document if we have one
    if ( mDoc!=NULL )
    {
#ifdef PMT_USE_XERCES_2
        delete mDoc;
#endif
#ifdef PMT_USE_MSXML
        mDoc->Release();
#endif
        mDoc = NULL;

    }
    
    mFileName = "";
}

// close to memory
int PmtDIG35Accessor::close( char* buf, unsigned int bufSize )
{
    // return_value is set to a positive int equal to the size of the document
    // if the close is successful; to a negative int with absolute value equal
    // to the required buffer size, if the passed in buffer isn't big enough;
    // to 0 for any other error
    
    int returnValue = 0;
    if ( mDoc )
    {
        try
        {
#ifdef PMT_USE_XERCES_2
            // get a serializer, an instance of DOMWriter
            XMLCh tempStr[100];
            XERCES_CPP_NAMESPACE_QUALIFIER XMLString::transcode("LS", tempStr, 99);
            XERCES_CPP_NAMESPACE_QUALIFIER DOMImplementation *impl = 
                XERCES_CPP_NAMESPACE_QUALIFIER 
                DOMImplementationRegistry::getDOMImplementation(tempStr);
            XERCES_CPP_NAMESPACE_QUALIFIER DOMWriter *theSerializer = 
                ((XERCES_CPP_NAMESPACE_QUALIFIER DOMImplementationLS*)
                impl)->createDOMWriter();

            // Plug in a format target to receive the resultant
            // XML stream from the serializer.
            XERCES_CPP_NAMESPACE_QUALIFIER MemBufFormatTarget *myFormTarget =
                new XERCES_CPP_NAMESPACE_QUALIFIER MemBufFormatTarget();

            // get the DOM representation
            XERCES_CPP_NAMESPACE_QUALIFIER DOMNode *doc = mDoc;

            // do the serialization through DOMWriter::writeNode();
            theSerializer->writeNode(myFormTarget, *doc);

            delete theSerializer;

            unsigned int domDocSize = myFormTarget->getLen();
            if ( domDocSize > bufSize )
                returnValue = - (int)domDocSize;
            else
            {
                memcpy(buf,myFormTarget->getRawBuffer(),domDocSize);
                returnValue = domDocSize;
            }

            delete myFormTarget;
#endif

#ifdef PMT_USE_MSXML
            // We get the xml as a string, then write it out to the file.
            BSTR xml;
            if(!FAILED(mDoc->get_xml(&xml)))
            {
                // convert to wchar_t
                wchar_t* temp = xml;
                // now put into PmtSAXString
                PmtSAXString temp2(temp);
                // and finally get the ascii string
                const char * outXML = temp2.getAsciiString();
                unsigned int domDocSize = strlen(outXML)*sizeof(char);
                if( domDocSize > bufSize )
                {
                    // the buffer passed in is to small for the xml
                    returnValue = - (int)domDocSize;
                }
                else
                {
                    // copy the xml to the buffer passed in
                    memcpy(buf,outXML,domDocSize);
                    returnValue = domDocSize;
                }
            }
#endif
        }
        // catch all exceptions
        catch (...)
        {
            returnValue = 0;
        }        
    }
    
    return returnValue;
}

// Open the specified file and parse into the DOM
bool PmtDIG35Accessor::open(const char *filename, PmtOpenMode mode)
{
    bool success = true;
    
    // if the mode is READONLY or READWRITE, then we parse the specified file,
    // otherwise (CREATE mode) we create a new DOM document. All this work
    // is actually done in the open from memory function...
    if ( mode == PMT_READONLY || mode == PMT_READWRITE )
    {
        FILE* file = fopen(filename, "rb");
        if (file)
        {
            fseek(file, 0, SEEK_END);
            int size = ftell(file);
            fseek(file, 0, SEEK_SET);
            unsigned char* buf = new unsigned char[size];
            if ( fread(buf,size,1,file) == 1 )
                success = open(buf, size, mode);
            else
                success = false;
            
            fclose( file );
            delete buf;
        }
        else
            success = false;
    }
    else if ( mode == PMT_CREATE )
    {
        success = open(NULL,0,mode);
    }

    if ( success )
        mFileName = filename;

    return success;
}

// Open the DIG35 document from memory and parse into the DOM
bool PmtDIG35Accessor::open(const unsigned char * buf,
                            const unsigned int bufSize, PmtOpenMode mode)
{
    bool success = true;
    close();
    mMode = mode;
    
    // Make sure any schema and translation table changes are handled
    loadIfAny();

#ifdef PMT_USE_MSXML
    if( FAILED(CoInitialize(NULL)) )
        success = false;
    else if ( FAILED(CoCreateInstance(__uuidof(DOMDocument), NULL,
                   CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&mDoc)) )
        success = false;
    else
#endif
    if ( mode == PMT_READONLY || mode == PMT_READWRITE )
    {
        // if we are opening in readonly or read/write mode, then there
        // is an existing document that we must parse
        try
        {
#ifdef PMT_USE_XERCES_2
            // parse into the mem buffer
            XERCES_CPP_NAMESPACE_QUALIFIER XercesDOMParser* docParser =
                new XERCES_CPP_NAMESPACE_QUALIFIER XercesDOMParser;
            try
            {
                docParser->setDoNamespaces(true);
                XERCES_CPP_NAMESPACE_QUALIFIER MemBufInputSource memBuf( buf,
                    bufSize, "DIG35_DOC_ID" );
                docParser->parse(memBuf);

                // take ownership of the DOMDocument and delete the parser
                mDoc = docParser->adoptDocument();
            }
            catch (...)
            {
                success = false;
            }
            delete docParser;

#endif

#ifdef PMT_USE_MSXML
            wchar_t * wSrc = new wchar_t[bufSize+1];
            mbstowcs(wSrc,(const char *)buf,bufSize);
            wSrc[bufSize]=0;
            
            VARIANT_BOOL* isSuccessful = new VARIANT_BOOL;
            HRESULT result;
            result = mDoc->loadXML(wSrc, isSuccessful);
            delete isSuccessful;
            if ( FAILED(result) )
                success = false;

            delete[] wSrc;
#endif
        }
        // catch any thrown exceptions
        catch (...)
        {
            success = false;
        }
    }
    else if ( mode == PMT_CREATE )
    {
#ifdef PMT_USE_XERCES_2
        // if we are opening in create mode, then we need to create a new
        // DOMDocument
        XMLCh* domImpl = XERCES_CPP_NAMESPACE_QUALIFIER 
            XMLString::transcode("Core");
        XERCES_CPP_NAMESPACE_QUALIFIER DOMImplementation* impl =
            XERCES_CPP_NAMESPACE_QUALIFIER 
            DOMImplementationRegistry::getDOMImplementation(domImpl);
        delete domImpl;

        mDoc = impl->createDocument(0,dig35RootElement.getXMLCh(),0);
        if ( mDoc )
        {
            XMLCh* version = XERCES_CPP_NAMESPACE_QUALIFIER 
                XMLString::transcode("1.0");
            mDoc->setVersion( version );
            delete version;
            XMLCh* encoding = XERCES_CPP_NAMESPACE_QUALIFIER 
                XMLString::transcode("UTF-8");
            mDoc->setEncoding( encoding );
            delete encoding;
        }
        else
            success = false;
#endif

#ifdef PMT_USE_MSXML
        // we are creating a new doc from scratch. create and insert the root
        IXMLDOMElement* root;
        if( FAILED(mDoc->createElement(dig35Metadata.getXMLCh(), &root)) )
            success = false;
        else
        {
            mDoc->putref_documentElement( root );
        }

#endif
    }
    else
        success = false;  
    
    return success;
}

// create a new DIG35 document
bool PmtDIG35Accessor::open()
{
    return open(NULL, 0, PMT_CREATE);
}

// append/merge a DIG35 file
bool PmtDIG35Accessor::append( const char *filename )
{
    bool success = true;
    
    // we must have an open document to be able to append anything to it...
    if ( mDoc )
    {
        // read the file to memory and append using the "from memory" append
        FILE* file = fopen(filename, "rb");
        if (file)
        {
            fseek(file, 0, SEEK_END);
            int size = ftell(file);
            fseek(file, 0, SEEK_SET);
            unsigned char* buf = new unsigned char[size];
            if ( fread(buf,size,1,file) == 1 )
                success = append(buf, size);
            else
                success = false;
            
            fclose( file );
            delete buf;
        }
        else
            success = false;
    }
    else
        success = false;
        
    return success;
}

// append/merge a DIG35 doc from memory
bool PmtDIG35Accessor::append( const unsigned char * buf,
    const unsigned int bufSize )
{
    bool success = true;

    // we must have an open document to be able to append anything to it...
    if ( mDoc )
    {
#ifdef PMT_USE_XERCES_2
        try
        {
            XERCES_CPP_NAMESPACE_QUALIFIER XercesDOMParser* docParser =
                new XERCES_CPP_NAMESPACE_QUALIFIER XercesDOMParser;
            docParser->setDoNamespaces(true);
            XERCES_CPP_NAMESPACE_QUALIFIER MemBufInputSource memBuf( buf,
                bufSize, "DIG35_DOC_ID" );
            docParser->parse(memBuf);
            XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* docToAppend =
                docParser->getDocument();
            
            XERCES_CPP_NAMESPACE_QUALIFIER DOMElement* rootElem = 
                (XERCES_CPP_NAMESPACE_QUALIFIER DOMElement*)
                mDoc->getElementsByTagName(dig35RootElement.getXMLCh())->item(0);
                            
            if ( rootElem!=NULL )
            {
                XERCES_CPP_NAMESPACE_QUALIFIER DOMElement* docToAppendRoot =
                    docToAppend->getDocumentElement();
                
                const XMLCh* docRoot = docToAppendRoot->getTagName();
                PmtDomString temp(docRoot);
                if (temp.getString() == dig35RootElement.getString())
                {
                    XERCES_CPP_NAMESPACE_QUALIFIER DOMNodeList* 
                        childrenToAppend = docToAppendRoot->getChildNodes();
                    int numChildren = childrenToAppend->getLength();
                    for ( int i = 0; i < numChildren; i++ )
                    {
                        // only append the Element nodes.
                        if ( childrenToAppend->item(i)->getNodeType() == 
                            XERCES_CPP_NAMESPACE_QUALIFIER 
                            DOMNode::ELEMENT_NODE )
                        {
                            XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* child =
                                mDoc->importNode(childrenToAppend->
                                                 item(i),true);
                            rootElem->appendChild( child );
                        }
                    }
                
                }
                else if ( isMetadataBlockKey(docRoot) )
                {
                    XERCES_CPP_NAMESPACE_QUALIFIER DOMElement* newBlock =
                        mDoc->createElement( docRoot );
                    XERCES_CPP_NAMESPACE_QUALIFIER DOMNodeList* blockChildren = 
                        docToAppendRoot->getChildNodes();
                    int numChildren = blockChildren->getLength();
                    
                    for ( int i = 0; i < numChildren; i++ )
                    {
                        // only append the Element nodes.
                        if (blockChildren->item(i)->getNodeType() == 
                            XERCES_CPP_NAMESPACE_QUALIFIER 
                            DOMNode::ELEMENT_NODE )
                        {
                            XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* child =
                                mDoc->importNode(blockChildren->item(i), true);
                            newBlock->appendChild( child );
                        }
                    }
                    mDoc->getDocumentElement()->appendChild( newBlock );
                }
                else
                    success = false;
            }
            else
                success = false;

            delete docParser;
        }
        catch (XERCES_CPP_NAMESPACE_QUALIFIER DOMException e)
        {
            success = false;
        }
#endif

#ifdef PMT_USE_MSXML
        IXMLDOMDocument* docToAppend;

        if( FAILED(CoInitialize(NULL)) )
            success = false;
        else if ( FAILED(CoCreateInstance(__uuidof(DOMDocument), NULL, CLSCTX_INPROC_SERVER, 
            IID_IXMLDOMDocument, (void**)&docToAppend)) )
            success = false;
        else
        {
            wchar_t * wSrc = new wchar_t[bufSize+1];
            mbstowcs(wSrc,(const char *)buf,bufSize);
            wSrc[bufSize]=0;
            
            VARIANT_BOOL* isSuccessful = new VARIANT_BOOL;
            HRESULT result;
            result = docToAppend->loadXML(wSrc, isSuccessful);
            delete isSuccessful;
            if ( FAILED(result) )
            {
                success = false;
                docToAppend->Release();
            }
            else
            {
                IXMLDOMElement* rootElemToAppend;
                docToAppend->get_documentElement(&rootElemToAppend);
                
                BSTR docToAppendRootName;
                rootElemToAppend->get_tagName(&docToAppendRootName);

                BSTR temp(dig35Metadata.getXMLCh());
                
                // Check to see if the root of the document is METADATA or a BlockKey
                if (wcscmp(docToAppendRootName,temp)==0)
                {
                    // The root element of the document to append is "METADATA"
                    IXMLDOMNodeList* childrenToAppend;
                    rootElemToAppend->get_childNodes(&childrenToAppend);
                    
                    long* numOfChildren = new long;
                    childrenToAppend->get_length(numOfChildren);
                    for( long i = 0; i < *numOfChildren; i++ )
                    {
                        IXMLDOMElement * docRoot;
                        mDoc->get_documentElement(&docRoot);
                        IXMLDOMNode * child;
                        IXMLDOMNode * newChild;
                        childrenToAppend->nextNode(&child);
                        docRoot->appendChild( child, &newChild );
                    }               
                }
                else if ( isMetadataBlockKey(docToAppendRootName) )
                {
                    // The document is a fragment (block) of a DIG35 document, so we just
                    // append the whole thing.
                    IXMLDOMElement * docRoot;
                    mDoc->get_documentElement(&docRoot);

                    IXMLDOMNode * newChild;       
                    docRoot->appendChild(rootElemToAppend, &newChild);
                }
                else
                    success = false;
            }
        }

#endif
    }
    else
    {
        success = false;
    }
    
    return success;
}

// recognize a DIG35 file
bool PmtDIG35Accessor::recognize(const SPmtString &name) const
{
    bool recognized = false;
    
    PmtDIG35Accessor* acc = new PmtDIG35Accessor();
    if ( acc->open( name.c_str(), PMT_READONLY ) )
    {
#ifdef PMT_USE_XERCES_2
        // Verify that the document has the METADATA root element
        XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* rootElem =
            acc->getDOMNode( DIG35_ROOT_ELEMENT, false );
        if ( rootElem != NULL )
            recognized = true;
#endif

#ifdef PMT_USE_MSXML
        // Verify that the document has the METADATA root element
        IXMLDOMNode* rootElem = acc->getDOMNode(DIG35_ROOT_ELEMENT, false);
        if( rootElem != NULL )
            recognized = true;
#endif
        acc->close();
    }
    delete acc;
    
    return recognized;
}

// get the value from the DOM and set it on the Metadata object passed in
bool PmtDIG35Accessor::getMdValue( PmtMetadataPtr& md )
{
    bool success = false;
    PmtKeyType key = md->fullKey(false);
    
    // Prevent deletion of the translation table while we're using it.
    // (shouldn't this just be some kind of protected counter?)
    EK_GUARD(EK_DEFAULT_MUTEX, PmtTranslationTable::msLock);
    
    // Get the translation table column for this accessor
    PmtTranslationTableElements * theTable = 
        msTranslationTable->getSubTable(PMT_DIG35_ELEMENT_NAME);
    
    if ( theTable != NULL )
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
            SPmtString dig35Key = tableElement->mTag;
#ifdef PMT_USE_XERCES_2
            PmtDomString dig35KeyStr(dig35Key);
            
            XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* node =
                getDOMNode( dig35KeyStr.getAsciiString(), false );
            if ( node!=NULL )
            {
                PmtDomString temp(node->getFirstChild()->getNodeValue());
                EkString value = EK_STR2WIDE(temp.getString());
                md->setValueStr( value );
                success = true;
            }
#endif

#ifdef PMT_USE_MSXML
            PmtSAXString dig35KeyStr = tableElement->mTag;
            IXMLDOMNode* node = getDOMNode( dig35KeyStr.getAsciiString(), false );
            if ( node!=NULL )
            {
                BSTR value;
                node->get_text(&value);
                PmtSAXString temp(value);
                EkString val = EK_STR2WIDE(temp.getString());
                md->setValueStr( val );
                success = true;
            }
#endif
        }
    }
    
    return success;
}

bool PmtDIG35Accessor::setMdValue( const PmtMetadataPtr& md )
{
    bool success = false;
    PmtKeyType key = md->fullKey(false);

    // Prevent deletion of the translation table while we're using it.
    // (shouldn't this just be some kind of protected counter?)
    EK_GUARD(EK_DEFAULT_MUTEX, PmtTranslationTable::msLock);
    
    // Get the translation table column for this accessor
    PmtTranslationTableElements * theTable = 
        msTranslationTable->getSubTable(PMT_DIG35_ELEMENT_NAME);

    if ( theTable != NULL )
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
            SPmtString dig35Key = tableElement->mTag;
#ifdef PMT_USE_XERCES_2
            PmtDomString dig35KeyStr(dig35Key);
            
            XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* node =
                getDOMNode( dig35KeyStr.getAsciiString(), true );
            if ( node!=NULL )
            {
                XERCES_CPP_NAMESPACE_QUALIFIER DOMNodeList* children =
                node->getChildNodes();
                XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* textNode = NULL;
            
                int i = 0;
                int numChildren = children->getLength();
                while ( i < numChildren && textNode==NULL )
                {
                    if ( children->item(i)->getNodeType() == 
                        XERCES_CPP_NAMESPACE_QUALIFIER DOMNode::TEXT_NODE )
                    {
                        textNode = children->item(i);
                        break;
                    }
                    i++;
                }
            
                PmtDomString temp(md->getValueStr().c_str());
                XMLCh* nodeValue = temp.getXMLCh();
                if ( textNode != NULL )
                {
                    textNode->setNodeValue( nodeValue );
                }
                else
                {
                    textNode = mDoc->createTextNode(nodeValue);
                    node->appendChild( textNode );
                }
                
                success = true;
            }
#endif

#ifdef PMT_USE_MSXML
            PmtSAXString dig35KeyStr(dig35Key);
            IXMLDOMNode* node = getDOMNode( dig35KeyStr.getAsciiString(), true );
            if( node!=NULL )
            {
                SPmtString temp = md->getValueStr();
                PmtSAXString val(temp);
                BSTR value(val.getXMLCh());
                node->put_text(value);
                success = true;
            }
#endif
        }

    }

    return success;
}

// Remove the metadata specified by md from the file.
void PmtDIG35Accessor::removeMdValue(const PmtMetadataPtr & md)
{
    PmtKeyType key = md->fullKey(false);
    
    // Prevent deletion of the translation table while we're using it.
    // (shouldn't this just be some kind of protected counter?)
    EK_GUARD(EK_DEFAULT_MUTEX, PmtTranslationTable::msLock);
    
    // Get the translation table column for this accessor
    PmtTranslationTableElements * theTable = 
        msTranslationTable->getSubTable(PMT_DIG35_ELEMENT_NAME);
    
    if ( theTable != NULL )
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
            SPmtString dig35Key = tableElement->mTag;
#ifdef PMT_USE_XERCES_2
            PmtDomString dig35KeyStr(dig35Key);
            
            XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* node =
                getDOMNode( dig35KeyStr.getAsciiString(), false );
            if ( node!=NULL )
            {
                // remove the node...
                XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* parent =
                    node->getParentNode();
                parent->removeChild(node);
            }
#endif

#ifdef PMT_USE_MSXML
            PmtSAXString dig35KeyStr = tableElement->mTag;
            IXMLDOMNode* node = getDOMNode( dig35KeyStr.getAsciiString(), false );
            if ( node!=NULL )
            {
                // remove the node...
                IXMLDOMNode* parent;
                IXMLDOMNode* removedChild;
                node->get_parentNode(&parent);

                parent->removeChild(node, &removedChild);
            }
#endif
        }
    }
}

#ifdef PMT_USE_XERCES_2
// get the DOM node specified by the fullDIG35Key
XERCES_CPP_NAMESPACE_QUALIFIER DOMNode*
    PmtDIG35Accessor::getDOMNode( const char* fullDIG35Key, bool create )
{
    XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* node = NULL;
    XERCES_CPP_NAMESPACE_QUALIFIER DOMElement* elementNode = NULL;
    
    XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* doc = mDoc;
    
    SPmtString fullKey = fullDIG35Key;
    SPmtString key;
    SPmtString::size_type pos;
    
    // get the first part of the full key (until the first .) and assign it to key
    pos = fullKey.find(".");
    key = fullKey.substr(0, pos);
    
    // set full key to be the remaining part of the original full key
    if (pos != SPmtString::npos)
        fullKey = fullKey.substr(pos + 1);
    else
        fullKey = "";
    
    // make sure we do have a key
    if ( !key.empty() )
    {
        XMLCh* xmlKey = XERCES_CPP_NAMESPACE_QUALIFIER 
            XMLString::transcode( key.c_str() );
        elementNode = (XERCES_CPP_NAMESPACE_QUALIFIER DOMElement*)
            doc->getElementsByTagName(xmlKey)->item(0);
        delete xmlKey;
        
        if ( elementNode==NULL && create )
        {
            XERCES_CPP_NAMESPACE_QUALIFIER DOMElement* rootElem = 
                (XERCES_CPP_NAMESPACE_QUALIFIER DOMElement*)
                doc->getElementsByTagName(
                dig35RootElement.getXMLCh() )->item(0);
            
            if ( rootElem!=NULL )
            {
                XMLCh* elementName = XERCES_CPP_NAMESPACE_QUALIFIER 
                    XMLString::transcode( key.c_str() );
                elementNode = doc->createElement( elementName );
                delete elementName;
                rootElem->appendChild( elementNode );
            }
        }
    
        pos = fullKey.find(".");
        key = fullKey.substr(0, pos);
        if (pos != SPmtString::npos)
            fullKey = fullKey.substr(pos + 1);
        else
            fullKey = "";
    }
    
    // continue going through the full key
    while ( !key.empty() && elementNode!=NULL )
    {
        XMLCh* xmlKey = XERCES_CPP_NAMESPACE_QUALIFIER 
            XMLString::transcode( key.c_str() );
        XERCES_CPP_NAMESPACE_QUALIFIER DOMElement* parent = elementNode;
        elementNode = (XERCES_CPP_NAMESPACE_QUALIFIER DOMElement*)
            doc->getElementsByTagName(xmlKey)->item(0);
        delete xmlKey;
        
        if ( elementNode==NULL && create )
        {
            XMLCh* elementName = XERCES_CPP_NAMESPACE_QUALIFIER 
                XMLString::transcode( key.c_str() );
            elementNode = doc->createElement( elementName );
            delete elementName;
            parent->appendChild( elementNode );
        }
        
        pos = fullKey.find(".");
        key = fullKey.substr(0, pos);
        if (pos != SPmtString::npos)
            fullKey = fullKey.substr(pos + 1);
        else
            fullKey = "";
    }
    
    node = elementNode;
    return node;
}
#endif

#ifdef PMT_USE_MSXML
// get the DOM node specified by the fullDIG35Key
IXMLDOMNode* PmtDIG35Accessor::getDOMNode( const char * fullDIG35Key, bool create )
{
    IXMLDOMNode* returnValue = NULL;
    IXMLDOMElement* elementNode = NULL;

    SPmtString fullKey = fullDIG35Key;
    SPmtString key;
    SPmtString::size_type pos;
    
    // get the first part of the full key (until the first .) and assign it to key
    pos = fullKey.find(".");
    key = fullKey.substr(0, pos);
    
    // set full key to be the remaining part of the original full key
    if (pos != SPmtString::npos)
        fullKey = fullKey.substr(pos + 1);
    else
        fullKey = "";
    
    // make sure we do have a key
    if ( !key.empty() )
    {
        PmtSAXString temp(key);
        BSTR xmlKey(temp.getXMLCh());
        IXMLDOMNodeList * list = NULL;
        
        mDoc->getElementsByTagName(xmlKey, &list);
        list->nextNode((IXMLDOMNode**)&elementNode);

        if ( elementNode==NULL && create )
        {
            BSTR dig35Root(dig35Metadata.getXMLCh());
            list = NULL;
            mDoc->getElementsByTagName(dig35Root,&list);
            IXMLDOMElement* rootElem = NULL;
            list->nextNode((IXMLDOMNode**)&rootElem);

            if ( rootElem!=NULL )
            {
                mDoc->createElement(xmlKey, &elementNode);
                IXMLDOMNode* newChild = NULL;
                rootElem->appendChild( elementNode, &newChild );
            }
        }

        pos = fullKey.find(".");
        key = fullKey.substr(0, pos);
        if (pos != SPmtString::npos)
            fullKey = fullKey.substr(pos + 1);
        else
            fullKey = "";
    }
    
    // continue going through the full key
    while ( !key.empty() && elementNode!=NULL )
    {
        PmtSAXString temp(key);
        BSTR xmlKey(temp.getXMLCh());
        IXMLDOMNodeList * list = NULL;

        IXMLDOMElement* parent = elementNode;
        mDoc->getElementsByTagName(xmlKey, &list);
        list->nextNode((IXMLDOMNode**)&elementNode);

        if ( elementNode==NULL && create )
        {
            mDoc->createElement(xmlKey, &elementNode);
            IXMLDOMNode* newChild = NULL;
            parent->appendChild( elementNode, &newChild );
        }
        
        pos = fullKey.find(".");
        key = fullKey.substr(0, pos);
        if (pos != SPmtString::npos)
            fullKey = fullKey.substr(pos + 1);
        else
            fullKey = "";
    }
    
    returnValue = elementNode;

    return returnValue;
}
#endif

// check if given key is that of a DIG35 metadata block
bool PmtDIG35Accessor::isMetadataBlockKey( const XMLCh* key )
{
    bool success = false;

#ifdef PMT_USE_XERCES_2
    if ( XERCES_CPP_NAMESPACE_QUALIFIER XMLString::equals(key,
        dig35BasicImageParam.getXMLCh() ) )
        success = true;
    else if ( XERCES_CPP_NAMESPACE_QUALIFIER XMLString::equals(key,
        dig35ImageCreation.getXMLCh() ) )
        success = true;
    else if ( XERCES_CPP_NAMESPACE_QUALIFIER XMLString::equals(key,
        dig35ContentDescription.getXMLCh() ) )
        success = true;
    else if ( XERCES_CPP_NAMESPACE_QUALIFIER XMLString::equals(key,
        dig35History.getXMLCh() ) )
        success = true;
    else if ( XERCES_CPP_NAMESPACE_QUALIFIER XMLString::equals(key,
        dig35IPR.getXMLCh() ) )
        success = true;
#endif

#ifdef PMT_USE_MSXML
    PmtSAXString tmpKey(key);
    BSTR tempKey(tmpKey.getXMLCh());
    BSTR dig35BIP(dig35BasicImageParam.getXMLCh());
    BSTR dig35IC(dig35ImageCreation.getXMLCh());
    BSTR dig35CD(dig35ContentDescription.getXMLCh());
    BSTR dig35H(dig35History.getXMLCh());
    BSTR dig35IPR(dig35IPR.getXMLCh());

    if(wcscmp(tempKey,dig35BIP)==0)
        success = true;
    else if(wcscmp(tempKey,dig35IC)==0)
        success = true;
    else if(wcscmp(tempKey,dig35CD)==0)
        success = true;
    else if(wcscmp(tempKey,dig35H)==0 )
        success = true;
    else if(wcscmp(tempKey,dig35IPR)==0)
        success = true;
#endif

    return success;
}


// end of PMT_DIG35 && (PMT_USE_XERCES_2 || PMT_USE_MSXML)
#endif
