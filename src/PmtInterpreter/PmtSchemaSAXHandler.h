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
 * Creation Date: 09/24/2003
 *
 * Original Author: 
 * Sam Fryer    samuel.fryer@kodak.com
 * 
 * Contributor(s): 
*/

#ifndef PMT_USE_NO_XML_PARSER

// This should only be used internally to PMT...
#ifdef PMT_SCHEMA_SAX_PARSER_CPP

#ifndef PMT_SCHEMA_SAX_HANDLER_H
#define PMT_SCHEMA_SAX_HANDLER_H

// These are the includes for parsing the file with SAX2 interface.
#include "EkCompiler.h"
#include "PmtSAX2.h"
#include "PmtOrder.h"
#include "PmtSchemaSAXParser.h"

//=============================================================================
// Our SAX2 Handler class
//=============================================================================
// The callback class for the SAX2 parser to call while parsing 
class PmtSchemaSAX2Handler : public PmtSAX2DefaultHandler
{
    public:

        // Constructor (requires a PmtSchemaSAXParser pointer)
        PmtSchemaSAX2Handler(PmtSchemaSAXParser * theTopParser) 
            : mNoErrors(true),mInAnnotation(false),
              mInBriefDocumentation(false),mInFullDocumentation(false),
              mCurrentType(NULL),mComplexParent(NULL),
              mElement(NULL),mCurrentAttribute(NULL),mSchema(NULL),
              mCurrentFacet(NULL),mParentParser(theTopParser),
              mMinOccurs(0),mMaxOccurs(1),mConvertedComplexTypeToElement(false)
        {
#if defined(PMT_USE_XERCES)
            // Necessary for Xerces implementations...
            XMLPlatformUtils::Initialize();
#endif
        };

        // Standard Deconstructor
        virtual ~PmtSchemaSAX2Handler()
        {
            if (mSchema != NULL)
                delete mSchema;
                
            // Destroy all the stragglers
            unsigned int i = 0;
            for (i=0;i<mStragglerList.size();i++)
                delete mStragglerList[i];

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

#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
        // The callback function for the characters inbetween
        void characters(    const   XMLCh* const chars,
                            const   unsigned int length);
#endif
                         
        // The callback function for the end of an element.
        void endElement( const XMLCh* const namespaceUri,
                         const XMLCh* const localName,
                         const XMLCh* const qname);
                  
        // Return whether or not we encountered any errors while parsing...   
        inline bool hadNoErrors()
        {
            return mNoErrors;
        };
                       
        inline PmtComplexTypeDefinition * getSchema()
        {
            return mSchema;
        };
        
    private:
        
        bool mNoErrors;                             // Errors encountered?
        bool mInAnnotation;                         // Ignore all elements
        bool mInBriefDocumentation;                 // Ignore all elements
        bool mInFullDocumentation;                  // Ignore all elements
        bool mInRestriction;                        // in a restriction block
        PmtSimpleTypeDefinition *mCurrentType;      // current type we're on
        SPmtString mCurrentElementName;             // current element name
        PmtComplexTypeDefinition * mComplexParent;  // current parent
        PmtOrderElement * mElement;                 // current element
        PmtOrderAttribute * mCurrentAttribute;      // current Attribute
        PmtComplexTypeDefinition * mSchema;         // The top of the schema
        PmtOrderFacet * mCurrentFacet;              // current facet
        PmtSchemaSAXParser * mParentParser;         // caller's pointer
        unsigned int mMinOccurs;                    // current minOccurs 
        unsigned int mMaxOccurs;                    // current maxOccurs
        bool mConvertedComplexTypeToElement;
        PmtBaseOrder::PmtBaseOrderList mStragglerList; // list of anything
                                                       //    that needs to be
                                                       //    deleted at the end

        // These are the routines to actually process the schema constructs
        bool processAttribute(const PmtSAXString &name, 
                              const PmtSAX2Attributes& attributes);
        bool processChoice(const PmtSAXString &name, 
                           const PmtSAX2Attributes& attributes);
        bool processComplexType(const PmtSAXString &name, 
                                const PmtSAX2Attributes& attributes);
        bool processElement(const PmtSAXString &name, 
                            const PmtSAX2Attributes& attributes);
        bool processEnumeration(PmtSAXString &name, 
                                const PmtSAX2Attributes& attributes);
        bool processExtension(const PmtSAXString &name, 
                              const PmtSAX2Attributes& attributes);
        bool processIncludes(const PmtSAXString &name, 
                             const PmtSAX2Attributes& attributes);
        bool processList(const PmtSAXString &name, 
                         const PmtSAX2Attributes& attributes);
        bool processMinMaxInclusive(PmtSAXString &name, 
                                    const PmtSAX2Attributes& attributes);
        bool processRestriction(const PmtSAXString &name, 
                                const PmtSAX2Attributes& attributes);
        bool processSchema(const PmtSAXString &name, 
                           const PmtSAX2Attributes& attributes);
        bool processSequence(const PmtSAXString &name, 
                             const PmtSAX2Attributes& attributes);
        bool processSimpleContent(const PmtSAXString &name, 
                                  const PmtSAX2Attributes& attributes);
        bool processSimpleType(const PmtSAXString &name, 
                               const PmtSAX2Attributes& attributes);
        
        // method to extract the value of the attribute named "name", 
        // or return an empty string
        SPmtString getAttribute(const char *name, 
                                const PmtSAX2Attributes& attributes);
        
        // Convience method to set mMinOccurs and mMaxOccurs from the
        // given attributes (or to the defaults)
        void getMinAndMax(const PmtSAX2Attributes& attributes);
};


// PMT_SCHEMA_SAX_HANDLER_H
#endif

// PMT_SCHEMA_SAX_PARSER_CPP
#endif

// PMT_USE_NO_XML_PARSER
#endif

