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
 * Creation Date: 11/1/00
 *
 * Original Author: 
 * Chris Lin <ti.lin@kodak.com> 
 * 
 * Contributor(s): 
 */

#ifndef PMT_ERROR_H
#define PMT_ERROR_H

#include "EkCompiler.h"
#include "EkError.h"

#include "PmtCoreDefs.h"
#include "PmtCommon.h"

//:
// The following constant should be used within PMT as the component ID
// of EkError objects created by PMT.  It is used automatically by the 
// helper class PmtError.
extern const char *PMT;

//: Indicates which type of error to throw
typedef enum
{
    PMT_THROW_REGISTRY_ERROR = 0x0001,
    PMT_THROW_IO_ERROR = 0x0002,
    PMT_THROW_CONVERSION_ERROR = 0x0004,
    PMT_THROW_ALL_ERRORS = 0xffff
}
PmtErrorHandling;

//: The following are PMT Error Codes.
//: General Error Codes
const int PMT_ERROR = 1;

//: Error Codes for Registry Parsing starts at 1000
const int PMT_PARSER_CANT_OPEN_REGISTRY_FILE = 1001;
const int PMT_PARSER_SOME_ERROR = 1002;
const int PMT_PARSER_UNKNOWN_TAG_IGNORED = 1003;
const int PMT_PARSER_MISSING_REQUIRED_ATTRIBUTE = 1004;
const int PMT_PARSER_REGISTRY_FILE_EMPTY = 1005;
const int PMT_PARSER_MISSING_XML_DUPLICATE_REGISTRY_ELEMENT = 1006;
const int PMT_PARSER_MISSING_TYPE_ATTRIBUTE_IN_ARRAYSIZE = 1007;
const int PMT_PARSER_BAD_TYPE_ATTRIBUTE_IN_ARRAYSIZE = 1008;
const int PMT_PARSER_MISSING_SIZE_ATTRIBUTE_IN_ARRAYSIZE = 1009;
const int PMT_PARSER_NEGATIVE_SIZE_ATTRIBUTE_IN_ARRAYSIZE = 1010;
const int PMT_PARSER_NEW_FAILED = 1011;
const int PMT_PARSER_MISSING_TAGLOCATION_ATTRIBUTE_IN_EXIF = 1012;
const int PMT_PARSER_BAD_TAGLOCATION_ATTRIBUTE_IN_EXIF = 1013;
const int PMT_PARSER_MISSING_TAGNUMBER_ATTRIBUTE_IN_EXIF = 1014;
const int PMT_PARSER_MISSING_TAGTYPE_ATTRIBUTE_IN_EXIF = 1015;
const int PMT_PARSER_BAD_TAGTYPE_ATTRIBUTE_IN_EXIF = 1016;
const int PMT_PARSER_INTERNAL_ERROR = 1017;
const int PMT_PARSER_MISSING_KEY_ATTRIBUTE_IN_ENTRY = 1018;
const int PMT_PARSER_MISSING_FACTORYKEY_ATTRIBUTE_IN_ENTRY = 1019;
const int PMT_PARSER_BAD_FACTORYKEY_ATTRIBUTE_IN_ENTRY = 1020;
const int PMT_PARSER_PARSER_NOT_FOUND = 1021;

//: Error codes for translator
const int PMT_TRANSLATOR_NOT_FOUND = 1101;
const int PMT_TRANSLATION_INFO_NOT_FOUND = 1102;
const int PMT_TRANSLATION_MD_OBJECT_NOT_FOUND = 1103;

//: Error code for version class
const int PMT_BAD_VERSION = 1201;
const int PMT_BAD_UTILITY = 1202;
const int PMT_INTERNAL_ERROR = 1203;

//: Error codes for accessors
const int PMT_ACCESSOR_FILE_NOT_FOUND = 1301;
const int PMT_ACCESSOR_FILE_READ_ONLY = 1302;

//: Error Codes for PmtRegistry loading
const int PMT_REGISTRY_INTERNAL_ERROR = 2001;

//: Error Codes for PmtFormatEntry
const int PMT_FORMAT_ENTRY_NO_ENTRY_IN_REGISTRY = 3001;
const int PMT_FORMAT_ENTRY_VALUE_CONVERSION_ERROR = 3003;
const int PMT_EXIF_FORMAT_ENTRY_NO_TAG_IN_FILE = 3100;
const int PMT_EXIF_FORMAT_ENTRY_BAD_TAG_LOCATION = 3101;
const int PMT_EXIF_FORMAT_ENTRY_BAD_TAG_TYPE = 3102;
const int PMT_EXIF_FORMAT_ENTRY_NOT_FOUND = 3103;

const int PMT_TIFF_FORMAT_ENTRY_NO_TAG_IN_FILE = 3201;
const int PMT_TIFF_FORMAT_ENTRY_BAD_TAG_LOCATION = 3202;
const int PMT_TIFF_FORMAT_ENTRY_BAD_TAG_TYPE = 3203;
const int PMT_TIFF_FORMAT_ENTRY_NOT_FOUND = 3204;

//: Error Codes for PmtAbstractImage
const int PMT_ABSTRACT_IMAGE_SOURCE_CONFLICT = 3301;
const int PMT_ABSTRACT_IMAGE_FILE_OPEN = 3302;
const int PMT_ABSTRACT_IMAGE_BAD_STREAM_HANDLE = 3303;

//: Error Codes for PmtInterpreter
const int PMT_INTERP_GENERAL = 4001;
const int PMT_INTERP_NO_ROOT_ELEMENT = 4002;
const int PMT_INTERP_ROOT_ELEMENT_NO_COMPLEX = 4003;
const int PMT_INTERP_PARSER_INIT_ERROR = 4004;
const int PMT_INTERP_COULD_NOT_PROCESS_ELEMENT = 4005;
const int PMT_INTERP_NULL_INTERPRETER = 4006;
const int PMT_INTERP_NO_TRANSLATION_TABLE = 4007;
const int PMT_INTERP_CANNOT_OPEN_FILE = 4008;
const int PMT_INTERP_INVALID_ELEMENT = 4009;
const int PMT_INTERP_TYPEDEF_NOTFOUND = 4010;
const int PMT_INTERP_INVALID_ANONYMOUS_TYPE_DEF = 4011;
const int PMT_INTERP_ELEMENTREF_NOTFOUND = 4012;
const int PMT_INTERP_INVALID_SIMPLE_TYPE = 4013;
const int PMT_INTERP_INVALID_LIST_DEF = 4014;
const int PMT_INTERP_ATTRIBUTEREF_NOTFOUND = 4015;
const int PMT_INTERP_INVALID_ATTRIBUTE = 4016;
const int PMT_INTERP_INVALID_ATTRIBUTE_REQUIRED = 4017;
const int PMT_INTERP_INVALID_ATTRIBUTE_PROHIBITED = 4018;
const int PMT_INTERP_INVALID_ATTRIBUTE_FIXED = 4019;
const int PMT_INTERP_INVALID_ATTRIBUTE_NILABLE = 4020;


//: Other Error Codes for PmtCore
const int PMT_CORE_NULL_METADATA_RETURNED = 4101;

//! This class is an EkError which has a component ID of PMT and allows you to set a message SPmtString at construction.
class PMTCORE_DECL PmtError:public EkError
{
  public:

    //! Constructor
    /*! Creates an error with the PMT component ID, the specified 
       error code, and optionally a descriptive message, file name,
       and line number.  The last two arguments can be supplied 
       using the WHERE macro defined in EkError.h, as in
       PmtError err(10, "Error 10 Occurred", WHERE)
     */
    PmtError(int errcode, const char *msg, const char *filename, int line);

    //! Constructor - Receives the message SPmtString as a standard SPmtString object (which is not optional)
     PmtError(int errorcode,
              const SPmtString & msg, const char *filename, int line);
};

#endif

