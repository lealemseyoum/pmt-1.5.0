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
 * Copyright (C) 1999-2000 Eastman Kodak Company. All 
 * Rights Reserved. 
 * 
 * Original Author: 
 * Lin, Chris ti.lin@kodak.com
 * Duda, Diane J. 970704N@knotes.kodak.com
 * 
 * Contributor(s): 
 * Rupe, Dan
 * Upadhyay, Kundan
 * Sam Fryer <samuel.fryer@kodak.com>
 * 
 * Date: July 7, 2000
*/

#if (defined _MSC_VER)
#pragma warning (disable:4503)
#pragma warning (disable:4786)

#include <wchar.h>
#endif

#include <exception>
#include <fstream>
#include "EkCompiler.h"
#include "PmtInstanceInterpreter.h"
#include "PmtError.h"
#include "PmtCoreCommon.h"
#include "PmtMisc.h"
#include "PmtCompositeMetadata.h"
#include "PmtAttributeT.h"
#include "PmtXMLInstanceParser.h"

const char *const NAMESPACE_URI_NOTFOUND = "NamespaceURINotFound";


/******************************************************************************/
/* Method:             constructor                                            */
/* Input parameters:   NONE                                                   */
/* Functionality:      This method initializes all pointers to NULL,          */
/*                     and creates a new root PMT Metadata pointer            */
/* Output:             NONE                                                   */
/******************************************************************************/
PmtLogicalInstanceInterpreter::PmtLogicalInstanceInterpreter()
    :mOpenMode(PMT_READWRITE), mInSync(true), mTnsPrefix(""),mMdFactory(NULL)
{
    mRootMd = PmtMetadata::create();
}

/******************************************************************************/
/* Method:             constructor                                            */
/* Input parameters:   PmtMetadataPtr                                         */
/* Functionality:      This method initializes all pointers to NULL, and      */
/*                     mRootMd to the passed in MD pointer                    */
/* Output:             NONE                                                   */
/******************************************************************************/
PmtLogicalInstanceInterpreter::PmtLogicalInstanceInterpreter(PmtMetadataPtr md)
    :mRootMd(md), mOpenMode(PMT_READWRITE), mInSync(true), 
     mTnsPrefix(""), mMdFactory(NULL)
{
}

/******************************************************************************/
/* Method:             destructor                                             */
/* Input parameters:   NONE                                                   */
/* Functionality:      This method deletes the filter, filename, and parser,  */
/*                     pointers if they exist.                                */
/* Output:             NONE                                                   */
/******************************************************************************/
PmtLogicalInstanceInterpreter::~PmtLogicalInstanceInterpreter()
{
}

/******************************************************************************/
/* Method:             open                                                   */
/* Input parameters:   const SPmtString&                                      */
/* Functionality:      This method opens the given file for reading & writing */
/*                     If the file exists, but is empty, create an empty      */
/*                     DOMNode tree. If the file exists and contains data,    */
/*                     parse the file. If the file does not exist, create an  */
/*                     empty file & an empty DOMNode tree. Set the inSync     */
/*                     flag to true.                                          */
/* Output:             NONE                                                   */
/******************************************************************************/
bool PmtLogicalInstanceInterpreter::open(const SPmtString & fileName,
                                         PmtOpenMode mode)
{
    bool returnValue = true;
    if (!fileName.empty())
    {
      int openMode = ios::in;
      if (mode == PMT_READONLY)
          openMode = ios::in;
      else if (mode == PMT_CREATE)
          openMode = ios::out | ios::trunc;
      else if (mode == PMT_READWRITE)
          openMode = ios::in | ios::out;

      if (mode != PMT_CREATE)
      {
              returnValue = parseFile(fileName);
      }

      if (returnValue)
      {
        // for readonly and readwrite, just lock the file here so that others 
        // won't be able to change it    
#if (defined _MSC_VER) | (defined macintosh)
        mFstream.open(fileName.c_str(), (ios_base::openmode) openMode);
#elif (defined __GNUC__)
        mFstream.open(fileName.c_str(), (ios::openmode) openMode);
#endif
        if (mFstream.fail())
            returnValue = false;

        mOpenMode = mode;
        mFileName = fileName;
        mInSync = true;
      }
    }
    else
        returnValue = false;
    return returnValue;
}

/******************************************************************************/
/* Method:             validate                                               */
/* Input parameters:   NONE                                                   */
/* Functionality:      This method validates the DOMNode Tree.                */
/* Output:             NONE                                                   */
/******************************************************************************/
bool PmtLogicalInstanceInterpreter::validate()
{
    bool success = true;
    return success;
}

/******************************************************************************/
/* Method:             flush                                                  */
/* Input parameters:   NONE                                                   */
/* Functionality:      If the DOMNode tree is not empty and the root node     */
/*                     has child nodes, this method calls the write() function*/
/*                     and sets the inSync flag to true.                      */
/* Output:             NONE                                                   */
/******************************************************************************/
void PmtLogicalInstanceInterpreter::flush()
{
    if ((mOpenMode != PMT_READONLY) && (mFstream.is_open()))
    {
        // aaaha, flush here still have to write everything out from the 
        // beginning of the stream
        if (mRootMd)
        {
            if (!mInSync)
            {
                mFstream.close();
                writeXML(mFileName);
                mFstream.open(mFileName.c_str()
#if defined( __GNUC__ ) && ( __GNUC__ < 3 )
                              ,ios::in | ios::out | ios::ate
#endif
                );
            }
        }
    }
    mInSync = true;
}

/******************************************************************************/
/* Method:             close                                                  */
/* Input parameters:   NONE                                                   */
/* Functionality:      If the DOMNode tree is not empty and the root node has */
/*                     children...if the file & the tree are not inSync, this */
/*                     method will call the write() function, then close the  */
/*                     file. If the file & tree are inSync or if there isn't  */
/*                     a tree or children in the tree, it just closes the     */
/*                     file if it was open.                                   */
/* Output:             NONE                                                   */
/******************************************************************************/
void PmtLogicalInstanceInterpreter::close()
{
    if (mFstream.is_open())
    {
        flush();
        mFstream.close();
        mFileName = "";
    }
    mMdFactory = NULL;
}


/******************************************************************************/
/* Method:             readMetadata(PmtMetadataPtr)                           */
/* Input parameters:   PmtMetadataPtr - the parent pointer                    */
/* Functionality:      This method will do one of two things. If the parent   */
/*                     metadata is a composite, it will find the parent in the*/
/*                     DOM Tree, retrieve the list of its children, iterate   */
/*                     through the list of children and fill in their values. */
/*                     If the parent metadata is not a composite - this means */
/*                     it is a high-level simple type, it will simply retrieve*/
/*                     the value, fill in the parent metadata and return the  */
/*                     parent.                                                */
/* Output:             The PmtMetadataPtr that was passed in will be returned */
/*                     with all values filled in.                             */
/******************************************************************************/
PmtMetadataPtr PmtLogicalInstanceInterpreter::readMetadata(PmtMetadataPtr &md)
{
    if (md)
    {
        if (checkNamespace(md))
        {
             SPmtString fullKey = md->fullKey(false);
            if (fullKey == ROOT_ELEMENT_NAME )
            {
                md = mRootMd;
            }
            else
            {
                if (md->isComposite())
                {
                  // take care of the children
                  // What about attributes?
                  PmtMetadataIterator mdIter = md->getMetadata("*[*]", false);    
                  PmtMetadataPtr mdPtr = mdIter.start();
                  PmtMetadataPtr tmd;
                  while (mdPtr)
                  {
                      tmd = mRootMd->getMetadatum(mdPtr->fullKey(false),false);
                      if (tmd)
                      {
                          //Performance?
                          mdPtr->setValueStr(tmd->getValueStr());
                      }
                      mdPtr = mdIter.next();
                  }
                }
                else
                {
                  // Now the value itself
                  PmtMetadataPtr tmd = mRootMd->getMetadatum(fullKey,false);
                  if (tmd)
                      //Performance?
                      md->setValueStr(tmd->getValueStr());
                }
            }
        }
    }

    return md;
}

// Simply return all the metadata we've read (i.e., our root).
PmtMetadataPtr PmtLogicalInstanceInterpreter::readMetadata()
{
    return mRootMd;
}

/******************************************************************************/
/* Method:             writeMetadata(PmtMetadataPtr)                          */
/* Input parameters:   PmtmetadataPtr - the ptr to the parent node            */
/* Functionality:      Copies the Metadata in "md" to mRootMd                 */
/* Output:             NONE                                                   */
/******************************************************************************/
// Note: Could we use something like "clone"ing here instead of the actual
// copying?
void PmtLogicalInstanceInterpreter::writeMetadata(const PmtMetadataPtr & md)
{
    if ((mOpenMode != PMT_READONLY) &&
        (md) &&
        (checkNamespace(md)))
    {
      mInSync = false;
      SPmtString mdKey;
      mdKey = md->key();

      if (md->isComposite())
      {
        PmtMdIterList theList = 
                       ((PmtCompositeMetadata *)md.ptr())->getMdIterList();
        PmtMdIterListIter curIter = theList.begin();
        PmtMdIterListIter end = theList.end();
        bool totallyEmpty = true;
        while(totallyEmpty && (curIter != end))
        {
            if (!(*curIter)->second->isEmpty())
                totallyEmpty = false;
            curIter++;
        }
        curIter = theList.begin();
        
        if (!totallyEmpty && (curIter != end))
        {
          PmtMetadataPtr newMd;
          if (md->fullKey() == ROOT_ELEMENT_NAME )
              newMd = mRootMd;
          else
              newMd = mRootMd->getMetadatum(md->fullKey(),true);
          
          PmtAttributeMap attrMap = md->getAttributes(false);
          PmtAttributeMap::const_iterator attrMapIter = attrMap.begin();
          while (attrMapIter != attrMap.end())
          {
              if ((*attrMapIter).second)
              {
                  newMd->setAttribute((*attrMapIter).second->name(),
                                      (*attrMapIter).second);
              }
              attrMapIter++;
          }
    
          while (curIter != end)
          {
            if (!(*curIter)->second->isEmpty())
            {
              int i = 1;
              PmtMetadataPtr childMd =(*curIter)->second->getMetadatum(0,false);
              while (childMd)
              {
                writeMetadata(childMd);
                childMd = (*curIter)->second->getMetadatum(i,false);
                i++;
              }
            }

            curIter++;
          }
        }
      }
      else
      {
          PmtMetadataPtr newMd = mRootMd->getMetadatum(md->fullKey(),true);
          if (newMd)
          {
              // Could this be done a little more effeciently?
              newMd->setValueStr(md->getValueStr());
          }
          else
              throw PmtError(PMT_ERROR, "No Metadata found", WHERE);
      }
    }
}

SPmtString printXMLLeaders(ostream & out, PmtMetadataPtr md,
                           SPmtString last, SPmtString tab,
                           SPmtString deltaTab)
{
    SPmtString fullKey = md->fullKey();
    SPmtString myTab = tab;
    if (fullKey != ROOT_ELEMENT_NAME)
    {
        out << myTab << "<" << ROOT_ELEMENT_NAME << ">";
        myTab += deltaTab;
    }
    while (fullKey != md->key())
    {
        // there has to be a "." as that is the definition of
        // more than one item in a key.
        unsigned long pos = fullKey.find('.');
        out << myTab << "<" << fullKey.substr(0,pos) << ">";
        fullKey = fullKey.substr(pos+1,fullKey.length());
        myTab += deltaTab;
    }
    return myTab;
}

SPmtString printXMLTrailers(ostream & out, PmtMetadataPtr md,
                           SPmtString last, SPmtString tab,
                           SPmtString deltaTab)
{
    SPmtString fullKey = md->fullKey();
    SPmtString myTab = tab;
    if (fullKey != md->key())
    {
        while (!fullKey.empty())
        {
            myTab = myTab.substr(0,myTab.length()-deltaTab.length());
            unsigned long pos = fullKey.find_last_of('.');
            fullKey = fullKey.substr(0,pos);
            unsigned long pos2 = fullKey.find_last_of('.');
            if (pos2 == SPmtString::npos)
            {
                out << myTab << "</" << fullKey << ">";
                fullKey = "";
            }
            else
            {
                out << myTab << "</" << fullKey.substr(pos2+1,fullKey.length()) << ">";
                fullKey = fullKey.substr(0,pos2);
            }
        }
    }
    myTab = myTab.substr(0,myTab.length()-deltaTab.length());
    if (md->fullKey() != ROOT_ELEMENT_NAME)
        out << myTab << "</" << ROOT_ELEMENT_NAME << ">";
    return myTab;
}

/******************************************************************************/
/* Method:             writeXML()                                             */
/* Input parameters:   ostream & out (where to write the XML)                 */
/* Functionality:      This method traverses the Metadata Tree and writes all */
/*                     nodes and their values in XML format out to the        */
/*                     specified output stream.                               */
/* Output:             Filled ostream with XML                                */
/******************************************************************************/
void PmtLogicalInstanceInterpreter::writeXML(ostream & out)
{
    if ((mRootMd) && 
        (mOpenMode != PMT_READONLY) &&
        (checkNamespace(mRootMd)) )
    {
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>";

        // Since we may not be working with the true root, we have to print out
        // the nodes above our root
        SPmtString newTab = printXMLLeaders(out,mRootMd," ","\n","\t");
        
        // Now write out our node and children
        writeXML(out,mRootMd," ",newTab,"\t");
        
        // Now write out the end nodes of our parents 
        // (if we're not the true root)
        printXMLTrailers(out,mRootMd," ",newTab,"\t");
    }
}

/******************************************************************************/
/* Method:             writeXML()                                             */
/* Input parameters:   ostream & out (where to write the XML)                 */
/*                     SPmtStrings for setting the tabs                       */
/* Functionality:      This method traverses the Metadata Tree and writes all */
/*                     nodes and their values in XML format out to the        */
/*                     specified output stream.                               */
/* Output:             Filled ostream with XML                                */
/******************************************************************************/
void PmtLogicalInstanceInterpreter::writeXML(ostream & out, PmtMetadataPtr md,
                                             SPmtString last, SPmtString tab,
                                             SPmtString deltaTab)
{
    SPmtString mdKey;
    mdKey = md->key();

    if (md->isComposite())
    {
        PmtMdIterList theList = 
                            ((PmtCompositeMetadata *)md.ptr())->getMdIterList();
        PmtAttributeMap attrMap = md->getAttributes(false);
        PmtMdIterListIter curIter = theList.begin();
        PmtMdIterListIter end = theList.end();
        bool totallyEmpty = true;
        
        if (attrMap.empty())
        {
            while(totallyEmpty && (curIter != end))
            {
                if (!(*curIter)->second->isEmpty())
                    totallyEmpty = false;
                curIter++;
            }
        }
        curIter = theList.begin();
        
        if (!totallyEmpty && (curIter != end))
        {
          out << tab << "<" << mdKey;
          
          PmtAttributeMap::const_iterator attrMapIter = attrMap.begin();
          while (attrMapIter != attrMap.end())
          {
              if ((*attrMapIter).second)
              {
                  bool isString = false;
                  if ((*attrMapIter).second->trueType()=="<SPmtString>")
                      isString = true;
                  out << " " << (*attrMapIter).second->name() << "=";
                  if (isString) out << '\"';
                  out << EK_WIDE2STR( (*attrMapIter).second->getValueStr() );
                  if (isString) out << '\"';
              }
              attrMapIter++;
          }
    
          out << ">";
          while (curIter != end)
          {
            if (!(*curIter)->second->isEmpty())
            {
              int i = 1;
              PmtMetadataPtr childMd =(*curIter)->second->getMetadatum(0,false);
              while (childMd)
              {
                writeXML(out,childMd,last,tab+deltaTab,deltaTab);
                childMd = (*curIter)->second->getMetadatum(i,false);
                i++;
              }
            }

            curIter++;
          }

          out << tab << "</" << mdKey << ">";
        }
    }
    else
    {
        out << tab << "<" << mdKey << ">" << EK_WIDE2STR( md->getValueStr() )
            << "</" << mdKey << ">";
    }
}


/******************************************************************************/
/* Method:             writeXML()                                             */
/* Input parameters:   SPmtString fileName                                    */
/* Functionality:      This method traverses the metadata tree and writes all */
/*                     nodes and their values in XML format out to the        */
/*                     specified output file.                                 */
/* Output:             File with XML                                          */
/******************************************************************************/
void PmtLogicalInstanceInterpreter::writeXML(SPmtString fileName)
{
    if (!fileName.empty())
    {
        ofstream f(fileName.c_str());
        writeXML(f);
    }
    else
        writeXML(cout);
}

/******************************************************************************/
/* Method:             removeMetadata(PmtMetadataPtr)                         */
/* Input parameters:   PmtmetadataPtr - the ptr to the parent node            */
/* Functionality:      removes the Metadata in "md" from the mRootMd          */
/* Output:             NONE                                                   */
/******************************************************************************/
void PmtLogicalInstanceInterpreter::removeMetadata(const PmtMetadataPtr & md)
{
    if ((mOpenMode != PMT_READONLY) && (md))
    {
        mRootMd->deleteMetadata(md->fullKey());
        mInSync = false;
    }
}

/******************************************************************************
 * Method:             parseFile(SPmtString&)                                 *
 * Input parameters:   SPmtString& - the filename containing the XML Instance *
 * Functionality:      This method parses the file, retrieves the doc node    *
 *                     creates a treeWalker and retrieves & saves the root    *
 *                     node of the DOM Tree in a PmtMetadata structure.       *
 * Output:             NONE                                                   *
 ******************************************************************************/
bool PmtLogicalInstanceInterpreter::parseFile(const SPmtString & fileName)
{
    bool returnValue = false;

    if (!fileName.empty())
    {
        PmtXMLInstanceParser xmlParser(fileName);
        returnValue = xmlParser.parse();
        if (returnValue)
        {
           nsInit();
           mRootMd = xmlParser.getRootMetadata();
        }
    }

    return returnValue;
}

// This reads in an XML stream and converts it to a PmtMetadata type structure
bool PmtLogicalInstanceInterpreter::open(istream & in)
{
    bool returnValue = false;
    PmtXMLInstanceParser xmlParser(in);
    returnValue = xmlParser.parse();
    if (returnValue)
       mRootMd = xmlParser.getRootMetadata();
       
    return returnValue;
}

//Function to return the URI of the given namespace
SPmtString PmtLogicalInstanceInterpreter::
getNamespaceURI(const SPmtString & prefix) const
{
    map < SPmtString, SPmtString >::const_iterator s = mURIMap.find(prefix);
    if (s != mURIMap.end())
        return (*s).second;
    else
        return NAMESPACE_URI_NOTFOUND;
}

// Function to see if we're aware of the namespace used by the md ptr.
bool PmtLogicalInstanceInterpreter::checkNamespace(PmtMetadataPtr md)
{
    bool returnValue = false;
    
    PmtMetadataFactory *fact = md->getMdFactory();
    if (mMdFactory)
    {
        if (mMdFactory == fact)
            returnValue = true;
    }
    else
    {
        SPmtString uri = getNamespaceURI(mTnsPrefix);
        if (uri == NAMESPACE_URI_NOTFOUND || uri == md->getNameSpace())
        {
            mMdFactory = fact;
            returnValue = true;
        }
    }
    return returnValue;
}

// Function to set the current working namespace prefix 
bool PmtLogicalInstanceInterpreter::setNSPrefix(const SPmtString & nsPrefix)
{
    if (mTnsPrefix.empty() && mURIMap.size() == 0)
    {
        mTnsPrefix = nsPrefix;
        return true;
    }
    return false;
}

// initialize namespaces...(???)
void PmtLogicalInstanceInterpreter::nsInit()
{
    // todo: check elementForm and reset mTnsPrefix to null
//     DOMElement *root = mDoc->getDocumentElement();
//     const XMLCh * domStr = root->getPrefix();
//     PmtDomString pmtStr((XMLCh*)domStr);
//     mTnsPrefix = pmtStr.getString();
//     domStr = root->getNamespaceURI();
//     PmtDomString pmtStr2((XMLCh*)domStr);
    mTnsPrefix = "";
    SPmtString nameSpaceURI = "";
    mURIMap.insert(map < SPmtString,
                   SPmtString >::value_type(mTnsPrefix, nameSpaceURI));

    // todo: check other namespaces
}

