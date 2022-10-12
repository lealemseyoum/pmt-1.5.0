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
* Chris Lin  ti.lin@kodak.com
*
* Contributor(s):
* Ricardo Rosario  ricardo.rosario@kodak.com (September 27, 2001)
* Sam Fryer samuel.fryer@kodak.com
*
* Date: November 20, 2000
*/

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include "PmtCommon.h"
#include <fstream>
#include "PmtMetadataT.h"
#if defined(HAVE_KIESEXIF) || defined(HAVE_OPENEXIF)
#include "PmtExifAccessor.h"
#endif
#include "TiffDirectory.h"
#include "TiffStripImage.h"
#include "PmtTiffAccessor.h"
#include "PmtXmlAccessor.h"
#include "PmtDefinitionInterpreter.h"
#include "PmtMisc.h"
#include "../Common/PmtLocalDefs.h"
#include "../Common/PmtLocalTable.h"
#include "../Common/PmtLocalAliases.h"
#include "../Common/times.h"


// For multi-platform threading
#include "EkThread.h"
// The max number of threads the user can specify
#define MAX_THREADS 1024

#if ((defined TEST_APS) && (!defined __GNUC__))
#include "PmtApsAccessor.h"
#endif

#ifdef PMT_JPEG2000
#include "PmtDIG35Accessor.h"
#include "PmtJ2KAccessor.h"

#ifdef PMT_USE_KAKADU_4
#include "kdu_stripe_compressor.h"
#endif

#endif // PMT_JPEG2000

#if ((defined TEST_APS) && (!defined __GNUC__))
void testApsAcc();
#endif

const int MAX_OUTPUT_STRING_LEN = 100;

#if defined( HAVE_KIESEXIF )
void testExifAcc(int id);
#endif

#if defined( HAVE_KIESEXIF ) || defined( HAVE_OPENEXIF )
void testExifAccVariousSources();

void testExifAccAllKeys(bool closeFileBeforeReading, int id,
						char *exifOriginal = "Kodak_PictureCD.jpg");
void testExifAccFloat(PmtAccessor * exifAcc, char *exifCopy,
					  bool closeFileBeforeReading);
void testExifAccUnsignedByte(PmtAccessor * exifAcc, char *exifCopy,
							 bool closeFileBeforeReading);
void testExifAccBoolean(PmtAccessor * exifAcc, char *exifCopy,
						bool closeFileBeforeReading);
void testExifAccString(PmtAccessor * exifAcc, char *exifCopy,
					   bool closeFileBeforeReading);
void testExifAccUnsignedShort(PmtAccessor * exifAcc, char *exifCopy,
							  bool closeFileBeforeReading);
void testExifAccUnsignedInt(PmtAccessor * exifAcc, char *exifCopy,
							bool closeFileBeforeReading);
void testExifAccPmt_vuint8(PmtAccessor * exifAcc, char *exifCopy,
						   bool closeFileBeforeReading);
void testExifAccPmt_vuint16(PmtAccessor * exifAcc, char *exifCopy,
							bool closeFileBeforeReading);
void testExifAccPmt_vfloat(PmtAccessor * exifAcc, char *exifCopy,
						   bool closeFileBeforeReading);
void testExifAccPmt_vuint32(PmtAccessor * exifAcc, char *exifCopy,
							bool closeFileBeforeReading);
void testExifAccPmt_vint16(PmtAccessor * exifAcc, char *exifCopy,
						   bool closeFileBeforeReading);
void testExifAccPmt_vint32(PmtAccessor * exifAcc, char *exifCopy,
						   bool closeFileBeforeReading);
void testExifAccByte(PmtAccessor * exifAcc, char *exifCopy,
					 bool closeFileBeforeReading);

void comSegmentTest();
void jfifTest(int id);
#endif

void testTiffAcc(int id);
void testTiffAccAllKeys(int id);
void testXmlAcc(int id);
void testLoadMemory(int id);

#ifdef PMT_JPEG2000
void testJ2KAcc();
#endif // PMT_JPEG2000

#ifdef PMT_XMP_ACCESSOR
void TestXMPAccessor();
#endif //PMT_XMP_ACCESSOR

#if macintosh
#include "../common/console_app.h"
char *logFileName = "TestResults.txt";  // cout results go to this file instead
#endif

// Global Variables
int gnThreads = 1;              // Number of threads we're running
int gnIterations = 1;           // Number of iterations per thread
int gnExitedWithError = 0;      // Number of exceptions caught
EK_DEFAULT_MUTEX gThreadMutex;  // global mutex

// Example Mutex use (simply put at top of the block of code):  
//EK_GUARD(EK_DEFAULT_MUTEX, gThreadMutex);

// function to run all the tests, including timing and catching exceptions
void runTests(int id)
{
#ifdef PMT_XMP_ACCESSOR
#ifdef PMT_USE_XERCES

	XMLPlatformUtils::Initialize();	

#endif
#endif //PMT_XMP_ACCESSOR
	int i = 0;

	try
	{
		// Loop through the specified number of iterations ( the -i flag )
		for (i = 0; i < gnIterations; i++)
		{
			// load metadata definition
			PmtLogicalDefinitionInterpreter interpreter;
			interpreter.loadWithAliases("aliases.xml");
			interpreter.loadWithAliases("../Common/LocalInfo.xml",
				"../Common/LocalAliases.xml");
#if defined( HAVE_KIESEXIF )
			testExifAcc(id);
#endif

#if defined( HAVE_KIESEXIF ) || defined( HAVE_OPENEXIF )
			testExifAccVariousSources();
			testExifAccAllKeys(true, id);   // close file after writing and before reading
			testExifAccAllKeys(false, id);  // do not close file after writing and before reading

			comSegmentTest();
			jfifTest(id);

			cout << "...END OF EXIF ACCESSOR TEST..." << endl;
#endif

			testTiffAcc(id);
			testTiffAccAllKeys(id);
			testXmlAcc(id);

#if ((defined TEST_APS) && (!defined __GNUC__))
			testApsAcc();
#endif
#ifdef PMT_XMP_ACCESSOR
			TestXMPAccessor();
#endif
			testLoadMemory(id);

#ifdef PMT_JPEG2000
			cout << "testing J2K Accessor..." << endl;
			testJ2KAcc();
#endif // PMT_JPEG2000

		}
	}
	catch(PmtError & e)
	{
		cout << e << endl;
		gnExitedWithError++;
	}
	catch(...)
	{
		cout << "**** Some Exception caught." << endl;
		gnExitedWithError++;
	}

}


//////////////////////////////////////////////////////////////////////

// NOTE: Use of EkThread is not supported by Pmt. Just using for testing purposes.
// Our class for running the testing threads
class TestThread:public EkThread < EK_DEFAULT_MUTEX >
{
public:
	virtual void run(void);
	int uniqueNumber;
};

//////////////////////////////////////////////////////////////////////

// The routine that actually gets run in each thread
void TestThread::run(void)
{
	runTests(uniqueNumber);
	return;
}

//////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	int i = 0;
	double t0 = 0, t1 = 0;
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
		runTests(0);
	else
	{
		// create and run each thread
		for (i = 0; i < gnThreads; i++)
		{
			child[i] = new TestThread;
			child[i]->uniqueNumber = i;
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

void printXmlFile(const char *file)
{
	FILE *f = fopen(file, "r");
	char c;
	while ((c = getc(f)) != EOF)
	{
		cout << c;
	}
	fclose(f);
}

void testXmlAcc(int id)
{
	cout << endl << "********Test Xml accessor********" << endl;

#if defined( HAVE_KIESEXIF ) || defined( HAVE_OPENEXIF )
	char inFile[] = "KodakDC260.jpg";
#else
	char inFile[] = "TestInput.xml";
#endif    
	char filename[128];
	sprintf(filename, "XmlAccTest.%d.xml", id);
	SPmtString xmlFile = filename;

	cout << endl <<
		"--- Testing read all metadata from an Exif file and write to an xml file"
		<< endl;

	// ----- testing read from an Exif file and write to an xml file
	PmtAccessorPtr inAcc = PmtAccessor::getAccessor(inFile);

	PmtMetadataPtr mdRoot = inAcc->readMetadata();
	PmtAccessorPtr xmlAcc = new PmtXmlAccessor();
	if (!xmlAcc->open(xmlFile.c_str(), PMT_CREATE))
	{
		throw PmtError(PMT_ERROR, "can't create xml file", WHERE);
	}

	// writing xml
	xmlAcc->writeMetadata(mdRoot);

	xmlAcc->close();

	printXmlFile(xmlFile.c_str());

	cout << endl <<
		"--- Testing read from an xml file and write to another xml file" <<
		endl;

#ifndef PMT_USE_NO_XML_PARSER
	// read from an xml file and write to another xml file
	mdRoot = PmtMetadata::create();
	char inFile2[] = "TestInput.xml";
	inAcc = PmtAccessor::getAccessor(inFile2);

	mdRoot = inAcc->readMetadata();
	if (!xmlAcc->open(xmlFile.c_str(), PMT_CREATE))
	{
		throw PmtError(PMT_ERROR, "can't create xml file", WHERE);
	}

	// writing xml
	xmlAcc->writeMetadata(mdRoot);

	xmlAcc->close();

	printXmlFile(xmlFile.c_str());


	xmlAcc = PmtAccessor::getAccessor(xmlFile.c_str());
	if (!xmlAcc)
	{
		throw PmtError(PMT_ERROR,
			"can't open xml file with PmtAccessor::getAccessor",
			WHERE);
	}
	mdRoot = PmtMetadata::create();
	PmtMetadataIterator mdIt = mdRoot->getMetadata("alias_1");
	xmlAcc->readMetadata(mdIt);
	xmlAcc->close();

	xmlAcc = new PmtXmlAccessor();
	if (!xmlAcc->open(xmlFile.c_str(), PMT_CREATE))
	{
		throw PmtError(PMT_ERROR, "can't create xml file", WHERE);
	}
	xmlAcc->writeMetadata(mdRoot);
	xmlAcc->close();

	printXmlFile(xmlFile.c_str());

	cout << endl << "--- Testing read/write from the same xml file" << endl;

	// open xml for reading and writing
	if (!xmlAcc->open(xmlFile.c_str(), PMT_READWRITE))
	{
		throw PmtError(PMT_ERROR, "can't open xml file", WHERE);
	}

	mdRoot = PmtMetadata::create();
	mdIt = mdRoot->getMetadata("alias_2");
	inAcc->readMetadata(mdIt);

	PmtMetadataPtr md = mdRoot->getMetadatum("CaptureDevice.Make");
	md->setValueStr(EK_L("EKC"));

	// writing xml
	xmlAcc->writeMetadata(mdRoot);
	xmlAcc->close();

	printXmlFile(xmlFile.c_str());

	cout << endl <<
		"--- Testing PmtLogicalInstanceInterpreter read/write from stream" <<
		endl;

	// test read/write from stream
	PmtMetadataPtr xmlMd;
	fstream f, f2;
	f.open(xmlFile.c_str(), ios::in);
	f >> xmlMd;
	cout << xmlMd;
	f.close();

#if (defined _MSC_VER) | (defined macintosh)
	f.open(xmlFile.c_str(), (ios_base::openmode) (ios::out || ios::trunc));
#elif (defined __GNUC__)
	f.open(xmlFile.c_str(), (ios::openmode) (ios::out || ios::trunc));
#endif
	f << xmlMd;
	f.close();

	mdRoot = PmtMetadata::create();
	mdRoot->getMetadatum("CaptureDevice.Make");
	mdRoot->getMetadatum("CaptureDevice.Model");
	md = mdRoot->getMetadatum("CaptureDevice", false);
	f2.open(xmlFile.c_str(), ios::in);
	f2 >> md;
	cout << endl << endl;
	cout << md;
	cout << endl;
	f2.close();
	inAcc->close();
#endif
}


// Class that holds a key, a value to be tested (write and read back) to the
// key and a bool which indicates whether the key is currently supported
template < class TYPE > class KeyTestInfo
{
private:
	char *mFullKey;
	TYPE mValue;
	bool mSupported;

public:
	// default constructor
	KeyTestInfo()
	{
	}

	// constructor
	KeyTestInfo(char *k, TYPE v, bool s = true)
	{
		mFullKey = k;
		mValue = v;
		mSupported = s;
	}

	char *getKey()
	{
		return mFullKey;
	}

	TYPE getValue()
	{
		return mValue;
	}

	bool isSupported()
	{
		return mSupported;
	}
};

#if defined( HAVE_KIESEXIF )
void testExifAcc(int id)
{
	cout << "********Test Exif accessor********" << endl;

	char inExif[] = "KodakDC260.jpg";
	char outExif[128];
	sprintf(outExif, "test.%d.jpg", id);
	char buf[MAX_OUTPUT_STRING_LEN];

	NIFStatus status;
	NIFExifImageInfo imgInfo;
	NIFExifFileType fileType;
	NIFExifImageHandle hIn, hOut;
	status =
		NIFEXIF_OpenByFileName(inExif, &fileType, &imgInfo, NIF_OPEN_READONLY,
		&hIn);
	if (status != NIF_OK)
	{
		sprintf(buf, "testExifAcc: can't open file %s\n", inExif);
		cout << buf;
		exit(-1);
	}

	status = NIFEXIF_CreateByFileName(outExif, fileType, &imgInfo, &hOut);
	if (status != NIF_OK)
	{
		sprintf(buf, "testExifAcc: can't create file %s\n", outExif);
		cout << buf;
		NIFEXIF_Close(&hIn);
		exit(-1);
	}

	// write image data from input image to output image
	unsigned int i;
	NIFImageDesc imgDesc;
	imgDesc.numberOfComponents = imgInfo.numChannels;
	imgDesc.numberOfColumns = imgInfo.imageWidth;
	imgDesc.numberOfRows = 1;
	for (i = 0; i < imgDesc.numberOfComponents; i++)
		imgDesc.components[i].theData =
		new unsigned char[imgDesc.numberOfColumns *
		imgDesc.numberOfComponents];
	unsigned short numWritten, nextLine;

	for (i = 0; i < imgInfo.imageHeight; i++)
	{
		NIFEXIF_ReadScanLines(&hIn, &imgDesc, 1, &numWritten, &nextLine);
		NIFEXIF_WriteScanLines(&hOut, &imgDesc, 1, &numWritten, &nextLine);
	}

	for (i = 0; i < imgDesc.numberOfComponents; i++)
		delete imgDesc.components[i].theData;

	// instantiate accessors
	PmtExifAccessor exifAcc(&hIn), outAcc(&hOut);
	PmtAccessor *inAcc = &exifAcc;

	// ---- write metadata from input file to output file
	sprintf(buf, "\nWriting metadata from %s to %s: \n", inExif, outExif);
	cout << buf;
	// builtin2ushort
	PmtMetadataPtr rootMd = PmtMetadata::create();
	PmtMetadataPtr md = rootMd->getMetadatum("ImageContainer.Orientation");
	inAcc->readMetadata(md);
	md->print(cout);
	cout << endl;
	outAcc.writeMetadata(md);
	// builtinto2float
	md = rootMd->getMetadatum("CaptureConditions.ExposureTime");
	inAcc->readMetadata(md);
	md->print(cout);
	cout << endl;
	outAcc.writeMetadata(md);
	// builtin2ascii
	md = rootMd->getMetadatum("CaptureDevice.Make");
	inAcc->readMetadata(md);
	md->print(cout);
	cout << endl;
	outAcc.writeMetadata(md);
	// builtin2ascii
	md = rootMd->getMetadatum("CaptureDevice.Model");
	inAcc->readMetadata(md);
	md->print(cout);
	cout << endl;
	outAcc.writeMetadata(md);
	// builtinto2char
	md = rootMd->getMetadatum("CaptureDevice.MakerNotes");
	inAcc->readMetadata(md);
	md->print(cout);
	cout << endl;
	outAcc.writeMetadata(md);
	// audio
	md = rootMd->getMetadatum("SceneContent.Audio");
	vector < unsigned char >val;
	for (i = 0; i < 88; i++)
		val.push_back(88 - i);
	dynamic_cast < PmtMetadataT < vector <
		unsigned char > > *>(md.ptr())->value() = val;
	md->print(cout);
	cout << endl;
	outAcc.writeMetadata(md);

	md = rootMd->getMetadatum("CaptureConditions.PrintQuantity");
	dynamic_cast < PmtMetadataT < unsigned short >*>(md.ptr())->value() = 18;
	md->print(cout);
	cout << endl;
	outAcc.writeMetadata(md);

#ifndef PMT_USE_NO_XML_PARSER
	md = rootMd->getMetadatum("NewMd");
	dynamic_cast < PmtMetadataT < unsigned short >*>(md.ptr())->value() = 10;
	md->print(cout);
	cout << endl;
	outAcc.writeMetadata(md);
#endif

	md = rootMd->getMetadatum("DigitalProcess.History.EditTagArray");
	vector < unsigned short >val2;
	for (i = 0; i < 8; i++)
		val2.push_back(8 - i);
	dynamic_cast < PmtMetadataT < vector <
		unsigned short > > *>(md.ptr())->value() = val2;
	md->print(cout);
	cout << endl;
	outAcc.writeMetadata(md);

	NIFEXIF_Close(&hIn);
	NIFEXIF_Close(&hOut);


	// test for reading
	status =
		NIFEXIF_OpenByFileName(outExif, &fileType, &imgInfo, NIF_OPEN_READONLY,
		&hIn);
	if (status != NIF_OK)
	{
		sprintf(buf, "testExifAcc: can't open file %s\n", outExif);
		cout << buf;
		exit(-1);
	}
	PmtExifAccessor rAcc(&hIn);
	sprintf(buf, "\nReading metadata from %s:\n", outExif);
	cout << buf;

	md = rootMd->getMetadatum("ImageContainer.Orientation");
	inAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureConditions.ExposureTime");
	inAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureDevice.Make");
	inAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureDevice.Model");
	inAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureDevice.MakerNotes");
	inAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("SceneContent.Audio");
	inAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureConditions.PrintQuantity");
	inAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

#ifndef PMT_USE_NO_XML_PARSER
	md = rootMd->getMetadatum("NewMd");
	inAcc->readMetadata(md);
	md->print(cout);
	cout << endl;
#endif

	md = rootMd->getMetadatum("DigitalProcess.History.EditTagArray");
	inAcc->readMetadata(md);
	md->print(cout);
	cout << endl;


	NIFEXIF_Close(&hIn);

	rootMd->deleteMetadata("*");

}
#endif

#if defined( HAVE_KIESEXIF ) || defined( HAVE_OPENEXIF )
//=========================================================
// Test the Exif Accessor with images from various sources
//=========================================================
void testExifAccVariousSources()
{
	//For various images, the image metadata will be read
	//and some values will be compared to expected values.
	cout << "Testing the PmtExifAccessor with images from various sources..."
		<< endl;

	//Files to be used for testing of the Exif Accessor.
	char dc220Exif[] = "Kodak_DC220.jpg";
	char dc290Exif[] = "Kodak_DC290.jpg";
	char dc4800Exif[] = "Kodak_DC4800.jpg";
	char mc3Exif[] = "Kodak_MC3.jpg";
	char palmpixExif[] = "Kodak_Palmpix.jpg";
	char pictureCDExif[] = "Kodak_PictureCD.jpg";

	char *mdKey;                // key currently being read and verified
	EkString value;             // value of the metadata read from the image file
	EkString expectedValue;     // value expected to be read from the image file

	//get accessor and read metadata from DC220 image
	cout << "Reading metadata from: " << dc220Exif << "..." << endl;
	PmtAccessorPtr accIn = PmtAccessor::getAccessor(dc220Exif);
	PmtMetadataPtr rootMd = accIn->readMetadata();

	//check for some metadata keys, values
	cout << "Comparing some obtained values with expected values..." << endl <<
		endl;

	mdKey = "CaptureConditions.Fnumber";
	expectedValue = EK_L("4.76");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "CaptureConditions.MeteringMode";
	expectedValue = EK_L("2");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "CaptureDevice.Make";
	expectedValue = EK_L("Eastman Kodak Company");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "ImageContainer.CompressedBitsPerPixel";
	expectedValue = EK_L("2.4");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	rootMd->deleteMetadata("*");

	//get accessor and read metadata from DC290 image
	cout << "Reading metadata from: " << dc290Exif << "..." << endl;
	accIn = PmtAccessor::getAccessor(dc290Exif);
	rootMd = accIn->readMetadata();

	//check for some metadata keys, values
	cout << "Comparing some obtained values with expected values..." << endl <<
		endl;

	mdKey = "CaptureConditions.Aperture";
	expectedValue = EK_L("5");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "CaptureConditions.SubjectDistance";
	expectedValue = EK_L("5.88");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "ImageContainer.ActualImageHeight";
	expectedValue = EK_L("480");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "ImageContainer.Thumbnail.DisplayXResolution";
	expectedValue = EK_L("72");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	rootMd->deleteMetadata("*");

	//get accessor and read metadata from DC4800 image
	cout << "Reading metadata from: " << dc4800Exif << "..." << endl;
	accIn = PmtAccessor::getAccessor(dc4800Exif);
	rootMd = accIn->readMetadata();

	//check for some metadata keys, values
	cout << "Comparing some obtained values with expected values..." << endl <<
		endl;

	mdKey = "CaptureConditions.ExposureIndex";
	expectedValue = EK_L("200");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "CaptureDevice.SensingMethod";
	expectedValue = EK_L("2");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "DigitalProcess.ImageSource";
	expectedValue = EK_L("3");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "ImageContainer.Thumbnail.Orientation";
	expectedValue = EK_L("1");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "SceneContent.SceneType";
	expectedValue = EK_L("1");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	rootMd->deleteMetadata("*");

	//get accessor and read metadata from MC3 image
	cout << "Reading metadata from: " << mc3Exif << "..." << endl;
	accIn = PmtAccessor::getAccessor(mc3Exif);
	rootMd = accIn->readMetadata();

	//check for some metadata keys, values
	cout << "Comparing some obtained values with expected values..." << endl <<
		endl;

	mdKey = "CaptureConditions.FocalLength";
	expectedValue = EK_L("5.6");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "CaptureDevice.Model";
	expectedValue = EK_L("Kodak mc3");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "DigitalProcess.ImageSource";
	expectedValue = EK_L("3");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "ImageContainer.ColorSpaceInformation";
	expectedValue = EK_L("1");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "ImageContainer.Thumbnail.Compression";
	expectedValue = EK_L("6");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "SceneContent.ImageDescription";
	expectedValue = EK_L("Exif_JPEG");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	rootMd->deleteMetadata("*");

	//get accessor and read metadata from Palmpix image
	cout << "Reading metadata from: " << palmpixExif << "..." << endl;
	accIn = PmtAccessor::getAccessor(palmpixExif);
	rootMd = accIn->readMetadata();

	//check for some metadata keys, values
	cout << "Comparing some obtained values with expected values..." << endl <<
		endl;

	mdKey = "CaptureDevice.Make";
	expectedValue = EK_L("EASTMAN KODAK COMPANY");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "ImageContainer.DisplayResolutionUnit";
	expectedValue = EK_L("2");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "ImageContainer.Thumbnail.Compression";
	expectedValue = EK_L("6");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	rootMd->deleteMetadata("*");

	//get accessor and read metadata from PictureCD image
	cout << "Reading metadata from: " << pictureCDExif << "..." << endl;
	accIn = PmtAccessor::getAccessor(pictureCDExif);
	rootMd = accIn->readMetadata();

	//check for some metadata keys, values
	cout << "Comparing some obtained values with expected values..." << endl <<
		endl;

	/* APP3 key
	mdKey = "CaptureDevice.FilmGencode";
	expectedValue = EK_L("2");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if( value != expectedValue )
	{
	throw PmtError(PMT_ERROR, "Obtained value didn't match expected value!", WHERE);
	}
	*/

	mdKey = "CaptureDevice.Software";
	expectedValue = EK_L("Digital Lab System");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "ImageContainer.ActualImageWidth";
	expectedValue = EK_L("1500");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}

	mdKey = "ImageContainer.Thumbnail.DisplayResolutionUnit";
	expectedValue = EK_L("2");
	value = rootMd->getMetadatum(mdKey)->getValueStr();
	if (value != expectedValue)
	{
		throw PmtError(PMT_ERROR,
			"Obtained value didn't match expected value!", WHERE);
	}
	rootMd->deleteMetadata("*");

}                               // testExifAccVariousSources


//=================================================================
// Test writing metadata to an Exif image and then reading it back
//=================================================================
void testExifAccAllKeys(bool closeFileBeforeReading, int id,
						char *exifOriginal)
{
	//Every single key in the default schema will be tested...
	cout << "Testing all keys with the PmtExifAccessor..." << endl;

	if (closeFileBeforeReading)
	{
		cout <<
			"The file is closed after writing the metadata and before reading it back..."
			<< endl;
	}
	else
	{
		cout <<
			"The file is not closed after writing the metadata and before reading it back..."
			<< endl;
	}

	// Make a copy for each thread...
	char exifCopy[128];
	sprintf(exifCopy, "exif_copy.%d.jpg", id);

	//Create a copy of the image for writing to and reading from
	PmtCopyFile(exifOriginal, exifCopy, true);

	//Get an accessor for the image
	PmtExifAccessor exifAc;
	PmtAccessor *exifAccessor = &exifAc;
	if (!exifAccessor->open(exifCopy, PMT_READWRITE))
	{
		cout << "Could not open file: " << exifCopy << endl;
		return;
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
	}

	testExifAccFloat(exifAccessor, exifCopy, closeFileBeforeReading);
	testExifAccUnsignedByte(exifAccessor, exifCopy, closeFileBeforeReading);
	testExifAccBoolean(exifAccessor, exifCopy, closeFileBeforeReading);
	testExifAccString(exifAccessor, exifCopy, closeFileBeforeReading);
	testExifAccUnsignedShort(exifAccessor, exifCopy, closeFileBeforeReading);
	testExifAccUnsignedInt(exifAccessor, exifCopy, closeFileBeforeReading);
	testExifAccPmt_vuint8(exifAccessor, exifCopy, closeFileBeforeReading);
	testExifAccPmt_vuint16(exifAccessor, exifCopy, closeFileBeforeReading);
	testExifAccPmt_vfloat(exifAccessor, exifCopy, closeFileBeforeReading);
	testExifAccPmt_vuint32(exifAccessor, exifCopy, closeFileBeforeReading);
	testExifAccPmt_vint16(exifAccessor, exifCopy, closeFileBeforeReading);
	testExifAccPmt_vint32(exifAccessor, exifCopy, closeFileBeforeReading);
	testExifAccByte(exifAccessor, exifCopy, closeFileBeforeReading);
	exifAccessor->close();

	//exifAccessor->open(exifCopy);
	//cout << exifAccessor->readMetadata() << endl;
	//exifAccessor->close();

	// ************************************************************************
	// Testing the functionality DigitalProcess.Jpeg keys... Will not test
	// reading or writing the keys with the accessors at this point
	// ************************************************************************

	//Create two md roots, one for the metadata to be written to file, the
	//other for the metadata to be read from the file afterwords.
	PmtMetadataPtr rootMdOrig = PmtMetadata::create();
	PmtMetadataPtr rootMdCopy = PmtMetadata::create();

	//Two md ptrs for handling metadata objects within the two roots.
	PmtMetadataPtr mdHandleOrig;
	PmtMetadataPtr mdHandleCopy;

	vector < unsigned char >origVector(0);
	vector < unsigned char >copyVector(0);

	origVector.push_back('a');
	origVector.push_back('b');
	origVector.push_back('c');
	origVector.push_back('d');
	origVector.push_back('e');
	origVector.push_back('f');
	origVector.push_back('g');
	origVector.push_back('h');
	origVector.push_back('i');
	origVector.push_back('j');
	origVector.push_back('k');

	const int jpegKeys = 4;
	char *keys[jpegKeys];
	keys[0] = "DigitalProcess.Jpeg.AlphaQTable";
	keys[1] = "DigitalProcess.Jpeg.Chroma1QTable";
	keys[2] = "DigitalProcess.Jpeg.Chroma2QTable";
	keys[3] = "DigitalProcess.Jpeg.LuminQTable";

	for (int j = 0; j < jpegKeys; j++)
	{
		mdHandleOrig = rootMdOrig->getMetadatum(keys[j]);
		dynamic_cast < PmtMetadataT < vector <
			unsigned char > > *>(mdHandleOrig.ptr())->value() = origVector;
		copyVector =
			dynamic_cast < PmtMetadataT < vector <
			unsigned char > > *>(mdHandleOrig.ptr())->value();

		if (copyVector != origVector)
		{
			cout << "Error with " << keys[j] << ":" << endl;
			//  throw PmtError(PMT_ERROR,
			//      "Value read from file is different than the value written.", WHERE);
		}
	}
}

//=====================
// Test float elements
//=====================
void testExifAccFloat(PmtAccessor * exifAccessor, char *exifCopy,
					  bool closeFileBeforeReading)
{
	//Create two md roots, one for the metadata to be written to file, the
	//other for the metadata to be read from the file afterwords.
	PmtMetadataPtr rootMdOrig = PmtMetadata::create();
	PmtMetadataPtr rootMdCopy = PmtMetadata::create();

	//Two md ptrs for handling metadata objects within the two roots.
	PmtMetadataPtr mdHandleOrig;
	PmtMetadataPtr mdHandleCopy;

	const int fElements = 28;   // # of float elements in the default schema
	KeyTestInfo < float >fKeys[fElements];
	fKeys[0] =
		KeyTestInfo < float >("CaptureConditions.Aperture", (float) 2.8);
	fKeys[1] =
		KeyTestInfo < float >("CaptureConditions.BatteryLevel", (float) 45.77,
		false);
	fKeys[2] =
		KeyTestInfo < float >("CaptureConditions.Brightness",
		(float) 13778.99);
	fKeys[3] =
		KeyTestInfo < float >("CaptureConditions.ExposureBias",
		(float) -13.987);
	fKeys[4] =
		KeyTestInfo < float >("CaptureConditions.ExposureIndex",
		(float) 11.13);
	fKeys[5] =
		KeyTestInfo < float >("CaptureConditions.ExposureTime", (float) 0.05);
	fKeys[6] =
		KeyTestInfo < float >("CaptureConditions.Flash.Energy",
		(float) 1387.33);
	fKeys[7] =
		KeyTestInfo < float >("CaptureConditions.Fnumber", (float) 3.69);
	fKeys[8] =
		KeyTestInfo < float >("CaptureConditions.FocalLength", (float) 300.50);
	fKeys[9] =
		KeyTestInfo < float >("CaptureConditions.Magnification", (float) 7.75);
	fKeys[10] =
		KeyTestInfo < float >("CaptureConditions.MaxAperture", (float) 2.05);
	fKeys[11] =
		KeyTestInfo < float >("CaptureConditions.ShutterSpeed", (float) 0.05);
	fKeys[12] =
		KeyTestInfo < float >("CaptureConditions.SubjectDistance",
		(float) 3.5);
	fKeys[13] =
		KeyTestInfo < float >("CaptureDevice.FocalPlaneXResolution",
		(float) 72);
	fKeys[14] =
		KeyTestInfo < float >("CaptureDevice.FocalPlaneYResolution",
		(float) 72);
	fKeys[15] =
		KeyTestInfo < float >("ImageContainer.CompressedBitsPerPixel",
		(float) 11.99);
	fKeys[16] =
		KeyTestInfo < float >("ImageContainer.DisplayXResolution", (float) 72);
	fKeys[17] =
		KeyTestInfo < float >("ImageContainer.DisplayYResolution", (float) 72);
	fKeys[18] =
		KeyTestInfo < float >("ImageContainer.Thumbnail.DisplayXResolution",
		(float) 72);
	fKeys[19] =
		KeyTestInfo < float >("ImageContainer.Thumbnail.DisplayYResolution",
		(float) 72);
	fKeys[20] =
		KeyTestInfo < float >("SceneContent.GPS.Altitude", (float) 4275.75);
	fKeys[21] = KeyTestInfo < float >("SceneContent.GPS.DOP", (float) 30.85);
	fKeys[22] =
		KeyTestInfo < float >("SceneContent.GPS.DestBearing", (float) 288.45);
	fKeys[23] =
		KeyTestInfo < float >("SceneContent.GPS.DestDistance", (float) 1609);
	fKeys[24] =
		KeyTestInfo < float >("SceneContent.GPS.ImgDirection", (float) 359);
	fKeys[25] =
		KeyTestInfo < float >("SceneContent.GPS.Speed", (float) 212.35);
	fKeys[26] = KeyTestInfo < float >("SceneContent.GPS.Track", (float) 555.5);
	fKeys[27] =
		KeyTestInfo < float >("CaptureConditions.DigitalZoomRatio",
		(float) 2.5);

	//Variables for holding the values of md objects
	float fValueOrig;
	float fValueCopy;

	if (closeFileBeforeReading)
	{
		exifAccessor->open(exifCopy, PMT_READWRITE);
	}

	// loop through the float elements, writing values to the file
	int i;
	for (i = 0; i < fElements; i++)
	{
		mdHandleOrig = rootMdOrig->getMetadatum(fKeys[i].getKey());
		dynamic_cast < PmtMetadataT < float >*>(mdHandleOrig.ptr())->value() =
			fKeys[i].getValue();

		exifAccessor->writeMetadata(mdHandleOrig);

		rootMdOrig->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
		exifAccessor->open(exifCopy);
	}

	// loop through the float elements, reading values from the file and
	// comparing them to the values written (if supported)
	for (i = 0; i < fElements; i++)
	{
		mdHandleCopy = rootMdCopy->getMetadatum(fKeys[i].getKey());

		exifAccessor->readMetadata(mdHandleCopy);

		fValueOrig = fKeys[i].getValue();
		fValueCopy =
			dynamic_cast < PmtMetadataT <
			float >*>(mdHandleCopy.ptr())->value();

		// Check if the key is supported. If so, check if the value read is
		// equal to the value written. If not, check if value is 0, which is
		// the default value.
		if (fKeys[i].isSupported())
		{
			if (fValueOrig != fValueCopy)
			{
				cout << "Error with " << fKeys[i].getKey() << ":" << endl;
				cout << "Value written: " << fValueOrig << endl;
				cout << "Value read: " << fValueCopy << endl;
				//  throw PmtError(PMT_ERROR,
				//      "Value read from file is different than the value written.", WHERE);
			}
		}
		else
		{
			if (fValueCopy != 0)
			{
				cout << "Error with " << fKeys[i].getKey() << ":" << endl;
				cout << "This key is not supposed to be supported in EXIF." <<
					endl;
			}
		}

		rootMdCopy->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
	}
}                               // testExifAccFloat

//============================
// Test unsignedByte elements
//============================
void testExifAccUnsignedByte(PmtAccessor * exifAccessor, char *exifCopy,
							 bool closeFileBeforeReading)
{
	//Create two md roots, one for the metadata to be written to file, the
	//other for the metadata to be read from the file afterwords.
	PmtMetadataPtr rootMdOrig = PmtMetadata::create();
	PmtMetadataPtr rootMdCopy = PmtMetadata::create();

	//Two md ptrs for handling metadata objects within the two roots.
	PmtMetadataPtr mdHandleOrig;
	PmtMetadataPtr mdHandleCopy;

	const int ubElements = 35;  // # of unsignedByte elements in the default schema
	KeyTestInfo < unsigned char >ubKeys[ubElements];
	int i = 0;
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.Flash.Return",
		(unsigned char) 'r');
	//GPSAltitudeRef should have a value of 0 or 1. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("SceneContent.GPS.AltitudeRef",
		(unsigned char) 1);
	//SceneType has a default value of 1. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("SceneContent.SceneType",
		(unsigned char) 1);
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("DigitalProcess.ImageSource",
		(unsigned char) 7);
	//ExposureProgram should have a value between 0 and 8. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.ExposureProgram",
		(unsigned char) 5);
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.Flash.Flash",
		(unsigned char) 7);
	//LightSource should have a value between 0 and 24 or 255. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.LightSource",
		(unsigned char) 23);
	//MeteringMode should have a value between 0 and 6 or 255. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.MeteringMode",
		(unsigned char) 4);
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.PAR",
		(unsigned char) 50);
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureDevice.FilmCategory",
		(unsigned char) 77);
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureDevice.FilmSize",
		(unsigned char) 121);
	//ResolutionUnit should have a value of 2 (inches) or 3 (cm). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureDevice.FocalPlaneResolutionUnit",
		(unsigned char) 2);
	//SensingMethod should have a value between 1 and 8. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureDevice.SensingMethod",
		(unsigned char) 7);
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("DigitalProcess.ImageRotationStatus",
		(unsigned char) 27);
	//ResolutionUnit should have a value of 2 (inches) or 3 (cm). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("ImageContainer.DisplayResolutionUnit",
		(unsigned char) 2);
	//Orientation should have a value between 1 and 8. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("ImageContainer.Orientation",
		(unsigned char) 4);
	//PhotometricInterpretation should have a value of 2 (RGB) or 6 (YCbCr). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo <
		unsigned char >("ImageContainer.PhotometricInterpretation",
		(unsigned char) 2);
	//PlanarConfiguration should have a value of 1 (chunky) or 2 (planar). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("ImageContainer.PlanarConfiguration",
		(unsigned char) 1);
	//ResolutionUnit should have a value of 2 (inches) or 3 (cm). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo <
		unsigned char >("ImageContainer.Thumbnail.DisplayResolutionUnit",
		(unsigned char) 2);
	//Orientation should have a value between 1 and 8. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("ImageContainer.Thumbnail.Orientation",
		(unsigned char) 1);
	//PhotometricInterpretation should have a value of 2 (RGB) or 6 (YCbCr). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo <
		unsigned char >("ImageContainer.Thumbnail.PhotometricInterpretation",
		(unsigned char) 2);
	//PlanarConfiguration should have a value of 1 (chunky) or 2 (planar). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo <
		unsigned char >("ImageContainer.Thumbnail.PlanarConfiguration",
		(unsigned char) 1);
	//YCbCrPositioning sould have a value of 1 (centered) or 2 (co-sited). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo <
		unsigned char >("ImageContainer.Thumbnail.YCbCrPositioning",
		(unsigned char) 2);
	//YCbCrPositioning sould have a value of 1 (centered) or 2 (co-sited). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("ImageContainer.YCbCrPositioning",
		(unsigned char) 2);
	//Contrast should have a value between 0 and 2. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.Contrast",
		(unsigned char) 2);
	//CustomRendered should have a value of 0 or 1. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.CustomRendered",
		(unsigned char) 1);
	//ExposureMode should have a value of 0, 1, or 2. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.ExposureMode",
		(unsigned char) 1);
	//GPSDifferential should have a value of 0 or 1. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("SceneContent.GPS.Differential",
		(unsigned char) 1);
	//GainControl should have a value between 0 and 4. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.GainControl",
		(unsigned char) 4);
	//Saturation should have a value between 0 and 2. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.Saturation",
		(unsigned char) 1);
	//SceneCaptureType should have a value between 0 and 3. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.SceneCaptureType",
		(unsigned char) 3);
	//Sharpness should have a value between 0 and 2. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.Sharpness",
		(unsigned char) 0);
	//SubjectDistanceRange should have a value between 0 and 3. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.SubjectDistanceRange",
		(unsigned char) 3);
	//WhiteBalance should have a value of 0 or 1. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.WhiteBalance",
		(unsigned char) 0);
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("DigitalProcess.ImagePrintStatus",
		(unsigned char) 2);

	//Variables for holding the values of md objects
	unsigned char ubValueOrig;
	unsigned char ubValueCopy;

	if (closeFileBeforeReading)
	{
		exifAccessor->open(exifCopy, PMT_READWRITE);
	}

	for (i = 0; i < ubElements; i++)
	{
		mdHandleOrig = rootMdOrig->getMetadatum(ubKeys[i].getKey());
		dynamic_cast < PmtMetadataT <
			unsigned char >*>(mdHandleOrig.ptr())->value() =
			ubKeys[i].getValue();

		exifAccessor->writeMetadata(mdHandleOrig);

		rootMdOrig->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
		exifAccessor->open(exifCopy);
	}

	for (i = 0; i < ubElements; i++)
	{
		mdHandleCopy = rootMdCopy->getMetadatum(ubKeys[i].getKey());

		exifAccessor->readMetadata(mdHandleCopy);

		ubValueOrig = ubKeys[i].getValue();
		ubValueCopy =
			dynamic_cast < PmtMetadataT <
			unsigned char >*>(mdHandleCopy.ptr())->value();

		if (ubKeys[i].isSupported())
		{
			if (ubValueOrig != ubValueCopy)
			{
				cout << "Error with " << ubKeys[i].getKey() << ":" << endl;
				cout << "Value written: " << ubValueOrig << endl;
				cout << "Value read: " << ubValueCopy << endl;
				//  throw PmtError(PMT_ERROR,
				//      "Value read from file is different than the value written.", WHERE);
			}
		}
		else
		{
			if (ubValueCopy != 0)
			{
				cout << "Error with " << ubKeys[i].getKey() << ":" << endl;
				cout << "This key is not supposed to be supported in EXIF." <<
					endl;
			}
		}

		rootMdCopy->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
	}
}                               // testExifAccUnsignedByte

//=======================
// Test boolean elements
//=======================
void testExifAccBoolean(PmtAccessor * exifAccessor, char *exifCopy,
						bool closeFileBeforeReading)
{
	//Create two md roots, one for the metadata to be written to file, the
	//other for the metadata to be read from the file afterwords.
	PmtMetadataPtr rootMdOrig = PmtMetadata::create();
	PmtMetadataPtr rootMdCopy = PmtMetadata::create();

	//Two md ptrs for handling metadata objects within the two roots.
	PmtMetadataPtr mdHandleOrig;
	PmtMetadataPtr mdHandleCopy;

	const int bElements = 1;    // # of boolean elements in the default schema
	KeyTestInfo < bool > bKeys[bElements];
	bKeys[0] = KeyTestInfo < bool > ("CaptureConditions.Flash.Fired", true);

	//Variables for holding the values of md objects
	bool bValueOrig;
	bool bValueCopy;

	if (closeFileBeforeReading)
	{
		exifAccessor->open(exifCopy, PMT_READWRITE);
	}

	int i;
	for (i = 0; i < bElements; i++)
	{
		mdHandleOrig = rootMdOrig->getMetadatum(bKeys[i].getKey());
		dynamic_cast < PmtMetadataT < bool > *>(mdHandleOrig.ptr())->value() =
			bKeys[i].getValue();

		exifAccessor->writeMetadata(mdHandleOrig);

		rootMdOrig->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
		exifAccessor->open(exifCopy);
	}

	for (i = 0; i < bElements; i++)
	{
		mdHandleCopy = rootMdCopy->getMetadatum(bKeys[i].getKey());

		exifAccessor->readMetadata(mdHandleCopy);

		bValueOrig = bKeys[i].getValue();
		bValueCopy =
			dynamic_cast < PmtMetadataT < bool >
			*>(mdHandleCopy.ptr())->value();

		if (bKeys[i].isSupported())
		{
			if (bValueOrig != bValueCopy)
			{
				cout << "Error with " << bKeys[i].getKey() << ":" << endl;
				cout << "Value written: " << bValueOrig << endl;
				cout << "Value read: " << bValueCopy << endl;
				//  throw PmtError(PMT_ERROR,
				//      "Value read from file is different than the value written.", WHERE);
			}
		}
		else
		{
			if (bValueCopy != 0)
			{
				cout << "Error with " << bKeys[i].getKey() << ":" << endl;
				cout << "This key is not supposed to be supported in EXIF." <<
					endl;
			}
		}

		rootMdCopy->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
	}
}                               // testExifAccBoolean

//======================
// Test string elements
//======================
void testExifAccString(PmtAccessor * exifAccessor, char *exifCopy,
					   bool closeFileBeforeReading)
{
	//Create two md roots, one for the metadata to be written to file, the
	//other for the metadata to be read from the file afterwords.
	PmtMetadataPtr rootMdOrig = PmtMetadata::create();
	PmtMetadataPtr rootMdCopy = PmtMetadata::create();

	//Two md ptrs for handling metadata objects within the two roots.
	PmtMetadataPtr mdHandleOrig;
	PmtMetadataPtr mdHandleCopy;

	int i = 0;
	const int sElements = 39;   // # of string elements in the default schema
	KeyTestInfo < string > sKeys[sElements];
	sKeys[i++] =
		KeyTestInfo < string > ("CaptureConditions.SecurityClassification",
		"Top Secret", false);
	sKeys[i++] =
		KeyTestInfo < string > ("CaptureDevice.CameraOwner.CameraOwner",
		"The Owner");
	sKeys[i++] =
		KeyTestInfo < string > ("CaptureDevice.HostComputer",
		"My Computer", false);
	sKeys[i++] =
		KeyTestInfo < string > ("CaptureDevice.Make",
		"Eastman Kodak Company");
	sKeys[i++] = KeyTestInfo < string > ("CaptureDevice.Model", "DCS 660");

	sKeys[i++] =
		KeyTestInfo < string > ("CaptureDevice.Scanner.ModelAndVersion", "EK9000");

	sKeys[i++] =
		KeyTestInfo < string > ("CaptureDevice.Software", "GNU GIMP");
	sKeys[i++] =
		KeyTestInfo < string > ("CaptureDevice.SpectralSensitivity",
		"Tungsten Balanced");
	//DateTime should be in the format "YYYY:MM:DD HH:MM:SS"
	sKeys[i++] =
		KeyTestInfo < string > ("DigitalProcess.ImageFileChangeDateTime",
		"2002:05:21 10:07:33");
	//DateTime should be in the format "YYYY:MM:DD HH:MM:SS"
	sKeys[i++] =
		KeyTestInfo < string > ("DigitalProcess.ImageFileCreationDateTime",
		"2000:12:25 11:00:00");
	sKeys[i++] =
		KeyTestInfo < string > ("ImageContainer.Interoperability.Index",
		"R98");
	sKeys[i++] =
		KeyTestInfo < string > ("IntellectualProperty.Copyright",
		"Photog 2001");
	sKeys[i++] =
		KeyTestInfo < string > ("IntellectualProperty.ImageCreator",
		"The Owner");
	sKeys[i++] =
		KeyTestInfo < string > ("IntellectualProperty.Photographer",
		"The Owner");
	//GPSDestBearingRef should have a value of 'T' or 'M'. Other values are reserved.
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.DestBearingRef", "T");
	//GPSDestDistanceRef should have a value of 'K', 'M', or 'N'. Other values are reserved.
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.DestDistanceRef", "K");
	//GPSDestLatitudeRef should have a value of 'N' or 'S'. Other values are reserved.
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.DestLatitudeRef", "S");
	//GPSDestLongitudeRef should have a value of 'E' or 'W'. Other value are reserved.
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.DestLongitudeRef", "W");
	//GPSImgDirectionRef should have a value of 'T' or 'M'. Ther values are reserved.
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.ImgDirectionRef", "M");
	//GPSLatitudeRef should have a value of 'N' or 'S'.
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.LatitudeRef", "N");
	//GPSLongitudeRef should have a value of 'E' or 'W'
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.LongitudeRef", "E");
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.MapDatum", "TOKYO");
	//GPSMeasureMode should have a value of '2' or '3'. Other values are reserved.
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.MeasureMode", "3");
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.Satellites", "sixteen");
	//GPSSpeedRef should have a value of 'K', 'M', or 'N'. Other values are reserved.
	sKeys[i++] = KeyTestInfo < string > ("SceneContent.GPS.SpeedRef", "M");
	//GPSStatus should have a value of 'A' or 'V'.
	sKeys[i++] = KeyTestInfo < string > ("SceneContent.GPS.Status", "V");
	//GPSTrackRef should have a value of 'T' or 'M'. Other values are reserved.
	sKeys[i++] = KeyTestInfo < string > ("SceneContent.GPS.TrackRef", "T");
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GroupCaption.GroupCaption",
		"ISSL Lab");
	sKeys[i++] =
		KeyTestInfo < string >
		("SceneContent.GroupCaption.UserInputGroupTitle", "ISSL Lab Picture");
	//DateTime should be in the format "YYYY:MM:DD HH:MM:SS"
	sKeys[i++] =
		KeyTestInfo < string >
		("SceneContent.ImageCaptureDateTime.ImageCaptureDateTime",
		"2001:08:24 10:09:27");
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.ImageDescription",
		"very pretty");
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.RelatedSoundFile",
		"SND00001.WAV");
	//DateTime should be in the format "YYYY:MM:DD HH:MM:SS"
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.SoundCaptureDateTime",
		"2001:08:24 10:09:27", false);
	sKeys[i++] =
		KeyTestInfo < string >
		("SceneContent.UserComment.UserInputFrameTitle", "birthday pictures");
	sKeys[i++] =
		KeyTestInfo < string >
		("SceneContent.UserComment.UserSelectFrameTitle",
		"anniversary pictures");
	//GPSDateStamp should be in the format "YYYY:MM:DD".
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.DateStamp",
		"2002:05:14");
	//ImageUniqueID should have a Count of 33 (including null termination character).
	sKeys[i++] =
		KeyTestInfo < string > ("ImageContainer.ImageUniqueID",
		"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
	sKeys[i++] =
		KeyTestInfo < string > ("CaptureDevice.FilmGencode", "GenCode");
	sKeys[i++] =
		KeyTestInfo < string > ("CaptureDevice.FilmProductCode", "ProdCode");

	//Variables for holding the values of md objects
	string sValueOrig;
	string sValueCopy;

	if (closeFileBeforeReading)
	{
		exifAccessor->open(exifCopy, PMT_READWRITE);
	}

	for (i = 0; i < sElements; i++)
	{
		mdHandleOrig = rootMdOrig->getMetadatum(sKeys[i].getKey());

		dynamic_cast < PmtMetadataT < string >
			*>(mdHandleOrig.ptr())->value() = sKeys[i].getValue();

		exifAccessor->writeMetadata(mdHandleOrig);

		rootMdOrig->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
		exifAccessor->open(exifCopy);
	}

	for (i = 0; i < sElements; i++)
	{
		mdHandleCopy = rootMdCopy->getMetadatum(sKeys[i].getKey());

		exifAccessor->readMetadata(mdHandleCopy);

		sValueOrig = sKeys[i].getValue();
		sValueCopy =
			dynamic_cast < PmtMetadataT < string >
			*>(mdHandleCopy.ptr())->value();

		if (sKeys[i].isSupported())
		{
			if (sValueOrig != sValueCopy)
			{
				cout << "Error with " << sKeys[i].getKey() << ":" << endl;
				cout << "Value written: " << sValueOrig << endl;
				cout << "Value read: " << sValueCopy << endl;
				//  throw PmtError(PMT_ERROR,
				//      "Value read from file is different than the value written.", WHERE);
			}
		}
		else
		{
			if (sValueCopy != "")
			{
				cout << "Error with " << sKeys[i].getKey() << ":" << endl;
				cout << "This key is not supposed to be supported in EXIF." <<
					endl;
			}
		}

		rootMdCopy->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
	}
}                               // testExifAccString

//=============================
// Test unsignedShort elements
//=============================
void testExifAccUnsignedShort(PmtAccessor * exifAccessor, char *exifCopy,
							  bool closeFileBeforeReading)
{
	//Create two md roots, one for the metadata to be written to file, the
	//other for the metadata to be read from the file afterwords.
	PmtMetadataPtr rootMdOrig = PmtMetadata::create();
	PmtMetadataPtr rootMdCopy = PmtMetadata::create();

	//Two md ptrs for handling metadata objects within the two roots.
	PmtMetadataPtr mdHandleOrig;
	PmtMetadataPtr mdHandleCopy;

	int i = 0;
	const int usElements = 18;  // # of unsignedShort elements in the default schema
	KeyTestInfo < unsigned short >usKeys[usElements];
	usKeys[i++] =
		KeyTestInfo < unsigned short >("CaptureConditions.PrintQuantity",
		(unsigned short) 33);
	usKeys[i++] =
		KeyTestInfo < unsigned short >("CaptureConditions.SelfTimerMode",
		(unsigned short) 3, false);
	usKeys[i++] =
		KeyTestInfo < unsigned short >("CaptureDevice.ISOSpeedRating",
		(unsigned short) 640);
	usKeys[i++] =
		KeyTestInfo < unsigned short >("CaptureDevice.Interlace",
		(unsigned short) 45, false);
	usKeys[i++] =
		KeyTestInfo <
		unsigned short >("DigitalProcess.History.SBAInputImageColorspace",
		(unsigned short) 1);
	usKeys[i++] =
		KeyTestInfo < unsigned short >("DigitalProcess.CellLength",
		(unsigned short) 480, false);
	usKeys[i++] =
		KeyTestInfo < unsigned short >("DigitalProcess.CellWidth",
		(unsigned short) 600, false);
	usKeys[i++] =
		KeyTestInfo <
		unsigned short >("DigitalProcess.ScanFrameSequenceNumber",
		(unsigned short) 4);
	usKeys[i++] =
		KeyTestInfo < unsigned short >("DigitalProcess.Thresholding",
		(unsigned short) 24, false);
	//SamplesPerPixel has a default value of 3.
	usKeys[i++] =
		KeyTestInfo < unsigned short >("ImageContainer.SamplesPerPixel",
		(unsigned short) 3);
	//SamplesPerPixel has a default value of 3.
	usKeys[i++] =
		KeyTestInfo <
		unsigned short >("ImageContainer.Thumbnail.SamplesPerPixel",
		(unsigned short) 3);
	usKeys[i++] =
		KeyTestInfo <
		unsigned short >("OutputOrder.SimpleRenderInst.FrameNumber",
		(unsigned short) 6);
	usKeys[i++] =
		KeyTestInfo <
		unsigned short >("CaptureConditions.FocalLengthIn35mmFilm",
		(unsigned short) 24);
	//Compression should have a value of 1 (uncompressed) or 6 
	//(JPEG compression). Other values are reserved.
	usKeys[i++] =
		KeyTestInfo < unsigned short >("ImageContainer.Compression",
		(unsigned short) 6);
	//Compression should have a value of 1 (uncompressed) or 6 (JPEG compression). Other values are reserved.
	usKeys[i++] =
		KeyTestInfo < unsigned short >("ImageContainer.Thumbnail.Compression",
		(unsigned short) 6);
	usKeys[i++] =
		KeyTestInfo < unsigned short >("DigitalProcess.ImageSourceEK",
		(unsigned short) 5);
	usKeys[i++] =
		KeyTestInfo < unsigned short >("ImageContainer.SoundType",
		(unsigned short) 277, false);
	//ColorSpace should have a value of 1 (sRGB) or FFFF.H (Uncalibrated).
	// Other values are reserved.
	usKeys[i++] =
		KeyTestInfo < unsigned short >("ImageContainer.ColorSpaceInformation",
		(unsigned short) 0xFFFF);

	//Variables for holding the values of md objects
	unsigned short usValueOrig;
	unsigned short usValueCopy;

	if (closeFileBeforeReading)
	{
		exifAccessor->open(exifCopy, PMT_READWRITE);
	}

	for (i = 0; i < usElements; i++)
	{
		mdHandleOrig = rootMdOrig->getMetadatum(usKeys[i].getKey());
		dynamic_cast < PmtMetadataT <
			unsigned short >*>(mdHandleOrig.ptr())->value() =
			usKeys[i].getValue();

		exifAccessor->writeMetadata(mdHandleOrig);

		rootMdOrig->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
		exifAccessor->open(exifCopy);
	}

	for (i = 0; i < usElements; i++)
	{
		mdHandleCopy = rootMdCopy->getMetadatum(usKeys[i].getKey());

		exifAccessor->readMetadata(mdHandleCopy);

		usValueOrig = usKeys[i].getValue();
		usValueCopy =
			dynamic_cast < PmtMetadataT <
			unsigned short >*>(mdHandleCopy.ptr())->value();

		if (usKeys[i].isSupported())
		{
			if (usValueOrig != usValueCopy)
			{
				cout << "Error with " << usKeys[i].getKey() << ":" << endl;
				cout << "Value written: " << usValueOrig << endl;
				cout << "Value read: " << usValueCopy << endl;
				//  throw PmtError(PMT_ERROR,
				//      "Value read from file is different than the value written.", WHERE);
			}
		}
		else
		{
			if (usValueCopy != 0)
			{
				cout << "Error with " << usKeys[i].getKey() << ":" << endl;
				cout << "This key is not supposed to be supported in EXIF." <<
					endl;
			}
		}

		rootMdCopy->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
	}
}                               // testExifAccUnsignedShort

//===========================
// Test unsignedInt elements
//===========================
void testExifAccUnsignedInt(PmtAccessor * exifAccessor, char *exifCopy,
							bool closeFileBeforeReading)
{
	//Create two md roots, one for the metadata to be written to file, the
	//other for the metadata to be read from the file afterwords.
	PmtMetadataPtr rootMdOrig = PmtMetadata::create();
	PmtMetadataPtr rootMdCopy = PmtMetadata::create();

	//Two md ptrs for handling metadata objects within the two roots.
	PmtMetadataPtr mdHandleOrig;
	PmtMetadataPtr mdHandleCopy;

	int i = 0;
	const int uiElements = 7;   // # of unsignedInt elements in the default schema
	KeyTestInfo < unsigned long >uiKeys[uiElements];
	uiKeys[i++] =
		KeyTestInfo < unsigned long >("CaptureConditions.ImageNumber",
		(unsigned long) 1277, false);
	uiKeys[i++] =
		KeyTestInfo < unsigned long >("ImageContainer.ActualImageHeight",
		(unsigned long) 600);
	uiKeys[i++] =
		KeyTestInfo < unsigned long >("ImageContainer.ActualImageWidth",
		(unsigned long) 480);
	//uiKeys[i++] = KeyTestInfo<unsigned long>( "ImageContainer.Height", (unsigned long)72 );
	uiKeys[i++] =
		KeyTestInfo < unsigned long >("ImageContainer.RowsPerStrip",
		(unsigned long) 5025);
	uiKeys[i++] =
		KeyTestInfo < unsigned long >("ImageContainer.Thumbnail.RowsPerStrip",
		(unsigned long) 11);
	uiKeys[i++] =
		KeyTestInfo < unsigned long >("ImageContainer.Thumbnail.Height",
		(unsigned long) 8999);
	uiKeys[i++] =
		KeyTestInfo < unsigned long >("ImageContainer.Thumbnail.Width",
		(unsigned long) 65000);
	//uiKeys[i++] = KeyTestInfo<unsigned long>( "ImageContainer.Width", (unsigned long)0 );
	//  uiKeys[i++] = KeyTestInfo<unsigned long>( "ImageContainer.Thumbnail.OffsetToJPEGSOI", (unsigned long)90545 );
	//  uiKeys[i++] = KeyTestInfo<unsigned long>( "ImageContainer.Thumbnail.BytesOfJPEGData", (unsigned long)250000 );


	//Variables for holding the values of md objects
	unsigned long uiValueOrig;
	unsigned long uiValueCopy;

	if (closeFileBeforeReading)
	{
		exifAccessor->open(exifCopy, PMT_READWRITE);
	}

	for (i = 0; i < uiElements; i++)
	{
		mdHandleOrig = rootMdOrig->getMetadatum(uiKeys[i].getKey());

		dynamic_cast < PmtMetadataT <
			unsigned long >*>(mdHandleOrig.ptr())->value() =
			uiKeys[i].getValue();

		exifAccessor->writeMetadata(mdHandleOrig);

		rootMdOrig->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
		exifAccessor->open(exifCopy);
	}

	for (i = 0; i < uiElements; i++)
	{
		mdHandleCopy = rootMdCopy->getMetadatum(uiKeys[i].getKey());

		exifAccessor->readMetadata(mdHandleCopy);

		uiValueOrig = uiKeys[i].getValue();
		uiValueCopy =
			dynamic_cast < PmtMetadataT <
			unsigned long >*>(mdHandleCopy.ptr())->value();

		if (uiKeys[i].isSupported())
		{
			if (uiValueOrig != uiValueCopy)
			{
				cout << "Error with " << uiKeys[i].getKey() << ":" << endl;
				cout << "Value written: " << uiValueOrig << endl;
				cout << "Value read: " << uiValueCopy << endl;
				//  throw PmtError(PMT_ERROR,
				//      "Value read from file is different than the value written.", WHERE);
			}
		}
		else
		{
			if (uiValueCopy != 0)
			{
				cout << "Error with " << uiKeys[i].getKey() << ":" << endl;
				cout << "This key is not supposed to be supported in EXIF." <<
					endl;
			}
		}

		rootMdCopy->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
	}
}                               // testExifAccUnsignedInt

//==========================
// Test pmt_vuint8 elements
//==========================
void testExifAccPmt_vuint8(PmtAccessor * exifAccessor, char *exifCopy,
						   bool closeFileBeforeReading)
{
	//Create two md roots, one for the metadata to be written to file, the
	//other for the metadata to be read from the file afterwords.
	PmtMetadataPtr rootMdOrig = PmtMetadata::create();
	PmtMetadataPtr rootMdCopy = PmtMetadata::create();

	//Two md ptrs for handling metadata objects within the two roots.
	PmtMetadataPtr mdHandleOrig;
	PmtMetadataPtr mdHandleCopy;

	const int vucElements = 30; // # of pmt_vuint8 elements in the default schema
	KeyTestInfo < vector < unsigned char > > vucKeys[vucElements];

	vector < unsigned char >ucVector(0);
	ucVector.push_back((unsigned char) 2);
	ucVector.push_back((unsigned char) 2);

	int i = 0;

	ucVector.clear();
	ucVector.push_back((unsigned char) 2);
	ucVector.push_back((unsigned char) 2);
	ucVector.push_back((unsigned char) 0);
	ucVector.push_back((unsigned char) 0);
	//GPSVersionID must have a count of 4 and should have a value of 2.2.0.0. Other values are reserved.
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("SceneContent.GPS.VersionID",
		ucVector);

	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("CaptureConditions.DeviceSettingDescription",
		ucVector);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("CaptureDevice.CFAPattern",
		ucVector);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("CaptureDevice.CameraOwner.EK",
		ucVector);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("CaptureDevice.MakerNotes",
		ucVector);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("CaptureDevice.Noise",
		ucVector, false);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("CaptureDevice.OECF",
		ucVector);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("CaptureDevice.SerialNumber.Camera", ucVector);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("CaptureDevice.SpatialFrequencyResponse", ucVector);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("DigitalProcess.History.SBAExposureRecord", ucVector);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("DigitalProcess.ICCProfile",
		ucVector, false);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("DigitalProcess.RollGuid",
		ucVector);

	ucVector.clear();
	ucVector.push_back(4);
	ucVector.push_back(5);
	ucVector.push_back(6);
	ucVector.push_back(0);
	//ComponentsConfiguration must have a Count of 4. All values must be between 0 and 6. Others are reserved.
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("ImageContainer.ComponentsConfiguration", ucVector);

	ucVector.clear();
	ucVector.push_back(0);
	ucVector.push_back(2);
	ucVector.push_back(2);
	ucVector.push_back(0);
	//ExifVersion must have a count of 4. Default is "0220"
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("ImageContainer.ExifVersion",
		ucVector);

	ucVector.clear();
	ucVector.push_back(0);
	ucVector.push_back(1);
	ucVector.push_back(0);
	ucVector.push_back(0);
	//FlashpixVersion must have a count of 4. Default is "0100". Other values are reserved.
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("ImageContainer.FlashpixVersion", ucVector);
	ucVector.push_back('r');

	//!!!
	//vucKeys[i++] = KeyTestInfo< vector<unsigned char> >( "ImageContainer.IPTC_NAA", ucVector );
	//ucVector.push_back('r');


	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("ImageContainer.MetadataNumber", ucVector);

	ucVector.clear();
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("ImageContainer.SourceImageDirectory", ucVector);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("ImageContainer.SourceImageFileName", ucVector);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("ImageContainer.SourceImageVolumeName", ucVector);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("ImageContainer.TiffepVersion",
		ucVector, false);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("OutputOrder.Information.DealerIDNumber", ucVector);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("OutputOrder.Information.EnvelopeNumber", ucVector);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("SceneContent.Audio",
		ucVector);

	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("SceneContent.UserComment.UserComment", ucVector);
	ucVector.clear();
	ucVector.push_back((unsigned char) 48);
	ucVector.push_back((unsigned char) 49);
	ucVector.push_back((unsigned char) 48);
	ucVector.push_back((unsigned char) 48);
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("ImageContainer.Interoperability.Version", ucVector);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("SceneContent.GPS.ProcessingMethod", ucVector);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("SceneContent.GPS.AreaInformation", ucVector);
	ucVector.push_back((unsigned char)3);
	vucKeys[i++] = 
		KeyTestInfo < vector <
		unsigned char > > ("CaptureDevice.FID", ucVector);
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("OutputOrder.Information.VendorOrderNumber",
		ucVector);
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("SceneContent.GroupCaption.UserSelectGroupTitle",
		ucVector);

	//Variables for holding the values of md objects
	vector < unsigned char >vucValueOrig(0);
	vector < unsigned char >vucValueCopy(0);

	if (closeFileBeforeReading)
	{
		exifAccessor->open(exifCopy, PMT_READWRITE);
	}

	for (i = 0; i < vucElements; i++)
	{
		mdHandleOrig = rootMdOrig->getMetadatum(vucKeys[i].getKey());
		dynamic_cast < PmtMetadataT < vector <
			unsigned char > > *>(mdHandleOrig.ptr())->value() =
			vucKeys[i].getValue();

		exifAccessor->writeMetadata(mdHandleOrig);

		rootMdOrig->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
		exifAccessor->open(exifCopy);
	}

	for (i = 0; i < vucElements; i++)
	{
		mdHandleCopy = rootMdCopy->getMetadatum(vucKeys[i].getKey());
		exifAccessor->readMetadata(mdHandleCopy);

		vucValueOrig = vucKeys[i].getValue();
		vucValueCopy =
			dynamic_cast < PmtMetadataT < vector <
			unsigned char > > *>(mdHandleCopy.ptr())->value();

		if (vucKeys[i].isSupported())
		{
			if (vucValueOrig != vucValueCopy)
			{
				cout << "Error with " << vucKeys[i].getKey() << ":" << endl;
				//cout << "Value written: " << vucValueOrig << endl;
				//cout << "Value read: " << vucValueCopy << endl;
				//  throw PmtError(PMT_ERROR,
				//      "Value read from file is different than the value written.", WHERE);
			}
		}
		else
		{
			if (vucValueCopy.size() != 1 || vucValueCopy[0] != 0)
			{
				cout << "Error with " << vucKeys[i].getKey() << ":" << endl;
				cout << "This key is not supposed to be supported in EXIF." <<
					endl;
			}
		}

		rootMdCopy->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
	}
}                               // testExifAccPmt_vuint8

//===========================
// Test pmt_vuint16 elements
//===========================
void testExifAccPmt_vuint16(PmtAccessor * exifAccessor, char *exifCopy,
							bool closeFileBeforeReading)
{
	//Create two md roots, one for the metadata to be written to file, the
	//other for the metadata to be read from the file afterwords.
	PmtMetadataPtr rootMdOrig = PmtMetadata::create();
	PmtMetadataPtr rootMdCopy = PmtMetadata::create();

	//Two md ptrs for handling metadata objects within the two roots.
	PmtMetadataPtr mdHandleOrig;
	PmtMetadataPtr mdHandleCopy;

	int i = 0;
	const int vusElements = 12; // # of pmt_vuint16 elements in the default schema
	KeyTestInfo < vector < unsigned short > > vusKeys[vusElements];

	vector < unsigned short >usVector(0);

	usVector.push_back(45);
	usVector.push_back(3);
	usVector.push_back(77);
	usVector.push_back(27);
	usVector.push_back(7);
	usVector.push_back(125);
	usVector.push_back(0);
	usVector.push_back(227);
	usVector.push_back(15);
	usVector.push_back(14);
	usVector.push_back(33);

	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("CaptureDevice.CFARepeatPatternDim", usVector,
		false);
	usVector.push_back(1);
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("DigitalProcess.HalftoneHints", usVector, false);
	usVector.push_back(2);
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("DigitalProcess.History.EditTagArray", usVector);
	usVector.push_back(3);
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("DigitalProcess.History.SBAInputImageBitDepth",
		usVector);

	usVector.clear();
	usVector.push_back(2);
	usVector.push_back(2);
	//YCbCrSubSampling should have a value of [2,1] or [2,2]. Other values are reserved.
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("ImageContainer.Thumbnail.YCbCrSubSampling",
		usVector);
	usVector.push_back(5);
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("ImageContainer.TransferFunction", usVector);
	usVector.push_back(6);
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("ImageContainer.TransferRange", usVector, false);

	usVector.clear();
	usVector.push_back(2);
	usVector.push_back(2);
	//YCbCrSubSampling should have a value of [2,1] or [2,2]. Other values are reserved.
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("ImageContainer.YCbCrSubSampling", usVector);

	usVector.clear();
	usVector.push_back((unsigned short) 10);
	usVector.push_back((unsigned short) 10);
	usVector.push_back((unsigned short) 10);
	//BitsPerSample must have a Count of 3. Default is [8,8,8]
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("ImageContainer.Thumbnail.BitsPerSample", usVector);

	usVector.clear();
	usVector.push_back((unsigned short) 12);
	usVector.push_back((unsigned short) 12);
	usVector.push_back((unsigned short) 12);
	//BitsPerSample must have a Count of 3. Default is [8,8,8]
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("ImageContainer.BitsPerSample", usVector);

	usVector.clear();
	usVector.push_back((unsigned short) 7);
	usVector.push_back((unsigned short) 99);
	//SubjectLocation must have a Count of 2.
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("SceneContent.SubjectLocation", usVector);

	usVector.clear();
	usVector.push_back((unsigned short) 77);
	usVector.push_back((unsigned short) 78);
	usVector.push_back((unsigned short) 79);
	//SubjectArea must have a count of 2 or 3 or 4.
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("CaptureConditions.SubjectArea", usVector);

	//Variables for holding the values of md objects
	vector < unsigned short >vusValueOrig(0);
	vector < unsigned short >vusValueCopy(0);

	if (closeFileBeforeReading)
	{
		exifAccessor->open(exifCopy, PMT_READWRITE);
	}

	for (i = 0; i < vusElements; i++)
	{
		mdHandleOrig = rootMdOrig->getMetadatum(vusKeys[i].getKey());

		dynamic_cast < PmtMetadataT < vector <
			unsigned short > > *>(mdHandleOrig.ptr())->value() =
			vusKeys[i].getValue();

		exifAccessor->writeMetadata(mdHandleOrig);

		rootMdOrig->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
		exifAccessor->open(exifCopy);
	}

	for (i = 0; i < vusElements; i++)
	{
		mdHandleCopy = rootMdCopy->getMetadatum(vusKeys[i].getKey());

		exifAccessor->readMetadata(mdHandleCopy);

		vusValueOrig = vusKeys[i].getValue();
		vusValueCopy =
			dynamic_cast < PmtMetadataT < vector <
			unsigned short > > *>(mdHandleCopy.ptr())->value();

		if (vusKeys[i].isSupported())
		{
			if (vusValueOrig != vusValueCopy)
			{
				cout << "Error with " << vusKeys[i].getKey() << ":" << endl;
				//cout << "Value written: " << vusValueOrig << endl;
				//cout << "Value read: " << vusValueCopy << endl;
				//  throw PmtError(PMT_ERROR,
				//      "Value read from file is different than the value written.", WHERE);
			}
		}
		else
		{
			if (vusValueCopy.size() != 1 || vusValueCopy[0] != 0)
			{
				cout << "Error with " << vusKeys[i].getKey() << ":" << endl;
				cout << "This key is not supposed to be supported in EXIF." <<
					endl;
			}
		}

		rootMdCopy->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
	}
}                               // testExifAccPmt_vuint16

//==========================
// Test pmt_vfloat elements
//==========================
void testExifAccPmt_vfloat(PmtAccessor * exifAccessor, char *exifCopy,
						   bool closeFileBeforeReading)
{
	//Create two md roots, one for the metadata to be written to file, the
	//other for the metadata to be read from the file afterwords.
	PmtMetadataPtr rootMdOrig = PmtMetadata::create();
	PmtMetadataPtr rootMdCopy = PmtMetadata::create();

	//Two md ptrs for handling metadata objects within the two roots.
	PmtMetadataPtr mdHandleOrig;
	PmtMetadataPtr mdHandleCopy;

	const int vfElements = 10;  // # of pmt_vfloat elements in the default schema
	KeyTestInfo < vector < float > > vfKeys[vfElements];

	vector < float >fVector(0);

	int i = 0;

	fVector.clear();
	fVector.push_back((float) 45.56);
	fVector.push_back((float) 0.5984);
	fVector.push_back((float) .55);
	fVector.push_back((float) 3.77);
	fVector.push_back((float) 10.27);
	fVector.push_back((float) 1027.1977);
	//Primary chromaticities must have a Count of 6.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("ImageContainer.PrimaryChromaticities",
		fVector);

	fVector.clear();
	fVector.push_back((float) 56.78);
	fVector.push_back((float) 0.967);
	fVector.push_back((float) 4.88);
	fVector.push_back((float) 11.34);
	fVector.push_back((float) 12.445);
	fVector.push_back((float) 2213.13);
	//ReferenceBlackWhite must have a Count of 6.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("ImageContainer.ReferenceBlackWhite",
		fVector);

	fVector.clear();
	fVector.push_back((float) 1);
	fVector.push_back((float) 2);
	fVector.push_back((float) 3);
	//YCbCrCoefficients must have a Count of 3.
	vfKeys[i++] =
		KeyTestInfo < vector <
		float > > ("ImageContainer.Thumbnail.YCbCrCoefficients", fVector);

	fVector.clear();
	fVector.push_back((float) 10);
	fVector.push_back((float) 15);
	fVector.push_back((float) 20);
	//GPSDestLatitude must have a Count of 3.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("SceneContent.GPS.DestLatitude",
		fVector);

	fVector.clear();
	fVector.push_back((float) 20);
	fVector.push_back((float) 15);
	fVector.push_back((float) 10);
	//GPSDestLongitude must have a Count of 3.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("SceneContent.GPS.DestLongitude",
		fVector);

	fVector.clear();
	fVector.push_back((float) 30);
	fVector.push_back((float) 30);
	fVector.push_back((float) 30);
	//GPSLatitude must have a Count of 3.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("SceneContent.GPS.Latitude", fVector);

	fVector.clear();
	fVector.push_back((float) 45);
	fVector.push_back((float) 45);
	fVector.push_back((float) 45);
	//GPSLongitude must have a Count of 3.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("SceneContent.GPS.Longitude", fVector);

	fVector.clear();
	fVector.push_back((float) 15);
	fVector.push_back((float) 37);
	fVector.push_back((float) 27);
	//GPSTimeStamp must have a Count of 3.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("SceneContent.GPS.TimeStamp", fVector);

	fVector.clear();
	fVector.push_back((float) 1);
	fVector.push_back((float) 2);
	fVector.push_back((float) 3);
	//YCbCrCoefficients must have a Count of 3.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("ImageContainer.YCbCrCoefficients",
		fVector);

	fVector.clear();
	fVector.push_back((float) 77);
	fVector.push_back((float) 7);
	//WhitePoint must have a Count of 2.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("ImageContainer.WhitePoint", fVector);

	//Variables for holding the values of md objects
	vector < float >vfValueOrig(0);
	vector < float >vfValueCopy(0);

	if (closeFileBeforeReading)
	{
		exifAccessor->open(exifCopy, PMT_READWRITE);
	}

	for (i = 0; i < vfElements; i++)
	{
		mdHandleOrig = rootMdOrig->getMetadatum(vfKeys[i].getKey());
		dynamic_cast < PmtMetadataT < vector <
			float > > *>(mdHandleOrig.ptr())->value() = vfKeys[i].getValue();

		exifAccessor->writeMetadata(mdHandleOrig);

		rootMdOrig->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
		exifAccessor->open(exifCopy);
	}

	for (i = 0; i < vfElements; i++)
	{
		mdHandleCopy = rootMdCopy->getMetadatum(vfKeys[i].getKey());

		exifAccessor->readMetadata(mdHandleCopy);

		vfValueOrig = vfKeys[i].getValue();
		vfValueCopy =
			dynamic_cast < PmtMetadataT < vector <
			float > > *>(mdHandleCopy.ptr())->value();

		if (vfKeys[i].isSupported())
		{
			if (vfValueOrig != vfValueCopy)
			{
				cout << "Error with " << vfKeys[i].getKey() << ":" << endl;
				//cout << "Value written: " << vfValueOrig << endl;
				//cout << "Value read: " << vfValueCopy << endl;
				//  throw PmtError(PMT_ERROR,
				//      "Value read from file is different than the value written.", WHERE);
			}
		}
		else
		{
			if (vfValueCopy.size() != 1 || vfValueCopy[0] != 0)
			{
				cout << "Error with " << vfKeys[i].getKey() << ":" << endl;
				cout << "This key is not supposed to be supported in EXIF." <<
					endl;
			}
		}

		rootMdCopy->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
	}
}                               // testExifAccPmt_vfloat

//===========================
// Test pmt_vuint32 elements
//===========================
void testExifAccPmt_vuint32(PmtAccessor * exifAccessor, char *exifCopy,
							bool closeFileBeforeReading)
{
	//Create two md roots, one for the metadata to be written to file, the
	//other for the metadata to be read from the file afterwords.
	PmtMetadataPtr rootMdOrig = PmtMetadata::create();
	PmtMetadataPtr rootMdCopy = PmtMetadata::create();

	//Two md ptrs for handling metadata objects within the two roots.
	PmtMetadataPtr mdHandleOrig;
	PmtMetadataPtr mdHandleCopy;

	const int vulElements = 5;  // # of pmt_vuint32 elements in the default schema
	KeyTestInfo < vector < unsigned long > > vulKeys[vulElements];
	int i = 0;
	vector < unsigned long >ulVector(0);

	ulVector.push_back((unsigned long) 453);

	vulKeys[i++] =
		KeyTestInfo < vector <
		unsigned long > > ("ImageContainer.SubIfdPointers", ulVector, false);

	ulVector.push_back((unsigned long) 77);
	ulVector.push_back((unsigned long) 78);
	ulVector.push_back((unsigned long) 79);
	vulKeys[i++] =
		KeyTestInfo < vector <
		unsigned long > > ("ImageContainer.StripByteCounts", ulVector);
	ulVector.push_back((unsigned long) 77);
	ulVector.push_back((unsigned long) 78);
	ulVector.push_back((unsigned long) 79);
	vulKeys[i++] =
		KeyTestInfo < vector < unsigned long > > ("ImageContainer.StripOffsets",
		ulVector);
	ulVector.push_back((unsigned long) 77);
	ulVector.push_back((unsigned long) 78);
	ulVector.push_back((unsigned long) 79);
	vulKeys[i++] =
		KeyTestInfo < vector <
		unsigned long > > ("ImageContainer.Thumbnail.StripByteCounts", ulVector);
	ulVector.push_back((unsigned long) 77);
	ulVector.push_back((unsigned long) 78);
	ulVector.push_back((unsigned long) 79);
	vulKeys[i++] =
		KeyTestInfo < vector <
		unsigned long > > ("ImageContainer.Thumbnail.StripOffsets", ulVector);

	//Variables for holding the values of md objects
	vector < unsigned long >vulValueOrig(0);
	vector < unsigned long >vulValueCopy(0);

	if (closeFileBeforeReading)
	{
		exifAccessor->open(exifCopy, PMT_READWRITE);
	}

	for (i = 0; i < vulElements; i++)
	{
		mdHandleOrig = rootMdOrig->getMetadatum(vulKeys[i].getKey());
		dynamic_cast < PmtMetadataT < vector <
			unsigned long > > *>(mdHandleOrig.ptr())->value() =
			vulKeys[i].getValue();

		exifAccessor->writeMetadata(mdHandleOrig);

		rootMdOrig->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
		exifAccessor->open(exifCopy);
	}

	for (i = 0; i < vulElements; i++)
	{
		mdHandleCopy = rootMdCopy->getMetadatum(vulKeys[i].getKey());

		exifAccessor->readMetadata(mdHandleCopy);

		vulValueOrig = vulKeys[i].getValue();
		vulValueCopy =
			dynamic_cast < PmtMetadataT < vector <
			unsigned long > > *>(mdHandleCopy.ptr())->value();

		if (vulKeys[i].isSupported())
		{
			if (vulValueOrig != vulValueCopy)
			{
				cout << "Error with " << vulKeys[i].getKey() << ":" << endl;
				//cout << "Value written: " << vulValueOrig << endl;
				//cout << "Value read: " << vulValueCopy << endl;
				//  throw PmtError(PMT_ERROR,
				//      "Value read from file is different than the value written.", WHERE);
			}
		}
		else
		{
			if (vulValueCopy.size() != 1 || vulValueCopy[0] != 0)
			{
				cout << "Error with " << vulKeys[i].getKey() << ":" << endl;
				cout << "This key is not supposed to be supported in EXIF." <<
					endl;
			}
		}

		rootMdCopy->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
	}
}                               // testExifAccPmt_vuint32

//==========================
// Test pmt_vint16 elements
//==========================
void testExifAccPmt_vint16(PmtAccessor * exifAccessor, char *exifCopy,
						   bool closeFileBeforeReading)
{
	//Create two md roots, one for the metadata to be written to file, the
	//other for the metadata to be read from the file afterwords.
	PmtMetadataPtr rootMdOrig = PmtMetadata::create();
	PmtMetadataPtr rootMdCopy = PmtMetadata::create();

	//Two md ptrs for handling metadata objects within the two roots.
	PmtMetadataPtr mdHandleOrig;
	PmtMetadataPtr mdHandleCopy;

	const int vsElements = 1;   // # of pmt_vint16 elements in the default schema
	KeyTestInfo < vector < short > > vsKeys[vsElements];

	vector < short >sVector(0);

	sVector.push_back((short) 1545);
	sVector.push_back((short) 785);
	sVector.push_back((short) 3);
	sVector.push_back((short) 244);
	sVector.push_back((short) 875);
	sVector.push_back((short) 2587);
	sVector.push_back((short) 655);
	sVector.push_back((short) 1545);

	vsKeys[0] =
		KeyTestInfo < vector <
		short > > ("SceneContent.ImageCaptureDateTime.TimeZoneOffset", sVector,
		false);

	//Variables for holding the values of md objects
	vector < short >vsValueOrig(0);
	vector < short >vsValueCopy(0);

	if (closeFileBeforeReading)
	{
		exifAccessor->open(exifCopy, PMT_READWRITE);
	}

	int i;
	for (i = 0; i < vsElements; i++)
	{
		mdHandleOrig = rootMdOrig->getMetadatum(vsKeys[i].getKey());

		dynamic_cast < PmtMetadataT < vector <
			short > > *>(mdHandleOrig.ptr())->value() = vsKeys[i].getValue();

		exifAccessor->writeMetadata(mdHandleOrig);

		rootMdOrig->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
		exifAccessor->open(exifCopy);
	}

	for (i = 0; i < vsElements; i++)
	{
		mdHandleCopy = rootMdCopy->getMetadatum(vsKeys[i].getKey());

		exifAccessor->readMetadata(mdHandleCopy);

		vsValueOrig = vsKeys[i].getValue();
		vsValueCopy =
			dynamic_cast < PmtMetadataT < vector <
			short > > *>(mdHandleCopy.ptr())->value();

		if (vsKeys[i].isSupported())
		{
			if (vsValueOrig != vsValueCopy)
			{
				cout << "Error with " << vsKeys[i].getKey() << ":" << endl;
				//cout << "Value written: " << vsValueOrig << endl;
				//cout << "Value read: " << vsValueCopy << endl;
				//  throw PmtError(PMT_ERROR,
				//      "Value read from file is different than the value written.", WHERE);
			}
		}
		else
		{
			if (vsValueCopy.size() != 1 || vsValueCopy[0] != 0)
			{
				cout << "Error with " << vsKeys[i].getKey() << ":" << endl;
				cout << "This key is not supposed to be supported in EXIF." <<
					endl;
			}
		}

		rootMdCopy->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
	}
}                               // testExifAccPmt_vint16

//==========================
// Test pmt_vint32 elements
//==========================
void testExifAccPmt_vint32(PmtAccessor * exifAccessor, char *exifCopy,
						   bool closeFileBeforeReading)
{
	//Create two md roots, one for the metadata to be written to file, the
	//other for the metadata to be read from the file afterwords.
	PmtMetadataPtr rootMdOrig = PmtMetadata::create();
	PmtMetadataPtr rootMdCopy = PmtMetadata::create();

	//Two md ptrs for handling metadata objects within the two roots.
	PmtMetadataPtr mdHandleOrig;
	PmtMetadataPtr mdHandleCopy;

	const int vlElements = 2;   // # of pmt_vint32 elements in the default schema
	KeyTestInfo < vector < long > > vlKeys[vlElements];

	vector < long >lVector(0);

	lVector.push_back((long) 1545523);
	lVector.push_back((long) 1584561);
	lVector.push_back((long) 897421);
	lVector.push_back((long) 5145214);
	lVector.push_back((long) 891456);
	lVector.push_back((long) 34675);
	lVector.push_back((long) 3);
	lVector.push_back((long) 1977);


	vlKeys[0] =
		KeyTestInfo < vector < long > > ("DigitalProcess.History.SBARGBShifts",
		lVector);
	lVector.push_back((long) 7);
	vlKeys[1] =
		KeyTestInfo < vector <
		long > > ("DigitalProcess.History.UserAdjSBARGBShifts", lVector);

	//Variables for holding the values of md objects
	vector < long >vlValueOrig(0);
	vector < long >vlValueCopy(0);

	if (closeFileBeforeReading)
	{
		exifAccessor->open(exifCopy, PMT_READWRITE);
	}

	int i;
	for (i = 0; i < vlElements; i++)
	{
		mdHandleOrig = rootMdOrig->getMetadatum(vlKeys[i].getKey());

		dynamic_cast < PmtMetadataT < vector <
			long > > *>(mdHandleOrig.ptr())->value() = vlKeys[i].getValue();

		exifAccessor->writeMetadata(mdHandleOrig);

		rootMdOrig->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
		exifAccessor->open(exifCopy);
	}

	for (i = 0; i < vlElements; i++)
	{
		mdHandleCopy = rootMdCopy->getMetadatum(vlKeys[i].getKey());

		exifAccessor->readMetadata(mdHandleCopy);

		vlValueOrig = vlKeys[i].getValue();
		vlValueCopy =
			dynamic_cast < PmtMetadataT < vector <
			long > > *>(mdHandleCopy.ptr())->value();

		if (vlKeys[i].isSupported())
		{
			if (vlValueOrig != vlValueCopy)
			{
				cout << "Error with " << vlKeys[i].getKey() << ":" << endl;
				//cout << "Value written: " << vlValueOrig << endl;
				//cout << "Value read: " << vlValueCopy << endl;
				//  throw PmtError(PMT_ERROR,
				//      "Value read from file is different than the value written.", WHERE);
			}
		}
		else
		{
			if (vlValueCopy.size() != 1 || vlValueCopy[0] != 0)
			{
				cout << "Error with " << vlKeys[i].getKey() << ":" << endl;
				cout << "This key is not supposed to be supported in EXIF." <<
					endl;
			}
		}

		rootMdCopy->deleteMetadata("*");
	}

	if (closeFileBeforeReading)
	{
		exifAccessor->close();
	}
}                               // testExifAccPmt_vint32

//====================
// Test byte elements
//====================
void testExifAccByte(PmtAccessor * exifAccessor, char *exifCopy,
					 bool closeFileBeforeReading)
{
	/* NO BYTE KEYS IN SCHEMA
	//Create two md roots, one for the metadata to be written to file, the
	//other for the metadata to be read from the file afterwords.
	PmtMetadataPtr rootMdOrig = PmtMetadata::create();
	PmtMetadataPtr rootMdCopy = PmtMetadata::create();

	//Two md ptrs for handling metadata objects within the two roots.
	PmtMetadataPtr mdHandleOrig;
	PmtMetadataPtr mdHandleCopy;

	const int cElements = 1; // # of byte elements in the default schema
	KeyTestInfo<char> cKeys[cElements];

	//Variables for holding the values of md objects
	char cValueOrig;
	char cValueCopy;

	if( closeFileBeforeReading )
	{
	exifAccessor->open(exifCopy, PMT_READWRITE);
	}

	int i;
	for( i = 0; i < cElements; i++ )
	{
	mdHandleOrig = rootMdOrig->getMetadatum(cKeys[i].getKey());
	dynamic_cast<PmtMetadataT<char>*>(mdHandleOrig.ptr())->value() =
	cKeys[i].getValue();

	exifAccessor->writeMetadata(mdHandleOrig);

	rootMdOrig->deleteMetadata("*");
	}

	if( closeFileBeforeReading )
	{
	exifAccessor->close();
	exifAccessor->open(exifCopy);
	}

	for( i = 0; i < cElements; i++ )
	{
	mdHandleCopy = rootMdCopy->getMetadatum(cKeys[i].getKey());

	exifAccessor->readMetadata(mdHandleCopy);

	cValueOrig = cKeys[i].getValue();
	cValueCopy = dynamic_cast<PmtMetadataT<char>*>(mdHandleCopy.ptr())->value();

	if( cKeys[i].isSupported() )
	{
	if( cValueOrig != cValueCopy )
	{
	cout << "Error with " << cKeys[i].getKey() << ":" << endl;
	cout << "Value written: " << cValueOrig << endl;
	cout << "Value read: " << cValueCopy << endl;
	//  throw PmtError(PMT_ERROR,
	//      "Value read from file is different than the value written.", WHERE);
	}
	}
	else
	{
	if( cValueCopy != 0 )
	{
	cout << "Error with " << cKeys[i].getKey() << ":" << endl;
	cout << "This key is not supposed to be supported in EXIF." << endl;
	}
	}
	rootMdCopy->deleteMetadata("*");
	}

	*/
}                               // testExifAccByte

void comSegmentTest()
{
	cout << "Testing the PmtExifAccessor with image containing COM segment..."
		<< endl;

	char image1[] = "DCP_0433_com.JPG";

	PmtMetadataPtr rootMd = PmtMetadata::create();

	cout << "Reading metadata from: " << image1 << "..." << endl;
	PmtAccessorPtr accIn = PmtAccessor::getAccessor(image1);
	if (!accIn)
		cout << "could not open file..." << endl;
	else
	{
		cout << "file opened successfully!..." << endl;
		rootMd = accIn->readMetadata();
		accIn->close();

		cout << rootMd << endl;
		rootMd->deleteMetadata("*");
	}
}

void jfifTest(int id)
{
	PmtMetadataPtr rootMd = PmtMetadata::create();
	PmtMetadataPtr mdHandle;
	PmtAccessorPtr accIn;

	char *jfifOriginal = "JFIF.jpg";    //A copy of this image will be used

	// Make a copy for each thread...
	char jfifCopy[128];
	sprintf(jfifCopy, "JFIF_copy.%d.jpg", id);

	//Create a copy of the image for writing to and reading from
	PmtCopyFile(jfifOriginal, jfifCopy, true);
	cout << "Opening: " << jfifCopy << "..." << endl;

	PmtExifAccessorPtr exifAccIn = new PmtExifAccessor;
	if (!exifAccIn->open(jfifCopy, PMT_READWRITE))
		cout << "could not open file: " << jfifCopy << endl;
	else
	{
		cout << "file opened successfully!..." << endl;

		mdHandle = rootMd->getMetadatum("SceneContent.ImageDescription");
		mdHandle->setValueStr(EK_L("a string"));

		mdHandle = rootMd->getMetadatum("CaptureConditions.Aperture");
		mdHandle->setValueStr(EK_L("2.8"));

		mdHandle = rootMd->getMetadatum("CaptureDevice.Model");
		mdHandle->setValueStr(EK_L("KODAK DCXXX"));

		exifAccIn->writeMetadata(rootMd);
		exifAccIn->close();
		rootMd->deleteMetadata("*");
		accIn = PmtAccessor::getAccessor(jfifCopy);
		rootMd = accIn->readMetadata();
		accIn->close();
		cout << rootMd << endl;
		rootMd->deleteMetadata("*");

		testExifAccAllKeys(false, id, jfifCopy);
	}
}

#endif                          // end of exif accessor testing


#define WIDTH   20
#define HEIGHT  20

void testTiffAcc(int id)
{
	cout << endl << "********Test Tiff accessor********" << endl;
	char file[128];
	sprintf(file, "test.%d.tif", id);

	int i;
	TiffImageFile imgFile;
	imgFile.open(file, "w");
	imgFile.createDirectory();
	TiffDirectory *dir = imgFile.getDirectory(0);

	TiffTagEntry *et;
	et = new TiffTagEntryT < PMTTIFF_UINT32 >(TIFFTAG_IMAGEWIDTH, EKTIFF_ULONG,
		1, WIDTH);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < PMTTIFF_UINT32 >(TIFFTAG_IMAGELENGTH, EKTIFF_ULONG,
		1, HEIGHT);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < unsigned short >(TIFFTAG_PHOTOMETRIC,
		EKTIFF_USHORT, 1,
		PHOTOMETRIC_MINISBLACK);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < unsigned short >(TIFFTAG_PLANARCONFIG,
		EKTIFF_USHORT, 1,
		PLANARCONFIG_CONTIG);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < unsigned short >(TIFFTAG_BITSPERSAMPLE,
		EKTIFF_USHORT, 1, 8);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < PMTTIFF_UINT32 >(TIFFTAG_ROWSPERSTRIP,
		EKTIFF_ULONG, 1, HEIGHT);
	dir->setGenericTag(*et);
	delete et;

	TiffStripImage *img = (TiffStripImage *) (dir->getImage());
	char buf[WIDTH];
	memset(buf, 0, WIDTH);
	for (i = 0; i < HEIGHT; i++)
		img->writeScanline(buf, i);
	img->flush();

	//---------
	dir->addSubDir(TIFFTAG_SUBIFD);
	dir = dir->getSubDir(TIFFTAG_SUBIFD);

	et = new TiffTagEntryT < PMTTIFF_UINT32 >(TIFFTAG_IMAGEWIDTH, EKTIFF_ULONG,
		1, WIDTH);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < PMTTIFF_UINT32 >(TIFFTAG_IMAGELENGTH, EKTIFF_ULONG,
		1, HEIGHT);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < unsigned short >(TIFFTAG_PHOTOMETRIC,
		EKTIFF_USHORT, 1,
		PHOTOMETRIC_MINISBLACK);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < unsigned short >(TIFFTAG_PLANARCONFIG,
		EKTIFF_USHORT, 1,
		PLANARCONFIG_CONTIG);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < unsigned short >(TIFFTAG_BITSPERSAMPLE,
		EKTIFF_USHORT, 1, 8);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < PMTTIFF_UINT32 >(TIFFTAG_ROWSPERSTRIP,
		EKTIFF_ULONG, 1, HEIGHT);
	dir->setGenericTag(*et);
	delete et;

	img = (TiffStripImage *) (dir->getImage());
	memset(buf, 0, WIDTH);
	for (i = 0; i < HEIGHT; i++)
		img->writeScanline(buf, i);
	img->flush();

	try
	{
		PmtMetadataPtr md;
		PmtKeyType key;
		PmtAccessorPtr tifAcc;
		tifAcc = new PmtTiffAccessor(&imgFile);

		char buf[MAX_OUTPUT_STRING_LEN];
		sprintf(buf, "\nWriting metadata to %s:\n", file);
		cout << buf;

		PmtMetadataPtr rootMd = PmtMetadata::create();
		// write in IFD_MAIN
		key = "CaptureDevice.Model";
		md = rootMd->getMetadatum(key);
		dynamic_cast < PmtMetadataT < string > *>(md.ptr())->value() =
			"Camera Model here";
		tifAcc->writeMetadata(md);
		md->print(cout);
		cout << endl;

		// write in IFD_SUB
		key = "ImageContainer.Thumbnail.Orientation";
		md = rootMd->getMetadatum(key);
		dynamic_cast < PmtMetadataT < unsigned char >*>(md.ptr())->value() = 2;
		tifAcc->writeMetadata(md);
		md->print(cout);
		cout << endl;

		// write in IFD_EXIF
		key = "SceneContent.ImageCaptureDateTime.ImageCaptureDateTime";
		md = rootMd->getMetadatum(key);
		dynamic_cast < PmtMetadataT < string > *>(md.ptr())->value() =
			"Datetime original here";
		tifAcc->writeMetadata(md);
		md->print(cout);
		cout << endl;

		key = "CaptureDevice.FilmSize";
		md = rootMd->getMetadatum(key);
		dynamic_cast < PmtMetadataT < unsigned char >*>(md.ptr())->value() = 1;
		tifAcc->writeMetadata(md);
		md->print(cout);
		cout << endl;

		key = "CaptureConditions.Fnumber";
		md = rootMd->getMetadatum(key);
		dynamic_cast < PmtMetadataT < float >*>(md.ptr())->value() =
			(float) 0.08;
		tifAcc->writeMetadata(md);
		md->print(cout);
		cout << endl;

		key = "SceneContent.SceneType";
		md = rootMd->getMetadatum(key);
		unsigned char val1 = (unsigned char) 1;
		dynamic_cast < PmtMetadataT < unsigned char >*>(md.ptr())->value() =
			val1;
		tifAcc->writeMetadata(md);
		md->print(cout);
		cout << endl;

		// write in IFD_EXIF_INTER
		/*
		key = "ImageContainer.Interoperability";
		md = rootMd->getMetadatum(key);
		dynamic_cast<PmtMetadataT<string>*>(md.ptr())->value() = "Interoperability here";
		tifAcc->writeMetadata(md);
		md->print(cout) ;
		cout << endl;
		*/

		// audio
		key = "SceneContent.Audio";
		md = rootMd->getMetadatum(key);
		vector < unsigned char >val2;
		for (i = 0; i < 88; i++)
			val2.push_back(88 - i);
		dynamic_cast < PmtMetadataT < vector <
			unsigned char > > *>(md.ptr())->value() = val2;
		tifAcc->writeMetadata(md);
		md->print(cout);
		cout << endl;

		key = "CaptureConditions.PrintQuantity";
		md = rootMd->getMetadatum(key);
		dynamic_cast < PmtMetadataT < unsigned short >*>(md.ptr())->value() =
			18;
		tifAcc->writeMetadata(md);
		md->print(cout);
		cout << endl;

#ifndef PMT_USE_NO_XML_PARSER
 		key = "NewMd";
 		md = rootMd->getMetadatum(key);
 		dynamic_cast < PmtMetadataT < unsigned short >*>(md.ptr())->value() = 10;
 		tifAcc->writeMetadata(md);
 		md->print(cout);
 		cout << endl;
#endif

		key = "DigitalProcess.History.EditTagArray";
		md = rootMd->getMetadatum(key);
		vector < unsigned short >val3;
		for (i = 0; i < 8; i++)
			val3.push_back(8 - i);
		dynamic_cast < PmtMetadataT < vector <
			unsigned short > > *>(md.ptr())->value() = val3;
		tifAcc->writeMetadata(md);
		md->print(cout);
		cout << endl;

		imgFile.close();


		// ---- read metadata
		sprintf(buf, "\nReading metadata from %s:\n", file);
		cout << buf;

		imgFile.open(file, "r");
		tifAcc = new PmtTiffAccessor(&imgFile);

		rootMd->deleteMetadata("*");
		// read from IFD_MAIN
		key = "CaptureDevice.Model";
		md = rootMd->getMetadatum(key);
		tifAcc->readMetadata(md);
		md->print(cout);
		cout << endl;

		rootMd->deleteMetadata("*");
		// read from IFD_SUB
		key = "ImageContainer.Thumbnail.Orientation";
		md = rootMd->getMetadatum(key);
		tifAcc->readMetadata(md);
		md->print(cout);
		cout << endl;

		rootMd->deleteMetadata("*");
		// read from IFD_EXIF
		key = "SceneContent.ImageCaptureDateTime.ImageCaptureDateTime";
		md = rootMd->getMetadatum(key);
		tifAcc->readMetadata(md);
		md->print(cout);
		cout << endl;

		rootMd->deleteMetadata("*");
		key = "CaptureDevice.FilmSize";
		md = rootMd->getMetadatum(key);
		tifAcc->readMetadata(md);
		md->print(cout);
		cout << endl;

		rootMd->deleteMetadata("*");
		key = "CaptureConditions.Fnumber";
		md = rootMd->getMetadatum(key);
		tifAcc->readMetadata(md);
		md->print(cout);
		cout << endl;

		rootMd->deleteMetadata("*");
		key = "SceneContent.SceneType";
		md = rootMd->getMetadatum(key);
		tifAcc->readMetadata(md);
		md->print(cout);
		cout << endl;

		//  key = "OutputOrder.Information.DealerIDNumber";
		//      md = rootMd->getMetadatum(key);
		//      tifAcc->readMetadata(md);
		//      md->print(cout) ;
		//      cout << endl;

		//        key = "DigitalProcess.History.SBARGBShifts";
		//        md = rootMd->getMetadatum(key);
		//        tifAcc->readMetadata(md);
		//        md->print(cout) ;
		//        cout << endl;

		// read from IFD_EXIF_INTER
		/*
		key = "ImageContainer.Interoperability";
		md = rootMd->getMetadatum(key);
		tifAcc->readMetadata(md);
		md->print(cout) ;
		cout << endl;
		*/
		rootMd->deleteMetadata("*");
		// audio
		key = "SceneContent.Audio";
		md = rootMd->getMetadatum(key);
		tifAcc->readMetadata(md);
		md->print(cout);
		cout << endl;

		rootMd->deleteMetadata("*");
		key = "CaptureConditions.PrintQuantity";
		md = rootMd->getMetadatum(key);
		tifAcc->readMetadata(md);
		md->print(cout);
		cout << endl;

#ifndef PMT_USE_NO_XML_PARSER
 		rootMd->deleteMetadata("*");
 		key = "NewMd";
 		md = rootMd->getMetadatum(key);
 		tifAcc->readMetadata(md);
 		md->print(cout);
 		cout << endl;
#endif

		rootMd->deleteMetadata("*");
		key = "DigitalProcess.History.EditTagArray";
		md = rootMd->getMetadatum(key);
		tifAcc->readMetadata(md);
		md->print(cout);
		cout << endl;

		imgFile.close();

	}
	catch(EkError & err)
	{
		cout << err << endl;
		exit(1);
	}
	catch(...)
	{
		cout << " **** Some Exception caught. \n" << endl;
		exit(1);
	}
}

void testTiffAccWriteFloat(PmtAccessorPtr tiffAcc)
{
	PmtMetadataPtr rootMd = PmtMetadata::create();
	PmtMetadataPtr mdHandle;

	int i = 0;
	const int fElements = 28;   // # of float elements in the default schema
	KeyTestInfo < float >fKeys[fElements];
	fKeys[i++] =
		KeyTestInfo < float >("CaptureConditions.Aperture", (float) 2.8);
	fKeys[i++] =
		KeyTestInfo < float >("CaptureConditions.BatteryLevel", (float) 45.77);
	fKeys[i++] =
		KeyTestInfo < float >("CaptureConditions.Brightness",
		(float) 13778.99);
	fKeys[i++] =
		KeyTestInfo < float >("CaptureConditions.ExposureBias",
		(float) 13.987);
	fKeys[i++] =
		KeyTestInfo < float >("CaptureConditions.ExposureIndex",
		(float) 11.13);
	fKeys[i++] =
		KeyTestInfo < float >("CaptureConditions.ExposureTime", (float) 0.05);
	fKeys[i++] =
		KeyTestInfo < float >("CaptureConditions.Flash.Energy",
		(float) 1387.33);
	fKeys[i++] =
		KeyTestInfo < float >("CaptureConditions.Fnumber", (float) 3.69);
	fKeys[i++] =
		KeyTestInfo < float >("CaptureConditions.FocalLength", (float) 300.50);
	fKeys[i++] =
		KeyTestInfo < float >("CaptureConditions.Magnification", (float) 7.75);
	fKeys[i++] =
		KeyTestInfo < float >("CaptureConditions.MaxAperture", (float) 2.05);
	fKeys[i++] =
		KeyTestInfo < float >("CaptureConditions.ShutterSpeed", (float) 0.05);
	fKeys[i++] =
		KeyTestInfo < float >("CaptureConditions.SubjectDistance",
		(float) 3.5);
	fKeys[i++] =
		KeyTestInfo < float >("CaptureDevice.FocalPlaneXResolution",
		(float) 72);
	fKeys[i++] =
		KeyTestInfo < float >("CaptureDevice.FocalPlaneYResolution",
		(float) 72);
	fKeys[i++] =
		KeyTestInfo < float >("ImageContainer.CompressedBitsPerPixel",
		(float) 11.99);
	fKeys[i++] =
		KeyTestInfo < float >("ImageContainer.DisplayXResolution", (float) 72);
	fKeys[i++] =
		KeyTestInfo < float >("ImageContainer.DisplayYResolution", (float) 72);
	fKeys[i++] =
		KeyTestInfo < float >("ImageContainer.Thumbnail.DisplayXResolution",
		(float) 72);
	fKeys[i++] =
		KeyTestInfo < float >("ImageContainer.Thumbnail.DisplayYResolution",
		(float) 72);
	fKeys[i++] =
		KeyTestInfo < float >("SceneContent.GPS.Altitude", (float) 4275.75);
	fKeys[i++] = KeyTestInfo < float >("SceneContent.GPS.DOP", (float) 30.85);
	fKeys[i++] =
		KeyTestInfo < float >("SceneContent.GPS.DestBearing", (float) 288.45);
	fKeys[i++] =
		KeyTestInfo < float >("SceneContent.GPS.DestDistance", (float) 1609);
	fKeys[i++] =
		KeyTestInfo < float >("SceneContent.GPS.ImgDirection", (float) 359);
	fKeys[i++] =
		KeyTestInfo < float >("SceneContent.GPS.Speed", (float) 212.35);
	fKeys[i++] =
		KeyTestInfo < float >("SceneContent.GPS.Track", (float) 555.5);
	fKeys[i++] =
		KeyTestInfo < float >("CaptureConditions.DigitalZoomRatio",
		(float) 2.5);

	// loop through the float elements, writing values to the file
	for (i = 0; i < fElements; i++)
	{
		mdHandle = rootMd->getMetadatum(fKeys[i].getKey());
		dynamic_cast < PmtMetadataT < float >*>(mdHandle.ptr())->value() =
			fKeys[i].getValue();

		tiffAcc->writeMetadata(mdHandle);

		rootMd->deleteMetadata("*");
	}

	return;
}                               //testTiffAccWriteFloat

void testTiffAccWriteUnsignedByte(PmtAccessorPtr tiffAcc)
{
	PmtMetadataPtr rootMd = PmtMetadata::create();
	PmtMetadataPtr mdHandle;

	const int ubElements = 35;  // # of unsignedByte elements in the default schema
	KeyTestInfo < unsigned char >ubKeys[ubElements];
	int i = 0;
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.Flash.Return",
		(unsigned char) 'r');
	//GPSAltitudeRef should have a value of 0 or 1. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("SceneContent.GPS.AltitudeRef",
		(unsigned char) 1);
	//SceneType has a default value of 1. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("SceneContent.SceneType",
		(unsigned char) 1);
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("DigitalProcess.ImageSource",
		(unsigned char) 7);
	//ExposureProgram should have a value between 0 and 8. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.ExposureProgram",
		(unsigned char) 5);
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.Flash.Flash",
		(unsigned char) 7);
	//LightSource should have a value between 0 and 24 or 255. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.LightSource",
		(unsigned char) 23);
	//MeteringMode should have a value between 0 and 6 or 255. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.MeteringMode",
		(unsigned char) 4);
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.PAR",
		(unsigned char) 50);
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureDevice.FilmCategory",
		(unsigned char) 77);
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureDevice.FilmSize",
		(unsigned char) 121);
	//ResolutionUnit should have a value of 2 (inches) or 3 (cm). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureDevice.FocalPlaneResolutionUnit",
		(unsigned char) 2);
	//SensingMethod should have a value between 1 and 8. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureDevice.SensingMethod",
		(unsigned char) 7);
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("DigitalProcess.ImageRotationStatus",
		(unsigned char) 27);
	//ResolutionUnit should have a value of 2 (inches) or 3 (cm). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("ImageContainer.DisplayResolutionUnit",
		(unsigned char) 2);
	//Orientation should have a value between 1 and 8. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("ImageContainer.Orientation",
		(unsigned char) 4);
	//PhotometricInterpretation should have a value of 2 (RGB) or 6 (YCbCr). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo <
		unsigned char >("ImageContainer.PhotometricInterpretation",
		(unsigned char) 2);
	//PlanarConfiguration should have a value of 1 (chunky) or 2 (planar). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("ImageContainer.PlanarConfiguration",
		(unsigned char) 1);
	//ResolutionUnit should have a value of 2 (inches) or 3 (cm). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo <
		unsigned char >("ImageContainer.Thumbnail.DisplayResolutionUnit",
		(unsigned char) 2);
	//Orientation should have a value between 1 and 8. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("ImageContainer.Thumbnail.Orientation",
		(unsigned char) 1);
	//PhotometricInterpretation should have a value of 2 (RGB) or 6 (YCbCr). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo <
		unsigned char >("ImageContainer.Thumbnail.PhotometricInterpretation",
		(unsigned char) 2);
	//PlanarConfiguration should have a value of 1 (chunky) or 2 (planar). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo <
		unsigned char >("ImageContainer.Thumbnail.PlanarConfiguration",
		(unsigned char) 1);
	//YCbCrPositioning sould have a value of 1 (centered) or 2 (co-sited). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo <
		unsigned char >("ImageContainer.Thumbnail.YCbCrPositioning",
		(unsigned char) 2);
	//YCbCrPositioning sould have a value of 1 (centered) or 2 (co-sited). Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("ImageContainer.YCbCrPositioning",
		(unsigned char) 2);
	//Contrast should have a value between 0 and 2. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.Contrast",
		(unsigned char) 2);
	//CustomRendered should have a value of 0 or 1. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.CustomRendered",
		(unsigned char) 1);
	//ExposureMode should have a value of 0, 1, or 2. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.ExposureMode",
		(unsigned char) 1);
	//GPSDifferential should have a value of 0 or 1. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("SceneContent.GPS.Differential",
		(unsigned char) 1);
	//GainControl should have a value between 0 and 4. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.GainControl",
		(unsigned char) 4);
	//Saturation should have a value between 0 and 2. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.Saturation",
		(unsigned char) 1);
	//SceneCaptureType should have a value between 0 and 3. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.SceneCaptureType",
		(unsigned char) 3);
	//Sharpness should have a value between 0 and 2. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.Sharpness",
		(unsigned char) 0);
	//SubjectDistanceRange should have a value between 0 and 3. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.SubjectDistanceRange",
		(unsigned char) 3);
	//WhiteBalance should have a value of 0 or 1. Other values are reserved.
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("CaptureConditions.WhiteBalance",
		(unsigned char) 0);
	ubKeys[i++] =
		KeyTestInfo < unsigned char >("DigitalProcess.ImagePrintStatus",
		(unsigned char) 2);

	for (i = 0; i < ubElements; i++)
	{
		mdHandle = rootMd->getMetadatum(ubKeys[i].getKey());
		dynamic_cast < PmtMetadataT <
			unsigned char >*>(mdHandle.ptr())->value() = ubKeys[i].getValue();

		tiffAcc->writeMetadata(mdHandle);

		rootMd->deleteMetadata("*");
	}

	return;
}                               // testTiffAccWriteUnsignedByte

void testTiffAccWriteBoolean(PmtAccessorPtr tiffAcc)
{
	PmtMetadataPtr rootMd = PmtMetadata::create();
	PmtMetadataPtr mdHandle;

	const int bElements = 1;    // # of boolean elements in the default schema
	KeyTestInfo < bool > bKeys[bElements];
	bKeys[0] = KeyTestInfo < bool > ("CaptureConditions.Flash.Fired", true);

	int i;
	for (i = 0; i < bElements; i++)
	{
		mdHandle = rootMd->getMetadatum(bKeys[i].getKey());
		dynamic_cast < PmtMetadataT < bool > *>(mdHandle.ptr())->value() =
			bKeys[i].getValue();

		tiffAcc->writeMetadata(mdHandle);

		rootMd->deleteMetadata("*");
	}
}

void testTiffAccWriteString(PmtAccessorPtr tiffAcc)
{
	PmtMetadataPtr rootMd = PmtMetadata::create();
	PmtMetadataPtr mdHandle;

	int i = 0;
	const int sElements = 39;   // # of string elements in the default schema
	KeyTestInfo < string > sKeys[sElements];
	sKeys[i++] =
		KeyTestInfo < string > ("CaptureConditions.SecurityClassification",
		"Top Secret", false);
	sKeys[i++] =
		KeyTestInfo < string > ("CaptureDevice.CameraOwner.CameraOwner",
		"The Owner");
	sKeys[i++] =
		KeyTestInfo < string > ("CaptureDevice.HostComputer",
		"My Computer", false);
	sKeys[i++] =
		KeyTestInfo < string > ("CaptureDevice.Make",
		"Eastman Kodak Company");
	sKeys[i++] = KeyTestInfo < string > ("CaptureDevice.Model", "DCS 660");

	sKeys[i++] =
		KeyTestInfo < string > ("CaptureDevice.Scanner.ModelAndVersion", "EK9000");

	sKeys[i++] =
		KeyTestInfo < string > ("CaptureDevice.Software", "GNU GIMP");
	sKeys[i++] =
		KeyTestInfo < string > ("CaptureDevice.SpectralSensitivity",
		"Tungsten Balanced");
	//DateTime should be in the format "YYYY:MM:DD HH:MM:SS"
	sKeys[i++] =
		KeyTestInfo < string > ("DigitalProcess.ImageFileChangeDateTime",
		"2002:05:21 10:07:33");
	//DateTime should be in the format "YYYY:MM:DD HH:MM:SS"
	sKeys[i++] =
		KeyTestInfo < string > ("DigitalProcess.ImageFileCreationDateTime",
		"2000:12:25 11:00:00");
	sKeys[i++] =
		KeyTestInfo < string > ("ImageContainer.Interoperability.Index",
		"R98");
	sKeys[i++] =
		KeyTestInfo < string > ("IntellectualProperty.Copyright",
		"Photog 2001");
	sKeys[i++] =
		KeyTestInfo < string > ("IntellectualProperty.ImageCreator",
		"The Owner");
	sKeys[i++] =
		KeyTestInfo < string > ("IntellectualProperty.Photographer",
		"The Owner");
	//GPSDestBearingRef should have a value of 'T' or 'M'. Other values are reserved.
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.DestBearingRef", "T");
	//GPSDestDistanceRef should have a value of 'K', 'M', or 'N'. Other values are reserved.
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.DestDistanceRef", "K");
	//GPSDestLatitudeRef should have a value of 'N' or 'S'. Other values are reserved.
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.DestLatitudeRef", "S");
	//GPSDestLongitudeRef should have a value of 'E' or 'W'. Other value are reserved.
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.DestLongitudeRef", "W");
	//GPSImgDirectionRef should have a value of 'T' or 'M'. Ther values are reserved.
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.ImgDirectionRef", "M");
	//GPSLatitudeRef should have a value of 'N' or 'S'.
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.LatitudeRef", "N");
	//GPSLongitudeRef should have a value of 'E' or 'W'
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.LongitudeRef", "E");
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.MapDatum", "TOKYO");
	//GPSMeasureMode should have a value of '2' or '3'. Other values are reserved.
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.MeasureMode", "3");
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.Satellites", "sixteen");
	//GPSSpeedRef should have a value of 'K', 'M', or 'N'. Other values are reserved.
	sKeys[i++] = KeyTestInfo < string > ("SceneContent.GPS.SpeedRef", "M");
	//GPSStatus should have a value of 'A' or 'V'.
	sKeys[i++] = KeyTestInfo < string > ("SceneContent.GPS.Status", "V");
	//GPSTrackRef should have a value of 'T' or 'M'. Other values are reserved.
	sKeys[i++] = KeyTestInfo < string > ("SceneContent.GPS.TrackRef", "T");
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GroupCaption.GroupCaption",
		"ISSL Lab");
	sKeys[i++] =
		KeyTestInfo < string >
		("SceneContent.GroupCaption.UserInputGroupTitle", "ISSL Lab Picture");
	//DateTime should be in the format "YYYY:MM:DD HH:MM:SS"
	sKeys[i++] =
		KeyTestInfo < string >
		("SceneContent.ImageCaptureDateTime.ImageCaptureDateTime",
		"2001:08:24 10:09:27");
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.ImageDescription",
		"very pretty");
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.RelatedSoundFile",
		"SND00001.WAV");
	//DateTime should be in the format "YYYY:MM:DD HH:MM:SS"
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.SoundCaptureDateTime",
		"2001:08:24 10:09:27", false);
	sKeys[i++] =
		KeyTestInfo < string >
		("SceneContent.UserComment.UserInputFrameTitle", "birthday pictures");
	sKeys[i++] =
		KeyTestInfo < string >
		("SceneContent.UserComment.UserSelectFrameTitle",
		"anniversary pictures");
	//GPSDateStamp should be in the format "YYYY:MM:DD".
	sKeys[i++] =
		KeyTestInfo < string > ("SceneContent.GPS.DateStamp",
		"2002:05:14");
	//ImageUniqueID should have a Count of 33 (including null termination character).
	sKeys[i++] =
		KeyTestInfo < string > ("ImageContainer.ImageUniqueID",
		"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
	sKeys[i++] =
		KeyTestInfo < string > ("CaptureDevice.FilmGencode", "GenCode");
	sKeys[i++] =
		KeyTestInfo < string > ("CaptureDevice.FilmProductCode", "ProdCode");

	for (i = 0; i < sElements; i++)
	{
		mdHandle = rootMd->getMetadatum(sKeys[i].getKey());

		dynamic_cast < PmtMetadataT < string >
			*>(mdHandle.ptr())->value() = sKeys[i].getValue();

		tiffAcc->writeMetadata(mdHandle);

		rootMd->deleteMetadata("*");
	}
}                               // testTiffAccWriteString

void testTiffAccWriteUnsignedShort(PmtAccessorPtr tiffAcc)
{
	PmtMetadataPtr rootMd = PmtMetadata::create();
	PmtMetadataPtr mdHandle;

	int i = 0;
	const int usElements = 18;  // # of unsignedShort elements in the default schema
	KeyTestInfo < unsigned short >usKeys[usElements];
	usKeys[i++] =
		KeyTestInfo < unsigned short >("CaptureConditions.PrintQuantity",
		(unsigned short) 33);
	usKeys[i++] =
		KeyTestInfo < unsigned short >("CaptureConditions.SelfTimerMode",
		(unsigned short) 3);
	usKeys[i++] =
		KeyTestInfo < unsigned short >("CaptureDevice.ISOSpeedRating",
		(unsigned short) 640);
	usKeys[i++] =
		KeyTestInfo < unsigned short >("CaptureDevice.Interlace",
		(unsigned short) 45, false);
	usKeys[i++] =
		KeyTestInfo <
		unsigned short >("DigitalProcess.History.SBAInputImageColorspace",
		(unsigned short) 1);
	usKeys[i++] =
		KeyTestInfo < unsigned short >("DigitalProcess.CellLength",
		(unsigned short) 480);
	usKeys[i++] =
		KeyTestInfo < unsigned short >("DigitalProcess.CellWidth",
		(unsigned short) 600);
	usKeys[i++] =
		KeyTestInfo <
		unsigned short >("DigitalProcess.ScanFrameSequenceNumber",
		(unsigned short) 4);
	usKeys[i++] =
		KeyTestInfo < unsigned short >("DigitalProcess.Thresholding",
		(unsigned short) 24, false);
	//SamplesPerPixel has a default value of 3.
	usKeys[i++] =
		KeyTestInfo < unsigned short >("ImageContainer.SamplesPerPixel",
		(unsigned short) 3);
	//SamplesPerPixel has a default value of 3.
	usKeys[i++] =
		KeyTestInfo <
		unsigned short >("ImageContainer.Thumbnail.SamplesPerPixel",
		(unsigned short) 3);
	usKeys[i++] =
		KeyTestInfo <
		unsigned short >("OutputOrder.SimpleRenderInst.FrameNumber",
		(unsigned short) 6);
	usKeys[i++] =
		KeyTestInfo <
		unsigned short >("CaptureConditions.FocalLengthIn35mmFilm",
		(unsigned short) 24);
	//Compression should have a value of 1 (uncompressed) or 6 (JPEG compression). Other values are reserved.
	usKeys[i++] =
		KeyTestInfo < unsigned short >("ImageContainer.Compression",
		(unsigned short) 6);
	//Compression should have a value of 1 (uncompressed) or 6 (JPEG compression). Other values are reserved.
	usKeys[i++] =
		KeyTestInfo < unsigned short >("ImageContainer.Thumbnail.Compression",
		(unsigned short) 6);
	usKeys[i++] =
		KeyTestInfo < unsigned short >("DigitalProcess.ImageSourceEK",
		(unsigned short) 5);
	usKeys[i++] =
		KeyTestInfo < unsigned short >("ImageContainer.SoundType",
		(unsigned short) 277);
	//ColorSpace should have a value of 1 (sRGB) or FFFF.H (Uncalibrated). Other values are reserved.
	usKeys[i++] =
		KeyTestInfo < unsigned short >("ImageContainer.ColorSpaceInformation",
		(unsigned short) 0xFFFF);

	for (i = 0; i < usElements; i++)
	{
		mdHandle = rootMd->getMetadatum(usKeys[i].getKey());
		dynamic_cast < PmtMetadataT <
			unsigned short >*>(mdHandle.ptr())->value() = usKeys[i].getValue();

		tiffAcc->writeMetadata(mdHandle);

		rootMd->deleteMetadata("*");
	}
	return;
}                               // testTiffAccWriteUnsignedShort

void testTiffAccWriteUnsignedInt(PmtAccessorPtr tiffAcc)
{
	PmtMetadataPtr rootMd = PmtMetadata::create();
	PmtMetadataPtr mdHandle;

	int i = 0;
	const int uiElements = 11;  // # of unsignedInt elements in the default schema
	KeyTestInfo < unsigned long >uiKeys[uiElements];
	uiKeys[i++] =
		KeyTestInfo < unsigned long >("CaptureConditions.ImageNumber",
		(unsigned long) 70000);
	uiKeys[i++] =
		KeyTestInfo < unsigned long >("ImageContainer.ActualImageHeight",
		(unsigned long) 600);
	uiKeys[i++] =
		KeyTestInfo < unsigned long >("ImageContainer.ActualImageWidth",
		(unsigned long) 480);
	uiKeys[i++] =
		KeyTestInfo < unsigned long >("ImageContainer.Height",
		(unsigned long) 72);
	uiKeys[i++] =
		KeyTestInfo < unsigned long >("ImageContainer.RowsPerStrip",
		(unsigned long) 5025);
	uiKeys[i++] =
		KeyTestInfo < unsigned long >("ImageContainer.Thumbnail.RowsPerStrip",
		(unsigned long) 11);
	uiKeys[i++] =
		KeyTestInfo < unsigned long >("ImageContainer.Thumbnail.Height",
		(unsigned long) 8999);
	uiKeys[i++] =
		KeyTestInfo < unsigned long >("ImageContainer.Thumbnail.Width",
		(unsigned long) 65000);
	uiKeys[i++] =
		KeyTestInfo < unsigned long >("ImageContainer.Width",
		(unsigned long) 15676);
	uiKeys[i++] =
		KeyTestInfo <
		unsigned long >("ImageContainer.Thumbnail.OffsetToJPEGSOI",
		(unsigned long) 90545);
	uiKeys[i++] =
		KeyTestInfo <
		unsigned long >("ImageContainer.Thumbnail.BytesOfJPEGData",
		(unsigned long) 250000);

	for (i = 0; i < uiElements; i++)
	{
		mdHandle = rootMd->getMetadatum(uiKeys[i].getKey());

		dynamic_cast < PmtMetadataT <
			unsigned long >*>(mdHandle.ptr())->value() = uiKeys[i].getValue();

		tiffAcc->writeMetadata(mdHandle);

		rootMd->deleteMetadata("*");
	}
	return;
}                               // testTiffAccWriteUnsignedInt

void testTiffAccWritePmt_vuint8(PmtAccessorPtr tiffAcc)
{
	PmtMetadataPtr rootMd = PmtMetadata::create();
	PmtMetadataPtr mdHandle;

	const int vucElements = 30; // # of pmt_vuint8 elements in the default schema
	KeyTestInfo < vector < unsigned char > > vucKeys[vucElements];

	vector < unsigned char >ucVector(0);
	ucVector.push_back((unsigned char) 2);
	ucVector.push_back((unsigned char) 2);

	int i = 0;

	ucVector.clear();
	ucVector.push_back((unsigned char) 2);
	ucVector.push_back((unsigned char) 2);
	ucVector.push_back((unsigned char) 0);
	ucVector.push_back((unsigned char) 0);
	//GPSVersionID must have a count of 4 and should have a value of 2.2.0.0. Other values are reserved.
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("SceneContent.GPS.VersionID",
		ucVector);

	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("CaptureConditions.DeviceSettingDescription",
		ucVector);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("CaptureDevice.CFAPattern",
		ucVector);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("CaptureDevice.CameraOwner.EK",
		ucVector);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("CaptureDevice.MakerNotes",
		ucVector);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("CaptureDevice.Noise",
		ucVector, false);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("CaptureDevice.OECF",
		ucVector);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("CaptureDevice.SerialNumber.Camera", ucVector);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("CaptureDevice.SpatialFrequencyResponse", ucVector);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("DigitalProcess.History.SBAExposureRecord", ucVector);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("DigitalProcess.ICCProfile",
		ucVector, false);
	ucVector.push_back((unsigned char) 2);
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("DigitalProcess.RollGuid",
		ucVector);

	ucVector.clear();
	ucVector.push_back(4);
	ucVector.push_back(5);
	ucVector.push_back(6);
	ucVector.push_back(0);
	//ComponentsConfiguration must have a Count of 4. All values must be between 0 and 6. Others are reserved.
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("ImageContainer.ComponentsConfiguration", ucVector);

	ucVector.clear();
	ucVector.push_back(0);
	ucVector.push_back(2);
	ucVector.push_back(2);
	ucVector.push_back(0);
	//ExifVersion must have a count of 4. Default is "0220"
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("ImageContainer.ExifVersion",
		ucVector);

	ucVector.clear();
	ucVector.push_back(0);
	ucVector.push_back(1);
	ucVector.push_back(0);
	ucVector.push_back(0);
	//FlashpixVersion must have a count of 4. Default is "0100". Other values are reserved.
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("ImageContainer.FlashpixVersion", ucVector);
	ucVector.push_back('r');

	//!!!
	//vucKeys[i++] = KeyTestInfo< vector<unsigned char> >( "ImageContainer.IPTC_NAA", ucVector );
	//ucVector.push_back('r');


	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("ImageContainer.MetadataNumber", ucVector);

	ucVector.clear();
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("ImageContainer.SourceImageDirectory", ucVector);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("ImageContainer.SourceImageFileName", ucVector);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("ImageContainer.SourceImageVolumeName", ucVector);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("ImageContainer.TiffepVersion",
		ucVector, false);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("OutputOrder.Information.DealerIDNumber", ucVector);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("OutputOrder.Information.EnvelopeNumber", ucVector);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector < unsigned char > > ("SceneContent.Audio",
		ucVector);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("SceneContent.UserComment.UserComment", ucVector);
	ucVector.clear();
	ucVector.push_back((unsigned char) 48);
	ucVector.push_back((unsigned char) 49);
	ucVector.push_back((unsigned char) 48);
	ucVector.push_back((unsigned char) 48);
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("ImageContainer.Interoperability.Version", ucVector);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("SceneContent.GPS.ProcessingMethod", ucVector);
	ucVector.push_back('r');
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("SceneContent.GPS.AreaInformation", ucVector);
	ucVector.push_back((char)3);
	vucKeys[i++] = 
		KeyTestInfo < vector <
		unsigned char > > ("CaptureDevice.FID", ucVector);
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("OutputOrder.Information.VendorOrderNumber",
		ucVector);
	ucVector.push_back((char)6);
	vucKeys[i++] =
		KeyTestInfo < vector <
		unsigned char > > ("SceneContent.GroupCaption.UserSelectGroupTitle",
		ucVector);

	for (i = 0; i < vucElements; i++)
	{
		mdHandle = rootMd->getMetadatum(vucKeys[i].getKey());

		dynamic_cast < PmtMetadataT < vector <
			unsigned char > > *>(mdHandle.ptr())->value() =
			vucKeys[i].getValue();

		tiffAcc->writeMetadata(mdHandle);

		rootMd->deleteMetadata("*");
	}
	return;
}                               // testTiffAccWritePmt_vuint8

void testTiffAccWritePmt_vuint16(PmtAccessorPtr tiffAcc)
{
	PmtMetadataPtr rootMd = PmtMetadata::create();
	PmtMetadataPtr mdHandle;

	int i = 0;
	const int vusElements = 12; // # of pmt_vuint16 elements in the default schema
	KeyTestInfo < vector < unsigned short > > vusKeys[vusElements];

	vector < unsigned short >usVector(0);

	usVector.push_back(45);
	usVector.push_back(3);
	usVector.push_back(77);
	usVector.push_back(27);
	usVector.push_back(7);
	usVector.push_back(125);
	usVector.push_back(0);
	usVector.push_back(227);
	usVector.push_back(15);
	usVector.push_back(14);
	usVector.push_back(33);

	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("CaptureDevice.CFARepeatPatternDim", usVector);
	usVector.push_back(1);
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("DigitalProcess.HalftoneHints", usVector);
	usVector.push_back(2);
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("DigitalProcess.History.EditTagArray", usVector);
	usVector.push_back(3);
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("DigitalProcess.History.SBAInputImageBitDepth",
		usVector);

	usVector.clear();
	usVector.push_back(2);
	usVector.push_back(2);
	//YCbCrSubSampling should have a value of [2,1] or [2,2]. Other values are reserved.
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("ImageContainer.Thumbnail.YCbCrSubSampling",
		usVector);
	usVector.push_back(5);
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("ImageContainer.TransferFunction", usVector);
	usVector.push_back(6);
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("ImageContainer.TransferRange", usVector);

	usVector.clear();
	usVector.push_back(2);
	usVector.push_back(2);
	//YCbCrSubSampling should have a value of [2,1] or [2,2]. Other values are reserved.
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("ImageContainer.YCbCrSubSampling", usVector);

	usVector.clear();
	usVector.push_back((unsigned short) 10);
	usVector.push_back((unsigned short) 10);
	usVector.push_back((unsigned short) 10);
	//BitsPerSample must have a Count of 3. Default is [8,8,8]
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("ImageContainer.Thumbnail.BitsPerSample", usVector);

	usVector.clear();
	usVector.push_back((unsigned short) 12);
	usVector.push_back((unsigned short) 12);
	usVector.push_back((unsigned short) 12);
	//BitsPerSample must have a Count of 3. Default is [8,8,8]
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("ImageContainer.BitsPerSample", usVector);

	usVector.clear();
	usVector.push_back((unsigned short) 7);
	usVector.push_back((unsigned short) 99);
	//SubjectLocation must have a Count of 2.
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("SceneContent.SubjectLocation", usVector);

	usVector.clear();
	usVector.push_back((unsigned short) 77);
	usVector.push_back((unsigned short) 78);
	usVector.push_back((unsigned short) 79);
	//SubjectArea must have a count of 2 or 3 or 4.
	vusKeys[i++] =
		KeyTestInfo < vector <
		unsigned short > > ("CaptureConditions.SubjectArea", usVector);

	for (i = 0; i < vusElements; i++)
	{
		mdHandle = rootMd->getMetadatum(vusKeys[i].getKey());

		dynamic_cast < PmtMetadataT < vector <
			unsigned short > > *>(mdHandle.ptr())->value() =
			vusKeys[i].getValue();

		tiffAcc->writeMetadata(mdHandle);

		rootMd->deleteMetadata("*");
	}
	return;
}                               // testTiffAccWritePmt_vuint16

void testTiffAccWritePmt_vfloat(PmtAccessorPtr tiffAcc)
{
	PmtMetadataPtr rootMd = PmtMetadata::create();
	PmtMetadataPtr mdHandle;

	const int vfElements = 10;  // # of pmt_vfloat elements in the default schema
	KeyTestInfo < vector < float > > vfKeys[vfElements];

	vector < float >fVector(0);

	int i = 0;

	fVector.clear();
	fVector.push_back((float) 45.56);
	fVector.push_back((float) 0.5984);
	fVector.push_back((float) .55);
	fVector.push_back((float) 3.77);
	fVector.push_back((float) 10.27);
	fVector.push_back((float) 1027.1977);
	//Primary chromaticities must have a Count of 6.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("ImageContainer.PrimaryChromaticities",
		fVector);

	fVector.clear();
	fVector.push_back((float) 56.78);
	fVector.push_back((float) 0.967);
	fVector.push_back((float) 4.88);
	fVector.push_back((float) 11.34);
	fVector.push_back((float) 12.445);
	fVector.push_back((float) 2213.13);
	//ReferenceBlackWhite must have a Count of 6.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("ImageContainer.ReferenceBlackWhite",
		fVector);

	fVector.clear();
	fVector.push_back((float) 1);
	fVector.push_back((float) 2);
	fVector.push_back((float) 3);
	//YCbCrCoefficients must have a Count of 3.
	vfKeys[i++] =
		KeyTestInfo < vector <
		float > > ("ImageContainer.Thumbnail.YCbCrCoefficients", fVector);

	fVector.clear();
	fVector.push_back((float) 10);
	fVector.push_back((float) 15);
	fVector.push_back((float) 20);
	//GPSDestLatitude must have a Count of 3.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("SceneContent.GPS.DestLatitude",
		fVector);

	fVector.clear();
	fVector.push_back((float) 20);
	fVector.push_back((float) 15);
	fVector.push_back((float) 10);
	//GPSDestLongitude must have a Count of 3.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("SceneContent.GPS.DestLongitude",
		fVector);

	fVector.clear();
	fVector.push_back((float) 30);
	fVector.push_back((float) 30);
	fVector.push_back((float) 30);
	//GPSLatitude must have a Count of 3.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("SceneContent.GPS.Latitude", fVector);

	fVector.clear();
	fVector.push_back((float) 45);
	fVector.push_back((float) 45);
	fVector.push_back((float) 45);
	//GPSLongitude must have a Count of 3.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("SceneContent.GPS.Longitude", fVector);

	fVector.clear();
	fVector.push_back((float) 15);
	fVector.push_back((float) 37);
	fVector.push_back((float) 27);
	//GPSTimeStamp must have a Count of 3.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("SceneContent.GPS.TimeStamp", fVector);

	fVector.clear();
	fVector.push_back((float) 1);
	fVector.push_back((float) 2);
	fVector.push_back((float) 3);
	//YCbCrCoefficients must have a Count of 3.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("ImageContainer.YCbCrCoefficients",
		fVector);

	fVector.clear();
	fVector.push_back((float) 77);
	fVector.push_back((float) 7);
	//WhitePoint must have a Count of 2.
	vfKeys[i++] =
		KeyTestInfo < vector < float > > ("ImageContainer.WhitePoint", fVector);

	for (i = 0; i < vfElements; i++)
	{
		mdHandle = rootMd->getMetadatum(vfKeys[i].getKey());
		dynamic_cast < PmtMetadataT < vector <
			float > > *>(mdHandle.ptr())->value() = vfKeys[i].getValue();

		tiffAcc->writeMetadata(mdHandle);

		rootMd->deleteMetadata("*");
	}
	return;
}                               // testTiffAccWritePmt_vfloat

void testTiffAccWritePmt_vuint32(PmtAccessorPtr tiffAcc)
{
	PmtMetadataPtr rootMd = PmtMetadata::create();
	PmtMetadataPtr mdHandle;

	const int vulElements = 5;  // # of pmt_vuint32 elements in the default schema
	KeyTestInfo < vector < unsigned long > > vulKeys[vulElements];
	int i = 0;
	vector < unsigned long >ulVector(0);

	ulVector.push_back((unsigned long) 453);

	vulKeys[i++] =
		KeyTestInfo < vector <
		unsigned long > > ("ImageContainer.SubIfdPointers", ulVector);

	ulVector.push_back((unsigned long) 77);
	ulVector.push_back((unsigned long) 78);
	ulVector.push_back((unsigned long) 79);
	vulKeys[i++] =
		KeyTestInfo < vector <
		unsigned long > > ("ImageContainer.StripByteCounts", ulVector);
	ulVector.push_back((unsigned long) 77);
	ulVector.push_back((unsigned long) 78);
	ulVector.push_back((unsigned long) 79);
	vulKeys[i++] =
		KeyTestInfo < vector < unsigned long > > ("ImageContainer.StripOffsets",
		ulVector);
	ulVector.push_back((unsigned long) 77);
	ulVector.push_back((unsigned long) 78);
	ulVector.push_back((unsigned long) 79);
	vulKeys[i++] =
		KeyTestInfo < vector <
		unsigned long > > ("ImageContainer.Thumbnail.StripByteCounts", ulVector);
	ulVector.push_back((unsigned long) 77);
	ulVector.push_back((unsigned long) 78);
	ulVector.push_back((unsigned long) 79);
	vulKeys[i++] =
		KeyTestInfo < vector <
		unsigned long > > ("ImageContainer.Thumbnail.StripOffsets", ulVector);

	for (i = 0; i < vulElements; i++)
	{
		mdHandle = rootMd->getMetadatum(vulKeys[i].getKey());
		dynamic_cast < PmtMetadataT < vector <
			unsigned long > > *>(mdHandle.ptr())->value() =
			vulKeys[i].getValue();

		tiffAcc->writeMetadata(mdHandle);

		rootMd->deleteMetadata("*");
	}
	return;
}                               // testTiffAccWritePmt_vuint32

void testTiffAccWritePmt_vint16(PmtAccessorPtr tiffAcc)
{
	PmtMetadataPtr rootMd = PmtMetadata::create();
	PmtMetadataPtr mdHandle;

	const int vsElements = 1;   // # of pmt_vint16 elements in the default schema
	KeyTestInfo < vector < short > > vsKeys[vsElements];

	vector < short >sVector(0);

	sVector.push_back((short) 1545);
	sVector.push_back((short) 785);
	sVector.push_back((short) 3);
	sVector.push_back((short) 244);
	sVector.push_back((short) 875);
	sVector.push_back((short) 2587);
	sVector.push_back((short) 655);
	sVector.push_back((short) 1545);

	vsKeys[0] =
		KeyTestInfo < vector <
		short > > ("SceneContent.ImageCaptureDateTime.TimeZoneOffset", sVector);

	int i;
	for (i = 0; i < vsElements; i++)
	{
		mdHandle = rootMd->getMetadatum(vsKeys[i].getKey());

		dynamic_cast < PmtMetadataT < vector <
			short > > *>(mdHandle.ptr())->value() = vsKeys[i].getValue();

		tiffAcc->writeMetadata(mdHandle);

		rootMd->deleteMetadata("*");
	}
	return;
}                               // testTiffAccWritePmt_vint16

void testTiffAccWritePmt_vint32(PmtAccessorPtr tiffAcc)
{
	PmtMetadataPtr rootMd = PmtMetadata::create();
	PmtMetadataPtr mdHandle;

	const int vlElements = 2;   // # of pmt_vint32 elements in the default schema
	KeyTestInfo < vector < long > > vlKeys[vlElements];

	vector < long >lVector(0);

	lVector.push_back((long) 1545523);
	lVector.push_back((long) 1584561);
	lVector.push_back((long) 897421);
	lVector.push_back((long) 5145214);
	lVector.push_back((long) 891456);
	lVector.push_back((long) 34675);
	lVector.push_back((long) 3);
	lVector.push_back((long) 1977);


	vlKeys[0] =
		KeyTestInfo < vector < long > > ("DigitalProcess.History.SBARGBShifts",
		lVector);
	lVector.push_back((long) 7);
	vlKeys[1] =
		KeyTestInfo < vector <
		long > > ("DigitalProcess.History.UserAdjSBARGBShifts", lVector);

	int i;
	for (i = 0; i < vlElements; i++)
	{
		mdHandle = rootMd->getMetadatum(vlKeys[i].getKey());

		dynamic_cast < PmtMetadataT < vector <
			long > >*>(mdHandle.ptr())->value() = vlKeys[i].getValue();

		tiffAcc->writeMetadata(mdHandle);

		rootMd->deleteMetadata("*");
	}
	return;
}                               // testTiffAccWritePmt_vint32

void testTiffAccWriteByte(PmtAccessorPtr tiffAcc)
{
	/* NO BYTE KEYS IN SCHEMA
	PmtMetadataPtr rootMd = PmtMetadata::create();
	PmtMetadataPtr mdHandle;

	const int cElements = 1; // # of byte elements in the default schema
	KeyTestInfo<char> cKeys[cElements];

	int i;
	for( i = 0; i < cElements; i++ )
	{
	mdHandle = rootMd->getMetadatum(cKeys[i].getKey());
	dynamic_cast<PmtMetadataT<char>*>(mdHandle.ptr())->value() =
	cKeys[i].getValue();

	tiffAcc->writeMetadata(mdHandle);

	rootMd->deleteMetadata("*");
	}
	return;
	*/
}                               // testTiffAccWriteByte

void testTiffAccAllKeys(int id)
{
	cout << endl << "********Test all keys with Tiff accessor********" << endl;
	char file[128];
	sprintf(file, "test2.%d.tif", id);

	int i;
	TiffImageFile imgFile;
	imgFile.open(file, "w");
	imgFile.createDirectory();
	TiffDirectory *dir = imgFile.getDirectory(0);

	TiffTagEntry *et;
	et = new TiffTagEntryT < PMTTIFF_UINT32 >(TIFFTAG_IMAGEWIDTH, EKTIFF_ULONG,
		1, WIDTH);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < PMTTIFF_UINT32 >(TIFFTAG_IMAGELENGTH, EKTIFF_ULONG,
		1, HEIGHT);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < unsigned short >(TIFFTAG_PHOTOMETRIC,
		EKTIFF_USHORT, 1,
		PHOTOMETRIC_MINISBLACK);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < unsigned short >(TIFFTAG_PLANARCONFIG,
		EKTIFF_USHORT, 1,
		PLANARCONFIG_CONTIG);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < unsigned short >(TIFFTAG_BITSPERSAMPLE,
		EKTIFF_USHORT, 1, 8);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < PMTTIFF_UINT32 >(TIFFTAG_ROWSPERSTRIP,
		EKTIFF_ULONG, 1, HEIGHT);
	dir->setGenericTag(*et);
	delete et;

	TiffStripImage *img = (TiffStripImage *) (dir->getImage());
	char buf[WIDTH];
	memset(buf, 0, WIDTH);
	for (i = 0; i < HEIGHT; i++)
		img->writeScanline(buf, i);
	img->flush();

	//---------
	dir->addSubDir(TIFFTAG_SUBIFD);
	dir = dir->getSubDir(TIFFTAG_SUBIFD);

	et = new TiffTagEntryT < PMTTIFF_UINT32 >(TIFFTAG_IMAGEWIDTH, EKTIFF_ULONG,
		1, WIDTH);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < PMTTIFF_UINT32 >(TIFFTAG_IMAGELENGTH, EKTIFF_ULONG,
		1, HEIGHT);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < unsigned short >(TIFFTAG_PHOTOMETRIC,
		EKTIFF_USHORT, 1,
		PHOTOMETRIC_MINISBLACK);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < unsigned short >(TIFFTAG_PLANARCONFIG,
		EKTIFF_USHORT, 1,
		PLANARCONFIG_CONTIG);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < unsigned short >(TIFFTAG_BITSPERSAMPLE,
		EKTIFF_USHORT, 1, 8);
	dir->setGenericTag(*et);
	delete et;

	et = new TiffTagEntryT < PMTTIFF_UINT32 >(TIFFTAG_ROWSPERSTRIP,
		EKTIFF_ULONG, 1, HEIGHT);
	dir->setGenericTag(*et);
	delete et;

	img = (TiffStripImage *) (dir->getImage());
	memset(buf, 0, WIDTH);
	for (i = 0; i < HEIGHT; i++)
		img->writeScanline(buf, i);
	img->flush();

	try
	{
		PmtMetadataPtr md;
		PmtKeyType key;
		PmtAccessorPtr tifAcc;
		tifAcc = new PmtTiffAccessor(&imgFile);

		cout << "Writing metadata to " << file << endl;

		testTiffAccWriteFloat(tifAcc);
		testTiffAccWriteUnsignedByte(tifAcc);
		testTiffAccWriteBoolean(tifAcc);
		testTiffAccWriteString(tifAcc);
		testTiffAccWriteUnsignedShort(tifAcc);
		testTiffAccWriteUnsignedInt(tifAcc);
		testTiffAccWritePmt_vuint8(tifAcc);
		testTiffAccWritePmt_vuint16(tifAcc);
		testTiffAccWritePmt_vfloat(tifAcc);
		testTiffAccWritePmt_vuint32(tifAcc);
		testTiffAccWritePmt_vint16(tifAcc);
		testTiffAccWritePmt_vint32(tifAcc);
		testTiffAccWriteByte(tifAcc);

		imgFile.close();


		// ---- read metadata
		cout << "Reading metadata from " << file << endl;

		imgFile.open(file, "r");
		tifAcc = new PmtTiffAccessor(&imgFile);

		PmtMetadataPtr rootMD = PmtMetadata::create();
		rootMD = tifAcc->readMetadata();

		//-------------------------------------------
		// Need to remove this call as soon as EkTiff gets updated for automatically
		// registering GPS keys. Then update GroundTrue.txt as well.
		rootMD->deleteMetadata("SceneContent.GPS.*");
		//-------------------------------------------

		cout << rootMD << endl;


		imgFile.close();

	}
	catch(EkError & err)
	{
		cout << err << endl;
		exit(1);
	}
	catch(...)
	{
		cout << " **** Some Exception caught. \n" << endl;
		exit(1);
	}

	return;
}

#if ((defined TEST_APS) && (!defined __GNUC__))
void testApsAcc()
{
	char ldrFile[] = "Leader.bin";
	char opldrFile[] = "OpLeader.bin";
	char frmFile[] = "Frame.bin";
	char opfrmFile[] = "OpFrame.bin";
	char buf[MAX_OUTPUT_STRING_LEN];

	AID_LeaderDataIn leader, *pLeader;
	AID_OpticalLeaderDataIn opLeader, *pOpLeader;
	CAID_ProcessedLeader *pProcessedLeader = 0;

	AID_FrameDataIn frame, *pFrame;
	AID_OpticalFrameDataIn opFrame, *pOpFrame;
	CAID_ProcessedFrame *pProcessedFrame = 0;
	FILE *f;

	pLeader = &leader;
	pOpLeader = &opLeader;
	pFrame = &frame;
	pOpFrame = &opFrame;

	// read leader
	f = fopen(ldrFile, "rb");
	if (f)
	{
		fread((void *) &leader, sizeof(AID_LeaderDataIn), 1, f);
		fclose(f);
	}
	else
	{
		sprintf(buf, "testApsAcc: can't open APS file %s\n", ldrFile);
		cout << buf;
		exit(1);
	}

	// read opleader
	f = fopen(opldrFile, "rb");
	if (f)
	{
		fread((void *) &opLeader, sizeof(AID_OpticalLeaderDataIn), 1, f);
		fclose(f);
	}
	else
	{
		sprintf(buf, "testApsAcc: can't open APS file %s\n", opldrFile);
		cout << buf;
		exit(1);
	}


	// read frame
	f = fopen(frmFile, "rb");
	if (f)
	{
		fread((void *) &frame, sizeof(AID_FrameDataIn), 1, f);
		fclose(f);
	}
	else
	{
		sprintf(buf, "testApsAcc: can't open APS file %s\n", frmFile);
		cout << buf;
		exit(1);
	}

	// read OpFrame
	f = fopen(opfrmFile, "rb");
	if (f)
	{
		fread((void *) &opLeader, sizeof(AID_OpticalFrameDataIn), 1, f);
		fclose(f);
	}
	else
	{
		sprintf(buf, "testApsAcc: can't open APS file %s\n", opfrmFile);
		cout << buf;
		exit(1);
	}

	// create roll
	AIDRoll *pRoll = new AIDRoll(true, true);
	pRoll->setLeaderData(pLeader, pOpLeader);
	pRoll->AddFrame(1, pFrame, pOpFrame);

	// create aps accessor
	PmtAPSAccessorPtr apsAcc = new PmtAPSAccessor(pRoll);
	apsAcc->setFrameNumber(1);

	cout << endl << "*******Test APS accessor*******" << endl;
	cout << "Reading APS metadata..." << endl;

	PmtMetadataPtr rootMd = PmtMetadata::create();

	PmtMetadataPtr md = rootMd->getMetadatum("CaptureDevice.FID");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureConditions.PAR");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureConditions.Flash.Fired");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureConditions.Flash.Return");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureConditions.Brightness");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureConditions.Magnification");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureConditions.LightSource");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureConditions.ExposureBias");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureConditions.Fnumber");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureConditions.Aperture");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureConditions.ShutterSpeed");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureConditions.MeteringMode");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureConditions.FocalLength");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("CaptureConditions.MaxAperture");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("OutputOrder.SimpleRenderInst.FrameNumber");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("SceneContent.ImageCaptureDateTime");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("SceneContent.GroupCaption.UserInputGroupTitle");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

	md = rootMd->getMetadatum("SceneContent.UserComment.UserInputFrameTitle");
	apsAcc->readMetadata(md);
	md->print(cout);
	cout << endl;

}
#endif


void testLoadMemory(int id)
{
	// Test reading and writing a newly-loaded item.
	// (Other tests concerning loadMemory() are done in the PmtInterpreterTest program.)

	PmtLogicalDefinitionInterpreter interpreter;
	PmtMetadataPtr root;
	PmtMetadataPtr md;

	// Get in-memory schema into string object.
	SPmtString localSchema;
	interpreter.convertMemoryVariableToString(PmtLocalDefs, localSchema);

	// Get in-memory translation table into string object.
	SPmtString localTable;
	interpreter.convertMemoryVariableToString(PmtLocalTable, localTable);

	// Get in-memory aliases file into string object.
	SPmtString localAliases;
	interpreter.convertMemoryVariableToString(PmtLocalAliases, localAliases);

	if (gnThreads <= 1)
		interpreter.erase();
	interpreter.load();
	interpreter.loadMemory(localSchema.c_str(), localTable.c_str(),
		localAliases.c_str());

	root = PmtMetadata::create();
	EkString testValue = EK_L("12345");

#if defined( HAVE_KIESEXIF )
	char testFile[128];
	sprintf(testFile, "test.%d.jpg", id);

	// Write value to file.
	PmtAccessorPtr acc = PmtAccessor::getAccessor(testFile, PMT_READWRITE);
	if (!acc)
	{
		throw PmtError(PMT_ERROR, "Call to PmtAccessor::getAcessor() failed!",
			WHERE);
	}
	acc->throwErrors() = true;
#ifndef PMT_USE_NO_XML_PARSER
	md = root->getMetadatum("NewMd");
	if (!md)
		throw PmtError(PMT_ERROR, "Test Error!", WHERE);
	md->setValueStr(testValue);
	acc->writeMetadata(md);
#endif
	acc->close();

	// Read same value, and ensure it's the same.
	acc = PmtAccessor::getAccessor(testFile);
	if (!acc)
	{
		throw PmtError(PMT_ERROR, "Call to PmtAccessor::getAcessor() failed!",
			WHERE);
	}
	acc->throwErrors() = true;
#ifndef PMT_USE_NO_XML_PARSER
	md = root->getMetadatum("NewMd");
	if (!md)
		throw PmtError(PMT_ERROR, "Test Error!", WHERE);
	acc->readMetadata(md);
	if (md->getValueStr() != testValue)
		throw PmtError(PMT_ERROR, "Test Error!", WHERE);
#endif
	acc->close();
#endif
}

#ifdef PMT_JPEG2000

#ifdef PMT_USE_KAKADU_3_EK
// The function below was stolen from Kakadu's simple_example
static void
process_tile(kdu_tile tile, kdu_byte *buffer)
{
	int c, num_components = tile.get_num_components(); assert(num_components<=3);
	bool use_ycc = tile.get_ycc(); // Should be true here if `num_components'=3

	// Open tile-components and create processing engines and resources
	kdu_dims dims;
	kdu_sample_allocator allocator;
	kdu_tile_comp comps[3];
	kdu_line_buf lines[3];
	kdu_push_ifc engines[3];
	for (c=0; c < num_components; c++)
	{
		comps[c] = tile.access_component(c);
		assert(comps[c].get_reversible());
		kdu_resolution res = comps[c].access_resolution(); // Get top resolution
		kdu_dims comp_dims; res.get_dims(comp_dims);
		if (c == 0)
			dims = comp_dims;
		else
			assert(dims == comp_dims); // Must be true in this simple example.
		lines[c].pre_create(&allocator,dims.size.x,true,true);
		if (res.which() == 0) // No DWT levels (should not occur in this example)
			engines[c] =
			kdu_encoder(res.access_subband(LL_BAND),&allocator,true);
		else
			engines[c] = kdu_analysis(res,&allocator,true);
		// Note: if you wish to specify a region of interest for special
		// attention (higher fidelity) at encode time, an appropriately
		// derived "kdu_roi_node" object may be supplied to any of the
		// constructors above.  This object will normally be obtained from
		// "kdu_roi_image::acquire_node", where the 'kdu_roi_image' object
		// itself must be derived in a suitable manner to support the region
		// specification techniques which are most appropriate to the
		// application.  One very simple and one very sophisticated example of
		// this are provided by "roi_sources.h" and "roi_sources.cpp" found
		// in the "kdu_compress" directory -- their use is amply
		// demonstrated by the "kdu_compress" application.  You should be
		// aware that, in addition to supplying ROI mask information, it is
		// necessary to tell the system what to do with it. This is controlled
		// by the "Rshift", "Rlevels" and "Rweight" attributes offered by the
		// "roi_params" coding parameter object.  These may be set in the
		// same way as any of the coding parameter attributes (e.g., by
		// calling "codestream.access_siz()->parse_string", as demonstrated
		// below in "main()").
	}
	allocator.finalize(); // Actually creates buffering resources
	for (c=0; c < num_components; c++)
		lines[c].create(); // Grabs resources from the allocator.

	// Now walk through the lines of the buffer, pushing them into the
	// relevant tile-component processing engines.

	while (dims.size.y--)
	{
		for (c=0; c < num_components; c++)
		{
			kdu_sample16 *dest = lines[c].get_buf16();
			kdu_byte *sp = buffer+c;
			for (int n=dims.size.x; n > 0; n--, dest++, sp+=num_components)
				dest->ival = ((kdu_int16)(*sp)) - 128; // Convert to signed quantity.
		}
		buffer += dims.size.x * num_components; // Move buffer to next row.
		if ((num_components == 3) && use_ycc)
			kdu_convert_rgb_to_ycc(lines[0],lines[1],lines[2]);
		for (c=0; c < num_components; c++)
			engines[c].push(lines[c],true);
	}

	// Cleanup
	for (c=0; c < num_components; c++)
		engines[c].destroy(); // engines are interfaces; no default destructors
}
#endif

void testJ2KAcc()
{
	cout << "\n*****************************************" << endl;
	cout <<   "***   Testing the Jpeg2000 Accessor   ***" << endl;
	cout <<   "*****************************************" << endl;

	char * inFile1 = "pmt1.jp2";
	char * inFile2 = "pmt2.jp2";
	char * inFile3 = "pmt3.jp2";
	char * outFile = "test.jp2";

	PmtMetadataPtr mdToWrite;
	PmtAccessorPtr inAcc;


	cout << "getting accessor for " << inFile1 << endl;
	inAcc = PmtAccessor::getAccessor(inFile1);
	if (!inAcc)
	{
		cout << "Error getting accessor for " << inFile1 << endl;
	}
	else
	{
		PmtMetadataPtr md = inAcc->readMetadata();
		cout << "Known Metadata in " << inFile1 << ":\n" << md << endl;
		mdToWrite = inAcc->readMetadata();
	}

	cout << "getting accessor for " << inFile2 << endl;
	inAcc = PmtAccessor::getAccessor(inFile2);
	if (!inAcc)
	{
		cout << "Error getting accessor for " << inFile2 << endl;
	}
	else
	{
		PmtMetadataPtr md = inAcc->readMetadata();
		cout << "Known Metadata in " << inFile2 << ":\n" << md << endl;
	}

	inAcc = PmtAccessor::getAccessor(inFile3);
	if (!inAcc)
	{
		cout << "Error getting accessor for " << inFile3 << endl;
	}
	else
	{
		PmtMetadataPtr md = inAcc->readMetadata();
		cout << "Known Metadata in " << inFile3 << ":\n" << md << endl;
	}

	// *** Testing writing metadata to a JP2 target ***

	int num_components = 3;
	int height = 30;
	int width = 40;

	kdu_byte* buffer = new kdu_byte[height*width*3];//(kdu_byte*)malloc(height*width*3);

	for ( int i = 0; i < height*width*3; i += 3 )
	{
		buffer[i] = 0;
		buffer[i+1] = 30;
		buffer[i+2] = 60;
	}

	// Construct code-stream object
	siz_params siz;
	siz.set(Scomponents,0,0,num_components);
	siz.set(Sdims,0,0,height);  // Height of first image component
	siz.set(Sdims,0,1,width);   // Width of first image component
	siz.set(Sprecision,0,0,8);  // Image samples have original bit-depth of 8
	siz.set(Ssigned,0,0,false); // Image samples are originally unsigned
	kdu_params *siz_ref = &siz; siz_ref->finalize();


	jp2_target* target = new jp2_target();

#ifdef PMT_USE_KAKADU_3_EK
	target->open(outFile);

	// create an accessor for the JP2 target and write metadata to it
	PmtAccessorPtr outAcc = new PmtJ2KAccessor(target);
	outAcc->writeMetadata(mdToWrite);
	// close the accessor (this will update the jp2_target to contain the
	// new metadata
	outAcc->close();
#endif

#if PMT_USE_KAKADU_4
	jp2_family_tgt* family_target = new jp2_family_tgt();
	family_target->open(outFile);
	target->open(family_target);
#endif

	jp2_dimensions dims = target->access_dimensions(); dims.init(&siz);
	jp2_colour colr = target->access_colour();
	colr.init((num_components==3)?JP2_sRGB_SPACE:JP2_sLUM_SPACE);

	kdu_codestream codestream; codestream.create(&siz,target);
	// Set up any specific coding parameters and finalize them.
	codestream.access_siz()->parse_string("Clayers=12");
	codestream.access_siz()->parse_string("Creversible=yes");
	codestream.access_siz()->finalize_all(); // Set up coding defaults

#ifdef PMT_USE_KAKADU_3_EK
	// Open the one and only tile in use here and push in all the samples.
	kdu_tile tile = codestream.open_tile(kdu_coords(0,0));
	process_tile(tile,buffer);
	tile.close();

	// Produce the final compressed output.
	kdu_long layer_bytes[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	codestream.flush(layer_bytes,12);
	codestream.destroy(); // All done: simple as that.

	// verify that the JP2 target now has the metadata
	jp2_xml_data xmlBoxes = target->access_xml();
	int numOfBoxes = xmlBoxes.get_num_boxes();
	if (numOfBoxes > 0)
	{
		int bufferSize;
		cout << "Created J2K metadata successfully:" << endl;
		cout << (char*)xmlBoxes.get_data(0, &bufferSize) << endl;
	}
	else
		cout << "Error creating J2K metadata!" << endl;
#endif

#ifdef PMT_USE_KAKADU_4
	target->write_header();

	// create an accessor for the JP2 target and write metadata to it
	PmtAccessorPtr outAcc = new PmtJ2KAccessor(family_target);
	outAcc->writeMetadata(mdToWrite);
	// close the accessor (this will update the jp2_target to contain the
	// new metadata
	outAcc->close();

	target->open_codestream(true);

	// Now compress the image in one hit, using `kdu_stripe_compressor'
	kdu_stripe_compressor compressor;
	compressor.start(codestream);
	int stripe_heights[3]={height,height,height};
	compressor.push_stripe(buffer,stripe_heights);
	compressor.finish();

	codestream.destroy();
#endif

	// close the JP2 target
	target->close();

#ifdef PMT_USE_KAKADU_4
	family_target->close();
	delete family_target;
#endif

	delete [] buffer;
	delete target;

	// Reopen the created file and reverify its metadata
	inAcc = PmtAccessor::getAccessor(outFile);
	if (!inAcc)
	{
		cout << "Error getting accessor for " << outFile << endl;
	}
	else
	{
		PmtMetadataPtr md = inAcc->readMetadata();
		cout << "Known Metadata in " << outFile << ":\n" << md << endl;
	}

}
#endif // PMT_JPEG2000


#ifdef PMT_XMP_ACCESSOR

void TestPMTMetadata(const char* const compareFile, const char* const refernceFile)
{
	PmtAccessorPtr AccCom = PmtAccessor::getAccessor(compareFile,PMT_READONLY);
	PmtMetadataPtr rootCom = AccCom->readMetadata();

	PmtAccessorPtr AccRef = PmtAccessor::getAccessor(refernceFile,PMT_READONLY);
	PmtMetadataPtr rootRef = AccRef->readMetadata();

	SPmtString strFullKey;
	PmtMetadataPtr mdCom, mdRef;

	// All metadataums in compared should be found and equaled in reference file
	PmtMetadataIterator mdIterCom = rootCom->getMetadata("*[*]", false);
	mdCom = mdIterCom.start();
	while(mdCom)
	{
		strFullKey = mdCom->fullKey();
		mdRef = rootRef->getMetadatum(strFullKey,false);
		if(mdRef && (mdRef->getValueStr() == mdCom->getValueStr()))
			mdCom = mdIterCom.next();
		else
		{
			string strErr = strFullKey; 
			strErr = "Obtained value didn't match expected value : " + strErr; 
			throw PmtError(PMT_ERROR,strErr , WHERE);
		}
	}
	// All metadataums in reference should be found and equaled in compared file
	PmtMetadataIterator mdIterRef = rootRef->getMetadata("*[*]", false);
	mdRef = mdIterCom.start();
	while(mdRef)
	{
		strFullKey = mdRef->fullKey();
		mdCom = rootRef->getMetadatum(strFullKey,false);
		if(mdRef && (mdCom->getValueStr() == mdRef->getValueStr()))
			mdRef = mdIterCom.next();
		else
		{
			string strErr = strFullKey; 
			strErr = "Obtained value didn't match expected value : " + strErr; 
			throw PmtError(PMT_ERROR,strErr , WHERE);
		}
	}
}
#define XMPMETADATATEST(mdKey,exValue) \
	expectedValue = exValue; \
	realValue = rootMd->getMetadatum(mdKey)->getValueStr(); \
	if (realValue != expectedValue) \
	{ \
		string strErr = mdKey; \
		strErr = "Obtained value didn't match expected value : " + strErr; \
		throw PmtError(PMT_ERROR,strErr , WHERE); \
	}


#define XMPMETADATATESTFLOATE(mdKey, exValue) \
	md = rootMd->getMetadatum(mdKey); \
	mdF = (PmtMetadataT<float>*) md.ptr(); \
	if(abs(exValue - mdF->value()) > 0.1) \
	{ \
		string strErr = mdKey; \
		strErr = "Obtained value didn't match expected value : " + strErr; \
		throw PmtError(PMT_ERROR,strErr , WHERE); \
	}	

#define XMPMETADATATESTSETVALUE(mdKey, newValue) \
	rootMd->getMetadatum(mdKey)->setValueStr(newValue);

void TestXMPAccessor()
{
	cout << "Begin XMP acessor testing..." << endl;

	// Test Pure XMP file of UTF8, UTF16LE,UTF16BE
	char* PureXMP[] = {
		"PureXMPFileUTF8.xmp",
#ifdef PMT_USE_XERCES //MSXML only support UTF8
#if (XERCES_VERSION_MAJOR > 2)
		"PureXMPFileUTF16_LE.xmp",
		"PureXMPFileUTF16_BE.xmp",
#endif
#endif
	};

	EkString expectedValue;
	EkString realValue;

	int nIndex = 0;
	for(nIndex = 0; nIndex < sizeof(PureXMP) / sizeof(char*); nIndex ++)
	{
		PmtAccessorPtr AccUTF = PmtAccessor::getAccessor(PureXMP[nIndex],PMT_READONLY);
		PmtMetadataPtr rootMd = AccUTF->readMetadata();
		try
		{
			XMPMETADATATEST("ImageContainer.Orientation", EK_L("1"));
			XMPMETADATATEST("ImageContainer.DisplayXResolution", EK_L("72"));
			XMPMETADATATEST("ImageContainer.DisplayYResolution", EK_L("72"));
			XMPMETADATATEST("ImageContainer.DisplayResolutionUnit", EK_L("2"));
			XMPMETADATATEST("ImageContainer.Width", EK_L("360"));
			XMPMETADATATEST("ImageContainer.Height", EK_L("216"));
			XMPMETADATATEST("ImageContainer.ColorSpaceInformation", EK_L("1"));
			XMPMETADATATEST("ImageContainer.BitsPerSample", EK_L("8 8 8"));
			XMPMETADATATEST("SceneContent.UserComment.MS.Keywords[0]", EK_L("88 0 77 0 80 0 0 0"));
			XMPMETADATATEST("SceneContent.UserComment.MS.Keywords[1]", EK_L("66 0 108 0 117 0 101 0 32 0 83 0 113 0 117 0 97 0 114 0 101 0 0 0"));
			XMPMETADATATEST("SceneContent.UserComment.MS.Keywords[2]", EK_L("116 0 101 0 115 0 116 0 32 0 102 0 105 0 108 0 101 0 0 0"));
			XMPMETADATATEST("SceneContent.UserComment.MS.Keywords[3]", EK_L("80 0 104 0 111 0 116 0 111 0 115 0 104 0 111 0 112 0 0 0"));
			XMPMETADATATEST("SceneContent.UserComment.MS.Keywords[4]", EK_L("46 0 106 0 112 0 103 0 0 0"));
			XMPMETADATATEST("SceneContent.UserComment.MS.Subject", //vuint8
				EK_L("88 0 77 0 80 0 70 0 105 0 108 0 101 0 115 0 32 0 66 0 108 0 117 0 101 0 83 0 113 0 117 0 97 0 114 0 101 0 32 0 116 0 101 0 115 0 116 0 32 0 102 0 105 0 108 0 101 0 44 0 32 0 99 0 114 0 101 0 97 0 116 0 101 0 100 0 32 0 105 0 110 0 32 0 80 0 104 0 111 0 116 0 111 0 115 0 104 0 111 0 112 0 32 0 67 0 83 0 50 0 44 0 32 0 115 0 97 0 118 0 101 0 100 0 32 0 97 0 115 0 32 0 46 0 112 0 115 0 100 0 44 0 32 0 46 0 106 0 112 0 103 0 44 0 32 0 97 0 110 0 100 0 32 0 46 0 116 0 105 0 102 0 46 0 0 0"));
			XMPMETADATATEST("SceneContent.UserComment.MS.Title", //vuint8
				EK_L("66 0 108 0 117 0 101 0 32 0 83 0 113 0 117 0 97 0 114 0 101 0 32 0 84 0 101 0 115 0 116 0 32 0 70 0 105 0 108 0 101 0 32 0 45 0 32 0 46 0 106 0 112 0 103 0 0 0"));
		}
		catch (PmtError e)
		{
			cout << e.getMsg();
			throw e;
		}	
	}
	cout << "Finished character set testing..." << endl;

	PmtCopyFile("testxmp.xml", "copyedxmp.xml",true );
	PmtAccessorPtr accIn = PmtAccessor::getAccessor("copyedxmp.xml",PMT_READWRITE);
	PmtMetadataPtr rootMd = accIn->readMetadata();

	PmtMetadataT<float>* mdF;
	PmtMetadataPtr md;
	try
	{
		XMPMETADATATEST("CaptureConditions.CustomRendered", EK_L("0"));
		XMPMETADATATEST("CaptureConditions.ExposureBias", EK_L("1"));
		XMPMETADATATEST("CaptureConditions.ExposureMode", EK_L("0"));
		XMPMETADATATEST("CaptureConditions.ExposureProgram", EK_L("3"));
		XMPMETADATATEST("CaptureConditions.ExposureTime", EK_L("1"));
		XMPMETADATATEST("CaptureConditions.Flash.Flash", EK_L("0"));
		XMPMETADATATEST("CaptureConditions.Flash.Fired", EK_L("false"));
		XMPMETADATATEST("CaptureConditions.Flash.Return", EK_L("0"));
		XMPMETADATATEST("CaptureConditions.Fnumber", EK_L("1.8"));
		XMPMETADATATEST("CaptureConditions.MeteringMode", EK_L("1"));
		XMPMETADATATEST("CaptureConditions.SceneCaptureType", EK_L("0"));
		XMPMETADATATEST("CaptureConditions.WhiteBalance", EK_L("1"));
		XMPMETADATATEST("CaptureDevice.FocalPlaneResolutionUnit", EK_L("2"));
		XMPMETADATATEST("CaptureDevice.ISOSpeedRating", EK_L("100"));
		XMPMETADATATEST("CaptureDevice.Make", EK_L("Canon"));
		XMPMETADATATEST("CaptureDevice.Model", EK_L("Canon EOS 10D"));
		XMPMETADATATEST("CaptureDevice.SensingMethod", EK_L("2"));
		XMPMETADATATEST("DigitalProcess.ImageFileChangeDateTime", EK_L("2007:08:18 17:58:55"));
		XMPMETADATATEST("DigitalProcess.ImageFileCreationDateTime", EK_L("2007:08:18 17:58:55"));
		XMPMETADATATEST("DigitalProcess.ImageSource", EK_L("3"));
		XMPMETADATATEST("ImageContainer.ColorSpaceInformation", EK_L("1"));
		XMPMETADATATEST("ImageContainer.Compression", EK_L("6"));
		XMPMETADATATEST("ImageContainer.DisplayResolutionUnit", EK_L("2"));
		XMPMETADATATEST("ImageContainer.ExifVersion", EK_L("48 50 50 49 0"));
		XMPMETADATATEST("ImageContainer.FlashpixVersion", EK_L("48 49 48 48 0"));
		XMPMETADATATEST("ImageContainer.Orientation", EK_L("1"));
		XMPMETADATATEST("ImageContainer.YCbCrPositioning", EK_L("1"));
		XMPMETADATATEST("SceneContent.ImageCaptureDateTime.ImageCaptureDateTime", EK_L("2007:08:18 17:58:55"));
		XMPMETADATATEST("SceneContent.UserComment.MS.Author[0]", EK_L("72 0 97 0 110 0 98 0 105 0 110 0 103 0 32 0 87 0 117 0 0 0"));
		XMPMETADATATEST("SceneContent.UserComment.MS.Author[1]", EK_L("80 0 104 0 105 0 108 0 105 0 112 0 32 0 87 0 117 0 0 0"));
		XMPMETADATATEST("SceneContent.UserComment.MS.Keywords[0]", EK_L("88 0 77 0 80 0 0 0"));
		XMPMETADATATEST("SceneContent.UserComment.MS.Keywords[1]", EK_L("80 0 77 0 84 0 0 0"));
		XMPMETADATATEST("SceneContent.UserComment.MS.Keywords[2]", EK_L("116 0 101 0 115 0 116 0 32 0 102 0 105 0 108 0 101 0 0 0"));
		XMPMETADATATEST("SceneContent.UserComment.MS.Keywords[3]", EK_L("80 0 104 0 105 0 108 0 105 0 112 0 32 0 87 0 117 0 0 0"));
		XMPMETADATATEST("SceneContent.UserComment.MS.Keywords[4]", EK_L("46 0 120 0 109 0 108 0 0 0"));
		XMPMETADATATEST("SceneContent.UserComment.MS.Rating", EK_L("25"));
		XMPMETADATATEST("SceneContent.UserComment.MS.Subject", EK_L("84 0 104 0 105 0 115 0 32 0 97 0 32 0 116 0 101 0 120 0 116 0 32 0 88 0 77 0 80 0 32 0 102 0 105 0 108 0 101 0 44 0 32 0 106 0 117 0 115 0 116 0 32 0 116 0 111 0 32 0 116 0 101 0 115 0 116 0 32 0 97 0 108 0 108 0 32 0 116 0 104 0 101 0 32 0 88 0 77 0 80 0 32 0 109 0 101 0 116 0 97 0 100 0 97 0 116 0 97 0 115 0 32 0 97 0 114 0 101 0 32 0 112 0 97 0 114 0 115 0 101 0 100 0 46 0 0 0"));
		XMPMETADATATEST("SceneContent.UserComment.MS.Title", EK_L("84 0 101 0 115 0 116 0 32 0 88 0 77 0 80 0 32 0 102 0 105 0 108 0 101 0 0 0"));

		XMPMETADATATESTFLOATE("CaptureConditions.Aperture", 1.696);
		XMPMETADATATESTFLOATE("CaptureConditions.FocalLength", 50);
		XMPMETADATATESTFLOATE("CaptureConditions.MaxAperture", 1.696);
		XMPMETADATATESTFLOATE("CaptureConditions.ShutterSpeed", 11.5508);
		XMPMETADATATESTFLOATE("CaptureDevice.FocalPlaneXResolution", 3443.95);
		XMPMETADATATESTFLOATE("CaptureDevice.FocalPlaneYResolution", 3442.02);
		XMPMETADATATESTFLOATE("ImageContainer.CompressedBitsPerPixel", 3);
		XMPMETADATATESTFLOATE("ImageContainer.DisplayXResolution", 180);
		XMPMETADATATESTFLOATE("ImageContainer.DisplayYResolution", 180);
	}
	catch (PmtError e)
	{
		cout << e.getMsg();
		throw e;
	}
	cout << "Finished common XMP tags testing" << endl;
	//Test write metadata.
	PmtMetadataIterator mdIter = rootMd->getMetadata("*[*]", false);
	md = mdIter.start();
	while(md)
	{
		accIn->removeMetadata(md);
		md = mdIter.next();
	}

	//rootMd->deleteMetadata("CaptureConditions.*");
	try
	{
		XMPMETADATATESTSETVALUE("CaptureConditions.Aperture", EK_L("1.5")); //
		XMPMETADATATESTSETVALUE("CaptureConditions.CustomRendered", EK_L("2"));
		XMPMETADATATESTSETVALUE("CaptureConditions.ExposureMode", EK_L("1"));
		XMPMETADATATESTSETVALUE("CaptureConditions.ExposureProgram", EK_L("2"));
		XMPMETADATATESTSETVALUE("CaptureConditions.ExposureTime", EK_L("2"));
		XMPMETADATATESTSETVALUE("CaptureConditions.Flash.Flash", EK_L("3"));
		XMPMETADATATESTSETVALUE("CaptureConditions.Flash.Fired", EK_L("true"));
		XMPMETADATATESTSETVALUE("CaptureConditions.Flash.Return", EK_L("1"));
		XMPMETADATATESTSETVALUE("CaptureConditions.Fnumber", EK_L("1.6"));
		XMPMETADATATESTSETVALUE("CaptureConditions.FocalLength", EK_L("1.5"));  //
		XMPMETADATATESTSETVALUE("CaptureConditions.MaxAperture", EK_L("1.2"));
		XMPMETADATATESTSETVALUE("CaptureConditions.MeteringMode", EK_L("1"));
		XMPMETADATATESTSETVALUE("CaptureConditions.SceneCaptureType", EK_L("1"));
		XMPMETADATATESTSETVALUE("CaptureConditions.ShutterSpeed", EK_L("11.2"));
		XMPMETADATATESTSETVALUE("CaptureConditions.WhiteBalance", EK_L("1"));
		XMPMETADATATESTSETVALUE("CaptureDevice.FocalPlaneResolutionUnit", EK_L("2"));
		XMPMETADATATESTSETVALUE("CaptureDevice.FocalPlaneXResolution", EK_L("3000"));
		XMPMETADATATESTSETVALUE("CaptureDevice.FocalPlaneYResolution", EK_L("3000"));
		XMPMETADATATESTSETVALUE("CaptureDevice.ISOSpeedRating", EK_L("80"));
		XMPMETADATATESTSETVALUE("CaptureDevice.Make", EK_L("Kodak"));
		XMPMETADATATESTSETVALUE("CaptureDevice.Model", EK_L("Kodak express"));
		XMPMETADATATESTSETVALUE("CaptureDevice.SensingMethod", EK_L("2"));
		XMPMETADATATESTSETVALUE("DigitalProcess.ImageFileChangeDateTime", EK_L("2009:10:30 10:58:55"));
		XMPMETADATATESTSETVALUE("DigitalProcess.ImageFileCreationDateTime", EK_L("2009:10:30 10:58:55"));
		XMPMETADATATESTSETVALUE("DigitalProcess.ImageSource", EK_L("3"));
		XMPMETADATATESTSETVALUE("ImageContainer.ColorSpaceInformation", EK_L("2"));
		XMPMETADATATESTSETVALUE("ImageContainer.CompressedBitsPerPixel", EK_L("1")); //
		XMPMETADATATESTSETVALUE("ImageContainer.Compression", EK_L("6"));
		XMPMETADATATESTSETVALUE("ImageContainer.DisplayResolutionUnit", EK_L("1"));
		XMPMETADATATESTSETVALUE("ImageContainer.DisplayXResolution", EK_L("160")); //
		XMPMETADATATESTSETVALUE("ImageContainer.DisplayYResolution", EK_L("160")); //
		XMPMETADATATESTSETVALUE("ImageContainer.ExifVersion", EK_L("49 50 50 49 0"));
		XMPMETADATATESTSETVALUE("ImageContainer.FlashpixVersion", EK_L("49 49 48 48 0"));
		XMPMETADATATESTSETVALUE("ImageContainer.Orientation", EK_L("1"));
		XMPMETADATATESTSETVALUE("ImageContainer.YCbCrPositioning", EK_L("1"));
		XMPMETADATATESTSETVALUE("SceneContent.ImageCaptureDateTime.ImageCaptureDateTime", EK_L("2009:08:18 10:58:55"));
		XMPMETADATATESTSETVALUE("SceneContent.UserComment.MS.Author[0]", EK_L("75 0 95 0 110 0 98 0 105 0 110 0 103 0 32 0 87 0 117 0 0 0"));
		XMPMETADATATESTSETVALUE("SceneContent.UserComment.MS.Author[1]", EK_L("82 0 105 0 105 0 108 0 105 0 112 0 32 0 87 0 117 0 0 0"));
		XMPMETADATATESTSETVALUE("SceneContent.UserComment.MS.Author[2]", EK_L("82 0 105 0 105 0 108 0 105 0 112 0 32 0 87 0 117 0 0 0"));
		XMPMETADATATESTSETVALUE("SceneContent.UserComment.MS.Keywords[0]", EK_L("89 0 76 0 80 0 0 0"));
		XMPMETADATATESTSETVALUE("SceneContent.UserComment.MS.Keywords[1]", EK_L("81 0 77 0 84 0 0 0"));
		XMPMETADATATESTSETVALUE("SceneContent.UserComment.MS.Keywords[2]", EK_L("115 0 100 0 115 0 116 0 32 0 102 0 105 0 108 0 101 0 0 0"));
		XMPMETADATATESTSETVALUE("SceneContent.UserComment.MS.Keywords[3]", EK_L("90 0 108 0 105 0 108 0 105 0 112 0 32 0 87 0 117 0 0 0"));
		XMPMETADATATESTSETVALUE("SceneContent.UserComment.MS.Rating", EK_L("26"));
		XMPMETADATATESTSETVALUE("SceneContent.UserComment.MS.Subject", EK_L("85 0 107 0 115 0 112 0 33 0 97 0 32 0 116 0 101 0 120 0 116 0 32 0 88 0 77 0 80 0 32 0 102 0 105 0 108 0 101 0 44 0 32 0 106 0 117 0 115 0 116 0 32 0 116 0 111 0 32 0 116 0 101 0 115 0 116 0 32 0 97 0 108 0 108 0 32 0 116 0 104 0 101 0 32 0 88 0 77 0 80 0 32 0 109 0 101 0 116 0 97 0 100 0 97 0 116 0 97 0 115 0 32 0 97 0 114 0 101 0 32 0 112 0 97 0 114 0 115 0 101 0 100 0 46 0 0 0"));
		XMPMETADATATESTSETVALUE("SceneContent.UserComment.MS.Title", EK_L("88 0 104 0 115 0 116 0 32 0 88 0 77 0 80 0 32 0 102 0 105 0 108 0 101 0 0 0"));

		accIn->writeMetadata(rootMd);
	}
	catch(...)
	{
		cout << "Unknown error" << endl;
	}
	accIn = NULL;
	try
	{
		TestPMTMetadata("copyedxmp.xml","ResultOfTestXMP.xml");
	}
	catch (PmtError e)
	{
		cout << e.getMsg() << endl;
		throw e;
	}
	cout << "Finished metadata writing ..." << endl;
	cout << "Begin testing XMP block in EXIF file..." ;
	char* ExifXMPFiles[] =
	{
		"Kodak_DC4800_xmp.jpg",
		//The content is the same, while some elements in XMP block differs
		"Kodak_DC4800_xmp_wrong.jpg"
	};
	for (nIndex = 0; nIndex < sizeof(ExifXMPFiles) / sizeof(char*); nIndex ++)
	{
	        char exifCopy[128];
		PmtOpenMode mode = PMT_READONLY;
		char * xmpFilename = ExifXMPFiles[nIndex];
		if(nIndex == 0)
                {
	            // Make a copy for testing...
	            sprintf(exifCopy, "%s.%d.jpg", xmpFilename,nIndex);

	            //Create a copy of the image for writing to and reading from
	            PmtCopyFile(xmpFilename, exifCopy, true);
		    mode = PMT_READWRITE;
                    xmpFilename = exifCopy;
                }
 
		PmtAccessorPtr AccXMP = PmtAccessor::getAccessor(xmpFilename,mode);
		PmtMetadataPtr rootMd = AccXMP->readMetadata();
		try
		{
			cout << nIndex;
			XMPMETADATATEST("ImageContainer.Orientation", EK_L("1"));
			XMPMETADATATEST("ImageContainer.DisplayXResolution", EK_L("230"));
			XMPMETADATATEST("ImageContainer.DisplayYResolution", EK_L("230"));
			XMPMETADATATEST("ImageContainer.DisplayResolutionUnit", EK_L("2"));
			XMPMETADATATEST("ImageContainer.Width", EK_L("2160"));
			XMPMETADATATEST("ImageContainer.Height", EK_L("1440"));
			XMPMETADATATEST("ImageContainer.ColorSpaceInformation", EK_L("1"));
			XMPMETADATATEST("ImageContainer.ComponentsConfiguration", EK_L("1 2 3 0"));
			XMPMETADATATEST("ImageContainer.ExifVersion", EK_L("48 50 49 48"));
			XMPMETADATATEST("ImageContainer.FlashpixVersion", EK_L("48 49 48 48"));
			XMPMETADATATEST("ImageContainer.YCbCrPositioning", EK_L("1"));

			XMPMETADATATEST("CaptureConditions.Aperture", EK_L("3"));
			XMPMETADATATEST("CaptureConditions.ExposureBias", EK_L("0"));
			XMPMETADATATEST("CaptureConditions.ExposureIndex", EK_L("200"));
			XMPMETADATATEST("CaptureConditions.ExposureProgram", EK_L("2"));
			XMPMETADATATEST("CaptureConditions.ExposureTime", EK_L("0.0333333"));
			XMPMETADATATEST("CaptureConditions.Flash.Flash", EK_L("0"));
			XMPMETADATATEST("CaptureConditions.Fnumber", EK_L("2.8"));
			XMPMETADATATEST("CaptureConditions.FocalLength", EK_L("5.9"));
			XMPMETADATATEST("CaptureConditions.LightSource", EK_L("0"));
			XMPMETADATATEST("CaptureConditions.MaxAperture", EK_L("2.8"));
			XMPMETADATATEST("CaptureConditions.MeteringMode", EK_L("3"));
			XMPMETADATATEST("CaptureConditions.ShutterSpeed", EK_L("5"));
			XMPMETADATATEST("CaptureConditions.SubjectDistance", EK_L("2.64"));


			XMPMETADATATEST("CaptureDevice.CameraOwner.CameraOwner", EK_L("Philip wu"));
			XMPMETADATATEST("CaptureDevice.Make", EK_L("EASTMAN KODAK COMPANY"));
			XMPMETADATATEST("CaptureDevice.Model", EK_L("KODAK DC4800 ZOOM DIGITAL CAMERA"));
			XMPMETADATATEST("CaptureDevice.SensingMethod", EK_L("2"));
			XMPMETADATATEST("CaptureDevice.Software", EK_L("Adobe Photoshop CS2 Windows"));


			//XMPMETADATATEST("DigitalProcess.ImageFileChangeDateTime", EK_L("2009:12:11 16:05:26"));
			//XMPMETADATATEST("DigitalProcess.ImageFileCreationDateTime", EK_L("2000:05:25 16:22:01"));
			XMPMETADATATEST("DigitalProcess.ImageSource", EK_L("3"));
			XMPMETADATATEST("DigitalProcess.Jpeg.SOF.ChrominanceSampling", EK_L("4:4:4"));
			XMPMETADATATEST("DigitalProcess.Jpeg.SOF.Components", EK_L("3"));
			XMPMETADATATEST("DigitalProcess.Jpeg.SOF.Height", EK_L("1440"));
			XMPMETADATATEST("DigitalProcess.Jpeg.SOF.HorizontalSampling", EK_L("1 1 1"));
			XMPMETADATATEST("DigitalProcess.Jpeg.SOF.Precision", EK_L("8"));
			XMPMETADATATEST("DigitalProcess.Jpeg.SOF.VerticalSampling", EK_L("1 1 1"));
			XMPMETADATATEST("DigitalProcess.Jpeg.SOF.Width", EK_L("2160"));

			XMPMETADATATEST("SceneContent.ImageCaptureDateTime.ImageCaptureDateTime", EK_L("2000:05:25 16:22:01"));
			XMPMETADATATEST("SceneContent.ImageDescription", EK_L("This a test picture for XMP"));
			XMPMETADATATEST("SceneContent.SceneType", EK_L("1"));
			XMPMETADATATEST("SceneContent.UserComment.MS.Author[0]", EK_L("80 0 104 0 105 0 108 0 105 0 112 0 32 0 119 0 117 0 0 0"));
			//The sequence of the keywords not always match.
			// 			XMPMETADATATEST("SceneContent.UserComment.MS.Keywords[0]", EK_L("88 0 77 0 80 0 0 0"));
			// 			XMPMETADATATEST("SceneContent.UserComment.MS.Keywords[1]", EK_L("116 0 101 0 115 0 116 0 0 0"));
			// 			XMPMETADATATEST("SceneContent.UserComment.MS.Keywords[2]", EK_L("112 0 104 0 105 0 108 0 105 0 112 0 0 0"));
			XMPMETADATATEST("SceneContent.UserComment.MS.Subject", //vuint8
				EK_L("84 0 104 0 105 0 115 0 32 0 97 0 32 0 116 0 101 0 115 0 116 0 32 0 112 0 105 0 99 0 116 0 117 0 114 0 101 0 32 0 102 0 111 0 114 0 32 0 88 0 77 0 80 0 0 0"));
			XMPMETADATATEST("SceneContent.UserComment.MS.Title", //vuint8
				EK_L("88 0 77 0 80 0 32 0 116 0 101 0 115 0 116 0 32 0 102 0 105 0 108 0 101 0 0 0"));

			if(nIndex == 0)
			{
				//Re-write the metadatas
				AccXMP->writeMetadata(rootMd);
			}
		}
		catch (PmtError e)
		{
			cout << e.getMsg();
			throw e;
		}	
	}
	cout << endl << "End testing XMP block in EXIF file..." << endl;
	cout << "Finished XMP accessor testing successfully." << endl;
}
#endif //PMT_XMP_ACCESSOR

