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
 * Creation Date: 08/15/2000
 *
 * Original Author: 
 * Dan Rupe  rupe@kodak.com
 * 
 * Contributor(s): 
 * Sam Fryer <samuel.fryer@kodak.com>
 * 
*/

#ifndef PMT_SCHEMA_INFO_PARSER_H
#define PMT_SCHEMA_INFO_PARSER_H
    
// External Definitions....
class PmtSchemaInfoParserData;

// MULTI-THREAD WARNING:
// This class is not thread safe.  It should have a lock applied above it.
// (The PmtLogicalDefinitionInterpreter does this in most circumstances.)
// This class should ONLY be run by the PmtLogicalDefinitionInterpreter class.

//! Parser for the Metadata Definition Information file.
class PmtSchemaInfoParser
{
  public:
    //! This is the default constructor
    PmtSchemaInfoParser(const char *schemaInfoURI);

    //! This is the virtual destructor
    ~PmtSchemaInfoParser();

    //! This returns the first schema's info args
    bool getFirstSchemaInfoArgs(SPmtString &schemaURI, 
                                SPmtString &rootElementName,
                                SPmtString &translationTableURI);
    //! This returns the next schema's info args
    bool getNextSchemaInfoArgs(SPmtString &schemaURI,
                               SPmtString &rootElementName,
                               SPmtString &translationTableURI);

  private:
  
    PmtSchemaInfoParserData * mListTop;    // A linked-list of the schema info
    PmtSchemaInfoParserData * mCurrentPos; // Where we are in that list
    
};

#endif                          // PMT_SCHEMA_INFO_PARSER_H

