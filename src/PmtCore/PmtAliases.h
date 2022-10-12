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
 * Creation Date: 11/02/2000
 *
 * Original Author: 
 * Dan Rupe  rupe@kodak.com
 * 
 * Contributor(s): 
*/

#ifndef PMT_ALIASES_H
#define PMT_ALIASES_H

#ifdef _MSC_VER
#pragma warning (disable:4503)
#pragma warning (disable:4786)
#endif

#include "EkCompiler.h"
#include "PmtCommon.h"
#include "PmtCoreDefs.h"
#include "EkLocking.h"

typedef EK_DLL_MAP(PmtKeyType, PmtKeyList) PmtAliasMap;

//! The PmtAliases class maintains the association between aliases and their assigned group of metadata keys
class PMTCORE_DECL PmtAliases
{
  public:
    //! The default constructor
    inline PmtAliases()
    {
    }
    //! The destructor
    inline ~ PmtAliases()
    {
    }

    //! This returns a list of metadata keys associated with the alias key
    /*! \param aliasKey is of type \em PmtKeyType and is the key or identifier
       for the alias. This is the SPmtString assigned to the \em AliasKey attribute
       of the \em MetadataAlias construct in the alias definition file.
       \param returnKeyList is of type \em PmtKeyList and is the list of
       metadata keys that were assigned to the alias.
     */
    void getKeyList(const PmtKeyType & aliasKey,
                    PmtKeyList & returnKeyList) const;

    //! This method inserts an alias into the array
    /*! \param aliasKey is of type \em PmtKeyType and is the key or identifier
       for the alias. This is the SPmtString assigned to the \em AliasKey attribute
       of the \em MetadataAlias construct in the alias definition file.
       \param aliasmembers is of type \em PmtKeyList and is the list of
       metadata keys that were assigned to the alias.
     */
    void insert(const PmtKeyType & aliasKey, const PmtKeyList & aliasMembers);

    void clear();

    void show();

  private:
    PmtAliasMap mAliasMap;
    mutable EK_DEFAULT_MUTEX mLock;

    // AliasSubStrComp is an STL binary predicate that performs the sub-<SPmtString>
    // comparisons needed by findAliases() to search for partially qualified keys.     
    struct AliasSubStrComp:public binary_function < PmtAliasMap::value_type,
        PmtAliasMap::value_type, bool >
    {
      public:
        result_type operator() (const first_argument_type & lhs,
                                const second_argument_type & rhs) const
        {
            return lhs.first.find(rhs.first) == 0;
        }
    };

    // This method finds aliases based on a key
    void findAliases(const PmtKeyType & key,
                     PmtAliasMap::const_iterator & begin,
                     PmtAliasMap::const_iterator & end) const;

    void showKeyList(const PmtKeyList & keyList);

    // This is a private copy constructor
    inline PmtAliases(const PmtAliases &)
    {
    }
    // This is a private assignment operator
    inline PmtAliases & operator=(PmtAliases &)
    {
        return *this;
    }
};

#endif                          // PMT_ALIASES_H

