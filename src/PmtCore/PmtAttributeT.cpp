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
 * Creation date: February 2002
 *
 * Original Author: 
 * Dan Rupe rupe@kodak.com
 * 
 * Contributor(s): 
 *
*/

#include "PmtAttribute.h"
#include "PmtAttributeT.h"


template class PmtAttributeT < vector < char > >;
template class PmtAttributeT < char >;
template class PmtAttributeT < vector < unsigned char > >;
template class PmtAttributeT < unsigned char >;
template class PmtAttributeT < vector < short > >;
template class PmtAttributeT < short >;
template class PmtAttributeT < vector < unsigned short > >;
template class PmtAttributeT < unsigned short >;
template class PmtAttributeT < vector < long > >;
template class PmtAttributeT < long >;
template class PmtAttributeT < vector < unsigned long > >;
template class PmtAttributeT < unsigned long >;
template class PmtAttributeT < vector < bool > >;
template class PmtAttributeT < bool >;
template class PmtAttributeT < vector < float > >;
template class PmtAttributeT < float >;
template class PmtAttributeT < vector < double > >;
template class PmtAttributeT < double >;
template class PmtAttributeT < vector < SPmtString > >;
template class PmtAttributeT < SPmtString >;

