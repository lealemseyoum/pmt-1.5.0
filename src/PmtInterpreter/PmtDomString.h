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
 * Creation Date: 07/21/2000
 *
 * Original Author: 
 * Dan Rupe  rupe@kodak.com
 * 
 * Contributor(s): 
 * Sam Fryer  samuel.fryer@kodak.com
 * 
*/

#ifndef PMT_DOMSTRING_H
#define PMT_DOMSTRING_H

#include "PmtCoreDefs.h"

#ifdef USE_GENERIC_DOM
// HERE ????
#elif defined (USE_XERCES_1)    // Using Xerces 1.2
#include "dom/XMLString.hpp"
#define PMT_WS_TO_DS( x ) x
#elif defined (USE_XERCES_2)    // Using Xerces 2.*
#include "xercesc/util/XMLString.hpp"
#define PMT_WS_TO_DS( x ) (XMLCh*) x
#else
// WHAT HERE????
#endif


/**
 * The PmtDomString class is the interface between the Xerces' XMLCh* class
 * and PMT.  It provides Unicode translation services.
 *
 * For example, if you need to store a XMLCh* into a std::string, use
 * PmtDomString as an intermediate step:
 *
 * @code
 *      XMLCh* ds = L"String with unicode text";
 *      PmtDomString pds(ds);
 *      SPmtString s = pds.getString();
 * @endcode
 *
 * Using PmtDomString ensures that `s' contains UTF-8 encoded unicode text.
 * This allows us to use Unicode strings throughout PMT, allowing us to ignore
 * code-pages and other internationalization issues.
 *
 * PmtDomString must also be used to go from ASCII/UTF-8 text to XMLCh*s:
 * 
 * @code
 *      SPmtString s = "UTF-8 encoded text...";
 *      PmtDomString pds(s);
 *      XMLCh* ds = pds.getXMLCh();
 * @endcode
 *
 * This ensures that UTF-8 encoded text is properly translated into Unicode
 * UTF-16 when creating the XMLCh*.
 */
 
class PMTCORE_DECL PmtDomString
{
  public:

    //! This is a constructor which takes a const XMLCh * that someone else owns.
    inline PmtDomString(const XMLCh* domString)
               :mDomString((XMLCh*)domString),mAsciiString(NULL),
                mOwnDomString(false),mOwnAsciiString(false)
    {
    }

    //! This is a constructor which takes a const char * that someone else owns.
    inline PmtDomString(const char *utf8Str)
               :mDomString(NULL),mAsciiString((char*)utf8Str),
                mOwnDomString(false),mOwnAsciiString(false)
    {
    }
    
    
    //! This is a constructor which takes a const SPmtString that someone else owns.
    inline PmtDomString(const SPmtString utf8Str)
               :mDomString(NULL),mAsciiString((char*)utf8Str.c_str()),
                mOwnDomString(false),mOwnAsciiString(false)
    {
    }


    //! This is the destructor
    ~PmtDomString()
    {
       if ((mOwnAsciiString)&&(mAsciiString != NULL))
		   XERCES_CPP_NAMESPACE_QUALIFIER 
		   XMLString::release(&mAsciiString);
       if ((mOwnDomString)&&(mDomString != NULL))
           //delete[] mDomString;
		   XERCES_CPP_NAMESPACE_QUALIFIER 
		   XMLString::release(&mDomString);
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
            mAsciiString = XERCES_CPP_NAMESPACE_QUALIFIER 
                           XMLString::transcode(mDomString);
            mOwnAsciiString = true;
        }
        return mAsciiString;
    }

    //! This method returns the XMLCh * representation
    inline XMLCh* getXMLCh()
    {
        if (mDomString == NULL)
        {
            mDomString = XERCES_CPP_NAMESPACE_QUALIFIER 
                           XMLString::transcode(mAsciiString);
            mOwnDomString = true;
        }
        return mDomString;
    }

  private:
    XMLCh *mDomString;
    char *mAsciiString;
    bool mOwnDomString;
    bool mOwnAsciiString;

    //! This is a private constructor
    inline PmtDomString()
               :mDomString(NULL),mAsciiString(NULL),
                mOwnDomString(false),mOwnAsciiString(false)
    {
    }

};


#endif                          // PMT_DOMSTRING_H

