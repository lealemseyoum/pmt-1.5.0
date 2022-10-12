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
 * Sam Fryer <samuel.fryer@kodak.com>
 * 
*/

#ifndef PMT_DEFINITION_INTERPRETER_H
#define PMT_DEFINITION_INTERPRETER_H

#include <set>

#ifdef _MSC_VER
#include <windows.h>
#endif
#include "EkGuard.h"
#include "EkLocking.h"
#include "PmtSchemaSAXParser.h"
#include "PmtCommon.h"
#include "PmtCoreDefs.h"
#include "PmtCoreCommon.h"

typedef vector < pair < pair < SPmtString, SPmtString >, bool > >PmtTtbType;

class PmtAliasesParser;


//! Interprets the output of the XML Parser into PmtMetadata prototypes for the factory
class PMTCORE_DECL PmtLogicalDefinitionInterpreter
{
  public:

    //! This is the default constructor
    inline PmtLogicalDefinitionInterpreter() 
    { 
    }
    
    //! This is the destructor
    inline ~ PmtLogicalDefinitionInterpreter()
    {
    }

    //! Load the default schema, the one we provide.
    static int load();

    //! Load the specified schema
    static int load(const char *schemaInfoURI);

    //! Load the default schema with aliases
    static int loadWithAliases(const char *aliasesURI);

    //! Load the specified schema with aliases
    static int loadWithAliases(const char *schemaInfoURI,
                               const char *aliasesURI);

    //! Load in-memory schema, root element name, translation table, and aliases information.
    static int loadMemory(const char *schema,
                      const char *translationTable = DEFAULT_TRANSLATION_TABLE,
                      const char *aliases = NO_ALIASES, 
                      const char *rootElementName = ROOT_ELEMENT_NAME);

    //! Takes an in-memory variable from a .h file and returns a SPmtString object.
    static void convertMemoryVariableToString(const char *const *memoryVariable,
                                              SPmtString & resultantString);

    //! Unload all prototypes from the metadata factories.
    static void erase();

    //! Unload prototypes from the metadata factory origniating from nameSpace.
    static void erase(const SPmtString & nameSpace);

    // Make sure we haven't loaded these schemas before!
    static bool schemaLoaded(const char *schemaInfoURI);
    static bool schemaInfoURILoaded(const char *schemaInfoURI);

    inline static bool getErased()
    {
        return msErased;
    }

    inline static void setErased(bool erased)
    {
        msErased = erased;
    }

	inline static bool ttbFilesChanged()
    {
        return msTtbFilesChanged;
    }
    inline static void ttbFilesChanged(bool changed)
    {
        msTtbFilesChanged = changed;
    }

    inline static EK_DEFAULT_MUTEX & loadLock()
    {
        return msLoadLock;
    }

    inline static PmtTtbType & ttbFiles()
    {
        return msTtbFiles;
    }

  private:

    static const char *const msPmtDefaultDefinitionsArray[];

    static PmtTtbType msTtbFiles;

    static bool msErased;

    static bool msTtbFilesChanged;

    static bool msDefaultSchemaLoaded;

    static EK_DEFAULT_MUTEX msLoadLock;

    //! Save the names of the loaded schemas
    static set < SPmtString > mssLoadedSchemas;

    //! Load the specified schema
    static int privateLoad(const char *schemaInfoURI);

    //! Common erase code for erasing with or without namespaces.
    static void privateEraseCommon();

    //! Load an additional schema. filePath is relative to current working directory.
    static int loadSchema(const char *schemaURI, const char *rootElementName,
                          const char *translationTableURI);

    //! Loads translation table URI for use by Accessors.
    static void loadTranslationTableInformation(const char *translationTableURI,
                                                SPmtString nameSpace);

    //! Loads the specified aliases
    static int loadAliases(const char *aliasesURI);

    //! Loads the specified aliases given in-memory.
    static int loadAliasesMemory(const char *aliases);

    //! Returns the schema path
    static SPmtString getSchemaPath(const PmtKeyType & relativeFilePath);

    //! Returns full schema path
    static SPmtString getFullSchemaPath(const SPmtString & relativePath);

    //! Returns the current working directory, in a form suitable for a schema path
    static SPmtString getCwdForSchemaPath();

// We don't currently handle namespaces...
//     static void loadSchemaNS(const char *schemaFile);

    //! Replace particular chars in string to another char.
    static void replaceCharInString(SPmtString & stringToChange,
                                    char charToReplace, char charToReplaceTo);

};


#endif                          // PMT_DEFINITION_INTERPRETER_H

