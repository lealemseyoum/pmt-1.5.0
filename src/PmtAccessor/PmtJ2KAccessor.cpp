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
 * Creation Date: April 15, 2003
 *
 * Original Author: 
 * Ricardo Rosario ricardo.rosario@kodak.com
 * 
 * Contributor(s): 
 */


#include "EkCompiler.h"

#ifdef PMT_JPEG2000

#include <cstdio>

#include "PmtJ2KAccessor.h"

#include <string.h>

// #if _MSC_VER < 1300
//     /*
//      * If this is missing on VC6, the EK_PROVIDE statement (below) will 
//      * generate compile-time errors relating to EkFactoryT.  VC7 compiles 
//      * the code properly.  I'm not sure why adding a dummy, unused, factory 
//      * object allows VC6 to compile this, but it works, so don't knock it!
//      */
//     static PmtAccessorFactoryType gVc6IsStupid_DoNotUse;
// #endif
// 
// #if defined ( __GNUC__ ) && ( __GNUC__ > 2 )
// // SMF 2/11/2002 -- This is a work-around for a bug (ICE) in the GCC 3.0.3 compiler.
// int *PmtJ2KAccessor_tmp = (int*)PMT_ACCESSOR_TYPE_INIT(PMT_FORMAT_JP2, PmtJ2KAccessor); 
// const int *PmtJ2KAccessor_tmp2[] = { PmtJ2KAccessor_tmp};
// const int *PmtJ2KAccessor_FILE[1];
// void * PmtJ2KAccessor_FILE_Addr = &PmtJ2KAccessor_FILE;
// bool PmtJ2KAccessor_tmp3 = (PmtJ2KAccessor_FILE_Addr = &PmtJ2KAccessor_tmp2);
// #else
// EK_PROVIDE(PmtJ2KAccessor, PMT_ACCESSOR_TYPE_INIT(PMT_FORMAT_JP2, PmtJ2KAccessor))
// #endif


// Constructor
PmtJ2KAccessor::PmtJ2KAccessor()
  : PmtAccessor(PMT_FORMAT_JP2), mDIG35Accessor(NULL) ,mJP2Sink(NULL)
{ }

#ifdef PMT_USE_KAKADU_3_EK
// Constructor that takes a JP2 source
PmtJ2KAccessor::PmtJ2KAccessor( jp2_source* src )
  : PmtAccessor(PMT_FORMAT_JP2), mJP2Sink(NULL)
{
    jp2_xml_data xmlBoxes = src->access_xml();

    int numOfXMLBoxes = xmlBoxes.get_num_boxes();
    mDIG35Accessor = new PmtDIG35Accessor();
    if ( !mDIG35Accessor->open() )
    {
        delete mDIG35Accessor;
        mDIG35Accessor = NULL;
    }
    else
    {
        for( int i = 0; i < numOfXMLBoxes; i++ )
        {
            int bufferSize = 0;
            kdu_byte * xmlData = xmlBoxes.get_data( i, &bufferSize );
    
            mDIG35Accessor->append( xmlData, bufferSize );
        }
    }
}

// Constructor that takes a JP2 target
PmtJ2KAccessor::PmtJ2KAccessor( jp2_target* snk )
  : PmtAccessor(PMT_FORMAT_JP2), mJP2Sink(snk)
{
    mDIG35Accessor = new PmtDIG35Accessor();
    
    if ( !mDIG35Accessor->open() )
    {
        delete mDIG35Accessor;
        mDIG35Accessor = NULL;
    }
}
#endif

#ifdef PMT_USE_KAKADU_4
//! Constructor that takes a JP2 source
PmtJ2KAccessor::PmtJ2KAccessor( jp2_family_src* src )
  : PmtAccessor(PMT_FORMAT_JP2)
{
    // if we have an open jp2 source (it exists) then, open the first box
    // and recursively extract all the XML (this is done in the extractAllXML
    // function).
    if ( src->exists() )
    {
        jp2_input_box* inputBox = new jp2_input_box();

        if ( inputBox->open(src) )
            extractAllXML(inputBox);

        delete inputBox;
    } 
}

// Constructor that takes a JP2 target
PmtJ2KAccessor::PmtJ2KAccessor( jp2_family_tgt* snk )
  : PmtAccessor(PMT_FORMAT_JP2), mJP2Sink(snk)
{
    mDIG35Accessor = new PmtDIG35Accessor();
    
    if ( !mDIG35Accessor->open() )
    {
        delete mDIG35Accessor;
        mDIG35Accessor = NULL;
    }
}
#endif

// Destructor
PmtJ2KAccessor::~PmtJ2KAccessor() 
{
    close() ;
}

// close the accessor
void PmtJ2KAccessor::close() 
{
#ifdef PMT_USE_KAKADU_3_EK
    if ( mJP2Sink!=NULL && mDIG35Accessor!=NULL )
    {
        int reqBufSize = - mDIG35Accessor->close(NULL,0);
        kdu_byte * buf = new kdu_byte[reqBufSize];
        if ( mDIG35Accessor->close((char*)buf,reqBufSize) == reqBufSize )
            mJP2Sink->access_xml().add_box(buf,reqBufSize);

        delete buf;
    
        // Get rid of our reference to the jp2_target. However, do not delete
        // because we don't own it.
        mJP2Sink = NULL;
    }
#endif

#ifdef PMT_USE_KAKADU_4
    if ( mJP2Sink!=NULL && mDIG35Accessor!=NULL )
    {
        int reqBufSize = - mDIG35Accessor->close(NULL,0);
        kdu_byte * buf = new kdu_byte[reqBufSize];
        if ( mDIG35Accessor->close((char*)buf,reqBufSize) == reqBufSize )
        {
            jp2_output_box* box = new jp2_output_box();
            box->open( mJP2Sink, jp2_xml_4cc );
            box->write( buf,reqBufSize );
            box->close();
            delete box;
        }

        delete buf;
    
        // Get rid of our reference to the jp2_target. However, do not delete
        // because we don't own it.
        mJP2Sink = NULL;
    }
#endif

    if ( mDIG35Accessor!=NULL )
    {
        delete mDIG35Accessor;
        mDIG35Accessor = NULL;
    }
}

// Open the specified file
bool PmtJ2KAccessor::open(const char *filename, PmtOpenMode mode)
{
    bool success = false;
    close();
    
    if ( mode == PMT_READONLY )
    {
#ifdef PMT_USE_KAKADU_3_EK
        jp2_source inputFile;
        jp2_xml_data xmlBoxes;
    
        inputFile.open( filename );
        xmlBoxes = inputFile.access_xml();
    
        int numOfXMLBoxes = xmlBoxes.get_num_boxes();
        mDIG35Accessor = new PmtDIG35Accessor();
        if ( !mDIG35Accessor->open() )
        {
            delete mDIG35Accessor;
            mDIG35Accessor = NULL;
        }
        else
        {
            for( int i = 0; i < numOfXMLBoxes; i++ )
            {
                int bufferSize = 0;
                kdu_byte * xmlData = xmlBoxes.get_data( i, &bufferSize );
        
                mDIG35Accessor->append( xmlData, bufferSize );
            }
            success = true;
        }
    
        inputFile.close();
#endif

#ifdef PMT_USE_KAKADU_4
        jp2_family_src* inputFile = new jp2_family_src();
        inputFile->open(filename);
        
        mDIG35Accessor = new PmtDIG35Accessor();
        if ( !mDIG35Accessor->open() )
        {
            delete mDIG35Accessor;
            mDIG35Accessor = NULL;
        }
        else
        {
            if ( inputFile->exists() )
            {
                jp2_input_box* inputBox = new jp2_input_box();
                if ( inputBox->open(inputFile) )
                {
                    extractAllXML(inputBox);
                    success = true;
                }
            
                delete inputBox;
            }
        }
        
        delete inputFile;
#endif

    }
    
    return success;
}

#ifdef PMT_USE_KAKADU_4
// This function will recursively extract all the XML from the boxes and pass
// them on to the DIG35 accessor.
void PmtJ2KAccessor::extractAllXML( jp2_input_box* box )
{
    if ( box->get_box_type() == jp2_xml_4cc )
    {
        // this is an xml box
        // create a buffer to read in the XML, and then pass it on to the
        // DIG35 accessor
        int bufferSize = box->get_box_bytes();
        kdu_byte* buf = new kdu_byte[bufferSize];
        box->read( buf, bufferSize );
        
        mDIG35Accessor->append( buf, bufferSize );
        
        // clean up
        delete buf;
    }

    /*else
    {
        cout << "Not an XML box!!!" << endl;
        jp2_input_box* child = new jp2_input_box();
        cout << "trying to open child box" << endl;
        child->open(box);
        cout << "checking if child exists..." << endl;
        while ( child->exists() )
        {
            extractAllXML(child);
            child->close();
            child->open_next();
            cout << "child does exist" << endl;
            
            // process child!!!
        }
        cout << "deleting child" << endl;
        delete child;
    }*/

    // close the box and go on to the next on if it exists.
    box->close();
    box->open_next();
    if (box->exists())
        extractAllXML( box );
}
#endif

// recognize a J2K file
bool PmtJ2KAccessor::recognize(const SPmtString &name) const
{
    // Look at the file extension. This is not the best solution, but Kakadu
    // doesn't provide a function for determining whether a file is a valid
    // jp2 file
    bool success = true;
    
    SPmtString::size_type pos = name.length()-4;
    SPmtString fileExt = name.substr(pos);
    if ( fileExt!=".jp2" && fileExt!=".JP2" && fileExt!=".jP2" &&
        fileExt!=".Jp2" )
        success = false;
    
    return success;
}

bool PmtJ2KAccessor::getMdValue( PmtMetadataPtr& md )
{
    // we currently only support DIG35 metadata so the DIG35 accessor handles
    // it for us
    bool success = false;
    if ( mDIG35Accessor!=NULL )
        success = mDIG35Accessor->getMdValue(md);
    
    return success;
}

bool PmtJ2KAccessor::setMdValue( const PmtMetadataPtr& md )
{    
    // we currently only support DIG35 metadata so the DIG35 accessor handles
    // it for us
    bool success = false;
    if ( mDIG35Accessor!=NULL )
        success = mDIG35Accessor->setMdValue(md);
    
    return success;
}

// Remove the metadata specified by md from the file.
void PmtJ2KAccessor::removeMdValue(const PmtMetadataPtr & md)
{
    if ( mDIG35Accessor!=NULL )
        mDIG35Accessor->removeMdValue(md);
}

#endif // PMT_JPEG2000
