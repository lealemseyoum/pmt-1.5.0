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
 * Original Author: 
 * John Hall  
 *
 * Copyright (C) 1996 Eastman Kodak Company. 
 * 
 * Contributor(s): 
 * Martin R. Hoffman martin.hoffman@kodak.com
 *
 * Date: December 1996
 */

/*! \file
  \brief Macros for the control of template instantiation.

  Comments: Gnu g++ (and possibly other compilers) will compile template
  classes into every source file where they are used.  When several such 
  compilation units are linked together the redundant template code increases
  the size of the resulting system.  These macros allow you to control which
  compilation actually instantiates a  template class, and which ones refer to
  it as an external definition.
 
  Use EK_IMPL_CLASS_TMPL or EK_IMPL_CLASS_TMPL2 where you want the
  class to be compiled.
  
  Use EK_EXTERN_CLASS_TMPL and EK_EXTERN_CLASS_TMPL2 where you want to
  declare the use of a template instance (usually in an appropriate
  header file).
  
  Code may be compiled under g++ and other compilers without using these
  macros.  It will run, but may result in larger than necessary
  executable files.  Failure to use the macros may also prevent the
  proper inlining of methods (depending on compiler).
  
  These macros may be used with any compiler.  They produce null
  results on compilers where they are not needed.
*/

#ifndef EK_TEMPLATE_H
#define EK_TEMPLATE_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#if !defined (EK_COMPILER_H)
#include "EkCompiler.h"
#endif

#if defined (EK_EXTERN_TEMPLATE_DECLS)
#define EK_EXTERN_CLASS_TMPL(templ,param)            \
                    extern template class templ< param >
#define EK_EXTERN_CLASS_TMPL2(templ,p1,p2)           \
                    extern template class templ< p1 , p2 >
#else
#define EK_EXTERN_CLASS_TMPL(templ,param)
#define EK_EXTERN_CLASS_TMPL2(templ,p1,p2)
#endif

#if defined (EK_EXPLICIT_TEMPLATE_IMPL)
#define EK_IMPL_CLASS_TMPL(templ,param)              \
                    template class templ< param >
#define EK_IMPL_CLASS_TMPL2(templ,p1,p2)             \
                    template class templ< p1 , p2 >
#else
#define EK_IMPL_CLASS_TMPL(templ,param)
#define EK_IMPL_CLASS_TMPL2(templ,p1,p2)
#endif

#endif

