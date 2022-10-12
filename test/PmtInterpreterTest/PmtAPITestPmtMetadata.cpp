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
 * Copyright (C) 2000-2001 Eastman Kodak Company. All 
 * Rights Reserved. 
 * 
 * Original Author: 
 * Rosario, Ricardo     ricardo.rosario@kodak.com
 * 
 * Contributor(s): 
 * 
 *
 * Date: July 19, 2001
*/

#include "PmtAPITestPmtMetadata.h"

extern int gnThreads;

//This is the function that should be called by main to run all the 
//PmtMetadata tests.
void PmtAPITestPmtMetadata::runTest()
{
    cout << "****************************************" << endl;
    cout << "* Testing PmtMetadata Static Functions *" << endl;
    cout << "****************************************" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load();
    delete definitionInterpreter;

    testCreate();
    testGetAliases();
    testGetFactory();

    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}

void PmtAPITestPmtMetadata::testCreate()
{
    cout << "   ------------------------" << endl;
    cout << "    Executing testCreate() " << endl;
    cout << "   ------------------------" << endl;

    cout << "create()->show(): " << endl;
    PmtMetadata::create()->show();
}

void PmtAPITestPmtMetadata::testGetAliases()
{
    cout << "   ----------------------------" << endl;
    cout << "    Executing testGetAliases() " << endl;
    cout << "   ----------------------------" << endl;
}

void PmtAPITestPmtMetadata::testGetFactory()
{
    cout << "   ----------------------------" << endl;
    cout << "    Executing testGetFactory() " << endl;
    cout << "   ----------------------------" << endl;
}

