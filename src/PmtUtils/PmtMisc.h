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
 * Creation Date: 08/15/2000
 *
 * Original Author: 
 * Dan Rupe  rupe@kodak.com
 * 
 * Contributor(s): 
*/

#ifndef PMT_MISC_H
#define PMT_MISC_H

#include <stdio.h>
#include "PmtCoreDefs.h"


const int MAX_PATH_LEN = 512;


//! Copies file.
/*! \param overwriteIfExists determines whether or not to overwrite toPath 
     file, if it already exists.
 \return true if success; false if failure.
*/
extern PMTCORE_DECL bool PmtCopyFile(const char *fromPath, const char *toPath,
                                     bool overwriteIfExists);

//! A version of fopen that works on all our platforms.
extern PMTCORE_DECL FILE *PmtFOpen(const char *filename, const char *mode);

//! Does file exist
extern PMTCORE_DECL bool PmtFileExists(const char *filename);

//! Return modified path for PMT use.
extern PMTCORE_DECL SPmtString PmtChangePathSlashes(const char *path);

extern PMTCORE_DECL bool PmtGetOSSpecificFilePathForXmlParser(SPmtString &filePath);

#if macintosh
//! Change Unix-like path to traditional MacIntosh style path. 
extern void PmtChangePathStyleFromUnixToMac(SPmtString & filePath);
#endif

#endif                          // PMT_MISC_H

