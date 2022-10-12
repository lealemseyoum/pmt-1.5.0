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

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#ifndef PMT_SCHEMA_SAX_PARSER_H
#define PMT_SCHEMA_SAX_PARSER_H

#include "EkCompiler.h"

//! The general translation table class interface.
class PmtSchemaSAXParser
{
    public:
        
        //! Constructors
        PmtSchemaSAXParser();

        //! Constructors
        PmtSchemaSAXParser(istream &in);

        //! Constructors
        PmtSchemaSAXParser(SPmtString filename);
        
        //! Deconstructors
        ~PmtSchemaSAXParser()
        {
        }

        //! Method to kick-off the parsing of the data
        bool parse();

        //! Method to kick-off the parsing of the data
        bool parse(SPmtString rootElementName);
        
        inline SPmtString nameSpace() { return "";};

        vector < SPmtString > mIncludes; // files to include

    private:
    
        istream *mInStream;       // The input stream
        short mUse;               // Whether to use file input or stream input
        SPmtString mFileName;     // The name of the file to parse
        SPmtString mRootElementName; // The root element name to look for
        
        //! Method to actually parse the stream data
        bool parseStream(istream *inStream);

        //! Method to actually parse the file data
        bool parseFile(SPmtString theFileName, void * ihandler = NULL);
        
        bool parseDefault();
};



#endif
