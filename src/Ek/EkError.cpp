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
 * Creation Date: 1996
 *
 * Original Author: 
 * John Hall
 * 
 * Contributor(s): 
 * Martin R. Hoffman martin.hoffman@kodak.com
 * Gregory Gray greg.gray@kodak.com
*/


#include "EkPrecompiled.h"

#include "EkCompiler.h"

#include <iostream>
#if defined (_MSC_VER) || ( defined (__GNUC__) && (__GNUC__ > 2))
#include <sstream>
#else
#include <strstream.h>
#endif

//#include <string>
#include <ctime>
#include "EkError.h"
#include "EkBool.h"
#include "EkLocking.h"

// This flag turns off using the date so that test scripts can compare equal.
// We put something in that has the same length so that formatting can be checked.
bool EkError::msUseDate = true;
static const char *EK_NO_DATE_STRING = "Day Mon No hh:mm:ss Year\n";

// This flag turns off the line numbers, so that slight modifications of files won't
// keep changing error messages.  Default is on.
bool EkError::msUseLines = true;


EkError::EkError(const char *component, int code, const char *fileName,
                 int line):mMyComponent(component), mMyCode(code),
mMyLine(line), mMyFileName(fileName),
#ifndef __DCC__
mMyTimestamp(time(0)),
#endif
mMyMessage(""), mInitialized(true), mUsageCount(0)
{
}


EkError::EkError():mMyComponent("EkError"), mMyCode(0), mMyLine(0),
mMyFileName(""),
#ifndef __DCC__
mMyTimestamp(time(0)),
#endif
    mMyMessage("Uninitialized EkError object."), mInitialized(false),
mUsageCount(0)
{
}


EkError::EkError(const EkError & e):mMyComponent(e.mMyComponent),
mMyCode(e.mMyCode), mMyLine(e.mMyLine), mMyFileName(e.mMyFileName),
mMyTimestamp(e.mMyTimestamp), mMyMessage(e.mMyMessage),
mInitialized(e.mInitialized), mUsageCount(0)
{
}


EkError::~EkError()throw()
{
}


EkError & EkError::operator=(const EkError & rhs)
{
    // Check for assignment to self
    if (this == &rhs)
        return *this;

    // Copy fields
    mMyCode = rhs.mMyCode;
    mMyLine = rhs.mMyLine;
    mMyTimestamp = rhs.mMyTimestamp;
    mMyComponent = rhs.mMyComponent;
    mMyFileName = rhs.mMyFileName;
    mMyMessage = rhs.mMyMessage;
    mInitialized = rhs.mInitialized;

    return *this;
}


bool EkError::isA(const char *component, int errcode) const
{

    // Make sure component argument isn't nil
    if (component)
        // compare component and error code
        return (bool) ((SPmtString(component) == mMyComponent)
                       && (errcode == mMyCode));
    else
        return false;
}


const SPmtString & EkError::component() const
{
    return mMyComponent;
}


const int &EkError::errcode() const
{
    return mMyCode;
}


const int &EkError::line() const
{
    return mMyLine;
}


const SPmtString & EkError::filename() const
{
    return mMyFileName;
}


const time_t & EkError::timestamp() const
{
    return mMyTimestamp;
}


bool EkError::isInitialized() const
{
    return mInitialized;
}


EkError & EkError::setMsg(const char *m)
{
    mMyMessage = m;

    return *this;
}


EkError & EkError::setMsg(const SPmtString & m)
{
    mMyMessage = m;

    return *this;
}


/*********************************
  Description: Format and return a SPmtString that describes the error.
    This function exists to override the virtual function in std::exception
    It simply calls getMsg() below.

  Note: The SPmtString that this function's return value points to
        lives only until this EkError object is deleted.
  *********************************/
const char *EkError::what() const throw()
{
    return getMsg();
}


/*********************************
  Function Name: getMsg()
  Description: Format and return a SPmtString that describes the error

  Note: The SPmtString that this function's return value points to
        lives only until this EkError object is deleted.
  *********************************/
const char *EkError::getMsg() const
{
    static EK_DEFAULT_MUTEX sLock;

    if (mMyMessage.length() == 0)
    {
        // Convert the timestamp to text
        SPmtString stamp(msUseDate ? ctime(&mMyTimestamp) : EK_NO_DATE_STRING);
        stamp = stamp.substr(0, 24);    // trim newline

        // Build the message in an ostringstream
#if defined (_MSC_VER) || ( defined (__GNUC__) && (__GNUC__ > 2))
        ostringstream strout;
#else
        char c[1000];           // If we use a dynamic ostrstream, the memory is never
        // freed, once str() is called below.  This array is on
        // the stack rather than static to preserve thread-safety.
        ostrstream strout(c, sizeof(c));
#endif                          // _MSC_VER

        strout << "EkError - " << mMyComponent << ':' << mMyCode << ", ";
        if (msUseLines)
        {
            strout << mMyFileName << " line " << mMyLine;
        }
        else
        {
            strout << "FILE line XXX";
        }
        strout << "  " << stamp << ends;

        // Assign value of ostringstream to myMessage, casting
        // away constness.  getMsg() is conceptually const, but
        // not bitwise const.
        EK_GUARD(EK_DEFAULT_MUTEX, sLock);

        SPmtString *nonConstMyMessagePtr =
            const_cast < SPmtString * >(&mMyMessage);
        *nonConstMyMessagePtr = strout.str();
    }

    // Return const SPmtString value of myMessage
    return mMyMessage.c_str();
}


/*********************************
  Description: Write textual error message to an ostream
  *********************************/
void EkError::print(ostream & os) const
{
    os << getMsg();
}


/*********************************
  Description: default destructor
  *********************************/
EkErrp::EkErrp():mErrorPtr(0)
{
}


/*********************************
  Description: copy constructor
  *********************************/
EkErrp::EkErrp(const EkErrp & e):mErrorPtr(e.mErrorPtr)
{
    if (mErrorPtr)
        mErrorPtr->mUsageCount++;
}


/*********************************
  Description: construct from pointer to EkError
  *********************************/
EkErrp::EkErrp(EkError * e):mErrorPtr(e)
{
    if (mErrorPtr)
        mErrorPtr->mUsageCount++;
}


/*********************************
  Description: construct from reference to EkError
  *********************************/
EkErrp::EkErrp(EkError & e):mErrorPtr(&e)
{
    mErrorPtr->mUsageCount++;
}


/*********************************
  Description: destructor
  *********************************/
EkErrp::~EkErrp()
{
    // Decrememnt EkError's usage count and delete if zero
    if (mErrorPtr && (--(mErrorPtr->mUsageCount) == 0))
        delete mErrorPtr;
}


/*********************************
  Description: assignment operator
  *********************************/
EkErrp & EkErrp::operator=(const EkErrp & e)
{
    *this = e.mErrorPtr;        // use operator=(EkError*)
    return *this;
}


/*********************************
  Description: assignment operator - from a pointer to EkError
  *********************************/
EkErrp & EkErrp::operator=(EkError * newp)  // assignment operator, EkError ptr
{
    if (mErrorPtr != newp)      // I'm not already pointing to this, am I?
    {
        // Decrement previous EkError's usage count and delete if zero
        if (mErrorPtr && (--(mErrorPtr->mUsageCount) == 0))
        {
            EkError *oldp = mErrorPtr;
            mErrorPtr = 0;      // Just in case delete below throws an exception
            delete oldp;
        }

        // Assign pointer and increment new EkError's usage count
        mErrorPtr = newp;
        if (mErrorPtr)
            mErrorPtr->mUsageCount++;
    }

    return *this;
}


/*********************************
  Description: Dereference
  *********************************/
EkError *EkErrp::operator->() const
{
    // It's an error if p is nil, but what to do?
    return mErrorPtr;
}


/*********************************
  Description: convert to EkError*
  
  Note: May return 0
  *********************************/
EkErrp::operator    EkError * ()
const
{
    return mErrorPtr;
}


/*********************************
  Description: Check for nil pointer
  *********************************/
bool EkErrp::isNil() const
{
    return (bool) (mErrorPtr == 0);
}


/*********************************
  Description: accessor function - returns pointer to EkError or 0
  *********************************/
EkError *EkErrp::getp() const
{
    return mErrorPtr;
}

