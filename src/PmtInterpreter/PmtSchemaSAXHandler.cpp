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
 * Creation Date: 09/05/2003
 *
 * Original Author: 
 * Sam Fryer    samuel.fryer@kodak.com
 * 
 * Contributor(s): 
*/

#include "PmtConfig.h"

//=============================================================================
#ifndef PMT_USE_NO_XML_PARSER

#define PMT_SCHEMA_SAX_PARSER_CPP

#include "PmtSchemaSAXHandler.h"

//=============================================================================
// Local defines 
//=============================================================================
#define PMT_OCCURS_UNBOUNDED "unbounded"
// const int PMT_MAX_OCCURS = 0x7ffffff;
const int PMT_MAX_OCCURS = INT_MAX;
const SPmtString WorkingNameSpace("xsd:");
#define BRIEF_DOCUMENTATION "brief" 
#define FULL_DOCUMENTATION "detail"

//=============================================================================
// Method Definitions 
//=============================================================================

// The callback function for the start of a new element.
void PmtSchemaSAX2Handler::startElement(  const   XMLCh* const    uri,
                                          const   XMLCh* const    localName,
                                          const   XMLCh* const    qname,
                                          const PmtSAX2Attributes& attributes)
{
    PmtSAXString name(localName);
    if (!mInAnnotation)
    {
        switch (name.getString()[0])
        {
            case 'a':
                switch (name.getString()[1])
                {
                    case 'n': // <annotation>
                        mInAnnotation = true;
                        mNoErrors = true;
                        break;
                    case 't': // <attribute>
                        mNoErrors = processAttribute(name,attributes);
                        break;
                    default:
                        // Others can be there -- we just skip them.
                        mNoErrors = true;
                }
                break;
            case 'c':
                switch (name.getString()[1])
                {
                    case 'h': // <choice>
                        mNoErrors = processChoice(name,attributes);
                        break;
                    case 'o': // <complexType>
                        if (name.getString()[7] == 'T')
                            mNoErrors = processComplexType(name,attributes);
                        else // <complexContent>
                            // ComplexContent users need substantial help...
                            mNoErrors = true;
                        break;
                    default:
                        mNoErrors = false;
                }
                break;
            case 'd': // <doc> <documentation>
                mInAnnotation = true;
                break;
            case 'e':
                switch (name.getString()[1])
                {
                    case 'l': // <element>
                        mNoErrors = processElement(name,attributes);
                        break;
                    case 'n': // <enumeration>
                        if (mInRestriction)
                            mNoErrors = processEnumeration(name,attributes);
                        break;
                    case 'x': // <extension>
                        mNoErrors = processExtension(name,attributes);
                        break;
                    default:
                        mNoErrors = false;
                }
                break;
            case 'i': // <include> <import>
                mNoErrors = processIncludes(name,attributes);
                break;
            case 'l': // <list>
                mNoErrors = processList(name,attributes);
                break;
            case 'm': // <minInclusive> <maxInclusive>
                if (mInRestriction)
                    mNoErrors = processMinMaxInclusive(name,attributes);
                break;
            case 'r': // <restriction>
                mNoErrors = processRestriction(name,attributes);
                break;
            case 's':
                switch (name.getString()[1])
                {
                    case 'c': // <schema>
                        mNoErrors = processSchema(name,attributes);
                        break;
                    case 'e': // <sequence>
                        mNoErrors = processSequence(name,attributes);
                        break;
                    case 'i': // <simpleType>
                        if (name.getString()[6] == 'T')
                            mNoErrors = processSimpleType(name,attributes);
                        else // <simpleContent>
                            //Anyone using simpleContent should get 
                            //professional help
                            mNoErrors = processSimpleContent(name,attributes);                          
                        break;
                    default:
                        mNoErrors = false;
                }
                break;
            default:
                mNoErrors = false;
        } 
    }
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
    else
    {
        if (name.getString() == BRIEF_DOCUMENTATION)
            mInBriefDocumentation = true;
        else if (name.getString() == FULL_DOCUMENTATION)
            mInFullDocumentation = true;
    }
#endif
}

bool PmtSchemaSAX2Handler::processAttribute(const PmtSAXString &name, 
                                            const PmtSAX2Attributes& attributes)
{
    bool returnValue = true;
    PmtOrderAttribute * attribute = NULL;
    
    if (mComplexParent)
    {
        SPmtString ref = getAttribute("ref",attributes);
        if (ref.empty())
        {
            attribute = new PmtOrderAttribute();
            attribute->mName = getAttribute("name",attributes);
            attribute->mType = getAttribute("type",attributes);
            attribute->mUse = getAttribute("use",attributes);
            attribute->mFixed = getAttribute("fixed",attributes);
            attribute->mDefault = getAttribute("default",attributes);
            mComplexParent->mAttributeList.insert(
                PmtBaseOrder::PmtBaseOrderMapType::value_type(attribute->mName,
                                                              attribute));
        }
        else
        {
            attribute = 
                 (PmtOrderAttribute*)mComplexParent->findAttributeInList(ref);
        }
    }
    
    if (attribute)    
    { 
        if (mElement)
            mElement->addAttribute(attribute);
        else if (mComplexParent)
            mComplexParent->addAttribute(attribute);
        mCurrentAttribute = attribute;
    }
    else
        returnValue = false;

    return returnValue;
}

bool PmtSchemaSAX2Handler::processChoice(const PmtSAXString &name, 
                                         const PmtSAX2Attributes& attributes)
{
    bool returnValue = true;

    if (mComplexParent)
    {
        PmtOrderChoice * choice = new PmtOrderChoice();
        mComplexParent->addOrderingType(choice);
        mStragglerList.push_back(choice);
    }
    else
        returnValue = false;

    return returnValue;
}

bool PmtSchemaSAX2Handler::processComplexType(const PmtSAXString &name, 
                                           const PmtSAX2Attributes& attributes)
{
    bool returnValue = true;
    unsigned int tmpMin = mMinOccurs;
    unsigned int tmpMax = mMaxOccurs;

    SPmtString complexName = getAttribute("name",attributes);
    getMinAndMax(attributes);
    if (complexName.empty())
    {
        complexName = mCurrentElementName;
        if (mMinOccurs<tmpMin) mMinOccurs = tmpMin;
        if (mMaxOccurs<tmpMax) mMaxOccurs = tmpMax;
    }
    
    PmtComplexTypeDefinition * complexType = new PmtComplexTypeDefinition();
    complexType->mName = complexName;
    complexType->mType = getAttribute("type",attributes);
    complexType->mMinOccurs = mMinOccurs;
    complexType->mMaxOccurs = mMaxOccurs;
    if (mComplexParent)
    {
      complexType->mParent = mComplexParent;
      mComplexParent->addElement(complexType);
      mComplexParent->mElementList.insert(
        PmtBaseOrder::PmtBaseOrderMapType::value_type(complexName,complexType));
      mComplexParent->mTypeMap.insert(
        PmtBaseOrder::PmtBaseOrderMapType::value_type(complexName,complexType));
    }
    mComplexParent = complexType;

    return returnValue;
}

bool PmtSchemaSAX2Handler::processElement(const PmtSAXString &name, 
                                          const PmtSAX2Attributes& attributes)
{
    bool returnValue = true;
    
    SPmtString elementName = getAttribute("name",attributes);

    if (!elementName.empty())
    {
        mCurrentElementName = elementName;
        SPmtString elementType = getAttribute("type",attributes);
        getMinAndMax(attributes);
        
        if (!elementType.empty())
        {
            if ((elementType == "xsd:complexType") || 
                (elementType == "complexType"))
            {
                processComplexType(name,attributes);
            }
            else
            {
                PmtOrderElement * element = new PmtOrderElement(elementName,
                                                      elementType,
                                                      mMinOccurs,mMaxOccurs);
                element->mNillable = getAttribute("nillable",attributes);
                
                if (mComplexParent)
                {
                    mComplexParent->addElement(element);
                    mComplexParent->mElementList.insert(
                         PmtBaseOrder::PmtBaseOrderMapType::value_type(
                                                         elementName,element));
                }
                else
                {
                    delete element;
                    element = NULL;
                }
                mElement = element;
            }
        }
    } 
    else
    {
        // The only time we care about references are when they have a parent,
        // right?
        if (mComplexParent)
        {
            // This may be a reference element which would have the "ref" tag
            SPmtString ref = getAttribute("ref",attributes);

            if (!ref.empty())
            {
                PmtBaseOrder * element = mComplexParent->findElementInList(ref);
                if (element != NULL)
                    mComplexParent->addElement(element);
            }
        }
    }
    
    return returnValue;
}

bool PmtSchemaSAX2Handler::processEnumeration(PmtSAXString &name, 
                                           const PmtSAX2Attributes& attributes)
{
    bool returnValue = true;
    PmtOrderFacet * facet = NULL;
    
    if (mCurrentFacet != NULL)
    {
        facet = mCurrentFacet;
    }
    else
    {    
        facet = new PmtOrderFacet();
        facet->mName = WorkingNameSpace;
        facet->mName += name.getString();
        facet->mFixed = (getAttribute("fixed",attributes) == "true");
        mStragglerList.push_back(facet);
    }
    
    SPmtString theValue = getAttribute("value",attributes);
    const char * cv = theValue.c_str();
    facet->mValue.push_back(SPmtString(cv));

    if (mCurrentFacet == NULL)
    {
        mCurrentFacet = facet;
        if (mCurrentType != NULL) 
            mCurrentType->mFacets.push_back(facet);
        else if (mCurrentAttribute != NULL) 
            mCurrentAttribute->mFacets.push_back(facet);
        else if (mElement) 
            mElement->mFacets.push_back(facet);
        else if (mComplexParent) 
            mComplexParent->mFacets.push_back(facet);
    }
    
    return returnValue;
}

bool PmtSchemaSAX2Handler::processExtension(const PmtSAXString &name, 
                                            const PmtSAX2Attributes& attributes)
{
    bool returnValue = true;

    SPmtString base = getAttribute("base",attributes);

    if (!base.empty())
    {
        if (mElement)
            mElement->mReferenceType = base;
        if (mComplexParent)
            mComplexParent->mReferenceType = base;
        else
            returnValue = false;
    }
    else
        returnValue = false;

    return returnValue;
}

bool PmtSchemaSAX2Handler::processIncludes(const PmtSAXString &name, 
                                           const PmtSAX2Attributes& attributes)
{
    bool returnValue = true;
    
    SPmtString nameSpace = getAttribute("namespace",attributes);
    SPmtString location = getAttribute("schemaLocation",attributes);

    if (mParentParser)
    {
        mParentParser->mIncludes.push_back(location);
    }
    return returnValue;
}

bool PmtSchemaSAX2Handler::processList(const PmtSAXString &name, 
                                       const PmtSAX2Attributes& attributes)
{
    bool returnValue = true;
    
    if (!mCurrentType)
        returnValue = false;
    else
    {
        PmtSimpleTypeDefinition * s = mCurrentType;
        s->mReferenceType = getAttribute("itemType",attributes);
        s->mVector = true;
    }

    return returnValue;
}

bool PmtSchemaSAX2Handler::processMinMaxInclusive(PmtSAXString &name, 
                                           const PmtSAX2Attributes& attributes)
{
    bool returnValue = true;
    
    PmtOrderFacet * facet = new PmtOrderFacet();
    facet->mName = WorkingNameSpace;
    facet->mName += name.getString();
    mStragglerList.push_back(facet);

    SPmtString theValue = getAttribute("value",attributes);
    const char * cv = theValue.c_str();
    facet->mValue.push_back(SPmtString(cv));
    facet->mFixed = (getAttribute("fixed",attributes) == "true");

    if (mCurrentType != NULL) 
        mCurrentType->mFacets.push_back(facet);
    else if (mCurrentAttribute != NULL) 
        mCurrentAttribute->mFacets.push_back(facet);
    else if (mElement) 
        mElement->mFacets.push_back(facet);
    else if (mComplexParent) 
        mComplexParent->mFacets.push_back(facet);
    else
        returnValue = false;
    
    return returnValue;
}

bool PmtSchemaSAX2Handler::processRestriction(const PmtSAXString &name, 
                                           const PmtSAX2Attributes& attributes)
{
    bool returnValue = true;
    
    if (!mCurrentType)
        returnValue = false;
    else
    {
        PmtSimpleTypeDefinition * s = mCurrentType;
        s->mReferenceType = getAttribute("base",attributes);
        s->mVector = false;
        mInRestriction = true;
    }

    return returnValue;
}

bool PmtSchemaSAX2Handler::processSchema(const PmtSAXString &name, 
                                         const PmtSAX2Attributes& attributes)
{
    // We'll need to pay attention later for Namespace support!
    
    bool returnValue = false;
    if (mComplexParent == NULL)
    {
        mSchema = new PmtComplexTypeDefinition();
        mSchema->mName = "schema";
        mSchema->mType = "schema";
        mComplexParent = mSchema;
        returnValue = true;
    }
    return returnValue;
}

bool PmtSchemaSAX2Handler::processSequence(const PmtSAXString &name, 
                                           const PmtSAX2Attributes& attributes)
{
    bool returnValue = true;
    if (mComplexParent)
    {
        PmtOrderSequence * sequence = new PmtOrderSequence();
        mComplexParent->addOrderingType(sequence);
        mStragglerList.push_back(sequence);
    }
    else
        returnValue = false;
    return returnValue;
}

bool PmtSchemaSAX2Handler::processSimpleContent(const PmtSAXString &name, 
                                           const PmtSAX2Attributes& attributes)
{
    bool returnValue = true;
    if (mComplexParent)
    {
        mElement = mComplexParent;
        mComplexParent = mComplexParent->mParent;
        mElement->mSwitchType = PmtBaseOrder::PMT_ORDER_ELEMENT;
        mConvertedComplexTypeToElement = true;
    }
    else
        returnValue = false;
    return returnValue;
}

bool PmtSchemaSAX2Handler::processSimpleType(const PmtSAXString &name, 
                                           const PmtSAX2Attributes& attributes)
{
    bool returnValue = true;

    if (mComplexParent)
    {
        unsigned int tmpMin = mMinOccurs;
        unsigned int tmpMax = mMaxOccurs;
        SPmtString simpleName = getAttribute("name",attributes);
        getMinAndMax(attributes);
        if (simpleName.empty())
        {
            simpleName = mCurrentElementName + ".SimpleType";
            if (mMinOccurs<tmpMin) mMinOccurs = tmpMin;
            if (mMaxOccurs<tmpMax) mMaxOccurs = tmpMax;

            PmtOrderElement * element = new PmtOrderElement(mCurrentElementName,
                                                  simpleName,
                                                  mMinOccurs,mMaxOccurs);
            mComplexParent->addElement(element);
            mComplexParent->mElementList.insert(
                       PmtBaseOrder::PmtBaseOrderMapType::value_type(
                                                mCurrentElementName, element));
            mElement = element;
        }

        if (!simpleName.empty())
        {
            PmtSimpleTypeDefinition * s = new PmtSimpleTypeDefinition();
            s->mReferenceType = getAttribute("itemType",attributes);
            mCurrentType = s;

            mComplexParent->addElement(s);
            mComplexParent->mTypeMap.insert(
                       PmtBaseOrder::PmtBaseOrderMapType::value_type(
                                                                simpleName,s));
        }
        else
            returnValue = false;
    }
    else
        returnValue = false;

    return returnValue;
}

SPmtString PmtSchemaSAX2Handler::getAttribute(const char *name, 
                                           const PmtSAX2Attributes& attributes)
{
    // Check for attributes
    bool found = false;
    SPmtString value;
    int len;
    PMT_GETSAX2ATTR_LEN(attributes,len);

    if (len > 0)
    {
      for (int index=0; (index < len) && !found ; index++)
      {
        PmtSAXString * attrName = PmtGetSAX2AttributeLocalName(&attributes,
                                                               index);
        if (attrName->getString() == name)
        {
            found = true;
            // get the value
            const XMLCh * theAttrValue;
            PMT_GETSAX2ATTR_VALUE(attributes,index,theAttrValue);
            PmtSAXString attrValue(theAttrValue);
            value = attrValue.getString();
        }

        delete attrName;
      }
    }
    return value;
}
 
void PmtSchemaSAX2Handler::getMinAndMax(const PmtSAX2Attributes& attributes)
{
    // Set the defaults.
    mMinOccurs = 0;
    mMaxOccurs = 1;

    // Get minOccurs
    SPmtString occurs = getAttribute("minOccurs",attributes);
    if (!occurs.empty())
    {
        mMinOccurs = atoi(occurs.c_str());
    }

    // Get maxOccurs
    occurs = getAttribute("maxOccurs",attributes);
    if (!occurs.empty())
    {
        if (occurs == PMT_OCCURS_UNBOUNDED)
            mMaxOccurs = PMT_MAX_OCCURS;
        else          
            mMaxOccurs = atoi(occurs.c_str());
        if (mMaxOccurs < 1) mMaxOccurs = 1;
        if (mMaxOccurs < mMinOccurs) mMaxOccurs = mMinOccurs;
    }
}
       
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
// The callback function for the characters inbetween
void PmtSchemaSAX2Handler::characters(const XMLCh* const chars,
                                      const unsigned int length)
{
    if ((mInBriefDocumentation) || (mInFullDocumentation))
    {
        PmtBaseOrder * item = NULL;
        if (mElement != NULL) item=mElement;
        else if (mCurrentType != NULL) item=mCurrentType;
        else if (mComplexParent != NULL) item=mComplexParent;

        if (item != NULL)
        {
            PmtSAXString description(chars);
            SPmtString asciiDocs = description.getString();
            unsigned int i = 0;
            unsigned int wsCount = 0;
            for (i=0;i<asciiDocs.length(); i++)
            {
                if ((asciiDocs[i] == '\t') || (asciiDocs[i] == '\n'))
                    asciiDocs[i] = ' ';
                if (asciiDocs[i] == ' ')
                {
                    wsCount++;
                }
                else
                {
                    if (wsCount > 2)
                        asciiDocs = asciiDocs.substr(0,i-wsCount) + 
                                                        asciiDocs.substr(i);
                    wsCount = 0;
                }
            }
            if (wsCount == asciiDocs.length())
                asciiDocs = "";
            if (mInBriefDocumentation)
            {
               if (!item->mBriefDocumentation.empty())
                   item->mBriefDocumentation += ' ';
               item->mBriefDocumentation += asciiDocs;
            }
            else
            {
               if (!item->mFullDocumentation.empty())
                   item->mFullDocumentation += ' ';
               item->mFullDocumentation += asciiDocs;
            }
        }
    }
}                                          
#endif

// The callback function for the end of an element.
void PmtSchemaSAX2Handler::endElement( const XMLCh* const namespaceUri,
                                       const XMLCh* const localName,
                                       const XMLCh* const qname)
{
    PmtSAXString name(localName);
    if (!mInAnnotation)
    {
        switch (name.getString()[0])
        {
            case 'a': // <attribute>
                mCurrentAttribute = NULL;
                break;
            case 'c':
                // If we're ending a complexType ....
                if (name.getString()[7] == 'T')
                {
                    if (mConvertedComplexTypeToElement)
                        mConvertedComplexTypeToElement  = false;
                    else
                    {
                        mElement = NULL;
                        if (mComplexParent)
                            mComplexParent = mComplexParent->mParent;
                    }
                }
                break;
            case 'e':
                // If we're ending am element ....
                if (name.getString()[1] == 'l')
                {
                    mElement = NULL;
                }
                break;
            case 'r':
                mInRestriction = false;
                mCurrentFacet = NULL;
                break;
            case 's':
                mCurrentType = NULL;
                break;
            default:
                break;
        }
    }
    else
    {
        // if we're ending an annotation block
        if (name.getString() == "annotation")
        {
            mInAnnotation = false;
            mInBriefDocumentation = false;
            mInFullDocumentation = false;
        }
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
        else if (name.getString() == BRIEF_DOCUMENTATION)
            mInBriefDocumentation = false;
        else if (name.getString() == FULL_DOCUMENTATION)
            mInFullDocumentation = false;
#endif
    }
}

// end #ifndef PMT_USE_NO_XML_PARSER
#endif 
//=============================================================================


