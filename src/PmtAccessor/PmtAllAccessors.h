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
 * Creation date: July, 2003
 *
 * Original Author:
 * Sam Fryer <samuel.fryer@kodak.com>
 *
 * Contributor(s):
 *
 *
 */

/***********************************************************
 * This file loads all the accessors that are in the 
 * PMT build that is being used.
 ***********************************************************/
#ifndef PMT_ALL_ACCESSORS_H
#define PMT_ALL_ACCESSORS_H


/***********************************************************
 * Bring in the PMT build configuration
 ***********************************************************/
#include "PmtConfig.h"

/***********************************************************
 * XML Accessor
 ***********************************************************/
#if !defined (PMT_USE_NO_XML_PARSER)
#include "PmtXmlAccessor.h"
#endif

/***********************************************************
 * EXIF Accessor
 ***********************************************************/
#if defined (HAVE_OPENEXIF) || defined (HAVE_KIESEXIF)
#include "PmtExifAccessor.h"
#endif

/***********************************************************
 * TIFF Accessor
 ***********************************************************/
#if defined (HAVE_OPENTIFF) || defined (HAVE_EKTIFF)
#include "PmtTiffAccessor.h"
#endif

/***********************************************************
 * Image accessors
 ***********************************************************/
#if defined (PMT_IMAGE_ACCESSOR_REQUIRED) 
#include "PmtImageAccessor.h"
#include "PmtImageRect.h"
#endif

/***********************************************************
 * DIG35 Accessor
 ***********************************************************/
#if defined (PMT_DIG35)
#include "PmtDIG35Accessor.h"
#endif

/***********************************************************
 * JPEG2000 Accessor
 ***********************************************************/
#if defined (PMT_JPEG2000)
#include "PmtJ2KAccessor.h"
#endif

/***********************************************************
 * PNG Accessor
 ***********************************************************/
#if defined (PMT_PNG_ACCESSOR)
#include "PmtPngAccessor.h"
#endif

/***********************************************************
 * QuickTime Accessor
 ***********************************************************/
#if defined (PMT_QT_ACCESSOR)
#include "PmtQtAccessor.h"
#endif

/***********************************************************
 * XMP Accessor
 ***********************************************************/
#if defined (PMT_XMP_ACCESSOR)
#include "PmtXMPAccessor.h"
#endif

#endif
