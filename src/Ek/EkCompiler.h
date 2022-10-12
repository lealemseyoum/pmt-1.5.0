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
 * Creation Date: 08/1997
 *
 * Original Author: 
 * Martin R. Hoffman martin.hoffman@kodak.com
 * 
 * Contributor(s): 
*/

/** \file
    \brief This file defines a number of macros whose values depend on
    characteristics of the compiler being used to build the framework.
    
    The following macros are used to distinguish the compilers being
    used for various builds:
<pre>    
    __mips          SGI's C++ compiler
    __SUNPRO_CC     Sun's C++ compiler with Rogue Wave's STL
    __GNUC__        GNU C++ compiler
    _MSC_VER        Microsoft Visual C++
    _WIN32_WCE      Windows CE compiler
</pre>
*/

/*  WARNING DISABLING
 *
 *      There are a number of VC++ warnings which EK disables, because
 *      they are safe to ignore in the context in which EK gets them
 *      (and they are very annoying).  If you don't want EK disabling
 *      any warnings, make sure EK_ALL_WARNINGS is defined before
 *      EkCompiler.h is included in the compilation unit.
 *
 *      You can also selectively prevent warnings from being disabled
 *      by defining EK_ALLOW_WARN_xxxx where xxxx is the warning number.
 *      Look below to see which warning numbers we're disabling.
 *
 *      If you define EK_MOST_WARNINGS, then the only warning which 
 *      EK will disable is the horrid 4786 which is the one about 
 *      debugger symbols being truncated at 256 characters.  If you
 *      have any experience with STL in VC++, you'll know that it is
 *      better to define EK_MOST_WARNINGS than EK_ALL_WARNINGS.
*/

#ifndef EK_COMPILER_H
#define EK_COMPILER_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

// Include the configuration file 
#ifdef HAVE_CONFIG_H
#undef HAVE_CONFIG_H
#endif 
#include "PmtConfig.h"

#ifdef _MSC_VER

#if (! defined EK_ALL_WARNINGS) && (! defined EK_MOST_WARNINGS)

//:
// We can disable any warning which the user does not
// explicitly tell us to leave intact

#ifndef EK_ALLOW_WARN_4231
//:
// C4231: nonstandard extension used: 'extern' before template instantiation
// This warning results from the way in which Microsoft says we need to 
// declare template instantiations for import from the EK DLLs.  Microsoft
// says to ignore this warning.
#pragma warning (disable:4231)
#endif

#ifndef EK_ALLOW_WARN_4237
//:
// C4237: nonstandard extension used : 'bool' keyword is reserved for future use.
// This is an old left over warning.  bool is a reserved word and should be
// usable without producing any warnings.  But in early versions of the VC++
// compiler which supported bool, you still get this warning.
#pragma warning (disable:4237)
#endif

#ifndef EK_ALLOW_WARN_4251
//:
// C4251: <identifier> : class <XXX> needs to have dll-interface to be used by 
//                       clients of class <YYY>
// This warning is unavoidable when exporting instantiations of certain STL
// classes.  In particular, when one exports an STL map instantiation, the
// warning is issued for an instantiation of class _Tree<> which depends on
// the map.  There's no way to export _Tree<> until the map is instantiated, 
// at which point it is too late.  Microsoft suggests it be ignored in this case.
//
// Note that there are cases where this warning is avoidable.  Unfortunately,
// selectively disabling it only when we know it is unavoidable does not work.
#pragma warning (disable:4251)
#endif

#ifndef EK_ALLOW_WARN_4661
//:
// C4661: <identifier> : no suitable definition provided for explicit template
//                       instantiation request
// This occurs when you are exporting a template instantiation which is abstract.
// You get this warning for each pure virtual function.  This does not happen
// when exporting an abstract class which is not a template instantiation.
#pragma warning (disable:4661)
#endif

#ifndef EK_ALLOW_WARN_4786
//:
// C4786: <identifier> truncated to 256 characters...
// This occurs when mangled STL names are too long for the debugger symbol
// table.  Microsoft says it is okay to ignore it.
#pragma warning (disable:4786)
#endif

#ifndef EK_ALLOW_WARN_4503
//:
// C4503: 'identifier' : decorated name length exceeded, name was truncated
// The decorated name was longer than the maximum the compiler allows (247), 
// and was truncated.
#pragma warning (disable:4503)
#endif

#endif

#ifdef EK_MOST_WARNINGS

// the user will allow us to disable the horrid warning 4786, but no others
#pragma warning (disable:4786)

#endif

#endif

//:
//  EK_TYPENAME
//
//      Some compilers do not support the "typename" keyword correctly, and
//      so they don't require/accept it where they should.  Other compilers do.
//      For now, only GCC is complaining, but others can be added as needed.
//
#if (defined __GNUC__) || ((defined _MSC_VER) && (_MSC_VER >= 1300))
#define EK_TYPENAME typename
#endif
// default
#ifndef EK_TYPENAME
#define EK_TYPENAME
#endif

//:
// EK_DECL & EK_TMPL_EXT & EK_EXPLICIT
//
//      These macro are to be used in EK header files only!
//
//      For non-Windows platforms, these macros will be defined as nothing.
//
//      For Windows platforms, these macros will be defined based on the 
//      context in which the EK headers will be used:
//
//          - if we are building EK into a static library, these macros
//            will be defined as nothing
//
//          - if we are using EK with the intent of linking EK statically,
//            these macros will be defined as nothing.
//
//          - if we are building an EK DLL, EK_DECL will be defined as
//            "__declspec(dllexport)" and EK_TMPL_EXT will be defined as nothing.
//
//          - if we are using EK with the intent of linking to an EK DLL,
//            EK_DECL will be defined as "__declspec(dllimport)" and EK_TMPL_EXT
//            will be defined as "extern".
//
//      EK_EXPLICIT is only defined if EK_DECL is defined as a declspec.
//
//      It is important that applications which intend to use an EK DLL
//      define a preprocessor symbol of EK_DLL before EkCompiler.h or any
//      other EK header file is directly or indirectly included in any
//      compilation unit which is part of the application.

#ifdef _MSC_VER
/* Windows platforms */
#if    (defined EK_BUILD_DLL)

#define EK_DECL __declspec(dllexport)
#define EK_EXPLICIT
#elif  (defined EK_DLL)
#define EK_DECL __declspec(dllimport)
#define EK_EXPLICIT
#define EK_TMPL_EXT extern
#endif

#endif

#ifndef EK_DECL
#define EK_DECL
#endif

#ifndef EK_TMPL_EXT
#define EK_TMPL_EXT
#endif

//: 
// EK_USING_STD
//
//    For compilers which recognize namespaces, this macro translates to
//    the following:
//
//        namespace std { }; using namespace std;
//
//    Otherwise the macro is defined as the empty string.

#ifdef __mips
#define EK_USING_STD
#endif

#ifdef __SUNPRO_CC
#define EK_USING_STD
#endif

#if defined (__GNUC__) && ( __GNUC__ < 3 )
#ifdef EK_USE_NAMESPACE
#define EK_USING_STD namespace std { }; using namespace std;
#else
#define EK_USING_STD
#endif
#endif

#ifdef _WIN32_WCE
#define EK_USING_STD
#endif

#ifdef _MSC_VER
#if (_MSC_VER < 1100)
#define EK_USING_STD
#else
#define EK_USING_STD namespace std { }; using namespace std;
#endif
#endif

#ifndef EK_USING_STD
#define EK_USING_STD namespace std { }; using namespace std;
#endif                          // default

EK_USING_STD
/* Only replace the STL map (map) with EkMapT when we are compiling
 * for a DLL.
 */
#if ( defined(_MSC_VER) && (defined(EK_BUILD_DLL) || defined(EK_DLL) ) )
#include "EkMapT.h"
#if defined(EK_DLL_MAP)
#undef EK_DLL_MAP
#endif
#define EK_DLL_MAP( k, v )  EkMapT < k, v >
#if defined(EK_DLL_MULTIMAP)
#undef EK_DLL_MULTIMAP
#endif
#define EK_DLL_MULTIMAP( k, v )  EkMultiMapT < k, v >
#if defined(EK_DLL_MAP_L)
#undef EK_DLL_MAP_L
#endif
#define EK_DLL_MAP_L(k,v,l)       EkMapT<k, v, l >
#if defined(EK_DLL_MULTIMAP_L)
#undef EK_DLL_MULTIMAP_L
#endif
#define EK_DLL_MULTIMAP_L(k,v,l)  EkMultiMapT<k, v, l >
#else                           // NOT DLL COMPILE SO USE REGULAR MAP
#if defined(EK_DLL_MAP)
#undef EK_DLL_MAP
#endif
#define EK_DLL_MAP( k, v )  map<k, v>
#if defined(EK_DLL_MULTIMAP)
#undef EK_DLL_MULTIMAP
#endif
#define EK_DLL_MULTIMAP( k, v )  multimap<k, v>
#if defined(EK_DLL_MAP_L)
#undef EK_DLL_MAP_L
#endif
#define EK_DLL_MAP_L(k,v,l)  map<k,v,l>
#if defined(EK_DLL_MULTIMAP_L)
#undef EK_DLL_MULTIMAP_L
#endif
#define EK_DLL_MULTIMAP_L(k,v,l)  multimap<k,v,l>
#endif
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <list>
//:
//  EK_EXTERN_TEMPLATE_DECLS & EK_EXPLICIT_TEMPLATE_IMPL
//
//      Some compilers support explicit template instantiation, in which
//      the non-inline code for a template class is only compiled into  
//      module if the compiler is told to do so.  The first of these two
//      macros determine whether the compiler supports external template
//      declarations (i.e., declarations of template instantiations which
//      do not trigger the non-inline code to be compiled) and the second
//      determines whether the compiler supports explicit instantiation.
//
//      These macros are used in EkTemplate.h to define macros which
//      can be used to conveniently do extern and explicit decls.
#ifdef __mips
#endif
#ifdef __SUNPRO_CC
#endif
#ifdef __GNUC__
#define EK_EXTERN_TEMPLATE_DECLS  1
#define EK_EXPLICIT_TEMPLATE_IMPL 1
#endif
#ifdef _MSC_VER
//#define EK_EXPLICIT_TEMPLATE_IMPL 1
#endif
//:
//  EK_COMPILER_HAS_BOOL
//
//      Some compilers do not support the data type "bool".  The file
//      EkBool.h will test EK_COMPILER_HAS_BOOL, to determine whether to 
//      use a typedef to define "bool" and #define for "true" and "false".
//
//      In the case of the microsoft compiler, we supress the warning 
//      about the "bool" name being a future reserved word.
#define EK_COMPILER_HAS_BOOL 1
// default
#ifdef __SUNPRO_CC
#undef EK_COMPILER_HAS_BOOL
#endif
#if ((defined _MSC_VER) && (_MSC_VER < 1100))
#undef EK_COMPILER_HAS_BOOL
#endif
//:
//  POSIX DEFINES
//
//      In order for other modules to determine the availability of 
//      certain POSIX features, this header file includes unistd.h.
//      This file may not exist on all platforms, so the inclusion 
//      is conditional based on compiler.
/*
 *      This is really more of a platform issue, rather than a compiler
 *      issue, so we should probably be checking for things like "solaris"
 *      versus "windows NT", rather than by compiler.
 */
#ifdef __mips
#include "unistd.h"
#endif
#ifdef __SUNPRO_CC
#include "unistd.h"
#endif
#ifdef __GNUC__
#include "unistd.h"
#endif
#ifdef _MSC_VER
#endif
// Include any string information
#include "EkStrings.h"
#endif                          // EK_COMPILER_H

