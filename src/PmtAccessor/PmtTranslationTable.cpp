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

// The obligatory header files
#include "PmtMisc.h"
#include "PmtError.h"
#include "PmtTranslationTable.h"

// for a global lock against Xerces...
#include "PmtDefinitionInterpreter.h"


// Include the default pre-parsed translation table itself.
// By pre-parsing the default translation table, we
// can save about 3% to 8% on execution run times.
#include "PmtTranslationTbl.h"

//The static lock (blocks everyone ... really required now?)
EK_DEFAULT_MUTEX PmtTranslationTable::msLock;


#ifndef PMT_USE_NO_XML_PARSER

// These are the includes for parsing the file with SAX2 interface.
#include "PmtSAX2.h"

// Constants required for querying the XML
const char *const XERCES_TRANS_TABLE_ID = "PmtTranslationTableMemoryBufferID";
const char *const XML_TAG_TRANSLATION = "TRANSLATION";
const char *const XML_TAG_ENTRY = "ENTRY";
const char *const XML_ATTR_KEY = "Key";
const char *const XML_ATTR_URI = "uri";
PmtSAXString gXmlTagTranslation(XML_TAG_TRANSLATION);
PmtSAXString gXmlTagEntry(XML_TAG_ENTRY);
PmtSAXString gXmlAttrKey(XML_ATTR_KEY);
PmtSAXString gXmlAttrUri(XML_ATTR_URI);

// more constants required for querying the XML
const char * PMT_XML_ATTR_LOC        = "Location";
const char * PMT_XML_ATTR_TAG        = "Tag";
const char * PMT_XML_ATTR_TYPE       = "Type";
const char * PMT_XML_ATTR_TRANSLATOR = "Translator";
PmtSAXString gXmlAttrLoc(PMT_XML_ATTR_LOC);
PmtSAXString gXmlAttrTag(PMT_XML_ATTR_TAG);
PmtSAXString gXmlAttrType(PMT_XML_ATTR_TYPE);
PmtSAXString gXmlAttrTranslator(PMT_XML_ATTR_TRANSLATOR);

// The callback class for the SAX2 parser to call while parsing 
// the translation table
class PmtTranslationTableSAX2Handler : public PmtSAX2DefaultHandler
{
    public:

        // Constructor (requires a PmtTranslationTable pointer)
        PmtTranslationTableSAX2Handler(PmtTranslationTable * table) 
            : mTable(table) 
        {
        };

        // Standard Deconstructor
        virtual ~PmtTranslationTableSAX2Handler()
        {
        };

        // The callback function for the start of a new element.
        void startElement(    const   XMLCh* const    uri,
                            const   XMLCh* const    localname,
                            const   XMLCh* const    qname,
                            const PmtSAX2Attributes& attributes);

    private:
        
        // The translation table we're adding things to.
        PmtTranslationTable * mTable;
        
        // The current key that we're working on
        SPmtString mKey;

		// And the current key uri that we're working on
        SPmtString mUri;
        
};


// The callback function for the start of a new element.
// It should be noted here that this uses a significantly loser 
// interpretation of the XML Translation Table.  The following will be
// entirely read in without a hiccup:
/*
<TRANSLATION> 
 <TIFF Tag="270" Type="ascii" Location="IFD_MAIN" Translator="builtin2string"/>
<ENTRY Key="SceneContent.ImageDescription">
 <EXIF Tag="270" Type="ascii" Location="APP1_IFD0" Translator="builtin2ascii"/>
 <TIFF Tag="270" Type="ascii" Location="IFD_MAIN" Translator="builtin2string"/>
</ENTRY>
 <TIFF Tag="270" Type="ascii" Location="IFD_MAIN" Translator="builtin2string"/>
</TRANSLATION>
<TRANSLATION> 
<ENTRY>
 <EXIF Tag="270" Type="ascii" />
 <TIFF Tag="270" Type="ascii" Location="IFD_MAIN" Translator="builtin2string"/>
</ENTRY>
</TRANSLATION>
 <TIFF Tag="270" Type="ascii" Location="IFD_MAIN" Translator="builtin2string"/>
*/
void PmtTranslationTableSAX2Handler::startElement(    
                    const   XMLCh* const    uri,
                    const   XMLCh* const    localname,
                    const   XMLCh* const    qname,
                    const PmtSAX2Attributes& attributes)
{
    // First, translate the name to something we can use
    PmtSAXString nodeName(localname);

    int len;
    PMT_GETSAX2ATTR_LEN(attributes,len);
    
    // if it's an ENTRY node, save off the key.
    if (nodeName.getString() == gXmlTagEntry.getString())
    {
        if (len > 0)
        {
            // loop through all the attributes until we find the key
            bool keyFound = false;
            bool uriFound = false;
            for (int index=0; (index < len) && (!keyFound) && (!uriFound) ; index++)
            {
                PmtSAXString * attrName =
                    PmtGetSAX2AttributeLocalName(&attributes,index);
                if (attrName->getString() == gXmlAttrKey.getString())
                {
                    // get the key value and save it off in mKey
                    const XMLCh * theAttrValue;
                    PMT_GETSAX2ATTR_VALUE(attributes,index,theAttrValue);
                    PmtSAXString attrValue(theAttrValue);
                    mKey = attrValue.getString();
                    keyFound = true;
                } 
                else if (attrName->getString() == gXmlAttrUri.getString())
                {
                    // get the key value and save it off in mKey
                    const XMLCh * theAttrValue;
                    PMT_GETSAX2ATTR_VALUE(attributes,index,theAttrValue);
                    PmtSAXString attrValue(theAttrValue);
                    mUri = attrValue.getString();
                    uriFound = true;
                } 
                delete attrName;
            }
            if (!keyFound) mKey = "";
        }
    }
    // if it's not a TRANSLATION node, then it must be a format specific node
    else if (nodeName.getString() != gXmlTagTranslation.getString())
    {
        // if there are no attributes, we can't do anything
        if (len > 0)
        {
            SPmtString sLoc;
            SPmtString sTag;
            SPmtString sType;
            SPmtString sTran;

            // look through all the attributes for the four that we use
            // (nothing else gets saved.)
            for (int index = 0; index < len ; index++)
            {
                PmtSAXString * attrName =
                    PmtGetSAX2AttributeLocalName(&attributes,index);
                const XMLCh * theAttrValue;
                PMT_GETSAX2ATTR_VALUE(attributes,index,theAttrValue);
                PmtSAXString attrValue(theAttrValue);
                if (attrName->getString() == gXmlAttrLoc.getString())
                    sLoc = attrValue.getString();
                else if (attrName->getString() == gXmlAttrTag.getString())
                    sTag = attrValue.getString();
                else if (attrName->getString() == gXmlAttrType.getString())
                    sType = attrValue.getString();
                else if (attrName->getString() == gXmlAttrTranslator.getString())
                    sTran = attrValue.getString();

                delete attrName;
            }

            // Create a new translation table element
            PmtTranslationTableElement *tableElement = 
                      new PmtTranslationTableElement(mKey.c_str(),
                                                     mUri.c_str(),
                                                     sTag.c_str(),
                                                     sType.c_str(),
                                                     sLoc.c_str(),
                                                     sTran.c_str());

            // get/create the column that this element belongs to
            PmtTranslationTableElements * theTable = 
                            mTable->getSubTable(nodeName.getString());

            if (theTable == NULL)
            {
                theTable = new PmtTranslationTableElements;
                mTable->mTranslationTable.insert(
                            PmtTranslationTables::value_type(
                            nodeName.getString(),theTable));
            }

            // insert the element
            theTable->push_back(tableElement);
        }
    }
};

// end if NO XML PARSER
#endif


// Routine to load the translation table from a file.
void PmtTranslationTable::load(const SPmtString & filename)
{
#ifndef PMT_USE_NO_XML_PARSER
    // Really needed since we're only adding stuff?  Yes -- it keeps threads
    // from jumping ahead and using an incomplete table.
    EK_GUARD(EK_DEFAULT_MUTEX, msLock);
#if defined(PMT_USE_XERCES)
    // Necessary for Xerces implementations...
    XMLPlatformUtils::Initialize();
#endif

    SPmtString filePath = filename;

    // Verify the file and the path to it.
    if (!PmtGetOSSpecificFilePathForXmlParser(filePath))
    {
        SPmtString message = "PmtAccessor::load could not locate file path: ";
        message += filePath.c_str();
        throw PmtError(PMT_ACCESSOR_FILE_NOT_FOUND, message, WHERE);
    }

    //// create the SAX2 handler for SAX2 callbacks
    PmtTranslationTableSAX2Handler handler(this);

    //// Create the parser and set the handler
    PmtSAX2XMLReader* parser = PmtCreateSAX2XMLReader();
    parser->setContentHandler(&handler);

    // parse the file
    PMT_SAX2_PARSEFILE(parser,filePath.c_str());

    // delete the parser
    PMT_DELETE_SAX2_PARSER(parser);

#if defined(PMT_USE_XERCES)
    // Necessary for Xerces implementations...
    XMLPlatformUtils::Terminate();
#endif

// end if NO XML PARSER
#endif
}


// Routine to load the translation table from a memory location.
void PmtTranslationTable::loadMemory(const char *translationTable)
{
#ifndef PMT_USE_NO_XML_PARSER
    // Really needed since we're only adding stuff?  Yes -- it keeps threads
    // from jumping ahead and using an incomplete table.
    EK_GUARD(EK_DEFAULT_MUTEX, msLock);
#if defined(PMT_USE_XERCES)
    // Necessary for Xerces implementations...
    XMLPlatformUtils::Initialize();
#endif

    // put it in a parseable format (note: no NULL characters allowed until end)
    MemBufInputSource * memSrc = 
        PmtGetSAX2MemorySource((const unsigned char *const) (translationTable),
                               strlen(translationTable) + 1,
                               PMT_TRANS_TABLE_ID);

    // create the SAX2 handler for SAX2 callbacks
    PmtTranslationTableSAX2Handler handler(this);

    // Create the parser and set the handler
    PmtSAX2XMLReader* parser = PmtCreateSAX2XMLReader ();
    parser->setContentHandler(&handler);

    //parse it
    parser->parse(*memSrc);

    // delete the parser
    PMT_DELETE_SAX2_PARSER(parser);

    // delete the memory source
    PMT_DELETE_SAX2_MEMSRC(memSrc);

#if defined(PMT_USE_XERCES)
    // Necessary for Xerces implementations...
    XMLPlatformUtils::Terminate();
#endif

// end if NO XML PARSER
#endif
}


// Routine to load the default translation table. 
void PmtTranslationTable::load()
{    
    int i = 0;  // Index for which key we're on
    unsigned int j = 0;  // Index for which element of the key we're on
    
    // Loop through all the keys.
    while (defaultTranslationTableArray[i].key != NULL)
    {
        // make addressing the element a little easier...
        const char ** element = (const char **)
           defaultTranslationTableArray[i].elements;

        // Loop through each element associated with the key
        while(element[j] != NULL)
        {

            // Create a new translation table element
            PmtTranslationTableElement *tableElement = 
                      new PmtTranslationTableElement(
                                        defaultTranslationTableArray[i].key,
                                        defaultTranslationTableArray[i].uri,
                                        element[j+1],
                                        element[j+2],
                                        element[j+3],
                                        element[j+4]);

            // get/create the column that this element belongs to
            PmtTranslationTableElements * theTable = 
                    getSubTable(element[j]);

            if (theTable == NULL)
            {
                theTable = new PmtTranslationTableElements;
                mTranslationTable.insert(
                            PmtTranslationTables::value_type(
                            element[j],theTable));
            }

            // insert the element
            theTable->push_back(tableElement);

            // Jump to the next set.
            j += 5;
        };
        
        // go on to the next key.
        i++;
        j=0;
    }
}

const CharPmtTT * PmtTranslationTable::getDefaultTranslationTable()
{ return defaultTranslationTableArray; }

