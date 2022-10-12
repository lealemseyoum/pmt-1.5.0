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


#ifndef PMT_SINGLE_METADATA_VISITOR_T_H
#define PMT_SINGLE_METADATA_VISITOR_T_H



#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#pragma warning (disable:4503)
#endif

#include "PmtCoreDefs.h"
#include "PmtMdVisitorImpl.h"


template < class V > class PmtSingleMetadataVisitorT:public PmtMdVisitorImpl
{
  public:
  PmtSingleMetadataVisitorT(V & u):userVisitor(u)
    {
    }
    virtual ~ PmtSingleMetadataVisitorT(void)
    {
    }

    virtual void visit(PmtMetadata & md)
    {
        userVisitor.visit(md);
    }


    virtual void visit(PmtMetadataT < bool > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < vector < bool > > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < char >&md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < vector < char > > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < unsigned char > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < vector < unsigned char > > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < short >&md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < vector < short > > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < unsigned short > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < vector < unsigned short > > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < long >&md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < vector < long > > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < unsigned long > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < vector < unsigned long > > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < int > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < vector < int > > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < unsigned int > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < vector < unsigned int > > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < float > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < vector < float > > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < double > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < vector < double > > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < string > &md)
    {
        userVisitor.visit(md);
    }
    virtual void visit(PmtMetadataT < vector < string > >&md)
    {
        userVisitor.visit(md);
    }

  private:
    V & userVisitor;
};


#endif                          /* PMT_SINGLE_METADATA_VISITOR_T_H */

