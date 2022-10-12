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
 * Creation Date: mm/dd/yyyy
 *
 * Original Author: 
 * FirstName LastName <email address> 
 * <copyright notice>
 *
 * Contributor(s): 
 * FirstName LastName <email address>
 * FirstName LastName <email address>
 */


#ifndef PMT_METADATA_VISITOR_H
#define PMT_METADATA_VISITOR_H



#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#pragma warning (disable:4503)
#endif

#include <vector>

class PmtMetadata;

#include "EkCompiler.h"
#include "PmtCoreDefs.h"

class PMTCORE_DECL PmtMetadataVisitor
{
  public:
    virtual ~ PmtMetadataVisitor()
    {
    }

    virtual void visit(PmtMetadata & md) = 0;

  protected:
    PmtMetadataVisitor()
    {
    }

};

#endif                          /* PMT_METADATA_VISITOR_H */
