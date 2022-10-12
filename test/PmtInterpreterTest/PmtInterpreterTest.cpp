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
 * Copyright (C) 1999-2000 Eastman Kodak Company. All 
 * Rights Reserved. 
 * 
 * Original Author: 
 * Rupe, Dan  rupe@kodak.com
 * 
 * Contributor(s): 
 * Duda, Diane J. 970704N@knotes.kodak.com
 * Hailey, Keith    hailey@kodak.com
 * Kundan, Upadhayay kundan.upadhyay@kodak.com
 * Sam Fryer  samuel.fryer@kodak.com
 *
 * Date: July 21, 2000
 * Last modified: September 18, 2002
*/

/*****************************************************************************/
/* Test the LogicalDefinitionInterpreter and the LogicalInstanceInterpreter. */
/*****************************************************************************/

/* 
 * Program Usage:
 * interpTest [ -i numIterations ] [ -t numThreads ]
 * where numIterations is the number of times each test is run per thread
 * and numThreads is the number of threads run.
 * Default for both values is 1.
*/

/****************************************************************/
/* This test program may be built different ways.  Just use the */
/* appropriate #define value.                                   */
/****************************************************************/
// Define if you want to enable timing analysis
// #define BUILD_WITH_TIMING_SUPPORT


// The max number of threads the user can specify
#define MAX_THREADS 1024

/********************************/
/* End of build #define values. */
/********************************/

// -------------------

#ifdef _MSC_VER
#define STRICT 1
#pragma warning (disable:4786)
#pragma warning (disable:4503)
#endif

// System Includes
#include <cstdio>
#include <time.h>

// PMT Includes
#include "PmtError.h"
#include "PmtInstanceInterpreter.h"
#include "PmtMetadataT.h"
#include "PmtAttributeT.h"

#include "PmtAPITestPmtMetadata.h"
#include "PmtAPITestPmtMetadataT.h"
#include "PmtAPITestPmtCompMetadata.h"
#include "../Common/PmtLocalDefs.h"
#include "../Common/PmtLocalAliases.h"
#include "../Common/times.h"

// Needed for multi-platform threading.
#include "EkThread.h"

#include "PmtMisc.h"


// Constant File Names

const char *const MAIN_EXAMPLE_SCHEMA_INFO_FILE = "PmtMetadataSchemaInfo.xml";
const char *const TEST_FILE = "PmtInterpreterTest.xml";
const char *const TEST_FILE_COPY = "PmtInterpreterTestCopy";
const char *const ALIASES_FILE = "Aliases.xml";
const char *const FACET_SCHEMA_INFO_FILE = "FacetInfo.xml";
const char *const FACET_INSTANCE_DOC = "Facet.xml";
const char *const ALL_TYPES_INFO_FILE = "AllTypesInfo.xml";
const char *const ALL_TYPES_FILE = "AllTypes.xml";
const char *const ALL_TYPES_FILE_COPY = "AllTypesCopy.xml";
const char *const ALL_TYPES_FILE_COPY2 = "AllTypesCopy2.xml";
const char *const ALL_TYPES_FILE_WRITE_COPY = "AllTypesWriteCopy.xml";
const char *const MIN_MAX_OCCURS_INFO_FILE = "MinMaxOccursInfo.xml";
const char *const MIN_MAX_OCCURS_FILE = "MinMaxOccurs.xml";
const char *const MIN_MAX_OCCURS_FILE_COPY = "MinMaxOccursCopy.xml";
const char *const REF_INFO_FILE = "RefInfo.xml";
const char *const REF_FILE = "Ref.xml";
const char *const OCCURRENCES_INFO_FILE = "OccurrencesInfo.xml";
const char *const SIMPLE_TYPE_INFO_FILE = "SimpleTypeInfo.xml";
const char *const PRINT_QUANTITY_INFO_FILE = "PrintQuantityInfo.xml";
const char *const EXIF_PERSIST_ALIASES_FILE = "ExifPersistAliases.xml";
const char *const LOCAL_INFO_FILE = "../Common/LocalInfo.xml";
const char *const LOCAL_ALIASES_FILE = "../Common/LocalAliases.xml";
const char *const MODEL_GROUPS_INFO_FILE = "ModelGroupsInfo.xml";
const char *const SIM_CONT_INFO_FILE = "CompTypeSimpleContentInfo.xml";
const char *const LOCAL_INSTANCE_DOC = "LocalInstanceDoc.xml";
const char *const COMPLEX_CONT_INFO_FILE = "CompTypeComplexContentInfo.xml";
const char *const ATTRIBUTE_FACET_FILE = "AttFacet.xml";
const char *const ATTRIBUTE_FACET_COPY = "AttFacetCopy.xml";
const char *const INCLUDE_INFO_FILE = "IncludeInfo.xml";
const char *const IMPORT_INFO_FILE = "ImportInfo.xml";



// Function Definitions

void ShowFactory();
void testDefault();
void testOccurrences();
void testAliases();
void testSchemaInfoFile();
void testDelete();
void testAllTypes();
void testMinMaxOccurs();
void testRef();
void testSimpleType();
void testSimpleTypeFacets();
void testMiscellaneous();
void testFalseCreate();
void testExifPersist();
void testLocal();
void testAttrFacet();
void testModelGroups();
void testCompTypeSimpCont();
void runAPITest();
void testLoadMemory();
void testCompTypeComplexCont();
void testGetNoCreate(PmtMetadataPtr & root);
void testWriteNoRead();
void testAttribConstructs();
void testIncludeTag();
void testImportTag();

// Global Variables
int gnThreads = 1;              // Number of threads we're running
int gnIterations = 1;           // Number of iterations per thread
int gnExitedWithError = 0;      // Number of exceptions caught
EK_DEFAULT_MUTEX gThreadMutex;  // global mutex 

// Example Mutex use (simply put at top of the block of code):  
//EK_GUARD(EK_DEFAULT_MUTEX, gThreadMutex);

// function to run all the tests, including timing and catching exceptions
void runTests()
{
    try
    {
#ifdef BUILD_WITH_TIMING_SUPPORT
        TimeVal begin;
        GetTime(begin);
#endif                          /* def BUILD_WITH_TIMING_SUPPORT */



        // go through each test n times
        for (int iterations = 0; iterations != gnIterations; ++iterations)
        {
            // Output date and time for each iteration
            time_t myTime;
            struct tm *timeStruct;
            time(&myTime);
            timeStruct = localtime(&myTime);
            cout << "Date and time is: " << asctime(timeStruct) << endl;

            // Run the tests
            testDefault();
            testOccurrences();
            testAliases();
            testSchemaInfoFile();
            testDelete();
            testAllTypes();
            testMinMaxOccurs();
            testRef();
            testSimpleType();
            //testSimpleTypeFacets();
            testMiscellaneous();
            testFalseCreate();
            testExifPersist();
            testLocal();
            testAttrFacet();
            testWriteNoRead();
            //testModelGroups();
            testCompTypeSimpCont();
            runAPITest();
            testLoadMemory();
            testCompTypeComplexCont();

            testIncludeTag();
            testImportTag();

        }

#ifdef BUILD_WITH_TIMING_SUPPORT
        TimeVal end;
        GetTime(end);
        TimeVal freq;
        GetFrequency(freq);
#endif                          /* def BUILD_WITH_TIMING_SUPPORT */

        cout << endl << "... end of test" << endl;

#ifdef BUILD_WITH_TIMING_SUPPORT
        cout << "begin: " << begin << endl;
        cout << "end:   " << end << endl;
        double t = GetDifference(begin, end, freq);

        cout << "Time to execute: " << t << " seconds" << endl;
        cout << "Time per iteration: " << (t /
                                           iterations) << " seconds" << endl;
#endif                          /* def BUILD_WITH_TIMING_SUPPORT */
    }
    catch(PmtError & e)
    {
        cout << "PmtError encountered : ";
        cout << e.getMsg();
        gnExitedWithError++;
    }
    catch(...)
    {
        cout << "GENERAL EXCEPTION CAUGHT!!" << endl;
        gnExitedWithError++;
    }
};

//////////////////////////////////////////////////////////////////////

// NOTE: Use of EkThread is not supported by Pmt. Just using for testing purposes.
// Our class for running the testing threads
class TestThread:public EkThread < EK_DEFAULT_MUTEX >
{
  public:
    virtual void run(void);
};

//////////////////////////////////////////////////////////////////////

// The routine that actually gets run in each thread nb
void TestThread::run(void)
{
    runTests();
    return;
}

//////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{

    int i = 0;
    double t0=0, t1=0;
    CPUDEFS;
    t0 = CPUTIME;

    // Create pointers for all the possible threads
    EkSmartPtr < TestThread > child[MAX_THREADS];

    // Read the input arguements
    if (argc >= 2)
    {
        for (i = 1; i < argc; i++)
        {
            if ((!strcmp(argv[i], "-t")) && (i < argc - 1))
                gnThreads = atoi(argv[++i]);
            else if ((!strcmp(argv[i], "-i")) && (i < argc - 1))
                gnIterations = atoi(argv[++i]);
            else if ((!strcmp(argv[i], "-?")) || (!strcmp(argv[i], "-h")))
            {
                cout << " Usage:\n"
                    << argv[0] << " [ -i numIterations ] [ -t numThreads ]\n";
                exit(0);
            }
        }
    }

    // Make sure we're not over our limit
    if (gnThreads > MAX_THREADS)
        gnThreads = MAX_THREADS;

    // If there's only 1 thread, we don't need all the overhead.
    if (gnThreads == 1)
        runTests();
    else
    {
        // create and run each thread
        for (i = 0; i < gnThreads; i++)
        {
            child[i] = new TestThread;
            child[i]->launch();
        }

        // wait for each thread to exit
        for (i = 0; i < gnThreads; i++)
            EkThread < EK_DEFAULT_MUTEX >::waitForThread(child[i]->getThreadId());
    }

    // print out the number exceptions caught, if any
    if (gnExitedWithError > 0)
    {
        cout << "\n" << gnExitedWithError
            << " exceptions were caught during testing.\n\n";
    }

    t1 = CPUTIME;
    if (gnThreads > 1 )
    {
        t1 += CHILDCPUTIME;
    }

    cout << "Time: " << t1 - t0 << " seconds" << endl;

    return 0;
}

//////////////////////////////////////////////////////////////////////

void testDefault()
{
    cout << "------------------------" << endl;
    cout << " Executing testDefault()" << endl;
    cout << "------------------------" << endl;

    // Test the Logical Definition Interpreter.
    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load();
    delete definitionInterpreter;
    ShowFactory();

    // Make a copy for each thread...
    char testFileCopyFilename[1024];
    sprintf(testFileCopyFilename, "%s.%d.xml", TEST_FILE_COPY, getpid());
    PmtCopyFile(TEST_FILE, testFileCopyFilename, true);

    PmtMetadataPtr root;
    PmtMetadataPtr captureConditions;

    void testGetMetadatum(PmtMetadataPtr & root,
                          PmtMetadataPtr & captureConditions);
    testGetMetadatum(root, captureConditions);

    void testGetMetadata(PmtMetadataPtr & root);
    testGetMetadata(root);

    void testGetMetadataList(PmtMetadataPtr & root);
    testGetMetadataList(root);

    // Test the Logical Instance Interpreter
    PmtLogicalInstanceInterpreter *instanceInterpreter =
        new PmtLogicalInstanceInterpreter();

    if (instanceInterpreter->open(testFileCopyFilename))
    {
        PmtMetadataPtr md;

        cout << "--------------------" << endl;
        cout << " Test readMetadata()" << endl;
        cout << "--------------------" << endl;
        
        
        // CaptureConditions
        float floatValue;
        instanceInterpreter->readMetadata(captureConditions);

        md = captureConditions->getMetadatum("Aperture");
        floatValue = PMT_CAST_TYPE(md, float)->value();

        if (floatValue != 2.222F)
            throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
        md->setValueStr(EK_L("7.777"));

        md = captureConditions->getMetadatum("BatteryLevel");
        floatValue = PMT_CAST_TYPE(md, float)->value();
        if (floatValue != 3.333F)
            throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
        md->setValueStr(EK_L("8.888"));

        instanceInterpreter->writeMetadata(captureConditions);

        // CaptureDevice
        string stringValue;
        PmtMetadataPtr captureDevice = root->getMetadatum("CaptureDevice");

        md = captureDevice->getMetadatum("CameraOwner.CameraOwner");
        instanceInterpreter->readMetadata(md);
        stringValue = PMT_CAST_TYPE(md, string)->value();
        if (stringValue != "Original camera owner")
            throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
        instanceInterpreter->writeMetadata(md);

        md = captureDevice->getMetadatum("FilmGencode");
        instanceInterpreter->readMetadata(md);
        instanceInterpreter->writeMetadata(md);

        md = captureDevice->getMetadatum("FilmProductCode");
        instanceInterpreter->readMetadata(md);
        instanceInterpreter->writeMetadata(md);

        md = captureDevice->getMetadatum("Interlace");
        instanceInterpreter->readMetadata(md);
        unsigned short shortValue = PMT_CAST_TYPE(md, unsigned short)->value();
        if (shortValue != 44444)
            throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
        md->setValueStr(EK_L("65535"));
        instanceInterpreter->writeMetadata(md);


        // Make sure NULL is handled OK.
        instanceInterpreter->writeMetadata(NULL);

        instanceInterpreter->close();
    }
    else
        cout << "Could not open file" << endl;

    if (instanceInterpreter)
        delete instanceInterpreter;

    // Static method, so don't need to keep a PmtLogicalDefinitionInterpreter *
    // around (the definitionInterpreter pointer was deleted above) to call erase().
    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}


void testGetMetadatum(PmtMetadataPtr & root,
                      PmtMetadataPtr & captureConditions)
{
    cout << endl;
    cout << "-----------------------------" << endl;
    cout << " Executing testGetMetadatum()" << endl;
    cout << "-----------------------------" << endl;
    cout << endl;

    PmtMetadataPtr md;
    PmtMetadataIterator mdIter;

    // To start out with the Logical Instance Interpreter, first obtain the
    // root element associated with the loaded schema.
    // This root element metadata item will be used to call through, to work
    // with all the other metadata items logically contained inside it.
    root = PmtMetadata::create();

    // This shows how to fill in a particular metadata item by calling
    // getMetadatum() with keys that match the tagnames in the XML file.
    cout << "--------------------" << endl;
    cout << " Test getMetadatum()" << endl;
    cout << "--------------------" << endl << endl;
    PmtMetadataPtr fired;
    captureConditions = root->getMetadatum("CaptureConditions");
    captureConditions->getMetadatum("Aperture");
    captureConditions->getMetadatum("ImageNumber");
    mdIter = captureConditions->getMetadata("");
    if (mdIter.size() != 2)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    captureConditions->show();
    cout << endl;

    // Can obtain an element with a key "path", which is relative
    // to the metadata item called through.  In this case, "Flash.Fired"
    // is two levels below "CaptureConditions", which is the metadata
    // item we're calling through via "captureConditions->".
    // Any number of levels may be specified in the path, as long as
    // the path is relative to the metadata item being called through.
    fired = captureConditions->getMetadatum("Flash.Fired");
    mdIter = captureConditions->getMetadata("");
    if (mdIter.size() != 3)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    fired->show();
    cout << endl;

    // Test re-getting metadata pointer that has already been obtained.
    PmtMetadataPtr flash;
    flash = captureConditions->getMetadatum("Flash");
    // Can also obtain an element one level under "Flash" this way
    // (instead of calling with a key path via the captureConditions pointer).
    // Notice the drilling down one level (from CaptureConditions) with use of the
    // "flash->" pointer.
    flash->getMetadatum("Energy");
    mdIter = flash->getMetadata("");
    if (mdIter.size() != 2)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    flash->show();
    cout << endl;

    // Make sure some bogus keys just return NULL.
    md = root->getMetadatum(" ");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("   ");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum(".");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum(" .bogus");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("bogus");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("bogus key with spaces");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("bogus.key.with.dots");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->
        getMetadatum("bogus[2].key.with[0].occurrence.specifcations[*]");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("bogus.key.with.wildcard*");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("bogus*.key.with?.wildcards");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("bogus*.key.with*.wildcards.*");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("Cap*.*");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("CaptureConditions.*.*");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("CaptureConditions.*.?");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

}


void testGetMetadata(PmtMetadataPtr & root)
{
    PmtMetadataIterator mdIter;

    cout << endl;
    cout << "----------------------------" << endl;
    cout << " Executing testGetMetadata()" << endl;
    cout << "----------------------------" << endl;
    cout << endl;

    cout << "---------------------" << endl;
    cout << " Test getMetadata(\"\")" << endl;
    cout << "---------------------" << endl << endl;

    // The blank key calls should all return the same objects, regardless as to the 2nd bool parameter.

    mdIter = root->getMetadata("");
    if (mdIter.size() != 4)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    mdIter = root->getMetadata("", false);
    if (mdIter.size() != 4)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    mdIter = root->getMetadata("", true);
    if (mdIter.size() != 4)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    cout << "-------------------------" << endl;
    cout << " Test getMetadata(\"*[*]\")" << endl;
    cout << "-------------------------" << endl << endl;

    mdIter = root->getMetadata("*[*]", false);
    if (mdIter.size() != 4)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    cout << "----------------------" << endl;
    cout << " Test getMetadata(\"*\")" << endl;
    cout << "----------------------" << endl << endl;

    PmtMetadataPtr sceneContent = root->getMetadatum("SceneContent");
    mdIter = sceneContent->getMetadata("*");
    if (mdIter.size() != 63)
    {
        cout << "Size : " << mdIter.size() << "  Expected : " << 62 << endl; 
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    }
    mdIter.show();
    cout << endl;

    cout << "----------------------------------------------------------" <<
        endl;
    cout << " Test getMetadata(\"CaptureDevice.CameraOwner.CameraOwner\")" <<
        endl;
    cout << "----------------------------------------------------------" <<
        endl << endl;

    mdIter = root->getMetadata("CaptureDevice.CameraOwner.CameraOwner");
    if (mdIter.size() != 1)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();

    cout << endl;
    cout << "----------------------------" << endl;
    cout << " Test getMetadata(\"Output*\")" << endl;
    cout << "----------------------------" << endl << endl;

    mdIter = root->getMetadata("Output*");
    if (mdIter.size() != 4)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    cout << "------------------------------------------" << endl;
    cout << " Test getMetadata(\"DigitalProcess.Jpeg.*\")" << endl;
    cout << "------------------------------------------" << endl << endl;

    mdIter = root->getMetadata("DigitalProcess.Jpeg.*");
    if (mdIter.size() != 11)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    cout << "----------------------------------------" << endl;
    cout << " Test getMetadata(\"SceneContent.Image*\")" << endl;
    cout << "----------------------------------------" << endl << endl;

    mdIter = root->getMetadata("SceneContent.Image*");
    if (mdIter.size() != 4)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    cout << "-------------------------" << endl;
    cout << " Test getMetadata(\"Cap*\")" << endl;
    cout << "-------------------------" << endl << endl;

    mdIter = root->getMetadata("Cap*");
    // Note -- if this is a second, or later iteration, with multi-threads,
    // there will be 70 due to aliases introduced in testAlias
    if ((mdIter.size() == 67) ||
        (((gnIterations > 1) || (gnThreads > 1)) && (mdIter.size() == 73)))
    {
        mdIter.show();
        cout << endl;
    }
    else
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    cout << "----------------------------------------" << endl;
    cout << " Test getMetadata(\"CaptureConditions.?\")" << endl;
    cout << "----------------------------------------" << endl << endl;

    mdIter = root->getMetadata("CaptureConditions.?");
    if (mdIter.size() != 33)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    cout << "------------------------------------------" << endl;
    cout << " Test getMetadata(\"DigitalProcess.Jpeg.?\")" << endl;
    cout << "------------------------------------------" << endl << endl;

    mdIter = root->getMetadata("DigitalProcess.Jpeg.?");
    if (mdIter.size() != 4)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    cout << "------------------------------------" << endl;
    cout << " Test getMetadata(\"SceneContent.S?\")" << endl;
    cout << "------------------------------------" << endl << endl;

    mdIter = root->getMetadata("SceneContent.S?");
    if (mdIter.size() != 3)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    cout << "--------------------------------------" << endl;
    cout << " Test getMetadata(\"SceneContent.I?.?\")" << endl;
    cout << "--------------------------------------" << endl << endl;

    mdIter = root->getMetadata("SceneContent.I?.?");
    if (mdIter.size() != 4)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;


    cout << "----------------" << endl;
    cout << " Test bogus keys" << endl;
    cout << "----------------" << endl << endl;

    // Make sure some bogus keys just return NULL.
    mdIter = root->getMetadata(" ");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter = root->getMetadata("   ");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter = root->getMetadata(".");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter = root->getMetadata(" .bogus");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter = root->getMetadata("bogus");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter = root->getMetadata("bogus key with spaces");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter = root->getMetadata("bogus.key.with.dots");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter =
        root->getMetadata("bogus[2].key.with[0].occurrence.specifcations[*]");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter = root->getMetadata("bogus.key.with.wildcard*");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter = root->getMetadata("bogus*.key.with?.wildcards");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter = root->getMetadata("bogus*.key.with*.wildcards.*");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter = root->getMetadata("Cap*.*");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter = root->getMetadata("CaptureConditions.*.*");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter = root->getMetadata("CaptureConditions.*.?");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    cout << "Bogus keys tested ok." << endl;
}


void testGetMetadataList(PmtMetadataPtr & root)
{
    cout << endl;
    cout << "------------------------------" << endl;
    cout << " Executing testGetMetadaList()" << endl;
    cout << "------------------------------" << endl;
    cout << endl;

    PmtMetadataIterator mdIter;

    cout << "--------------------------------------------" << endl;
    cout << " Test getMetadata(const PmtKeyList& keyList)" << endl;
    cout << "--------------------------------------------" << endl << endl;

    PmtKeyList myList;
    myList.push_back("DigitalProcess.CellLength");
    myList.push_back("DigitalProcess.CellWidth");
    myList.push_back("OutputOrder.Information.EnvelopeNumber");
    myList.push_back("Capture*");
    mdIter = root->getMetadata(myList);
    // Test for 2 sizes, since this is called without and with aliases loaded.
    if ((mdIter.size() != 70) && (mdIter.size() != 77))
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;
}


//////////////////////////////////////////////////////////////////////

void testOccurrences()
{
    cout << endl;
    cout << "----------------------------" << endl;
    cout << " Executing testOccurrences()" << endl;
    cout << "----------------------------" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load(OCCURRENCES_INFO_FILE);
    delete definitionInterpreter;
    ShowFactory();

    PmtMetadataPtr root = PmtMetadata::create();
    PmtMetadataPtr md;
    PmtMetadataIterator mdIter;

    // Verify one PmtCompositeMetadata object is returned.
    md = root->getMetadatum("Five");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    // Verify that no PmtCompositeMetadata object is returned.
    mdIter = root->getMetadata("Five");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    // Verify one PmtMetadataT object is returned.
    md = root->getMetadatum("Five.A");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md->show();
    cout << endl;

    md = 0;
    mdIter.clear();
    root->deleteMetadata("*");
    mdIter = root->getMetadata("");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    // Verify one object is returned, with no occurrence specification.
    md = root->getMetadatum("One");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md->show();
    cout << endl;

    md = 0;
    mdIter.clear();
    root->deleteMetadata("*");
    mdIter = root->getMetadata("");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    // Verify one object is returned, with occurrence specification.
    mdIter = root->getMetadata("One[3]");
    if (mdIter.size() != 1)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    root->show();
    cout << endl;

    // Verify objects before [3] were created too.
    mdIter = root->getMetadata("*[*]", false);
    if (mdIter.size() != 4)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    // Verify object between [3] and [7] created too.
    root->getMetadata("One[7]");
    mdIter = root->getMetadata("*[*]", false);
    // Set values to verify correct occurrence goes away when we delete one in the middle.
    char counter = 'a';
    md = mdIter.start();
    while (md)
    {
        PMT_CAST_TYPE(md, char)->value() = counter++;
        md = mdIter.next();
    }
    if (mdIter.size() != 8)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    // Delete an occurrence, causing subsequent occurrences to collapse
    // into the previous occurrence's spots.
    md = 0;
    mdIter.clear();
    root->deleteMetadata("One[2]");
    mdIter = root->getMetadata("*[*]", false);
    if (mdIter.size() != 7)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    // Delete everything, and ensure it worked.
    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");
    mdIter = root->getMetadata("*[*]", false);
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    // Get an object, whose min is greater than 1, verifying
    // the min number of objects were created.
    mdIter = root->getMetadata("Two");
    if (mdIter.size() != 1)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter = root->getMetadata("*[*]", false);
    if (mdIter.size() != 3)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");

    // Test occurrences on a sub-object.
    md = root->getMetadatum("Five");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter = md->getMetadata("B[9]");
    if (mdIter.size() != 1)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter = md->getMetadata("*[*]", false);
    if (mdIter.size() != 10)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");

    // Verify cannot create occurrence greater than explicitly specified max.
    md = root->getMetadatum("Five");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter = md->getMetadata("B[10]");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    // Verify cannot create occurrence greater than default max.
    mdIter = md->getMetadata("A[1]");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    // Test that wildcard creates maxOccurs number of elements.
    mdIter = root->getMetadata("Big.Two[*]");
    if (mdIter.size() != 5)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    // Test unbounded case when there are no instantiations, so we expect 1 occurrence
    // to get created.
    mdIter = root->getMetadata("Big.Three[*]");
    if (mdIter.size() != 1)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    // Test unbounded case when there are already some instantiations, so we expect
    // the existing ones to get returned.
    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");
    md = root->getMetadatum("Big.Three[0]");
    md = root->getMetadatum("Big.Three[1]");
    md = root->getMetadatum("Big.Three[2]");
    mdIter = root->getMetadata("Big.Three[*]");
    if (mdIter.size() != 3)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");
    mdIter = root->getMetadata("Big[1].*");
    mdIter.show();
    cout << endl;
    root->show(false, 0, true);
    cout << endl;

    mdIter = root->getMetadata("Big[*].*", false);
    mdIter.show();
    cout << endl;

    // Test combinations of occurrences with multiply-contained elements and various
    // occurrence specifications.
    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");
    mdIter = root->getMetadata("Big[3].Two[2]");
    if (mdIter.size() != 1)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");

    mdIter.clear();
    mdIter = root->getMetadata("Big[3].Four[2].*[*]");
    cout << "---------------";
    cout << "size=" << mdIter.size();
    if (mdIter.size() != 440)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");
    md = root->getMetadatum("Five");
    mdIter = md->getMetadata("B[1-3, 7-10, 5]");    // does not get 10th
    if (mdIter.size() != 7)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");
    md = root->getMetadatum("Big.Four");
    mdIter = md->getMetadata("Y[0, 2, 3-4].*");
    if (mdIter.size() != 28)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    // While in the middle of doing various gets/deletes, ensure
    // this delete really delete's everything.
    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");
    mdIter = root->getMetadata("*[*]", false);
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    md = root->getMetadatum("Big.Four");
    mdIter = md->getMetadata("Y.?");
    if (mdIter.size() != 1)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");
    md = root->getMetadatum("Big.Four.Y");
    mdIter = md->getMetadata("?");
    if (mdIter.size() != 1)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");
    md = root->getMetadatum("Big.Four.Y");
    mdIter = md->getMetadata("?.?");
    if (mdIter.size() != 5)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");
    md = root->getMetadatum("Big.Four.Y");
    mdIter = md->getMetadata("I?");
    if (mdIter.size() != 1)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");
    md = root->getMetadatum("Big.Four.Y");
    mdIter = md->getMetadata("I?.?");
    if (mdIter.size() != 5)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");
    md = root->getMetadatum("Big.Four.Y");
    mdIter = md->getMetadata("I?.?.?");
    if (mdIter.size() != 7)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");
    md = root->getMetadatum("Big.Four.Y");
    mdIter = md->getMetadata("I?.?.*");
    if (mdIter.size() != 7)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");
    md = root->getMetadatum("Big.Four.Y");
    mdIter = md->getMetadata("I?.*");
    if (mdIter.size() != 7)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    md = 0;
    mdIter.clear();
    root->deleteMetadata("*[*]");
    md = root->getMetadatum("Big.Four");
    mdIter = md->getMetadata("?.I*");
    if (mdIter.size() != 10)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    // Verify that "*[*], false" and a blank key return the same thing.
    // There are 10 items that exist due to the last getMetadata() call.
    // Compare the keys returned from these 2 calls, just as extra verification
    // that things worked ok.
    mdIter = root->getMetadata("*[*]", false);
    if (mdIter.size() != 12)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    SPmtString first;
    md = mdIter.start();
    while (md)
    {
        first = first + md->fullKey(true);
        md = mdIter.next();
    }
    mdIter.show();
    cout << endl;

    mdIter = root->getMetadata("");
    if (mdIter.size() != 12)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    SPmtString second;
    md = mdIter.start();
    while (md)
    {
        second = second + md->fullKey(true);
        md = mdIter.next();
    }
    mdIter.show();
    cout << endl;

    if (first != second)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    // Get all occurrences of everything.
    mdIter = root->getMetadata("*[*]");

    // If we're running in a multi-threaded mode, the number will be different
    // due to the fact that we didn't do an "erase" previously
    if (((gnThreads <= 1) && (mdIter.size() != 7093)) ||
        ((gnThreads > 1) && (mdIter.size() < 6169)))
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    // Verify we get the same number of items back with blank key, now
    // that everything has already been instantiated.
    mdIter = root->getMetadata("");
    if (((gnThreads <= 1) && (mdIter.size() != 7093)) ||
        ((gnThreads > 1) && (mdIter.size() < 6169)))
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    // Verify deleteMetadata with a blank key works.
    md = 0;
    mdIter.clear();
    root->deleteMetadata("");
    mdIter = root->getMetadata("");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}

//////////////////////////////////////////////////////////////////////

void testAliases()
{
    cout << endl;
    cout << "------------------------" << endl;
    cout << " Executing testAliases()" << endl;
    cout << "------------------------" << endl;

    // Test the Logical Definition Interpreter.
    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->loadWithAliases(ALIASES_FILE);
    delete definitionInterpreter;
    ShowFactory();


    PmtMetadataIterator mdIter;
    PmtMetadataPtr root = PmtMetadata::create();

    testGetNoCreate(root);

    cout << "----------------------------------" << endl;
    cout << " Test getMetadata(\"FirstAliasKey\")" << endl;
    cout << "----------------------------------" << endl << endl;

    mdIter = root->getMetadata("FirstAliasKey");
    if (mdIter.size() != 15)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    cout << "-----------------------------------" << endl;
    cout << " Test getMetadata(\"SecondAliasKey\")" << endl;
    cout << "-----------------------------------" << endl << endl;

    mdIter = root->getMetadata("SecondAliasKey", PMT_ALIAS_KEYS_ONLY);
    if (mdIter.size() != 1)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    cout << "-----------------------------" << endl;
    cout << " Test getMetadata(\"Capture*\")" << endl;
    cout << "-----------------------------" << endl << endl;

    mdIter = root->getMetadata("Capture*");
    cout << "Capture size : " << mdIter.size() << endl;
    if (mdIter.size() != 74)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    testGetMetadataList(root);

    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}


void testGetNoCreate(PmtMetadataPtr & root)
{
    cout << endl;
    cout << "----------------------------" << endl;
    cout << " Executing testGetNoCreate()" << endl;
    cout << "----------------------------" << endl;
    cout << endl;

    PmtMetadataIterator mdIter;
    PmtMetadataPtr md;

    // First, create some metadata to subsequently test getting with no instantiations.

    root->getMetadata("DigitalProcess.CellLength");
    root->getMetadata("DigitalProcess.ICCProfile");
    root->getMetadata("DigitalProcess.ImageFileCreationDateTime");
    root->getMetadata("DigitalProcess.ImageSource");
    root->getMetadata("DigitalProcess.Jpeg.AlphaQTable");
    root->getMetadata("DigitalProcess.Jpeg.Chroma2QTable");
    root->getMetadata("DigitalProcess.RollGuid");
    root->getMetadata("DigitalProcess.Thresholding");
    // To test the DigitalAlias below, that gets matched with the Digital* wildcard,
    // if the alias is loaded.
    root->getMetadata("ImageContainer.ActualImageHeight");
    root->getMetadata("ImageContainer.ActualImageWidth");

    cout << "------------------------------------------------------" << endl;
    cout << " Test getMetadatum(\"DigitalProcess.CellLength\", false)" << endl;
    cout << "------------------------------------------------------" << endl <<
        endl;

    md = root->getMetadatum("DigitalProcess.CellLength", false);
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md->show();
    cout << endl;

    cout << "-----------------------------------------------------" << endl;
    cout << " Test getMetadatum(\"DigitalProcess.CellWidth\", false)" << endl;
    cout << "-----------------------------------------------------" << endl <<
        endl;

    md = root->getMetadatum("DigitalProcess.CellWidth", false);
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    cout <<
        "Correctly did not obtain a metadata pointer to DigitalProcess.CellWidth."
        << endl;
    cout << endl;

    cout << "--------------------------------------------" << endl;
    cout << " Test getMetadata(\"DigitalProcess.*\", false)" << endl;
    cout << "--------------------------------------------" << endl << endl;

    mdIter = root->getMetadata("DigitalProcess.*", false);
    if (mdIter.size() != 8)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    cout << "------------------------------------" << endl;
    cout << " Test getMetadata(\"Digital*\", false)" << endl;
    cout << "------------------------------------" << endl << endl;

    mdIter = root->getMetadata("Digital*", false);
    if (mdIter.size() != 10)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    cout << "---------------------------------------------" << endl;
    cout << " Test getMetadata(\"DigitalProcess.I*\", false)" << endl;
    cout << "---------------------------------------------" << endl << endl;

    mdIter = root->getMetadata("DigitalProcess.I*", false);
    if (mdIter.size() != 3)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;
}


//////////////////////////////////////////////////////////////////////

void testSchemaInfoFile()
{
    cout << endl;
    cout << "-------------------------------" << endl;
    cout << " Executing testSchemaInfoFile()" << endl;
    cout << "-------------------------------" << endl;

    // Test the Logical Definition Interpreter.
    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load(MAIN_EXAMPLE_SCHEMA_INFO_FILE);
    delete definitionInterpreter;
    ShowFactory();

    PmtMetadataPtr root = PmtMetadata::create();
    PmtMetadataIterator mdIter;
    PmtMetadataPtr md;

    root->getMetadata("CaptureConditions.Aperture");
    mdIter = root->getMetadata("CaptureConditions.Apertu*", false);
    if (mdIter.size() != 1)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    md = root->getMetadatum("test_int8");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md->show();
    md->setValueStr(EK_L("123"));
    md->show();

    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}


//////////////////////////////////////////////////////////////////////

void testDelete()
{
    cout << endl;
    cout << "-----------------------" << endl;
    cout << " Executing testDelete()" << endl;
    cout << "-----------------------" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->loadWithAliases(ALIASES_FILE);
    delete definitionInterpreter;
    ShowFactory();

    // We repeat the process of first loading items, and then deleting some.

    PmtMetadataPtr root = PmtMetadata::create();
    PmtMetadataIterator mdIter;
    PmtMetadataPtr captureDevice = root->getMetadatum("CaptureDevice");
    captureDevice->getMetadata("*");
    cout <<
        "First ensure the getMetadatum for CaptureDevice is working properly."
        << endl << endl;
    captureDevice->show();
    cout << endl;

    // Test *
    captureDevice->deleteMetadata("*");
    cout << "-------------------------" << endl;
    cout << " Test deleteMetadata(\"*\")" << endl;
    cout << "-------------------------" << endl << endl;
    cout << "Everything should be NULL below." << endl << endl;
    captureDevice->show();
    cout << endl;

    // Test ""
    captureDevice->getMetadata("*");
    captureDevice->deleteMetadata("");
    cout << "------------------------" << endl;
    cout << " Test deleteMetadata(\"\")" << endl;
    cout << "------------------------" << endl << endl;
    cout << "Everything should be NULL below." << endl << endl;
    captureDevice->show();
    cout << endl;

    // Test wildcard like A.B.C.*
    captureDevice->getMetadata("*");
    root->deleteMetadata("CaptureDevice.CameraOwner.*");
    cout << "---------------------------------------------------" << endl;
    cout << " Test deleteMetadata(\"CaptureDevice.CameraOwner.*\")" << endl;
    cout << "---------------------------------------------------" << endl <<
        endl;
    cout << "The CameraOwner's contents should be NULL below." << endl << endl;
    captureDevice->show();
    cout << endl;

    // Test wildcard like A.B.C*
    captureDevice->getMetadata("*");
    root->deleteMetadata("CaptureDevice.C*");
    cout << "----------------------------------------" << endl;
    cout << " Test deleteMetadata(\"CaptureDevice.C*\")" << endl;
    cout << "----------------------------------------" << endl << endl;
    cout << "The C* should be NULL below." << endl << endl;
    captureDevice->show();
    cout << endl;

    // Test alias
    captureDevice->getMetadata("*");
    root->deleteMetadata("FirstDeleteAlias");
    cout << "---------------------------------------" << endl;
    cout << " Test deleteMetadata(\"FirstDeleteAias\")" << endl;
    cout << "---------------------------------------" << endl << endl;
    cout << "CFAPattern and SerialNumber should be NULL below." << endl <<
        endl;
    captureDevice->show();
    cout << endl;

    // Test various deletions
    captureDevice->getMetadata("*");
    captureDevice->deleteMetadata("CameraOwner.CameraOwner");
    captureDevice->deleteMetadata("SensingMethod");
    captureDevice->deleteMetadata("SerialNumber");
    cout << "------------------------------" << endl;
    cout << " Test various deleteMetadata()" << endl;
    cout << "------------------------------" << endl << endl;
    cout <<
        "CameraOwner.CameraOwner, SensingMethod, and SerialNumber should be NULL below."
        << endl << endl;
    captureDevice->show();
    cout << endl;

    // Test wildcard like A* that can match alias too
    captureDevice->getMetadata("*");
    captureDevice->deleteMetadata("S*");
    cout << "--------------------------" << endl;
    cout << " Test deleteMetadata(\"S*\")" << endl;
    cout << "--------------------------" << endl << endl;
    cout << "CFAPattern, CameraOwner, and S* should be NULL below." << endl <<
        endl;
    captureDevice->show();
    cout << endl;

    // Test entryTypes = PMT_ALIAS_KEYS_ONLY
    captureDevice->getMetadata("*");
    captureDevice->deleteMetadata("S*", PMT_ALIAS_KEYS_ONLY);
    cout << "-----------------------------------------------" << endl;
    cout << " Test deleteMetadata(\"S*, PMT_ALIAS_KEYS_ONLY\")" << endl;
    cout << "-----------------------------------------------" << endl << endl;
    cout << "CFAPattern and CameraOwner should be NULL below." << endl << endl;
    captureDevice->show();
    cout << endl;

    // Test entryTypes = PMT_METADATA_KEYS_ONLY
    captureDevice->getMetadata("*");
    captureDevice->deleteMetadata("S*", PMT_METADATA_KEYS_ONLY);
    cout << "--------------------------------------------------" << endl;
    cout << " Test deleteMetadata(\"S*\", PMT_METADATA_KEYS_ONLY)" << endl;
    cout << "--------------------------------------------------" << endl <<
        endl;
    cout << "S* should be NULL below." << endl << endl;
    captureDevice->show();
    cout << endl;

    // Test ? deletion
    captureDevice->getMetadata("*");
    captureDevice->deleteMetadata("CameraOwner.?");
    cout << "-------------------------------------" << endl;
    cout << " Test deleteMetadata(\"CameraOwner.?\")" << endl;
    cout << "-------------------------------------" << endl << endl;
    mdIter = captureDevice->getMetadata("");
    if (mdIter.size() != 28)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    cout << "CameraOwner should be NULL below." << endl << endl;
    captureDevice->show();
    cout << endl;

    mdIter.clear();
    // Test A?-like deletions
    captureDevice->getMetadata("*");
    captureDevice->deleteMetadata("F?");
    cout << "-------------------------------------" << endl;
    cout << " Test deleteMetadata(\"F?\")" << endl;
    cout << "-------------------------------------" << endl << endl;
    mdIter = captureDevice->getMetadata("");
    if (mdIter.size() != 22)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    cout << "F? should be NULL below." << endl << endl;
    captureDevice->show();
    cout << endl;

    mdIter.clear();
    captureDevice->deleteMetadata("S?");
    cout << "-------------------------------------" << endl;
    cout << " Test deleteMetadata(\"S?\")" << endl;
    cout << "-------------------------------------" << endl << endl;
    mdIter = captureDevice->getMetadata("");
    if (mdIter.size() != 16)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    cout << "S? should be NULL below." << endl << endl;
    captureDevice->show();
    cout << endl;

    mdIter.clear();
    captureDevice->deleteMetadata("C?");
    cout << "-------------------------------------" << endl;
    cout << " Test deleteMetadata(\"C?\")" << endl;
    cout << "-------------------------------------" << endl << endl;
    mdIter = captureDevice->getMetadata("");
    if (mdIter.size() != 12)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    cout << "C? should be NULL below." << endl << endl;
    captureDevice->show();
    cout << endl;

    mdIter.clear();
    // Test delete of everything with blank key.
    captureDevice->getMetadata("*");
    captureDevice->deleteMetadata("");
    cout << "-------------------------------------" << endl;
    cout << " Test deleteMetadata(\"\")" << endl;
    cout << "-------------------------------------" << endl << endl;
    mdIter = captureDevice->getMetadata("");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    cout << "deleteMetadata(\"\") tested ok" << endl;
    cout << endl;

    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}


void testAllTypes()
{
    cout << endl;
    cout << "-------------------------" << endl;
    cout << " Executing testAllTypes()" << endl;
    cout << "-------------------------" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load(ALL_TYPES_INFO_FILE);
    delete definitionInterpreter;
    ShowFactory();

    // Make a copy for each thread...
    char testAllTypesCopyFilename[1024];
    sprintf(testAllTypesCopyFilename, "%s.%d.xml", ALL_TYPES_FILE_COPY,
            getpid());
    PmtCopyFile(ALL_TYPES_FILE, testAllTypesCopyFilename, true);

    // Test the Logical Instance Interpreter
    PmtLogicalInstanceInterpreter *instanceInterpreter =
        new PmtLogicalInstanceInterpreter();

    PmtMetadataPtr root = PmtMetadata::create();
    PmtMetadataPtr md;

    cout << "---------------------------------------" << endl;
    cout << " Test all the types in AllTypesCopy.xml" << endl;
    cout << "---------------------------------------" << endl << endl;

    if (instanceInterpreter->open(ALL_TYPES_FILE))
    {
        root->getMetadatum("test_int8")->setValueStr(EK_L("123"));
        root->getMetadatum("test_vint8")->
            setValueStr(EK_L("123 124 125 126 127"));
        root->getMetadatum("test_uint8")->setValueStr(EK_L("123"));
        root->getMetadatum("test_vuint8")->
            setValueStr(EK_L("0 1 2 3 4 5 6 7"));
        root->getMetadatum("test_int16")->setValueStr(EK_L("32767"));
        root->getMetadatum("test_vint16")->
            setValueStr(EK_L
                        ("-32767 -259 -258 -257 -256 -255 0 255 265 257 258 259 32767"));
        root->getMetadatum("test_uint16")->setValueStr(EK_L("123"));
        root->getMetadatum("test_vuint16")->
            setValueStr(EK_L("0 255 256 257 258 259 65535"));
        root->getMetadatum("test_int32")->setValueStr(EK_L("-123"));
        root->getMetadatum("test_vint32")->
            setValueStr(EK_L
                        ("-2147483648 -32769 -32768 -32767 0 32767 32768 32769 2147483647"));
        root->getMetadatum("test_uint32")->setValueStr(EK_L("1234567"));
        root->getMetadatum("test_vuint32")->
            setValueStr(EK_L("0 32767 32768 32769 2147483647"));
        root->getMetadatum("test_boolean")->setValueStr(EK_L("false"));
        root->getMetadatum("test_vboolean")->
            setValueStr(EK_L
                        ("true true true false true true false true true"));
        root->getMetadatum("test_float")->setValueStr(EK_L("12345.67e-22"));
        root->getMetadatum("test_vfloat")->
            setValueStr(EK_L("12345.67e-22 0 -0 0.111 0.222e-2"));
        root->getMetadatum("test_double")->setValueStr(EK_L("12345.67e-89"));
        root->getMetadatum("test_vdouble")->
            setValueStr(EK_L("12345.67e-89 0 .00000e-100"));
        root->getMetadatum("test_string")->
            setValueStr(EK_L("Another test string"));
        root->getMetadatum("test_vstring")->
            setValueStr(EK_L("A vector of strings"));
        instanceInterpreter->close();

        if (instanceInterpreter->open(testAllTypesCopyFilename))
        {
            instanceInterpreter->writeMetadata(root);
            instanceInterpreter->close();
        }
        else
            cout << "Could not open file" << endl;
    }
    else
        cout << "Could not open file" << endl;


    // Copy over a different file from the original, so the 2nd copy is different from the original.
    // The code below should change the 2nd copy to be just like the original.
    // Make a copy for each thread...
    char testAllTypesCopy2Filename[1024];
    sprintf(testAllTypesCopy2Filename, "%s.%d.xml", ALL_TYPES_FILE_COPY2,
            getpid());
    PmtCopyFile(testAllTypesCopyFilename, testAllTypesCopy2Filename, true);

    cout << "--------------------------------------" << endl;
    cout << " Test copying entire AllTypes.xml file" << endl;
    cout << "--------------------------------------" << endl << endl;
    if (instanceInterpreter->open(ALL_TYPES_FILE))
    {
        root->deleteMetadata("*");

        // If we're not erasing everything, we don't want everything
        // instantiated
        if (gnThreads <= 1)
            root->getMetadata("*");
        else
        {
            root->getMetadatum("test_int8")->setValueStr(EK_L("128"));
            root->getMetadatum("test_vint8")->setValueStr(EK_L("123 124 125"));
            root->getMetadatum("test_uint8")->setValueStr(EK_L("255"));
            root->getMetadatum("test_vuint8")->
                setValueStr(EK_L("0 1 2 3 4 5"));
            root->getMetadatum("test_int16")->setValueStr(EK_L("-32768"));
            root->getMetadatum("test_vint16")->
                setValueStr(EK_L("-32768 -257 -256 -255 0 255 265 257 32767"));
            root->getMetadatum("test_uint16")->setValueStr(EK_L("65535"));
            root->getMetadatum("test_vuint16")->
                setValueStr(EK_L("0 255 256 257 65535"));
            root->getMetadatum("test_int32")->setValueStr(EK_L("-2147483648"));
            root->getMetadatum("test_vint32")->
                setValueStr(EK_L
                            ("-2147483648 -257 -256 -255 0 255 256 257 2147483647"));
            root->getMetadatum("test_uint32")->setValueStr(EK_L("4294967295"));
            root->getMetadatum("test_vuint32")->
                setValueStr(EK_L("0 255 256 257 4294967295"));
            root->getMetadatum("test_boolean")->setValueStr(EK_L("true"));
            root->getMetadatum("test_vboolean")->
                setValueStr(EK_L("true false true false"));
            root->getMetadatum("test_float")->setValueStr(EK_L("3.4E-38"));
            root->getMetadatum("test_vfloat")->
                setValueStr(EK_L("0.00222 .00333"));
            root->getMetadatum("test_double")->setValueStr(EK_L("123.4"));
            root->getMetadatum("test_vdouble")->
                setValueStr(EK_L("0 .00222 .00333"));
            root->getMetadatum("test_string")->
                setValueStr(EK_L("This is a test string."));
            root->getMetadatum("test_vstring")->
                setValueStr(EK_L("Test string vector."));
        }

        instanceInterpreter->readMetadata(root);
        instanceInterpreter->close();

        if (instanceInterpreter->open(testAllTypesCopy2Filename))
        {
            instanceInterpreter->writeMetadata(root);
            instanceInterpreter->close();

            cout << "Done copying file." << endl << endl;
        }
        else
            cout << "Could not open file" << endl;
    }
    else
        cout << "Could not open file" << endl;


    if (instanceInterpreter)
        delete instanceInterpreter;


    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}


void testMinMaxOccurs()
{
    cout << endl;
    cout << "-----------------------------" << endl;
    cout << " Executing testMinMaxOccurs()" << endl;
    cout << "-----------------------------" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load(MIN_MAX_OCCURS_INFO_FILE);
    delete definitionInterpreter;
    ShowFactory();

    PmtCopyFile(MIN_MAX_OCCURS_FILE, MIN_MAX_OCCURS_FILE_COPY, true);

    PmtMetadataPtr root = PmtMetadata::create();

    cout << endl << "Testing getting a non-zero occurrence first" << endl <<
        endl;
    PmtMetadataPtr one, one1, one2, one3;
    one3 = root->getMetadatum("One[3]");
    one3->setValueStr(EK_L("44"));
    one1 = root->getMetadatum("One[1]");
    one1->setValueStr(EK_L("22"));
    one = root->getMetadatum("One");
    one2 = root->getMetadatum("One[2]");
    root->show();

    PmtMetadataPtr anotherOne, anotherOne1, anotherOne2, anotherOne3;
    anotherOne = root->getMetadatum("One[0]");
    anotherOne1 = root->getMetadatum("One[1]");
    anotherOne2 = root->getMetadatum("One[2]");
    anotherOne3 = root->getMetadatum("One[3]");
    anotherOne->setValueStr(EK_L("11"));
    anotherOne1->setValueStr(EK_L("22"));
    anotherOne3->setValueStr(EK_L("55"));
    cout << endl << "After changing more values" << endl << endl;
    root->show();

    if (!
        (one == anotherOne && one1 == anotherOne1 && one2 == anotherOne2
         && one3 == anotherOne3))
        throw PmtError(PMT_ERROR, "Mismatched metadata pointers", WHERE);

    cout << endl <<
        "Testing a minOccurs greater than 1, and asking for an occurrence less than minOccurs"
        << endl << endl;
    PmtMetadataPtr two, two1, two2;
    two = root->getMetadatum("Two");
    two1 = root->getMetadatum("Two[1]");
    two2 = root->getMetadatum("Two[2]");
    root->show();

    PmtMetadataPtr anotherTwo, anotherTwo1, anotherTwo2;
    anotherTwo1 = root->getMetadatum("Two[1]");
    anotherTwo1->setValueStr(EK_L("222"));
    anotherTwo = root->getMetadatum("Two");
    anotherTwo->setValueStr(EK_L("111"));
    anotherTwo2 = root->getMetadatum("Two[2]");
    anotherTwo2->setValueStr(EK_L("333"));
    cout << endl << "After changing values" << endl << endl;
    root->show();

    if (!(two == anotherTwo && two1 == anotherTwo1 && two2 == anotherTwo2))
        throw PmtError(PMT_ERROR, "Mismatched metadata pointers", WHERE);

    cout << endl <<
        "Testing asking for more occurrences than maxOccurs allows - with maxOccurs > 1"
        << endl << endl;
    PmtMetadataPtr three, three1, three2;
    three = root->getMetadatum("Three");
    three1 = root->getMetadatum("Three[1]");
    three2 = root->getMetadatum("Three[2]");
    if (three2)
        throw PmtError(PMT_ERROR, "Should have obtained a NULL pointer",
                       WHERE);
    three1->setValueStr(EK_L("1111"));
    root->show();

    cout << endl <<
        "Testing asking for more occurrences than maxOccurs allows - with maxOccurs == 1"
        << endl << endl;
    PmtMetadataPtr four, four1;
    four = root->getMetadatum("Four[0]");
    four1 = root->getMetadatum("Four[1]");
    if (four1)
        throw PmtError(PMT_ERROR, "Should have obtained a NULL pointer",
                       WHERE);
    four->setValueStr(EK_L("This is a test string"));
    root->show();

    cout << endl << "Testing re-getting items via getMetadata(list)" << endl <<
        endl;
    PmtMetadataIterator mdIter;
    PmtMetadataPtr md;
    PmtKeyList list;
    list.push_back("One[0]");
    list.push_back("One[1]");
    list.push_back("Two");
    list.push_back("Two[1]");
    list.push_back("Three[1]");

    mdIter = root->getMetadata(list);
    if (mdIter.size() != 5)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    md = mdIter.start();
    while (md)
    {
        md->show();
        md = mdIter.next();
    }
    cout << endl << endl;

    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}


void testRef()
{
    cout << endl;
    cout << "--------------------" << endl;
    cout << " Executing testRef()" << endl;
    cout << "--------------------" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load(REF_INFO_FILE);
    delete definitionInterpreter;
    ShowFactory();

    PmtMetadataPtr root = PmtMetadata::create();
    PmtMetadataPtr md;

    md = root->getMetadatum("A");
    md = root->getMetadatum("RefOne");
    md = root->getMetadatum("RefTwo.TwoTwo");
    md = root->getMetadatum("C");

    root->show();

    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}


void testSimpleType()
{
    cout << endl;
    cout << "---------------------------" << endl;
    cout << " Executing testSimpleType()" << endl;
    cout << "---------------------------" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load(SIMPLE_TYPE_INFO_FILE);
    delete definitionInterpreter;
    ShowFactory();

    PmtMetadataPtr root = PmtMetadata::create();
    PmtMetadataPtr md;

    md = root->getMetadatum("A");
    md = root->getMetadatum("C");
    md = root->getMetadatum("D");

    root->show();

    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}


void testSimpleTypeFacets()
{
    cout << endl;
    cout << "---------------------------------" << endl;
    cout << " Executing testSimpleTypeFacets()" << endl;
    cout << "---------------------------------" << endl;

    // Test the Logical Definition Interpreter.
    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load(FACET_SCHEMA_INFO_FILE);
    delete definitionInterpreter;
    ShowFactory();

    // Test the Logical Instance Interpreter
    PmtLogicalInstanceInterpreter *instanceInterpreter =
        new PmtLogicalInstanceInterpreter();

    if (instanceInterpreter->open(FACET_INSTANCE_DOC))
    {
        PmtMetadataPtr root = PmtMetadata::create();
        PmtMetadataPtr md;

        md = root->getMetadatum("MyElement");
        unsigned long value;
        bool valid;

        value = PMT_CAST_TYPE(md, unsigned long)->value();
        valid = PMT_CAST_TYPE(md, unsigned long)->validate();
        cout << "The value of MyElement is: " << value << endl;
        cout << "The validation result is: " << valid << endl;
        if (value != 0)
            throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
        if (valid != 0)
            throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

        instanceInterpreter->readMetadata(md);
        value = PMT_CAST_TYPE(md, unsigned long)->value();
        valid = PMT_CAST_TYPE(md, unsigned long)->validate();
        cout << "The value of MyElement is: " << value << endl;
        cout << "The validation result is: " << valid << endl;
        if (value != 2)
            throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
        if (valid != 1)
            throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

        md = root->getMetadatum("MyElement2");
        instanceInterpreter->readMetadata(md);
        string stringValue = PMT_CAST_TYPE(md, string)->value();
        valid = PMT_CAST_TYPE(md, string)->validate();
        if (stringValue != "NY")
            throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
        if (valid != 1)
            throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
        cout << "The value of MyElement2 is: " << stringValue << endl;
        cout << "The validation result is: " << valid << endl;
        cout << endl;
    }
    else
        cout << "Could not open file" << endl;

    if (instanceInterpreter)
        delete instanceInterpreter;

    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}


void testMiscellaneous()
{
    cout << endl;
    cout << "------------------------------" << endl;
    cout << " Executing testMiscellaneous()" << endl;
    cout << "------------------------------" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load();
    delete definitionInterpreter;
    ShowFactory();

    PmtMetadataPtr root = PmtMetadata::create();
    PmtMetadataPtr md;

    // Ensure getting a md item via its fullKey works ok. Tests bug fix 417035.
    md = root->getMetadatum("CaptureConditions.SubjectDistance");
    if (md)
    {
        md->show();
        PmtKeyType fullKey = md->fullKey();
        md = root->getMetadatum(fullKey);
        if (md)
            md->show();
    }

    // Tests bug fix 417445.
    root->getMetadata("*", true, PMT_METADATA_KEYS_ONLY);
    PmtMetadataPtr mdd = root->getMetadatum("DigitalProcess.Jpeg.AlphaQTable");
    root->deleteMetadata("DigitalProcess.CellLength");
    root->deleteMetadata("DigitalProcess.CellWidth");
    root->deleteMetadata("DigitalProcess.ICCProfile");
    root->deleteMetadata("DigitalProcess.ImageFileChangeDateTime");
    root->deleteMetadata("DigitalProcess.ImageFileCreationDateTime");
    root->deleteMetadata("DigitalProcess.ImageRotationStatus");
    root->deleteMetadata("DigitalProcess.ImageSource");
    root->deleteMetadata("DigitalProcess.ImageSource.EK");
    SPmtString fKey = mdd->fullKey();
    cout << fKey << endl;

    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}


void testFalseCreate()
{
    cout << endl;
    cout << "----------------------------" << endl;
    cout << " Executing testFalseCreate()" << endl;
    cout << "----------------------------" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load(OCCURRENCES_INFO_FILE);
    delete definitionInterpreter;
    ShowFactory();

    PmtMetadataPtr root = PmtMetadata::create();
    PmtMetadataPtr md;
    PmtMetadataPtr md2;
    PmtMetadataIterator mdIter;
    PmtKeyList myList;

    // Verify nothing returned when create is false.
    md = root->getMetadatum("Big.Four");
    myList.push_back("X");
    myList.push_back("Y.Inside1.2");
    myList.push_back("Y.Inside1.4");
    myList.push_back("Y.Inside3.5.MostInner2");
    myList.push_back("Z.Inside4");
    mdIter = md->getMetadata(myList, false);
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    // Verify items returned when create is true (default parameter).
    mdIter = md->getMetadata(myList);
    if (mdIter.size() != 5)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    // Now make sure same items returned when get everything with create equal to false.
    mdIter = root->getMetadata("*[*]", false);
    if (mdIter.size() != 5)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;

    // Verify when calling with false, that item is not returned if it doesn't already exist.
    md2 = md->getMetadatum("Z.Inside5", false);
    if (md2)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    // Now get that item and verify it's returned in another wildcard call.
    md->getMetadatum("Z.Inside5", true);
    mdIter = root->getMetadata("*", false);
    if (mdIter.size() != 6)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    mdIter.show();
    cout << endl;


    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}


void testExifPersist()
{
    cout << endl;
    cout << "----------------------------" << endl;
    cout << " Executing testExifPersist()" << endl;
    cout << "----------------------------" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load();
    definitionInterpreter->loadWithAliases(PRINT_QUANTITY_INFO_FILE,
                                           EXIF_PERSIST_ALIASES_FILE);
    delete definitionInterpreter;
    ShowFactory();

    PmtMetadataPtr root = PmtMetadata::create();
    PmtMetadataPtr md;

    md = root->getMetadatum("CaptureConditions.PAR");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md->show();
    cout << endl;

    md = root->getMetadatum("CaptureConditions.PrintQuantity");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md->show();
    cout << endl;

    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}


void testLocal()
{
    cout << endl;
    cout << "----------------------" << endl;
    cout << " Executing testLocal()" << endl;
    cout << "----------------------" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load();
    definitionInterpreter->loadWithAliases(LOCAL_INFO_FILE,
                                           LOCAL_ALIASES_FILE);
    delete definitionInterpreter;
    ShowFactory();

    PmtMetadataPtr root = PmtMetadata::create();
    PmtMetadataIterator mdIter;

    mdIter = root->getMetadata("MyAlias");
    if (mdIter.size() != 2)
        throw PmtError(PMT_ERROR, "Test Error!", WHERE);
    mdIter.show();
    cout << endl;

    mdIter = root->getMetadata("exif_persist2");
    if (mdIter.size() != 1)
        throw PmtError(PMT_ERROR, "Test Error!", WHERE);

    mdIter.show();
    cout << endl;

    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}


void showAttrFactory()
{
    cout << endl;
    cout << "Show PmtAttribute Factory..." << endl;
    PmtMetadataPtr root = PmtMetadata::create();
    PmtAttrFactory *factory = root->getAttrFactory();
    PmtAttrFactory::read_only_iterator itr = factory->getIterator();
    PmtAttributePtr attr = itr.start();
    while (attr)
    {
        attr->show(0);
        attr = itr.next();
    }

    cout << "...End of PmtAttribute Factory" << endl << endl;
}


void testAttrFacet()
{
    cout << endl;
    cout << "---------------------------------" << endl;
    cout << " Executing testAttrFacet()" << endl;
    cout << "---------------------------------" << endl;

    // Test the Logical Definition Interpreter.
    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load("AttrFacetInfo.xml");
    delete definitionInterpreter;
    ShowFactory();
    showAttrFactory();

    // test facets validating
    cout << "Testing facet validating..." << endl;
    PmtMetadataPtr rootMd = PmtMetadata::create();
    // PmtAttributePtr attr1 = rootMd->getAttribute("FixedAtrribute");

    PmtMetadataPtr md = rootMd->getMetadatum("ABC2.XYZ1");
    dynamic_cast < PmtMetadataT < string > *>(md.ptr())->value() = "Green";
    if (md->validate() == false)
        printf("testAttrFacet: error in validating metadata (%s, \"Green\")\n",
               md->fullKey(false).c_str());
    dynamic_cast < PmtMetadataT < string > *>(md.ptr())->value() = "Cyan";
    if (md->validate() == true)
        printf("testAttrFacet: error in validating metadata (%s, \"Cyan\")\n",
               md->fullKey(false).c_str());

    md = rootMd->getMetadatum("ABC1");
    dynamic_cast < PmtMetadataT < unsigned long >*>(md.ptr())->value() = 68;
    if (md->validate() == false)
        printf("testAttrFacet: error in validating metadata (%s, 68)\n",
               md->fullKey(false).c_str());
    dynamic_cast < PmtMetadataT < unsigned long >*>(md.ptr())->value() = 118;
    if (md->validate() == true)
        printf("testAttrFacet: error in validating metadata (%s, 118)\n",
               md->fullKey(false).c_str());

    cout << "...End of Testing facet validating" << endl << endl;

    // test attributes
    cout << "Testing attributes..." << endl;
    md = rootMd->getMetadatum("ABC2");
    md->getAttribute("Attr1");
    //md->getAttribute("Attr2");
    PmtAttributeMap attrMap = md->getAttributes(false);
    PmtAttributeMap::iterator s = attrMap.begin();
    while (s != attrMap.end())
    {
        (*s).second->show(0);
        s++;
    }
    md->getAttributes();
    md->show();

    PmtMetadataPtr md1 = rootMd->getMetadatum("ABC2");
    PmtAttributePtr attr1 = md1->getAttribute("Timestamp");
    PmtAttributePtr attr2 = md1->getAttribute("UseAttribute1");
    PmtAttributePtr attr3 = md1->getAttribute("UseAttribute2");
    PmtAttributePtr attr4 = md1->getAttribute("UseAttribute3");
    PmtAttributePtr attr5 = md1->getAttribute("FixedAtrribute");

    if (attr1 && attr2 && attr3 && attr4 && attr5)
    {
        attr1->show();
        attr2->show();
        attr3->show();
        attr4->show();
        attr5->show();
    }
    else
        cout << "Error getting the attributes for ABC2";

    testAttribConstructs();

    cout << "...End of Testing attributes" << endl << endl;


    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}


void deleteAllMetadataOccurrences(PmtMetadataPtr mdCallingThrough,
                                  const SPmtString key)
{
    PmtMetadataPtr md;
    SPmtString noOccurrenceKey = key;
    SPmtString::size_type pos;
    pos = noOccurrenceKey.find("[");
    if (pos != SPmtString::npos)    // strip off occurrence specification, if any
        noOccurrenceKey = noOccurrenceKey.substr(0, pos);

    md = mdCallingThrough->getMetadatum(noOccurrenceKey, false);
    while (md)                  // As long as there are occurrence instantiatations, delete them.
    {
        mdCallingThrough->deleteMetadata(noOccurrenceKey);

        md = mdCallingThrough->getMetadatum(noOccurrenceKey, false);
    }
}

void verifyChoice(PmtMetadataPtr mdCallingThrough, const char *key,
                  bool selectable)
{
    PmtMetadataPtr md;

    if (!mdCallingThrough)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    if (selectable)
    {
        //If selectable, the metadata might already exist. To verify that it exists,
        //call getMetadatum with createIfNotExists set to false. If it exists, there
        //is nothing else to do. If it doesn't already exist, then call getMetadatum
        //with createIfNotExists set to true. If md is not created then throw error,
        //else delete the metadata occurrences created before exiting.
        md = mdCallingThrough->getMetadatum(key, false);
        if (!md)
        {
            md = mdCallingThrough->getMetadatum(key, true);
            if (!md)
            {
                throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
            }
            deleteAllMetadataOccurrences(mdCallingThrough, key);
        }
    }
    else                        //not selectable
    {
        //If not selectable, verify that it doesn't get created.
        md = mdCallingThrough->getMetadatum(key, true);
        if (md)                 // must not obtain a md
            throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    }
}

void verifyChoices(PmtMetadataPtr mdCallingThrough, const SPmtString & keys,
                   bool selectable)
{
    char *buf = NULL;

    try
    {
        char *bufPtr;

        buf = new char[keys.size() + 1];
        strcpy(buf, keys.c_str());

        bufPtr = strtok(buf, " ,");
        while (bufPtr)
        {
            verifyChoice(mdCallingThrough->clone(), bufPtr, selectable);
            bufPtr = strtok(NULL, " ,");
        }

        if (buf)
            delete[]buf;
    }
    catch(...)
    {
        if (buf)
            delete[]buf;

        throw;
    }
}


void verifySelectableChoices(PmtMetadataPtr mdCallingThrough, SPmtString keys)
{
    verifyChoices(mdCallingThrough, keys, true);
}


void verifyNonSelectableChoices(PmtMetadataPtr mdCallingThrough,
                                SPmtString keys)
{
    verifyChoices(mdCallingThrough, keys, false);
}


void testModelGroupFirst(const PmtMetadataPtr & root)
{
    PmtMetadataPtr md;
    PmtMetadataIterator mdIter;
    PmtMetadataPtr first = root->getMetadatum("First");

    if (!first)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);


    md = first->getMetadatum("A");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(first,
                               "B, C, D, I, II, III, a, b, AIn, BIn, CIn, DIn, EIn, FIn");
    verifySelectableChoices(first, "A");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    first->deleteMetadata("A");

    md = first->getMetadatum("B");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(first,
                               "A, C, D, I, II, III, a, b, AIn, BIn, CIn, DIn, EIn, FIn");
    verifySelectableChoices(first, "B");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    first->deleteMetadata("B");

    md = first->getMetadatum("C");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(first,
                               "A, B, D, I, II, III, a, b, AIn, BIn, CIn, DIn, EIn, FIn");
    verifySelectableChoices(first, "C");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    first->deleteMetadata("C");

    md = first->getMetadatum("D");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(first,
                               "A, B, C, I, II, III, a, b, AIn, BIn, CIn, DIn, EIn, FIn");
    verifySelectableChoices(first, "D");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    first->deleteMetadata("D");

    md = first->getMetadatum("I");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(first,
                               "A, B, C, D, II, III, a, b, AIn, BIn, CIn, DIn, EIn, FIn");
    verifySelectableChoices(first, "I");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    first->deleteMetadata("I");

    md = first->getMetadatum("II");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(first,
                               "A, B, C, D, I, III, a, b, AIn, BIn, CIn, DIn, EIn, FIn");
    verifySelectableChoices(first, "II");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    first->deleteMetadata("II");

    md = first->getMetadatum("III");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(first,
                               "A, B, C, D, I, II, a, b, AIn, BIn, CIn, DIn, EIn, FIn");
    verifySelectableChoices(first, "III");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    first->deleteMetadata("III");


    // Take a break to verify that no items exist except for First.
    mdIter = root->getMetadata("");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("First", false);
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);



    md = first->getMetadatum("a");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(first, "A, B, C, D, I, II, III");
    verifySelectableChoices(first, "b, AIn, BIn, CIn, DIn, EIn, FIn");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    first->deleteMetadata("a");

    md = first->getMetadatum("b");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(first, "A, B, C, D, I, II, III");
    verifySelectableChoices(first, "a, AIn, BIn, CIn, DIn, EIn, FIn");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    first->deleteMetadata("b");

    md = first->getMetadatum("AIn");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(first, "A, B, C, D, I, II, III");
    verifySelectableChoices(first, "a, b, AIn, BIn, CIn, DIn, EIn, FIn");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    first->deleteMetadata("AIn");

    md = first->getMetadatum("BIn");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(first, "A, B, C, D, I, II, III");
    verifySelectableChoices(first, "a, b, AIn, BIn, CIn, DIn, EIn, FIn");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    first->deleteMetadata("BIn");

    md = first->getMetadatum("CIn");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(first, "A, B, C, D, I, II, III, DIn, EIn, FIn");
    verifySelectableChoices(first, "a, b, AIn, BIn, CIn");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    first->deleteMetadata("CIn");

    md = first->getMetadatum("DIn");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(first, "A, B, C, D, I, II, III, CIn, EIn, FIn");
    verifySelectableChoices(first, "a, b, AIn, BIn, DIn");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    first->deleteMetadata("DIn");

    md = first->getMetadatum("EIn");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(first, "A, B, C, D, I, II, III, CIn, DIn, FIn");
    verifySelectableChoices(first, "a, b, AIn, BIn");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    first->deleteMetadata("EIn");

    md = first->getMetadatum("FIn");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(first, "A, B, C, D, I, II, III, CIn, DIn, EIn");
    verifySelectableChoices(first, "a, b, AIn, BIn");

    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    first->deleteMetadata("FIn");


    // Verify that no items exist except for First.
    mdIter = root->getMetadata("");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("First", false);
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
}


void testModelGroupSecond(const PmtMetadataPtr & root)
{
    PmtMetadataPtr md;
    PmtMetadataIterator mdIter;
    PmtMetadataPtr second = root->getMetadatum("Second");
    if (!second)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    md = second->getMetadatum("A");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(second, "B, One, Two, C, D, Three, Four");
    verifySelectableChoices(second, "A");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    second->deleteMetadata("A");

    md = second->getMetadatum("B");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(second, "A, One, Two, C, D, Three, Four");
    verifySelectableChoices(second, "B");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    second->deleteMetadata("B");

    md = second->getMetadatum("One");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(second, "A, B, Two, Three, Four");
    verifySelectableChoices(second, "One, C, D");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    second->deleteMetadata("One");

    md = second->getMetadatum("Two");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(second, "A, B, One, Three, Four");
    verifySelectableChoices(second, "Two, C, D");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    second->deleteMetadata("Two");

    md = second->getMetadatum("C");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(second, "A, B, Three, Four");
    verifySelectableChoices(second, "One, Two, D");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    second->deleteMetadata("C");

    md = second->getMetadatum("D");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(second, "A, B, Three, Four");
    verifySelectableChoices(second, "One, Two, C");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    second->deleteMetadata("D");

    md = second->getMetadatum("Three");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(second, "A, B, One, Two, C, D, Four");
    verifySelectableChoices(second, "Three");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    second->deleteMetadata("Three");

    md = second->getMetadatum("Four");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(second, "A, B, One, Two, C, D, Three");
    verifySelectableChoices(second, "Four");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    second->deleteMetadata("Four");

    // Verify that no items exist except for Second.
    mdIter = root->getMetadata("");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("Second", false);
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
}


void testModelGroupThird(const PmtMetadataPtr & root)
{
    PmtMetadataPtr md;
    PmtMetadataIterator mdIter;
    PmtMetadataPtr third = root->getMetadatum("Third");
    if (!third)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    md = third->getMetadatum("A");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(third,
                               "I, II, a, b, AIn, BIn, CIn, DIn, EIn, FIn, B");
    verifySelectableChoices(third, "A");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    third->deleteMetadata("A");

    md = third->getMetadatum("I");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(third,
                               "A, II, a, b, AIn, BIn, CIn, DIn, EIn, FIn, B");
    verifySelectableChoices(third, "I");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    third->deleteMetadata("I");

    md = third->getMetadatum("II");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(third,
                               "A, I, a, b, AIn, BIn, CIn, DIn, EIn, FIn, B");
    verifySelectableChoices(third, "II");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    third->deleteMetadata("II");

    md = third->getMetadatum("a");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(third,
                               "A, I, II, b, AIn, BIn, CIn, DIn, EIn, FIn, B");
    verifySelectableChoices(third, "a");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    third->deleteMetadata("a");

    md = third->getMetadatum("b");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(third,
                               "A, I, II, a, AIn, BIn, CIn, DIn, EIn, FIn, B");
    verifySelectableChoices(third, "b");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    third->deleteMetadata("b");

    md = third->getMetadatum("AIn");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(third, "A, I, II, a, b, B");
    verifySelectableChoices(third, "AIn, BIn, CIn, DIn, EIn, FIn");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    third->deleteMetadata("AIn");

    md = third->getMetadatum("BIn");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(third, "A, I, II, a, b, B");
    verifySelectableChoices(third, "AIn, BIn, CIn, DIn, EIn, FIn");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    third->deleteMetadata("BIn");

    md = third->getMetadatum("CIn");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(third, "A, I, II, a, b, B");
    verifySelectableChoices(third, "AIn, BIn, CIn, DIn, EIn, FIn");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    third->deleteMetadata("CIn");

    md = third->getMetadatum("DIn");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(third, "A, I, II, a, b, B");
    verifySelectableChoices(third, "AIn, BIn, CIn, DIn, EIn, FIn");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    third->deleteMetadata("DIn");

    md = third->getMetadatum("EIn");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(third, "A, I, II, a, b, B");
    verifySelectableChoices(third, "AIn, BIn, CIn, DIn, EIn, FIn");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    third->deleteMetadata("EIn");

    md = third->getMetadatum("FIn");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(third, "A, I, II, a, b, B");
    verifySelectableChoices(third, "AIn, BIn, CIn, DIn, EIn, FIn");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    third->deleteMetadata("FIn");

    // Verify that no items exist except for Third.
    mdIter = root->getMetadata("");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("Third", false);
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
}


void testModelGroupFourth(const PmtMetadataPtr & root)
{
    PmtMetadataPtr md;
    PmtMetadataIterator mdIter;
    PmtMetadataPtr fourth = root->getMetadatum("Fourth");
    if (!fourth)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    md = fourth->getMetadatum("A");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifySelectableChoices(fourth, "A, B, C, D, E, F, G, H, I");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    fourth->deleteMetadata("A");

    md = fourth->getMetadatum("B");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifySelectableChoices(fourth, "A, B, C, D, E, F, G, H, I");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    fourth->deleteMetadata("B");

    md = fourth->getMetadatum("C");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifySelectableChoices(fourth, "A, B, C, D, E, F, G, H, I");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    fourth->deleteMetadata("C");

    md = fourth->getMetadatum("D");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fourth, "E");
    verifySelectableChoices(fourth, "A, B, C, D, F, G, H, I");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    fourth->deleteMetadata("D");

    md = fourth->getMetadatum("E");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fourth, "D");
    verifySelectableChoices(fourth, "A, B, C, E, F, G, H, I");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    fourth->deleteMetadata("E");

    md = fourth->getMetadatum("F");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fourth, "G, H, I");
    verifySelectableChoices(fourth, "A, B, C, D, E, F");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    fourth->deleteMetadata("F");

    md = fourth->getMetadatum("G");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fourth, "F, H, I");
    verifySelectableChoices(fourth, "A, B, C, D, E, G");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    fourth->deleteMetadata("G");

    md = fourth->getMetadatum("H");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fourth, "F, G, I");
    verifySelectableChoices(fourth, "A, B, C, D, E, H");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    fourth->deleteMetadata("H");

    md = fourth->getMetadatum("I");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fourth, "F, G, H");
    verifySelectableChoices(fourth, "A, B, C, D, E, I");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    fourth->deleteMetadata("I");

    // Verify that no items exist except for Fourth.
    mdIter = root->getMetadata("");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("Fourth", false);
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
}


void testModelGroupFifth(const PmtMetadataPtr & root)
{
    PmtMetadataPtr md;
    PmtMetadataIterator mdIter;
    PmtMetadataPtr fifth = root->getMetadatum("Fifth");
    if (!fifth)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    md = fifth->getMetadatum("A[3]");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fifth,
                               "A[10], A[1000], C[3], C[100], D[100], D[2], F[2], F[1000]");
    verifySelectableChoices(fifth,
                            "A[3], A[4], A[9], B[7], B[1], B[0], B, C[1], C[2], C[0], C, D, D[1],"
                            "E[100], E[0], E[1], F[1], F[0], F");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    deleteAllMetadataOccurrences(fifth, "A");

    md = fifth->getMetadatum("A[9]");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fifth,
                               "A[10], A[1000], C[3], C[100], D[100], D[2], F[2], F[1000]");
    verifySelectableChoices(fifth,
                            "A[3], A[4], A[9], B[7], B[1], B[0], B, C[1], C[2], C[0], C, D, D[1],"
                            "E[100], E[0], E[1], F[1], F[0], F");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    deleteAllMetadataOccurrences(fifth, "A");

    // Verify that no items exist except for Fifth.
    mdIter = root->getMetadata("");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("Fifth", false);
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);


    md = fifth->getMetadatum("B[0]");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fifth,
                               "A[10], A[1000], C[3], C[1000], F[2], F[1000]");
    verifySelectableChoices(fifth,
                            "A, A[0], A[1], A[9], B, B[0], B[1], B[100], C[0], C[2],"
                            "D, D[0], D[1], E, E[0], E[1], E[100], F, F[0], F[1]");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    deleteAllMetadataOccurrences(fifth, "B");

    md = fifth->getMetadatum("B[10]");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fifth,
                               "A[10], A[1000], C[3], C[1000], F[2], F[1000]");
    verifySelectableChoices(fifth,
                            "A, A[0], A[1], A[9], B, B[0], B[1], B[100], C[0], C[2],"
                            "D, D[0], D[1], E, E[0], E[1], E[100], F, F[0], F[1]");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    deleteAllMetadataOccurrences(fifth, "B");

    md = fifth->getMetadatum("B[100]");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fifth,
                               "A[10], A[1000], C[3], C[1000], F[2], F[1000]");
    verifySelectableChoices(fifth,
                            "A, A[0], A[1], A[9], B, B[0], B[1], B[100], C[0], C[2],"
                            "D, D[0], D[1], E, E[0], E[1], E[100], F, F[0], F[1]");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    deleteAllMetadataOccurrences(fifth, "B");

    // Verify that no items exist except for Fifth.
    mdIter = root->getMetadata("");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("Fifth", false);
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);


    md = fifth->getMetadatum("C[0]");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fifth,
                               "A[10], A[1000], C[3], C[1000], D, D[0], D[1],"
                               "D[2], E, E[0], E[10], E[100], F[0], F[1], F[2], F[100]");
    verifySelectableChoices(fifth,
                            "A, A[0], A[1], A[9], B, B[0], B[1], B[100], C[0], C[2]");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    deleteAllMetadataOccurrences(fifth, "C");

    md = fifth->getMetadatum("C[2]");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fifth,
                               "A[10], A[1000], C[3], C[1000], D, D[0], D[1],"
                               "D[2], E, E[0], E[10], E[100], F[0], F[1], F[2], F[100]");
    verifySelectableChoices(fifth,
                            "A, A[0], A[1], A[9], B, B[0], B[1], B[100], C[0], C[2]");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    deleteAllMetadataOccurrences(fifth, "C");

    md = fifth->getMetadatum("C[3]");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    // Verify that no items exist except for Fifth.
    mdIter = root->getMetadata("");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("Fifth", false);
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    md = fifth->getMetadatum("D[0]");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fifth,
                               "A[10], A[1000], C, C[0], C[2], C[3], C[1000], D[2],"
                               "D[100], E, E[0], E[10], E[100], F[0], F[1], F[2], F[100]");
    verifySelectableChoices(fifth,
                            "A, A[0], A[1], A[9], B, B[0], B[1], B[100], D, D[0], D[1]");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    deleteAllMetadataOccurrences(fifth, "D");

    md = fifth->getMetadatum("D[1]");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fifth,
                               "A[10], A[1000], C, C[0], C[2], C[3], C[1000], D[2],"
                               "D[100], E, E[0], E[10], E[100], F[0], F[1], F[2], F[100]");
    verifySelectableChoices(fifth,
                            "A, A[0], A[1], A[9], B, B[0], B[1], B[100], D, D[0], D[1]");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    deleteAllMetadataOccurrences(fifth, "D");

    md = fifth->getMetadatum("D[2]");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    // Verify that no items exist except for Fifth.
    mdIter = root->getMetadata("");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("Fifth", false);
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);


    md = fifth->getMetadatum("E[0]");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fifth,
                               "A[10], A[1000], C, C[0], C[2], C[3], C[1000], D, D[0],"
                               "D[2], D[100], F[2], F[100]");
    verifySelectableChoices(fifth,
                            "A, A[0], A[1], A[9], B, B[0], B[1], B[100], E, E[0], E[1],"
                            "E[10], E[100], F, F[0], F[1]");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    deleteAllMetadataOccurrences(fifth, "E");

    md = fifth->getMetadatum("E[7]");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fifth,
                               "A[10], A[1000], C, C[0], C[2], C[3], C[1000], D, D[0],"
                               "D[2], D[100], F[2], F[100]");
    verifySelectableChoices(fifth,
                            "A, A[0], A[1], A[9], B, B[0], B[1], B[100], E, E[0], E[1],"
                            "E[10], E[100], F, F[0], F[1]");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    deleteAllMetadataOccurrences(fifth, "E");

    md = fifth->getMetadatum("E[100]");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fifth,
                               "A[10], A[1000], C, C[0], C[2], C[3], C[1000], D, D[0],"
                               "D[2], D[100], F[2], F[100]");
    verifySelectableChoices(fifth,
                            "A, A[0], A[1], A[9], B, B[0], B[1], B[100], E, E[0], E[1],"
                            "E[10], E[100], F, F[0], F[1]");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    deleteAllMetadataOccurrences(fifth, "E");

    // Verify that no items exist except for Fifth.
    mdIter = root->getMetadata("");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("Fifth", false);
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);


    md = fifth->getMetadatum("F[0]");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fifth,
                               "A[10], A[1000], C, C[0], C[2], C[3], C[1000], D, D[0],"
                               "D[2], D[100], F[2], F[100]");
    verifySelectableChoices(fifth,
                            "A, A[0], A[1], A[9], B, B[0], B[1], B[100], E, E[0], E[1],"
                            "E[10], E[100], F, F[0], F[1]");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    deleteAllMetadataOccurrences(fifth, "F");

    md = fifth->getMetadatum("F[1]");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    verifyNonSelectableChoices(fifth,
                               "A[10], A[1000], C, C[0], C[2], C[3], C[1000], D, D[0],"
                               "D[2], D[100], F[2], F[100]");
    verifySelectableChoices(fifth,
                            "A, A[0], A[1], A[9], B, B[0], B[1], B[100], E, E[0], E[1],"
                            "E[10], E[100], F, F[0], F[1]");
    cout << "Model group test for item  " << md->
        fullKey() << "   - passed" << endl;
    deleteAllMetadataOccurrences(fifth, "F");

    md = fifth->getMetadatum("F[2]");
    if (md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

    // Verify that no items exist except for Fifth.
    mdIter = root->getMetadata("");
    if (mdIter.size() != 0)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md = root->getMetadatum("Fifth", false);
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);

}

void runAPITest()
{
    try
    {
        PmtAPITestPmtMetadataT test1;
        test1.runTest();

        PmtAPITestPmtCompositeMetadata test2;
        test2.runTest();

        PmtAPITestPmtMetadata test3;
        test3.runTest();

    }
    catch(PmtError & e)
    {
        cout << e.getMsg();
        gnExitedWithError++;
    }
    catch(...)
    {
        cout << "GENERAL EXCEPTION CAUGHT!!" << endl;
        gnExitedWithError++;
    }
}


void testLoadMemory()
{
    cout << endl;
    cout << "---------------------------" << endl;
    cout << " Executing testLoadMemory()" << endl;
    cout << "---------------------------" << endl;

    PmtLogicalDefinitionInterpreter interpreter;
    PmtMetadataPtr root;
    PmtMetadataPtr md;
    PmtMetadataIterator mdIter;

    // Get in-memory schema into string object.
    SPmtString localSchema;
    interpreter.convertMemoryVariableToString(PmtLocalDefs, localSchema);

    if (gnThreads <= 1)
        interpreter.erase();
    interpreter.loadMemory(localSchema.c_str());
    ShowFactory();

    root = PmtMetadata::create();
    md = root->getMetadatum("NewMd");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md->setValueStr(EK_L("123"));
    md->show();

    // Get in-memory aliases into string object.
    SPmtString localAliases;
    interpreter.convertMemoryVariableToString(PmtLocalAliases, localAliases);

    if (gnThreads <= 1)
        interpreter.erase();
    interpreter.load();
    interpreter.loadMemory(localSchema.c_str(), DEFAULT_TRANSLATION_TABLE,
                           localAliases.c_str());
    ShowFactory();

    root = PmtMetadata::create();
    md = root->getMetadatum("NewMd");
    if (!md)
        throw PmtError(PMT_ERROR, "Test Error!\n", WHERE);
    md->setValueStr(EK_L("124"));
    cout << "Showing NewMd object..." << endl;
    md->show();
    cout << endl;

    mdIter = root->getMetadata("MyAlias");
    cout << "Showing MyAlias members..." << endl;
    mdIter.show();
    cout << endl;
    mdIter = root->getMetadata("exif_persist2");
    cout << "Showing exif_persist2 members..." << endl;
    mdIter.show();
    cout << endl;
}


void testModelGroups()
{
    cout << endl;
    cout << "----------------------------" << endl;
    cout << " Executing testModelGroups()" << endl;
    cout << "----------------------------" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load(MODEL_GROUPS_INFO_FILE);
    delete definitionInterpreter;
    ShowFactory();
    cout << endl;

    PmtMetadataPtr root = PmtMetadata::create();

    testModelGroupFirst(root);
    testModelGroupSecond(root);
    testModelGroupThird(root);
    testModelGroupFourth(root);
    testModelGroupFifth(root);


    cout << endl << "Passed all tests for model groups." << endl << endl;

    if (gnThreads <= 1)
        PmtLogicalDefinitionInterpreter::erase();
}


void ShowFactory()
{
    PmtMetadataPtr md;

    cout << endl;
    cout << "Now showing Factory contents... " << endl;

    PmtMetadataFactory *factory = PmtMetadata::getFactory("");
    PmtMetadataFactory::read_only_iterator itr = factory->getIterator();
    md = itr.start();
    if (md)
    {
        while (md)
        {
            md->show(0);
            md = itr.next();
        }
    }
    else
    {
        cout << "Nothing in Factory.  Factory is empty" << endl;
    }

    cout << "... end of Factory contents." << endl;
    cout << endl;

}

void testCompTypeSimpCont()
{


    bool bSimContErr = false;
    SPmtString sAttr1, sAttr2, sAttr3, sAttr4;
    PmtAttributePtr attr1, attr2, attr3, attr4, attr5, attr6, attr7;

    cout << endl;
    cout << "-------------------------" << endl;
    cout << " Executing testCompTypeSimpCont()" << endl;
    cout << "-------------------------" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load(SIM_CONT_INFO_FILE);
    delete definitionInterpreter;
    ShowFactory();

// Not used?
//    PmtLogicalInstanceInterpreter * instanceInterpreter = new PmtLogicalInstanceInterpreter();

    PmtMetadataPtr root = PmtMetadata::create();

// CHECK item_price
    PmtMetadataPtr md1 = root->getMetadatum("item_price");
    if (md1)
    {
        md1->setValueStr(EK_L("345.67"));
        attr1 = md1->getAttribute("currency2");
        attr2 = md1->getAttribute("currency3");
        attr3 = md1->getAttribute("currency1");
        attr4 = md1->getAttribute("currency4");
        if (attr1 && attr2 && attr3 && attr4)
        {
            attr1->setValueStr(EK_L("This is currency 2"));
            attr2->setValueStr(EK_L("Currency 3 is fun"));
            attr3->setValueStr(EK_L("Euros are cool"));
            attr4->setValueStr(EK_L("Currency 4"));

            cout << endl << "----------------------------" << endl;
            cout << " Test getMetadata(\"item_price\")" << endl;
            cout << "----------------------------" << endl;
            cout << "item_price = " << PMT_CAST_TYPE(md1,
                                                     float)->value() << endl;

            cout << "Currency1 = " << EK_WIDE2STR(attr3->
                                                  getValueStr()) << endl;
            cout << "Currency2 = " << EK_WIDE2STR(attr1->
                                                  getValueStr()) << endl;
            cout << "Currency3 = " << EK_WIDE2STR(attr2->
                                                  getValueStr()) << endl;
            cout << "Currency4 = " << EK_WIDE2STR(attr4->
                                                  getValueStr()) << endl;
        }
        else
        {
            bSimContErr = true;
            cout << "Error in currency attributes" << endl;
        }
    }
    else
    {
        bSimContErr = true;
        cout << "Error; item_price not found" << endl;
    }

//CHECK item_description
    PmtMetadataPtr md2 = root->getMetadatum("item_description");
    if (md2)
    {
        md2->setValueStr(EK_L("Gadget"));
        attr1 = md2->getAttribute("colour");
        attr2 = md2->getAttribute("cat_number");
        attr3 = md2->getAttribute("instock");
        if (attr1 && attr2 && attr3)
        {
            attr1->setValueStr(EK_L("This is Red"));
            attr2->setValueStr(EK_L("2468"));
            attr3->setValueStr(EK_L("true"));

            cout << endl << "----------------------------" << endl;
            cout << " Test getMetadata(\"item_description\")" << endl;
            cout << "----------------------------" << endl;
            vector < string > value =
                PMT_CAST_TYPE(md2, vector < string >)->value();
            for (unsigned int i = 0; i < value.size(); i++)
            {

                cout << "item_description = " << value[i] << endl;

            }

            cout << "Colour = " << EK_WIDE2STR(attr1->getValueStr()) << endl;
            cout << "Cat_Number = " << EK_WIDE2STR(attr2->
                                                   getValueStr()) << endl;
            cout << "In Stock = " << EK_WIDE2STR(attr3->getValueStr()) << endl;
        }
        else
        {
            bSimContErr = true;
            cout << "Error in item_description attributes" << endl;
        }
    }
    else
    {
        bSimContErr = true;
        cout << "Error; item_description not found" << endl;
    }

//CHECK item_location
    PmtMetadataPtr md3 = root->getMetadatum("item_location");
    if (md3)
    {
        md3->setValueStr(EK_L("Harrow"));
        attr1 = md3->getAttribute("plant_number");
        if (attr1)
        {
            attr1->setValueStr(EK_L("357"));

            cout << endl << "----------------------------" << endl;
            cout << " Test getMetadata(\"item_location\")" << endl;
            cout << "----------------------------" << endl;
            cout << "item_location = " << PMT_CAST_TYPE(md3,
                                                        string)->
                value() << endl;

            cout << "Plant Number = " << EK_WIDE2STR(attr1->
                                                     getValueStr()) << endl;
        }
        else
        {
            bSimContErr = true;
            cout << "Error in item_location attributes" << endl;
        }
    }
    else
    {
        bSimContErr = true;
        cout << "Error; item_price not found" << endl;
    }

    cout << endl << "----------------------------" << endl;
    cout << " Test show item_price" << endl;
    cout << "----------------------------" << endl;

    PmtMetadataPtr md4 = root->getMetadatum("item_price");
    md4->show();

    cout << endl << "----------------------------" << endl;
    cout << " Test show item_description" << endl;
    cout << "----------------------------" << endl;

    PmtMetadataPtr md5 = root->getMetadatum("item_description");
    md5->show();

    cout << endl << "----------------------------" << endl;
    cout << " Test show item_location" << endl;
    cout << "----------------------------" << endl << endl;

    PmtMetadataPtr md6 = root->getMetadatum("item_location");
    md6->show();


    //CHECK THE  item_invoice

    PmtMetadataPtr md7 = root->getMetadatum("item_invoice.invoiceDescription");
    if (md7)
    {
        cout << endl << "----------------------------" << endl;
        cout << " Test show item_invoice" << endl;
        cout << "----------------------------" << endl << endl;
        md7->setValueStr(EK_L("item_invoice.invoiceDescription Gadget"));
        attr5 = md7->getAttribute("colour");
        attr6 = md7->getAttribute("cat_number");
        attr7 = md7->getAttribute("instock");
        if (attr5 & attr6 & attr7)
        {
            attr5->setValueStr(EK_L("green"));
            attr6->setValueStr(EK_L("567"));
            attr7->setValueStr(EK_L("true"));


            vector < string > value =
                PMT_CAST_TYPE(md7, vector < string >)->value();
            for (unsigned int i = 0; i < value.size(); i++)
            {
                cout << "item_description = " << value[i] << endl;
            }

            cout << "item_invoice Colour = " << EK_WIDE2STR(attr5->
                                                            getValueStr()) <<
                endl;
            cout << "item_invoice Cat_Number = " << EK_WIDE2STR(attr6->
                                                                getValueStr())
                << endl;
            cout << " item_invoice In Stock = " << EK_WIDE2STR(attr7->
                                                               getValueStr())
                << endl;

        }
        else
        {
            bSimContErr = true;
            cout << "Error in item_invoice.invoiceDescription attributes";
        }

    }
    else
    {
        bSimContErr = true;
        cout << "Error in item_invoice.invoiceDescription element";
    }

    PmtMetadataPtr md8 = root->getMetadatum("item_invoice.invoicePrice");
    if (md8)
    {
        md8->setValueStr(EK_L("345.67"));
        cout << "item_invoice.invoicePrice = " << PMT_CAST_TYPE(md1,
                                                                float)->
            value() << endl;

    }
    else
    {
        bSimContErr = true;
        cout << "Error in item_invoice.invoicePrice";
    }

    // SIMPLETYPETHREE

    PmtMetadataPtr md9 = root->getMetadatum("simpleTypeThree");
    PmtMetadataPtr md10 = root->getMetadatum("simpleTypeTwo");


    if (bSimContErr)
        cout << "COMPLEX TYPE SIMPLE CONTENT TEST FINISHED WITH ERRORS" <<
            endl;
    else
        cout << endl << "End of Complex Type Simple Content Test" << endl;
}



/*
    This method parses and tests the support for complex content in PMT.
    It parses the associated schema defined in COMPLEX_CONT_INFO_FILE, sets the 
    value of the tags (including those in the complex content - in this case shirt.size,
    and them shows them to the console (from the parsed & constructed PMT tree)
*/
void testCompTypeComplexCont()
{
    bool bComplexContErr = false;

    cout << endl;
    cout << "-------------------------" << endl;
    cout << " Executing testCompTypeComplexCont()" << endl;
    cout << "-------------------------" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load(COMPLEX_CONT_INFO_FILE);
    delete definitionInterpreter;
    ShowFactory();

// Not Used?
//    PmtLogicalInstanceInterpreter * instanceInterpreter = new PmtLogicalInstanceInterpreter();

    PmtMetadataPtr root = PmtMetadata::create();

    PmtMetadataPtr md1 = root->getMetadatum("shirt.name");
    PmtMetadataPtr md2 = root->getMetadatum("shirt.number");
    PmtMetadataPtr md3 = root->getMetadatum("shirt.size");

    PmtMetadataPtr md4 = root->getMetadatum("camera.name");
    PmtMetadataPtr md5 = root->getMetadatum("camera.model");
    PmtMetadataPtr md6 = root->getMetadatum("camera.color");

    root->show();

    PmtAttributePtr attr1, attr2, attr3, attr4;
    PmtMetadataPtr md7 = root->getMetadatum("shirt.price");
    PmtMetadataPtr md8 = root->getMetadatum("shirt.price.retailPrice");
    PmtMetadataPtr md9 = root->getMetadatum("shirt.price.wholeSalelPrice");


    if (md1)
    {
        md1->setValueStr(EK_L("test desc"));
        md1->show();
    }
    else
    {
        bComplexContErr = true;
        cout << "Error; shirt.name not found" << endl;
    }


    if (md2)
    {
        md2->setValueStr(EK_L("5"));
        md2->show();
    }
    else
    {
        bComplexContErr = true;
        cout << "Error; shirt.number not found" << endl;
    }

    if (md3)
    {
        md3->setValueStr(EK_L("test desc"));
        md3->show();
    }
    else
    {
        bComplexContErr = true;
        cout << "Error; shirt.size not found" << endl;
    }

    if (md4)
    {
        md4->setValueStr(EK_L("Kodak"));
        md4->show();
    }
    else
    {
        bComplexContErr = true;
        cout << "Error; camera.name not found" << endl;
    }

    if (md5)
    {
        md5->setValueStr(EK_L("DC210"));
        md5->show();
    }
    else
    {
        bComplexContErr = true;
        cout << "Error; camera.model not found" << endl;
    }

    if (md6)
    {
        md6->setValueStr(EK_L("Grey"));
        md6->show();
    }
    else
    {
        bComplexContErr = true;
        cout << "Error; camera.color not found" << endl;
    }
    if (md7)
    {
        md7->setValueStr(EK_L("143.5"));
        md7->show();

        attr1 = md7->getAttribute("currency1");
        attr2 = md7->getAttribute("currency2");
        attr3 = md7->getAttribute("currency3");
        attr4 = md7->getAttribute("currency4");

        if (attr1 && attr2 && attr3 && attr4)
        {
            attr1->setValueStr(EK_L("This is currency 2"));
            attr2->setValueStr(EK_L("Currency 3 is fun"));
            attr3->setValueStr(EK_L("Euros are cool"));
            attr4->setValueStr(EK_L("Currency 4"));

            cout << "Currency1 = " << EK_WIDE2STR(attr3->
                                                  getValueStr()) << endl;
            cout << "Currency2 = " << EK_WIDE2STR(attr1->
                                                  getValueStr()) << endl;
            cout << "Currency3 = " << EK_WIDE2STR(attr2->
                                                  getValueStr()) << endl;
            cout << "Currency4 = " << EK_WIDE2STR(attr4->
                                                  getValueStr()) << endl;
        }
        else
        {
            bComplexContErr = true;
            cout << "Error in currency attributes" << endl;
        }

    }
    else
    {
        bComplexContErr = true;
        cout << "Error; shirt.price not found" << endl;

    }
    if (md8)
    {
        md8->setValueStr(EK_L("123.4"));
        md8->show();
    }
    else
    {
        bComplexContErr = true;
        cout << "Error; price.retailPrice not found" << endl;
    }
    if (md9)
    {
        md9->setValueStr(EK_L("567.8"));
        md9->show();
    }
    else
    {
        bComplexContErr = true;
        cout << "Error; price.WholeSalePrice not found" << endl;
    }

    // if any of the elements had an error, test failed , show an error.    
    if (bComplexContErr)
        cout << "COMPLEX TYPE COMPLEX CONTENT TEST FINISHED WITH ERRORS" <<
            endl;
    else
        cout << "End of Complex Type Complex Content Test" << endl;
}

// END of testCompTypeComplexCont method.


void testAttribConstructs()
{

    // check for all the attributes defined in the AttFacet.xml file (ATTRIBUTE_FACET_FILE)

    // we need to check the following attributes.
    // default - we should get this value , if nothing is specified in the schema.
    // fixed - we should get an error , if any other value is specified in the instance 
    // use - test the prohibited, required and the optional values.


    cout << "--------------------" << endl;
    cout << " TestAttribConstruct()" << endl;
    cout << "--------------------" << endl;

    cout << "Called the testAttribConstructs to test the attributes.";

    PmtCopyFile(ATTRIBUTE_FACET_FILE, ATTRIBUTE_FACET_COPY, true);


    // till now the schema has been parsed , and stored in the factory.
    // now we will parse the elements in the instance.

    //create the instance interpreter, this will actually be used to parse the instance document.
    PmtLogicalInstanceInterpreter *instanceInterpreter =
        new PmtLogicalInstanceInterpreter();

    PmtMetadataPtr root;

    // parse the document (file specified), and then store the mParser.
    if (instanceInterpreter->open(ATTRIBUTE_FACET_COPY))
    {
        // 
        root = instanceInterpreter->readMetadata();
        PmtMetadataPtr mdabc1, md2;


        mdabc1 = root->getMetadatum("ABC2");
        // get the elements, attributes and then set the values, write back to the xml instance.
        PmtAttributePtr attr1 = mdabc1->getAttribute("Timestamp");
        if (attr1)
        {
            attr1->show();
        }
        PmtAttributePtr attr2 = mdabc1->getAttribute("Attr1");
        if (attr2)
        {
            attr2->show();
        }
        PmtAttributePtr attr3 = mdabc1->getAttribute("EffectiveDate");
        if (attr3)
        {
            attr3->show();
        }
        PmtAttributePtr attr4 = mdabc1->getAttribute("FixedAtrribute");
        if (attr4)
        {
            attr4->show();
        }
        PmtAttributePtr attr5 = mdabc1->getAttribute("useAtrribute1");
        if (attr5)
        {
            attr5->show();
        }


        md2 = root->getMetadatum("ABC5");
        // get the elements, attributes and then set the values, write back to the xml instance.
        PmtAttributePtr attr9 = md2->getAttribute("nillable");
        if (attr9)
        {
            attr9->show();
        }
        PmtAttributePtr attr10 = md2->getAttribute("additionalAttr");
        if (attr10)
        {
            attr10->show();
        }

        PmtLogicalInstanceInterpreter *instanceInterpreter1 =
            new PmtLogicalInstanceInterpreter();
        instanceInterpreter1->open("TestWrite.xml", PMT_CREATE);
        instanceInterpreter1->writeMetadata(root);
        instanceInterpreter1->flush();
        instanceInterpreter1->close();

        instanceInterpreter->flush();
        instanceInterpreter->close();

        delete instanceInterpreter1;

    }
    else
        cout << "could not open file AttFacet.xml";

    delete instanceInterpreter;

}


void testWriteNoRead()
{

    cout << "--------------------" << endl;
    cout << " testWriteNoRead()" << endl;
    cout << "--------------------" << endl;
    PmtCopyFile(ALL_TYPES_FILE, ALL_TYPES_FILE_WRITE_COPY, false);

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load(ALL_TYPES_INFO_FILE);
    delete definitionInterpreter;
    ShowFactory();
    showAttrFactory();


    PmtMetadataPtr rootMd = PmtMetadata::create();

    PmtMetadataPtr md = rootMd->getMetadatum("test_string");
    md->setValueStr(EK_L("test write"));

    PmtLogicalInstanceInterpreter *instanceInterpreter =
        new PmtLogicalInstanceInterpreter();
    instanceInterpreter->open(ALL_TYPES_FILE_WRITE_COPY);
    instanceInterpreter->writeMetadata(md);
    instanceInterpreter->flush();
    instanceInterpreter->close();

    delete instanceInterpreter;

    cout << "End of testWriteNoRead" << endl;


}



/*
    This method parses and tests the support for Include tag in PMT.
    It parses the associated schema defined in INCLUDE_INFO_FILE, sets the 
    value of the tags and them shows them to the console.
*/

void testIncludeTag()
{
    bool bIncludeErr = false;

    cout << endl;
    cout << "-------------------------" << endl;
    cout << " Executing TestIncludeTag" << endl;
    cout << "-------------------------" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load(INCLUDE_INFO_FILE);
    delete definitionInterpreter;

    PmtMetadataPtr root = PmtMetadata::create();

    PmtMetadataPtr md1 = root->getMetadatum("order.number");
    PmtMetadataPtr md2 = root->getMetadatum("order.customer.CustName");
    PmtMetadataPtr md3 = root->getMetadatum("order.customer.custAge");
    PmtMetadataPtr md4 = root->getMetadatum("order.item");
    PmtMetadataPtr md5 = root->getMetadatum("order.customer");

    PmtMetadataPtr md14 =
        root->getMetadatum("order.customer.personalDetails.Name");
    PmtMetadataPtr md15 =
        root->getMetadatum("order.customer.personalDetails.Name");
    PmtMetadataPtr md16 =
        root->getMetadatum("order.customer.personalDetails.Name");


    PmtMetadataPtr md6 = root->getMetadatum("shirt.price");
    PmtMetadataPtr md7 = root->getMetadatum("shirt.number");
    PmtMetadataPtr md8 = root->getMetadatum("shirt.name");
    PmtMetadataPtr md9 = root->getMetadatum("shirt.size");
    PmtMetadataPtr md10 = root->getMetadatum("shirt.color");


    if (md1)
    {
        md1->setValueStr(EK_L("1233"));
        md1->show();
    }
    else
    {
        bIncludeErr = true;
        cout << "Error: order.number not found" << endl;
    }
    if (md2)
    {
        md2->setValueStr(EK_L("1"));
        md2->show();
    }
    else
    {
        bIncludeErr = true;
        cout << "Error:order.customer.CustType not found" << endl;
    }
    if (md3)
    {
        md3->setValueStr(EK_L("25"));
        md3->show();
    }
    else
    {
        bIncludeErr = true;
        cout << "Error: order.customer.custAge not found" << endl;
    }
    if (md4)
    {
        md4->setValueStr(EK_L("4444"));
        md4->show();
    }
    else
    {
        bIncludeErr = true;
        cout << "Error: item not found" << endl;
    }

    if (md5)
    {
        PmtAttributePtr attr1 = md5->getAttribute("testAttr");
        if (attr1)
        {
            attr1->show();
        }
        else
            cout << "Attribute testAttr is null" << endl;

    }
    else
    {
        bIncludeErr = true;
        cout << "Error: item md5 not found" << endl;
    }

    //tests for the complexContens tags in te included file.
    if (md6)
    {
        md6->setValueStr(EK_L("1233"));
        md6->show();

    }
    else
    {
        bIncludeErr = true;
        cout << "Error: item md6 not found" << endl;
    }

    if (md7)
    {
        md7->setValueStr(EK_L("1233"));
        md7->show();

    }
    else
    {
        bIncludeErr = true;
        cout << "Error: item not found" << endl;
    }

    if (md8)
    {
        md8->setValueStr(EK_L("1233"));
        md8->show();

    }
    else
    {
        bIncludeErr = true;
        cout << "Error: item not found" << endl;
    }

    if (md9)
    {
        md9->setValueStr(EK_L("1233"));
        md9->show();

    }
    else
    {
        bIncludeErr = true;
        cout << "Error: item not found" << endl;
    }

    if (md10)
    {
        md10->setValueStr(EK_L("1233"));
        md10->show();

    }
    else
    {
        bIncludeErr = true;
        cout << "Error: item not found" << endl;
    }


    if (md14)
    {
        md14->setValueStr(EK_L("1233"));
        md14->show();
    }
    else
    {
        bIncludeErr = true;
        cout << "Error: order.number not found" << endl;
    }
    if (md15)
    {
        md15->setValueStr(EK_L("1"));
        md15->show();
    }
    else
    {
        bIncludeErr = true;
        cout << "Error:order.customer.CustType not found" << endl;
    }
    if (md16)
    {
        md16->setValueStr(EK_L("25"));
        md16->show();
    }
    else
    {
        bIncludeErr = true;
        cout << "Error: order.customer.custAge not found" << endl;
    }



// Test for the simple content.

    PmtMetadataPtr md11 = root->getMetadatum("price");
    PmtAttributePtr attr1, attr2, attr3, attr4;

    if (md11)
    {
        md11->setValueStr(EK_L("345.67"));
        attr1 = md11->getAttribute("currency12");
        attr2 = md11->getAttribute("currency13");
        attr3 = md11->getAttribute("currency11");
        attr4 = md11->getAttribute("currency14");
        if (attr1 && attr2 && attr3 && attr4)
        {
            attr1->setValueStr(EK_L("This is currency 2"));
            attr2->setValueStr(EK_L("Currency 3 is fun"));
            attr3->setValueStr(EK_L("Euros are cool"));
            attr4->setValueStr(EK_L("Currency 4"));

            cout << "price = " << PMT_CAST_TYPE(md11,
                                                     float)->value() << endl;

            cout << "Currency1 = " << EK_WIDE2STR(attr3->
                                                  getValueStr()) << endl;
            cout << "Currency2 = " << EK_WIDE2STR(attr1->
                                                  getValueStr()) << endl;
            cout << "Currency3 = " << EK_WIDE2STR(attr2->
                                                  getValueStr()) << endl;
            cout << "Currency4 = " << EK_WIDE2STR(attr4->
                                                  getValueStr()) << endl;
        }
        else
        {
            bIncludeErr = true;
            cout << "Error in currency attributes" << endl;
        }
    }
    else
    {
        bIncludeErr = true;
        cout << "Error: rice not found" << endl;
    }

    PmtMetadataPtr md12 = root->getMetadatum("payment.currency");
    if (md12)
    {
        md12->setValueStr(EK_L("Dollar"));
        md12->show();

    }
    else
    {
        bIncludeErr = true;
        cout << "Error: payment.currency not found" << endl;
    }

    PmtMetadataPtr md13 = root->getMetadatum("payment.amount");
    if (md13)
    {
        md13->setValueStr(EK_L("12345.67"));
        md13->show();

    }
    else
    {
        bIncludeErr = true;
        cout << "Error: payment.amount not found" << endl;
    }



    if (bIncludeErr)
        cout << "INCLUDE SCHEMA TEST FINISHED WITH ERRORS" << endl;
    else
        cout << "End of Include Schema Test" << endl;

}


void testImportTag()
{
    bool bImportErr = false;

    cout << endl;
    cout << "-------------------------" << endl;
    cout << " Executing testImportTag" << endl;
    cout << "-------------------------" << endl;

    PmtLogicalDefinitionInterpreter *definitionInterpreter =
        new PmtLogicalDefinitionInterpreter;
    definitionInterpreter->load(IMPORT_INFO_FILE);
    delete definitionInterpreter;


    PmtMetadataPtr root = PmtMetadata::create();

    PmtMetadataPtr md1 = root->getMetadatum("order.number");
    if (md1)
    {
        md1->setValueStr(EK_L("1233"));
        md1->show();
    }
    else
    {
        bImportErr = true;
        cout << "Error: order.number not found" << endl;
    }

    PmtMetadataPtr md2 = root->getMetadatum("order.items.product");
    if (md2)
    {
        md2->setValueStr(EK_L("1234"));
        md2->show();
    }
    else
    {
        bImportErr = true;
        cout << "Error: order.items.product not found" << endl;
    }


    PmtMetadataPtr md3 = root->getMetadatum("order.items.catalogID");
    if (md3)
    {
        md3->setValueStr(EK_L("1235"));
        md3->show();
    }
    else
    {
        bImportErr = true;
        cout << "Error: order.items.catalogID not found" << endl;
    }

    if (bImportErr)
        cout << "IMPORT SCHEMA TEST FINISHED WITH ERRORS" << endl;
    else
        cout << "End of Import Schema Test" << endl;
}

