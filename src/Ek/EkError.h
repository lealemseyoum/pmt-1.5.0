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
 * Creation Date: Unknown
 *
 * Original Author: 
 * John Hall
 * 
 * Contributor(s): 
*/

#ifndef EK_ERROR_H
#define EK_ERROR_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#ifndef EK_COMPILER_H
#include "EkCompiler.h"         // various #define's for compiler dependencies
#endif

#ifndef EKBOOL_H
#include "EkBool.h"
#endif

#include <iostream>
#include <ctime>
//#include <SPmtString>
#include <exception>

// Shorthand macro to be used in error object constructor calls to
// fill in file name and line where error occurred
#define WHERE __FILE__,__LINE__

//! Base class for error objects
/*! Specialized error object classes may be derived from this base class.
Such classes would add data members to hold domain-specific error
information, accessor functions to retrieve such data, and their own
getMsg() function to produce a comprehensible human-readable message
that describes the error.

This class may also be used by itself to create minimal, unspecialized
error objects.

EkError objects characterize errors with the following information:

  component - a SPmtString that names the software component that
              created the error.
  code      - a numeric error code.  Software components will
              probably create an enum to hold their various
              error codes.
  line      - source file line number where the error object
              was created.
  filename  - name of source file
  timestamp - time and date when error obejct was created

The component, code, line and fileName are all parameters to the
EkError constructor.  The WHERE macro provides a convenient shorthand
for providing the the line and filename values to the constructor

The getMsg() function returns a text SPmtString that describes the error.
getMsg() is a virtual function, and it is expected that derived
classes will redefine it to produce appropriate descriptions for the
errors they encompass.  The default implementation of getMsg() is
pretty simpleminded.  This virtual function allows general purpose
error logging and reporting code to be written that handle just 
EkErrors or EkErrps without needing to know the actual derived classes
of the errors.

The default constructor allows uninitialized error obejcts to be created.
These are identifiable byuse of the "isInitialized()" method.  Debug
versions will catch attempted use of uninitialized error objects by
failure of "assert()" calls.  If the "assert" mechanism is defeated,
as in release code, uninitialized error objects may appear in error logs
and displays.  They are characterized by component code "EkError" and
error code 0.

Things to consider when creating a derived error class:

- Make filename and line the last two parameters of the constructor,
  Default them to "<File name unknown>" and 0.

- Create a getMsg() function that creates a SPmtString in myMessage that 
  describes the error.

- If your class adds data members that are pointers, make sure to
  provide a copy constructor, an operator=, and a destructor that
  do the right thing.



Example:  (boring bits elided by ellipses: "...")

 OrderError - a derived error class containing an order number
\code
    class OrderError : public EkError
    {
    public:

        OrderError(const char* component, int errcode, OrderId order, 
                   const char* filename = "<file name unknown>", int line = 0);

        OrderId getOrderId();

        // produces OrderError-specific message
        virtual const char* getMsg(); 

        static const char* const className;

     protected:
         OrderId myOrderId;
     } // end class OrderError

     // Static initialization
     const char* const OrderError::classname = "OrderError";

     // Constructor
     OrderError::OrderError(const char* component, int errcode, OrderId order,
                            const char* filename, int line)
     : EkError(component, errcode, filename, line), myOrderId(order)
     {
         myClassName = OrderError::className;
     }

    // someFunction - returns an EkErrp that may point to one
    //                of several kinds of errors
    EkErrp someFunction(...)
    {
        ...
        if(...error discovered...)
        {
            EkErrp ep(new OrderError(ORDER_ENTRY, BAD_COUNT, OrderNo, WHERE));
            return ep;
        }
        ...
    }

    // Call someFunction and handle returned errors
    EkErrp ep = someFunction();
    if(ep)
    {
        // log and display message
        errorLog.write(ep);
        statusLine.displayMessage(ep->getMessage());

        // Cancel order if message type was OrderError 
        if(ep->getClassName() == OrderError::className)
            cancelOrder(((OrderError*)ep)->getOrderId);
     }
\endcode
*/

class EK_DECL EkError:public exception
{
    //smart pointer to an EkError
    friend class EkErrp;

  public:
    //: This static variable is set to true by default.
    static bool msUseDate;
    // If set to false, the date/time is not filled in, so that test scripts
    // can be automatically compared.

    //: This static variable is set to true by default.
    static bool msUseLines;
    // If set to false, the filename and line number are not filled in, so that test scripts
    // are not sensitive to minor changes in formatting in files or location of files
    // in the directory structure.

    //: This is a constructor
     EkError(const char *component, int errcode,
             const char *filename = "<file name unknown>", int line = 0);
    //: This is the default constructor
     EkError();
    //: This is the copy constructor
     EkError(const EkError &);

    //: This is the destructor
     virtual ~ EkError() throw();

    //: This is the assignment operator
     EkError & operator=(const EkError &);

    //: This is a relational operator needed by STL
    inline bool operator<(const EkError & rhs) const
    {
        return this < &rhs;
    }
    //: This is a relational operator needed by STL
    inline bool operator==(const EkError & rhs) const
    {
        return this == &rhs;
    }

    //: This tests for a particular error type 
    bool isA(const char *component, int errcode) const;

    //: Get the error component
    const SPmtString & component() const;
    //: Return the error code
    const int &errcode() const;
    //: Return the error line
    const int &line() const;
    //: Return the filename
    const SPmtString & filename() const;
    //: Return the error timestamp
    const time_t & timestamp() const;
    //: Checks for initialization
    bool isInitialized() const;

    //: Define a custom error message text
     EkError & setMsg(const SPmtString &);
    //: Define a custom error message text
     EkError & setMsg(const char *);

    //: Format and return a textual error msg 
    virtual const char *getMsg() const;

    //: Inherited from std::exception, it just calls getMsg() above
    virtual const char *what() const throw();

    //: Performs a print
    /* For some reason, if we export an ostream operator <<, it does not
     * show up in the DLL's export table.  So, we implement the logic in
     * this function and then call it from an inlined ostream operator.
     * Note that this function cannot return the stream, because that seems
     * to be what triggers the omission from the export table.
     */
    void print(ostream & os) const;

  protected:
     SPmtString mMyComponent;   // component
    int mMyCode;                // error code
    int mMyLine;                // line number
    SPmtString mMyFileName;     // file name
    time_t mMyTimestamp;        // time and date
    SPmtString mMyMessage;      // generated error message
    bool mInitialized;          // this EkError object has been initialized.

  private:
    //: This variable shows how many EkErrps point to this EkError
    unsigned mUsageCount;
};

//: Write error message (from getMsg) to an ostream
inline ostream & operator <<(ostream & os, const EkError & e)
{
    e.print(os);
    return os;
}

//! Smart pointer class for EkError

/*! With many different error classes based on EkError, it is tempting
  to use EkError* pointers as function return values and thrown
  exception values.  A problem arises as to where the objects that
  such pointers refer to will be stored.  If a function creates an
  error object in local memory, i.e. on the stack, it will no longer
  be valid after the function returns or after a thrown exception
  unrolls the stack.  If the error object is in global (static)
  memory, then a subsequent call to the same function may overwrite a
  previous error.  If the error objects are allocated on the heap, by
  "new", then somebody has to delete them.  Failure to delete them
  results in memory leaks.  Attempting to delete them more than once
  may lead to heap management problems.  The goto-like behavior of
  exceptions can bypass "delete" statements in all but very carefully
  crafted code.
 
  The smart pointer class, EkErrp, provides a "handle" to "carry
  around" EkError objects and objects of its derived types without
  knowing the exact type.  An EkErrp can be copied, thrown as an
  exception or returned as the value of a function without loss of
  information.

  EkErrp is a reference-counted smart pointer.  When the last EkErrp
  that points to an error object is destroyed (either explicitly or by
  going out of scope) the error object is deleted.
*/

class EK_DECL EkErrp
{
  public:
    //: This is the default constructor
    EkErrp();
    //: This is the copy constructor
    EkErrp(const EkErrp &);
    //: This is a constructor from a ptr to EkError
    EkErrp(EkError *);
    //: This is a constructor from a reference to EkError
    EkErrp(EkError &);
    //: This is the destructor
    ~EkErrp();

    //: This is the assignment operator
    EkErrp & operator=(const EkErrp &);
    //: This is another assignment operator
     EkErrp & operator=(EkError *);

    //: This is a dereference method
    EkError *operator->() const;
    //: This converts to EkError*
    operator    EkError *() const;

    //: This tests for a Nil pointer
    bool isNil() const;

    //: This returns the EkError*
    EkError *getp() const;

  private:
    //: This is a pointer to an error based on EkError
     EkError * mErrorPtr;
};

//: Write error message (from getMsg) to an ostream
inline ostream & operator <<(ostream & os, const EkErrp & e)
{
    if (!e.isNil())
        e->print(os);

    return os;
}

#endif

