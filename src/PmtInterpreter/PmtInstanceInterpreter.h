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
 * Copyright (C) 1999-2000 Eastman Kodak Company. All 
 * Rights Reserved. 
 * 
 * Original Author: 
 * Chris Lin
 * 
 * Contributor(s): 
 * Rupe, Dan
 * Sam Fryer <samuel.fryer@kodak.com>
 * 
 * Date: Nov 8, 2000
*/

#ifndef PMT_INSTANCE_INTERPRETER__H
#define PMT_INSTANCE_INTERPRETER__H

#if (defined _MSC_VER) && (!defined _WINDOWS_) && (!defined __AFXWIN_H__)
#include "windows.h"
#endif

#if (defined _MSC_VER)
#pragma warning (disable:4786)
#endif

#include <fstream>
#include "EkCompiler.h"
#include "PmtModelGroup.h"
#include "PmtMetadata.h"

//! Converts between XML Metadata instances and PmtMetadata instances
class PMTCORE_DECL PmtLogicalInstanceInterpreter
{
    friend PMTCORE_DECL istream & operator >>(istream & in,
                                              PmtLogicalInstanceInterpreter &
                                              interp);
    friend PMTCORE_DECL ostream & operator <<(ostream & out,
                                              PmtLogicalInstanceInterpreter &
                                              interp);

  public:
    //! This is the constructor
    PmtLogicalInstanceInterpreter();

    //! This is another constructor
    PmtLogicalInstanceInterpreter(PmtMetadataPtr md) ;

    //! This is the virtual destructor
    virtual ~ PmtLogicalInstanceInterpreter();

    //! This method opens the given filename and creates the associated 
    //  root element for the dom tree
    bool open(const SPmtString & fileName, PmtOpenMode mode = PMT_READWRITE);

    //! This method is unimplemented at the present time
    bool validate();

    //! This method flushes the XML Metadata to the currently open file
    void flush();

    //! This method flushes the XML Metadata to the currently open file, 
	// then closes the file
    void close();

    //! This method interprets XML Metadata instances into PmtMetadata instances
    PmtMetadataPtr readMetadata(PmtMetadataPtr &md);

    //! This method reads all XML metadata instances into PmtMetadata instances
    PmtMetadataPtr readMetadata();

    //! This method writes PmtMetadata instances into XML Metadata instances.
    void writeMetadata(const PmtMetadataPtr & md);

    //! Remove metadata from root
    void removeMetadata(const PmtMetadataPtr & md);

    SPmtString getNamespaceURI() const
    {
        return getNamespaceURI(mTnsPrefix);
    }

    bool setNSPrefix(const SPmtString & nsPrefix);

  protected:
    //! This method opens the input stream for reading;
    bool open(istream & in);

    //! This method writes the information out to the ostream.
    void writeXML(ostream & out);

    //! This method writes the information out to the file <fileName>.
    void writeXML(SPmtString fileName);

  private:
    //! This is the private copy constructor
    PmtLogicalInstanceInterpreter(const PmtLogicalInstanceInterpreter & src);

    //! This is the private assignment operator
    PmtLogicalInstanceInterpreter & operator=
                                        (PmtLogicalInstanceInterpreter & src);

    //! This method parses the incoming xml file and sets up the root node
    bool parseFile(const SPmtString & filename);

    //! This method creates a new dom node
    void createvoid(PmtMetadataPtr data, void * newData);

    //! Method to actually output the XML
    void writeXML(ostream & out, PmtMetadataPtr md, SPmtString last, 
                  SPmtString tab, SPmtString deltaTab);

    // read the contents of the specified node into the 
	// specified metadata instance
    void readMetadata(void * tnode, PmtMetadataPtr md);

    SPmtString getNamespaceURI(const SPmtString & prefix) const;
    bool checkNamespace(PmtMetadataPtr md);

    void addNonSchemaAttributes(void *node, PmtMetadataPtr md,
                                void *attr);

    void addRootAttributeTags(void *attr, PmtAttributeMap attributeMap,
                              PmtMetadataPtr md);

    // applies the W3C rules for nillable. Also adds the nillable to the 
	// attribute factory and attribute map.
    void verifyNillableAttr(SPmtString nillable, void *attr,
                            PmtAttributeMap attributeMap, PmtMetadataPtr md);

    void nsInit();

    PmtMetadataPtr mRootMd;
    fstream mFstream;
    SPmtString mFileName;
    PmtOpenMode mOpenMode;
    bool mInSync;
    map < SPmtString, SPmtString > mURIMap;
    SPmtString mTnsPrefix;
    PmtMetadataFactory *mMdFactory;
};

////////////////////////
// iostream operation for instance interpreter

inline istream & operator >>(istream & in,
                             PmtLogicalInstanceInterpreter & interp)
{
    interp.open(in);
    return in;
}

inline ostream & operator <<(ostream & out,
                             PmtLogicalInstanceInterpreter & interp)
{
    interp.writeXML(out);
    return out;
}


#endif

