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

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma warning (disable:4786)
#pragma warning (disable:4503)
#endif

#include "PmtDefinitionInterpreter.h"
#include "PmtMetadata.h"
#include "PmtSchemaInfoParser.h"
#include "PmtAliasesParser.h"
#ifdef _MSC_VER
#include <direct.h>
#else
#include <unistd.h>
#endif
#include "PmtMisc.h"
#include "PmtSchemaSAXParser.h"

#include <sstream>

const char *const XERCES_SCHEMA_ID = "PmtSchemaMemoryBufferID";


PmtTtbType PmtLogicalDefinitionInterpreter::msTtbFiles;
bool PmtLogicalDefinitionInterpreter::msErased = false;
bool PmtLogicalDefinitionInterpreter::msTtbFilesChanged = true;
bool PmtLogicalDefinitionInterpreter::msDefaultSchemaLoaded = false;
EK_DEFAULT_MUTEX PmtLogicalDefinitionInterpreter::msLoadLock;
set < SPmtString > PmtLogicalDefinitionInterpreter::mssLoadedSchemas;

// Load the default schema.
int PmtLogicalDefinitionInterpreter::load()
{
    EK_GUARD(EK_DEFAULT_MUTEX, msLoadLock);
    if (!msDefaultSchemaLoaded)
    {
        PmtSchemaSAXParser pmtParser;

        msDefaultSchemaLoaded = pmtParser.parse(ROOT_ELEMENT_NAME);

        loadTranslationTableInformation(DEFAULT_TRANSLATION_TABLE,
                                        pmtParser.nameSpace());
    }
    return msDefaultSchemaLoaded?0:-1;
}


// returns true if the schema file has been loaded
bool PmtLogicalDefinitionInterpreter::schemaLoaded(const char *schema)
{
    bool return_value = false;
    if (mssLoadedSchemas.count(schema))
        return_value = true;
    return return_value;
}

// returns true if the schema file has been loaded
bool PmtLogicalDefinitionInterpreter::schemaInfoURILoaded
    (const char *schemaInfoURI)
{
    bool return_value = false;
    if (mssLoadedSchemas.count(schemaInfoURI))
        return_value = true;
    return return_value;
}


int PmtLogicalDefinitionInterpreter::load(const char *schemaInfoURI)
{
    EK_GUARD(EK_DEFAULT_MUTEX, msLoadLock);
    return privateLoad(schemaInfoURI);
}


int PmtLogicalDefinitionInterpreter::privateLoad(const char *schemaInfoURI)
{
    int status = 0;
    SPmtString schemaURI;
    SPmtString rootElementName;
    SPmtString translationTableURI;

    PmtSchemaInfoParser infoParser(schemaInfoURI);

    bool xmlinfo =
        infoParser.getFirstSchemaInfoArgs(schemaURI, rootElementName,
                                          translationTableURI);
    if (xmlinfo)
    {
        if (!schemaLoaded(schemaURI.c_str()))
        {
            status = loadSchema(schemaURI.c_str(), rootElementName.c_str(), 
                                translationTableURI.c_str());

            // Process additional schema specified in the information file, 
            // if any.
            while ((infoParser.getNextSchemaInfoArgs(schemaURI, 
                                                    rootElementName,
                                                    translationTableURI)))
            {
                status += loadSchema(schemaURI.c_str(), 
                                    rootElementName.c_str(), 
                                    translationTableURI.c_str());
            }

            mssLoadedSchemas.insert(schemaURI);
        }
    }
    else
        status = -1;

// We don't currently handle namespaces
//     if (!xmlinfo)               // ns schema
//     {
//         if (!schemaLoaded(schemaInfoURI))
//         {
//             loadSchemaNS(schemaInfoURI);
//             mssLoadedSchemas.insert(schemaInfoURI);
//         }
//     }
    return status;
}


int PmtLogicalDefinitionInterpreter::loadWithAliases(const char *aliasesURI)
{
    // This overloaded version of loadWithAliases() loads the default schema 
    // before loading the given aliases file.
    
    int returnValue = 0;

    EK_GUARD(EK_DEFAULT_MUTEX, msLoadLock);

    if (!msDefaultSchemaLoaded)
    {
        PmtSchemaSAXParser pmtParser;

        msDefaultSchemaLoaded = pmtParser.parse(ROOT_ELEMENT_NAME);

        loadTranslationTableInformation(DEFAULT_TRANSLATION_TABLE,
                                        pmtParser.nameSpace());

        if (!msDefaultSchemaLoaded)
            returnValue = -1;
    }

    returnValue += loadAliases(aliasesURI);

    return returnValue;
}


int PmtLogicalDefinitionInterpreter::loadWithAliases(const char *schemaInfoURI,
                                                     const char *aliasesURI)
{
    // This overloaded version of loadWithAliases() loads the schema(s) in 
    // the schema information file before loading the given aliases file
    
    int returnValue = 0;

    EK_GUARD(EK_DEFAULT_MUTEX, msLoadLock);
    
    returnValue = privateLoad(schemaInfoURI);

    returnValue += loadAliases(aliasesURI);
        
    return returnValue;
}


int PmtLogicalDefinitionInterpreter::loadMemory(const char *schema,
                                                const char *translationTable,
                                                const char *aliases,
                                                const char *rootElementName)
{
    int returnValue = 0;

    EK_GUARD(EK_DEFAULT_MUTEX, msLoadLock);

    string schemaString = schema;
    istringstream schemaStream(schemaString);
    PmtSchemaSAXParser pmtParser(schemaStream);

    returnValue = pmtParser.parse(rootElementName);

    if (strlen(translationTable))
    {
        SPmtString translationTableString(translationTable);

        // We now have a Translation Table file path, so notify PmtAccessor 
        // about it.
        msTtbFiles.push_back(make_pair
                            (make_pair(translationTableString, 
                                       pmtParser.nameSpace()),true));
		ttbFilesChanged(true);
    }
    if (strcmp(aliases, NO_ALIASES))
        returnValue += loadAliasesMemory(aliases);

    return returnValue;
}


void PmtLogicalDefinitionInterpreter::
convertMemoryVariableToString(const char *const *memoryVariable,
                              SPmtString & resultantString)
{
    resultantString = "";
    const char *const *arrayPtr = memoryVariable;

    while (*arrayPtr != NULL)
        resultantString += *arrayPtr++;
}


void PmtLogicalDefinitionInterpreter::erase()
{
    // Unload all prototypes from the PmtMetadata::factory.  These prototypes were
    // associated with the previously-loaded schema(s).
    // Also unload all the aliases.

    EK_GUARD(EK_DEFAULT_MUTEX, msLoadLock);

    PmtMetadata::getFactories().clear();
    PmtAttribute::getFactories().clear();

    privateEraseCommon();
}

void PmtLogicalDefinitionInterpreter::erase(const SPmtString & nameSpace)
{
    // Unload all prototypes from the PmtMetadata::factory.  These prototypes 
    // were associated with the previously-loaded schema(s).
    // Also unload all the aliases.

    EK_GUARD(EK_DEFAULT_MUTEX, msLoadLock);

    PmtMetadataFactories & mdFacs = PmtMetadata::getFactories();
    mdFacs.clear(nameSpace);
    PmtAttrFactories & attrFacs = PmtAttribute::getFactories();
    attrFacs.clear(nameSpace);

    privateEraseCommon();
}


void PmtLogicalDefinitionInterpreter::privateEraseCommon()
{
    PmtMetadata::getAliases().clear();

    msTtbFiles.clear();

    msErased = true;

    msDefaultSchemaLoaded = false;

    mssLoadedSchemas.clear();
}


// Load an additional schema. filePath is relative to current working directory.
int PmtLogicalDefinitionInterpreter::loadSchema(const char *schemaURI,
                                               const char *rootElementName,
                                               const char *translationTableURI)
{
    int returnValue = 0;
    PmtSchemaSAXParser pmtParser(schemaURI);

    returnValue = pmtParser.parse(rootElementName);

    loadTranslationTableInformation(translationTableURI,
                                    pmtParser.nameSpace());
    
    return returnValue;
}


void PmtLogicalDefinitionInterpreter::
loadTranslationTableInformation(const char *translationTableURI,
                                SPmtString nameSpace)
{
    if (strlen(translationTableURI))
    {
        // We now have a Translation Table file path, so notify PmtAccessor 
        // about it.
        msTtbFiles.push_back(make_pair
                            (make_pair(getSchemaPath(translationTableURI),
                                       nameSpace), false));
		ttbFilesChanged(true);
    }
}


int PmtLogicalDefinitionInterpreter::loadAliases(const char *aliasesURI)
{
    PmtAliasesParser aliasesParser;

    bool noError = aliasesParser.loadAliasFile(aliasesURI);
    
    return noError?0:-1;
}


int PmtLogicalDefinitionInterpreter::loadAliasesMemory(const char *aliases)
{
    PmtAliasesParser aliasesParser;

    bool noError = aliasesParser.loadAliasMemory(aliases);
    
    return noError?0:-1;
}


// If the path is the literal "default" then just return that.
// Otherwise, determine the location of the schema file and pre-pend that
// path to the path of the Translation Table.
SPmtString PmtLogicalDefinitionInterpreter::
getSchemaPath(const PmtKeyType & relativeFilePath)
{
    if (relativeFilePath == DEFAULT_TRANSLATION_TABLE)
        return DEFAULT_TRANSLATION_TABLE;
    else
        return getFullSchemaPath(relativeFilePath);
}


// We assume that paths in an Info file can be specified with forward or 
// backward slashes.
// The Windows Xerces parser can handle backslashes or forward slashes in paths.
// The Unix Xerces parser handles only forward slashes in paths.
// The Macintosh Xerces parser uses Unix-like paths.
SPmtString PmtLogicalDefinitionInterpreter::
getFullSchemaPath(const SPmtString & relativePath)
{
    SPmtString relPath = relativePath;

    // First do backslash to forward slash conversion.
    replaceCharInString(relPath, '\\', '/');

    // If the path passed in starts with a backslash or slash,
    // assume it's an absolute path already, and return it.
    if ((relPath[0] == '\\') || (relPath[0] == '/'))
        return relPath;


    SPmtString fullPath = "";
    SPmtString currentWorkingDir = getCwdForSchemaPath();

    fullPath += currentWorkingDir;

    // If no slash at end of path so far, place one there.
    char endChar = fullPath[fullPath.size() - 1];
    if ((endChar != '\\') && (endChar != '/'))
        fullPath += '/';

    fullPath += relativePath;

    return fullPath;
}


SPmtString PmtLogicalDefinitionInterpreter::getCwdForSchemaPath()
{
    SPmtString currentWorkingDir;
    char dirBuf[MAX_PATH_LEN];

    currentWorkingDir = getcwd(dirBuf, sizeof(dirBuf));

#if macintosh
    // Do conversion to Unix-like path for Macintosh version, since
    // the Xerces parser is written to use Unix paths on the Mac.

    // The Macintosh getcwd() returns the mount point name with no
    // beginning slash character.  Pre-pend a slash now.
    currentWorkingDir = "/" + currentWorkingDir;

    // Change colons to slashes.
    SPmtString::size_type pos = 0;
    while ((pos = currentWorkingDir.find(':', pos)) != SPmtString::npos)
    {
        currentWorkingDir.replace(pos, 1, "/");
        pos++;
    }
#endif

    return currentWorkingDir;
}


void PmtLogicalDefinitionInterpreter::
replaceCharInString(SPmtString & stringToChange, char charToReplace,
                    char charToReplaceTo)
{
    SPmtString::size_type pos = 0;
    SPmtString _charToReplaceTo(1, charToReplaceTo);

    while ((pos = stringToChange.find(charToReplace, pos)) != SPmtString::npos)
    {
        stringToChange.replace(pos, 1, _charToReplaceTo);
        pos++;
    }
}


// void PmtLogicalDefinitionInterpreter::loadSchemaNS(const char *schemaFile)
// {
//     msPmtParser->initialize(true);
//     msPmtParser->parse(schemaFile);
// 
//     PmtItemInterpreterPtr interp =
//         PmtItemInterpreter::getItemInterp(msPmtParser);
//     interp->interpret();
// 
//     msPmtParser->uninitialize();
// }

