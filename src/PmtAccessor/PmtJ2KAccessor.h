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
 * Creation Date: April 14, 2003
 *
 * Original Author: 
 * Ricardo Rosario ricardo.rosario@kodak.com
 * Copyright (C) 2003 Eastman Kodak Company
 * 
 * Contributor(s): 
 */ 

#include "EkCompiler.h"

#ifdef PMT_JPEG2000

#ifndef PMT_J2K_ACCESSOR__H
#define PMT_J2K_ACCESSOR__H

#include "PmtConfig.h"
#include "PmtCommon.h"
#include "PmtAccrImplDefs.h"
#include "PmtAccessor.h"
#include "PmtDIG35Accessor.h"

#include "jp2.h"
#include "kdu_compressed.h"


class PmtJ2KAccessor ;

typedef EkSmartPtr<PmtJ2KAccessor>       PmtJ2KAccessorPtr;
typedef EkConstSmartPtr<PmtJ2KAccessor>  ConstPmtJ2KAccessorPtr;


/*!
  \author Ricardo Rosario <ricardo.rosario@kodak.com>
  \date   Wed Apr 15 13:26:00 EST 2003

  \brief  Provides access to JPEG 2000 Metadata

*/

class PMTACCR_IMPL_DECL PmtJ2KAccessor : public PmtAccessor
{
  public:
    //! Default constructor
    PmtJ2KAccessor() ;

#ifdef PMT_USE_KAKADU_3_EK
    //! Constructor that takes a JP2 source
    PmtJ2KAccessor( jp2_source* src );
    
    //! Constructor that takes a JP2 target
    PmtJ2KAccessor( jp2_target* snk );
#endif

#ifdef PMT_USE_KAKADU_4
    //! Constructor that takes a JP2 source
    PmtJ2KAccessor( jp2_family_src* src );
    
    //! Constructor that takes a JP2 target
    PmtJ2KAccessor( jp2_family_tgt* snk );
#endif

    //! destructor
    virtual ~PmtJ2KAccessor() ;

    //! Opens a J2K file for reading/writing (depending on the mode)
    virtual bool open(const char *name, PmtOpenMode mode = PMT_READONLY);

    //! Close
    virtual void close() ;

    /*! Return a new instance of the PmtJ2KAccessor. 
        
        NOTE: This method is only intended to be used by the Accessor Factory
            for creating new instances. This is not a deep copy.
     */
    inline virtual PmtAccessorPtr clone() const
    {
        return new PmtJ2KAccessor();
    }
  protected:

    // Check if the given file is a J2K file.
    virtual bool recognize(const SPmtString &name) const;

    virtual bool getMdValue(PmtMetadataPtr& md) ;

    virtual bool setMdValue(const PmtMetadataPtr& md) ;
    
    // Remove the metadata specified by md from the file.
    virtual void removeMdValue(const PmtMetadataPtr & md);

  private:
    PmtOpenMode mMode;
    
    PmtDIG35Accessor* mDIG35Accessor;

#ifdef PMT_USE_KAKADU_3_EK
    jp2_target* mJP2Sink;
#endif

#ifdef PMT_USE_KAKADU_4
    jp2_family_tgt* mJP2Sink;

    void extractAllXML( jp2_input_box* box );
#endif

};

//PMTACCR_IMPL_REQUIRE(PmtJ2KAccessor)

#endif // PMT_J2K_ACCESSOR__H

#endif // PMT_JPEG2000
