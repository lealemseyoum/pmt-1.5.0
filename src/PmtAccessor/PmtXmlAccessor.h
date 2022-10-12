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
 * Creation Date: December, 2000
 *
 * Original Author: 
 * Chris Lin <ti.lin@kodak.com> 
 * Copyright (C) 2000 Eastman Kodak Company
 * 
 * Contributor(s): 
 */

#ifndef PMT_XML_ACCESSOR__H
#define PMT_XML_ACCESSOR__H

#include "PmtCommon.h"
#include "PmtAccrImplDefs.h"
#include "PmtAccessor.h"
#include "PmtInstanceInterpreter.h"

class PmtXmlAccessor;

typedef EkSmartPtr < PmtXmlAccessor > PmtXmlAccessorPtr;
typedef EkConstSmartPtr < PmtXmlAccessor > ConstPmtXmlAccessorPtr;


/*! Provides access to metadata stored as XML in text files. 

  One should never have to include this header file, nor use this 
  interface directly. All access to this class' functionality should
  be through the PmtAccessor base class interface. 
*/
class PMTACCR_IMPL_DECL PmtXmlAccessor:public PmtAccessor
{
  public:
    //! Default constructor
    inline PmtXmlAccessor():PmtAccessor(PMT_FORMAT_XML), mClosing(false)
    {
    }
    //! destructor
    virtual ~ PmtXmlAccessor();

    //! Opens a XML text file for reading / writing (depending on the mode)
    virtual bool open(const char *name, PmtOpenMode mode = PMT_READONLY);

    //! Read all existing metadata and return a root metadata.
    virtual PmtMetadataPtr readMetadata(const SPmtString nsURI = "");

    virtual PmtMetadataPtr readMetadata(PmtMetadataPtr & md);

    virtual void writeMetadata(const PmtMetadataPtr & md, bool bWriteValid =
                               false);

    //! Remove metadata from a file
    virtual void removeMetadata(const PmtMetadataPtr & md);

    //! Close the Xml file.
    virtual void close();

    //! Return a copy of the PmtXmlAccessor.
    inline virtual PmtAccessorPtr clone() const
    {
        return new PmtXmlAccessor();
    }

    bool setNSPrefix(const SPmtString & nsPrefix);

    //! Check if the given file is an XML file.
    static bool recognize(const SPmtString & name);

  protected:

    virtual bool getMdValue(PmtMetadataPtr & md)
    {
        return false;
    }

    virtual bool setMdValue(const PmtMetadataPtr & md)
    {
        return false;
    }

  protected:

    PmtLogicalInstanceInterpreter interpreter;

  private:
    bool mClosing;
};

//PMTACCR_IMPL_REQUIRE(PmtXmlAccessor)
#endif

