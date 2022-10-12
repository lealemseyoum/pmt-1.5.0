#include "PmtCommon.h"
#include "PmtMetadataT.h"
#include "PmtAttributeT.h"
#include "PmtDefinitionInterpreter.h"
#include "PmtXmlAccessor.h"
#include "PmtExifAccessor.h"
#include "PmtTiffAccessor.h"
#include "TiffDirectory.h"
#include "TiffStripImage.h"

void showFactory(const string & nsURI);
void showAttrFactory();
void printXmlFile(const char *file);

void testDefInterp();
void testInstInterp();
void testXmlAcc();
void testTiffAcc();
void testInstancePrefix();

const char *const PMT_TEST_NS_URI = "http://www.kodak.com/pmt/test";
const char *const PMT_NS_URI = "http://www.kodak.com/pmt/MetadataDefinition";
const char *const PMT_DEFAULT_MD_DEF = "MdXmlInfo.xml";

int main(int argc, char *argv[])
{
    testDefInterp();
    testInstInterp();
    testXmlAcc();
    testTiffAcc();

    testInstancePrefix();

    return 0;
}


void showFactory(const string & nsURI)
{
    PmtMetadataPtr md;

    cout << endl;
    cout << "Now showing Factory contents... " << endl;
    cout << "Namespace URI: " << nsURI << endl;

    PmtMetadataPtr root = PmtMetadata::create(nsURI);
    PmtMetadataFactory *factory = root->getMdFactory();
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


void showAttrFactory(const string nsURI)
{
    cout << endl;
    cout << "Show PmtAttribute Factory..." << endl;
    cout << "Namespace URI: " << nsURI << endl;

    PmtMetadataPtr root = PmtMetadata::create(nsURI);
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


void testDefInterp()
{
    cout << endl <<
        "************ Test Definiston Interpreter for NS **********" << endl;

#define NUM_TEST_FILES 5
    char files[NUM_TEST_FILES][50] = {
        "TestXmlInfo1.xml",
        "TestXmlInfo2.xml",
        "TestXmlInfo3.xml",
        "TestXmlInfo4.xml",
        "TestXmlInfo5.xml",
    };

    for (int i = 0; i < NUM_TEST_FILES; i++)
    {
        cout << endl << "------ Testing file: ";
        cout << files[i] << "------" << endl;
        // Test the Logical Definition Interpreter.
        string nsURI = PMT_TEST_NS_URI;
        PmtLogicalDefinitionInterpreter::load(files[i]);
        showFactory(nsURI);
        showAttrFactory(nsURI);

        // test facets validating
        cout << "Testing facet validating..." << endl;
        PmtMetadataPtr rootMd = PmtMetadata::create(nsURI);
        PmtMetadataPtr md = rootMd->getMetadatum("ABC2.XYZ1");
        dynamic_cast < PmtMetadataT < string > *>(md.ptr())->value() = "Green";
        if (md->validate() == false)
            printf("testDefInterp: error in validating metadata (%s, \"Green\")\n",
                   md->fullKey(false).c_str());
        dynamic_cast < PmtMetadataT < string > *>(md.ptr())->value() = "Cyan";
        if (md->validate() == true)
            printf
                ("testDefInterp: error in validating metadata (%s, \"Cyan\")\n",
                 md->fullKey(false).c_str());

        md = rootMd->getMetadatum("ABC1");
        dynamic_cast < PmtMetadataT < unsigned long >*>(md.ptr())->value() =
            68;
        if (md->validate() == false)
            printf("testDefInterp: error in validating metadata (%s, 68)\n",
                   md->fullKey(false).c_str());
        dynamic_cast < PmtMetadataT < unsigned long >*>(md.ptr())->value() =
            118;
        if (md->validate() == true)
            printf("testDefInterp: error in validating metadata (%s, 118)\n",
                   md->fullKey(false).c_str());

        cout << "...End of Testing facet validating" << endl << endl;

        // test attributes
        cout << "Testing attributes..." << endl;
        md = rootMd->getMetadatum("ABC2");
        md->getAttribute("Attr1");
        md->getAttribute("Attr2");
        PmtAttributeMap attrMap = md->getAttributes(false);
        PmtAttributeMap::iterator s = attrMap.begin();
        while (s != attrMap.end())
        {
            (*s).second->show(0);
            s++;
        }
        md->getAttributes();
        md->show();

        PmtAttributePtr attr = md->getAttribute("Attr2");
        if (attr->validate() == false)
        {
            cout << "Failed to validate the attribute below:" << endl;
            attr->show(0);
        }
        dynamic_cast < PmtAttributeT < long >*>(attr.ptr())->value() = 1001;
        if (attr->validate() == true)
        {
            cout << "Failed to validate the attribute below:" << endl;
            attr->show(0);
        }
        cout << "...End of Testing attributes" << endl << endl;

        PmtLogicalDefinitionInterpreter::erase();
    }
}


void testReadInst(const char *xsdFile, const char *xmlFile)
{
    cout << endl << "------- Test Reading: ";
    cout << xmlFile << ", " << xsdFile << "-------" << endl;
    PmtLogicalDefinitionInterpreter::load(xsdFile);
    PmtAccessorPtr inAcc = PmtAccessor::getAccessor(xmlFile);
    PmtMetadataPtr mdRoot = inAcc->readMetadata(PMT_NS_URI);
    cout << mdRoot;
    PmtLogicalDefinitionInterpreter::erase();
}

void testWriteInst(const char *xsdFile, const char *xmlFile,
                   const char *prefix)
{
    char outXml[] = "TestOut.xml";
    cout << endl << endl << "------- Test Writing: ";
    cout << xmlFile << ", " << xsdFile << "--------" << endl;

    PmtLogicalDefinitionInterpreter::loadWithAliases(xsdFile, "aliases.xml");
    PmtAccessorPtr xmlAcc, inAcc;
    inAcc = PmtAccessor::getAccessor(xmlFile);
    if (!inAcc)
    {
        throw PmtError(PMT_ERROR,
                       "can't open xml file with PmtAccessor::getAccessor",
                       WHERE);
    }
    PmtMetadataPtr mdRoot = PmtMetadata::create(PMT_NS_URI);
    PmtMetadataIterator mdIt = mdRoot->getMetadata("alias_1");
    inAcc->readMetadata(mdIt);

    xmlAcc = new PmtXmlAccessor();
    if (!xmlAcc->open(outXml, PMT_CREATE))
    {
        throw PmtError(PMT_ERROR, "can't create xml file", WHERE);
    }
    dynamic_cast < PmtXmlAccessor * >(xmlAcc.ptr())->setNSPrefix(prefix);
    xmlAcc->writeMetadata(mdRoot);
    xmlAcc->close();

    printXmlFile(outXml);

    PmtLogicalDefinitionInterpreter::erase();
}


void testInstInterp()
{
    cout << endl << "************ Test Instance Interpreter ************" <<
        endl;
    testReadInst("MdXmlInfo.xml", "TestInput.xml");
    testReadInst("MdXmlInfo.xml", "TestInput1.xml");
    testReadInst("MdXmlInfo.xml", "TestInput2.xml");
    testReadInst("MdXmlInfo1.xml", "TestInput2.xml");

    testWriteInst("MdXmlInfo.xml", "TestInput.xml", "xyz");
    testWriteInst("MdXmlInfo1.xml", "TestInput.xml", "myns");

}


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

void testXmlAcc()
{
    cout << endl << "********Test Xml accessor********" << endl;

    PmtLogicalDefinitionInterpreter::loadWithAliases(PMT_DEFAULT_MD_DEF,
                                                     "aliases.xml");

#if defined( HAVE_KIESEXIF ) || defined( HAVE_OPENEXIF )
    char inFile[] = "KodakDC260.jpg";
#else
    char inFile[] = "TestInput.xml";
#endif
    string xmlFile = "XmlAccTest.xml";

    cout << endl <<
        "--- Testing read all metadata from an Exif file and write to an xml file"
        << endl;

    // ----- testing read from an Exif file and write to an xml file
    PmtAccessorPtr inAcc = PmtAccessor::getAccessor(inFile);

    PmtMetadataPtr mdRoot = inAcc->readMetadata(PMT_NS_URI);
    PmtAccessorPtr xmlAcc = new PmtXmlAccessor();
    if (!xmlAcc->open(xmlFile.c_str(), PMT_CREATE))
    {
        throw PmtError(PMT_ERROR, "can't create xml file", WHERE);
    }
//    dynamic_cast<PmtXmlAccessor*>(xmlAcc.ptr())->setNSPrefix("pmt2");

    // writing xml
    xmlAcc->writeMetadata(mdRoot);

    xmlAcc->close();

    printXmlFile(xmlFile.c_str());

    cout << endl <<
        "--- Testing read from an xml file and write to another xml file" <<
        endl;

    // read from an xml file and write to another xml file
    xmlAcc = PmtAccessor::getAccessor(xmlFile.c_str());
    if (!xmlAcc)
    {
        throw PmtError(PMT_ERROR,
                       "can't open xml file with PmtAccessor::getAccessor",
                       WHERE);
    }
    mdRoot = PmtMetadata::create(PMT_NS_URI);
    PmtMetadataIterator mdIt = mdRoot->getMetadata("alias_1");
    xmlAcc->readMetadata(mdIt);
    xmlAcc->close();

    xmlAcc = new PmtXmlAccessor();
    if (!xmlAcc->open(xmlFile.c_str(), PMT_CREATE))
    {
        throw PmtError(PMT_ERROR, "can't create xml file", WHERE);
    }
//    dynamic_cast<PmtXmlAccessor*>(xmlAcc.ptr())->setNSPrefix("pmt2");
    xmlAcc->writeMetadata(mdRoot);
    xmlAcc->close();

    printXmlFile(xmlFile.c_str());

    cout << endl << "--- Testing read/write from the same xml file" << endl;

    // open xml for reading and writing
    if (!xmlAcc->open(xmlFile.c_str(), PMT_READWRITE))
    {
        throw PmtError(PMT_ERROR, "can't open xml file", WHERE);
    }

    mdRoot = PmtMetadata::create(PMT_NS_URI);
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

    mdRoot = PmtMetadata::create(PMT_NS_URI);
    mdRoot->getMetadatum("CaptureDevice.Make");
    mdRoot->getMetadatum("CaptureDevice.Model");
    md = mdRoot->getMetadatum("CaptureDevice", false);
    f2.open(xmlFile.c_str(), ios::in);
    f2 >> md;
    cout << endl << endl;
    cout << md;
    f2.close();
    inAcc->close();

    PmtLogicalDefinitionInterpreter::erase();
}


#define WIDTH   20
#define HEIGHT  20

void testTiffAcc()
{
    cout << endl << "********Test Tiff accessor********" << endl;
    PmtLogicalDefinitionInterpreter::loadWithAliases(PMT_DEFAULT_MD_DEF,
                                                     "aliases.xml");

    char file[] = "test.tif";

    int i;
    TiffImageFile imgFile;
    imgFile.open(file, "w");
    imgFile.createDirectory();
    TiffDirectory *dir = imgFile.getDirectory(0);

    TiffTagEntry *et;
    et = new TiffTagEntryT < unsigned long >(TIFFTAG_IMAGEWIDTH, EKTIFF_ULONG,
                                             1, WIDTH);
    dir->setGenericTag(*et);
    delete et;

    et = new TiffTagEntryT < unsigned long >(TIFFTAG_IMAGELENGTH, EKTIFF_ULONG,
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

    et = new TiffTagEntryT < unsigned long >(TIFFTAG_ROWSPERSTRIP,
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

    et = new TiffTagEntryT < unsigned long >(TIFFTAG_IMAGEWIDTH, EKTIFF_ULONG,
                                             1, WIDTH);
    dir->setGenericTag(*et);
    delete et;

    et = new TiffTagEntryT < unsigned long >(TIFFTAG_IMAGELENGTH, EKTIFF_ULONG,
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

    et = new TiffTagEntryT < unsigned long >(TIFFTAG_ROWSPERSTRIP,
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

        char buf[256];
        sprintf(buf, "\nWriting metadata to %s:\n", file);
        cout << buf;

        PmtMetadataPtr rootMd = PmtMetadata::create(PMT_NS_URI);
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
        dynamic_cast < PmtMetadataT < unsigned char >*>(md.ptr())->value() = 1;
        tifAcc->writeMetadata(md);
        md->print(cout);
        cout << endl;

        // write in IFD_EXIF_INTER
        key = "ImageContainer.Interoperability";
        md = rootMd->getMetadatum(key);
        dynamic_cast < PmtMetadataT < string > *>(md.ptr())->value() =
            "Interoperability here";
        tifAcc->writeMetadata(md);
        md->print(cout);
        cout << endl;

        // audio
        md = rootMd->getMetadatum("SceneContent.Audio");
        vector < unsigned char >val;
        for (i = 0; i < 88; i++)
            val.push_back(88 - i);
        dynamic_cast < PmtMetadataT < vector < unsigned char > > * >
            (md.ptr())->value() = val;
        tifAcc->writeMetadata(md);
        md->print(cout);
        cout << endl;

        imgFile.close();


        // ---- read metadata
        sprintf(buf, "\nReading metadata from %s:\n", file);
        cout << buf;

        imgFile.open(file, "r");
        tifAcc = new PmtTiffAccessor(&imgFile);

        // read from IFD_MAIN
        key = "CaptureDevice.Model";
        md = rootMd->getMetadatum(key);
        tifAcc->readMetadata(md);
        md->print(cout);
        cout << endl;

        // read from IFD_SUB
        key = "ImageContainer.Thumbnail.Orientation";
        md = rootMd->getMetadatum(key);
        tifAcc->readMetadata(md);
        md->print(cout);
        cout << endl;

        // read from IFD_EXIF
        key = "SceneContent.ImageCaptureDateTime.ImageCaptureDateTime";
        md = rootMd->getMetadatum(key);
        tifAcc->readMetadata(md);
        md->print(cout);
        cout << endl;

        key = "CaptureDevice.FilmSize";
        md = rootMd->getMetadatum(key);
        tifAcc->readMetadata(md);
        md->print(cout);
        cout << endl;

        key = "CaptureConditions.Fnumber";
        md = rootMd->getMetadatum(key);
        tifAcc->readMetadata(md);
        md->print(cout);
        cout << endl;

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
        key = "ImageContainer.Interoperability";
        md = rootMd->getMetadatum(key);
        tifAcc->readMetadata(md);
        md->print(cout);
        cout << endl;

        // audio
        key = "SceneContent.Audio";
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

    PmtLogicalDefinitionInterpreter::erase();
}


void testInstancePrefix()
{
    string inFile = "TestInput.xml";
    string xmlFile = "XmlAccTest.xml";

    cout << endl << endl <<
        "*********************** Testing reading/writing instance doc with prefix *********"
        << endl << endl;
    cout << endl <<
        "--- Testing read from an xml file and write to another xml file" <<
        endl;

    PmtLogicalDefinitionInterpreter::loadWithAliases(PMT_DEFAULT_MD_DEF,
                                                     "aliases.xml");

    // read from an xml file and write to another xml file
    PmtAccessorPtr xmlAcc, inAcc;
    inAcc = PmtAccessor::getAccessor(inFile.c_str());
    if (!inAcc)
    {
        throw PmtError(PMT_ERROR,
                       "can't open xml file with PmtAccessor::getAccessor",
                       WHERE);
    }
    PmtMetadataPtr mdRoot = PmtMetadata::create(PMT_NS_URI);
    PmtMetadataIterator mdIt = mdRoot->getMetadata("alias_1");
    inAcc->readMetadata(mdIt);

    xmlAcc = new PmtXmlAccessor();
    if (!xmlAcc->open(xmlFile.c_str(), PMT_CREATE))
    {
        throw PmtError(PMT_ERROR, "can't create xml file", WHERE);
    }
    dynamic_cast < PmtXmlAccessor * >(xmlAcc.ptr())->setNSPrefix("pmt2");
    xmlAcc->writeMetadata(mdRoot);
    xmlAcc->close();

    printXmlFile(xmlFile.c_str());

    cout << endl << "--- Testing read/write from the same xml file" << endl;

    // open xml for reading and writing
    if (!xmlAcc->open(xmlFile.c_str(), PMT_READWRITE))
    {
        throw PmtError(PMT_ERROR, "can't open xml file", WHERE);
    }

    mdRoot = PmtMetadata::create(PMT_NS_URI);
    mdIt = mdRoot->getMetadata("alias_2");
    inAcc->readMetadata(mdIt);

    PmtMetadataPtr md = mdRoot->getMetadatum("CaptureDevice.Make");
    md->setValueStr(EK_L("EKC"));

    // writing xml
    xmlAcc->writeMetadata(mdRoot);
    xmlAcc->close();
    inAcc->close();

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

    PmtLogicalDefinitionInterpreter::erase();
}

