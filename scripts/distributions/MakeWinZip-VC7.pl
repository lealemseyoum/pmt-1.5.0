#! c:\perl\bin\perl.exe -w

use strict;
use Cwd;         # For cwd()
use File::Path;  # For rmtree()

my $PMT_DIR;                            # Is setup in beginning of Main();
my $PMT_DISTRIBUTIONS_DIR;              # Is setup in beginning of Main();
my $BINARY_ZIP_FILENAME = "pmt-1_4-bin-win32.zip";
my $SOURCE_ZIP_FILENAME = "pmt-1_4-src.zip";
my $BINARY_ZIP_FILENAME_SWIO = "pmt-1_4-bin-win32-SWIO.zip";
my $SOURCE_ZIP_FILENAME_SWIO = "pmt-1_4-src-SWIO.zip";

# The paths to where we will do our building and testing. Can be overriden with
# command line arguments (-buidAndTestdir).
my $BUILD_AND_TEST_DIR = "\\ZipTemp";
my $PMT_TEMP_DIR_OPEN = "$BUILD_AND_TEST_DIR\\open\\Pmt";
my $PMT_TEMP_DIR_SWIO = "$BUILD_AND_TEST_DIR\\swio\\Pmt";
my $BINARY_TEST_DIR = "$BUILD_AND_TEST_DIR\\UnzippedBinaryTest" ;
my $SOURCE_TEST_DIR = "$BUILD_AND_TEST_DIR\\UnzippedSourceTest" ;

# This is a directory where DLL's are placed for running the tests. The
# directory will be placed in the front of the current PATH so that the correct
#  DLL's get picked up. The default is C:\temp_dlls but this can be overridden
# with a command-line argument (-tempDLLDir <path>).
my $TEMP_DLL_PATH = "\\temp_dlls" ;

# These are for storing the paths to the underlying toolkits to be used.
my $SWIO_TIFF_ROOT = 0 ;
my $OPEN_TIFF_ROOT = 0 ;
my $SWIO_EXIF_ROOT = 0 ;
my $OPEN_EXIF_ROOT = 0 ;
my $SWIO_JPEG_ROOT = 0 ;
my $OPEN_JPEG_ROOT = 0 ;
my $SWIO_XERCES_ROOT = 0 ;
my $OPEN_XERCES_ROOT = 0 ;
my $SWIO_IJL_ROOT = 0 ;

# If 1, do not run memrunner
my $NO_MEMRUNNER = 0 ;
# If 1, do source distribution only
my $SOURCE_ONLY = 0 ;

sub DoesAbsoluteFileExist($)
{
    my ($file) = @_;
    return -e $file;
}

sub DoesAbsoluteDirExist($)
{
    my ($directory) = @_;
    if (DoesAbsoluteFileExist($directory))
    {
        return -d $directory;
    }
    return 0;
}

sub DoesRelativeFileExist($)
{
    my ($file) = @_;
    my $curDir = cwd();
    return -e "$curDir/$file";
}

sub DoesRelativeDirExist($)
{
    my ($directory) = @_;
    my $curDir = cwd();
    if (DoesRelativeFileExist($directory))
    {
        return -d "$curDir/$directory";
    }
    return 0;
}

sub CopyToolkits($)
{
    my ($whichToolkits) = @_;

    # Get up to top level of view, just above the Pmt directory.
    chdir "..";

    if ( $whichToolkits eq "open" )
    {
        # Copy Jpeg/inc and Jpeg/lib
        system("xcopy $OPEN_JPEG_ROOT\\inc\\*.* $PMT_TEMP_DIR_OPEN\\Toolkits\\Jpeg\\inc\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/Jpeg directory");
        system("xcopy $OPEN_JPEG_ROOT\\lib\\*.* $PMT_TEMP_DIR_OPEN\\lib\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/Jpeg directory");

        # Copy openexif/inc and openexif/lib
        system("xcopy $OPEN_EXIF_ROOT\\inc\\*.* $PMT_TEMP_DIR_OPEN\\Toolkits\\OpenExif\\inc\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/Jpeg directory");
        system("xcopy $OPEN_EXIF_ROOT\\lib\\*.lib $PMT_TEMP_DIR_OPEN\\lib\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/Jpeg directory");        
          
        # Copy opentiff/inc and opentiff/lib
        system("xcopy $OPEN_TIFF_ROOT\\inc\\*.* $PMT_TEMP_DIR_OPEN\\Toolkits\\OpenTiff\\inc\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/Jpeg directory");
        system("xcopy $OPEN_TIFF_ROOT\\lib\\*.lib $PMT_TEMP_DIR_OPEN\\lib\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/Jpeg directory");

        # Copy the DLL Files
        system("xcopy $OPEN_EXIF_ROOT\\lib\\openexifi.dll $PMT_TEMP_DIR_OPEN\\bin\\*.* /s /e") == 0
            or die("Error: could not xcopy $OPEN_EXIF_ROOT\\lib\\openexifi.dll file");
        system("xcopy $OPEN_TIFF_ROOT\\lib\\EkTiffi.dll $PMT_TEMP_DIR_OPEN\\bin\\*.* /s /e") == 0
            or die("Error: could not xcopy $OPEN_EXIF_ROOT\\lib\\EkTiffi.dll file");
    }
    else
    {
        my $toolkitsDir = "$SWIO_JPEG_ROOT\\.." ;
        
        # Copy entire Ijl
        system("xcopy $SWIO_IJL_ROOT\\*.* $PMT_TEMP_DIR_SWIO\\Toolkits\\Ijl\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/Ijl directory");

        # Copy Jpeg/inc and Jpeg/lib
        system("xcopy $SWIO_JPEG_ROOT\\inc\\*.* $PMT_TEMP_DIR_SWIO\\Toolkits\\Jpeg\\inc\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/Jpeg directory");
        system("xcopy $SWIO_JPEG_ROOT\\lib\\*.* $PMT_TEMP_DIR_SWIO\\Toolkits\\Jpeg\\lib\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/Jpeg directory");

        # Copy Kies/inc and certain Kies/lib files
        system("xcopy $SWIO_EXIF_ROOT\\inc\\*.* $PMT_TEMP_DIR_SWIO\\Toolkits\\Kies\\inc\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/Kies directory");
        system("xcopy $SWIO_EXIF_ROOT\\lib\\debug\\ekexifio140d.lib $PMT_TEMP_DIR_SWIO\\Toolkits\\Kies\\lib\\debug\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/Kies/lib directory");
        system("xcopy $SWIO_EXIF_ROOT\\lib\\debug\\ekfpixjpeg140d.lib $PMT_TEMP_DIR_SWIO\\Toolkits\\Kies\\lib\\debug\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/Kies/lib directory");
        system("xcopy $SWIO_EXIF_ROOT\\lib\\debug\\exifio.lib $PMT_TEMP_DIR_SWIO\\Toolkits\\Kies\\lib\\debug\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/Kies/lib directory");
        system("xcopy $SWIO_EXIF_ROOT\\lib\\release\\ekexifio140.lib $PMT_TEMP_DIR_SWIO\\Toolkits\\Kies\\lib\\release\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/Kies/lib directory");
        system("xcopy $SWIO_EXIF_ROOT\\lib\\release\\ekfpixjpeg140.lib $PMT_TEMP_DIR_SWIO\\Toolkits\\Kies\\lib\\release\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/Kies/lib directory");
        system("xcopy $SWIO_EXIF_ROOT\\lib\\release\\exifio.lib $PMT_TEMP_DIR_SWIO\\Toolkits\\Kies\\lib\\release\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/Kies/lib directory");

        # Copy xerces-c/inc and xerces-c/win32
        system("xcopy $SWIO_XERCES_ROOT\\include\\*.* $PMT_TEMP_DIR_SWIO\\Toolkits\\xerces-c\\include\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/xerces-c directory");
        system("xcopy $SWIO_XERCES_ROOT\\win32\\*.* $PMT_TEMP_DIR_SWIO\\Toolkits\\xerces-c\\win32\\*.* /s /e") == 0
            or die("Error: could not xcopy the Toolkits/xerces-c directory");

        # Get DLL files from bin directory to /bin
        system("xcopy $toolkitsDir\\..\\..\\bin\\ijl15.dll $PMT_TEMP_DIR_SWIO\\bin\\*.* /s /e") == 0
            or die("Error: could not xcopy $toolkitsDir\\..\\..\\bin\\ijl15.dll file");
        system("xcopy $toolkitsDir\\..\\..\\bin\\ekexifio140d.dll $PMT_TEMP_DIR_SWIO\\bin\\*.* /s /e") == 0
            or die("Error: could not xcopy $toolkitsDir\\..\\..\\bin\\ekexifio140d.dll file");
        system("xcopy $toolkitsDir\\..\\..\\bin\\ekexifio140.dll $PMT_TEMP_DIR_SWIO\\bin\\*.* /s /e") == 0
            or die("Error: could not xcopy $toolkitsDir\\..\\..\\bin\\ekexifio140.dll file");
        system("xcopy $toolkitsDir\\..\\..\\bin\\ekfpixjpeg140d.dll $PMT_TEMP_DIR_SWIO\\bin\\*.* /s /e") == 0
            or die("Error: could not xcopy $toolkitsDir\\..\\..\\bin\\ekfpixjpeg140d.dll file");
        system("xcopy $toolkitsDir\\..\\..\\bin\\ekfpixjpeg140.dll $PMT_TEMP_DIR_SWIO\\bin\\*.* /s /e") == 0
            or die("Error: could not xcopy $toolkitsDir\\..\\..\\bin\\ekfpixjpeg140.dll file");
        system("xcopy $toolkitsDir\\..\\..\\bin\\xerces-c_1_2.dll $PMT_TEMP_DIR_SWIO\\bin\\*.* /s /e") == 0
            or die("Error: could not xcopy $toolkitsDir\\..\\..\\bin\\xerces-c_1_2.dll file");
        system("xcopy $toolkitsDir\\..\\..\\bin\\xerces-c_1_2D.dll $PMT_TEMP_DIR_SWIO\\bin\\*.* /s /e") == 0
            or die("Error: could not xcopy $toolkitsDir\\..\\..\\bin\\xerces-c_1_2D.dll file");

        # Restore the current working directory to Pmt.
        chdir $PMT_DIR;

        print("\nToolkits directory copied to Pmt successfully...\n");
    }
}



sub ShowHelpScreen()
{
    print "\n\n";
    print "Usage: perl MakeWinZip-VC7.pl [options] \n";
    print "-h --help                -- Shows this help screen\n";
    print "-sourceOnly              -- Only build and test a source distribution\n";
    print "-noMemrunner             -- Do not run memrunner on test programs\n";
    print "-open                    -- Build with the OpenSource toolkits\n";
    print "-swio                    -- Build with the SWIO toolkits\n";
    print "-openToolkitsPath <path> -- Specify absolute path to OpenSource toolkits\n";
    print "                            It will be assumed that the toolkits will be in\n";
    print "                            the jpeg, openexif, opentiff and xerces-c\n";
    print "                            directories. This option can be used in combination\n";
    print "                            with any of the 4 options below to override the\n";
    print "                            previous assumption.\n";
    print " -openJpegPath <path>    -- Specify absolute path to IJG Jpeg toolkit\n";
    print " -openExifPath <path>    -- Specify absolute path to OpenExif toolkit\n";
    print " -openTiffPath <path>    -- Specify absolute path to OpenTiff toolkit\n";
    print " -openXercesPath <path>  -- Specify absolute path to Xerces-C toolkit\n";
    print "-swioToolkitsPath <path> -- Specify absolute path to SWIO toolkits\n";
    print "                            It will be assumed that the toolkits will be in\n";
    print "                            the jpeg, kies, ektiff, ijl and xerces-c\n";
    print "                            directories. This option can be used in combination\n";
    print "                            with any of the 5 options below to override the\n";
    print "                            previous assumption.\n";
    print " -swioJpegPath <path>    -- Specify absolute path to SWIO Jpeg toolkit\n";
    print " -swioExifPath <path>    -- Specify absolute path to SWIO Kies toolkit\n";
    print " -swioTiffPath <path>    -- Specify absolute path to SWIO EkTiff toolkit\n";
    print " -swioXercesPath <path>  -- Specify absolute path to SWIO Xerces-C toolkit\n";
    print " -swioIJLPath <path>     -- Specify absolute path to SWIO IJL toolkit\n";
    print "-tempDLLPath <path>      -- (Optional) Specify absolute path to where DLL's\n";
    print "                            will be copied. The path will be put in the front\n";
    print "                            of your PATHto ensure the correct DLL's are picked\n";
    print "                            up during testing. Default: \\temp_dlls \n";
    print "-buildAndTestDir <path>  -- (Optional) Specify path for building and testing\n";
    print "                            Default: \\ZipTemp \n";

    print "Example:  perl MakeWinZip-VC7.pl -open -openToolkitsPath D:\\Toolkits\\open\n";
    print "   This would create the zip files for both binary and source distributions\n";
    print "   using the OpenSource toolkits, and test them. The resultant zips file would\n";
    print "   be placed in the scripts\\distributions\\ directory.\n\n";
    print "NOTE: This perlscipt must be run from the scripts/distributions directory.";

    print "\n";
}

sub CreateBinaryZipFile($)
{
    my $whichToolkits= $_[0] ;
    
    # Make the zip file
    my $zipFilePath ;
    if ( $whichToolkits eq "open" )
    {
        $zipFilePath = "$PMT_DISTRIBUTIONS_DIR\\$BINARY_ZIP_FILENAME" ;
    }
    else
    {
        $zipFilePath = "$PMT_DISTRIBUTIONS_DIR\\$BINARY_ZIP_FILENAME_SWIO" ;
    }
    my $binaryOrSourceString = "Binary";
    CreateZipFileCommon($zipFilePath, $binaryOrSourceString, $whichToolkits);

}

sub CreateAndTestBinaryZipFile($)
{
    my ($whichToolkits) = @_ ;
    
    my $tempDir ;
    if ( $whichToolkits eq "open" )
    {
        $tempDir = $PMT_TEMP_DIR_OPEN ;
    }
    else
    {
        $tempDir = $PMT_TEMP_DIR_SWIO ;
    }
    
    SetEnvironmentVariables($whichToolkits) ; 

    # Build all projects in the temporary Pmt directory, so the libs can
    # get included in the zip file.
    BuildAllProjects("$PMT_TEMP_DIR_OPEN\\..", 1);

    CopyToolkits($whichToolkits) ;
    
    system("xcopy $tempDir\\lib\\*.dll $tempDir\\bin\\*.* /s /e /y") == 0
        or die("Error: could not xcopy $tempDir\\lib*.dll files");    
    
    CreateBinaryZipFile($whichToolkits);
    
    my $zipFilePath ;
    if ( $whichToolkits eq "open" )
    {
        $zipFilePath = "$PMT_DISTRIBUTIONS_DIR\\$BINARY_ZIP_FILENAME" ;
    }
    else
    {
        $zipFilePath = "$PMT_DISTRIBUTIONS_DIR\\$BINARY_ZIP_FILENAME_SWIO" ;
    }
        
    UnzipZipFile("$BINARY_TEST_DIR-$whichToolkits", "$zipFilePath");

    # Will build and test the test programs only. The binary distribution .sln's are
    # used, which do not include the source code projects. I.e., the libs are linked
    # against. The libs are not built here. (The libs were previously built in the
    # Pmt directory when BuildAllProjects() was called above. Those libs were zipped
    # up and unzipped into the UnzippedBinaryTest/lib directory.)

    # Clean $TEMP_DLL_PATH and set PATH to the \bin directory
    SetupEmptyDir("$TEMP_DLL_PATH") ;
    $ENV{PATH} = "$BINARY_TEST_DIR-$whichToolkits\\Pmt\\bin;$ENV{PATH}" ;

    my $memrunnerLogPathPrefix ;
    if ( $whichToolkits eq "open" )
    {
        CopyDLLs("$OPEN_XERCES_ROOT\\bin") ;
        $memrunnerLogPathPrefix = "$PMT_DISTRIBUTIONS_DIR\\open_bin_" ;
    }
    else
    {
        $memrunnerLogPathPrefix = "$PMT_DISTRIBUTIONS_DIR\\swio_bin_" ;
    }
    
    TestUnzippedFiles("$BINARY_TEST_DIR-$whichToolkits", 1, $memrunnerLogPathPrefix);
}

# checks the file (first argument) for the string (second argument)
sub IsTextInFile($$)
{
    # This subroutine inspects the given file for the given string.
    # If the string is found, then we return true; else, return false.

    my ($filePath, $searchText) = @_;
    open(INFILE, "$filePath") or die("Error: could not open $filePath");

    # Read entire file in.
    my @allLines;
    @allLines = <INFILE>;

    # grep whole file for text
    my $result;
    $result = grep { /$searchText/ } @allLines;
    close(INFILE) or die("Error: could not close $filePath");
    return $result;
}

# Remove the first two lines of the file which contain date and time information
sub CreateTempResultsFile($$)
{
    my ($resultsFilePath, $tempResultsFilePath) = @_;

    open(INFILE, $resultsFilePath) or die("Error: could not create temporary results file");
    open(OUTFILE, ">$tempResultsFilePath") or die("Error: could not create temporary results file");

    my @lines = <INFILE> ;
    # Leave first (Time and Date) and last (timing) lines out
    for ( my $i = 1; $i < $#lines; $i++ )
    {
        print OUTFILE $lines[$i] ;
    }

    close(INFILE) or die("Error: could not create temporary results file");
    close(OUTFILE) or die("Error: could not create temporary results file");
}

sub TestProject($$$$)
{
    my ($slnName, $configuration, $executablePathName, $memrunnerLogPath) = @_;
    my $BASELINE_FILE           = "GroundTrue.txt";
    my $BASELINE_FILE_TEMP      = "GroundTrue.tmp";
    my $BASELINE_FILE_RUN       = "GroundTrueRun.txt";
    my $BASELINE_FILE_RUN_TEMP  = "GroundTrueRun.tmp";

    my $WINDIFF_COMPARE_FILE = "BinaryDifferences.txt";

    system("devenv ../../projects/Win32/VC7.1/$slnName /build \"$configuration\"") == 0
        or die("Error: could not build \"$slnName - $configuration\"");

    if( $NO_MEMRUNNER )
    {
        system("$executablePathName > $BASELINE_FILE_RUN") == 0
            or die("Error: unable to execute test program $executablePathName");
    }
    else
    {
        system("memrunner -l \"$memrunnerLogPath\" $executablePathName > $BASELINE_FILE_RUN") == 0
            or die("Error: unable to execute test program $executablePathName");
    }

    CreateTempResultsFile($BASELINE_FILE, $BASELINE_FILE_TEMP);
    CreateTempResultsFile($BASELINE_FILE_RUN, $BASELINE_FILE_RUN_TEMP);

    # Windiff the results of each test program against a baseline text file that has the correct output.
    system("windiff $BASELINE_FILE_TEMP $BASELINE_FILE_RUN_TEMP -Sslrdx $WINDIFF_COMPARE_FILE") == 0
        or die("Error: unable to test unzipped files");

    not IsTextInFile($WINDIFF_COMPARE_FILE, "different") or die("Error: Results of test file did not compare to baseline results");

    print("\nSolution $slnName - $configuration tested successfully...\n");
}

sub TestUnzippedFiles($$$)
{
    my ($testDir, $binaryBuildsOnly, $memrunnerLogPathPrefix) = @_;

    # Always delete the test.xml file prior to running test, so the results are consistent when comparing
    # against the baseline.

    # PmtInterpreterTest
    chdir "$testDir/Pmt/Test/PmtInterpreterTest";

    if (! $binaryBuildsOnly)
    {
        unlink("test.xml");
        TestProject("PmtInterpreterTest.sln", "Debug",
            'debug\PmtInterpreterTest.exe', $memrunnerLogPathPrefix . "interp_debug.log");
            
        unlink("test.xml");
        TestProject("PmtInterpreterTest.sln", "Unicode Debug",
            'debugu\PmtInterpreterTest.exe', $memrunnerLogPathPrefix . "interp_unidebug.log");
            
        unlink("test.xml");
        TestProject("PmtInterpreterTesti.sln", "Debug",
            'debugi\PmtInterpreterTesti.exe', $memrunnerLogPathPrefix . "interpi_debug.log");
        
        unlink("test.xml");
        TestProject("PmtInterpreterTesti.sln", "Unicode Debug",
            'debugiu\PmtInterpreterTesti.exe', $memrunnerLogPathPrefix . "interpi_unidebug.log");
    }

    unlink("test.xml");
    TestProject("PmtInterpreterTest.sln", "Release",
        'release\PmtInterpreterTest.exe', $memrunnerLogPathPrefix . "interp_release.log");

    unlink("test.xml");
    TestProject("PmtInterpreterTest.sln", "Unicode Release",
        'releaseu\PmtInterpreterTest.exe', $memrunnerLogPathPrefix . "interp_unirelease.log");

    unlink("test.xml");
    TestProject("PmtInterpreterTesti.sln", "Release",
        'releasei\PmtInterpreterTesti.exe', $memrunnerLogPathPrefix . "interpi_release.log");

    unlink("test.xml");
    TestProject("PmtInterpreterTesti.sln", "Unicode Release",
        'releaseiu\PmtInterpreterTesti.exe', $memrunnerLogPathPrefix . "interpi_unirelease.log");

    # AccessorTest
    chdir "$testDir/Pmt/Test/AccessorTest";

    if (! $binaryBuildsOnly)
    {
        TestProject("AccessorTest.sln", "Debug", 'debug\AccessorTest.exe',
            $memrunnerLogPathPrefix . "acc_debug.log");
        TestProject("AccessorTest.sln", "Unicode Debug", 'debugu\AccessorTest.exe',
            $memrunnerLogPathPrefix . "acc_unidebug.log");
        TestProject("AccessorTesti.sln", "Debug", 'debugi\AccessorTesti.exe',
            $memrunnerLogPathPrefix . "acci_debug.log");
        TestProject("AccessorTesti.sln", "Unicode Debug", 'debugiu\AccessorTesti.exe',
            $memrunnerLogPathPrefix . "acci_unidebug.log");
    }

    TestProject("AccessorTest.sln", "Release", 'release\AccessorTest.exe',
        $memrunnerLogPathPrefix . "acc_release.log");
    TestProject("AccessorTest.sln", "Unicode Release", 'releaseu\AccessorTest.exe',
        $memrunnerLogPathPrefix . "acc_unirelease.log");
    TestProject("AccessorTesti.sln", "Release", 'releasei\AccessorTesti.exe',
        $memrunnerLogPathPrefix . "acci_release.log");
    TestProject("AccessorTesti.sln", "Unicode Release", 'releaseiu\AccessorTesti.exe',
        $memrunnerLogPathPrefix . "acci_unirelease.log");

    # VisitorTest
    #chdir "$testDir/Pmt/Test/VisitorTest";

    #if (! $binaryBuildsOnly)
    #{
    #    TestProject("VisitorTest.sln", "Debug", 'debug\VisitorTest.exe',
    #        $memrunnerLogPathPrefix . "visitor_debug.log");
    #    TestProject("VisitorTest.sln", "Unicode Debug", 'debugu\VisitorTest.exe',
    #        $memrunnerLogPathPrefix . "visitor_unidebug.log");
    #    TestProject("VisitorTesti.sln", "Debug", 'debugi\VisitorTesti.exe',
    #        $memrunnerLogPathPrefix . "visitori_debug.log");
    #    TestProject("VisitorTesti.sln", "Unicode Debug", 'debugiu\VisitorTesti.exe',
    #        $memrunnerLogPathPrefix . "visitori_unidebug.log");
    #}

    #TestProject("VisitorTest.sln", "Release", 'release\VisitorTest.exe',
    #    $memrunnerLogPathPrefix . "visitor_release.log");
    #TestProject("VisitorTest.sln", "Unicode Release", 'releaseu\VisitorTest.exe',
    #    $memrunnerLogPathPrefix . "visitor_unirelease.log");
    #TestProject("VisitorTesti.sln", "Release", 'releasei\VisitorTesti.exe',
    #    $memrunnerLogPathPrefix . "visitori_release.log");
    #TestProject("VisitorTesti.sln", "Unicode Release", 'releaseiu\VisitorTesti.exe',
    #    $memrunnerLogPathPrefix . "visitori_unirelease.log");


    # Restore to Pmt directory.
    chdir $PMT_DIR;

    print("\nTested all unzipped files for $testDir directory successfully...\n");
}

# Copy the DLL's to the path specified in $TEMP_DLL_PATH
sub CopyDLLs($)
{
    my ($dllFromPath) = @_;
    system("xcopy $dllFromPath\\*.dll \"$TEMP_DLL_PATH\\*.dll\" /Y") == 0
        or die("Error: could not xcopy the DLL's");
}

# Build all projects relative to given Pmt directory.
sub BuildAllProjects($$)
{
    my ($relativeDir,$binaryBuildsOnly) = @_;
    my $PROJECTS_DIR = "$relativeDir/Pmt/projects/Win32/VC7.1";
    chdir $PROJECTS_DIR;

    # Build all the static code and all the dynamic code.
	# Build only the Release (not Debug) projects if the $binaryBuildsOnly
	# flag is true.
    unless ($binaryBuildsOnly)
    {
        system("devenv BuildAllStatic.sln /build \"Debug\"") == 0
            or die("Error: could not build static debug projects");
        system("devenv BuildAllStatic.sln /build \"Unicode Debug\"") == 0
            or die("Error: could not build static unicode debug projects");
        system("devenv BuildAllDynamic.sln /build \"Debug\"") == 0
            or die("Error: could not build dynamic debug projects");
        system("devenv BuildAllDynamic.sln /build \"Unicode Debug\"") == 0
            or die("Error: could not build dynamic unicode debug projects");
    }

    system("devenv BuildAllStatic.sln /build \"Release\"") == 0
        or die("Error: could not build static release projects");
    system("devenv BuildAllStatic.sln /build \"Unicode Release\"") == 0
        or die("Error: could not build static unicode release projects");
    system("devenv BuildAllDynamic.sln /build \"Release\"") == 0
        or die("Error: could not build dynamic release projects");
    system("devenv BuildAllDynamic.sln /build \"Unicode Release\"") == 0
        or die("Error: could not build dynamic unicode release projects");

    # Copy the DLL's to the directory specified in TEMP_DLL_PATH
    CopyDLLs("$relativeDir\\Pmt\\lib");

    # Restore to Pmt directory.
    chdir $PMT_DIR;

    print("\nAll projects built successfully...\n");
}

sub SetEnvironmentVariables($)
{
    my $whichToolkits = $_[0] ;
    
    if ( $whichToolkits eq "open" )
    {
        $ENV{JPEGROOT} = $OPEN_JPEG_ROOT ;
        $ENV{EXIFROOT} = $OPEN_EXIF_ROOT ;
        $ENV{TIFFROOT} = $OPEN_TIFF_ROOT ;
        $ENV{XERCESCROOT} = "$OPEN_XERCES_ROOT" ;
    }
    else
    {
        $ENV{JPEGROOT} = $SWIO_JPEG_ROOT ;
        $ENV{EXIFROOT} = $SWIO_EXIF_ROOT ;
        $ENV{TIFFROOT} = $SWIO_TIFF_ROOT ;
        $ENV{XERCESCROOT} = "$SWIO_XERCES_ROOT" ;
        $ENV{IJLROOT} = $SWIO_IJL_ROOT ;    
    }
}

sub UnzipZipFile($$)
{
    my ($testDir, $zipFilePath) = @_;

    # First delete all of /Pmt, if it exists.
    if (DoesAbsoluteDirExist("$testDir"))
    {
        rmtree("$testDir") or die("Error: could not delete $testDir");
    }

    # Unzip entire zip file into /Pmt directory
    system("7z x $zipFilePath -o$testDir") == 0 or die("Error: unable to unzip $zipFilePath");

    print("\nFile unzipped successfully...\n");
}

sub CreateZipFileCommon($$$)
{
    my ($zipFilePath, $binaryOrSourceString, $whichToolkits) = @_;

    # Change to temporary directory, just above the Pmt directory.
    chdir "$BUILD_AND_TEST_DIR\\$whichToolkits";

    # Delete the old file first, if it exists.
    if (DoesAbsoluteFileExist("$zipFilePath"))
    {
        unlink("$zipFilePath") or die("Error: could not delete $zipFilePath");
    }
    
    system("7z a -tzip $zipFilePath -r \@$PMT_DISTRIBUTIONS_DIR/WinZip" .
		$binaryOrSourceString . "Files.txt" . " -x\@$PMT_DISTRIBUTIONS_DIR/WinZip" .
			$binaryOrSourceString . "FilesExclude.txt") == 0
        or die("Error: could not create $zipFilePath ZIP file");

    # Restore to Pmt directory.
    chdir $PMT_DIR;

    print("\nZip file created successfully...\n");
}

sub CreateSourceZipFile($)
{
    my $whichToolkits = $_[0] ;
    my $zipFilePath ;
    if ( $whichToolkits eq "open" )
    {
        $zipFilePath = "$PMT_DISTRIBUTIONS_DIR\\$SOURCE_ZIP_FILENAME" ;
    }
    else
    {
        $zipFilePath = "$PMT_DISTRIBUTIONS_DIR\\$SOURCE_ZIP_FILENAME_SWIO" ;
    }
    my $binaryOrSourceString = "Source";
    CreateZipFileCommon($zipFilePath, $binaryOrSourceString, $whichToolkits);
}

sub CreateAndTestSourceZipFile($)
{
    my $whichToolkits = $_[0] ;
    
    CreateSourceZipFile($whichToolkits);
    
    my $zipFilePath ;
    if ( $whichToolkits eq "open" )
    {
        $zipFilePath = "$PMT_DISTRIBUTIONS_DIR\\$SOURCE_ZIP_FILENAME" ;
    }
    else
    {
        $zipFilePath = "$PMT_DISTRIBUTIONS_DIR\\$SOURCE_ZIP_FILENAME_SWIO" ;
    }
    UnzipZipFile("$SOURCE_TEST_DIR-$whichToolkits", "$zipFilePath");
    SetEnvironmentVariables($whichToolkits);
    BuildAllProjects("$SOURCE_TEST_DIR-$whichToolkits", 0);
        
    my $memrunnerLogPathPrefix ;
    if ( $whichToolkits eq "open" )
    {
        CopyDLLs("$OPEN_XERCES_ROOT\\bin") ;
        CopyDLLs("$OPEN_TIFF_ROOT\\lib");
        CopyDLLs("$OPEN_EXIF_ROOT\\lib");
        $memrunnerLogPathPrefix = "$PMT_DISTRIBUTIONS_DIR\\open_src_" ;
    }
    else
    {
        system("xcopy $SWIO_XERCES_ROOT\\..\\..\\..\\bin\\xerces*.dll \"$TEMP_DLL_PATH\\*.*\" /Y") == 0
            or die("Error: could not xcopy the Xerces DLL's");            
        system("xcopy $SWIO_JPEG_ROOT\\..\\..\\..\\bin\\ijl15.dll \"$TEMP_DLL_PATH\\*.*\" /s /e") == 0
            or die("Error: could not xcopy $SWIO_JPEG_ROOT\\..\\..\\..\\bin\\ijl15.dll file");
        system("xcopy $SWIO_EXIF_ROOT\\..\\..\\bin\\ekexifio14*.dll \"$TEMP_DLL_PATH\\*.*\" /s /e") == 0
            or die("Error: could not xcopy $SWIO_EXIF_ROOT\\..\\..\\bin\\ekexifio14*.dll file");
        system("xcopy $SWIO_EXIF_ROOT\\..\\..\\..\\bin\\ekfpixjpeg*.dll \"$TEMP_DLL_PATH\\*.*\" /s /e") == 0
            or die("Error: could not xcopy $SWIO_EXIF_ROOT\\..\\..\\..\\bin\\ekfpixjpeg*.dll file");
            
        $memrunnerLogPathPrefix = "$PMT_DISTRIBUTIONS_DIR\\swio_src_" ;
    }
    
    TestUnzippedFiles("$SOURCE_TEST_DIR-$whichToolkits", 0, $memrunnerLogPathPrefix);
}

sub createBuildAndTestDistributions($)
{
    my ($toolkits) = @_ ;

    if ( $toolkits eq "open" or $toolkits eq "both" )
    {
        CreateAndTestSourceZipFile("open") ;
    }
    SetupEmptyDir("$TEMP_DLL_PATH") ;
    if ( $toolkits eq "swio" or $toolkits eq "both" )
    {
        CreateAndTestSourceZipFile("swio") ;
    }
    # SetupEmptyDir("$TEMP_DLL_PATH") ;
    unless ($SOURCE_ONLY)
    {
        if ( $toolkits eq "open" or $toolkits eq "both" )
        {
            CreateAndTestBinaryZipFile("open") ;
        }
        SetupEmptyDir("$TEMP_DLL_PATH") ;
        if ( $toolkits eq "swio" or $toolkits eq "both" )
        {
            CreateAndTestBinaryZipFile("swio") ;
        }
        SetupEmptyDir("$TEMP_DLL_PATH") ;
    }
}

sub CopySourceTreeToDir($)
{
    my $dir = $_[0] ;
    
    # make temp Pmt directory and copy the complete source tree to it
    mkdir("$dir") == 1 or die("Error: could not create $dir");
    system("xcopy *.* $dir /s /e") == 0 
	    or die("Error: could not xcopy to temporary directory"); 

    # Get up to top level of view, just above the Pmt directory.
    chdir "..";

    # Also copy the src\*.h files to an inc directory.
    opendir(DIR, $dir . "\\src");
    my $file;
    while (defined($file = readdir(DIR)))
    {
        my $dirPath = "$dir\\src\\$file";
        if ((-d $dirPath) and ($file ne ".") and 
	    ($file ne "..") and ($file ne "CVS"))
        {
            system("xcopy $dirPath\\*.h $dir\\inc /I") == 0
        		or die("Error: could not xcopy to temporary directory");
        }
    }
    closedir(DIR);

    # Restore the current working directory to Pmt.
    chdir $PMT_DIR;

    print("\nCopied files to temporary directory successfully...\n");
}

sub SetupEmptyDir($)
{
    my $dir = $_[0] ;
    if ( DoesAbsoluteDirExist("$dir") )
    {
	    # There's a flaky case in Windows that can occur. If the focus is on
	    # the temp directory in Windows Explorer, the directory may get
	    #  deleted but then may not be able to be created. Try to catch this
	    # error and issue a specific message about it.
	    rmtree("$dir") != 0 or 
	        die("\n\nError: could not delete $dir. " .
		    "Make sure you do not have the focus on this " .
		    "directory in Windows Explorer when running this script!") ;
    }
    mkdir("$dir") == 1 or die("Error: could not create $dir") ;
}

sub ProcessCommandLineArgs($)
{
    # NOTE: "$#ARGV" is 1 less than the actual number of command line arguments
    #       passed in.

    # If no arguments on the command line, or a -h or --help is passed
    # in, show the help screen.
    if (($#ARGV == -1) or ($ARGV[0] eq "-h") 
	    or ($ARGV[0] eq "--help"))
    {
        ShowHelpScreen();
        exit;
    }

    # Some variables used for processing command line arguments:
    # Default is to build and test distributions with both, the OpenSource
    # toolkits and the SWIO toolkits. The user can specify if he only want
    # to build and test with one or the other.
    my $openToolkits = 0 ;
    my $swioToolkits = 0 ;

    # If we are building with the OpenSource toolkits, the user must specify
    # the path to where all the toolkits are, and it will be assumed that the
    # the toolkit directories will be jpeg, opentiff, openexif, and xerces-c.
    # Also, the path to each toolkit can  be specified individually or
    # a combination of the general toolkit path and an individual toolkit path
    # that will override the general toolkit path for the specified toolkit.
    my $openToolkitsPath = 0 ;
    my $openJpegPath = 0 ;
    my $openExifPath = 0 ;
    my $openTiffPath = 0 ;
    my $openXercesPath = 0 ;

    # If we are building with the SWIO toolkits, the user must specify
    # the path to where all the toolkits are, and it will be assumed that the
    # the toolkit directories will be jpeg, ektiff, kies, and xerces-c.
    # Also, the path to each toolkit can  be specified individually or
    # a combination of the general toolkit path and an individual toolkit path
    # that will override the general toolkit path for the specified toolkit.   
    my $swioToolkitsPath = 0 ;
    my $swioJpegPath = 0 ;
    my $swioExifPath = 0 ;
    my $swioTiffPath = 0 ;
    my $swioXercesPath = 0 ;
    my $swioIJLPath = 0 ;

    for (my $argNum = 0 ; $argNum <= $#ARGV ; $argNum++)
    {
    	if ($ARGV[$argNum] eq "-sourceOnly") 
	    { 
	        $SOURCE_ONLY = 1;
	    }
	    elsif ($ARGV[$argNum] eq "-noMemrunner") 
	    { 
	        $NO_MEMRUNNER = 1; 
	    }
	    elsif ($ARGV[$argNum] eq "-tempDLLPath") 
	    {
	        $TEMP_DLL_PATH = $ARGV[++$argNum] ;
	    }
	    elsif ($ARGV[$argNum] eq "-buildAndTestDir")
	    {
	        $BUILD_AND_TEST_DIR = $ARGV[++$argNum] ;
	        $PMT_TEMP_DIR_OPEN = "$BUILD_AND_TEST_DIR\\open\\Pmt";
            $PMT_TEMP_DIR_SWIO = "$BUILD_AND_TEST_DIR\\swio\\Pmt";
            $BINARY_TEST_DIR = "$BUILD_AND_TEST_DIR\\UnzippedBinaryTest" ;
            $SOURCE_TEST_DIR = "$BUILD_AND_TEST_DIR\\UnzippedSourceTest" ;
	    }
	    elsif ($ARGV[$argNum] eq "-open") 
	    { 
	        $openToolkits = 1; 
	    }
	    elsif ($ARGV[$argNum] eq "-swio") 
	    { 
	        $swioToolkits = 1; 
	    }
	    elsif ($ARGV[$argNum] eq "-openToolkitsPath")
	    {
	        $openToolkitsPath = $ARGV[++$argNum] ;
	    }
	    elsif ($ARGV[$argNum] eq "-openJpegPath")
	    {
	        $openJpegPath = $ARGV[++$argNum] ;
	    }
	    elsif ($ARGV[$argNum] eq "-openExifPath")
	    {
	        $openExifPath = $ARGV[++$argNum] ;
	    }
	    elsif ($ARGV[$argNum] eq "-openTiffPath")
	    {
	        $openTiffPath = $ARGV[++$argNum] ;
	    }
	    elsif ($ARGV[$argNum] eq "-openXercesPath")
	    {
	        $openXercesPath = $ARGV[++$argNum] ;
	    }
	    elsif ($ARGV[$argNum] eq "-swioToolkitsPath")
	    {
	        $swioToolkitsPath = $ARGV[++$argNum] ;
	    }
	    elsif ($ARGV[$argNum] eq "-swioJpegPath")
	    {
	        $swioJpegPath = $ARGV[++$argNum] ;
	    }
	    elsif ($ARGV[$argNum] eq "-swioExifPath")
	    {
	        $swioExifPath = $ARGV[++$argNum] ;
	    }
	    elsif ($ARGV[$argNum] eq "-swioTiffPath")
	    {
	        $swioTiffPath = $ARGV[++$argNum] ;
	    }
	    elsif ($ARGV[$argNum] eq "-swioXercesPath")
	    {
	        $swioXercesPath = $ARGV[++$argNum] ;
	    }
	    elsif ($ARGV[$argNum] eq "-swioIJLPath")
	    {
            $swioIJLPath = $ARGV[++$argNum] ;
	    }
        else
        {
            print "\nERROR: Invalid argument - $ARGV[$argNum]\n";
            ShowHelpScreen();
            exit;
        }
    }

    # Set up global variables that point to open toolkits
    if( $openToolkitsPath )
    {
	    $OPEN_JPEG_ROOT = "$openToolkitsPath\\jpeg" ;
	    $OPEN_EXIF_ROOT = "$openToolkitsPath\\openexif" ;
	    $OPEN_TIFF_ROOT = "$openToolkitsPath\\opentiff" ;
	    $OPEN_XERCES_ROOT = "$openToolkitsPath\\xerces-c" ;
    }
    if ( $openJpegPath ) { $OPEN_JPEG_ROOT = $openJpegPath ; }
    if ( $openExifPath ) { $OPEN_EXIF_ROOT = $openExifPath ; }
    if ( $openTiffPath ) { $OPEN_TIFF_ROOT = $openTiffPath ; }
    if ( $openXercesPath ) { $OPEN_XERCES_ROOT = $openXercesPath ; }

    # Set up global variables that point to swio toolkits
    if ( $swioToolkitsPath )
    {
	    $SWIO_JPEG_ROOT = "$swioToolkitsPath\\jpeg" ;
	    $SWIO_EXIF_ROOT = "$swioToolkitsPath\\kies" ;
	    $SWIO_TIFF_ROOT = "$swioToolkitsPath\\ektiff" ;
	    $SWIO_XERCES_ROOT = "$swioToolkitsPath\\xerces-c" ;
	    $SWIO_IJL_ROOT = "$swioToolkitsPath\\ijl" ;
    }
    if ( $swioJpegPath ) { $SWIO_JPEG_ROOT = $swioJpegPath ; }
    if ( $swioExifPath ) { $SWIO_EXIF_ROOT = $swioExifPath ; }
    if ( $swioTiffPath ) { $SWIO_TIFF_ROOT = $swioTiffPath ; }
    if ( $swioXercesPath ) { $SWIO_XERCES_ROOT = $swioXercesPath ; }

    # If either was specified, so we do the default (both)
    if ( ! $openToolkits and ! $swioToolkits )
    {
	    $openToolkits = 1 ;
	    $swioToolkits = 1 ;
	    $_[0] = "both" ;
    }
    elsif ( $openToolkits )
    {
    	$_[0] = "open" ;
    }
    else
    {
    	$_[0] = "swio" ;
    }

    # make sure we have the path to all the toolkits we need and that they are
    # valid paths
    if ( $openToolkits )
    {
    	if ( ! $OPEN_JPEG_ROOT )
    	{
    	    print "Error: No path specified for the IJG Jpeg toolkit.";
    	    ShowHelpScreen() ;
    	    exit() ;
    	}
    	if ( ! $OPEN_EXIF_ROOT )
    	{
            print "Error: No path specified for the OpenExif toolkit.";
    	    ShowHelpScreen() ;
    	    exit() ;
    	}
    	if ( ! $OPEN_TIFF_ROOT )
    	{
    	    print "Error: No path specified for the OpenTiff toolkit.";
    	    ShowHelpScreen() ;
     	    exit() ;
    	}
    	if ( ! $OPEN_XERCES_ROOT )
    	{
    	    print "Error: No path specified for the OpenSource Xerces.";
    	    ShowHelpScreen() ;
    	    exit() ;
    	}
    	if( ! DoesAbsoluteDirExist($OPEN_JPEG_ROOT) )
	    {
            print "Error: Invalid path to IJG Jpeg toolkit: $OPEN_JPEG_ROOT";
    	    ShowHelpScreen() ;
    	    exit() ;
    	}
    	if ( ! DoesAbsoluteDirExist($OPEN_EXIF_ROOT) )
    	{
            print "Error: Invalid path to OpenExif toolkit: $OPEN_EXIF_ROOT";
       	    ShowHelpScreen() ;
    	    exit() ;
    	}
    	if ( ! DoesAbsoluteDirExist($OPEN_TIFF_ROOT) )
    	{
    	    print "Error: Invalid path to OpenTiff toolkit: $OPEN_TIFF_ROOT";
    	    ShowHelpScreen() ;
    	    exit() ;
    	}
        if ( ! DoesAbsoluteDirExist($OPEN_XERCES_ROOT) )
        {
	        print "Error: Invalid path to OpenSource Xerces: $OPEN_XERCES_ROOT";
	        ShowHelpScreen() ;
	        exit() ;
    	}
    }
    if ( $swioToolkits )
    {
    	if ( ! $SWIO_JPEG_ROOT )
    	{
   	        print "Error: No path specified for the SWIO Jpeg toolkit.";
            ShowHelpScreen() ;
       	    exit() ;
    	}
    	if ( ! $SWIO_EXIF_ROOT )
       	{
    	    print "Error: No path specified for the SWIO Kies toolkit.";
    	    ShowHelpScreen() ;
    	    exit() ;
    	}
    	if ( ! $SWIO_TIFF_ROOT )
    	{
    	    print "Error: No path specified for the SWIO EkTiff toolkit.";
	        ShowHelpScreen() ;
	        exit() ;
	    }
	    if ( ! $SWIO_XERCES_ROOT )
	    {
	        print "Error: No path specified for SWIO Xerces.";
	        ShowHelpScreen() ;
	        exit() ;
	    }
	    if ( ! $SWIO_IJL_ROOT )
	    {
	        print "Error: No path specified for SWIO IJL Toolkit.";
	        ShowHelpScreen() ;
	        exit() ;
	    }
	    if( ! DoesAbsoluteDirExist($SWIO_JPEG_ROOT) )
	    {
	        print "Error: Invalid path to SWIO Jpeg toolkit: $SWIO_JPEG_ROOT";
	        ShowHelpScreen() ;
	        exit() ;
	    }
	    if ( ! DoesAbsoluteDirExist($SWIO_EXIF_ROOT) )
	    {
	        print "Error: Invalid path to SWIO Kies toolkit: $SWIO_EXIF_ROOT";
	        ShowHelpScreen() ;
	        exit() ;
	    }
	    if ( ! DoesAbsoluteDirExist($SWIO_TIFF_ROOT) )
	    {
	        print "Error: Invalid path to SWIO EkTiff toolkit: $SWIO_TIFF_ROOT";
	        ShowHelpScreen() ;
	        exit() ;
	    }
	    if ( ! DoesAbsoluteDirExist($SWIO_XERCES_ROOT) )
	    {
	        print "Error: Invalid path to SWIO Xerces: $SWIO_XERCES_ROOT";
	        ShowHelpScreen() ;
	        exit() ;
	    }
	    if ( ! DoesAbsoluteDirExist($SWIO_IJL_ROOT) )
	    {
	        print "Error: Invalid path to SWIO IJL Toolkit: $SWIO_IJL_ROOT";
	        ShowHelpScreen() ;
	        exit() ;
	    }
    }
}

sub Main()
{
    system("cls");
    print "perl MakeWinZip-VC7.pl @ARGV\n";

    *STDERR = *STDOUT;

    # All the subroutines called directly from Main() assume the
    # current working directory is set to Pmt.
    # Get up to Pmt directory.
    chdir "../..";

    # Set the globals permanently.
    $PMT_DIR = cwd();
    $PMT_DISTRIBUTIONS_DIR = "$PMT_DIR/scripts/distributions";

    my $whichToolkits ;

    # Process the command line arguments
    ProcessCommandLineArgs($whichToolkits);    

    # Set up directory for building and testing. If it already exists,
    # delete it before creating the new one.
    SetupEmptyDir("$BUILD_AND_TEST_DIR") ;

    # Set up the directory for temporary DLL's and put it in the front of
    # the PATH
    SetupEmptyDir("$TEMP_DLL_PATH") ;
    $ENV{PATH} = "$TEMP_DLL_PATH;$ENV{PATH}" ;

    # Copy the complete source tree to the temporary directory we setup for
    # building and testing
    if ( $whichToolkits eq "open" or $whichToolkits eq "both" )
    {
        mkdir("$BUILD_AND_TEST_DIR\\open") == 1 
            or die("Error: could not create $BUILD_AND_TEST_DIR\\open");
        CopySourceTreeToDir("$PMT_TEMP_DIR_OPEN") ;
    }    
    if ( $whichToolkits eq "swio" or $whichToolkits eq "both" )
    {
        mkdir("$BUILD_AND_TEST_DIR\\swio") == 1 
            or die("Error: could not create $BUILD_AND_TEST_DIR\\swio");
        CopySourceTreeToDir("$PMT_TEMP_DIR_SWIO") ;
    }

    createBuildAndTestDistributions($whichToolkits) ;

    print "\n\nEntire script has run successfully...end\n\n\n";
}

Main();
