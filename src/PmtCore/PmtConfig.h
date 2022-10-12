/* src/PmtCore/PmtConfig.h.  Generated from PmtConfig.h.in by configure.  */
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
 * This file determines how the PMT is built, and can be
 * used by applications to determine what may be present 
 * in the build of PMT that they are dealing with.
 * 
 * Any feature that is #undef'd or commented out was NOT
 * compiled in.
 ***********************************************************/
#ifndef PMT_CONFIG_H
#define PMT_CONFIG_H


/***********************************************************
 * Which XML Parser to use
 * PMT_USE_NO_XML_PARSER ==> No XML.  (limited capability.)
 * PMT_USE_MSXML         ==> MSXML v4 (may work with v3)
 * PMT_USE_GENERIC_DOM   ==> defunct for now
 * PMT_USE_XERCES        ==> Xerces V 1.2
 *                       ==> Xerces V 2.x
 *                       ==> Xerces V 3.0
 ***********************************************************/
/* #undef PMT_USE_NO_XML_PARSER */
/* #undef PMT_USE_MSXML */
/* #undef PMT_USE_GENERIC_DOM */
#define PMT_USE_XERCES 1

/***********************************************************
 * Which Exif Library to use (KIES or OpenExif)
 ***********************************************************/
/* #undef HAVE_KIESEXIF */
#define HAVE_OPENEXIF 1

/***********************************************************
 * Which Tiff Library to use (EkTiff or OpenTiff)
 ***********************************************************/
/* #undef HAVE_EKTIFF */
#define HAVE_OPENTIFF 1

/***********************************************************
 * Whether to build in the image accessors or not
 * (to access the pixel data via PMT)
 ***********************************************************/
#define PMT_IMAGE_ACCESSOR_REQUIRED 1

/***********************************************************
 * Whether to build in the sound accessors or not
 * (to access the sound data via PMT)
 ***********************************************************/
/* #undef PMT_SOUND_ACCESSOR_REQUIRED */

/***********************************************************
 * Whether to build in the DIG35 Accessor or not
 ***********************************************************/
/* #undef PMT_DIG35 */

/***********************************************************
 * Whether to build in the JPEG2000 Accessor or not
 * (Currently, this requires the DIG35 Accessor)
 ***********************************************************/
/* #undef PMT_JPEG2000 */

#ifdef PMT_JPEG2000
/***********************************************************
 * Which J2K library we're using
 * PMT_USE_KAKADU_3_EK ==> Kodak's version of Kakadu 3
 * PMT_USE_KAKADU_4    ==> Kakadu v4 (currently only 4.0.2)
 ***********************************************************/
/* #undef PMT_USE_KAKADU_3_EK */
/* #undef PMT_USE_KAKADU_4 */
#endif

/***********************************************************
 * Whether to build in the PNG Accessor or not
 ***********************************************************/
/* #undef PMT_PNG_ACCESSOR */

/***********************************************************
 * Whether to build in the QuickTime Accessor or not
 ***********************************************************/
#define PMT_QT_ACCESSOR 1

/***********************************************************
 * Whether to build in the XMP Accessor or not
 * This requires that an XML parser listed above is present.
 ***********************************************************/
#define PMT_XMP_ACCESSOR 1

/***********************************************************
 * Whether X is available on the system for the examples
 ***********************************************************/
/* #undef PMT_EXAMPLES_USE_X */

/***********************************************************
 * Whether to enable embedded documentation for each 
 * metadata element.
 ***********************************************************/
#define PMT_USE_EMBEDDED_DOCUMENTATION 1

/***********************************************************
 * Whether to write out schema information as it is loaded.
 * This should only be set if you're creating a new default,
 * pre-parsed schema.  It requires an XML parser to be present.
 ***********************************************************/
/* #undef PMT_WRITE_OUT_SCHEMA */

#endif
