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
 * Creation date: February 2002
 *
 * Original Author: 
 * Dan Rupe rupe@kodak.com
 * 
 * Contributor(s): 
 *
*/

#ifdef _MSC_VER
#pragma warning (disable:4786)
#pragma warning (disable:4503)
#endif

#include "PmtMetadataT.h"


template PmtMetadataT < vector < char > >;
template PmtMetadataT < char >;
template PmtMetadataT < vector < unsigned char > >;
template PmtMetadataT < unsigned char >;
template PmtMetadataT < vector < short > >;
template PmtMetadataT < short >;
template PmtMetadataT < vector < unsigned short > >;
template PmtMetadataT < unsigned short >;
template PmtMetadataT < vector < long > >;
template PmtMetadataT < long >;
template PmtMetadataT < vector < unsigned long > >;
template PmtMetadataT < unsigned long >;
template PmtMetadataT < vector < bool > >;
template PmtMetadataT < bool >;
template PmtMetadataT < vector < float > >;
template PmtMetadataT < float >;
template PmtMetadataT < vector < double > >;
template PmtMetadataT < double >;
template PmtMetadataT < vector < string > >;
template PmtMetadataT < string >;

#if defined ( _MSCVER ) && ( _MSC_VER >= 1300 )
// Some people have been complaining of these missing template definitions
// Using VC 7.x
template PmtMetadataT < vector < int > >;
template PmtMetadataT < int >;
template PmtMetadataT < vector < unsigned int > >;
template PmtMetadataT < unsigned int >;
template PmtMetadataT < vector < wstring > >;
template PmtMetadataT < wstring >;
#endif

