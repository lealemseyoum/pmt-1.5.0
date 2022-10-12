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
 * Creation Date: 01/29/2001
 *
 * Original Author: 
 * Lin, Chris  ti.lin@kodak.com
 * Dan Rupe  rupe@kodak.com
 * Copyright (C) 2001 Eastman Kodak Company. All Rights Reserved. 
 * 
 * Contributor(s):
 * Sam Fryer  <samuel.fryer@kodak.com>
 * 
*/

// ATTENTION
// Several lines here have been commented out to reduce the memory footprint and 
// to decrease the time required for initialization.  These lines represent 
// technically possible, although highly unlikely, schema facet constructs.  
// Should you need one of these, simply remove the pertinent comments and 
// recompile.  -- Sam.


#include "EkCompiler.h"
#include "PmtFacetT.h"
#include "PmtDefinitionTypes.h"

//PmtFacetFactoryType>   PmtFacet::msFactory;
PmtFacetFactoryType PmtFacet::msFactory;


#define FACET_TYPE_INIT(name, type, facet)     \
    PmtFacet::getFactory().addEntry(((SPmtString() += name) += "-") += type, \
                                    new facet())


//===== EK_PROVIDE for Enum

EK_PROVIDE(PmtEnumFacet_int8,
           FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_INT8,
                           PmtFacetEnumeration < char > )) 
EK_PROVIDE(PmtEnumFacet_vint8, 
           FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_VINT8,
                           PmtFacetEnumeration < vector < char > > ))
EK_PROVIDE(PmtEnumFacet_uint8,
           FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_UINT8,
                           PmtFacetEnumeration < unsigned char > )) 
EK_PROVIDE(PmtEnumFacet_vuint8,
           FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_VUINT8,
                           PmtFacetEnumeration < vector < unsigned char > > ))
EK_PROVIDE(PmtEnumFacet_int16,
           FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_INT16,
                           PmtFacetEnumeration < short > )) 
// EK_PROVIDE(PmtEnumFacet_vint16,
//            FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_VINT16,
//                            PmtFacetEnumeration < vector < short > > ))
EK_PROVIDE(PmtEnumFacet_uint16,
           FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_UINT16,
                           PmtFacetEnumeration < unsigned short > )) 
// EK_PROVIDE(PmtEnumFacet_vuint16,
//            FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_VUINT16,
//                            PmtFacetEnumeration < vector < unsigned short > > ))
EK_PROVIDE(PmtEnumFacet_int32,
           FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_INT32,
                           PmtFacetEnumeration < long > )) 
// EK_PROVIDE(PmtEnumFacet_vint32,
//            FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_VINT32,
//                            PmtFacetEnumeration < vector < long > > ))
EK_PROVIDE(PmtEnumFacet_uint32,
           FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_UINT32,
                           PmtFacetEnumeration < unsigned long > )) 
// EK_PROVIDE(PmtEnumFacet_vuint32,
//            FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_VUINT32,
//                            PmtFacetEnumeration < vector < unsigned long > > ))
// EK_PROVIDE(PmtEnumFacet_boolean,
//            FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_BOOLEAN,
//                            PmtFacetEnumeration < bool > )) 
// EK_PROVIDE(PmtEnumFacet_vboolean,
//            FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_VBOOLEAN,
//                            PmtFacetEnumeration < vector < bool > > ))
// EK_PROVIDE(PmtEnumFacet_float,
//            FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_FLOAT,
//                            PmtFacetEnumeration < float > )) 
// EK_PROVIDE(PmtEnumFacet_vfloat,
//            FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_VFLOAT,
//                            PmtFacetEnumeration < vector < float > > ))
// EK_PROVIDE(PmtEnumFacet_double,
//            FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_DOUBLE,
//                            PmtFacetEnumeration < double > )) 
// EK_PROVIDE(PmtEnumFacet_vdouble,
//            FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_VDOUBLE,
//                            PmtFacetEnumeration < vector < double > > ))
EK_PROVIDE(PmtEnumFacet_string,
           FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_STRING,
                           PmtFacetEnumeration < string > )) 
EK_PROVIDE(PmtEnumFacet_vstring,
           FACET_TYPE_INIT(FACET_ENUMERATION, PMT_TRUE_TYPE_VSTRING,
                           PmtFacetEnumeration < vector < string >  > ))


//===== EK_PROVIDE for MinInclusive
EK_PROVIDE(PmtMinIncFacet_int8,
           FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_INT8,
                           PmtFacetMinInclusive < char > )) 
EK_PROVIDE(PmtMinIncFacet_vint8, 
           FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_VINT8,
                           PmtFacetMinInclusive < vector < char > > ))
EK_PROVIDE(PmtMinIncFacet_uint8,
           FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_UINT8,
                           PmtFacetMinInclusive < unsigned char > )) 
EK_PROVIDE(PmtMinIncFacet_vuint8,
           FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_VUINT8,
                           PmtFacetMinInclusive < vector < unsigned char > > ))
EK_PROVIDE(PmtMinIncFacet_int16,
           FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_INT16,
                           PmtFacetMinInclusive < short > )) 
EK_PROVIDE(PmtMinIncFacet_vint16,
           FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_VINT16,
                           PmtFacetMinInclusive < vector < short > > ))
EK_PROVIDE(PmtMinIncFacet_uint16,
           FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_UINT16,
                           PmtFacetMinInclusive < unsigned short > )) 
EK_PROVIDE(PmtMinIncFacet_vuint16,
           FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_VUINT16,
                           PmtFacetMinInclusive < vector < unsigned short > > ))
EK_PROVIDE(PmtMinIncFacet_int32,
           FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_INT32,
                           PmtFacetMinInclusive < long > )) 
EK_PROVIDE(PmtMinIncFacet_vint32,
           FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_VINT32,
                           PmtFacetMinInclusive < vector < long > > ))
EK_PROVIDE(PmtMinIncFacet_uint32,
           FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_UINT32,
                           PmtFacetMinInclusive < unsigned long > )) 
EK_PROVIDE(PmtMinIncFacet_vuint32,
           FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_VUINT32,
                           PmtFacetMinInclusive < vector < unsigned long > > ))
// EK_PROVIDE(PmtMinIncFacet_boolean,
//            FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_BOOLEAN,
//                            PmtFacetMinInclusive < bool > )) 
// EK_PROVIDE(PmtMinIncFacet_vboolean,
//            FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_VBOOLEAN,
//                            PmtFacetMinInclusive < vector < bool > > ))
EK_PROVIDE(PmtMinIncFacet_float,
           FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_FLOAT,
                           PmtFacetMinInclusive < float > )) 
EK_PROVIDE(PmtMinIncFacet_vfloat,
           FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_VFLOAT,
                           PmtFacetMinInclusive < vector < float > > ))
EK_PROVIDE(PmtMinIncFacet_double,
           FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_DOUBLE,
                           PmtFacetMinInclusive < double > )) 
EK_PROVIDE(PmtMinIncFacet_vdouble,
           FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_VDOUBLE,
                           PmtFacetMinInclusive < vector < double > > ))
// EK_PROVIDE(PmtMinIncFacet_string,
//            FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_STRING,
//                            PmtFacetMinInclusive < string > )) 
// EK_PROVIDE(PmtMinIncFacet_vstring,
//            FACET_TYPE_INIT(FACET_MIN_INCLUSIVE, PMT_TRUE_TYPE_VSTRING,
//                            PmtFacetMinInclusive < vector < string >  > ))


//===== EK_PROVIDE for MaxInclusive
EK_PROVIDE(PmtMaxIncFacet_int8,
           FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_INT8,
                           PmtFacetMaxInclusive < char > )) 
EK_PROVIDE(PmtMaxIncFacet_vint8, 
           FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_VINT8,
                           PmtFacetMaxInclusive < vector < char > > ))
EK_PROVIDE(PmtMaxIncFacet_uint8,
           FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_UINT8,
                           PmtFacetMaxInclusive < unsigned char > )) 
EK_PROVIDE(PmtMaxIncFacet_vuint8,
           FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_VUINT8,
                           PmtFacetMaxInclusive < vector < unsigned char > > ))
EK_PROVIDE(PmtMaxIncFacet_int16,
           FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_INT16,
                           PmtFacetMaxInclusive < short > )) 
EK_PROVIDE(PmtMaxIncFacet_vint16,
           FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_VINT16,
                           PmtFacetMaxInclusive < vector < short > > ))
EK_PROVIDE(PmtMaxIncFacet_uint16,
           FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_UINT16,
                           PmtFacetMaxInclusive < unsigned short > )) 
EK_PROVIDE(PmtMaxIncFacet_vuint16,
           FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_VUINT16,
                           PmtFacetMaxInclusive < vector < unsigned short > > ))
EK_PROVIDE(PmtMaxIncFacet_int32,
           FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_INT32,
                           PmtFacetMaxInclusive < long > )) 
EK_PROVIDE(PmtMaxIncFacet_vint32,
           FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_VINT32,
                           PmtFacetMaxInclusive < vector < long > > ))
EK_PROVIDE(PmtMaxIncFacet_uint32,
           FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_UINT32,
                           PmtFacetMaxInclusive < unsigned long > )) 
EK_PROVIDE(PmtMaxIncFacet_vuint32,
           FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_VUINT32,
                           PmtFacetMaxInclusive < vector < unsigned long > > ))
// EK_PROVIDE(PmtMaxIncFacet_boolean,
//            FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_BOOLEAN,
//                            PmtFacetMaxInclusive < bool > )) 
// EK_PROVIDE(PmtMaxIncFacet_vboolean,
//            FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_VBOOLEAN,
//                            PmtFacetMaxInclusive < vector < bool > > ))
EK_PROVIDE(PmtMaxIncFacet_float,
           FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_FLOAT,
                           PmtFacetMaxInclusive < float > )) 
EK_PROVIDE(PmtMaxIncFacet_vfloat,
           FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_VFLOAT,
                           PmtFacetMaxInclusive < vector < float > > ))
EK_PROVIDE(PmtMaxIncFacet_double,
           FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_DOUBLE,
                           PmtFacetMaxInclusive < double > )) 
EK_PROVIDE(PmtMaxIncFacet_vdouble,
           FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_VDOUBLE,
                           PmtFacetMaxInclusive < vector < double > > ))
// EK_PROVIDE(PmtMaxIncFacet_string,
//            FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_STRING,
//                            PmtFacetMaxInclusive < string > )) 
// EK_PROVIDE(PmtMaxIncFacet_vstring,
//            FACET_TYPE_INIT(FACET_MAX_INCLUSIVE, PMT_TRUE_TYPE_VSTRING,
//                            PmtFacetMaxInclusive < vector < string >  > ))


// Not sure why we needed all these EK_REQUIRES since we just did the
// EK_PROVIDE for the same stuff......

// // ===== EK_REQUIRE for Enum
// EK_REQUIRE(PmtEnumFacet_int8)
// EK_REQUIRE(PmtEnumFacet_vint8)
// EK_REQUIRE(PmtEnumFacet_uint8)
// EK_REQUIRE(PmtEnumFacet_vuint8)
// EK_REQUIRE(PmtEnumFacet_int16)
// // EK_REQUIRE(PmtEnumFacet_vint16)
// EK_REQUIRE(PmtEnumFacet_uint16)
// // EK_REQUIRE(PmtEnumFacet_vuint16)
// EK_REQUIRE(PmtEnumFacet_int32)
// // EK_REQUIRE(PmtEnumFacet_vint32)
// EK_REQUIRE(PmtEnumFacet_uint32)
// // EK_REQUIRE(PmtEnumFacet_vuint32)
// // EK_REQUIRE(PmtEnumFacet_boolean)
// // EK_REQUIRE(PmtEnumFacet_vboolean)
// // EK_REQUIRE(PmtEnumFacet_float)
// // EK_REQUIRE(PmtEnumFacet_vfloat)
// // EK_REQUIRE(PmtEnumFacet_double)
// // EK_REQUIRE(PmtEnumFacet_vdouble)
// EK_REQUIRE(PmtEnumFacet_string) 
// EK_REQUIRE(PmtEnumFacet_vstring)

// //===== EK_REQUIRE for MinInclusive
// EK_REQUIRE(PmtMinIncFacet_int8)
// EK_REQUIRE(PmtMinIncFacet_vint8)
// EK_REQUIRE(PmtMinIncFacet_uint8)
// EK_REQUIRE(PmtMinIncFacet_vuint8)
// EK_REQUIRE(PmtMinIncFacet_int16)
// EK_REQUIRE(PmtMinIncFacet_vint16)
// EK_REQUIRE(PmtMinIncFacet_uint16)
// EK_REQUIRE(PmtMinIncFacet_vuint16)
// EK_REQUIRE(PmtMinIncFacet_int32)
// EK_REQUIRE(PmtMinIncFacet_vint32)
// EK_REQUIRE(PmtMinIncFacet_uint32)
// EK_REQUIRE(PmtMinIncFacet_vuint32)
// // EK_REQUIRE(PmtMinIncFacet_boolean)
// // EK_REQUIRE(PmtMinIncFacet_vboolean)
// EK_REQUIRE(PmtMinIncFacet_float)
// EK_REQUIRE(PmtMinIncFacet_vfloat)
// EK_REQUIRE(PmtMinIncFacet_double)
// EK_REQUIRE(PmtMinIncFacet_vdouble)
// // EK_REQUIRE(PmtMinIncFacet_string) 
// // EK_REQUIRE(PmtMinIncFacet_vstring)

// //===== EK_REQUIRE for MaxInclusive
// EK_REQUIRE(PmtMaxIncFacet_int8)
// EK_REQUIRE(PmtMaxIncFacet_vint8)
// EK_REQUIRE(PmtMaxIncFacet_uint8)
// EK_REQUIRE(PmtMaxIncFacet_vuint8)
// EK_REQUIRE(PmtMaxIncFacet_int16)
// EK_REQUIRE(PmtMaxIncFacet_vint16)
// EK_REQUIRE(PmtMaxIncFacet_uint16)
// EK_REQUIRE(PmtMaxIncFacet_vuint16)
// EK_REQUIRE(PmtMaxIncFacet_int32)
// EK_REQUIRE(PmtMaxIncFacet_vint32)
// EK_REQUIRE(PmtMaxIncFacet_uint32)
// EK_REQUIRE(PmtMaxIncFacet_vuint32)
// // EK_REQUIRE(PmtMaxIncFacet_boolean)
// // EK_REQUIRE(PmtMaxIncFacet_vboolean)
// EK_REQUIRE(PmtMaxIncFacet_float)
// EK_REQUIRE(PmtMaxIncFacet_vfloat)
// EK_REQUIRE(PmtMaxIncFacet_double)
// EK_REQUIRE(PmtMaxIncFacet_vdouble)
// // EK_REQUIRE(PmtMaxIncFacet_string) 
// // EK_REQUIRE(PmtMaxIncFacet_vstring)

