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
 * Creation Date: October 2003
 *
 * Original Author: 
 * Sam Fryer <samuel.fryer@kodak.com>
 *
 * Contributor(s):
 *
*/

// The obligatory header files
#include "PmtMisc.h"
#include "PmtMetadata.h"
#include "PmtAliasesParser.h"

#ifndef PMT_USE_NO_XML_PARSER

// These are the includes for parsing the file with SAX2 interface.
#include "PmtSAX2.h"

// Constants required for querying the XML
const char *const ALIAS_ROOT_TAG = "MetadataAliases";
const char *const ALIAS_TAG = "MetadataAlias";
const char *const ALIAS_KEY_ATTR_NAME = "AliasKey";
const char *const ALIAS_MEMBER_TAG = "AliasMember";
const char *const METADATA_KEY_ATTR_NAME = "MetadataKey";
const char *const XERCES_ALIAS_ID = "PmtAliasesMemoryBufferID";


// The callback class for the SAX2 parser to call while parsing 
// the translation table
class PmtAliasParserSAX2Handler : public PmtSAX2DefaultHandler
{
    public:

        // Constructor (requires a PmtAliasParser pointer)
        PmtAliasParserSAX2Handler()
        : mInProcessingMode(false),mNoErrors(true),mInAlias(false)
        {
        };

        // Standard Deconstructor
        virtual ~PmtAliasParserSAX2Handler()
        {
        };

        // The callback function for the start of a new element.
        void startElement(    const   XMLCh* const    uri,
                            const   XMLCh* const    localname,
                            const   XMLCh* const    qname,
                            const PmtSAX2Attributes& attributes);

        // The callback function for the end of a new element.
        void endElement(    const   XMLCh* const    uri,
                            const   XMLCh* const    localname,
                            const   XMLCh* const    qname);

        // Returns whether or not NO errors were incountered
        inline bool hasNoErrors()
        {
            return mNoErrors;
        }
        
    private:
    
        bool mInProcessingMode;        // Can we start processing?
        bool mNoErrors;                // No errors yet?
        bool mInAlias;                 // Currently processing an alias?
        
        SPmtString mCurrentAliasKey;   // Last alias encountered
        PmtKeyList mCurrentAliasList;  // List of MD keys associated with alias
       
};


// The callback function for the start of a new element.
void PmtAliasParserSAX2Handler::startElement(    
                    const   XMLCh* const    uri,
                    const   XMLCh* const    localname,
                    const   XMLCh* const    qname,
                    const PmtSAX2Attributes& attributes)
{
    // First, translate the name to something we can use
    PmtSAXString nodeName(localname);

    if (mInProcessingMode)
    {
        if (mInAlias)
        {
            if (nodeName.getString() == ALIAS_MEMBER_TAG)
            {
                // get the metadata key value and save it off in in the list
                const XMLCh * theAttrValue;
                PMT_GETSAX2ATTR_VALUE(attributes,(unsigned int)0,theAttrValue);
                PmtSAXString attrValue(theAttrValue);
                SPmtString aliasMdKey = attrValue.getString();

                if (!aliasMdKey.empty())
                    mCurrentAliasList.push_back(aliasMdKey);
            }
            else
                mNoErrors = false;
        }
        else if (nodeName.getString() == ALIAS_TAG)
        {
            // get the key value and save it off in mCurrentAliasKey
            const XMLCh * theAttrValue;
            PMT_GETSAX2ATTR_VALUE(attributes,(unsigned int)0,theAttrValue);
            PmtSAXString attrValue(theAttrValue);
            mCurrentAliasKey = attrValue.getString();
            
            if (!mCurrentAliasKey.empty())
                mInAlias = true;
        }
        else
            mNoErrors = false;
    }
    else
    {
        // See if we can start processing!!!
        if (nodeName.getString() == ALIAS_ROOT_TAG)
            mInProcessingMode = true;
    }
};

// The callback function for the end of a new element.
void PmtAliasParserSAX2Handler::endElement(    
                    const   XMLCh* const    uri,
                    const   XMLCh* const    localname,
                    const   XMLCh* const    qname)
{
    if (mInProcessingMode)
    {
        // First, translate the name to something we can use
        PmtSAXString nodeName(localname);

        if (nodeName.getString() == ALIAS_TAG)
        {
            // Get the aliases from PmtMetadata and set the new ones
            PmtAliases & aliasMap = PmtMetadata::getAliases();
            aliasMap.insert(mCurrentAliasKey, mCurrentAliasList);
            
            // Clear the current list
            mCurrentAliasList.clear();
            mInAlias = false;
        }
        else if (nodeName.getString() == ALIAS_ROOT_TAG)
            mInProcessingMode = false;
    }
}

// end PMT_USE_NO_XML_PARSER
#endif

// Routine to load the aliases from a file.
bool PmtAliasesParser::loadAliasFile(const char * fileName)
{
    bool returnValue = false;

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
       PmtAliasParserSAX2Handler handler;

       //// Create the parser and set the handler
       PmtSAX2XMLReader* parser = PmtCreateSAX2XMLReader();
       parser->setContentHandler(&handler);

       // parse the file
       PMT_SAX2_PARSEFILE(parser,filePath.c_str());

       // delete the parser
       PMT_DELETE_SAX2_PARSER(parser);

       returnValue = handler.hasNoErrors();
    }
    
#if defined(PMT_USE_XERCES)
    // Necessary for Xerces implementations...
    XMLPlatformUtils::Terminate();
#endif

// end if NO XML PARSER
#endif

    return returnValue;
}


// Routine to load the aliases from memory.
bool PmtAliasesParser::loadAliasMemory(const char * mem)
{
    bool returnValue = false;

#ifndef PMT_USE_NO_XML_PARSER

#if defined(PMT_USE_XERCES)
    // Necessary for Xerces implementations...
    XMLPlatformUtils::Initialize();
#endif

    // put it in a parseable format (note: no NULL characters allowed until end)
    MemBufInputSource * memSrc = 
        PmtGetSAX2MemorySource((const unsigned char *const) (mem),
                               strlen(mem) + 1,
                               XERCES_ALIAS_ID);

    // create the SAX2 handler for SAX2 callbacks
    PmtAliasParserSAX2Handler handler;

    // Create the parser and set the handler
    PmtSAX2XMLReader* parser = PmtCreateSAX2XMLReader ();
    parser->setContentHandler(&handler);

    //parse it
    parser->parse(*memSrc);

    // delete the parser
    PMT_DELETE_SAX2_PARSER(parser);

    // delete the memory source
    PMT_DELETE_SAX2_MEMSRC(memSrc);

    returnValue = handler.hasNoErrors();
    
#if defined(PMT_USE_XERCES)
    // Necessary for Xerces implementations...
    XMLPlatformUtils::Terminate();
#endif

// end if NO XML PARSER
#endif

    return returnValue;
}



