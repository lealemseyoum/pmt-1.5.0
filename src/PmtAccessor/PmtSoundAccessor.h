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
 * Creation Date: 3/15/07
 *
 * Original Author: 
 * Sam Fryer <samuel.fryer@kodak.com> 
 * 
 * Contributor(s): 
 * 
 */

#ifdef PMT_SOUND_ACCESSOR_REQUIRED

#ifndef PMT_SOUND_ACCESSOR__H
#define PMT_SOUND_ACCESSOR__H

#include "PmtCommon.h"

#include "EkCompiler.h"
#include "EkSmartPtr.h"
#include "EkRefCount.h"


// Define some supported image types here to prevent confusion.
// This should really only be used for accessing detailed imaging
// that only one type of accessor can provide.  Only advanced users 
// (or picky image scientists) should use these.
#define PmtSoundAccessor_UNKNOWN           0
#define PmtSoundAccessor_WAV               1
#define PmtSoundAccessor_ULAW              2
#define PmtSoundAccessor_RAW               3


/*! The PmtSoundAccessor is the base class for image accessor implementations.

    An image accessor's primary responsibility is to provide applications
    with read / write access to the main image and thumbnail image data.
*/
class PMTACCR_IMPL_DECL PmtSoundAccessor : public EkRefCount < EK_DEFAULT_MUTEX >
{
  public:
    //! Constructor
    PmtSoundAccessor(unsigned char soundType = PmtSoundAccessor_UNKNOWN)
      : mcSoundType(soundType)
    {
    }

    //! Destructor
    ~PmtSoundAccessor()
    {
    }

    /*! 
        Get the declared type of the image class.
     */
    unsigned char getSoundType()
    {
        return mcSoundType;
    }
    
	// retrieve sound data
	//  returns 0 on success, non-zero = error.
	virtual int readSound() = 0;

	// write the sound data to a file
	//  returns 0 on success, non-zero = error.
	virtual int writeSoundToFile(SPmtString filename) = 0;

  protected:
    unsigned char mcSoundType;
    
  private:
};

// Smart pointers
typedef EkSmartPtr < PmtSoundAccessor > PmtSoundAccessorPtr;
typedef EkConstSmartPtr < PmtSoundAccessor > PmtConstSoundAccessorPtr;

#endif                          // PMT_SOUND_ACCESSOR__H

#endif                          // PMT_SOUND_ACCESSOR_REQUIRED

