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

// Note:  This started out as a simple ordering class for items such as
// "sequence" and "choice", but rapidly grew into holding everything, thus
// the naming is slightly inconsistent


// This should only be used internally to PMT...
#ifdef PMT_SCHEMA_SAX_PARSER_CPP

#ifndef PMT_ORDER_H
#define PMT_ORDER_H

#include "EkCompiler.h"

//! Base class for different schema items
class PmtBaseOrder
{
    public:
    
        // Define a list of these
        typedef vector < PmtBaseOrder * > PmtBaseOrderList;

        // The type used to map elements names to element pointers        
        typedef std::map < SPmtString, PmtBaseOrder * > PmtBaseOrderMapType;
    
        // The enum of possible types
        enum PmtBaseOrderType
        {
            PMT_ORDER_ELEMENT,
            PMT_ORDER_SEQUENCE,
            PMT_ORDER_CHOICE,
            PMT_ORDER_LIST,
            PMT_ORDER_COMPLEX_TYPE,
            PMT_ORDER_SIMPLE_TYPE,
            PMT_ORDER_ATTRIBUTE
        } ;
        
        // The type we were instantiated as...
        enum PmtBaseOrderType mSwitchType;
        
        // The element name (local),
        SPmtString mName;             

        // The brief documentation describing this node
        SPmtString mBriefDocumentation;

        // The detailed documentation describing this node
        SPmtString mFullDocumentation;

        // General constructor
        PmtBaseOrder(PmtBaseOrderType myType = PMT_ORDER_ELEMENT,
                     SPmtString myName = "")
        : mSwitchType(myType),mName(myName)
        {};
        
        // General Deconstructor
        virtual ~PmtBaseOrder() {};
        
        // Add something to our list...
        virtual void add(PmtBaseOrder * element) {};
};

//! A schema element
class PmtOrderElement : public PmtBaseOrder
{
    public:
    
        // A general constructor
        PmtOrderElement(SPmtString theName, SPmtString theType, 
                         unsigned int minOccurs=0, unsigned int maxOccurs=1) 
        : PmtBaseOrder(PMT_ORDER_ELEMENT,theName),mType(theType),
            mMinOccurs(minOccurs),mMaxOccurs(maxOccurs)
        {
        };
        
        // Another constructor only using the type...
        PmtOrderElement(PmtBaseOrder::PmtBaseOrderType theType) 
        : PmtBaseOrder(theType),
            mMinOccurs(0),mMaxOccurs(1)
        {
        };
        
        // General Deconstructor
        ~PmtOrderElement()
        {
            // facets are taken care of elsewhere...
            // attributes are cleaned up elsewhere...
        }
        
        // Add an attribute to the attribute list
        inline void addAttribute(PmtBaseOrder * attribute)
        {
            mAttributes.push_back(attribute);
        }
        
        // add a facet to the facet list 
        // (If it isn't already there...)
        inline void addFacet(PmtBaseOrder * facet)
        {
           bool found = false;
           unsigned int i;
           for (i=0; i<mFacets.size() && !found; i++)
               if (mFacets[i]->mName == facet->mName)
                   found = true;
           if (!found)
               mFacets.push_back(facet);
        }
        
        SPmtString mType;             // type
        SPmtString mNillable;         // nillable flag
        SPmtString mReferenceType;    // What do we reference for this?
        unsigned int mMinOccurs;      // min number allowed
        unsigned int mMaxOccurs;      // max number allowed
        PmtBaseOrderList mAttributes; // The attributes
        PmtBaseOrderList mFacets;     // The facets
};

// A Sequence item from the schema
class PmtOrderSequence : public PmtBaseOrder
{
    public:
    
        // A general constructor (the only kind...)
        PmtOrderSequence() : PmtBaseOrder(PMT_ORDER_SEQUENCE)
        {
        };
     
        // General Deconstructor
        ~PmtOrderSequence()
        {
            // mOrderArray elements are cleaned up elsewhere.
        }
        
        // A list of items in this sequence
        PmtBaseOrderList mOrderArray;
        
        // Add an item to the list
        void add(PmtBaseOrder *element) 
        {
            mOrderArray.push_back(element);
        };
};

// A Choice item from the schema
class PmtOrderChoice : public PmtBaseOrder
{
    public:
    
        // A general constructor 
        PmtOrderChoice() : PmtBaseOrder(PMT_ORDER_CHOICE)
        {
        };
        
        // General Deconstructor
        ~PmtOrderChoice()
        {
            // mOrderArray elements are cleaned up elsewhere.
        }
        
        // A list of items in this choice set
        PmtBaseOrderList mOrderArray;

        // Add an item to the list
        void add(PmtBaseOrder *element) 
        {
            mOrderArray.push_back(element);
        };
};

//! A definition of the complex type element of the schema.
class PmtComplexTypeDefinition : public PmtOrderElement
{
    public:

        // A general constructor 
        PmtComplexTypeDefinition()
        :PmtOrderElement(PMT_ORDER_COMPLEX_TYPE),
         mParent(NULL)
        {
        };
        
        // General Deconstructor
        ~PmtComplexTypeDefinition()
        {
            // Destroy all attribute pointers
            PmtBaseOrderMapType::iterator iter =  mAttributeList.begin();
            while (iter != mAttributeList.end())
            {
                delete (*iter).second;
                iter++;
            }
            
            // Destroy all type pointers that aren't elements or complexType
            iter = mTypeMap.begin();
            while (iter != mTypeMap.end())
            {
                if (((*iter).second->mSwitchType != 
                                     PmtBaseOrder::PMT_ORDER_COMPLEX_TYPE) &&
                    ((*iter).second->mSwitchType != 
                                     PmtBaseOrder::PMT_ORDER_ELEMENT))
                    delete (*iter).second;
                iter++;
            }
            
            // Destroy all element pointers
            iter = mElementList.begin();
            while (iter != mElementList.end())
            {
                delete (*iter).second;
                iter++;
            }
            
        }
        
        // Add an element to the complex type
        inline void addElement(PmtBaseOrder * element)
        {
            // We have to have an ordering type
            // otherwise we just ignore it!
            if ( mElements.size() > 0 )
                mElements.back()->add(element);
        };
        
        // Add an ordering (sequence, choice)
        inline void addOrderingType(PmtBaseOrder * orderingType)
        {
            mElements.push_back(orderingType);
        };
        
        // Add an ordering (sequence, choice)
        inline void addOrderingTypeToFront(PmtBaseOrder * orderingType)
        {
            mElements.insert(mElements.begin(),orderingType);
        };

        PmtBaseOrder * findElementInList(SPmtString elementName)
        {
            PmtBaseOrder * theElement = NULL;
            
            PmtBaseOrderMapType::iterator iter = mElementList.find(elementName);
            if (iter != mElementList.end())
            {
                theElement = (*iter).second;
            }
            else
            {
                if (mParent != NULL)
                    theElement = mParent->findElementInList(elementName);
            }
            
            return theElement;
        }
        
        PmtBaseOrder * findTypeInList(SPmtString typeName)
        {
            PmtBaseOrder * theType = NULL;
            
            PmtBaseOrderMapType::iterator iter = mTypeMap.find(typeName);
            if (iter != mTypeMap.end())
            {
                theType = (*iter).second;
            }
            else
            {
                if (mParent != NULL)
                    theType = mParent->findTypeInList(typeName);
            }
            
            return theType;
        }
        
        PmtBaseOrder * findAttributeInList(SPmtString attribName)
        {
            PmtBaseOrder * theAttr = NULL;
            
            PmtBaseOrderMapType::iterator iter =mAttributeList.find(attribName);
            if (iter != mAttributeList.end())
            {
                theAttr = (*iter).second;
            }
            else
            {
                if (mParent != NULL)
                    theAttr = mParent->findAttributeInList(attribName);
            }
            
            return theAttr;
        }
        
        // A list of items in this complex set
        PmtBaseOrder::PmtBaseOrderList mElements;
        
         // A list of items in this complex set
        PmtBaseOrderMapType mElementList;

         // A list of items in this complex set
        PmtBaseOrderMapType mTypeMap;

        // list of all attributes
        PmtBaseOrderMapType mAttributeList; 
        
       // The complex parent of this instance
        PmtComplexTypeDefinition * mParent;
};

//! A definition of the simple type element of the schema.
class PmtSimpleTypeDefinition : public PmtBaseOrder
{
    public:
    
        // A general constructor 
        PmtSimpleTypeDefinition()
        :PmtBaseOrder(PMT_ORDER_SIMPLE_TYPE),mVector(false)
        {
        };
        
        // General Deconstructor
        ~PmtSimpleTypeDefinition()
        {
            // facets are taken care of elsewhere...
        }
        
        bool mVector;               // Is this a vector type?
        SPmtString mReferenceType;  // What do we reference for this?
        PmtBaseOrderList mFacets;   // The facets
};

//! A definition of the attribute element of the schema.
class PmtOrderAttribute : public PmtBaseOrder
{
    public:
    
        // A general constructor 
        PmtOrderAttribute()
        :PmtBaseOrder(PMT_ORDER_ATTRIBUTE),mVector(false)
        {
        };
        
        // General Deconstructor
        ~PmtOrderAttribute()
        {
            // facets are taken care of elsewhere...
        }
        
        // add a facet to the facet list 
        // (If it isn't already there...)
        inline void addFacet(PmtBaseOrder * facet)
        {
           bool found = false;
           unsigned int i;
           for (i=0; i<mFacets.size() && !found; i++)
               if (mFacets[i]->mName == facet->mName)
                   found = true;
           if (!found)
               mFacets.push_back(facet);
        }
        
        SPmtString mType;           // What do we reference for this?
        SPmtString mDefault;        // The default value this should have
        SPmtString mFixed;          // ??? (Something that looked interesting!)
        SPmtString mUse;            // Is this required/optional/prohibited?
        bool mVector;               // Is this a vector type?
        PmtBaseOrderList mFacets;   // The facets
};

//! A definition of the facets of the schema.
class PmtOrderFacet : public PmtBaseOrder
{
    public:
    
        // A general constructor 
        PmtOrderFacet()
        :PmtBaseOrder(PMT_ORDER_ATTRIBUTE),mFixed(false)
        {
        };
        
        // General Deconstructor
        ~PmtOrderFacet()
        {
        }
        
        vector < SPmtString > mValue;  // list of values
        bool mFixed;                   // Is this fixed?
};

#endif
#endif

