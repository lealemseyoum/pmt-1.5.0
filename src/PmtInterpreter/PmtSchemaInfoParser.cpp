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
 * Creation Date: October 2003
 *
 * Original Author: 
 * Sam Fryer <samuel.fryer@kodak.com>
 *
 * Contributor(s):
 *
*/

// Obligatory includes
#include "PmtMisc.h"
#include "PmtSchemaInfoParser.h"

const char *SCHEMA_INFO_TAG = "MetadataDefinitionBinding";
const char *SCHEMA_URI_TAG = "MetadataDefnFileURI";
const char *ROOT_ELEMENT_NAME_TAG = "RootElementName";
const char *TRANSLATION_TABLE_URI_TAG = "AssociatedTranslationTableURI";

// This class is a container class of all the information in a 
// "MetadataDefinitionBinding" -- the Schema URI, the Root Element's name,
// and the translation table URI.
class PmtSchemaInfoParserData
{
    public:
    
        // Standard constructor
        PmtSchemaInfoParserData()
        : mNext(NULL)
        {
        }
  
        // Standard deconstructor... since everything is self-destroying,
        // we just have to delete the next one down the line.
        ~PmtSchemaInfoParserData()
        {
            if (mNext != NULL) delete mNext;
        }
        
        SPmtString mSchemaURI;           // The schema URI
        SPmtString mRootElementName;     // The root element's name
        SPmtString mTranslationTableURI; // The trans table URI
        PmtSchemaInfoParserData * mNext; // next in the list
};

#ifndef PMT_USE_NO_XML_PARSER

// These are the includes for parsing the file with SAX2 interface.
#include "PmtSAX2.h"

// The callback class for the SAX2 parser to call while parsing 
// the translation table
class PmtSchemaInfoParserSAX2Handler : public PmtSAX2DefaultHandler
{
    public:

        // Constructor 
        PmtSchemaInfoParserSAX2Handler()
        : mInProcessingMode(false),mNoErrors(true),mInSchemaURI(false),
          mInRootElementName(false),mInTranslationTable(true),
          mDataTop(NULL),mCurrentData(NULL)
        {
        };

        // Standard Deconstructor
        virtual ~PmtSchemaInfoParserSAX2Handler()
        {
        };

        // The callback function for the start of a new element.
        void startElement(  const   XMLCh* const    uri,
                            const   XMLCh* const    localname,
                            const   XMLCh* const    qname,
                            const PmtSAX2Attributes& attributes);

        // The callback function for the characters inbetween
        void xcharacters(    const   XMLCh* const chars,
                            const   unsigned int length);
                         
        // The callback function for the end of a new element.
        void endElement(    const   XMLCh* const    uri,
                            const   XMLCh* const    localname,
                            const   XMLCh* const    qname);

        // Returns whether or not NO errors were incountered
        inline bool hasNoErrors()
        {
            return mNoErrors;
        }
        
        // Returns the data linked-list.
        inline PmtSchemaInfoParserData * getDataTop()
        {
            return mDataTop;
        }
        
    private:
    
        bool mInProcessingMode;        // Can we start processing?
        bool mNoErrors;                // No errors yet?
        bool mInSchemaURI;             // Parsing a SchemaURI tag?
        bool mInRootElementName;       // Parsing a RootElementName tag?
        bool mInTranslationTable;      // Parsing a TransTableURI tag?
        PmtSchemaInfoParserData * mDataTop;     // Our data linked-list
        PmtSchemaInfoParserData * mCurrentData; // Our current position 
       
};


// The callback function for the start of a new element.
void PmtSchemaInfoParserSAX2Handler::startElement(    
                    const   XMLCh* const    uri,
                    const   XMLCh* const    localname,
                    const   XMLCh* const    qname,
                    const PmtSAX2Attributes& attributes)
{
    // First, translate the name to something we can use
    PmtSAXString nodeName(localname);

    if (mInProcessingMode)
    {
        // Determine which tag this is, and set the proper bits
        // so that we put the trailing characters in the right place
        if (nodeName.getString() == SCHEMA_URI_TAG)
        {
            mInSchemaURI = true;
            mInRootElementName = false;
            mInTranslationTable = false;
        }
        else if (nodeName.getString() == ROOT_ELEMENT_NAME_TAG)
        {
            mInSchemaURI = false;
            mInRootElementName = true;
            mInTranslationTable = false;
        }
        else if (nodeName.getString() == TRANSLATION_TABLE_URI_TAG)
        {
            mInSchemaURI = false;
            mInRootElementName = false;
            mInTranslationTable = true;
        }
        else
            mNoErrors = false;
    }
    else
    {
        // See if we can start processing!!!
        if (nodeName.getString() == SCHEMA_INFO_TAG)
        {
            // Create a new container to parse items into...
            PmtSchemaInfoParserData * data = new PmtSchemaInfoParserData();
            if (mDataTop == NULL) 
            {
                mDataTop = data;
                mCurrentData = data;
            }
            else
            {
                mCurrentData->mNext = data;
                mCurrentData = data;
            }
                 
            mInProcessingMode = true;
        }
    }
};

// The callback function for the characters inbetween
void PmtSchemaInfoParserSAX2Handler::xcharacters(const   XMLCh* const chars,
                                                 const   unsigned int length)
{
    if ((mInProcessingMode) && (mCurrentData != NULL))
    {
        // First, translate the name to something we can use
        PmtSAXString nodeData(chars);
            
        if (mInSchemaURI)
            mCurrentData->mSchemaURI = nodeData.getString().substr(0,length);
        else if (mInRootElementName)
            mCurrentData->mRootElementName = nodeData.getString().substr(0,length);
        else if (mInTranslationTable)
            mCurrentData->mTranslationTableURI = nodeData.getString().substr(0,length);
    }
}

// The callback function for the end of a new element.
void PmtSchemaInfoParserSAX2Handler::endElement(    
                    const   XMLCh* const    uri,
                    const   XMLCh* const    localname,
                    const   XMLCh* const    qname)
{
    if (mInProcessingMode)
    {
        // First, translate the name to something we can use
        PmtSAXString nodeName(localname);

        if (nodeName.getString() == SCHEMA_URI_TAG)
            mInSchemaURI = false;
        else if (nodeName.getString() == ROOT_ELEMENT_NAME_TAG)
            mInRootElementName = false;
        else if (nodeName.getString() == TRANSLATION_TABLE_URI_TAG)
            mInTranslationTable = false;
        else if (nodeName.getString() == SCHEMA_INFO_TAG)
            mInProcessingMode = false;
    }
}

// end PMT_USE_NO_XML_PARSER
#endif

// Constructor that loads the info from a file.
PmtSchemaInfoParser::PmtSchemaInfoParser(const char * fileName)
{
    mListTop = NULL;

#ifndef PMT_USE_NO_XML_PARSER

#if defined(PMT_USE_XERCES)
    // Necessary for Xerces implementations...
    XMLPlatformUtils::Initialize();
#endif

    SPmtString filePath = fileName;

    // Verify the file and the path to it.
    if (PmtGetOSSpecificFilePathForXmlParser(filePath))
    {
        //// create the SAX2 handler for SAX2 callbacks
        PmtSchemaInfoParserSAX2Handler handler;

        //// Create the parser and set the handler
        PmtSAX2XMLReader* parser = PmtCreateSAX2XMLReader();
        parser->setContentHandler(&handler);

        // parse the file
        PMT_SAX2_PARSEFILE(parser,filePath.c_str());

        // delete the parser
        PMT_DELETE_SAX2_PARSER(parser);

        mListTop = handler.getDataTop();
    }
    
#if defined(PMT_USE_XERCES)
    // Necessary for Xerces implementations...
    XMLPlatformUtils::Terminate();
#endif

// end if NO XML PARSER
#endif

    mCurrentPos = mListTop;
}

PmtSchemaInfoParser::~PmtSchemaInfoParser()
{
    if (mListTop != NULL) delete mListTop;
}

// This method returns the contents of one element in our linked-list.
bool PmtSchemaInfoParser::getFirstSchemaInfoArgs(SPmtString &schemaURI,
                                               SPmtString &rootElementName,
                                               SPmtString &translationTableURI)
{
    bool returnValue = false;
    
    if (mCurrentPos != NULL)
    {
        returnValue = true;
        schemaURI = mCurrentPos->mSchemaURI;
        rootElementName = mCurrentPos->mRootElementName;
        translationTableURI = mCurrentPos->mTranslationTableURI;
        mCurrentPos = mCurrentPos->mNext;
    }
    
    return returnValue;
}


// This method returns the contents of one element in our linked-list.
// (duplicate of PmtSchemaInfoParser::getFirstSchemaInfoArgs)
bool PmtSchemaInfoParser::getNextSchemaInfoArgs(SPmtString &schemaURI,
                                               SPmtString &rootElementName,
                                               SPmtString &translationTableURI)
{
    bool returnValue = false;
    
    if (mCurrentPos != NULL)
    {
        returnValue = true;
        schemaURI = mCurrentPos->mSchemaURI;
        rootElementName = mCurrentPos->mRootElementName;
        translationTableURI = mCurrentPos->mTranslationTableURI;
        mCurrentPos = mCurrentPos->mNext;
    }
    
    return returnValue;
}


