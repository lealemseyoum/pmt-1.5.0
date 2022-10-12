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
 * Creation Date: August 2003
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
#include "PmtCoreCommon.h"
#include "PmtCompositeMetadata.h"
#include "PmtMetadataT.h"
#include "PmtXMLInstanceParser.h"
#include "PmtAttributeT.h"

// for a global lock against Xerces...
#include "PmtDefinitionInterpreter.h"

//! Constructor
PmtXMLInstanceParser::PmtXMLInstanceParser(istream &in)
:mInStream(&in),mUseFile(false)
{
    mRootMd = PmtMetadata::create();
};

//! Constructor
PmtXMLInstanceParser::PmtXMLInstanceParser(SPmtString filename)
:mInStream(NULL),mUseFile(true),mFileName(filename)
{
    mRootMd = PmtMetadata::create();
};

//=============================================================================
#ifndef PMT_USE_NO_XML_PARSER

// These are the includes for parsing the file with SAX2 interface.
#include "PmtSAX2.h"

// Constants required for querying the XML
const char *const PMT_TRUE = "true";
const char *const PMT_NILLABLE_TAG = "nillable";
const char *const PMT_INSTANCE_NILLABLE_TAG = "xsi:nil";
PmtSAXString gNillableTag(PMT_INSTANCE_NILLABLE_TAG);


// The callback class for the SAX2 parser to call while parsing 
class PmtXMLInstanceSAX2Handler : public PmtSAX2DefaultHandler
{
    public:

        // Constructor (requires a PmtTranslationTable pointer)
        PmtXMLInstanceSAX2Handler(PmtMetadataPtr root) 
            : mRootMd(root), mNoErrors(true) 
        {
#if defined(PMT_USE_XERCES)
            // Necessary for Xerces implementations...
            XMLPlatformUtils::Initialize();
#endif
        };

        // Standard Deconstructor
        virtual ~PmtXMLInstanceSAX2Handler()
        {
#if defined(PMT_USE_XERCES)
            // Necessary for Xerces implementations...
            XMLPlatformUtils::Terminate();
#endif
        };

        // The callback function for the start of a new element.
        void startElement(  const   XMLCh* const    uri,
                            const   XMLCh* const    localname,
                            const   XMLCh* const    qname,
                            const PmtSAX2Attributes& attributes);

        // The callback function for the end of an element.
        void endElement( const XMLCh* const namespaceUri,
                         const XMLCh* const localName,
                         const XMLCh* const qname);
                         
        // The callback function for the characters within an element.
        void xcharacters( const XMLCh* const chars,
                         const unsigned int length);
                         
        inline bool hadNoErrors()
        {
            return mNoErrors;
        };
                         
    private:
        
        // The root metadata pointer
        PmtMetadataPtr mRootMd;
        
        // The current metadata pointer that we're working on
        PmtMetadataPtr mCurrentMd;

        // Error reporting....
        // false if we encountered errors while parsing
        bool mNoErrors;
        
        // The wild wicked ways of my past
        void verifyNillableAttr(PmtSAXString &attrName,
                                PmtSAXString &attrValue,
                                PmtMetadataPtr md);
                                          
        // Add attributes that weren't in the schema    
        void addNonSchemaAttributes(PmtMetadataPtr md,
                                    PmtSAXString &attrName,
                                    PmtSAXString &attrValue);
};


// The callback function for the start of a new element.
// It should be noted here that this uses a significantly loser 
// interpretation of the XML.
void PmtXMLInstanceSAX2Handler::startElement(    
                    const   XMLCh* const    uri,
                    const   XMLCh* const    localname,
                    const   XMLCh* const    qname,
                    const PmtSAX2Attributes& attributes)
{
    // First, translate the name to something we can use
    PmtSAXString nodeName(localname);

    // We should already have a valid root node!!!!!
    if ( nodeName.getString() == ROOT_ELEMENT_NAME )
    {
        mCurrentMd = mRootMd;
    }
    else if (mCurrentMd)
    {
        // What about multiple instances of a data item?
        mCurrentMd = mCurrentMd->getMetadatum(nodeName.getString(),true);
    }

    if (mCurrentMd)
    {
      // Check for attributes
      int len;
      PMT_GETSAX2ATTR_LEN(attributes,len);

      if (len > 0)
      {
        PmtAttributeMap attributeMap = mCurrentMd->getAttributes(false);

        for (int index=0; (index < len) ; index++)
        {
          PmtSAXString * attrName =
              PmtGetSAX2AttributeLocalName(&attributes,index);

          // get the value
          const XMLCh * theAttrValue;
          PMT_GETSAX2ATTR_VALUE(attributes,index,theAttrValue);
          PmtSAXString attrValue(theAttrValue);

          // make sure it's not a nillable attribute
          if (attrName->getString() != gNillableTag.getString())
          {
              PmtAttributePtr mdAttr = 
                         mCurrentMd->getAttribute(attrName->getString(),true);

              if (!mdAttr)
              {
                  // this is required for attributes not declared in the 
                  //  schema & the root attributes.
                  addNonSchemaAttributes(mCurrentMd, *attrName, attrValue);
              }
              else
              {
                  mdAttr->setValueStr(
#ifdef _UNICODE
                                      attrValue.getXMLCh()
#else
                                      attrValue.getString()
#endif
                  );
              }
          }
          else
          { 
              // set the nillable atribute to the PMT tree.
              verifyNillableAttr(*attrName, attrValue, mCurrentMd);
          }
          
          delete attrName;
        }
      }
    }
};

// The callback function for the characters within an element.
void PmtXMLInstanceSAX2Handler::xcharacters( const XMLCh* const chars,
                                            const unsigned int length)
{
    if (mCurrentMd)
    {
#ifdef _UNICODE
        wstring z(chars);
        mCurrentMd->setValueStr(z.substr(0,length));
#else
        // First, translate the name to something we can use
        PmtSAXString theValue(chars);
        mCurrentMd->setValueStr(theValue.getString().substr(0,length));
#endif
    }
}
                 
// The callback function for the end of an element.
void PmtXMLInstanceSAX2Handler::endElement( const XMLCh* const namespaceUri,
                                            const XMLCh* const localName,
                                            const XMLCh* const qname)
{
    if (mCurrentMd)
        mCurrentMd = (PmtMetadata*)mCurrentMd->parent();
}

/*******************************************************************************
*  Method: verifyNillableAttr                                                  *
*  Input Parameters: SPmtString nillable,DOMNode attr,                         *
*                    PmtAttributeMap attributeMap, PmtMetadataPtr md           *
*  Functionality: This function contains the logic to verify that the nillable *
*                 method is used correctly in the instance doc.if NOT          *
*                 nillable="true" in the schema, then the instance cannot have *
*                 the xsi:nil="true" for that element.                         *
*  Output: NONE                                                                *
*******************************************************************************/
void PmtXMLInstanceSAX2Handler::verifyNillableAttr(PmtSAXString &attrName,
                                                   PmtSAXString &attrValue,
                                                   PmtMetadataPtr md)
{

    // SMF -- This does nothing like the comment  below says.
    // if the schema says that the element cannot be nill, where as the 
    // instance document specifies it as nill,throw an error.
    // apart from the validation check, see also if there is a value in the 
    // instance document. If there is , then add it as an attribute to the 
    // PMTMetadata current node.

    if ((md->getNillable() == PMT_TRUE)
             && (attrValue.getString() == PMT_TRUE))
    {
        // add it to the PMTMetadata, so that it gets written to the output 
        // file, if a write is performed. This is different than the rest of 
        // the attributes because, nillable is a built in attribute (we never 
        // encounter a xsd:attribute tag for it. But unlike the other built in 
        // tags like the min, max, this needs to be written out to the instance
        // document as well, hence we need to add it to the attributeFactoy and
        // the map.
        // PmtAttrFactory* factory = md->getAttrFactory();
        PmtAttributePtr pmtAttr =
            new PmtAttributeT < SPmtString > (PMT_NILLABLE_TAG, "type",
                                              "<SPmtString>", "", "", "");
        pmtAttr->setValueStr(
#ifdef _UNICODE
                                   attrValue.getXMLCh()
#else
                                   attrValue.getString()
#endif
        );
        md->setAttribute(PMT_INSTANCE_NILLABLE_TAG, pmtAttr);
    }
}


// Add attributes that weren't in the schema    
void PmtXMLInstanceSAX2Handler::addNonSchemaAttributes(PmtMetadataPtr md,
                                                       PmtSAXString &attrName,
                                                       PmtSAXString &attrValue)
{
    PmtAttrFactory *factory = md->getAttrFactory();
    PmtAttributePtr pmtAttr;

    SPmtString fullKey = md->fullKey(false);
    fullKey += ":";

    fullKey += attrName.getString();
    pmtAttr =
        new PmtAttributeT < SPmtString > (fullKey, "type", "<SPmtString>", "",
                                          "", "");

    // add to the factory.
    if (factory->addEntry(fullKey, pmtAttr) == false)
    {
        // There's a chance that the item could have been inserted 
        // by another thread just as we were trying to.  So we need
        // to make sure it isn't in there before throwing an error.
        pmtAttr = factory->get(fullKey);
        if (!pmtAttr)
            mNoErrors = false;
    }

    pmtAttr->setValueStr(
#ifdef _UNICODE
                                   attrValue.getXMLCh()
#else
                                   attrValue.getString()
#endif
    );
    md->setAttribute(attrName.getString(), pmtAttr);
}


// end if NO XML PARSER
#endif

//=============================================================================

// Routine to set up and start the parser
bool PmtXMLInstanceParser::parse()
{
#ifndef PMT_USE_NO_XML_PARSER
    bool returnValue = false;
	
    // Protect us from XERCES!!!!
    EK_GUARD(EK_DEFAULT_MUTEX, PmtLogicalDefinitionInterpreter::loadLock());
	
    //// create the SAX2 handler for SAX2 callbacks
    PmtXMLInstanceSAX2Handler handler(mRootMd);

    //// Create the parser and set the handler
    PmtSAX2XMLReader* parser = PmtCreateSAX2XMLReader();
    parser->setContentHandler(&handler);

    if (mUseFile)
    {
        SPmtString filePath = mFileName;

        // Verify the file and the path to it.
        if (PmtGetOSSpecificFilePathForXmlParser(filePath))
        {
        	// parse the file
        	PMT_SAX2_PARSEFILE(parser,filePath.c_str());

        	// delete the parser
        	PMT_DELETE_SAX2_PARSER(parser);
        }
    }
    else
    {
        // Set the input stream source
        PmtStreamInputSource * strmSrc = PmtSAX2CreateStreamInputSource(mInStream);
        
        // parse the stream
        parser->parse(*strmSrc);

        // delete the parser
        PMT_DELETE_SAX2_PARSER(parser);
        PMT_DELETE_STREAM_SOURCE(strmSrc);
    }
    
    if (!returnValue)
        returnValue = handler.hadNoErrors();
#else
    bool returnValue = true;
#endif
        
    return returnValue;
}




