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
 * Creation date: 7/18/2000
 *
 * Original Author: 
 * Chris Lin <ti.lin@kodak.com> 
 * 
 * Contributor(s): 
 * Sam Fryer <samuel.fryer@kodak.com>
 *
*/

#define PMT_TRANSLATOR_CPP

#include "EkCompiler.h"
#include "PmtTranslator.h"
#include "PmtDefinitionTypes.h"
#include <climits>
#include <cfloat>

#include "PmtFaceTranslator.h"

template <>
void PmtTranslatorT < bool > ::assign(PmtMetadata * md, void * &vformatValue,
                            unsigned int &fvCount, bool valueToMetadata)
{
    bool * &formatValue = (bool *&)vformatValue;
    PmtMDType id = md->getMDType();
    bool bVector = isVector(id);

    // Do we need: bool <-> char, ...
    switch (id)
    {
    case PMT_MD_TYPE_BOOL:
    case PMT_MD_TYPE_VBOOL:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < bool > (0), md, fvCount,
                             formatValue);
            else
                upAssignToVal(bVector, static_cast < bool > (0), md, fvCount,
                              formatValue);

            break;

        }
    default:
        {
            SPmtString msg =
                "PmtTranslator::assignT: Cannot convert BOOL format type to ";
            msg += md->type();
            msg += " for key: ";
            msg += md->key();
            throw PmtError(PMT_FORMAT_ENTRY_VALUE_CONVERSION_ERROR, msg,
                           WHERE);
        }

    }
}

// assign to / from signed byte(char) format data types
template <>
void PmtTranslatorT < char > ::assign(PmtMetadata * md, void *&vformatValue,
                            unsigned int &fvCount, bool valueToMetadata)
{
    char * &formatValue = (char *&)vformatValue;
    PmtMDType id = md->getMDType();
    bool bVector = isVector(id);

    switch (id)
    {
    case PMT_MD_TYPE_INT8:
    case PMT_MD_TYPE_VINT8:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < char >(0), md, fvCount,
                             formatValue);
            else
                upAssignToVal(bVector, static_cast < char >(0), md, fvCount,
                              formatValue);

            break;
        }
    case PMT_MD_TYPE_UINT8:
    case PMT_MD_TYPE_VUINT8:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < unsigned char >(0), md,
                             fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < unsigned char >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_INT16:
    case PMT_MD_TYPE_VINT16:
    case PMT_MD_TYPE_UINT16:
    case PMT_MD_TYPE_VUINT16:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < short >(0), md, fvCount,
                             formatValue);
            else
                downAssignToVal(bVector, static_cast < short >(0), md, fvCount,
                                formatValue, static_cast < char >(SCHAR_MIN),
                                static_cast < char >(SCHAR_MAX));

            break;
        }
    case PMT_MD_TYPE_INT32:
    case PMT_MD_TYPE_VINT32:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < long >(0), md, fvCount,
                             formatValue);
            else
                downAssignToVal(bVector, static_cast < long >(0), md, fvCount,
                                formatValue, static_cast < char >(SCHAR_MIN),
                                static_cast < char >(SCHAR_MAX));
            break;
        }
    case PMT_MD_TYPE_FLOAT:
    case PMT_MD_TYPE_VFLOAT:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < float >(0), md, fvCount,
                             formatValue);
            else
                downAssignToVal(bVector, static_cast < float >(0), md, fvCount,
                                formatValue, static_cast < char >(SCHAR_MIN),
                                static_cast < char >(SCHAR_MAX));
            break;
        }
    case PMT_MD_TYPE_DOUBLE:
    case PMT_MD_TYPE_VDOUBLE:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < double >(0), md, fvCount,
                             formatValue);
            else
                downAssignToVal(bVector, static_cast < double >(0), md,
                                fvCount, formatValue,
                                static_cast < char >(SCHAR_MIN),
                                static_cast < char >(SCHAR_MAX));
            break;
        }
    default:
        {
            SPmtString msg =
                "PmtTranslator::assignT: Cannot convert CHAR format type to ";
            msg += md->type();
            msg += " for key: ";
            msg += md->key();
            throw PmtError(PMT_FORMAT_ENTRY_VALUE_CONVERSION_ERROR, msg,
                           WHERE);
        }
    }
}

// assign to / from unsigned byte(char) format data types
template <>
void PmtTranslatorT < unsigned char > ::assign(PmtMetadata * md, void *&vformatValue,
                            unsigned int &fvCount, bool valueToMetadata)
{
    unsigned char * &formatValue = (unsigned char *&)vformatValue;
    PmtMDType id = md->getMDType();
    bool bVector = isVector(id);

    switch (id)
    {
    case PMT_MD_TYPE_UINT8:
    case PMT_MD_TYPE_VUINT8:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < unsigned char >(0), md,
                             fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < unsigned char >(0), md,
                              fvCount, formatValue);
            break;
        }
    case PMT_MD_TYPE_UINT16:
    case PMT_MD_TYPE_VUINT16:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < unsigned short >(0), md,
                             fvCount, formatValue);
            else
                downAssignToVal(bVector, static_cast < unsigned short >(0), md,
                                fvCount, formatValue,
                                static_cast < unsigned char >(0),
                                static_cast < unsigned char >(UCHAR_MAX));
            break;
        }
    case PMT_MD_TYPE_UINT32:
    case PMT_MD_TYPE_VUINT32:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < unsigned long >(0), md,
                             fvCount, formatValue);
            else
                downAssignToVal(bVector, static_cast < unsigned long >(0), md,
                                fvCount, formatValue,
                                static_cast < unsigned char >(0),
                                static_cast < unsigned char >(UCHAR_MAX));
            break;
        }
    case PMT_MD_TYPE_FLOAT:
    case PMT_MD_TYPE_VFLOAT:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < float >(0), md, fvCount,
                             formatValue);
            else
                downAssignToVal(bVector, static_cast < float >(0), md, fvCount,
                                formatValue, static_cast < unsigned char >(0),
                                static_cast < unsigned char >(UCHAR_MAX));
            break;
        }
    case PMT_MD_TYPE_DOUBLE:
    case PMT_MD_TYPE_VDOUBLE:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < double >(0), md, fvCount,
                             formatValue);
            else
                downAssignToVal(bVector, static_cast < double >(0), md,
                                fvCount, formatValue,
                                static_cast < unsigned char >(0),
                                static_cast < unsigned char >(UCHAR_MAX));
            break;
        }
    default:
        {
            SPmtString msg =
                "PmtTranslator::assignT: Cannot convert UNSIGNED CHAR format type to ";
            msg += md->type();
            msg += " for key: ";
            msg += md->key();
            throw PmtError(PMT_FORMAT_ENTRY_VALUE_CONVERSION_ERROR, msg,
                           WHERE);
        }
    }
};

template <>
void PmtTranslatorT < short > ::assign(PmtMetadata * md, void *&vformatValue,
                            unsigned int &fvCount, bool valueToMetadata)
{
    short * &formatValue = (short *&)vformatValue;
    PmtMDType id = md->getMDType();
    bool bVector = isVector(id);

    switch (id)
    {
    case PMT_MD_TYPE_INT8:
    case PMT_MD_TYPE_VINT8:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md,
                               static_cast < char >(SCHAR_MIN),
                               static_cast < char >(SCHAR_MAX), fvCount,
                               formatValue);
            else
                upAssignToVal(bVector, static_cast < char >(0), md, fvCount,
                              formatValue);

            break;
        }
    case PMT_MD_TYPE_INT16:
    case PMT_MD_TYPE_VINT16:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < short >(0), md, fvCount,
                             formatValue);
            else
                upAssignToVal(bVector, static_cast < short >(0), md, fvCount,
                              formatValue);

            break;
        }
    case PMT_MD_TYPE_INT32:
    case PMT_MD_TYPE_VINT32:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < long >(0), md, fvCount,
                             formatValue);
            else
                downAssignToVal(bVector, static_cast < long >(0), md, fvCount,
                                formatValue, static_cast < short >(SHRT_MIN),
                                static_cast < short >(SHRT_MAX));

            break;
        }
    case PMT_MD_TYPE_FLOAT:
    case PMT_MD_TYPE_VFLOAT:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < float >(0), md, fvCount,
                             formatValue);
            else
                downAssignToVal(bVector, static_cast < float >(0), md, fvCount,
                                formatValue, static_cast < short >(SHRT_MIN),
                                static_cast < short >(SHRT_MAX));
            break;
        }
    case PMT_MD_TYPE_DOUBLE:
    case PMT_MD_TYPE_VDOUBLE:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < double >(0), md, fvCount,
                             formatValue);
            else
                downAssignToVal(bVector, static_cast < double >(0), md,
                                fvCount, formatValue,
                                static_cast < short >(SHRT_MIN),
                                static_cast < short >(SHRT_MAX));
            break;
        }
    default:
        {
            SPmtString msg =
                "PmtTranslator::assignT: Cannot convert SHORT format type to ";
            msg += md->type();
            msg += " for key: ";
            msg += md->key();
            throw PmtError(PMT_FORMAT_ENTRY_VALUE_CONVERSION_ERROR, msg,
                           WHERE);
        }
    }
}

template <>
void PmtTranslatorT < unsigned short > ::assign(PmtMetadata * md, void *&vformatValue,
                            unsigned int &fvCount, bool valueToMetadata)
{
    unsigned short * &formatValue = (unsigned short *&)vformatValue;
    PmtMDType id = md->getMDType();
    bool bVector = isVector(id);

    switch (id)
    {
    case PMT_MD_TYPE_UINT8:
    case PMT_MD_TYPE_VUINT8:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md,
                               static_cast < unsigned char >(0),
                               static_cast < unsigned char >(UCHAR_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < unsigned char >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_UINT16:
    case PMT_MD_TYPE_VUINT16:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < unsigned short >(0), md,
                             fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < unsigned short >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_UINT32:
    case PMT_MD_TYPE_VUINT32:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < unsigned long >(0), md,
                             fvCount, formatValue);
            else
                downAssignToVal(bVector, static_cast < unsigned long >(0), md,
                                fvCount, formatValue,
                                static_cast < unsigned short >(0),
                                static_cast < unsigned short >(USHRT_MAX));

            break;
        }
    case PMT_MD_TYPE_FLOAT:
    case PMT_MD_TYPE_VFLOAT:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < float >(0), md,
                             fvCount, formatValue);
            else
                downAssignToVal(bVector, static_cast < float >(0), md,
                                fvCount, formatValue,
                                static_cast < unsigned short >(0),
                                static_cast < unsigned short >(USHRT_MAX));

            break;
        }
    case PMT_MD_TYPE_DOUBLE:
    case PMT_MD_TYPE_VDOUBLE:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < double >(0), md,
                             fvCount, formatValue);
            else
                downAssignToVal(bVector, static_cast < double >(0), md,
                                fvCount, formatValue,
                                static_cast < unsigned short >(0),
                                static_cast < unsigned short >(USHRT_MAX));

            break;
        }
    default:
        {
            SPmtString msg =
                "PmtTranslator::assignT: Cannot convert UNSIGNED SHORT format type to ";
            msg += md->type();
            msg += " for key: ";
            msg += md->key();
            throw PmtError(PMT_FORMAT_ENTRY_VALUE_CONVERSION_ERROR, msg,
                           WHERE);
        }
    }
}


template <>
void PmtTranslatorT < int > ::assign(PmtMetadata * md, void *&vformatValue,
                            unsigned int &fvCount, bool valueToMetadata)
{
    int * &formatValue = (int *&)vformatValue;
    PmtMDType id = md->getMDType();
    bool bVector = isVector(id);

    switch (id)
    {
    case PMT_MD_TYPE_INT8:
    case PMT_MD_TYPE_VINT8:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md,
                               static_cast < char >(SCHAR_MIN),
                               static_cast < char >(SCHAR_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < char >(0), md, fvCount,
                              formatValue);

            break;
        }
    case PMT_MD_TYPE_INT16:
    case PMT_MD_TYPE_VINT16:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md,
                               static_cast < short >(SHRT_MIN),
                               static_cast < short >(SHRT_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < short >(0), md, fvCount,
                              formatValue);

            break;
        }
    case PMT_MD_TYPE_INT32:
    case PMT_MD_TYPE_VINT32:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < long >(0), md, fvCount,
                             formatValue);
            else
                upAssignToVal(bVector, static_cast < long >(0), md, fvCount,
                              formatValue);

            break;
        }
    case PMT_MD_TYPE_FLOAT:
    case PMT_MD_TYPE_VFLOAT:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < float >(0), md,
                             fvCount, formatValue);
            else
                downAssignToVal(bVector, static_cast < float >(0), md,
                                fvCount, formatValue,
                                static_cast < int >(INT_MIN),
                                static_cast < int >(INT_MAX));

            break;
        }
    case PMT_MD_TYPE_DOUBLE:
    case PMT_MD_TYPE_VDOUBLE:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < double >(0), md,
                             fvCount, formatValue);
            else
                downAssignToVal(bVector, static_cast < double >(0), md,
                                fvCount, formatValue,
                                static_cast < int >(INT_MIN),
                                static_cast < int >(INT_MAX));

            break;
        }
    default:
        {
            SPmtString msg =
                "PmtTranslator::assignT: Cannot convert LONG format type to ";
            msg += md->type();
            msg += " for key: ";
            msg += md->key();
            throw PmtError(PMT_FORMAT_ENTRY_VALUE_CONVERSION_ERROR, msg,
                           WHERE);
        }
    }
}

template <>
void PmtTranslatorT < unsigned int > ::assign(PmtMetadata * md, void *&vformatValue,
                            unsigned int &fvCount, bool valueToMetadata)
{
    unsigned int * &formatValue = (unsigned int *&)vformatValue;
    PmtMDType id = md->getMDType();
    bool bVector = isVector(id);

    switch (id)
    {
    case PMT_MD_TYPE_UINT8:
    case PMT_MD_TYPE_VUINT8:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md, static_cast < unsigned char >(0),
                               static_cast < unsigned char >(UCHAR_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < unsigned char >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_UINT16:
    case PMT_MD_TYPE_VUINT16:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md, static_cast < unsigned short >(0),
                               static_cast < unsigned short >(USHRT_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < unsigned short >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_UINT32:
    case PMT_MD_TYPE_VUINT32:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < unsigned long >(0), md,
                             fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < unsigned long >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_FLOAT:
    case PMT_MD_TYPE_VFLOAT:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < float >(0), md,
                             fvCount, formatValue);
            else
                downAssignToVal(bVector, static_cast < float >(0), md,
                                fvCount, formatValue,
                                static_cast < unsigned int >(0),
                                static_cast < unsigned int >(UINT_MAX));

            break;
        }
    case PMT_MD_TYPE_DOUBLE:
    case PMT_MD_TYPE_VDOUBLE:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < double >(0), md,
                             fvCount, formatValue);
            else
                downAssignToVal(bVector, static_cast < double >(0), md,
                                fvCount, formatValue,
                                static_cast < unsigned int >(0),
                                static_cast < unsigned int >(UINT_MAX));

            break;
        }
    default:
        {
            SPmtString msg =
                "PmtTranslator::assignT: Cannot convert UNSIGNED LONG format type to ";
            msg += md->type();
            msg += " for key: ";
            msg += md->key();
            throw PmtError(PMT_FORMAT_ENTRY_VALUE_CONVERSION_ERROR, msg,
                           WHERE);
        }
    }
}

template <>
void PmtTranslatorT < float > ::assign(PmtMetadata * md, void *&vformatValue,
                            unsigned int &fvCount, bool valueToMetadata)
{
    float * &formatValue = (float *&)vformatValue;
    PmtMDType id = md->getMDType();
    bool bVector = isVector(id);

    switch (id)
    {
    case PMT_MD_TYPE_INT8:
    case PMT_MD_TYPE_VINT8:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md, static_cast < char >(SCHAR_MIN),
                               static_cast < char >(SCHAR_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < char >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_UINT8:
    case PMT_MD_TYPE_VUINT8:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md, static_cast < unsigned char >(0),
                               static_cast < unsigned char >(UCHAR_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < unsigned char >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_INT16:
    case PMT_MD_TYPE_VINT16:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md, static_cast < short >(SHRT_MIN),
                               static_cast < short >(SHRT_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < short >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_UINT16:
    case PMT_MD_TYPE_VUINT16:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md, static_cast < unsigned short >(0),
                               static_cast < unsigned short >(USHRT_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < unsigned short >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_INT32:
    case PMT_MD_TYPE_VINT32:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md, static_cast < long >(LONG_MIN),
                               static_cast < long >(LONG_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < long >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_UINT32:
    case PMT_MD_TYPE_VUINT32:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md, static_cast < unsigned long >(0),
                               static_cast < unsigned long >(ULONG_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < unsigned long >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_FLOAT:
    case PMT_MD_TYPE_VFLOAT:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < float >(0), md, fvCount,
                             formatValue);
            else
                upAssignToVal(bVector, static_cast < float >(0), md, fvCount,
                              formatValue);

            break;
        }
    case PMT_MD_TYPE_DOUBLE:
    case PMT_MD_TYPE_VDOUBLE:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < double >(0), md, fvCount,
                             formatValue);
            else
                downAssignToVal(bVector, static_cast < double >(0), md,
                                fvCount, formatValue,
                                static_cast < float >(FLT_MIN),
                                static_cast < float >(FLT_MAX));

            break;
        }
    default:
        {
            SPmtString msg =
                "PmtTranslator::assignT: Cannot convert FLOAT format type to ";
            msg += md->type();
            msg += " for key: ";
            msg += md->key();
            throw PmtError(PMT_FORMAT_ENTRY_VALUE_CONVERSION_ERROR, msg,
                           WHERE);
        }
    }
}

template <>
void PmtTranslatorT < double > ::assign(PmtMetadata * md, void *&vformatValue,
                            unsigned int &fvCount, bool valueToMetadata)
{
    double * &formatValue = (double *&)vformatValue;
    PmtMDType id = md->getMDType();
    bool bVector = isVector(id);

    switch (id)
    {
    case PMT_MD_TYPE_INT8:
    case PMT_MD_TYPE_VINT8:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md, static_cast < char >(SCHAR_MIN),
                               static_cast < char >(SCHAR_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < char >(0), md, fvCount,
                              formatValue);

            break;
        }
    case PMT_MD_TYPE_UINT8:
    case PMT_MD_TYPE_VUINT8:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md, static_cast < unsigned char >(0),
                               static_cast < unsigned char >(UCHAR_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < unsigned char >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_INT16:
    case PMT_MD_TYPE_VINT16:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md, static_cast < short >(SHRT_MIN),
                               static_cast < short >(SHRT_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < short >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_UINT16:
    case PMT_MD_TYPE_VUINT16:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md, static_cast < unsigned short >(0),
                               static_cast < unsigned short >(USHRT_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < unsigned short >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_INT32:
    case PMT_MD_TYPE_VINT32:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md, static_cast < long >(LONG_MIN),
                               static_cast < long >(LONG_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < long >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_UINT32:
    case PMT_MD_TYPE_VUINT32:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md, static_cast < unsigned long >(0),
                               static_cast < unsigned long >(ULONG_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < unsigned long >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_FLOAT:
    case PMT_MD_TYPE_VFLOAT:
        {
            if (valueToMetadata)
                downAssignToMd(bVector, md,
                               static_cast < float >(FLT_MIN),
                               static_cast < float >(FLT_MAX),
                               fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < float >(0), md,
                              fvCount, formatValue);

            break;
        }
    case PMT_MD_TYPE_DOUBLE:
    case PMT_MD_TYPE_VDOUBLE:
        {
            if (valueToMetadata)
                upAssignToMd(bVector, static_cast < double >(0), md,
                             fvCount, formatValue);
            else
                upAssignToVal(bVector, static_cast < double >(0), md,
                              fvCount, formatValue);

            break;
        }
    default:
        {
            SPmtString msg =
                "PmtTranslator::assignT: Cannot convert DOUBLE format type to ";
            msg += md->type();
            msg += " for key: ";
            msg += md->key();
            throw PmtError(PMT_FORMAT_ENTRY_VALUE_CONVERSION_ERROR, msg,
                           WHERE);
        }
    }
}

template <>
void PmtTranslatorT < SPmtString > ::assign(PmtMetadata * md, void * &vFormatValue,
                            unsigned int &fvCount, bool valueToMetadata)
{
    SPmtString * &value = (SPmtString *&)vFormatValue;
    PmtMDType id = md->getMDType();

    switch (id)
    {
    case PMT_MD_TYPE_STRING:
        {
            PmtMetadataT < string > *mdt =
                ( PmtMetadataT < string > *)(md);
            if (valueToMetadata)
            {
                mdt->value() = *value;
                delete value;
            }
            else
            {
                value = new SPmtString(mdt->value());
            }

            break;
        }
    default:
        {
            SPmtString msg =
                "PmtTranslator::assignT: Cannot convert STRING format type to ";
            msg += md->type();
            msg += " for key: ";
            msg += md->key();
            throw PmtError(PMT_FORMAT_ENTRY_VALUE_CONVERSION_ERROR, msg,
                           WHERE);
        }
    }
}

template <>
void PmtTranslatorT < char * > ::assign(PmtMetadata * md, void *&vFValue,
                                            unsigned int &vCount, bool valToMd)
{
    char * &value = (char *&)vFValue;
    if (valToMd)
    {
        // SPmtString handles the case where vCount == 0
        SPmtString aStr(value, vCount);
        delete [] value;
        md->setValueStr(EK_STR2WIDE(aStr));
    }
    else
    {
        SPmtString aStr = EK_WIDE2STR(md->getValueStr());
        vCount = aStr.size();
        value = new char[vCount];
        strncpy(value, aStr.c_str(),vCount);
    }

}


template <>
void PmtTranslatorT < void * > ::assign(PmtMetadata * md, void *&vFValue,
                                        unsigned int &vCount, bool valToMd)
{
    char * &value = (char *&)vFValue;
    PmtMDType id = md->getMDType();
    bool bVector = isVector(id);

    switch (id)
    {
    case PMT_MD_TYPE_UINT8:
    case PMT_MD_TYPE_VUINT8:
        {
            if (valToMd)
                upAssignToMd(bVector, static_cast < unsigned char >(0), md,
                             vCount, (unsigned char *&) value);
            else
                upAssignToVal(bVector, static_cast < unsigned char >(0), md,
                              vCount, (unsigned char *&) value);

            break;
        }
    case PMT_MD_TYPE_STRING:
    // There are some special cases that are defined as string in the schema
    // but should be UNDEFINED in the file.
        {
            if (valToMd)
            {
                SPmtString aStr(value, vCount);
                delete [] value;
                md->setValueStr(EK_STR2WIDE(aStr));
            }
            else
            {
                SPmtString aStr = EK_WIDE2STR(md->getValueStr());
                vCount = aStr.size();
                value = new char[vCount];
                strncpy(value, aStr.c_str(),vCount);
            }
            
            break;
        }
    default:
        {
            SPmtString msg =
       "PmtBuiltin2Undefined::assign: Cannot convert UNDEFINED format type to ";
            msg += md->type();
            msg += " (TrueType=";
            msg += md->trueType();
            msg += ") for key: ";
            msg += md->key();
            throw PmtError(PMT_FORMAT_ENTRY_VALUE_CONVERSION_ERROR, msg,
                           WHERE);
        }
    }
}


//--------------------------------------------------------------------
// Template Instantiations.
// (So the code all stays here...)
//--------------------------------------------------------------------

template class PmtTranslatorT < bool > ;
template class PmtTranslatorT < char > ;
template class PmtTranslatorT < unsigned char > ;
template class PmtTranslatorT < short > ;
template class PmtTranslatorT < unsigned short > ;
template class PmtTranslatorT < int > ;
template class PmtTranslatorT < unsigned int > ;
template class PmtTranslatorT < float > ;
template class PmtTranslatorT < double > ;
template class PmtTranslatorT < SPmtString > ;
template class PmtTranslatorT < char * > ;
template class PmtTranslatorT < void * > ;


//====================================================================
//====================================================================
// PmtTranslator Class method definitions
//--------------------------------------------------------------------

PmtTranslatorPtr PmtTranslator::create(const SPmtString & type)
{
    PmtTranslator *newTranslator = NULL;

    switch (stringToFactoryKey(type))
    {
    case PMT_TRANSLATOR_BUILTIN2BOOL:
        newTranslator = new PmtTranslatorT < bool > ();
        break;
    case PMT_TRANSLATOR_BUILTIN2CHAR:
        newTranslator = new PmtTranslatorT < char > ();
        break;
    case PMT_TRANSLATOR_BUILTIN2UCHAR:
        newTranslator = new PmtTranslatorT < unsigned char > ();
        break;
    case PMT_TRANSLATOR_BUILTIN2SHORT:
        newTranslator = new PmtTranslatorT < short > ();
        break;
    case PMT_TRANSLATOR_BUILTIN2USHORT:
        newTranslator = new PmtTranslatorT < unsigned short > ();
        break;
    case PMT_TRANSLATOR_BUILTIN2LONG:
        newTranslator = new PmtTranslatorT < int > ();
        break;
    case PMT_TRANSLATOR_BUILTIN2ULONG:
        newTranslator = new PmtTranslatorT < unsigned int > ();
        break;
    case PMT_TRANSLATOR_BUILTIN2FLOAT:
        newTranslator = new PmtTranslatorT < float > ();
        break;
    case PMT_TRANSLATOR_BUILTIN2DOUBLE:
        newTranslator = new PmtTranslatorT < double > ();
        break;
    case PMT_TRANSLATOR_BUILTIN2STRING:
        newTranslator = new PmtTranslatorT < SPmtString > ();
        break;
    case PMT_TRANSLATOR_BUILTIN2ASCII:
        newTranslator = new PmtTranslatorT < char * > ();
        break;
//     case PMT_TRANSLATOR_BUILTIN2RATIONAL:
//         newTranslator = new PmtTranslatorT < Rational > ();
//         break;
//     case PMT_TRANSLATOR_BUILTIN2URATIONAL:
//         newTranslator = new PmtTranslatorT < URational > ();
//         break;
    case PMT_TRANSLATOR_BUILTIN2FACE:
        newTranslator = new PmtFaceTranslator();
        break;
    case PMT_TRANSLATOR_BUILTIN2UNDEFINED:
    default:
        newTranslator = new PmtTranslatorT < void * > ();
        break;
    }
    return newTranslator;
}

bool PmtTranslator::isVector(const PmtMDType & id)
{
    bool isVec = false;
    switch (id)
    {
    case PMT_MD_TYPE_VBOOL:
    case PMT_MD_TYPE_VINT8:
    case PMT_MD_TYPE_VUINT8:
    case PMT_MD_TYPE_VINT16:
    case PMT_MD_TYPE_VUINT16:
    case PMT_MD_TYPE_VINT32:
    case PMT_MD_TYPE_VUINT32:
    case PMT_MD_TYPE_VFLOAT:
    case PMT_MD_TYPE_VDOUBLE:
    case PMT_MD_TYPE_VSTRING:
    case PMT_MD_TYPE_VWSTRING:
        isVec = true;

    default:;
        // if it's not one of the PMT_V* types -- we just return false.
    }
    return isVec;
}

// This method converts the translator type from the string to the corresponding
// factory key, using a hash function.
// The function is (char[8]<<1)+char[9], where char = "builtin2bool",
// "builtin2long", etc.
unsigned int PmtTranslator::stringToFactoryKey(const SPmtString & type)
{
    unsigned int typeSum = 0;
    if (type.length() >= 10)
    {
        typeSum = ((type.c_str())[8]) << 1;
        typeSum += (type.c_str())[9];
    }
    return typeSum;
}

