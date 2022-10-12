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

#define PMT_SCHEMA_SAX_PARSER_CPP

// The obligatory header files
#include "PmtMisc.h"
#include "PmtError.h"
#include "PmtCoreCommon.h"
#include "PmtCompositeMetadata.h"
#include "PmtMetadataT.h"
#include "PmtAttributeT.h"
#include "PmtSchemaSAXParser.h"
#include "PmtOrder.h"
#include "PmtDefinitionTypes.h"
#include "PmtAttribute.h"
#include "PmtRootMetadata.h"
#include "PmtTranslationTable.h"

// This is the pre-parsed default schema
#include "PmtDefaultPreparsedSchema.h"
    

//=============================================================================
// Local defines 
//=============================================================================
enum
{
    PMT_SCHEMA_SAX_PARSER_USE_DEFAULT=0,
    PMT_SCHEMA_SAX_PARSER_USE_STREAM,
    PMT_SCHEMA_SAX_PARSER_USE_FILE
};

#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
#define PMT_DEFAULT_SCHEMA_INDEX_SIZE 5
#else
#define PMT_DEFAULT_SCHEMA_INDEX_SIZE 3
#endif

//=============================================================================
// Local functions 
//=============================================================================

// Stolen (and modified) from PmtItemInterpreter ...
PmtMetadataPtr createPrimitiveMetadata(SPmtString name,
                                       SPmtString mdtype,
                                       SPmtString nillable,
                                       int switchFactor)
{
    PmtMetadataPtr md;
    switch (switchFactor)
    {
        case PMT_MD_TYPE_INT8:
            md = new PmtMetadataT < char >(name, mdtype, 
                                           PMT_MD_TYPE_INT8,
                                           0, nillable);
            break;
        case PMT_MD_TYPE_VINT8:
            md = new PmtMetadataT < vector < char > > (name, mdtype,
                                                       PMT_MD_TYPE_VINT8,
                                                       vector < char >(1,0),
                                                       nillable);
            break;
        case PMT_MD_TYPE_UINT8:
            md = new PmtMetadataT < unsigned char >(name, mdtype,
                                                    PMT_MD_TYPE_UINT8,
                                                    0,
                                                    nillable);
            break;
        case PMT_MD_TYPE_VUINT8:
            md = new PmtMetadataT < vector < unsigned char > > (name,mdtype,
                                            PMT_MD_TYPE_VUINT8,
                                            vector < unsigned char >(1,0),
                                            nillable);
            break;
        case PMT_MD_TYPE_INT16:
            md = new PmtMetadataT < short >(name, mdtype, 
                                            PMT_MD_TYPE_INT16,
                                            0, nillable);
            break;
        case PMT_MD_TYPE_VINT16:
            md = new PmtMetadataT < vector < short > > (name, mdtype,
                                            PMT_MD_TYPE_VINT16,
                                            vector < short >(1,0),
                                            nillable);
            break;
        case PMT_MD_TYPE_UINT16:
            md = new PmtMetadataT < unsigned short >(name, mdtype,
                                                     PMT_MD_TYPE_UINT16,
                                                     0,
                                                     nillable);
            break;
        case PMT_MD_TYPE_VUINT16:
            md = new PmtMetadataT < vector < unsigned short > > (name,
                                            mdtype,
                                            PMT_MD_TYPE_VUINT16,
                                            vector < unsigned short >(1,0),
                                            nillable);
            break;
        case PMT_MD_TYPE_INT32:
            md = new PmtMetadataT < long >(name, mdtype, 
                                          PMT_MD_TYPE_INT32,
                                          0, nillable);
            break;
        case PMT_MD_TYPE_VINT32:
            md = new PmtMetadataT < vector < long > > (name, mdtype,
                                                      PMT_MD_TYPE_VINT32,
                                                      vector < long >(1,0),
                                                      nillable);
            break;
        case PMT_MD_TYPE_UINT32:
            md = new PmtMetadataT < unsigned long >(name, mdtype,
                                                   PMT_MD_TYPE_UINT32,
                                                   0, nillable);
            break;
        case PMT_MD_TYPE_VUINT32:
            md = new PmtMetadataT < vector < unsigned long > > (name,mdtype,
                                          PMT_MD_TYPE_VUINT32,
                                          vector < unsigned long >(1,0),
                                          nillable);
            break;
        case PMT_MD_TYPE_BOOL:
            md = new PmtMetadataT < bool > (name, mdtype,
                                            PMT_MD_TYPE_BOOL, false,
                                            nillable);
            break;
        case PMT_MD_TYPE_VBOOL:
            md = new PmtMetadataT < vector < bool > >(name, mdtype,
                                             PMT_MD_TYPE_VBOOL,
                                             vector < bool > (1, false),
                                             nillable);
            break;
        case PMT_MD_TYPE_FLOAT:
            md = new PmtMetadataT < float >(name, mdtype, 
                                            PMT_MD_TYPE_FLOAT,
                                            0.0, nillable);
            break;
        case PMT_MD_TYPE_VFLOAT:
            md = new PmtMetadataT < vector < float > > (name, mdtype,
                                            PMT_MD_TYPE_VFLOAT,
                                            vector < float >(1,0.0),
                                            nillable);
            break;
        case PMT_MD_TYPE_DOUBLE:
            md = new PmtMetadataT < double >(name, mdtype, 
                                             PMT_MD_TYPE_DOUBLE,
                                             0.0, nillable);
            break;
        case PMT_MD_TYPE_VDOUBLE:
            md = new PmtMetadataT < vector < double > > (name, mdtype,
                                             PMT_MD_TYPE_VDOUBLE,
                                             vector < double >(1,0.0),
                                             nillable);
            break;
        case PMT_MD_TYPE_STRING:
            md = new PmtMetadataT < string > (name, mdtype,
                                                  PMT_MD_TYPE_STRING, "",
                                                  nillable);
            break;
        case PMT_MD_TYPE_VSTRING:
            md = new PmtMetadataT < vector < string > >(name, mdtype,
                                                 PMT_MD_TYPE_VSTRING,
                                                 vector < string > (1, ""),
                                                 nillable);
        // !!! support WSTRING at some point
        //else if (type == PMT_TYPE_WSTRING)
        //{
        //    if (isVector)
        //        md = new PmtMetadataT<vector<wstring> >(name, mdtype, 
        //                        PMT_MD_TYPE_VWSTRING, 
        //                        vector<wstring>(1,static_cast<wstring>(L"")));
        //    else
        //        md = new PmtMetadataT<wstring>(name, mdtype, 
        //                                    PMT_MD_TYPE_WSTRING, wstring());
        //}

    } 
    
    return md;
}

//=============================================================================
#ifndef PMT_USE_NO_XML_PARSER

#include "PmtSchemaSAXHandler.h"


#ifdef PMT_WRITE_OUT_SCHEMA

// This function sanitizes output (i.e., inserts a \ before a " ).
SPmtString writeOutCorrect(SPmtString input)
{
    SPmtString output;
    unsigned int i = 0;
    for (i=0; i<input.length();i++)
    {
        if (input[i] == '\"')
            output += '\\';
        output += input[i];
    }
    return output;
}

// This function writes out the metadata information in an easy-to-ready form.
void writeOut(SPmtString factoryKey, PmtMetadataPtr md, PmtBaseOrder * base)
{
    cout << "\"" << md->key(false) << "\",";
    cout << "\"" << md->type() << "\",";
	unsigned int a = md->getMDType();
	if (md->multipleOccurrence()) a |= 0x80000000;
    cout << "(char *)" << a << ",\n";
    cout << "#ifdef PMT_USE_EMBEDDED_DOCUMENTATION\n";
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
    if (md->briefDocumentation() == NULL)
        cout << "NULL,";
    else
        cout << "\"" << writeOutCorrect(md->briefDocumentation()) << "\",\n";
    if (md->fullDocumentation() == NULL)
        cout << "NULL,\n";
    else
        cout << "\"" << writeOutCorrect(md->fullDocumentation()) << "\",\n";
#else
    cout << "NULL,NULL,\n";
#endif
    cout << "#endif\n";
}
          
#endif


//=============================================================================
// Class to transform all our BaseOrder stuff into a PmtMetadata factory
//=============================================================================
class PmtBaseOrderToFactory
{
	public:
        // Standard Constructor
        PmtBaseOrderToFactory(PmtSchemaSAX2Handler * theHandler)
        {
            mHandler = theHandler;
            mCurrentAttributeFactory = NULL;
        }

        // Method to add results from theHandler to the static factory
        bool addResultsToFactory(SPmtString rootElementName = "");
        
    private:
        PmtSchemaSAX2Handler * mHandler;
        SPmtString mRootElementName;
        PmtAttrFactory * mCurrentAttributeFactory;
        
        // Stolen (and modified) from PmtItemInterpreter ...
        PmtMetadataPtr createMetadata(PmtOrderElement * element,
                                      PmtComplexTypeDefinition * oParent,
                                      PmtCompositeMetadataPtr parent,
                                      SPmtString baseName="");

        // Stolen (and modified) from PmtItemInterpreter ...
        PmtAttributePtr createPrimitiveAttribute(PmtOrderAttribute * attribute,
                                           PmtComplexTypeDefinition * oParent);

        bool addAttributes(PmtMetadataPtr md, 
                           SPmtString factoryKey,
                           PmtComplexTypeDefinition * parent,
                           PmtBaseOrder::PmtBaseOrderList &attrs);
        bool addFacets(PmtMetadataPtr md, PmtOrderElement * element);

        bool addTreeToFactory(PmtBaseOrder * currentNode,
                              PmtComplexTypeDefinition * oParent,
                              PmtCompositeMetadataPtr parent,
                              SPmtString baseName,
                              SPmtString workingName,
                              SPmtString workingType="");

        bool processComplexElements( PmtMetadataPtr md,
                                     PmtComplexTypeDefinition * complex,
                                     SPmtString factoryKey);
};


// Stolen (and modified) from PmtItemInterpreter ...
PmtAttributePtr PmtBaseOrderToFactory::createPrimitiveAttribute(
                                            PmtOrderAttribute * attribute,
                                            PmtComplexTypeDefinition * oParent)
{
    SPmtString name = attribute->mName;
    SPmtString xsdType = attribute->mType;

    PmtAttributePtr attr = 0;
    
    //find the base type
    // Note that we're using the default namespace here ... need to change
    // Also note that were going straight to the base type without adding
    // any of the little things that may be along the way (Facets, etc.)
    SPmtString xsdPrefix = "xsd:";
    while ((xsdType.find(xsdPrefix) != 0) && (!xsdType.empty()))
    {
        PmtBaseOrder * theType = oParent->findTypeInList(xsdType);
        if (theType != NULL)
        {
            if (theType->mSwitchType == PmtBaseOrder::PMT_ORDER_SIMPLE_TYPE)
            {
                PmtSimpleTypeDefinition * s = 
                                   (PmtSimpleTypeDefinition *)theType;
                xsdType = s->mReferenceType;
                attribute->mVector = s->mVector;
                
                // Add in the facets....
                unsigned int i;
                for (i=0;i<s->mFacets.size();i++)
                {
                    attribute->addFacet(s->mFacets[i]);
                }
            }
//             else
//             {
//                 // Can Attributes be complex???? NO!
//             }
        }
        else
            xsdType = "";
    }

    SPmtString type = xsdType.substr(xsdType.find(":") + 1);
    SPmtString attrType = attribute->mType;
    if (attrType.empty())
        attrType = PMT_TYPE_ANONYMOUS;

    
    if (type == PMT_TYPE_INT8)

    {
        char def = atoi(attribute->mDefault.c_str());
        char fixed = atoi(attribute->mFixed.c_str());
        if (attribute->mVector)
            attr = new PmtAttributeT < vector < char > > (name, attrType,
                                              PMT_TRUE_TYPE_VINT8,
                                              vector < char >(1,def),
                                              vector < char >(1,fixed),
                                              attribute->mUse);
        else
            attr = new PmtAttributeT < char >(name, attrType, 
                                              PMT_TRUE_TYPE_INT8,
                                              def,
                                              fixed, 
                                              attribute->mUse);
    }
    else if (type == PMT_TYPE_UINT8)
    {
        unsigned char def = atoi(attribute->mDefault.c_str());
        unsigned char fixed = atoi(attribute->mFixed.c_str());
        if (attribute->mVector)
            attr =
                new PmtAttributeT < vector < unsigned char > > (name, attrType,
                                             PMT_TRUE_TYPE_VUINT8,
                                             vector < unsigned char >(1,def),
                                             vector < unsigned char >(1,fixed),
                                             attribute->mUse);
        else
            attr = new PmtAttributeT < unsigned char >(name, attrType,
                                                       PMT_TRUE_TYPE_UINT8,
                                                       def,
                                                       fixed, 
                                                       attribute->mUse);
    }
    else if (type == PMT_TYPE_INT16)
    {
        short def = atoi(attribute->mDefault.c_str());
        short fixed = atoi(attribute->mFixed.c_str());
        if (attribute->mVector)
            attr = new PmtAttributeT < vector < short > > (name, attrType,
                                               PMT_TRUE_TYPE_VINT16,
                                               vector < short >(1,def),
                                               vector < short >(1,fixed),
                                               attribute->mUse);
        else
            attr = new PmtAttributeT < short >(name, attrType, 
                                               PMT_TRUE_TYPE_INT16,
                                               def,
                                               fixed, 
                                               attribute->mUse);
    }
    else if (type == PMT_TYPE_UINT16)
    {
        unsigned short def = atoi(attribute->mDefault.c_str());
        unsigned short fixed = atoi(attribute->mFixed.c_str());
        if (attribute->mVector)
            attr =
                new PmtAttributeT < vector < unsigned short > > (name, attrType,
                                            PMT_TRUE_TYPE_VUINT16,
                                            vector < unsigned short >(1,def),
                                            vector < unsigned short >(1,fixed), 
                                            attribute->mUse);
        else
            attr = new PmtAttributeT < unsigned short >(name, attrType,
                                                        PMT_TRUE_TYPE_UINT16,
                                                        def,
                                                        fixed, 
                                                        attribute->mUse);
    }
    else if (type == PMT_TYPE_INT32)
    {
        long  def = atoi(attribute->mDefault.c_str());
        long  fixed = atoi(attribute->mFixed.c_str());
        if (attribute->mVector)
            attr = new PmtAttributeT < vector < long  > > (name, attrType,
                                             PMT_TRUE_TYPE_VINT32,
                                             vector < long  >(1,def),
                                             vector < long  >(1,fixed),
                                             attribute->mUse);
        else
            attr = new PmtAttributeT < long  >(name, attrType, 
                                             PMT_TRUE_TYPE_INT32,
                                             def,
                                             fixed, 
                                             attribute->mUse);
    }
    else if (type == PMT_TYPE_UINT32)
    {
        unsigned long  def = atoi(attribute->mDefault.c_str());
        unsigned long  fixed = atoi(attribute->mFixed.c_str());
        if (attribute->mVector)
            attr =
                new PmtAttributeT < vector < unsigned long  > > (name, attrType,
                                              PMT_TRUE_TYPE_VUINT32,
                                              vector < unsigned long >(1,def),
                                              vector < unsigned long >(1,fixed),
                                              attribute->mUse);
        else
            attr = new PmtAttributeT < unsigned long  >(name, attrType,
                                                      PMT_TRUE_TYPE_UINT32,
                                                      def,
                                                      fixed, 
                                                      attribute->mUse);
    }
    else if (type == PMT_TYPE_BOOLEAN)
    {
        bool def = (attribute->mDefault == "true");
        bool fixed = (attribute->mFixed == "true");
        if (attribute->mVector)
            attr = new PmtAttributeT < vector < bool > >(name, attrType,
                                               PMT_TRUE_TYPE_VBOOLEAN,
                                               vector < bool > (1, def),
                                               vector < bool > (1, fixed),
                                               attribute->mUse);
        else
            attr = new PmtAttributeT < bool > (name, attrType,
                                               PMT_TRUE_TYPE_BOOLEAN, 
                                               def,
                                               fixed,
                                               attribute->mUse);
    }
    else if (type == PMT_TYPE_FLOAT)
    {
        float def = (float)atof(attribute->mDefault.c_str());
        float fixed = (float)atof(attribute->mFixed.c_str());
        if (attribute->mVector)
            attr = new PmtAttributeT < vector < float > > (name, attrType,
                                               PMT_TRUE_TYPE_VFLOAT,
                                               vector < float >(1, def),
                                               vector < float >(1, fixed),
                                               attribute->mUse);
        else
            attr = new PmtAttributeT < float >(name, attrType, 
                                               PMT_TRUE_TYPE_FLOAT,
                                               def,
                                               fixed, 
                                               attribute->mUse);
    }
    else if (type == PMT_TYPE_DOUBLE)
    {
        double def = atof(attribute->mDefault.c_str());
        double fixed = atof(attribute->mFixed.c_str());
        if (attribute->mVector)
            attr = new PmtAttributeT < vector < double > > (name, attrType,
                                                   PMT_TRUE_TYPE_VDOUBLE,
                                                   vector < double >(1,def),
                                                   vector < double >(1,fixed),
                                                   attribute->mUse);
        else
            attr = new PmtAttributeT < double >(name, attrType, 
                                                PMT_TRUE_TYPE_DOUBLE,
                                                def,
                                                fixed, 
                                                attribute->mUse);
    }
    else if (type == PMT_TYPE_STRING)
    {
        if (attribute->mVector)
            attr = new PmtAttributeT < vector < string > >(name, attrType,
                                     PMT_TRUE_TYPE_VSTRING,
                                     vector < string > (1,attribute->mDefault),
                                     vector < string > (1,attribute->mFixed),
                                     attribute->mUse);
        else
            attr = new PmtAttributeT < string > (name, attrType,
                                                  PMT_TRUE_TYPE_STRING,
                                                  attribute->mDefault,
                                                  attribute->mFixed,
                                                  attribute->mUse);
    }

    return attr;
}


// Stolen (and modified) from PmtItemInterpreter ...
PmtMetadataPtr 
PmtBaseOrderToFactory::createMetadata(PmtOrderElement * element,
                                      PmtComplexTypeDefinition * oParent,
                                      PmtCompositeMetadataPtr parent,
                                      SPmtString baseName)
{
    PmtMetadataPtr md = 0;

    if (element != NULL)
    {
    
      SPmtString xsdType = element->mType;
      SPmtString mdtype = element->mType;
      SPmtString name = element->mName;
      SPmtString nillable = element->mNillable;
      bool isVector = false;
      bool isAlreadyInFactory = false;

      //find the base type
      // Note that we're using the default namespace here ... need to change
      SPmtString xsdPrefix = "xsd:";
      PmtBaseOrder * theType = NULL;
      if (element->mReferenceType.empty())
          theType = oParent->findTypeInList(xsdType);
      else
          theType = oParent->findTypeInList(element->mReferenceType);

      while ((xsdType.find(xsdPrefix) != 0) && (!xsdType.empty()))
      {
          if (theType != NULL)
          {
              if (theType->mSwitchType == PmtBaseOrder::PMT_ORDER_SIMPLE_TYPE)
              {
                  PmtSimpleTypeDefinition * s = 
                                     (PmtSimpleTypeDefinition *)theType;
                  xsdType = s->mReferenceType;
                  isVector = s->mVector;

                  // Add in the facets....
                  unsigned int i;
                  for (i=0;i<s->mFacets.size();i++)
                  {
                      element->addFacet(s->mFacets[i]);
                  }
              }
              else if (theType->mSwitchType == PmtBaseOrder::PMT_ORDER_ELEMENT)
              {
                  PmtOrderElement * s = (PmtOrderElement *)theType;
                  if (s->mReferenceType.empty())
                      xsdType = s->mType;
                  else
                      xsdType = s->mReferenceType;

                  // Add in the facets....
                  unsigned int i;
                  for (i=0;i<s->mFacets.size();i++)
                  {
                      element->addFacet(s->mFacets[i]);
                  }
                  // Add in the attributes....
                  for (i=0;i<s->mAttributes.size();i++)
                  {
                      element->addAttribute(s->mAttributes[i]);
                  }
              }
              else
              {
                  // It's a complex type -- it had better already be in the 
                  // factory ... irregardless, we're done here...
                  md = PmtMetadata::getFactory("")->get(element->mType);
                  
                  // OK, if it's not in the factory, then we try to find 
                  // it and put it there....
                  if (!md)
                  {
                      SPmtString factoryKey = baseName + "." + name;
                      md = PmtMetadata::getFactory("")->get(factoryKey);
                      
                      if (!md)
                      {
                        isAlreadyInFactory = addTreeToFactory(theType,oParent,
                                                              parent,
                                                              baseName,
                                                              name,mdtype);
                      }
                  }
                  // We have to stop the turning, churning, burning 
                  xsdType="";
              }
          }
          else
          {
               // Talk about ignoring namespaces, but this just obliviates them!
               if (xsdType.find(':') != SPmtString::npos)
                   xsdType = xsdType.substr(xsdType.find(':')+1);
               else
                   // Not found ... bail out?
                   xsdType="";
          }
          if (!xsdType.empty())
              theType = oParent->findTypeInList(xsdType);
      }

      if ((!md) && (!isAlreadyInFactory))
      {
        // This seems a little questionable.....
        // I know find will return -1 if it can't find anything,
        // so this will amount to substr(0) in most cases.
        // Isn't there a better way to handle this?
        SPmtString theType = xsdType.substr(xsdType.find(":") + 1);

        // Strip off the namespace prefix, like "xsd:".
        PmtKeyType::size_type pos;
        pos = mdtype.find(":");
        if (pos != PmtKeyType::npos)
            mdtype = mdtype.substr(pos + 1);


        if (mdtype.empty())
            mdtype = PMT_TYPE_ANONYMOUS;
        
            
        int enumMdType = PmtMetadata::getType(theType);
        if (isVector) enumMdType++;
        md = createPrimitiveMetadata(name,
                                     mdtype,
                                     nillable,
                                     enumMdType);
		if (element->mMaxOccurs > 1) 
			md->multipleOccurrence() = true;
        
      } // end if !md
    } // end if element != NULL
    
    return md;
}

bool PmtBaseOrderToFactory::addAttributes(PmtMetadataPtr md, 
                                          SPmtString factoryKey,
                                          PmtComplexTypeDefinition * parent,
                                         PmtBaseOrder::PmtBaseOrderList &attrs)
{
    bool returnValue = true;
    
    PmtOrderAttribute * attr = NULL;
    for (unsigned int i = 0; i<attrs.size(); i++)
    {
        attr = (PmtOrderAttribute *)(attrs[i]);
        SPmtString fullkey = factoryKey + ":" + attr->mName;
        PmtAttributePtr a = createPrimitiveAttribute(attr,parent);
        if (a)
        {
            unsigned int i;
            for (i=0; i<attr->mFacets.size();i++)
            {
                PmtOrderFacet * facet = (PmtOrderFacet *)attr->mFacets[i];
                a->addFacet(facet->mName,facet->mValue,facet->mFixed);
            }
            
            if (mCurrentAttributeFactory)
            {
                if (mCurrentAttributeFactory->addEntry(fullkey,a))
                    md->setAttribute(attr->mName, 0);
            }
        }
    }
    
    return returnValue;
}

bool PmtBaseOrderToFactory::addFacets(PmtMetadataPtr md, 
                                      PmtOrderElement * element)
{
    bool returnValue = true;
    unsigned int i;
    for (i=0; i<element->mFacets.size();i++)
    {
        PmtOrderFacet * facet = (PmtOrderFacet *)element->mFacets[i];
        md->addFacet(facet->mName,facet->mValue,facet->mFixed);
    }

    return returnValue;
}

bool PmtBaseOrderToFactory::processComplexElements( PmtMetadataPtr md,
                                        PmtComplexTypeDefinition * complexElem,
                                        SPmtString factoryKey)
{
    bool returnValue = true;
    PmtBaseOrder::PmtBaseOrderList::iterator iter = 
                                              complexElem->mElements.begin();
    PmtModelGroupPtr model;

    PmtCompositeMetadataPtr cmd = (PmtCompositeMetadata *)md.ptr();

    while (iter != complexElem->mElements.end())
    {
        PmtBaseOrder::PmtBaseOrderList * list = NULL;
#ifdef PMT_WRITE_OUT_SCHEMA
        SPmtString orderType = "";
#endif
        if ((*iter)->mSwitchType == PmtBaseOrder::PMT_ORDER_SEQUENCE)
        {
            model = new PmtSequenceModelGroup(cmd.ptr());
            list = &(((PmtOrderSequence *)(*iter))->mOrderArray);
#ifdef PMT_WRITE_OUT_SCHEMA
            orderType = "xsd:sequence";
#endif
        }
        else if ((*iter)->mSwitchType == PmtBaseOrder::PMT_ORDER_CHOICE)
        {
            model = new PmtChoiceModelGroup(cmd.ptr());
            list = &(((PmtOrderChoice *)(*iter))->mOrderArray);
#ifdef PMT_WRITE_OUT_SCHEMA
            orderType = "xsd:list";
#endif
        }

#ifdef PMT_WRITE_OUT_SCHEMA
        cout << "\""<<orderType<<"\""<<",NULL,(char*)1,\n";
        cout << "#ifdef PMT_USE_EMBEDDED_DOCUMENTATION\nNULL,NULL,\n#endif\n";
#endif

		if (cmd->modelGroup())
            cmd->modelGroup()->addChildModelGroup(model);
        else
            cmd->modelGroup() = model;

        PmtBaseOrder::PmtBaseOrderList::iterator model_iter = 
                                                         list->begin();

        while (model_iter != list->end())
        {
            returnValue = addTreeToFactory((*model_iter),complexElem,
                                           cmd,factoryKey,
                                           (*model_iter)->mName);
            model_iter++;
        }

#ifdef PMT_WRITE_OUT_SCHEMA
        cout << "\""<<orderType<<"\""<<",NULL,(char*)-1,\n";
        cout << "#ifdef PMT_USE_EMBEDDED_DOCUMENTATION\nNULL,NULL,\n#endif\n";
#endif
        iter++;
    }
    return returnValue;
}

bool PmtBaseOrderToFactory::addTreeToFactory(PmtBaseOrder * currentNode,
                                             PmtComplexTypeDefinition * oParent,
                                             PmtCompositeMetadataPtr parent,
                                             SPmtString baseName,
                                             SPmtString workingName,
                                             SPmtString workingType)
{
  bool returnValue = true;

  PmtMetadataPtr md;
  SPmtString factoryKey = baseName;
  if (!baseName.empty()) factoryKey += ".";

  // We're ignoring namespace (again and again and again...)
  // Should this be moved to be a class member?
  SPmtString ns = "";
  PmtMetadataFactory * mdFactory = PmtMetadata::getFactory(ns);
  
  switch (currentNode->mSwitchType)
  {
    case PmtBaseOrder::PMT_ORDER_COMPLEX_TYPE:
      {
        bool mdInFactoryAlready = true;
        PmtComplexTypeDefinition * complexType = 
                                      (PmtComplexTypeDefinition *) currentNode;
        factoryKey += workingName;
        SPmtString typeAttr = complexType->mType;

		bool multiple = false;
		if (oParent != NULL)
		{
			PmtComplexTypeDefinition * actualNode = 
				(PmtComplexTypeDefinition *)oParent->findElementInList(workingName);
			if ((actualNode != NULL) && (actualNode->mMaxOccurs > 1))
                multiple = true;
		}

        // If this is an anonymous complexType, then the type attribute of its 
        // associated element will be nothing. In that case, substitute the  
        // literal "anonymous".
        // Otherwise, use the type attribute of the associated element. 
        if (typeAttr.empty())
        {
          if (workingType.empty())
            typeAttr = PMT_TYPE_ANONYMOUS;
          else 
            typeAttr = workingType;
        }
          
        if (workingName != mRootElementName)
        {
            // First, check to make sure we don't already have this item
            md = mdFactory->get(factoryKey);

            if (!md)
            {
                md = new PmtCompositeMetadata(workingName, typeAttr,
                                              PMT_MD_TYPE_COMPOSITE);
				md->multipleOccurrence() = multiple;
                mdInFactoryAlready = false;
            }
        }
        else
        {

            // For some odd reason, everyone expects the root element
            // name to be "".  Don't ask me why......
            factoryKey = "";
            
            // Make sure we don't already have this item
            md = mdFactory->get(factoryKey);

            PmtAttrFactory * attrFactory = PmtAttribute::getFactory(ns);
            mCurrentAttributeFactory = attrFactory;
            if (!md)
            {
                md = new PmtRootMetadata(workingName, typeAttr,
                                         PMT_MD_TYPE_COMPOSITE, mdFactory,
                                         attrFactory, ns);
                mdInFactoryAlready = false;
            }
        }

        // put prototype into factory
        // Look at how we ignore namespaces! (bad PMT, bad PMT)
        if (!mdInFactoryAlready)
        {
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
          md->setBriefDocumentation(complexType->mBriefDocumentation.c_str());
          md->setFullDocumentation(complexType->mFullDocumentation.c_str());
#endif

          returnValue = mdFactory->addEntry(factoryKey, md);

#ifdef PMT_WRITE_OUT_SCHEMA
          writeOut(factoryKey,md,complexType);
#endif

          // Add to parent model group
          // -- WARNING! This only adds to the first model group!!!!!!!
          // -- if there are any others, they'll be empty!!
          if (parent)
          {
            PmtModelGroupPtr parent_model = parent->modelGroup();
            parent_model->addMetadataKey(workingName,factoryKey,
                                         complexType->mMinOccurs,
                                         complexType->mMaxOccurs,
                                         complexType->mNillable);
          }
            
        }
        
        if (returnValue)
        {
          SPmtString ref = complexType->mReferenceType;
          while (!ref.empty())
          {
              PmtComplexTypeDefinition * c = 
                      (PmtComplexTypeDefinition *)oParent->findTypeInList(
                                               complexType->mReferenceType);
              if (c != NULL)
              {
                  // Add in the facets....
                  unsigned int i;
                  for (i=0;i<c->mFacets.size();i++)
                  {
                      complexType->addFacet(c->mFacets[i]);
                  }
                  // Add in the attributes....
                  for (i=0;i<c->mAttributes.size();i++)
                  {
                      complexType->addAttribute(c->mAttributes[i]);
                  }
                  // Add in the elements....
                  int j;
                  for (j=c->mElements.size()-1;j>=0;j--)
                  {
                      complexType->addOrderingTypeToFront(c->mElements[j]);
                  }
                  ref = c->mReferenceType;
              }
              else 
                  ref = "";
          }
          
          addAttributes(md,factoryKey,complexType,complexType->mAttributes);
          addFacets(md,complexType);
          returnValue = processComplexElements(md,complexType,factoryKey);
#ifdef PMT_WRITE_OUT_SCHEMA
          cout << "\"" << md->key() << "\",NULL,(char*)-1,\n";
          cout << "#ifdef PMT_USE_EMBEDDED_DOCUMENTATION\nNULL,NULL,\n#endif\n";
#endif
        }
      }
      break;

    case PmtBaseOrder::PMT_ORDER_ELEMENT:
      {
          PmtOrderElement * element = (PmtOrderElement *) currentNode;
          factoryKey += workingName;

          // 1) Create new metadata instance
          md = createMetadata(element,oParent,parent,baseName);

          // 2) add it to the factory
          //    Again, we're ignoring namespaces here....
          if (md)
          {
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
            md->setBriefDocumentation(element->mBriefDocumentation.c_str());
            md->setFullDocumentation(element->mFullDocumentation.c_str());
#endif
            returnValue = 
                  PmtMetadata::getFactory("")->addEntry(factoryKey, md);

            if (returnValue)
            {
                addAttributes(md, factoryKey, oParent, element->mAttributes);
                addFacets(md,element);

               // 3) Add to parent model group
               // -- WARNING! This only adds to the first model group!!!!!
               // -- if there are any others, they'll be empty!!
                PmtModelGroupPtr model = parent->modelGroup();
                model->addMetadataKey(workingName,factoryKey,
                                      element->mMinOccurs,
                                      element->mMaxOccurs,
                                      element->mNillable);
            }
#ifdef PMT_WRITE_OUT_SCHEMA
            writeOut(factoryKey,md,element);
#endif
          }
      }
      break;

    // We should only have ComplexTypes and Elements ...
    // The rest are errors!
    // WRONG -- we could get embedded sequence & choice orderings!
    case PmtBaseOrder::PMT_ORDER_SIMPLE_TYPE:
    case PmtBaseOrder::PMT_ORDER_SEQUENCE:
    case PmtBaseOrder::PMT_ORDER_CHOICE:
    case PmtBaseOrder::PMT_ORDER_LIST:
    default:
      returnValue = false;
  }

  return returnValue;
}

bool PmtBaseOrderToFactory::addResultsToFactory(SPmtString rootElementName)
{
    bool returnValue = false;
#ifndef PMT_USE_NO_XML_PARSER
    mRootElementName = rootElementName;
    PmtComplexTypeDefinition * schema = mHandler->getSchema();

    if (schema != NULL)
    {
      // First look for the root element in the tree
      PmtBaseOrder * root = schema->findTypeInList(mRootElementName);

      if (root == NULL)
      {
          // second, look for it in the elementlist, and make necessary changes
          PmtOrderElement * element = (PmtOrderElement*)
                                   schema->findElementInList(mRootElementName);
          if (element != NULL)
          {
              root = schema->findTypeInList(element->mType);
              if (root != NULL)
              {
                  ((PmtComplexTypeDefinition *)root)->mName = element->mName;
                  ((PmtComplexTypeDefinition *)root)->mType = element->mType;
              }
          }
      }

      if (root)
          returnValue = addTreeToFactory(root,schema,NULL,"",mRootElementName);
    }
#endif
    return returnValue;
}

// PMT_USE_NO_XML_PARSER
#endif 
//=============================================================================


//=============================================================================
// Actual PmtSchemaSAXParser method definitions
//=============================================================================

//! Constructor for using the default schema
PmtSchemaSAXParser::PmtSchemaSAXParser()
:mInStream(NULL),mUse(PMT_SCHEMA_SAX_PARSER_USE_DEFAULT),
 mRootElementName(ROOT_ELEMENT_NAME)
{
};

//! Constructor for parsing a stream
PmtSchemaSAXParser::PmtSchemaSAXParser(istream &in)
:mInStream(&in),mUse(PMT_SCHEMA_SAX_PARSER_USE_STREAM),
 mRootElementName(ROOT_ELEMENT_NAME)
{
};

//! Constructor for parsing a file
PmtSchemaSAXParser::PmtSchemaSAXParser(SPmtString filename)
:mInStream(NULL),mUse(PMT_SCHEMA_SAX_PARSER_USE_FILE),mFileName(filename),
 mRootElementName(ROOT_ELEMENT_NAME)
{
};

// Method to parse a schema using the given root element name
bool PmtSchemaSAXParser::parse(SPmtString rootElementName)
{
    if (!rootElementName.empty())
        mRootElementName = rootElementName;
    
    return this->parse();
}

// method to parse the schema.  
// This simply calls the appropriate parsing method.
bool PmtSchemaSAXParser::parse()
{
    bool returnValue = false;

    switch(mUse)
    {
        case PMT_SCHEMA_SAX_PARSER_USE_DEFAULT:
            returnValue = parseDefault();
            break;
        case PMT_SCHEMA_SAX_PARSER_USE_STREAM:
            returnValue = parseStream(mInStream);
            break;
        case PMT_SCHEMA_SAX_PARSER_USE_FILE:
            returnValue = parseFile(mFileName);
            break;
        default:
            break;
    }
    return returnValue;
}

// method for parsing a schema in a stream
bool PmtSchemaSAXParser::parseStream(istream *inStream)
{
    bool returnValue = false;

#if !defined ( PMT_USE_NO_XML_PARSER ) 
    //// create the SAX2 handler for SAX2 callbacks
    PmtSchemaSAX2Handler handler(this);

    //// Create the parser and set the handler
    PmtSAX2XMLReader* parser = PmtCreateSAX2XMLReader();
    parser->setContentHandler(&handler);

    // Set the input stream source
    PmtStreamInputSource *strmSrc = PmtSAX2CreateStreamInputSource(inStream);

    // parse the stream
    parser->parse(*strmSrc);
    
    // delete the parser
    PMT_DELETE_SAX2_PARSER(parser);
    PMT_DELETE_STREAM_SOURCE(strmSrc);

    returnValue = handler.hadNoErrors();
    if (returnValue)
    {
        PmtBaseOrderToFactory b2f(&handler);
        returnValue = b2f.addResultsToFactory(mRootElementName);
    }
#endif

    return returnValue;
}

// Method for parsing a schema file
bool PmtSchemaSAXParser::parseFile(SPmtString theFileName, void * ihandler)
{
    bool returnValue = false;

#ifndef PMT_USE_NO_XML_PARSER
    SPmtString filePath;
    filePath = theFileName;

    // Verify the file and the path to it.
    if (PmtGetOSSpecificFilePathForXmlParser(filePath))
    {
        //// create the SAX2 handler for SAX2 callbacks
        PmtSchemaSAX2Handler * handler;
        if (ihandler == NULL)
            handler = new PmtSchemaSAX2Handler(this);
        else
            handler = (PmtSchemaSAX2Handler *)ihandler;

        //// Create the parser and set the handler
        PmtSAX2XMLReader* parser = PmtCreateSAX2XMLReader();
        parser->setContentHandler(handler);

        // parse the file
        PMT_SAX2_PARSEFILE(parser,filePath.c_str());

        // delete the parser
        PMT_DELETE_SAX2_PARSER(parser);
        
        returnValue = handler->hadNoErrors();
        if (returnValue && (ihandler == NULL))
        {
            while(mIncludes.size()>0)
            {
                SPmtString newFile = mIncludes.back();
                mIncludes.pop_back();
                parseFile(newFile,handler);
            }

            PmtBaseOrderToFactory b2f(handler);
            returnValue = b2f.addResultsToFactory(mRootElementName);
        }
        
        if (ihandler == NULL)
            delete handler;
    }
#endif

    return returnValue;
}

// Method to insert the default schema
// This could be implemented several differenct ways, and should probably
// be broken up into multiple routines instead of one long one.  But that's 
// how it goes....
bool PmtSchemaSAXParser::parseDefault()
{
    bool returnValue = true;

    // Stacks help keep track of where we are in the tree.
    vector < PmtCompositeMetadataPtr > compositeStack;
    vector < PmtModelGroupPtr > modelGroupStack;
    
    // other variables we need to keep track of through the while loop 
    PmtMetadataPtr md;
    PmtCompositeMetadataPtr parent;
    PmtModelGroupPtr model;
    SPmtString fullKey;
    SPmtString ns = ""; // Set this to the default namespace
    PmtMetadataFactory * mdFactory = PmtMetadata::getFactory(ns);
    const CharPmtTT * transTable = PmtTranslationTable::getDefaultTranslationTable();

    // We have to check to see if the root element already exists.
    // If it doesn't, we go ahead and create it.  This could
    // cause some unexpected results if another schema is loaded first
    // and has a different root element name than this one.
    md = mdFactory->get("");
    if (!md)
    {
        PmtAttrFactory * attrFactory = PmtAttribute::getFactory(ns);
        parent = new PmtRootMetadata(PmtDefaultPreparsedSchema[0], 
                                     PmtDefaultPreparsedSchema[1],
                                     PMT_MD_TYPE_COMPOSITE, mdFactory,
                                     attrFactory, ns);
        md = parent.ptr();
        returnValue = mdFactory->addEntry(ns,md);

#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
        // Set the documentation
        md->setBriefDocumentation(PmtDefaultPreparsedSchema[3]);
        md->setFullDocumentation(PmtDefaultPreparsedSchema[4]);
#endif

    }
    else
        parent = (PmtCompositeMetadata *)md.ptr();
    compositeStack.push_back(parent);
    
    unsigned int index = PMT_DEFAULT_SCHEMA_INDEX_SIZE;
    
    // Loop through until we get to the closing of the root element 
    // (and we don't have any errors.)  Note that unless our default
    // schema is created correctly, this could cause problems.
    while ((strcmp(PmtDefaultPreparsedSchema[index],PmtDefaultPreparsedSchema[0]))
           && returnValue)
    {
		// does this item allow multiple instances?
		bool multiple = false;

        // This factor tells us what we need to do....
        long switchFactor = (long)PmtDefaultPreparsedSchema[index+2];
		if ((switchFactor != -1) && (switchFactor & 0x80000000))
		{
			multiple = true;
			switchFactor &= 0x7FFFFFFF;
		}
        switch (switchFactor)
        {
            case -1:
              {
                // This means we've reached the end of a composite type
                // Since model groups also parade under the composite flag,
                // we have to check for them and act accordingly
                if ((!strcmp(PmtDefaultPreparsedSchema[index],"xsd:sequence"))
                    || (!strcmp(PmtDefaultPreparsedSchema[index],"xsd:choice")))
                {
                    // we've reached the end of this model group.
                    // pop it of the stack and get the next in line.
                    modelGroupStack.pop_back();
                    if (modelGroupStack.size() > 0)
                        model = modelGroupStack.back();
                    else
                        model = NULL;
                }
                else if (parent->key() == PmtDefaultPreparsedSchema[index])
                {
                    // We've reached the end of the complexType.
                    // pop it off the stack and set the new parent accordingly
                    // (note that we're assuming the stack will never be
                    //  empty here 'cause there's always the root element)
                    compositeStack.pop_back();
                    parent = compositeStack.back();
                    
                    // reset the fullkey to remove this branch
                    unsigned int dotPlace = fullKey.find_last_of('.');
                    if (dotPlace == SPmtString::npos)
                        fullKey = "";
                    else
                        fullKey = fullKey.substr(0,dotPlace);
                }
                else
                    returnValue = false;
              }
              break;
            case PMT_MD_TYPE_UNKNOWN:
              // We don't currently handle unknown types
              returnValue = false;
              break;
            case PMT_MD_TYPE_COMPOSITE:
              {
                // We use the composite flag for both composite elements and
                // model groups.  Since we only support sequence and choice
                // model groups in PMT, check for them and act accordingly.
                if (!strcmp(PmtDefaultPreparsedSchema[index],"xsd:sequence"))
                {
                  // create the new model group and add it to the parent's list
                  model = new PmtSequenceModelGroup(parent.ptr());
		          if (parent->modelGroup())
                      parent->modelGroup()->addChildModelGroup(model);
                  else
                      parent->modelGroup() = model;
                      
                  // we also have to keep track of this for the future...
                  modelGroupStack.push_back(model);
                }
                else if (!strcmp(PmtDefaultPreparsedSchema[index],"xsd:choice"))
                {
                  // create the new model group and add it to the parent's list
                  model = new PmtChoiceModelGroup(parent.ptr());
		          if (parent->modelGroup())
                      parent->modelGroup()->addChildModelGroup(model);
                  else
                      parent->modelGroup() = model;
                      
                  // we also have to keep track of this for the future...
                  modelGroupStack.push_back(model);
                }
                else
                {
                  // It's a normal composite element.
                  // So determine it's full key and create it
                  SPmtString factoryKey;
                  if (!fullKey.empty()) factoryKey = fullKey + ".";
                  factoryKey += PmtDefaultPreparsedSchema[index];
                  md = new PmtCompositeMetadata(PmtDefaultPreparsedSchema[index],
                                                PmtDefaultPreparsedSchema[index+1],
                                                PMT_MD_TYPE_COMPOSITE);
				  md->multipleOccurrence() = multiple;

#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
                  // Set the documentation
                  md->setBriefDocumentation(PmtDefaultPreparsedSchema[index+3]);
                  md->setFullDocumentation(PmtDefaultPreparsedSchema[index+4]);
#endif

                  // Now add it to the factory and the parent's model group
                  returnValue = mdFactory->addEntry(factoryKey,md);
                  if (returnValue)
                  {
                      fullKey = factoryKey;
					  unsigned int maxOccurs = 1;
					  if (multiple) maxOccurs = 0x7FFFFFFF;
                      model->addMetadataKey(PmtDefaultPreparsedSchema[index],
                                               fullKey,0,maxOccurs,"");
                                               
                      // And keep track of this for future use
                      parent = (PmtCompositeMetadata*)md.ptr();
                      compositeStack.push_back(parent);
                  }
                }
              }
              break;
            default:
              {
                // It must be a normal metadata object.
                // So create it and insert it into the factory.
                md = createPrimitiveMetadata(PmtDefaultPreparsedSchema[index],
                                             PmtDefaultPreparsedSchema[index+1],
                                             "",
                                             switchFactor);

				if (md)
                {
#ifdef PMT_USE_EMBEDDED_DOCUMENTATION
                  // Set the documentation
                  md->setBriefDocumentation(PmtDefaultPreparsedSchema[index+3]);
                  md->setFullDocumentation(PmtDefaultPreparsedSchema[index+4]);
#endif
				  md->multipleOccurrence() = multiple;

                  //fullKey should never be empty here...
                  SPmtString factoryKey = fullKey + ".";
                  factoryKey += PmtDefaultPreparsedSchema[index];
                  
				  // Determine the URI from the translationTable
				  const CharPmtTT * tmpTrans = transTable;
				  transTable++;
				  while ((transTable != tmpTrans) && (strcmp(transTable->key,factoryKey.c_str())))
				  {
					  transTable++;
					  if (transTable->uri == NULL) transTable = PmtTranslationTable::getDefaultTranslationTable();
				  }
				  if ((transTable != tmpTrans)||(!strcmp(transTable->key,factoryKey.c_str())))
					  md->setURI(transTable->uri);

                  // Insert in the factory and the parent's model group
                  returnValue = mdFactory->addEntry(factoryKey,md);
                  if (returnValue)
                  {
					  unsigned int maxOccurs = 1;
					  if (multiple) maxOccurs = 0x7FFFFFFF;
 					  model->addMetadataKey(PmtDefaultPreparsedSchema[index],
                                            factoryKey,0,maxOccurs,"");
                  }
                }
              }
        }
        index+=PMT_DEFAULT_SCHEMA_INDEX_SIZE;
    }

    return returnValue;
}

