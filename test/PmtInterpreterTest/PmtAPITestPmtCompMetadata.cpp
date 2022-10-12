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

#include "PmtAPITestPmtCompMetadata.h"

extern int gnThreads;

//This is the function that should be called by main to run all the 
//PmtComposite Metadata tests.
void PmtAPITestPmtCompositeMetadata::runTest()
{
    cout << "******************************************" << endl;
    cout << "* Testing PmtCompositeMetadata Functions *" << endl;
    cout << "******************************************" << endl;

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
    testGetMetadatum();
    testGetMetadata();
    testDeleteMetadata();
    testPrint();
    testClone();
    testShow();
    testAccept();
    testSetFacet();

    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}

//Call a PmtCompositeMetadata object's key() (passing nothing - default, true
//and false as arguments) and printing the results.
void PmtAPITestPmtCompositeMetadata::testKey()
{
    cout << "   ---------------------" << endl;
    cout << "    Executing testKey() " << endl;
    cout << "   ---------------------" << endl;

    char *mdKey = "CaptureConditions";

    PmtMetadataPtr rootMd = PmtMetadata::create();

    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "key(): " << mdHandle->key() << endl;
    cout << "key(true): " << mdHandle->key(true) << endl;
    cout << "key(false): " << mdHandle->key(false) << endl;
}

//Call a PmtCompositeMetadata object's fullKey() (passing nothing - default,
//true and false as arguments) and printing the results.
void PmtAPITestPmtCompositeMetadata::testFullKey()
{
    cout << "   -------------------------" << endl;
    cout << "    Executing testFullKey() " << endl;
    cout << "   -------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions.Flash";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "fullKey(): " << mdHandle->fullKey() << endl;
    cout << "fullKey(true): " << mdHandle->fullKey(true) << endl;
    cout << "fullKey(false): " << mdHandle->fullKey(false) << endl;
}

//DOESN'T COMPILE!!!
void PmtAPITestPmtCompositeMetadata::testOccurrence()
{
    cout << "   ----------------------------" << endl;
    cout << "    Executing testOccurrence() " << endl;
    cout << "   ----------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureDevice";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    //const unsigned int &x = mdHandle->occurrence();
    //cout << "occurrence(): " << mdHandle->occurrence() << endl;
}

//Test type() on PmtCompositeMetadata objects.
void PmtAPITestPmtCompositeMetadata::testType()
{
    cout << "   ----------------------" << endl;
    cout << "    Executing testType() " << endl;
    cout << "   ----------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "CaptureConditions.Flash";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "CaptureDevice";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;

    mdKey = "DigitalProcess";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "type(): " << mdHandle->type() << endl;
}

//Test that trueType() returns "composite" for PmtCompositeMetadata objects.
void PmtAPITestPmtCompositeMetadata::testTrueType()
{
    cout << "   --------------------------" << endl;
    cout << "    Executing testTrueType() " << endl;
    cout << "   --------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "CaptureConditions.Flash";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "CaptureDevice";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;

    mdKey = "DigitalProcess";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "trueType(): " << mdHandle->trueType() << endl;
}

//Call parent() on a PmtCompositeMetadata object and display the parent's
//full key.
void PmtAPITestPmtCompositeMetadata::testParent()
{
    cout << "   ------------------------" << endl;
    cout << "    Executing testParent() " << endl;
    cout << "   ------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureDevice.Scanner";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "parent()->fullKey(): " << mdHandle->parent()->fullKey() << endl;
}

//isComposite() should return true (or 1) for all PmtCompositeMetadata objects
void PmtAPITestPmtCompositeMetadata::testIsComposite()
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

//In the base class, PmtMetadata, setValueStr() is a virtual function defined
//to do nothing and getValueStr() is a virtual function that returns an empty
//string. They are not overrided in PmtCompositeMetadata, therefore it
//should behave as its parent. This is verified in this test.
void PmtAPITestPmtCompositeMetadata::testSetAndGetValueStr()
{
    cout << "   -----------------------------------" << endl;
    cout << "    Executing testSetAndGetValueStr() " << endl;
    cout << "   -----------------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"Any Value\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("Any Value"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "CaptureConditions.Flash";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"Any Value\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("Any Value"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;

    mdKey = "CaptureDevice";
    cout << "Metadata being tested: " << mdKey << endl;
    mdHandle = rootMd->getMetadatum(mdKey);
    cout << "setValueStr( \"Any Value\" ),show():" << endl;
    mdHandle->setValueStr(EK_L("Any Value"));
    mdHandle->show();
    cout << "getValueStr(): " << EK_WIDE2STR(mdHandle->getValueStr()) << endl;
}

//This function tests PmtCompositeMetadata's getMetadatum(). It tests it with
//a "simple" key (e.g. "Aperture") and with a key "path" (e.g. "Flash.Fired").
//It calls the function with the key only (second argument gets the default),
//with the key and true, and with the key and false as arguments.
void PmtAPITestPmtCompositeMetadata::testGetMetadatum()
{
    cout << "   ------------------------------" << endl;
    cout << "    Executing testGetMetadatum() " << endl;
    cout << "   ------------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions";
    char *mdKey2 = "Aperture";
    char *mdKey3 = "Flash.Fired";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);

    cout << "getMetadatum( \"" << mdKey2 << "\" )->show():" << endl;
    mdHandle->getMetadatum(mdKey2)->show();
    mdHandle->show();
    cout << "Deleting all metadata in " << mdKey << "..." << endl;
    mdHandle->deleteMetadata("*");
    cout << "show(): " << endl;
    mdHandle->show();

    cout << "getMetadatum( \"" << mdKey2 << "\", true )->show():" << endl;
    mdHandle->getMetadatum(mdKey2, true)->show();
    mdHandle->show();
    //Try to get the metadata just created, passing false to getMetadatum
    //and show the metadata.
    cout << "getMetadatum( \"" << mdKey2 << "\", false )->show():" << endl;
    mdHandle->getMetadatum(mdKey2, false)->show();
    cout << "Deleting all metadata in " << mdKey << "..." << endl;
    mdHandle->deleteMetadata("*");
    cout << "show(): " << endl;
    mdHandle->show();

    cout << "getMetadatum( \"" << mdKey2 << "\", false ),show() ";
    mdHandle->getMetadatum(mdKey2, false);
    mdHandle->show();
    cout << "Deleting all metadata in " << mdKey << "..." << endl;
    mdHandle->deleteMetadata("*");
    cout << "show(): " << endl;
    mdHandle->show();

    cout << "getMetadatum( \"" << mdKey3 << "\" )->show():" << endl;
    mdHandle->getMetadatum(mdKey3)->show();
    mdHandle->show();
    cout << "Deleting all metadata in " << mdKey << "..." << endl;
    mdHandle->deleteMetadata("*");
    cout << "show(): " << endl;
    mdHandle->show();

    cout << "getMetadatum( \"" << mdKey3 << "\", true )->show():" << endl;
    mdHandle->getMetadatum(mdKey3, true)->show();
    mdHandle->show();
    //Try to get the metadata just created, passing false to getMetadatum
    //and show the metadata.
    cout << "getMetadatum( \"" << mdKey3 << "\", false )->show():" << endl;
    mdHandle->getMetadatum(mdKey3, false)->show();
    cout << "Deleting all metadata in " << mdKey << "..." << endl;
    mdHandle->deleteMetadata("*");
    cout << "show(): " << endl;
    mdHandle->show();

    cout << "getMetadatum( \"" << mdKey3 << "\", false ),show(): " << endl;
    mdHandle->getMetadatum(mdKey3, false);
    mdHandle->show();
    cout << "Deleting all metadata in " << mdKey << "..." << endl;
    mdHandle->deleteMetadata("*");
    cout << "show(): " << endl;
    mdHandle->show();
}

//This function tests the getMetadata() when called on a PmtComposite object.
//It is tested with qualified metadata keys, wildcard metadata keys, key lists,
//etc.
void PmtAPITestPmtCompositeMetadata::testGetMetadata()
{
    cout << "   -----------------------------" << endl;
    cout << "    Executing testGetMetadata() " << endl;
    cout << "   -----------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();
    PmtMetadataIterator mdIter;

    //The blank key calls should all return the same objects regardless as
    //to the 2nd bool parameter. First create a three metadata objects and
    //then test the blank key calls.
    cout << "Creating three metadata objects..." << endl;
    rootMd->getMetadatum("CaptureConditions.Aperture");
    rootMd->getMetadatum("CaptureDevice.FID");
    rootMd->getMetadatum("DigitalProcess.ImageSource");
    cout << "getMetadata( \"\" ).size(): ";
    cout << rootMd->getMetadata("").size() << endl;
    cout << "getMetadata( \"\", true ).size(): ";
    cout << rootMd->getMetadata("", true).size() << endl;
    cout << "getMetadata( \"\", false ).size(): ";
    cout << rootMd->getMetadata("", false).size() << endl;
    cout << "getMetadata( \"*\", false ).size(): ";
    cout << rootMd->getMetadata("*", false).size() << endl;
    cout << "show(): " << endl;
    rootMd->getMetadata("").show();
    cout << "Deleting metadata objects created..." << endl;
    rootMd->deleteMetadata("*");
    if (rootMd->getMetadata("").size() != 0)
        cout << "ERROR!" << endl;

    char *mdKey = "CaptureConditions";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);

    mdIter = mdHandle->getMetadata("*");
    cout << "getMetadata(\"*\").size(): " << mdIter.size() << endl;
    cout << "getMetadata(\"*\").show(): " << endl;
    mdIter.show();
    mdHandle->deleteMetadata("*");
    if (mdHandle->getMetadata("").size() != 0)
        cout << "ERROR!" << endl;

    mdIter = mdHandle->getMetadata("*[*]");
    cout << "getMetadata(\"*[*]\").size(): " << mdIter.size() << endl;
    cout << "getMetadata(\"*[*]\").show(): " << endl;
    mdIter.show();
    mdHandle->deleteMetadata("*");
    if (mdHandle->getMetadata("").size() != 0)
        cout << "ERROR!" << endl;

    mdIter = mdHandle->getMetadata("Flash.Energy");
    if (mdIter.size() != 1)
        throw PmtError(PMT_ERROR, "Test Error!", WHERE);
    cout << "getMetadata(\"Flash.Energy\").size(): " << mdIter.size() << endl;
    cout << "getMetadata(\"Flash.Energy\").show(): " << endl;
    mdIter.show();
    mdHandle->deleteMetadata("*");
    if (mdHandle->getMetadata("").size() != 0)
        cout << "ERROR!" << endl;

    mdIter = mdHandle->getMetadata("Flash.*");
    if (mdIter.size() != 4)
        throw PmtError(PMT_ERROR, "Test Error!", WHERE);
    cout << "getMetadata(\"Flash.*\").size(): " << mdIter.size() << endl;
    cout << "getMetadata(\"Flash.*\").show(): " << endl;
    mdIter.show();
    mdHandle->deleteMetadata("*");
    if (mdHandle->getMetadata("").size() != 0)
        cout << "ERROR!" << endl;

    mdIter = mdHandle->getMetadata("F*");
    if (mdIter.size() != 7)
        throw PmtError(PMT_ERROR, "Test Error!", WHERE);
    cout << "getMetadata(\"F*\").size(): " << mdIter.size() << endl;
    cout << "getMetadata(\"F*\").show(): " << endl;
    mdIter.show();
    mdHandle->deleteMetadata("*");
    if (mdHandle->getMetadata("").size() != 0)
        cout << "ERROR!" << endl;

    PmtKeyList myList;
    myList.push_back("Aperture");
    myList.push_back("Flash.Return");
    myList.push_back("MeteringMode");
    myList.push_back("S*");
    mdIter = mdHandle->getMetadata(myList);
    cout << "getMetadata(myList).size(): " << mdIter.size() << endl;
    cout << "getMetadata(myList).show(): " << endl;
    mdIter.show();
    mdHandle->deleteMetadata("*");
    if (mdHandle->getMetadata("").size() != 0)
        cout << "ERROR!" << endl;
}

void PmtAPITestPmtCompositeMetadata::testDeleteMetadata()
{
    cout << "   --------------------------------" << endl;
    cout << "    Executing testDeleteMetadata() " << endl;
    cout << "   --------------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    char *mdKey2 = "ImageNumber";
    cout << "Calling getMetadatum( \"" << mdKey2 << "\" )..." << endl;
    PmtMetadataPtr mdHandle2 = mdHandle->getMetadatum(mdKey2);
    cout << "show(): " << endl;
    mdHandle->show();
    cout << "Calling deleteMetadata( \"" << mdKey2 << "\" )..." << endl;
    mdHandle->deleteMetadata(mdKey2);
    cout << "show(): " << endl;
    mdHandle->show();
}

//Test the print() function on a PmtCompositeMetadata object.
void PmtAPITestPmtCompositeMetadata::testPrint()
{
    cout << "   -----------------------" << endl;
    cout << "    Executing testPrint() " << endl;
    cout << "   -----------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "print(): " << endl;
    mdHandle->print();
    cout << endl;
}

//This function tests clone() by creating a clone of a PmtCompositeMetadata
//object, modifying the clone, and then verifying that the original object
//was not modified.
void PmtAPITestPmtCompositeMetadata::testClone()
{
#if 0
    cout << "   -----------------------" << endl;
    cout << "    Executing testClone() " << endl;
    cout << "   -----------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureDevice";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "Original: " << endl;
    mdHandle->show();
    PmtMetadataPtr mdHandleClone = mdHandle->clone();
    cout << "Clone: " << endl;
    mdHandleClone->show();
    mdHandleClone->set
        PmtMetadataIterator mdIterCloneChildren =
        mdHandleClone->getMetadata("*");
    cout << "Modified clone: " << endl;
    mdHandleClone->show();
    cout << "Original: " << endl;
    mdHandle->show();
#endif
}

//Test show() with a PmtCompositeMetadata object.
void PmtAPITestPmtCompositeMetadata::testShow()
{
    cout << "   ----------------------" << endl;
    cout << "    Executing testShow() " << endl;
    cout << "   ----------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureConditions.Flash";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "show(): " << endl;
    mdHandle->show();
}

void PmtAPITestPmtCompositeMetadata::testAccept()
{
    cout << "   ------------------------" << endl;
    cout << "    Executing testAccept() " << endl;
    cout << "   ------------------------" << endl;
}

//setFacet() is a virtual function in the base class PmtMetadata and is defined
//to do nothing. It is not overrided in PmtCompositeMetadata, therefore it
//should do nothing here.
void PmtAPITestPmtCompositeMetadata::testSetFacet()
{
    cout << "   --------------------------" << endl;
    cout << "    Executing testSetFacet() " << endl;
    cout << "   --------------------------" << endl;

    PmtMetadataPtr rootMd = PmtMetadata::create();

    char *mdKey = "CaptureDevice.CameraOwner";
    cout << "Metadata being tested: " << mdKey << endl;
    PmtMetadataPtr mdHandle = rootMd->getMetadatum(mdKey);
    cout << "Calling setFacet(\"A string\",\"A string\",\"A string\")..." <<
        endl;
    cout << "show(): " << endl;
    mdHandle->show();
}

