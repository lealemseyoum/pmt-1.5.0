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
 * Creation Date: December 1998
 *
 * Original Author: 
 * George Sotak george.sotak@kodak.com 
 * Copyright (C) 1998 Eastman Kodak Company
 * 
 * Contributor(s): 
 * Gregory Gray greg.gray@kodak.com
 */

#if !defined (EK_RATIONAL_H)
#define EK_RATIONAL_H

#if ((defined _MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif

#if (defined _MSC_VER) && (!defined _WINDOWS_) && (!defined __AFXWIN_H__)
#include "windows.h"
#endif

#include <cmath>
#include <typeinfo>
#include "EkCompiler.h"
#include "EkLocking.h"
#include "EkRefCount.h"
#include "EkSmartPtr.h"
#include "EkTemplate.h"

template < class Type > class EkRational;

typedef EkSmartPtr < EkRational < long > > EkRationalLongPtr;
typedef EkConstSmartPtr < EkRational < long > > EkConstRationalLongPtr;
typedef EkSmartPtr < EkRational < unsigned long > > EkRationalULongPtr;
typedef EkConstSmartPtr < EkRational < unsigned long > > EkConstRationalULongPtr;


//! The EkRational class provides a templated implementation of the rational number.
/*! A rational number is a fraction, p/q, where p and q are 
  integers, with p being the Numerator and q the Denominator. This class can 
  be constructed in several ways. First, using the constructor that takes two 
  intergers the numerator and denominator. Second, using the default 
  constructor and later supplying the numerator and denominator through class 
  methods. Lastly, using the default constructor and supplying a floating 
  point number to the rationalize() method that inturn calculates the integer 
  numerator and denominator to the given precision (default is 1E-10). Smart 
  pointers to the long and unsigned long versions of the EkRational class are 
  pre-defined in the header file.
*/

template < class Type > class EkRational:public EkRefCount < EK_DEFAULT_MUTEX >
{
  public:
    //! Default constructor
    inline EkRational(void):mNumerator(0), mDenominator(1),
        mValueChanged(false), mFpVal(0.0)
    {
    }

    //! constructor
    inline EkRational(const Type & numer,
                      const Type & denom):mNumerator(numer),
        mDenominator(denom), mValueChanged(false),
        mFpVal(static_cast < double >(numer) / static_cast < double >(denom))
    {
    }

    //! constructor
    inline EkRational(const EkRational < Type >
                      &theSrc):mNumerator(theSrc.mNumerator),
        mDenominator(theSrc.mDenominator), mValueChanged(theSrc.mValueChanged),
        mFpVal(theSrc.mFpVal)
    {
    }

    //! This is the destructor
    inline ~ EkRational(void)
    {
    }

    //! This is an operator equals
    inline EkRational < Type > &operator=(const EkRational < Type > &rhs);
    //! This is an operator +=
    inline EkRational < Type > &operator+=(const EkRational < Type > &rhs);
    //! This is an operator -=
    inline EkRational < Type > &operator-=(const EkRational < Type > &rhs);
    //! This is an operator *=
    inline EkRational < Type > &operator*=(const EkRational < Type > &rhs);
    //! This is an operator /=
    inline EkRational < Type > &operator/=(const EkRational < Type > &rhs);

    //! This is an operator ==
    inline bool operator==(const EkRational < Type > &rhs) const;
    //! This is an operator !=
    inline bool operator!=(const EkRational < Type > &rhs) const;
    //! This is an operator <
    inline bool operator<(const EkRational < Type > &rhs) const;
    //! This is an operator <=
    inline bool operator<=(const EkRational < Type > &rhs) const;
    //! This is an operator >
    inline bool operator>(const EkRational < Type > &rhs) const;
    //! This is an operator >=
    inline bool operator>=(const EkRational < Type > &rhs) const;

    //! This converts the rational to a double
    inline double asDouble(void) const;
    //! This prints to the stream
    inline ostream & print(ostream & os) const;
    //! This reads from the stream
    inline istream & read(istream & is);
    //! Convert a floating point number into a rational
    double rationalize(const double &mFpVal, const double error = 1.0e-10);

    //! Access the numerator of the rational
    inline const Type & numerator(void) const
    {
        return mNumerator;
    }

    //! Access the denominator of the rational
    inline const Type & denominator(void) const
    {
        return mDenominator;
    }

    //! This method sets the numerator
    inline void numerator(const Type & numer)
    {
        mNumerator = numer;
        mValueChanged = true;
        return;
    }

    //! This mehtod sets the denominator
    inline void denominator(const Type & denom)
    {
        mDenominator = denom;
        mValueChanged = true;
        return;
    }

  private:
    Type mNumerator;
    Type mDenominator;
    bool mValueChanged;
    mutable double mFpVal;

    //! This method calculates the Fp value
    inline void calcFpVal(void) const
    {
        if (mValueChanged)
            mFpVal =
                static_cast < double >(mNumerator) / static_cast <
            double >(mDenominator);
         return;
    }
};

template < class Type >
    inline EkRational < Type > &EkRational <
    Type >::operator=(const EkRational < Type > &rhs)
{
    mNumerator = rhs.mNumerator;
    mDenominator = rhs.mDenominator;
    mValueChanged = true;
    return *this;
}

template < class Type >
    inline EkRational < Type > &EkRational <
    Type >::operator+=(const EkRational < Type > &rhs)
{
    mNumerator = mNumerator * rhs.mDenominator + rhs.mNumerator * mDenominator;
    mDenominator = mDenominator * rhs.mDenominator;
    mValueChanged = true;
    return *this;
}

template < class Type >
    inline EkRational < Type > &EkRational <
    Type >::operator-=(const EkRational < Type > &rhs)
{
    mNumerator = mNumerator * rhs.mDenominator - rhs.mNumerator * mDenominator;
    mDenominator = mDenominator * rhs.mDenominator;
    mValueChanged = true;
    return *this;
}

template < class Type >
    inline EkRational < Type > &EkRational <
    Type >::operator*=(const EkRational < Type > &rhs)
{
    mNumerator = mNumerator * rhs.mNumerator;
    mDenominator = mDenominator * rhs.mDenominator;
    mValueChanged = true;
    return *this;
}

template < class Type >
    inline EkRational < Type > &EkRational <
    Type >::operator/=(const EkRational < Type > &rhs)
{
    mNumerator = mNumerator * rhs.mDenominator;
    mDenominator = mDenominator * rhs.mNumerator;
    mValueChanged = true;
    return *this;
}

template < class Type >
    inline bool EkRational < Type >::operator==(const EkRational < Type >
                                                &rhs) const
{
    return ((mNumerator == rhs.mNumerator)
            && (mDenominator == rhs.mDenominator));
}

template < class Type >
    inline bool EkRational < Type >::operator!=(const EkRational < Type >
                                                &rhs) const
{
    return ((mNumerator != rhs.mNumerator)
            || (mDenominator != rhs.mDenominator));
}

template < class Type >
    inline bool EkRational < Type >::operator<(const EkRational < Type >
                                               &rhs) const
{
    return (asDouble() < rhs.asDouble());
}

template < class Type >
    inline bool EkRational < Type >::operator<=(const EkRational < Type >
                                                &rhs) const
{
    return (asDouble() <= rhs.asDouble());
}

template < class Type >
    inline bool EkRational < Type >::operator>(const EkRational < Type >
                                               &rhs) const
{
    return (asDouble() > rhs.asDouble());
}

template < class Type >
    inline bool EkRational < Type >::operator>=(const EkRational < Type >
                                                &rhs) const
{
    return (asDouble() >= rhs.asDouble());
}

template < class Type > inline double EkRational < Type >::asDouble(void) const
{
    calcFpVal();
    return mFpVal;
}

template < class Type >
    inline const EkRational < Type > operator+(const EkRational < Type > &lhs,
                                               const EkRational < Type > &rhs)
{
    return EkRational < Type > (lhs) += rhs;
}

template < class Type >
    inline const EkRational < Type > operator-(const EkRational < Type > &lhs,
                                               const EkRational < Type > &rhs)
{
    return EkRational < Type > (lhs) -= rhs;
}

template < class Type >
    inline const EkRational < Type > operator*(const EkRational < Type > &lhs,
                                               const EkRational < Type > &rhs)
{
    return EkRational < Type > (lhs) *= rhs;
}

template < class Type >
    inline const EkRational < Type > operator/(const EkRational < Type > &lhs,
                                               const EkRational < Type > &rhs)
{
    return EkRational < Type > (lhs) /= rhs;
}

template < class Type >
    inline istream & EkRational < Type >::read(istream & is)
{
    char ch;
    is >> mNumerator;
    is >> ch;
    is >> mDenominator;

    mValueChanged = true;

    return is;
}

template < class Type >
    inline istream & operator>>(istream & is, EkRational < Type > &rat)
{
    return rat.read(is);
}

template < class Type >
    inline ostream & EkRational < Type >::print(ostream & os) const
{
    os << mNumerator << "/" << mDenominator;
    return os;
}

template < class Type >
    inline ostream & operator<<(ostream & os, const EkRational < Type > &rat)
{
    return rat.print(os);
}

/* This algorithm will not work with negative floating point
 * numbers - so record the sign and set the calculated numerator's
 * sign at end.
 */
template < class Type >
    double EkRational < Type >::rationalize(const double &fpNum,
                                            const double error)
{
    bool setSign = false;
    if (fpNum < 0.0)
    {
        // cannot handle a negative floating point value if
        // the underlying type of EkRational is unsigned
        if (typeid(*this).name() == typeid(unsigned long).name())
             return -1.0;

        setSign = true;
        mFpVal = -fpNum;
    }
    else
        mFpVal = fpNum;

    if (mFpVal < 3.0e-10 || mFpVal > 4.0e+10 || error < 0.0)
        return (-1.0);

    unsigned long D, N;
    double epsilon = 0.0;

    mDenominator = D = 1;
    mNumerator = static_cast < int >(mFpVal);
    N = mNumerator + 1;

    while (true)
    {
        double r = 0.0;
        if (mFpVal * (mDenominator) != static_cast < double >(mNumerator))
        {
            r = (N - mFpVal * D) / ((mFpVal * mDenominator) - mNumerator);
            if (r <= 1.0)
            {
                unsigned long t = N;
                N = mNumerator;
                mNumerator = t;
                t = D;
                D = mDenominator;
                mDenominator = t;
            }
        }
        else
            break;

        epsilon = fabs(1.0 - (mNumerator / (mFpVal * mDenominator)));
        if (epsilon <= error)
            break;

        double m = 10.0;
        while (m * epsilon < 1.0)
            m *= 10.0;

        epsilon = 1.0 / m * ((int) (0.5 + m * epsilon));

        if (epsilon <= error)
            break;

        if (r < 1.0)
            r = 1.0 / r;

        N += mNumerator * static_cast < unsigned long >(r);
        D += mDenominator * static_cast < unsigned long >(r);
        mNumerator += N;
        mDenominator += D;
    }
    if (setSign)
        mNumerator = -mNumerator;
    return epsilon;
}

#endif

