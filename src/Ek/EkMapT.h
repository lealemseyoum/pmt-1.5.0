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
 * Creation Date: June, 1999
 *
 * Original Author: 
 * George Sotak  george.sotak@kodak.com
 * 
 * Contributor(s): 
*/

#ifndef EK_MAP_T_H
#define EK_MAP_T_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif


#include <algorithm>
#include <functional>
#include <utility>
#include <list>

//! EkMapT is an implementation of the STL map container that can cross DLL boundaries.
/*! The STL map container is not able to cross DLL boundaries. The EkMapT class bypasses
  this problem by providing an implementation equivalent to the STL map container, but
  is implemented with the STL list container. This is intended to only be used when 
  building DLLs. To assist in this, the EkCompiler.h header contains the macro
  EK_DLL_MAP( k, v ). This macro will automatically switch between the STL map
  and EkMapT depending on build flags.
*/
/* Issues: 
 * 1. Since value_type is mPair<K, V> instead of mPair<const K, V>
 *    nothing can prevent the key from being changed via iterator.
 */

template < class K, class V, class Pred = less < K >, class A =
    allocator < V > >class EkMapT
{
    friend bool operator==(const EkMapT < K, V, Pred, A > &, const EkMapT < K,
                           V, Pred, A > &);
    friend bool operator<(const EkMapT < K, V, Pred, A > &, const EkMapT < K,
                          V, Pred, A > &);

  public:

    typedef K key_type;
    typedef V data_type;
    typedef Pred key_compare;
    typedef A allocator_type;
    typedef EK_TYPENAME pair < K, V > value_type;
    typedef V & reference;
    typedef const V & const_reference;
    typedef EK_TYPENAME list < value_type >::iterator iterator;
    typedef EK_TYPENAME list < value_type >::const_iterator const_iterator;
    typedef EK_TYPENAME list < value_type >::reverse_iterator reverse_iterator;
    typedef EK_TYPENAME list < value_type >::const_reverse_iterator const_reverse_iterator;
    typedef EK_TYPENAME pair < iterator, bool > _Pairib;
    typedef EK_TYPENAME list < value_type >::size_type size_type;

    //: This is a constructor
    inline EkMapT(const Pred & comp = Pred(), const A & al = A())
    {
        key_cmp = comp;
    }
    //: This is another constructor
    inline EkMapT(const EkMapT & x)
    {
        key_cmp = x.key_cmp;
        *this = x;
    }
    //: This is yet another constructor
    inline EkMapT(const value_type * first, const value_type * last,
                  const Pred & comp = Pred(), const A & al = A())
    {
        key_cmp = comp;
        insert(first, last);
    }

    //: This is the destructor
    inline ~ EkMapT()
    {
    }

    //: This is an assignment operator
    inline EkMapT < K, V, Pred, A > &operator=(const EkMapT < K, V, Pred,
                                               A > &fromMap)
    {
        vMap = fromMap.vMap;
        return *this;
    }

    //: This is a vector operator
    inline data_type & operator[] (const K & key)
    {
        _Pairib ib = insert(value_type(key, V()));
        return (*(ib.first)).second;
    }

    //: This class is a binary compare function
    class value_compare:public binary_function < value_type, value_type, bool >
    {
        friend class EkMapT < K, V, Pred, A >;
      public:
         inline bool operator() (const value_type & x, const value_type & y) const
        {
            return (comp(x.first, y.first));
        }
      protected:
         Pred comp;
        inline value_compare(Pred pred):comp(pred)
        {
        }
    };

    //: This returns the beginning of the iterator
    inline iterator begin()
    {
        return vMap.begin();
    }

    //: This also returns the beginning of the iterator
    inline const_iterator begin() const
    {
        return vMap.begin();
    }

    //: This returns the end of the iterator
    inline iterator end()
    {
        return vMap.end();
    }

    //: This also returns the end of the iterator
    inline const_iterator end() const
    {
        return vMap.end();
    }

    //: This returns the beginning of the reverse iterator
    inline reverse_iterator rbegin()
    {
        return vMap.rbegin();
    }
    //: This also returns the beginning of the reverse iterator
    inline const_reverse_iterator rbegin() const
    {
        return vMap.rbegin();
    }

    //: This returns the end of the reverse iterator
    inline reverse_iterator rend()
    {
        return vMap.rend();
    }
    //: This also returns the end of the reverse iterator
    inline const_reverse_iterator rend() const
    {
        return vMap.rend();
    }

    //: This clears the iterator
    inline void clear()
    {
        vMap.clear();
    }

    //: This returns whether or not the element associated with the key is a composite
    inline size_type count(const K & key)
    {
        size_type s = 0;
        const_iterator i = find(key);
        if (i != end())
            s = 1;

        return s;
    }

    //: This is an equal_range method
    inline pair < iterator, iterator > equal_range(const K & key)
    {
        return pair < iterator, iterator > (lower_bound(key),
                                            upper_bound(key));
    }

    //: This is also an equal range method
    inline pair < const_iterator, const_iterator > equal_range(const K & key) const
    {
        return pair < const_iterator, const_iterator > (lower_bound(key),
                                                        upper_bound(key));
    }

    //: This erases the iterator
    inline iterator erase(iterator it)
    {
        return vMap.erase(it);
    }

    //: This also erases the iterator
    inline iterator erase(iterator first, iterator last)
    {
        return vMap.erase(first, last);
    }

    //: This returns true or false depending on whether the iterator was erased or not
    inline size_type erase(const K & key)
    {
        iterator first, last;
        first = lower_bound(key);
        last = upper_bound(key);

        size_type s = 0;
        if (first != last)
        {
            s = 1;
            vMap.erase(first, last);
        }

        return s;
    }

    //: This finds an element based on the key
    inline iterator find(const K & key)
    {
        return find_if(begin(), end(), bind2nd(equalKey(), key));
    }

    //: This also finds an element based on a key
    inline const_iterator find(const K & key) const
    {
        return find_if(begin(), end(), bind2nd(equalKey(), key));
    }

    //: This checks to see if the iterator is empty
    inline bool empty()
    {
        return vMap.empty();
    }

    //: This inserts a value into the map
    inline _Pairib insert(const value_type & vt)
    {
        iterator s = upper_bound(vt.first);
        iterator testIt = s;
        if ((s != begin()) && ((*(--testIt)).first == vt.first))
            return _Pairib(testIt, false);
        else
        {
            s = vMap.insert(s, vt);
            return _Pairib(s, true);
        }
    }

    //: This also inserts a value into the map
    inline iterator insert(iterator it, const value_type & vt)
    {
        _Pairib ib = insert(vt);
        return ib.first;
    }

    //: This also inserts a value into the map
    inline void insert(const value_type * first, const value_type * last)
    {
        const value_type *p = first;
        while (!p && p != last)
        {
            insert(*p);
            p++;
        }
    }

    //: This returns the key compare value
    inline key_compare key_comp() const
    {
        return key_cmp;
    }

    //: This returns the max size of the iterator
    inline size_type max_size() const
    {
        return vMap.max_size();
    }

    //: This returns the lower bound of the iterator based on the key
    inline iterator lower_bound(const K & key)
    {
#ifdef _MSC_VER
        return std::lower_bound(begin(), end(), pair<K,V>(key,V()) , KeyCmp2());
#else
        return lower_bound(begin(), end(), pair<K,V>(key,V()), KeyCmp2());
#endif
    }

    //: This returns the lower bound of the iterator based on the key
    inline const_iterator lower_bound(const K & key) const
    {
#ifdef _MSC_VER
        return std::lower_bound(begin(), end(), pair<K,V>(key,V()), KeyCmp2());
#else
        return lower_bound(begin(), end(), pair<K,V>(key,V()), KeyCmp2());
#endif
    }

    //: This returns the size of the iterator
    inline size_type size() const
    {
        return vMap.size();
    }

    //: This swaps iterators
    inline void swap(EkMapT & smap)
    {
        vMap.swap(smap.vMap);
    }

    //: This returns the upper bonud of the iterator based on the key
    inline iterator upper_bound(const K & key)
    {
#ifdef _MSC_VER
        return std::upper_bound(begin(), end(), pair<K,V>(key,V()), KeyCmp());
#else
        return upper_bound(begin(), end(), pair<K,V>(key,V()), KeyCmp());
#endif
    }

    //: This returns the upper bound of the iterator based on the key
    inline const_iterator upper_bound(const K & key) const
    {
#ifdef _MSC_VER
        return std::upper_bound(begin(), end(), pair<K,V>(key,V()), KeyCmp());
#else
        return upper_bound(begin(), end(), pair<K,V>(key,V()), KeyCmp());
#endif
    }

    //: This returns the value of the key compare
    inline value_compare value_comp() const
    {
        return value_compare(key_comp());
    }

  protected:
     list < value_type > vMap;
    key_compare key_cmp;

    struct equalKey:public binary_function < value_type, key_type, bool >
    {
        result_type operator() (const first_argument_type & x,
                                const second_argument_type & y) const
        {
            return (x.first == y);
        }
    };
    struct KeyCmp:public binary_function < value_type, value_type, bool >
    {
        result_type operator() (const first_argument_type & x,
                                const second_argument_type & y) const
        {
            return key_compare() (x.first, y.first);
        }
    };
    struct KeyCmp2:public binary_function < value_type, value_type, bool >
    {
        result_type operator() (const first_argument_type & x,
                                const second_argument_type & y) const
        {
            return key_compare() (x.first, y.first);
        }
    };
};

template < class K, class V, class Pred, class A > inline
    bool EK_DECL operator==(const EkMapT < K, V, Pred, A > &lmap,
                            const EkMapT < K, V, Pred, A > &rmap)
{
    return lmap.vMap == rmap.vMap;
}


template < class K, class V, class Pred, class A > inline
    bool EK_DECL operator<(const EkMapT < K, V, Pred, A > &lmap,
                           const EkMapT < K, V, Pred, A > &rmap)
{
    return lmap.vMap < rmap.vMap;
}


//! EkMultiMapT is an implementation of the STL multimap container that can cross DLL boundaries.
/*! The STL multimap container is not able to cross DLL
  boundaries. The EkMultiMapT class bypasses this problem by providing
  an implementation equivalent to the STL multimap container, but is
  implemented with the STL list container. This is intended to only be
  used when building DLLs. To assist in this, the EkCompiler.h header
  contains the macro EK_DLL_MULTIMAP( k, v ). This macro will automatically
  switch between the STL multimap and EkMultiMapT depending on build flags.
*/
/*
 * The MultiMap Implementation
 */
template < class K, class V, class Pred = less < K >, class A =
    allocator < V > >class EkMultiMapT
{
    //: This is the comparison operator
    friend bool operator==(const EkMultiMapT < K, V, Pred, A > &,
                           const EkMultiMapT < K, V, Pred, A > &);
    //: This is the less than operator
    friend bool operator<(const EkMultiMapT < K, V, Pred, A > &,
                          const EkMultiMapT < K, V, Pred, A > &);

  public:

    typedef K key_type;
    typedef V data_type;
    typedef Pred key_compare;
    typedef A allocator_type;
    typedef EK_TYPENAME pair < K, V > value_type;
    typedef V & reference;
    typedef const V & const_reference;
    typedef EK_TYPENAME list < value_type >::iterator iterator;
    typedef EK_TYPENAME list < value_type >::const_iterator const_iterator;
    typedef EK_TYPENAME list < value_type >::reverse_iterator reverse_iterator;
    typedef EK_TYPENAME list < value_type >::const_reverse_iterator const_reverse_iterator;
    typedef EK_TYPENAME pair < iterator, bool > _Pairib;
    typedef EK_TYPENAME list < value_type >::size_type size_type;

    class value_compare:public binary_function < value_type, value_type, bool >
    {
        friend class EkMultiMapT < K, V, Pred, A >;
      public:
         bool operator() (const value_type & x, const value_type & y) const
        {
            return (comp(x.first, y.first));
        }
      protected:
         inline value_compare(Pred pred):comp(pred)
        {
        }
        Pred comp;
    };

  public:
    //: This is a constructor
    inline EkMultiMapT(const Pred & comp = Pred(), const A & al = A())
    {
        key_cmp = comp;
    }
    //: This is a constructor
    inline EkMultiMapT(const EkMultiMapT & x)
    {
        key_cmp = x.key_cmp;
        *this = x;
    }
    //: This is a constructor
    inline EkMultiMapT(const value_type * first, const value_type * last,
                       const Pred & comp = Pred(), const A & al = A())
    {
        key_cmp = comp;
        insert(first, last);
    }

    //: This is a destructor
    inline ~ EkMultiMapT()
    {
    }

    //: This is an assignment operator
    inline EkMultiMapT < K, V, Pred, A > &operator=(const EkMultiMapT < K, V,
                                                    Pred, A > &fromMap)
    {
        vMap = fromMap.vMap;
        return *this;
    }

    //: This is a vector operator
    inline data_type & operator[] (const K & key)
    {
        iterator ib = insert(value_type(key, V()));
        return (*ib).second;
    }

    //: This returns the beginning of the iterator
    inline iterator begin()
    {
        return vMap.begin();
    }

    //: This also returns the beginning of the iterator
    inline const_iterator begin() const
    {
        return vMap.begin();
    }

    //: This returns the end of the iterator
    inline iterator end()
    {
        return vMap.end();
    }

    //: This also returns the end of the iterator
    inline const_iterator end() const
    {
        return vMap.end();
    }

    //: This returns the beginning of the reverse iterator
    inline reverse_iterator rbegin()
    {
        return vMap.rbegin();
    }
    //: This also returns the beginning of the reverse iterator
    inline const_reverse_iterator rbegin() const
    {
        return vMap.rbegin();
    }

    //: This returns the end of the reverse iterator
    inline reverse_iterator rend()
    {
        return vMap.rend();
    }
    //: This also returns the end of the reverse iterator
    inline const_reverse_iterator rend() const
    {
        return vMap.rend();
    }

    //: This clears the iterator
    inline void clear()
    {
        vMap.clear();
    }

    //: This returns whether or not the element is a composite
    inline size_type count(const K & key)
    {
        size_type s = 0;
        const_iterator i = find(key);
        if (i != end())
            s = 1;

        return s;
    }

    //: This is an equal range operator
    inline pair < iterator, iterator > equal_range(const K & key)
    {
        return pair < iterator, iterator > (lower_bound(key),
                                            upper_bound(key));
    }

    //: This is an equal range operator
    inline pair < const_iterator, const_iterator > equal_range(const K & key) const
    {
        return pair < const_iterator, const_iterator > (lower_bound(key),
                                                        upper_bound(key));
    }

    //: This erases the iterator
    inline iterator erase(iterator it)
    {
        return vMap.erase(it);
    }

    //: This also erases the iterator
    inline iterator erase(iterator first, iterator last)
    {
        return vMap.erase(first, last);
    }

    //: This returns true or false based on whether or not the iterator was erased
    inline size_type erase(const K & key)
    {
        iterator first, last;
        first = lower_bound(key);
        last = upper_bound(key);

        size_type s = 0;
        if (first != last)
        {
            s = 1;
            vMap.erase(first, last);
        }

        return s;
    }

    //: This finds an element based on the key
    inline iterator find(const K & key)
    {
        return find_if(begin(), end(), bind2nd(equalKey(), key));
    }

    //: This finds an element based on the key
    inline const_iterator find(const K & key) const
    {
        return find_if(begin(), end(), bind2nd(equalKey(), key));
    }

    //: This returns whether or not the iterator is empty
    inline bool empty()
    {
        return vMap.empty();
    }

    //: This inserts a value into the map
    inline iterator insert(const value_type & vt)
    {
        return vMap.insert(upper_bound(vt.first), vt);
    }

    // :This inserts a value into the map
    inline iterator insert(iterator it, const value_type & vt)
    {
        return insert(vt);
    }

    //: This inserts elements between the first & last into the map
    inline void insert(const value_type * first, const value_type * last)
    {
        const value_type *p = first;
        while (!p && p != last)
        {
            insert(*p);
            p++;
        }
    }

    //: This returns the value of key compare
    inline key_compare key_comp() const
    {
        return key_cmp;
    }

    //: This returns the max size of the iterator
    inline size_type max_size() const
    {
        return vMap.max_size();
    }

    //: This returns the lower bound of the iterator based on a key
    inline iterator lower_bound(const K & key)
    {
#ifdef _MSC_VER
		return std::lower_bound(begin(), end(), pair<K,V>(key,V()), KeyCmp2());
#else
		return lower_bound(begin(), end(), pair<K,V>(key,V()), KeyCmp2());
#endif		
    }

    //: This returns the lower bound of the iterator based on a key
    inline const_iterator lower_bound(const K & key) const
    {
#ifdef _MSC_VER
		return std::lower_bound(begin(), end(), pair<K,V>(key,V()), KeyCmp2());
#else
		return lower_bound(begin(), end(), pair<K,V>(key,V()), KeyCmp2());
#endif	
	}

    //: This returns the size of the iterator
    inline size_type size() const
    {
        return vMap.size();
    }

    //: This swaps iterators
    inline void swap(EkMultiMapT & smap)
    {
        vMap.swap(smap.vMap);
    }

    //: This returns the upper bound of an iterator based on the key
    inline iterator upper_bound(const K & key)
    {
#ifdef _MSC_VER
		return std::upper_bound(begin(), end(), pair<K,V>(key,V()), KeyCmp());
#else
		return upper_bound(begin(), end(), pair<K,V>(key,V()), KeyCmp());
#endif
    }

    //: This returns the upper bound of an iterator based on the key
    inline const_iterator upper_bound(const K & key) const
    {
#ifdef _MSC_VER
		return std::upper_bound(begin(), end(), pair<K,V>(key,V()), KeyCmp());
#else
		return upper_bound(begin(), end(), pair<K,V>(key,V()), KeyCmp());
#endif
    }

    //: This returns the value of the key compare
    inline value_compare value_comp() const
    {
        return value_compare(key_comp());
    }

  protected:
     list < value_type > vMap;
    key_compare key_cmp;

    struct equalKey:public binary_function < value_type, key_type, bool >
    {
        result_type operator() (const first_argument_type & x,
                                const second_argument_type & y) const
        {
            return (x.first == y);
        }
    };
    struct KeyCmp:public binary_function < value_type, value_type, bool >
    {
        result_type operator() (const first_argument_type & x,
                                const second_argument_type & y) const
        {
            return key_compare() (x.first, y.first);
        }
    };
    struct KeyCmp2:public binary_function < value_type, value_type, bool >
    {
        result_type operator() (const first_argument_type & x,
                                const second_argument_type & y) const
        {
            return key_compare() (x.first, y.first);
        }
    };

  private:

};

template < class K, class V, class Pred, class A > inline
    bool EK_DECL operator==(const EkMultiMapT < K, V, Pred, A > &lmap,
                            const EkMultiMapT < K, V, Pred, A > &rmap)
{
    return lmap.vMap == rmap.vMap;
}


template < class K, class V, class Pred, class A > inline
    bool EK_DECL operator<(const EkMultiMapT < K, V, Pred, A > &lmap,
                           const EkMultiMapT < K, V, Pred, A > &rmap)
{
    return lmap.vMap < rmap.vMap;
}

#endif                          // EK_MAP_T_H

