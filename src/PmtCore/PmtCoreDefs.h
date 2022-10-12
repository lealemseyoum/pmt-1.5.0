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
 * Creation Date: Unknown
 *
 * Original Author: 
 * Unknown
 * 
 * Contributor(s): 
*/

#ifndef PMT_CORE_DEFS_H
#define PMT_CORE_DEFS_H

#include "EkFactoryT.h"

#ifdef _MSC_VER
#if defined( PMTCORE_BUILD_DLL)
#define PMTCORE_DECL __declspec(dllexport)
#define PMTCORE_EXPLICIT 'e'
#define PMTCORE_REQUIRE(name) EK_REQUIRE_EXPORT_DLL(name)
#elif  (defined( PMT_DLL) || defined( PMTCORE_DLL))
#define PMTCORE_DECL __declspec(dllimport)
#define PMTCORE_EXPLICIT 'i'
#define PMTCORE_TMPL_EXT extern
#define PMTCORE_REQUIRE(name) EK_REQUIRE_IMPORT_DLL(name)
#endif
#endif

#if !defined( PMTCORE_REQUIRE )
#define PMTCORE_REQUIRE(name) EK_REQUIRE(name)
#endif

#ifndef PMTCORE_DECL
#define PMTCORE_DECL
#endif

#ifndef PMTCORE_TMPL_EXT
#define PMTCORE_TMPL_EXT
#endif

#endif                          // PMT_CORE_DEFS_H

