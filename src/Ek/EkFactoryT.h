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
 * Creation Date: 01/1999
 *
 * Original Author: 
 * George Sotak george.sotak@kodak.com
 * 
 * Contributor(s): 
*/



#ifndef EK_FACTORY_T_H
#define EK_FACTORY_T_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#if (defined _MSC_VER) && (!defined _WINDOWS_) && (!defined __AFXWIN_H__)
#include "windows.h"
#endif


#include "EkCompiler.h"
#include "EkGPtr.h"
#include "EkLocking.h"


// The EK_REQUIRE and EK_PROVIDE macros can be described somewhat as "hacks"
// to force the loading of the factory before main() is called.  These try to
// trick the compiler/loader into running a "loading" function (the "init" 
// function passed into EK_PROVIDE).  EK_REQUIRE attempts to force the loader 
// into running the function by requiring a variable that depends on it 
// in some bogus function.  As compilers get smarter at optimization,
// they realize that the function is bogus, and new methods of "tricking" them 
// must be developed.  Perhaps a better way should be found.

// If you run accross a situation where it appears the factories have not
// been loaded, then this is the place to look.  


#if defined( _MSC_VER )

#ifndef EK_REQUIRE_EXPORT_DLL
#define EK_REQUIRE_EXPORT_DLL(name)              \
__declspec(dllexport) const bool name##_FILE;    \
static const bool *PULL_##name = &name##_FILE;   \
static void FUNC_##name(void)                    \
{                                                \
    const bool *b = PULL_##name;                 \
    b++;                                         \
    FUNC_##name();                               \
}
#endif


#ifndef EK_REQUIRE_IMPORT_DLL
#define EK_REQUIRE_IMPORT_DLL(name)                      \
extern  __declspec(dllimport) const bool name##_FILE;    \
static const bool *PULL_##name = &name##_FILE;           \
static void FUNC_##name(void)                            \
{                                                        \
    const bool *b = PULL_##name;                         \
    b++;                                                 \
    FUNC_##name();                                       \
}
#endif

#endif                          // _MSC_VER


#ifndef EK_PROVIDE
#define EK_PROVIDE(name, init) const bool name##_FILE = (bool)init;
#endif

#ifndef EK_REQUIRE
#define EK_REQUIRE(name)                       \
extern const bool name##_FILE;                 \
static const bool * FUNC_##name(void)          \
{                                              \
    const bool *b = &name##_FILE;              \
    return b;                                  \
}                                              
//static const bool *PULL_##name = FUNC_##name();
#endif

//! The EkFactoryT class provides the generic functionality for an object factory. 
/*! In this case, the factory comprises object prototypes and their
 associated factory keys. The factory requires the clone() method to
 be present on the interface of the class whose specializations can be
 created through the factory. The EkFactoryT class is templated with
 two parameters: the factory key type, and the base class pointer
 type.

 The EkFactoryT class can be used in one of two ways. First, create
 an instance of EkFactoryT in the base class and provide static
 methods on the base class interface that use the factory for
 adding, deleting, and creating derived classes, e.g.,
 <pre>
   class Widget ;
   typedef EkSmartPtr<Widget> WidgetPtr ;
   typedef SPmtString WidgetKeyType ;
   typedef EkFactoryT<WidgetKeyType, WidgetPtr> WidgetFactoryType;
   class Widget : public EkRefCount<EK_DEFAULT_MUTEX>
   {
     public:

       Widget(const WidgetKeyType& theKey, const WidgetKeyType& theFactoryKey )
         :key(theKey), factoryKey(theFactoryKey)
       {}
       ~Widget(void) {}

       static WidgetPtr createFromFactory( const WidgetKeyType& widgetKey, const WidgetKeyType& factoryKey ) ;
       // Provide access to addEntry and removeEntry methods
       static WidgetFactoryType& getFactory( void ) { return factory ; }

       virtual WidgetPtr clone( void ) = 0 ;

       const WidgetKeyType& getKey( void ) const { return key ; }
       void setKey( WidgetKeyType& theKey ) { key = theKey; return; }
       const WidgetKeyType& getFactoryKey( void ) const { return factoryKey ; }
       void setFactoryKey( WidgetKeyType& facKey ) { factoryKey = facKey; return; }
     private:
       static WidgetFactoryType factory ;
       WidgetKeyType key ;
       WidgetKeyType factoryKey ;
   };

 Usage:
   WidgetPtr aWidget = Widget::createFromFactory( "myWidget", "WidgetA" ) ;


 Second, derive a specialized factory by inheriting from
 EkFactoryT, e.g.,

   class WidgetFactory : public EkFactoryT<WidgetKeyType, WidgetPtr>
   {
      public:
        WidgetFactory( void ) {}
        ~WidgetFactory( void ) {}
   };

 Usage:
   WidgetFactory widgetFactory ;
   WidgetPtr aWidget = widgetFactory.create( "WidgetA" ) ;
   aWidget->setKey( "myWidget" ) ;

 </pre>
 Prototype registration:
 <p>
 There are two methods by which prototypes can be added to the
 factory. First, add an initialization method to either the base
 class or specialized factory class that instantiates all the
 specializations and adds them to the factory. One draw back with
 this technique is that this method must be modified each time a new
 specialization is created. Another technique is to allow
 specializations to register themselves from their implementation
 file. The EK_PROVIDE macro provides this capability with the
 addition of a specialized macro in the base class header. For
 example, in the Widget base class header file, we define the
 following macro:
<p>
   #define WIDGET_TYPE(facKey, widgetType) Widget::getFactory().addEntry( facKey, new widgetType("", facKey) )
 <p>
 Then in the specialization's implementation file, in this case 
 WidgetA, we add,
 <p>
   EK_PROVIDE(WidgetA,  MMA_METADATA_TYPE("WidgetA", WidgetA))
 <p>
 to register the specialization, WidgetA, with the
 factory. Additional control over the availability of prototypes in
 the factory (and what specializations are linked into the
 application) is given to the application through the EK_REQUIRE
 macro. For example, an application may only need access to WidgetA,
 WidgetB, and WidgetC. In this case, the following would be added
 to the application code (just outside of main()):
 <pre>
   EK_REQUIRE(WidgetA)
   EK_REQUIRE(WidgetB)
   EK_REQUIRE(WidgetC)
 </pre>
 As a convienence to applications that require all instances, one
 may define an inplementation file that captures all instances, e.g.,
 <p>
 File AllWidget.cpp:
 <pre>
   EK_REQUIRE(WidgetA)
   EK_REQUIRE(WidgetB)
   EK_REQUIRE(WidgetC)
   EK_REQUIRE(WidgetD)
   EK_REQUIRE(WidgetE)

   EK_PROVIDE(AllWidgets, NULL )
 </pre>
 The those applications requiring access to all widgets would just
 need:
 <p>
 <pre>
   EK_REQUIRE(AllWidgets)
 </pre>
*/

template < class FacKeyType, class FacType > class EkFactoryT
{
  public:
    //! This is the default constructor
    inline EkFactoryT()
    {
    }
    //! This is the destructor
    inline ~ EkFactoryT()
    {
    }

    //! Return a copy of the prototype with the factory key facKey.
    inline FacType create(const FacKeyType & facKey);

    //! Returns original prototype with the factory key facKey.
    inline FacType get(const FacKeyType & facKey);

    //! adds a new prototype to the factory with factory key, facKey, by creating a copy of "item". 
    /*! Returns "true" if successful, "false" otherwise. */
    inline bool addEntry(const FacKeyType & facKey, const FacType item);

    //! remove the prototype from the factory with factory key, facKey.
    /*! returns "true" if successful, "false" otherwise. */
    inline bool removeEntry(const FacKeyType & facKey);

    //! This clears the entire factory.
    inline void clear();

  protected:
    typedef EK_DLL_MAP(FacKeyType, FacType) RegistryType;
    RegistryType mRegistry;
    EK_DEFAULT_MUTEX mRegistryLock;

  public:
    class read_only_iterator
    {
        friend class EkFactoryT < FacKeyType, FacType >;
      public:

        inline FacType start()
        {
            mCurrent = mRegistry.begin();
            return rval();
        }

        inline FacType next()
        {
            ++mCurrent;
            return rval();
        }

      private:
        RegistryType & mRegistry;
        EK_TYPENAME RegistryType::iterator mCurrent;

        inline FacType rval()
        {
            if (mCurrent != mRegistry.end())
                return (*mCurrent).second;
            return 0;
        }

      read_only_iterator(RegistryType & registry):mRegistry(registry)
        {
        }
    };

    inline read_only_iterator getIterator();

  private:
    // Needed for VC6
    friend class read_only_iterator;

    //! This is the copy constructor
    inline EkFactoryT(const EkFactoryT &)
    {
    }
    //! This is the assignment operator
    inline EkFactoryT & operator=(EkFactoryT &)
    {
        return *this;
    }
};


template < class FacKeyType, class FacType >
    inline FacType EkFactoryT < FacKeyType,
    FacType >::create(const FacKeyType & facKey)
{
    EK_GUARD(EK_DEFAULT_MUTEX, mRegistryLock);

    FacType item(static_cast < FacType > (NULL));

    EK_TYPENAME RegistryType::iterator e = mRegistry.find(facKey);

    if (e != mRegistry.end())
    {
        item = (*e).second->clone();
    }

    return item;
}


template < class FacKeyType, class FacType >
    inline FacType EkFactoryT < FacKeyType,
    FacType >::get(const FacKeyType & facKey)
{
    EK_GUARD(EK_DEFAULT_MUTEX, mRegistryLock);

    FacType item(static_cast < FacType > (NULL));

    EK_TYPENAME RegistryType::iterator e = mRegistry.find(facKey);

    if (e != mRegistry.end())
    {
        item = (*e).second;
    }

    return item;
}


template < class FacKeyType, class FacType >
    inline bool EkFactoryT < FacKeyType,
    FacType >::addEntry(const FacKeyType & facKey, const FacType item)
{
    EK_GUARD(EK_DEFAULT_MUTEX, mRegistryLock);

    pair < EK_TYPENAME RegistryType::iterator, bool > res;
    res = mRegistry.insert(EK_TYPENAME RegistryType::value_type(facKey, item));

    return res.second;
}


template < class FacKeyType, class FacType >
    inline bool EkFactoryT < FacKeyType,
    FacType >::removeEntry(const FacKeyType & facKey)
{
    EK_GUARD(EK_DEFAULT_MUTEX, mRegistryLock);

    if (mRegistry.erase(facKey) > 0)
        return true;
    else
        return false;
}

template < class FacKeyType, class FacType >
    EK_TYPENAME EkFactoryT < FacKeyType, FacType >::read_only_iterator
    EkFactoryT < FacKeyType, FacType >::getIterator()
{
    read_only_iterator r(mRegistry);
    return r;
}

template < class FacKeyType, class FacType >
    inline void EkFactoryT < FacKeyType, FacType >::clear()
{
    EK_GUARD(EK_DEFAULT_MUTEX, mRegistryLock);

    mRegistry.clear();
}

#endif                          /* EK_FACTORY_T_H */

