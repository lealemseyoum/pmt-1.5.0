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
 * Ricardo Rosario  ricardo.rosario@kodak.com
*/

#include "EkCompiler.h"

#include "PmtAccessor.h"
#include "PmtMisc.h"
#include "PmtAllAccessors.h"

PmtAccessorPtr PmtAccessor::create(const PmtImageFileFormatName & accrId)
{
    PmtAccessorPtr accr;
    switch (accrId)
    {
#if defined (HAVE_KIESEXIF) || defined (HAVE_OPENEXIF)
        case PMT_FORMAT_EXIF:
            accr = new PmtExifAccessor();
            break;
#endif
#if defined (HAVE_EKTIFF) || defined (HAVE_OPENTIFF)
        case PMT_FORMAT_TIFF:
            accr = new PmtTiffAccessor();
            break;
#endif
#ifndef PMT_USE_NO_XML_PARSER
        case PMT_FORMAT_XML:
            accr = new PmtXmlAccessor();
            break;
#endif
#ifdef PMT_PNG_ACCESSOR
        case PMT_FORMAT_PNG:
            accr = new PmtPngAccessor();
            break;
#endif
#ifdef PMT_QT_ACCESSOR
        case PMT_FORMAT_QUICKTIME:
            accr = new PmtQTAccessor();
            break;
#endif
#ifdef PMT_XMP_ACCESSOR
        case PMT_FORMAT_XMP:
            accr = new PmtXMPAccessor();
            break;
#endif
#ifdef PMT_JPEG2000
        case PMT_FORMAT_JP2:
            accr = new PmtJ2KAccessor();
            break;
#endif
#ifdef PMT_DIG35
        case PMT_FORMAT_DIG35:
            accr = new PmtDIG35Accessor();
            break;
#endif
//         case PMT_FORMAT_UNKNOWN:
//         case PMT_FORMAT_FPX:
//         case PMT_FORMAT_APS:
        default:
            accr = NULL;
    }
    
    return accr;
}

PmtImageFileFormatName PmtAccessor::recognizeFilename(const SPmtString fileName)
{
    PmtImageFileFormatName return_value = PMT_FORMAT_UNKNOWN;
#if defined (HAVE_KIESEXIF) || defined (HAVE_OPENEXIF)
    if (PmtExifAccessor::recognize(fileName))
        return_value = PMT_FORMAT_EXIF;
    else
#endif
#if defined (HAVE_EKTIFF) || defined (HAVE_OPENTIFF)
    if (PmtTiffAccessor::recognize(fileName))
        return_value = PMT_FORMAT_TIFF;
    else
#endif
#ifndef PMT_USE_NO_XML_PARSER
    if (PmtXmlAccessor::recognize(fileName))
        return_value = PMT_FORMAT_XML;
    else
#endif
#ifdef PMT_QT_ACCESSOR
    if (PmtQTAccessor::recognize(fileName))
        return_value = PMT_FORMAT_QUICKTIME;
    else
#endif
#ifdef PMT_XMP_ACCESSOR
    if (PmtXMPAccessor::recognize(fileName))
        return_value = PMT_FORMAT_XMP;
    else
#endif
#ifdef PMT_PNG_ACCESSOR
    if (PmtPngAccessor::recognize(fileName))
        return_value = PMT_FORMAT_PNG;
    else
#endif
#ifdef PMT_JPEG2000
    if (PmtJ2KAccessor::recognize(fileName))
        return_value = PMT_FORMAT_JP2;
    else
#endif
#ifdef PMT_DIG35
    if (PmtDIG35Accessor::recognize(fileName))
        return_value = PMT_FORMAT_DIG35;
    else
#endif
        return_value = PMT_FORMAT_UNKNOWN;
    return return_value;
}
