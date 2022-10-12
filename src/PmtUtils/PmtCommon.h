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
 * Creation Date: January 1999
 *
 * Original Author: 
 * George Sotak   george.sotak@kodak.com
 * 
 * Contributor(s): 
 */

#ifndef PMT_COMMON_H
#define PMT_COMMON_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#include <list>
#include "EkCompiler.h"

typedef SPmtString PmtKeyType;

typedef list < PmtKeyType > PmtKeyList;

// common require used by application
#if (defined(PMT_DLL) || defined(PMTCORE_DLL) || defined(PMTACCR_IMPL_DLL))
#define PMT_REQUIRE(name)   EK_REQUIRE_IMPORT_DLL(name)
#else
#define PMT_REQUIRE(name)   EK_REQUIRE(name)
#endif

//: Indicates the type of entry
typedef enum
{
    PMT_METADATA_KEYS_ONLY,
    PMT_ALIAS_KEYS_ONLY,
    PMT_ALL_KEYS
}
PmtEntryTypeEnum;

//: Indicates which action to take concerning duplicates in the registry
typedef enum
{
    PMT_SILENTLY_OVERWRITE,     // Overwrite existing common entry
    PMT_SILENTLY_RETAIN,        // Retain existing common entry
    PMT_SILENTLY_APPEND,        // Append new format entries to existing common entry
    PMT_THROW_ERROR             // Throw an error
}
PmtRegistryHandleDuplicate;

//: Indicates the format of the incoming file
typedef enum
{
    PMT_FORMAT_UNKNOWN = 0,
    PMT_FORMAT_EXIF,
    PMT_FORMAT_FPX,
    PMT_FORMAT_TIFF,
    PMT_FORMAT_APS,
    PMT_FORMAT_XML,
    PMT_FORMAT_PNG,
    PMT_FORMAT_JP2,
    PMT_FORMAT_XMP,
    PMT_FORMAT_DIG35,
    PMT_FORMAT_QUICKTIME,
    PMT_FORMAT_LASTINLIST
}
PmtImageFileFormatName;

//: Indicates the status of the conversion
typedef enum
{
    VALUE_OKAY,
    VALUE_CLIPPED,
    VALUE_NOT_IN_ENTRY,
    VALUE_NOT_IN_ARRAY,
    TOO_MANY_VALUES,
    VALUE_TAG_TYPE_UNKNOWN
}
PmtConversionStatus;

//: Indicates the open mode for the incoming file
typedef enum
{
    PMT_CREATE,
    PMT_READONLY,
    PMT_READWRITE
}
PmtOpenMode;


#ifndef RL_BASIC_DATA_TYPES
typedef bool bool_t;

typedef char i8_t;
typedef unsigned char u8_t;
typedef short i16_t;
typedef unsigned short u16_t;
typedef long i32_t;
typedef unsigned long u32_t;

typedef void any_t;
#endif                          /* RL_BASIC_DATA_TYPES */

#ifndef RL_BASIC_DATA_TYPES
#define RL_BASIC_DATA_TYPES
#define I8_MIN -128
#define I8_MAX 127
#define U8_MIN 0
#define U8_MAX 255

#define I16_MIN -32768
#define I16_MAX 32767
#define U16_MIN 0
#define U16_MAX 65535U

#define I32_MIN i32_t(-2147483647 - 1)
#define I32_MAX 2147483647
#define U32_MIN 0
#define U32_MAX 4294967295U

#define I64_MIN -9223372036854775808
#define I64_MAX 9223372036854775807
#define U64_MIN 0
#define U64_MAX 18446744073709551615U
#endif                          /* RL_BASIC_DATA_TYPES */

#endif

