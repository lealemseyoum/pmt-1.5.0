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
 * Date: July 17, 2001
*/

#include "PmtAPITestPmtMetadataT.h"

extern int gnThreads;

//This is the function that should be called by main to run all the 
//PmtMetadataT tests.
void PmtAPITestPmtMetadataT::runTest()
{
    const char *const ALL_TYPES_INFO_FILE = "AllTypesInfo.xml";

    cout << "**********************************" << endl;
    cout << "* Testing PmtMetadataT Functions *" << endl;
    cout << "**********************************" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load();
    delete definitionInterpreter;

    testKey();
    testFullKey();
    testOccurrence();
    testType();
    testTrueType();
    testParent();
    testSetAndGetValueStr();
    testIsComposite();
    testValue();
    testGetMetadatum();
    testGetMetadata();
    testDeleteMetadata();
    testPrint();
    testClone();
    testShow();
    testAccept();
    testSetFacet();
    testShowFacets();

    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();

    // The following tests require a different schema to be loaded so all types
    // can be tested...
    definitionInterpreter = new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load(ALL_TYPES_INFO_FILE);
    delete definitionInterpreter;

    testType_AllTypes();
    testTrueType_AllTypes();
    testSetAndGetValueStr_AllTypes();
}

//Call a PmtMetadataT object's key() (passing nothing - default, true and false
//as arguments) and printing the results.
void PmtAPITestPmtMetadataT::testKey()
{
    cout << "   ---------------------" << endl;
    cout << "    Executing testKey() " << endl;
    cout << "   ---------------------" << endl;

    char *mdKey = "CaptureConditions.Aperture";

    PmtMetadataPtr rootMd = PmtMetadata::create();

    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "key(): " << mdHandle->key() << endl;
    cout << "key(true): " << mdHandle->key(true) << endl;
    cout << "key(false): " << mdHandle->key(false) << endl;
}

//Call a PmtMetadataT object's fullKey() (passing nothing - default, true and false
//as arguments) and printing the results.
void PmtAPITestPmtMetadataT::testFullKey()
{
    cout << "   -------------------------" << endl;
    cout << "    Executing testFullKey() " << endl;
    cout << "   -------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions.Flash.Fired";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "fullKey(): " << mdHandle->fullKey() << endl;
    cout << "fullKey(true): " << mdHandle->fullKey(true) << endl;
    cout << "fullKey(false): " << mdHandle->fullKey(false) << endl;
}

//DOESN'T COMPILE!!!
void PmtAPITestPmtMetadataT::testOccurrence()
{
    cout << "   ---------------------------" << endl;
    cout << "    Executing testOcurrence() " << endl;
    cout << "   ---------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureDevice.FilmSize";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtConstMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    //const unsigned int& myInt = mdHandle->occurrence();
    cout << "occurrence(): " << mdHandle->occurrence() << endl;
}

//Test type() with several PmtMetadataT objects that have different
//types.
void PmtAPITestPmtMetadataT::testType()
{
    cout << "   ----------------------" << endl;
    cout << "    Executing testType() " << endl;
    cout << "   ----------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions.ImageNumber";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "CaptureConditions.Flash.Fired";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "CaptureDevice.FilmProductCode";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "DigitalProcess.ICCProfile";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;
}

//Test trueType() with several PmtMetadataT objects that have different
//true types.
void PmtAPITestPmtMetadataT::testTrueType()
{
    cout << "   --------------------------" << endl;
    cout << "    Executing testTrueType() " << endl;
    cout << "   --------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions.ImageNumber";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "CaptureConditions.Flash.Fired";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "CaptureDevice.FilmProductCode";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "DigitalProcess.ICCProfile";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;
}

//Call parent() on a PmtMetadataT object and display the parent's
//full key.
void PmtAPITestPmtMetadataT::testParent()
{
    cout << "   ------------------------" << endl;
    cout << "    Executing testParent() " << endl;
    cout << "   ------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureDevice.Scanner.ModelAndVersion";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "parent()->fullKey(): " << mdHandle->parent()->fullKey() << endl;
}

//isComposite() should return false (or 0) for all PmtCompositeMetadata objects
void PmtAPITestPmtMetadataT::testIsComposite()
{
    cout << "   -----------------------------" << endl;
    cout << "    Executing testIsComposite() " << endl;
    cout << "   -----------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "isComposite(): " << mdHandle->isComposite() << endl;

    mdKey = "CaptureDevice";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "isComposite(): " << mdHandle->isComposite() << endl;
}

//Test setting and getting the PmtMetadataT objects value using the
//setValueStr() and getValueStr() functions. Test with several objects
//that have different types.
void PmtAPITestPmtMetadataT::testSetAndGetValueStr()
{
    cout << "   -----------------------------------" << endl;
    cout << "    Executing testSetAndGetValueStr() " << endl;
    cout << "   -----------------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions.ExposureTime";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"3.77\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("3.77"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "CaptureConditions.Flash.Fired";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"true\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("true"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "CaptureDevice.HostComputer";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"My Computer\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("My Computer"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "SceneContent.ImageDescription";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    mdHandle->setValueStr(EK_L("a string"));
    //setValueStr again to make sure this string replaces the previous one
    //and isn't appended to it.
    mdHandle->setValueStr(EK_L("another string"));
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    rootMd->deleteMetadata("*");
}

//Test setting and getting the PmtMetadataT object's value using the
//value() function.
void PmtAPITestPmtMetadataT::testValue()
{
    cout << "   -----------------------" << endl;
    cout << "    Executing testValue() " << endl;
    cout << "   -----------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions.ExposureTime";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);

    PmtMetadataT < float >*mdt =
        dynamic_cast < PmtMetadataT < float >*>(mdHandle.ptr());
    cout << "value(): " << mdt->value() << endl;
    mdt->value() = 27.75;
    cout << "value() = 27.75,show(): " << endl;
    mdt->show();
    cout << "value(): " << mdt->value() << endl;
}

//This function tests getMetadatum() called on a PmtMetadataT object.
//The function is defined as virtual in PmtMetadata but is not overrided
//in PmtMetadataT. This should cause a call to his parent's getMetadatum()
//function, which returns NULL.
void PmtAPITestPmtMetadataT::testGetMetadatum()
{
    cout << "   ------------------------------" << endl;
    cout << "    Executing testGetMetadatum() " << endl;
    cout << "   ------------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions.ExposureTime";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    PmtMetadataPtr mdNullHandle = mdHandle->getMetadatum("Any Key");
    cout << "getMetadatum( \"Any Key\" ): ";
    if (!mdNullHandle)
        cout << "NULL" << endl;
    else
        cout << "ERROR!" << endl;

}

//This function tests getMetadata() called on a PmtMetadataT object.
//The function is defined as virtual in PmtMetadata but is not overrided
//in PmtMetadataT. This should cause a call to his parent's getMetadatum()
//function, which returns an empty PmtMetadataIterator.
void PmtAPITestPmtMetadataT::testGetMetadata()
{
    cout << "   -----------------------------" << endl;
    cout << "    Executing testGetMetadata() " << endl;
    cout << "   -----------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions.Aperture";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "getMetadata(\"*\").size(): " << mdHandle->getMetadata("*").
        size() << endl;
}

//Test calling deleteMetadata() from a PmtMetadataT object. Should call
//the base class. PmtMetadata, implementation, which should do nothing.
void PmtAPITestPmtMetadataT::testDeleteMetadata()
{
    cout << "   --------------------------------" << endl;
    cout << "    Executing testDeleteMetadata() " << endl;
    cout << "   --------------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions.Brightness";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "Calling deleteMetadata(\"Any Key\")..." << endl;
    mdHandle->deleteMetadata("Any Key");
    cout << "Passed test!" << endl;
}

//Test the print() function on a PmtMetadataT object.
void PmtAPITestPmtMetadataT::testPrint()
{
    cout << "   -----------------------" << endl;
    cout << "    Executing testPrint() " << endl;
    cout << "   -----------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions.SubjectDistance";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    mdHandle->setValueStr(EK_L("8.5"));
    cout << "print(): " << endl;
    mdHandle->print();
    cout << endl;
}

//This function tests clone() by creating a clone of a PmtMetadataT object,
//modifying the clone, and then verifying that the original object was not
//modified.
void PmtAPITestPmtMetadataT::testClone()
{
    cout << "   -----------------------" << endl;
    cout << "    Executing testClone() " << endl;
    cout << "   -----------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions.Flash.Fired";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    mdHandle->setValueStr(EK_L("true"));
    cout << "Original: " << endl;
    mdHandle->show();
    PmtMetadataPtr mdHandleClone = mdHandle->clone();
    cout << "Clone: " << endl;
    mdHandleClone->show();
    mdHandleClone->setValueStr(EK_L("false"));
    cout << "Modified clone: " << endl;
    mdHandleClone->show();
    cout << "Original: " << endl;
    mdHandle->show();
}

//Test show() on a PmtMetadataT object.
void PmtAPITestPmtMetadataT::testShow()
{
    cout << "   ----------------------" << endl;
    cout << "    Executing testShow() " << endl;
    cout << "   ----------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions.Aperture";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "show(): " << endl;
    mdHandle->show();
}

void PmtAPITestPmtMetadataT::testAccept()
{
    cout << "   ------------------------" << endl;
    cout << "    Executing testAccept() " << endl;
    cout << "   ------------------------" << endl;
}

void PmtAPITestPmtMetadataT::testSetFacet()
{
    cout << "   --------------------------" << endl;
    cout << "    Executing testSetFacet() " << endl;
    cout << "   --------------------------" << endl;
}

void PmtAPITestPmtMetadataT::testShowFacets()
{
    cout << "   ----------------------------" << endl;
    cout << "    Executing testShowFacets() " << endl;
    cout << "   ----------------------------" << endl;

    //PmtMetadataPtr rootMd = PmtMetadata::create();

    //char * mdKey = "CaptureConditions.ExposureIndex";
    //cout << "Metadata being tested: " << mdKey << endl;
    //PmtMetadataPtr mdHandle = rootMd->getMetadatum( mdKey );

    //PmtMetadataT<float>* mdt = dynamic_cast<PmtMetadataT<float>*>(mdHandle.ptr());
    //cout << "showFacets(): " << endl;
    //mdt->showFacets();
}

//Test type() for all types.
void PmtAPITestPmtMetadataT::testType_AllTypes()
{
    cout << "   -------------------------------" << endl;
    cout << "    Executing testType_AllTypes() " << endl;
    cout << "   -------------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "test_int8";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_vint8";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_uint8";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_vuint8";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_int16";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_vint16";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_uint16";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_vuint16";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_int32";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_vint32";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_uint32";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_vuint32";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_boolean";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_vboolean";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_float";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_vfloat";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_double";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_vdouble";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_string";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "test_vstring";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;
}

//Test trueType() for all types
void PmtAPITestPmtMetadataT::testTrueType_AllTypes()
{
    cout << "   -----------------------------------" << endl;
    cout << "    Executing testTrueType_AllTypes() " << endl;
    cout << "   -----------------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "test_int8";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_vint8";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_uint8";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_vuint8";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_int16";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_vint16";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_uint16";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_vuint16";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_int32";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_vint32";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_uint32";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_vuint32";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_boolean";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_vboolean";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_float";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_vfloat";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_double";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_vdouble";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_string";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "test_vstring";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;
}

//Test setting and getting the PmtMetadataT objects value using the
//setValueStr() and getValueStr() functions. Test for all types.
void PmtAPITestPmtMetadataT::testSetAndGetValueStr_AllTypes()
{
    cout << "   --------------------------------------------" << endl;
    cout << "    Executing testSetAndGetValueStr_AllTypes() " << endl;
    cout << "   --------------------------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "test_int8";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"-123\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("-123"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_vint8";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"-123 124 -125 126 -127\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("-123 124 -125 126 -127"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_uint8";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"123\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("123"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_vuint8";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"0 1 2 3 4 5 6 7\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("0 1 2 3 4 5 6 7"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_int16";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"-32768\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("-32768"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_vint16";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"-32768 32767 -12345 12345\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("-32768 32767 -12345 12345"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_uint16";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"65535\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("65535"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_vuint16";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"65535 0 12345 54321 1\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("65535 0 12345 54321 1"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_int32";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"-2147483648\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("-2147483648"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_vint32";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"-2147483648 2147483647 -123456 654321\" ),show():"
        << endl;
    mdHandle->setValueStr(EK_L("-2147483648 2147483647 -123456 654321"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_uint32";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"4294967295\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("4294967295"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_vuint32";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"4294967295 0 123456789 987654321\" ),show():" <<
        endl;
    mdHandle->setValueStr(EK_L("4294967295 0 123456789 987654321"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_boolean";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"true\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("true"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_vboolean";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"true false true true false false true\" ),show():"
        << endl;
    mdHandle->setValueStr(EK_L("true false true true false false true"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_float";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"12345.6\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("12345.6"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_vfloat";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"12345.6 0 0 0.111 0.222\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("12345.6 0 0 0.111 0.222"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_double";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"12345.6\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("12345.6"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_vdouble";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"12345.6 0 1.234\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("12345.6 0 1.234"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_string";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"Test String\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("Test String"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "test_vstring";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"A vector of strings\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("A vector of strings"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;
}

