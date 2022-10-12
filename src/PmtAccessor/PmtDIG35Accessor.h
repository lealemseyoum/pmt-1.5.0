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
 * Creation Date: March 2003
 *
 * Original Author: 
 * Ricardo Rosario ricardo.rosario@kodak.com
 * Copyright (C) 2003 Eastman Kodak Company
 * 
 * Contributor(s): 
 */ 

#include "PmtConfig.h"

#if (defined PMT_DIG35) && ((defined PMT_USE_XERCES_2) || (defined PMT_USE_MSXML))

#ifndef PMT_DIG35_ACCESSOR__H
#define PMT_DIG35_ACCESSOR__H

#include "PmtCommon.h"
#include "PmtAccrImplDefs.h"
#include "PmtAccessor.h"

#ifdef PMT_USE_XERCES_2
#include "xercesc/parsers/XercesDOMParser.hpp"
#include "xercesc/dom/DOMNode.hpp"
#endif

#ifdef PMT_USE_MSXML
#include "msxml2.h"
#define XMLCh wchar_t
#endif

class PmtDIG35Accessor ;

typedef EkSmartPtr<PmtDIG35Accessor>       PmtDIG35AccessorPtr;
typedef EkConstSmartPtr<PmtDIG35Accessor>  ConstPmtDIG35AccessorPtr;

class PmtJ2KAccessor;

/*!
  \author Ricardo Rosario <ricardo.rosario@kodak.com>
  \date   Wed Mar 26 14:22:00 EST 2003

  \brief  Provides access to DIG35 Metadata

*/

class PMTACCR_IMPL_DECL PmtDIG35Accessor : public PmtAccessor
{
    // RLR -> I don't like this solution of having PmtJ2KAccessor being a friend
    // to be able to call getMdValue and setMdValue. Maybe this methods should
    // be moved to the public interface, and comment them so that apps don't use
    // them. DISCUSS in code review!
    friend class PmtJ2KAccessor;
    
  public:
    //! Default constructor
    PmtDIG35Accessor() ;
        
    //! destructor
    virtual ~PmtDIG35Accessor() ;

    /*! Opens a DIG35 text file for reading/writing (depending on the mode)
     
        \param name The name of the file to open
        \param mode The open mode
        \return true if successful, false otherwise   
     */
    virtual bool open(const char *name, PmtOpenMode mode = PMT_READONLY);
    
    /*! Opens a DIG35 document from memory
    
        \param buf The buffer with the document to open
        \param bufSize The size of the buffer
        \param mode The open mode
        \return true if successful, false otherwise
     */
    bool open(const unsigned char * buf, const unsigned int bufSize,
              PmtOpenMode mode = PMT_READONLY);

    /*! Creates a new DIG35 document
    
        \return true if successful, false otherwise
     */
    bool open();

    /*! Appends/merges a DIG35 text file to an already open document
         
        \param name The name of the file to append
        \return true if successful, false otherwise  
     */
    bool append( const char *name );

    /*! Appends/merges a DIG35 document from memory to an already open document
        
        \param buf The buffer with the document to open
        \param bufSize The size of the buffer
        \return true if successful, false otherwise
     */
    bool append( const unsigned char * buf, const unsigned int bufSize );

    //! Close
    virtual void close() ;

    /*! Close to Memory
    
        The caller is responsible for creating and deleting the passed in
        buffer.
        \param buf The buffer to use for writing the document
        \param bufSize The size of the buffer
        \return positive int equal to the size of the document, if successful;
                 negative int with absolute value equal to the required buffer
                    size, if passed in buffer isn't big enough;
                 0 for any other error.
     */
     int close( char * buf, unsigned int bufSize );


    /*! Return a new instance of the PmtDIG35Accessor. 
        
        NOTE: This method is only intended to be used by the Accessor Factory
            for creating new instances. This is not a deep copy.
     */
    inline virtual PmtAccessorPtr clone() const
    {
        return new PmtDIG35Accessor();
    }
    
  protected:

    // Check if the given file is a DIG35 file.
    virtual bool recognize(const SPmtString &name) const;

    // get the value from the DOM and set it on the Metadata object passed in
    virtual bool getMdValue(PmtMetadataPtr& md) ;

    // get the value from the Metadata object and set it in the DOM
    virtual bool setMdValue(const PmtMetadataPtr& md) ;
    
    // Remove the metadata specified by md from the file.
    virtual void removeMdValue(const PmtMetadataPtr & md);

  private:
    PmtOpenMode mMode;

#ifdef PMT_USE_XERCES_2
    // the DIG35 document
    XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument* mDoc;

    // get the DOM node specified by the fullDIG35Key
    XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* getDOMNode( const char* fullDIG35Key,
        bool create = false );
#endif

    // check if given key is that of a DIG35 metadata block
    bool isMetadataBlockKey( const XMLCh* key );


#ifdef PMT_USE_MSXML
    // get the DOM node specified by the fullDIG35Key
    IXMLDOMNode* getDOMNode( const char * fullDIG35Key, bool create = false );

    // the DIG35 document
    IXMLDOMDocument* mDoc;
#endif

};

//PMTACCR_IMPL_REQUIRE(PmtDIG35Accessor)

// end of PMT_DIG35_ACCESSOR__H
#endif

// end of PMT_DIG35 && (PMT_USE_XERCES_2 || PMT_USE_MSXML)
#endif
