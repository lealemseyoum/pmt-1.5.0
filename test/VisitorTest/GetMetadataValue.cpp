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
 * Creation Date: mm/dd/yyyy
 *
 * Original Author: 
 * George Sotak <email address> 
 * <copyright notice>
 *
 * Contributor(s): 
 * FirstName LastName <email address>
 * FirstName LastName <email address>
 */



#include "GetMetadataValue.h"

void GetMetadataValue::visit(PmtMetadataT < bool > &md)
{

    bool value = md.value();

    const char *charValue = value?"true":"false";
    cout << endl << "--- In visit PmtMetadataT < bool > ---" << endl;
    cout << "Key : " << md.fullKey(false) << "  value: " << charValue << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < vector < bool > > &md)
{

    vector < bool > value = md.value();

    cout << endl << "--- In visit PmtMetadataT < vector < bool > > ---" << endl;
    cout << "Key : " << md.fullKey(false) << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < char > &md)
{

    char value = md.value();

    cout << endl << "--- In visit PmtMetadataT < char > ---" << endl;
    cout << "Key : " << md.fullKey(false) << "  value: " << value << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < vector < char > > &md)
{

    vector < char > value = md.value();

    cout << endl << "--- In visit PmtMetadataT < vector < char > > ---" << endl;
    cout << "Key : " << md.fullKey(false) << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < unsigned char > &md)
{

    unsigned char value = md.value();

    cout << endl << "--- In visit PmtMetadataT < unsigned char > ---" << endl;
    cout << "Key : " << md.fullKey(false) << "  value: " << value << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < vector < unsigned char > > &md)
{

    vector < unsigned char > value = md.value();

    cout << endl << "--- In visit PmtMetadataT < vector < unsigned char > > ---" << endl;
    cout << "Key : " << md.fullKey(false) << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < short > &md)
{

    short value = md.value();

    cout << endl << "--- In visit PmtMetadataT < short > ---" << endl;
    cout << "Key : " << md.fullKey(false) << "  value: " << value << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < vector < short > > &md)
{

    vector < short > value = md.value();

    cout << endl << "--- In visit PmtMetadataT < vector < short > > ---" << endl;
    cout << "Key : " << md.fullKey(false) << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < unsigned short > &md)
{

    unsigned short value = md.value();

    cout << endl << "--- In visit PmtMetadataT < unsigned short > ---" << endl;
    cout << "Key : " << md.fullKey(false) << "  value: " << value << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < vector < unsigned short > > &md)
{

    vector < unsigned short > value = md.value();

    cout << endl << "--- In visit PmtMetadataT < vector < unsigned short > > ---" << endl;
    cout << "Key : " << md.fullKey(false) << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < int > &md)
{

    int value = md.value();

    cout << endl << "--- In visit PmtMetadataT < int > ---" << endl;
    cout << "Key : " << md.fullKey(false) << "  value: " << value << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < vector < int > > &md)
{

    vector < int > value = md.value();

    cout << endl << "--- In visit PmtMetadataT < vector < int > > ---" << endl;
    cout << "Key : " << md.fullKey(false) << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < unsigned int > &md)
{

    unsigned int value = md.value();

    cout << endl << "--- In visit PmtMetadataT < unsigned int > ---" << endl;
    cout << "Key : " << md.fullKey(false) << "  value: " << value << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < vector < unsigned int > > &md)
{

    vector < unsigned int > value = md.value();

    cout << endl << "--- In visit PmtMetadataT < vector < unsigned int > > ---" << endl;
    cout << "Key : " << md.fullKey(false) << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < long > &md)
{

    long value = md.value();

    cout << endl << "--- In visit PmtMetadataT < long > ---" << endl;
    cout << "Key : " << md.fullKey(false) << "  value: " << value << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < vector < long > > &md)
{

    vector < long > value = md.value();

    cout << endl << "--- In visit PmtMetadataT < vector < long > >  ---" << endl;
    cout << "Key : " << md.fullKey(false) << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < unsigned long > &md)
{

    unsigned long value = md.value();

    cout << endl << "--- In visit PmtMetadataT < unsigned long > ---" << endl;
    cout << "Key : " << md.fullKey(false) << "  value: " << value << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < vector < unsigned long > > &md)
{

    vector < unsigned long > value = md.value();

    cout << endl << "--- In visit PmtMetadataT < vector < unsigned long > > ---" << endl;
    cout << "Key : " << md.fullKey(false) << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < float > &md)
{

    float value = md.value();

    cout << endl << "--- In visit PmtMetadataT < float > ---" << endl;
    cout << "Key : " << md.fullKey(false) << "  value: " << value << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < vector < float > > &md)
{

    vector < float > value = md.value();

    cout << endl << "--- In visit PmtMetadataT < vector < float > > ---" << endl;
    cout << "Key : " << md.fullKey(false) << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < double > &md)
{

    double value = md.value();

    cout << endl << "--- In visit PmtMetadataT < double > ---" << endl;
    cout << "Key : " << md.fullKey(false) << "  value: " << value << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < vector < double > > &md)
{

    vector < double > value = md.value();

    cout << endl << "--- In visit PmtMetadataT < vector < double > > ---" << endl;
    cout << "Key : " << md.fullKey(false) << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < string > &md)
{

    string value = md.value();

    cout << endl << "--- In visit PmtMetadataT < string > ---" << endl;
    cout << "Key : " << md.fullKey(false) << "  value: " << value << endl;

    return;
}

void GetMetadataValue::visit(PmtMetadataT < vector < string > > &md)
{

    vector < string > value = md.value();

    cout << endl << "--- In visit PmtMetadataT < vector < string > > ---" << endl;
    cout << "Key : " << md.fullKey(false) << endl;

    return;
}
