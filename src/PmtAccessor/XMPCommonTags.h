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
* Creation Date: 10/10/1999
*
* Original Author: 
* Philip Wu   hanbing.wu@kodak.com
* 
* Contributor(s):
*/
#ifndef XMP_COMMON_TAGS__H
#define XMP_COMMON_TAGS__H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

//! This file defines some common XMP element tags

#define XMPMETADATA_ROOTTAG L"adobe:ns:meta/xmpmeta"
#define XMPMETADATA_RDFTAG L"http://www.w3.org/1999/02/22-rdf-syntax-ns#RDF"
#define XMPMETADATA_RDFDESCRIPTIONTAG L"http://www.w3.org/1999/02/22-rdf-syntax-ns#Description"
#define XMPMETADATA_BAGTAG L"http://www.w3.org/1999/02/22-rdf-syntax-ns#Bag"
#define XMPMETADATA_SEQTAG L"http://www.w3.org/1999/02/22-rdf-syntax-ns#Seq"
#define XMPMETADATA_ALTTAG L"http://www.w3.org/1999/02/22-rdf-syntax-ns#Alt"
#define XMPMETADATA_LITAG L"http://www.w3.org/1999/02/22-rdf-syntax-ns#li"
#define XMLMETADATA_LANGTAG L"http://www.w3.org/XML/1998/namespacelang"
#define XMPMETADATA_TIFFTAG L"http://ns.adobe.com/tiff/1.0/"
#define XMPMETADATA_EXIFTAG L"http://ns.adobe.com/exif/1.0/"
#define XMPMETADATA_DMTAG L"http://ns.adobe.com/xmp/1.0/mm/"

#endif