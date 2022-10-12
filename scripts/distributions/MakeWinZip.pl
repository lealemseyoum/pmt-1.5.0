#! c:\perl\bin\perl.exe -w


use strict;
use Cwd;         # For cwd()
use File::Path;  # For rmtree()


my $PMT_DIR;                            # Is setup in beginning of Main();
my $PMT_DISTRIBUTIONS_DIR;              # Is setup in beginning of Main();
my $W2K = 0;                            # May be modified in Main()
my $ROOT_TEMP_DIR = "\\ZipTemp";
my $PMT_TEMP_DIR = "$ROOT_TEMP_DIR\\Pmt";
my $BINARY_ZIP_FILENAME = "pmt-1_0_-bin-win32.zip";
my $SOURCE_ZIP_FILENAME = "pmt-1_0_-src-win32.zip";
my $BINARY_TEST_DIR = "$ROOT_TEMP_DIR\\UnzippedBinaryTest";
my $SOURCE_TEST_DIR = "$ROOT_TEMP_DIR\\UnzippedSourceTest";
my $ALL_PROJECTS = "9";     # Special value 1 greater than max number of projects

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


# This function will fixup a file with a .DSW extension.
# The file handle of the .DSW file is passed in as the first parameter.
# The file handle of a new fixed up file (that this function writes out) is
# passed in as the second parameter.
sub FixupDSWFile($$)
{
    my $BEGIN_STRING = "    begin source code control\n";
    my $END_STRING = "    end source code control\n";

    my($inFile, $newFile) = @_;

    while (<$inFile>)
    {
        if ($_ eq $BEGIN_STRING)
        {
            # Go into loop to skip over the lines dealing with source control.
            while (<$inFile>)
            {
                if ($_ eq $END_STRING)
                {
                    last;
                }

                # Else, do nothing - skip over line.
            }
        }
        else
        {
            print $newFile $_;
        }
    }
}


# This function will fixup a file with a .DSP extension.
# The file handle of the .DSP file is passed in as the first parameter.
# The file handle of a new fixed up file (that this function writes out) is
# passed in as the second parameter.
sub FixupDSPFile($$)
{
    my $FIRST_LINE = "PROP Scc_ProjName";
    my $SECOND_LINE = "PROP Scc_LocalPath";
    my $NO_ARGS_FIRST_LINE = "# PROP Scc_ProjName \"\"\n";
    my $NO_ARGS_SECOND_LINE = "# PROP Scc_LocalPath \"\"\n";
    my $found;

    my($inFile, $newFile) = @_;

    while (<$inFile>)
    {
        # If a source control line is found, write out one with no arguments.
        # Else, just write out the line as it is to preserve it.
        $found = grep { /$FIRST_LINE|$SECOND_LINE/ } $_;
        if ($found)
        {
            $found = grep { /$FIRST_LINE/ } $_;
            if ($found)
            {
                print $newFile $NO_ARGS_FIRST_LINE;
            }
            else
            {
                print $newFile $NO_ARGS_SECOND_LINE;
            }
        }
        else
        {
            print $newFile $_;
        }
    }
}


# This function will fixup a file by removing the lines that have to do with source control.
# A file name is passed in.
sub FixupFile($)
{
    my $TEMP_FILENAME = "temp.tmp";
    my($fileName) = @_;

    #print ("$fileName\n");

    open(INFILE, $fileName) or die("Error: could not open $fileName");
    open(NEWFILE, ">$TEMP_FILENAME") or die("Error: could not open $TEMP_FILENAME");

    my $result;
    $result = grep { /^.*\.dsw$/ } $fileName;

    if ($result)
    {
        FixupDSWFile(*INFILE, *NEWFILE);
        
    }
    else
    {
        FixupDSPFile(*INFILE, *NEWFILE);
    }

    close(INFILE) or die("Error: could not close $fileName");
    close(NEWFILE) or die("Error: could not close $TEMP_FILENAME");
    unlink($fileName) or die ("Error: could not delete $fileName");
    rename($TEMP_FILENAME, $fileName) or die("Error: could not rename file $TEMP_FILENAME");
}


# This function iterates over the appropriate files that need to get fixed up
# inside the given directory (directory is passed in).
# By "appropriate files" we mean thos files with an extension of DSW or DSP.
sub FixupFilesInDir($)
{
    my($dirName) = @_;
    my @files;
    my $file;

    # Get all files in directory that have a .dsw or .dsp extension.
    opendir(DIR, $dirName);
    @files = grep { /^.*\.(dsw|dsp)$/ } readdir(DIR);  # Find .DSW and .DSP files.
    closedir(DIR);
    
    # For all files found.
    foreach $file (@files)
    {
        FixupFile("$dirName/$file");
    }
}


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
    my ($workspaceName, $projectName, $executablePathName, $buildClean) = @_;
    my $BASELINE_FILE           = "GroundTrue.txt";
    my $BASELINE_FILE_TEMP      = "GroundTrue.tmp";
    my $BASELINE_FILE_RUN       = "GroundTrueRun.txt";
    my $BASELINE_FILE_RUN_TEMP  = "GroundTrueRun.tmp";

    my $WINDIFF_COMPARE_FILE = "BinaryDifferences.txt";

    my $forceBuildString;

    if ($buildClean)
    {
        $forceBuildString = " /REBUILD";
    }
    else
    {
        $forceBuildString = "";
    }

    system("msdev $workspaceName /MAKE \"$projectName\"$forceBuildString") == 0 or die("Error: could not build \"$projectName\"");
    system("$executablePathName > $BASELINE_FILE_RUN") == 0 or die("Error: unable to execute test program $executablePathName");

    CreateTempResultsFile($BASELINE_FILE, $BASELINE_FILE_TEMP);
    CreateTempResultsFile($BASELINE_FILE_RUN, $BASELINE_FILE_RUN_TEMP);

    # Windiff the results of each test program against a baseline text file that has the correct output.
    system("windiff $BASELINE_FILE_TEMP $BASELINE_FILE_RUN_TEMP -Sslrdx $WINDIFF_COMPARE_FILE") == 0 or die("Error: unable to test unzipped files");

    not IsTextInFile($WINDIFF_COMPARE_FILE, "different") or die("Error: Results of test file did not compare to baseline results");

    print("\nProject $projectName tested successfully...\n");
}


sub CopyFilesToTempDir()
{

    mkdir("$PMT_TEMP_DIR") == 1 or die("Error: could not create $PMT_TEMP_DIR");
    mkdir("$PMT_TEMP_DIR\\DefaultDefinitions") == 1 or die("Error: could not create $PMT_TEMP_DIR\\DefaultDefinitions");
    system("copy DefaultDefinitions\\PmtDefaultDefinitions.xsd $PMT_TEMP_DIR\\DefaultDefinitions\\PmtDefaultDefinitions.xsd") == 0 or die("Error: could not copy to temporary directory");
    system("copy ReleaseNote.txt $PMT_TEMP_DIR\\ReleaseNote.txt") == 0 or die("Error: could not copy to temporary directory");
    system("copy readme.html $PMT_TEMP_DIR\\readme.html") == 0 or die("Error: could not copy to temporary directory");
    system("copy license.html $PMT_TEMP_DIR\\license.html") == 0 or die("Error: could not copy to temporary directory");
    mkdir("$PMT_TEMP_DIR\\doc") == 1 or die("Error: could not create $PMT_TEMP_DIR\\doc");
    system("copy doc\\LicenseInfo.txt $PMT_TEMP_DIR\\doc\\LicenseInfo.txt") == 0 or die("Error: could not copy to temporary directory");
    system("copy doc\\PmtUserGuide.doc $PMT_TEMP_DIR\\doc\\PmtUserGuide.doc") == 0 or die("Error: could not copy to temporary directory");
    system("xcopy doc\\*.pdf $PMT_TEMP_DIR\\doc\\*.* /s") == 0 or die("Error: could not xcopy to temporary directory");
    system("xcopy projects\\*.ds? $PMT_TEMP_DIR\\projects\\*.* /s") == 0 or die("Error: could not xcopy to temporary directory");
    system("xcopy scripts\\*.* $PMT_TEMP_DIR\\scripts\\*.* /s /e") == 0 or die("Error: could not xcopy to temporary directory");
    system("xcopy src\\*.* $PMT_TEMP_DIR\\src\\*.* /s /e") == 0 or die("Error: could not xcopy to temporary directory");
    system("xcopy test\\AccessorTest\\*.* $PMT_TEMP_DIR\\test\\AccessorTest\\*.*") == 0 or die("Error: could not xcopy to temporary directory");
    system("xcopy test\\PmtInterpreterTest\\*.* $PMT_TEMP_DIR\\test\\PmtInterpreterTest\\*.*") == 0 or die("Error: could not xcopy to temporary directory");
    system("xcopy test\\Common\\*.* $PMT_TEMP_DIR\\test\\Common\\*.*") == 0 or die("Error: could not xcopy to temporary directory");
 
    # Get up to top level of view, just above the Pmt directory.
    chdir "..";
    
    # Also copy the src\*.h files to an inc directory, for the binary distribution.
    opendir(DIR, $PMT_TEMP_DIR . "\\src");
    my $file;
    while (defined($file = readdir(DIR)))
    {
        my $dirPath = "$PMT_TEMP_DIR\\src\\$file";
        if ((-d $dirPath) and ($file ne ".") and ($file ne "..") and ($file ne "CVS"))
        {
            system("xcopy $dirPath\\*.h $PMT_TEMP_DIR\\inc /I") == 0 or die("Error: could not xcopy to temporary directory");
        }
    }
    closedir(DIR);

    # Restore the current working directory to Pmt.
    chdir $PMT_DIR;

    print("\nCopied files to temporary directory successfully...\n");
}


sub CopyToolkits($)
{
    my ($toolkitsDir) = @_;

    # Get up to top level of view, just above the Pmt directory.
    chdir "..";

    # Copy entire Ijl
    system("xcopy $toolkitsDir\\Ijl\\*.* $PMT_TEMP_DIR\\Toolkits\\Ijl\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Ijl directory");

    # Copy Jpeg/inc and Jpeg/lib
    system("xcopy $toolkitsDir\\Jpeg\\inc\\*.* $PMT_TEMP_DIR\\Toolkits\\Jpeg\\inc\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Jpeg directory");
    system("xcopy $toolkitsDir\\Jpeg\\lib\\*.* $PMT_TEMP_DIR\\Toolkits\\Jpeg\\lib\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Jpeg directory");

    # Copy Kies/inc and certain Kies/lib files
    system("xcopy $toolkitsDir\\Kies\\inc\\*.* $PMT_TEMP_DIR\\Toolkits\\Kies\\inc\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Kies directory");
    system("xcopy $toolkitsDir\\Kies\\lib\\debug\\ekexifio140d.lib $PMT_TEMP_DIR\\Toolkits\\Kies\\lib\\debug\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Kies/lib directory");
    system("xcopy $toolkitsDir\\Kies\\lib\\debug\\ekfpixjpeg140d.lib $PMT_TEMP_DIR\\Toolkits\\Kies\\lib\\debug\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Kies/lib directory");
    system("xcopy $toolkitsDir\\Kies\\lib\\debug\\exifio.lib $PMT_TEMP_DIR\\Toolkits\\Kies\\lib\\debug\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Kies/lib directory");
    system("xcopy $toolkitsDir\\Kies\\lib\\release\\ekexifio140.lib $PMT_TEMP_DIR\\Toolkits\\Kies\\lib\\release\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Kies/lib directory");
    system("xcopy $toolkitsDir\\Kies\\lib\\release\\ekfpixjpeg140.lib $PMT_TEMP_DIR\\Toolkits\\Kies\\lib\\release\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Kies/lib directory");
    system("xcopy $toolkitsDir\\Kies\\lib\\release\\exifio.lib $PMT_TEMP_DIR\\Toolkits\\Kies\\lib\\release\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Kies/lib directory");

    # Get DLL files from bin directory to individual toolkit's directories
    system("xcopy $toolkitsDir\\..\\..\\bin\\ijl15.dll $PMT_TEMP_DIR\\Toolkits\\ijl\\lib\\*.* /s /e") == 0 or die("Error: could not xcopy $toolkitsDir\\..\\..\\bin\\ijl15.dll file");
    system("xcopy $toolkitsDir\\..\\..\\bin\\ekexifio140d.dll $PMT_TEMP_DIR\\Toolkits\\Kies\\lib\\debug\\*.* /s /e") == 0 or die("Error: could not xcopy $toolkitsDir\\..\\..\\bin\\ekexifio140d.dll file");
    system("xcopy $toolkitsDir\\..\\..\\bin\\ekexifio140.dll $PMT_TEMP_DIR\\Toolkits\\Kies\\lib\\release\\*.* /s /e") == 0 or die("Error: could not xcopy $toolkitsDir\\..\\..\\bin\\ekexifio140.dll file");
    system("xcopy $toolkitsDir\\..\\..\\bin\\ekfpixjpeg140d.dll $PMT_TEMP_DIR\\Toolkits\\Kies\\lib\\debug\\*.* /s /e") == 0 or die("Error: could not xcopy $toolkitsDir\\..\\..\\bin\\ekfpixjpeg140d.dll file");
    system("xcopy $toolkitsDir\\..\\..\\bin\\ekfpixjpeg140.dll $PMT_TEMP_DIR\\Toolkits\\Kies\\lib\\release\\*.* /s /e") == 0 or die("Error: could not xcopy $toolkitsDir\\..\\..\\bin\\ekfpixjpeg140.dll file");

    # Copy the Toolkit's DLL's to the directory specified in the PMT_DLL_PATH environment variable.
    # We don't do anything for Jpeg, since those are always static libraries.
    CopyDLLs("$PMT_TEMP_DIR\\Toolkits\\Ijl\\lib");
    CopyDLLs("$PMT_TEMP_DIR\\Toolkits\\Kies\\lib\\debug");
    CopyDLLs("$PMT_TEMP_DIR\\Toolkits\\Kies\\lib\\release");

    # Restore the current working directory to Pmt.
    chdir $PMT_DIR;

    print("\nToolkits directory copied to Pmt successfully...\n");
}

sub FixupExifTagsFile()   
{   
    my $EXIF_TAGS_FILE_PATH = "$ROOT_TEMP_DIR/Pmt/Toolkits/Kies/inc/ExifTags.h";   
    my $TEMP_FILENAME = "temp.tmp";   
    
    open(INFILE, $EXIF_TAGS_FILE_PATH) or die("Error: could not open $EXIF_TAGS_FILE_PATH");   
    open(NEWFILE, ">$TEMP_FILENAME") or die("Error: could not open $TEMP_FILENAME");   
    
    # First get rid of several #define's.   
    while (<INFILE>)   
    {   
        if ($_ =~ /Following Are Tags defined  for/)   
        {   
            # Go into loop to skip over the lines dealing with DeltaImage.   
            while (<INFILE>)   
            {   
                if ($_ =~ /19/)   
                {   
                    last;   
                }   
    
                # Else, do nothing - skip over line.   
            }   
        }   
        else   
        {   
            my $line = $_;   
            print NEWFILE $line;   
        }   
    }   
    
    close(INFILE) or die("Error: could not close $EXIF_TAGS_FILE_PATH");   
    close(NEWFILE) or die("Error: could not close $TEMP_FILENAME");   
    unlink($EXIF_TAGS_FILE_PATH) or die ("Error: could not delete $EXIF_TAGS_FILE_PATH");   
    rename($TEMP_FILENAME, $EXIF_TAGS_FILE_PATH) or die("Error: could not rename file $TEMP_FILENAME");   
    
    print("\nExifTags.h file fixed up successfully...\n");   
} 


sub FixupAllMakeFiles()
{
    chdir "$PMT_TEMP_DIR";

    my $VC_DIR = "Projects/Win32/Vc6.0";
    my $DYNAMIC_DIR = "$VC_DIR" . "/BuildAllDynamic";
    my $STATIC_DIR = "$VC_DIR" . "/BuildAllStatic";
    my $ACCESSOR_TEST_DIR = "test/AccessorTest";
    my $INTERPRETER_TEST_DIR = "test/PmtInterpreterTest";

    FixupFilesInDir($VC_DIR);
    FixupFilesInDir($DYNAMIC_DIR);
    FixupFilesInDir($STATIC_DIR);
    FixupFilesInDir($ACCESSOR_TEST_DIR);
    FixupFilesInDir($INTERPRETER_TEST_DIR);

    # Restore the current working directory to Pmt.
    chdir $PMT_DIR;

    print("\nAll project files fixed up successfully...\n");
}


# Copy the DLL's to the path specified in the PMT_DLL_PATH environment variable.
sub CopyDLLs($)
{
    my ($dllFromPath) = @_;
    my $dllToPath = $ENV{PMT_DLL_PATH};

    if ($W2K)
    {
        system("xcopy $dllFromPath\\*.dll \"$dllToPath\\*.dll\" /Y") == 0 or die("Error: could not xcopy the DLL's");
    }
    else
    {
        system("xcopy $dllFromPath\\*.dll \"$dllToPath\\*.dll\"") == 0 or die("Error: could not xcopy the DLL's");
    }
}


# Build all projects relative to given Pmt directory.
sub BuildAllProjects($$$$)
{
    my ($relativeDir, $buildClean, $projectToBuildAndTest, $binaryBuildsOnly) = @_;
    my $PROJECTS_DIR = "$relativeDir/Pmt/projects/Win32/Vc6.0";
    my $forceBuildString;

    if ($buildClean)
    {
        $forceBuildString = " /REBUILD";
    }
    else
    {
        $forceBuildString = "";
    }


    chdir $PROJECTS_DIR;

    # Relies on cwd being set, as it is just above.
    SetEnvironmentVariables($relativeDir);


    # Build all the static code and all the dynamic code, unless a specific build is requested.
	# Build only the Release (not Debug) projects if the $binaryBuildsOnly flag is true.

    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "1"))
    {
        if (! $binaryBuildsOnly)
        {
            system("msdev BuildAllStatic.dsw /MAKE \"_BuildAllStatic - Win32 Debug\"$forceBuildString") == 0 or die("Error: could not build static debug projects");
        }
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "2"))
    {
        # For some unknown reason, the build fails when built the first time. Just re-building it completes the build.
        system("msdev BuildAllStatic.dsw /MAKE \"_BuildAllStatic - Win32 Release\"$forceBuildString");
        system("msdev BuildAllStatic.dsw /MAKE \"_BuildAllStatic - Win32 Release\"$forceBuildString") == 0 or die("Error: could not build static release projects");
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "3"))
    {
        if (! $binaryBuildsOnly)
        {
            system("msdev BuildAllStatic.dsw /MAKE \"_BuildAllStatic - Win32 Unicode Debug\"$forceBuildString") == 0 or die("Error: could not build static unicode debug projects");
        }
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "4"))
    {
        system("msdev BuildAllStatic.dsw /MAKE \"_BuildAllStatic - Win32 Unicode Release\"$forceBuildString") == 0 or die("Error: could not build static unicode release projects");
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "5"))
    {
        if (! $binaryBuildsOnly)
        {
            system("msdev BuildAllDynamic.dsw /MAKE \"_BuildAllDynamic - Win32 Debug\"$forceBuildString") == 0 or die("Error: could not build dynamic debug projects");
        }
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "6"))
    {
        system("msdev BuildAllDynamic.dsw /MAKE \"_BuildAllDynamic - Win32 Release\"$forceBuildString") == 0 or die("Error: could not build dynamic release projects");
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "7"))
    {
        if (! $binaryBuildsOnly)
        {
            system("msdev BuildAllDynamic.dsw /MAKE \"_BuildAllDynamic - Win32 Unicode Debug\"$forceBuildString") == 0 or die("Error: could not build dynamic unicode debug projects");
        }
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "8"))
    {
        system("msdev BuildAllDynamic.dsw /MAKE \"_BuildAllDynamic - Win32 Unicode Release\"$forceBuildString") == 0 or die("Error: could not build dynamic unicode release projects");
    }


    # Copy the DLL's to the directory specified in the PMT_DLL_PATH environment variable.
    CopyDLLs("$relativeDir\\Pmt\\lib");


    # Restore to Pmt directory.
    chdir $PMT_DIR;

    print("\nAll projects built successfully...\n");
}


sub CreateBinaryZipFile()
{
    # For the binary distribution, we want to use different .dsw files that do not include
    # all of the other projects as source code, since we only want to link against the
    # already-built libraries of those projects instead.
    # This subroutine deals with temporarily renaming the .dsw files before zipping,
    # so we distribute these different .dsw's.
    # Also use a different set of .dsp's that do not have Debug build in them.

    ######################################
    # Deal with PmtInterpreterTest files.
    ######################################
    my $INTERPRETER_TEST_WORKSPACE_DIR                 = "$PMT_TEMP_DIR/test/PmtInterpreterTest";
    my $INTERPRETER_TEST_STATIC_WORKSPACE_FILE         = "$INTERPRETER_TEST_WORKSPACE_DIR/PmtInterpreterTest.dsw";
    my $INTERPRETER_TEST_STATIC_WORKSPACE_FILE_TEMP    = "$INTERPRETER_TEST_STATIC_WORKSPACE_FILE" . "saved";
    my $INTERPRETER_TEST_STATIC_WORKSPACE_FILE_BINARY  = "$INTERPRETER_TEST_WORKSPACE_DIR/PmtInterpreterTest_BinaryDistribution.dsw";
    my $INTERPRETER_TEST_DYNAMIC_WORKSPACE_FILE        = "$INTERPRETER_TEST_WORKSPACE_DIR/PmtInterpreterTesti.dsw";
    my $INTERPRETER_TEST_DYNAMIC_WORKSPACE_FILE_TEMP   = "$INTERPRETER_TEST_DYNAMIC_WORKSPACE_FILE" . "saved";
    my $INTERPRETER_TEST_DYNAMIC_WORKSPACE_FILE_BINARY = "$INTERPRETER_TEST_WORKSPACE_DIR/PmtInterpreterTesti_BinaryDistribution.dsw";
    #######################
    # Deal with the .dsp's
    #######################
    my $INTERPRETER_TEST_PROJECT_DIR                   = "$PMT_TEMP_DIR/test/PmtInterpreterTest";
    my $INTERPRETER_TEST_STATIC_PROJECT_FILE           = "$INTERPRETER_TEST_PROJECT_DIR/PmtInterpreterTest.dsp";
    my $INTERPRETER_TEST_STATIC_PROJECT_FILE_TEMP      = "$INTERPRETER_TEST_STATIC_PROJECT_FILE" . "saved";
    my $INTERPRETER_TEST_STATIC_PROJECT_FILE_BINARY    = "$INTERPRETER_TEST_PROJECT_DIR/PmtInterpreterTest_BinaryDistribution.dsp";
    my $INTERPRETER_TEST_DYNAMIC_PROJECT_FILE          = "$INTERPRETER_TEST_PROJECT_DIR/PmtInterpreterTesti.dsp";
    my $INTERPRETER_TEST_DYNAMIC_PROJECT_FILE_TEMP     = "$INTERPRETER_TEST_DYNAMIC_PROJECT_FILE" . "saved";
    my $INTERPRETER_TEST_DYNAMIC_PROJECT_FILE_BINARY   = "$INTERPRETER_TEST_PROJECT_DIR/PmtInterpreterTesti_BinaryDistribution.dsp";


    ####################################
    # Deal with the AccessorTest files.
    ####################################
    my $ACCESSOR_TEST_WORKSPACE_DIR                    = "$PMT_TEMP_DIR/test/AccessorTest";
    my $ACCESSOR_TEST_STATIC_WORKSPACE_FILE            = "$ACCESSOR_TEST_WORKSPACE_DIR/AccessorTest.dsw";
    my $ACCESSOR_TEST_STATIC_WORKSPACE_FILE_TEMP       = "$ACCESSOR_TEST_STATIC_WORKSPACE_FILE" . "saved";
    my $ACCESSOR_TEST_STATIC_WORKSPACE_FILE_BINARY     = "$ACCESSOR_TEST_WORKSPACE_DIR/AccessorTest_BinaryDistribution.dsw";
    my $ACCESSOR_TEST_DYNAMIC_WORKSPACE_FILE           = "$ACCESSOR_TEST_WORKSPACE_DIR/AccessorTesti.dsw";
    my $ACCESSOR_TEST_DYNAMIC_WORKSPACE_FILE_TEMP      = "$ACCESSOR_TEST_DYNAMIC_WORKSPACE_FILE" . "saved";
    my $ACCESSOR_TEST_DYNAMIC_WORKSPACE_FILE_BINARY    = "$ACCESSOR_TEST_WORKSPACE_DIR/AccessorTesti_BinaryDistribution.dsw";
    #######################
    # Deal with the .dsp's
    #######################
    my $ACCESSOR_TEST_PROJECT_DIR                      = "$PMT_TEMP_DIR/test/AccessorTest";
    my $ACCESSOR_TEST_STATIC_PROJECT_FILE              = "$ACCESSOR_TEST_PROJECT_DIR/AccessorTest.dsp";
    my $ACCESSOR_TEST_STATIC_PROJECT_FILE_TEMP         = "$ACCESSOR_TEST_STATIC_PROJECT_FILE" . "saved";
    my $ACCESSOR_TEST_STATIC_PROJECT_FILE_BINARY       = "$ACCESSOR_TEST_PROJECT_DIR/AccessorTest_BinaryDistribution.dsp";
    my $ACCESSOR_TEST_DYNAMIC_PROJECT_FILE             = "$ACCESSOR_TEST_PROJECT_DIR/AccessorTesti.dsp";
    my $ACCESSOR_TEST_DYNAMIC_PROJECT_FILE_TEMP        = "$ACCESSOR_TEST_DYNAMIC_PROJECT_FILE" . "saved";
    my $ACCESSOR_TEST_DYNAMIC_PROJECT_FILE_BINARY      = "$ACCESSOR_TEST_PROJECT_DIR/AccessorTesti_BinaryDistribution.dsp";


    # Rename the original .dsw's and .dsp's to save them.
    rename($INTERPRETER_TEST_STATIC_WORKSPACE_FILE, $INTERPRETER_TEST_STATIC_WORKSPACE_FILE_TEMP) or die("Error: unable to rename to temporary file");
    rename($INTERPRETER_TEST_DYNAMIC_WORKSPACE_FILE, $INTERPRETER_TEST_DYNAMIC_WORKSPACE_FILE_TEMP) or die("Error: unable to rename to temporary file");
    rename($ACCESSOR_TEST_STATIC_WORKSPACE_FILE, $ACCESSOR_TEST_STATIC_WORKSPACE_FILE_TEMP) or die("Error: unable to rename to temporary file");
    rename($ACCESSOR_TEST_DYNAMIC_WORKSPACE_FILE, $ACCESSOR_TEST_DYNAMIC_WORKSPACE_FILE_TEMP) or die("Error: unable to rename to temporary file");
    rename($INTERPRETER_TEST_STATIC_PROJECT_FILE, $INTERPRETER_TEST_STATIC_PROJECT_FILE_TEMP) or die("Error: unable to rename to temporary file");
    rename($INTERPRETER_TEST_DYNAMIC_PROJECT_FILE, $INTERPRETER_TEST_DYNAMIC_PROJECT_FILE_TEMP) or die("Error: unable to rename to temporary file");
    rename($ACCESSOR_TEST_STATIC_PROJECT_FILE, $ACCESSOR_TEST_STATIC_PROJECT_FILE_TEMP) or die("Error: unable to rename to temporary file");
    rename($ACCESSOR_TEST_DYNAMIC_PROJECT_FILE, $ACCESSOR_TEST_DYNAMIC_PROJECT_FILE_TEMP) or die("Error: unable to rename to temporary file");

    # Rename the binary distribution .dsw's and .dsp's to the regular names (to be distributed in the zip file).
    rename($INTERPRETER_TEST_STATIC_WORKSPACE_FILE_BINARY, $INTERPRETER_TEST_STATIC_WORKSPACE_FILE) or die("Error: unable to rename to temporary file");
    rename($INTERPRETER_TEST_DYNAMIC_WORKSPACE_FILE_BINARY, $INTERPRETER_TEST_DYNAMIC_WORKSPACE_FILE) or die("Error: unable to rename to temporary file");
    rename($ACCESSOR_TEST_STATIC_WORKSPACE_FILE_BINARY, $ACCESSOR_TEST_STATIC_WORKSPACE_FILE) or die("Error: unable to rename to temporary file");
    rename($ACCESSOR_TEST_DYNAMIC_WORKSPACE_FILE_BINARY, $ACCESSOR_TEST_DYNAMIC_WORKSPACE_FILE) or die("Error: unable to rename to temporary file");
    rename($INTERPRETER_TEST_STATIC_PROJECT_FILE_BINARY, $INTERPRETER_TEST_STATIC_PROJECT_FILE) or die("Error: unable to rename to temporary file");
    rename($INTERPRETER_TEST_DYNAMIC_PROJECT_FILE_BINARY, $INTERPRETER_TEST_DYNAMIC_PROJECT_FILE) or die("Error: unable to rename to temporary file");
    rename($ACCESSOR_TEST_STATIC_PROJECT_FILE_BINARY, $ACCESSOR_TEST_STATIC_PROJECT_FILE) or die("Error: unable to rename to temporary file");
    rename($ACCESSOR_TEST_DYNAMIC_PROJECT_FILE_BINARY, $ACCESSOR_TEST_DYNAMIC_PROJECT_FILE) or die("Error: unable to rename to temporary file");

    # Now make the zip file
    my $zipFilePath = "$PMT_DISTRIBUTIONS_DIR/$BINARY_ZIP_FILENAME";
    my $binaryOrSourceString = "Binary";

    CreateZipFileCommon($zipFilePath, $binaryOrSourceString);


    # Now rename things back to original

    # Rename the regular names back to the binary ones
    rename($INTERPRETER_TEST_STATIC_WORKSPACE_FILE, $INTERPRETER_TEST_STATIC_WORKSPACE_FILE_BINARY) or die("Error: unable to rename to temporary file");
    rename($INTERPRETER_TEST_DYNAMIC_WORKSPACE_FILE, $INTERPRETER_TEST_DYNAMIC_WORKSPACE_FILE_BINARY) or die("Error: unable to rename to temporary file");
    rename($ACCESSOR_TEST_STATIC_WORKSPACE_FILE, $ACCESSOR_TEST_STATIC_WORKSPACE_FILE_BINARY) or die("Error: unable to rename to temporary file");
    rename($ACCESSOR_TEST_DYNAMIC_WORKSPACE_FILE, $ACCESSOR_TEST_DYNAMIC_WORKSPACE_FILE_BINARY) or die("Error: unable to rename to temporary file");
    rename($INTERPRETER_TEST_STATIC_PROJECT_FILE, $INTERPRETER_TEST_STATIC_PROJECT_FILE_BINARY) or die("Error: unable to rename to temporary file");
    rename($INTERPRETER_TEST_DYNAMIC_PROJECT_FILE, $INTERPRETER_TEST_DYNAMIC_PROJECT_FILE_BINARY) or die("Error: unable to rename to temporary file");
    rename($ACCESSOR_TEST_STATIC_PROJECT_FILE, $ACCESSOR_TEST_STATIC_PROJECT_FILE_BINARY) or die("Error: unable to rename to temporary file");
    rename($ACCESSOR_TEST_DYNAMIC_PROJECT_FILE, $ACCESSOR_TEST_DYNAMIC_PROJECT_FILE_BINARY) or die("Error: unable to rename to temporary file");

    # Rename the temp names back to the regular ones
    rename($INTERPRETER_TEST_STATIC_WORKSPACE_FILE_TEMP, $INTERPRETER_TEST_STATIC_WORKSPACE_FILE) or die("Error: unable to rename to temporary file");
    rename($INTERPRETER_TEST_DYNAMIC_WORKSPACE_FILE_TEMP, $INTERPRETER_TEST_DYNAMIC_WORKSPACE_FILE) or die("Error: unable to rename to temporary file");
    rename($ACCESSOR_TEST_STATIC_WORKSPACE_FILE_TEMP, $ACCESSOR_TEST_STATIC_WORKSPACE_FILE) or die("Error: unable to rename to temporary file");
    rename($ACCESSOR_TEST_DYNAMIC_WORKSPACE_FILE_TEMP, $ACCESSOR_TEST_DYNAMIC_WORKSPACE_FILE) or die("Error: unable to rename to temporary file");
    rename($INTERPRETER_TEST_STATIC_PROJECT_FILE_TEMP, $INTERPRETER_TEST_STATIC_PROJECT_FILE) or die("Error: unable to rename to temporary file");
    rename($INTERPRETER_TEST_DYNAMIC_PROJECT_FILE_TEMP, $INTERPRETER_TEST_DYNAMIC_PROJECT_FILE) or die("Error: unable to rename to temporary file");
    rename($ACCESSOR_TEST_STATIC_PROJECT_FILE_TEMP, $ACCESSOR_TEST_STATIC_PROJECT_FILE) or die("Error: unable to rename to temporary file");
    rename($ACCESSOR_TEST_DYNAMIC_PROJECT_FILE_TEMP, $ACCESSOR_TEST_DYNAMIC_PROJECT_FILE) or die("Error: unable to rename to temporary file");
}


sub CreateSourceZipFile()
{
    my $zipFilePath = "$PMT_DISTRIBUTIONS_DIR/$SOURCE_ZIP_FILENAME";
    my $binaryOrSourceString = "Source";

    CreateZipFileCommon($zipFilePath, $binaryOrSourceString);
}


sub CreateZipFileCommon($$)
{
    my ($zipFilePath, $binaryOrSourceString) = @_;

    # Change to temporary directory, just above the Pmt directory.
    chdir $ROOT_TEMP_DIR;

    # Delete the old file first, if it exists.
    if (DoesAbsoluteFileExist("$zipFilePath"))
    {
        unlink("$zipFilePath") or die("Error: could not delete $zipFilePath");
    }

    system("wzzip -x@" . "$PMT_DISTRIBUTIONS_DIR/WinZip" . $binaryOrSourceString .
           "FilesExclude.txt -rP $zipFilePath @" .
           "$PMT_DISTRIBUTIONS_DIR/WinZip" . $binaryOrSourceString . "Files.txt") == 0 or die("Error: could not create $zipFilePath ZIP file");

    # Restore to Pmt directory.
    chdir $PMT_DIR;

    print("\nZip file created successfully...\n");
}


sub UnzipZipFile($$)
{
    my ($binaryOrSourceTestDir, $zipFilePath) = @_;

    # First delete all of /Pmt, if it exists.
    if (DoesAbsoluteDirExist("$binaryOrSourceTestDir"))
    {
        rmtree("$binaryOrSourceTestDir") or die("Error: could not delete $binaryOrSourceTestDir");
    }

    # Unzip entire zip file into /Pmt directory
    system("wzunzip -d $zipFilePath $binaryOrSourceTestDir") == 0 or die("Error: unable to unzip $zipFilePath");

    print("\nFile unzipped successfully...\n");
}


sub TestUnzippedFiles($$$$)
{
    my ($buildClean, $binaryOrSourceTestDir, $projectToBuildAndTest, $binaryBuildsOnly) = @_;

    # Always delete the test.xml file prior to running test, so the results are consistent when comparing
    # against the baseline.

    chdir "$binaryOrSourceTestDir/Pmt/Test/PmtInterpreterTest";

    # Relies on cwd being set, as it is just above.
    SetEnvironmentVariables($binaryOrSourceTestDir);


    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "1"))
    {
        if (! $binaryBuildsOnly)
        {
            unlink("test.xml");
            TestProject("PmtInterpreterTest.dsw", "PmtInterpreterTest - Win32 Debug", 'debug\PmtInterpreterTest.exe', $buildClean);
        }
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "2"))
    {
        unlink("test.xml");
        TestProject("PmtInterpreterTest.dsw", "PmtInterpreterTest - Win32 Release", 'release\PmtInterpreterTest.exe', $buildClean);
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "3"))
    {
        if (! $binaryBuildsOnly)
        {
            unlink("test.xml");
            TestProject("PmtInterpreterTest.dsw", "PmtInterpreterTest - Win32 Unicode Debug", 'debugu\PmtInterpreterTest.exe', $buildClean);
        }
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "4"))
    {
        unlink("test.xml");
        TestProject("PmtInterpreterTest.dsw", "PmtInterpreterTest - Win32 Unicode Release", 'releaseu\PmtInterpreterTest.exe', $buildClean);
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "5"))
    {
        if (! $binaryBuildsOnly)
        {
            unlink("test.xml");
            TestProject("PmtInterpreterTesti.dsw", "PmtInterpreterTesti - Win32 Debug", 'debugi\PmtInterpreterTesti.exe', $buildClean);
        }
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "6"))
    {
        unlink("test.xml");
        TestProject("PmtInterpreterTesti.dsw", "PmtInterpreterTesti - Win32 Release", 'releasei\PmtInterpreterTesti.exe', $buildClean);
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "7"))
    {
        if (! $binaryBuildsOnly)
        {
            unlink("test.xml");
            TestProject("PmtInterpreterTesti.dsw", "PmtInterpreterTesti - Win32 Unicode Debug", 'debugiu\PmtInterpreterTesti.exe', $buildClean);
        }
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "8"))
    {
        unlink("test.xml");
        TestProject("PmtInterpreterTesti.dsw", "PmtInterpreterTesti - Win32 Unicode Release", 'releaseiu\PmtInterpreterTesti.exe', $buildClean);
    }

    chdir "$binaryOrSourceTestDir/Pmt/Test/AccessorTest";

    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "1"))
    {
        if (! $binaryBuildsOnly)
        {
            TestProject("AccessorTest.dsw", "AccessorTest - Win32 Debug", 'debug\AccessorTest.exe', $buildClean);
        }
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "2"))
    {
        TestProject("AccessorTest.dsw", "AccessorTest - Win32 Release", 'release\AccessorTest.exe', $buildClean);
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "3"))
    {
        if (! $binaryBuildsOnly)
        {
            TestProject("AccessorTest.dsw", "AccessorTest - Win32 Unicode Debug", 'debugu\AccessorTest.exe', $buildClean);
        }
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "4"))
    {
        TestProject("AccessorTest.dsw", "AccessorTest - Win32 Unicode Release", 'releaseu\AccessorTest.exe', $buildClean);
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "5"))
    {
        if (! $binaryBuildsOnly)
        {
            TestProject("AccessorTesti.dsw", "AccessorTesti - Win32 Debug", 'debugi\AccessorTesti.exe', $buildClean);
        }
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "6"))
    {
        TestProject("AccessorTesti.dsw", "AccessorTesti - Win32 Release", 'releasei\AccessorTesti.exe', $buildClean);
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "7"))
    {
        if (! $binaryBuildsOnly)
        {
            TestProject("AccessorTesti.dsw", "AccessorTesti - Win32 Unicode Debug", 'debugiu\AccessorTesti.exe', $buildClean);
        }
    }
    if (($projectToBuildAndTest eq $ALL_PROJECTS) or ($projectToBuildAndTest eq "8"))
    {
        TestProject("AccessorTesti.dsw", "AccessorTesti - Win32 Unicode Release", 'releaseiu\AccessorTesti.exe', $buildClean);
    }

    # Restore to Pmt directory.
    chdir $PMT_DIR;

    print("\nTested all unzipped files for $binaryOrSourceTestDir directory successfully...\n");
}


sub ShowHelpScreen($)
{
    my ($showFewerLines) = @_;

    print "\n";
    print "Usage: perl MakeWinZip.pl [-binary | -source | -both | -h] [-nocopy -nobuild\n";
    print "              -nozip -notest -testNumXX -buildclean -W2K] toolkits_dir\n";
    print "-binary          -- Makes binary distribution zip file\n";
    print "-source          -- Makes source distribution zip file\n";
    print "-both            -- Makes binary and source distribution zip files\n";
    print "-h               -- Shows this help screen (following options ignored)\n";
    print "-nocopy          -- Skips copying files to temporary Pmt directory\n";
    print "-nobuild         -- Skips building the \"all\" projects - be aware that DLL's\n";
    print "                    are not copied to the PMT_DLL_PATH directory!\n";
    print "-nozip           -- Skips creating zip file (and unzipping to test)\n";
    print "-notest          -- Skips testing the zip file after it's created\n";
    print "-testNumXX       -- Performs testing of TestProject specified in XX\n";
    print "-buildclean      -- Forces clean builds of projects\n";
    print "-W2K             -- Specify this option if running on a Windows 2000 computer\n";
    print "toolkits_dir     -- Path to directory where Toolkits code and libraries reside\n\n";
    print "Example:  perl MakeWinZip.pl -both c:\\ClearCaseViews\\rupe_nightly\\Fw\\Toolkits\n";
    print "   This would create the zip files for both binary and source distributions,\n";
    print "   and test them. The resultant zips file would be placed in the\n";
    print "   $PMT_DISTRIBUTIONS_DIR directory.";

    unless ($showFewerLines) { print "\n"; }
}


sub ProcessCommandLineArgs($$$$$$$$$)
{
    # "$#ARGV" is 1 less than the actual number of command line arguments passed in.

    # If no or one arguments on the command line, or a -h is passed in, show the help screen.
    if (($#ARGV == -1) or ($#ARGV == 0) or ($ARGV[0] eq "-h"))
    {
        ShowHelpScreen(0);
        exit;
    }
    elsif (($ARGV[0] eq "-binary") or ($ARGV[0] eq "-source") or ($ARGV[0] eq "-both"))
    {
        $_[0] = $ARGV[0];
    }
    else
    {
        print "\nERROR: INVALID ARGUMENTS!\n";
        ShowHelpScreen(1);
        exit;
    }


    # The -nocopy, -nobuild, -buildclean, -nozip, and -notest arguments can be
    # passed in on the command line in any order, as long as they
    # come after the -binary or -source argument.

    # Default to building all the projects, before possibly building only the
    # projects specified in the -testNumXX argument.
    $_[6] = $ALL_PROJECTS;

    for (my $argNum = 1; $#ARGV - 1 >= $argNum; $argNum++)
    {
        if ($ARGV[$argNum] eq "-nocopy") { $_[1] = 1; }
        elsif ($ARGV[$argNum] eq "-nobuild") { $_[2] = 1; }
        elsif ($ARGV[$argNum] eq "-buildclean") { $_[3] = 1; }
        elsif ($ARGV[$argNum] eq "-nozip") { $_[4] = 1; }
        elsif ($ARGV[$argNum] eq "-notest") { $_[5] = 1; }
        elsif (substr($ARGV[$argNum], 0, 8) eq "-testNum")
        {
            $_[6] = substr($ARGV[$argNum], 8);
            if ($_[6] < 1 or $_[6] > 8)
            {
                print "\nERROR: The -testNum argument must be 1 to 8!\n";
                ShowHelpScreen(1);
                exit;
            }
        }
        elsif ($ARGV[$argNum] eq "-W2K") { $_[7] = 1; }
        else
        {
            print "\nERROR: INVALID ARGUMENTS!\n";
            ShowHelpScreen(1);
            exit;
        }
    }

    $_[8] = $ARGV[$#ARGV];
}


sub CreateAndTestBinaryZipFile($$$$$)
{
    my ($noBuild, $buildClean, $noZip, $noTest, $projectToBuildAndTest) = @_;

    # Build all projects in the temporary Pmt directory, so the libs can
    # get included in the zip file.
    unless ($noBuild)
    {
        BuildAllProjects($ROOT_TEMP_DIR, $buildClean, $projectToBuildAndTest, 1);
    }

    unless ($noZip)
    {
        CreateBinaryZipFile();

        UnzipZipFile($BINARY_TEST_DIR, "$PMT_DISTRIBUTIONS_DIR/$BINARY_ZIP_FILENAME");
    }

    # Will build and test the test programs only. The binary distribution .dsw's are
    # used, which do not include the source code projects. I.e., the libs are linked
    # against. The libs are not built here. (The libs were previously built in the
    # Pmt directory when BuildAllProjects() was called above. Those libs were zipped
    # up and unzipped into the UnzippedBinaryTest/lib directory.)

    unless ($noTest) { TestUnzippedFiles($buildClean, $BINARY_TEST_DIR, $projectToBuildAndTest, 1); }
}


sub CreateAndTestSourceZipFile($$$$$)
{
    my ($noBuild, $buildClean, $noZip, $noTest, $projectToBuildAndTest) = @_;

    unless ($noZip)
    {
        CreateSourceZipFile();

        UnzipZipFile($SOURCE_TEST_DIR, "$PMT_DISTRIBUTIONS_DIR/$SOURCE_ZIP_FILENAME");
    }

    # This ensures the libaries are built so the DLL's get copied over to the
    # directory specified in the PMT_DLL_PATH environment variable. This is so when
    # the code is tested below, the DLL's have been properly placed.
    unless ($noBuild)
    {
        BuildAllProjects($SOURCE_TEST_DIR, $buildClean, $projectToBuildAndTest, 0);
    }

    unless ($noTest) { TestUnzippedFiles($buildClean, $SOURCE_TEST_DIR, $projectToBuildAndTest, 0); }
}


sub SetEnvironmentVariables($)
{
    my ($path) = @_;
    my $curDirLetter = cwd();
    $curDirLetter = substr($curDirLetter, 0, 1);

    # Do not need the first backslash after the "c:", since $path is preceded by one.
    $ENV{IJLROOT} = "$curDirLetter:$path\\Pmt\\Toolkits\\Ijl";
    $ENV{JPEGROOT} = "$curDirLetter:$path\\Pmt\\Toolkits\\Jpeg";
    $ENV{KIESEXIFROOT} = "$curDirLetter:$path\\Pmt\\Toolkits\\Kies";
}


sub CheckEnvironmentVariables()
{
    # Enforce that the following environment variables are set before continuing.

    if (not defined $ENV{PMT_DLL_PATH})
    {
        print "\nYou must set up the PMT_DLL_PATH environment variable before running this Perl\n";
        print "script. Set PMT_DLL_PATH to a directory location where you want the dynamic\n";
        print "libraries built by this script to be copied to, and ensure this directory is\n";
        print "specified in your PATH environment variable too.\n\n";
        return 0;
    }

    if (not defined $ENV{PMT_XERCES_INC_PATH})
    {
        print "\nYou must set up the PMT_XERCES_INC_PATH environment variable before running\nthis Perl script!\n";
        return 0;
    }

    if (not defined $ENV{PMT_XERCES_LIB_PATH})
    {
        print "\nYou must set up the PMT_XERCES_LIB_PATH environment variable before running\nthis Perl script!\n";
        return 0;
    }

    # Now set these environment variables so the paths work relative to the 

    if (not defined $ENV{TIFFROOT})
    {
        print "\nYou must set up the TIFFROOT environment variable before running\nthis Perl script!\n";
        return 0;
    }

    return 1;
}


sub Main()
{
    if (not CheckEnvironmentVariables())
    {
        return;
    }

    system("cls");
    print "perl MakeWinZip @ARGV\n";

    # All the subroutines called directly from Main() assume the
    # current working directory is set to Pmt.
    # Get up to Pmt directory.
    chdir "../..";

    # Set the globals permanently.
    $PMT_DIR = cwd();
    $PMT_DISTRIBUTIONS_DIR = "$PMT_DIR/scripts/distributions";

    my $binarySourceBoth = 0;
    my $noBuild = 0;
    my $buildClean = 0;
    my $noTest = 0;
    my $noCopy = 0;
    my $noZip = 0;
    my $toolkitsDir = 0;
    my $projectToBuildAndTest = 0;
    my $w2k = 0;


    ProcessCommandLineArgs($binarySourceBoth, $noCopy, $noBuild, $buildClean, $noZip, $noTest,
        $projectToBuildAndTest, $w2k, $toolkitsDir);

    if ($w2k)
    {
        $W2K = 1;
    }


=pod
#!!!
    print "Binary/source/both: $binarySourceBoth\n";
    print "No build: $noBuild\n";
    print "No clean build: $buildClean\n";
    print "No test: $noTest\n";
    print "No copy: $noCopy\n";
    print "No zip: $noZip\n";
    print "Build & Test project #: $projectToBuildAndTest\n";
    print "Toolkits dir: $toolkitsDir\n";
    print "W2K: $W2K\n";
    return;
=cut


    # Copy files and perform necessary file fixups.
    unless ($noCopy)
    {
        if (DoesAbsoluteDirExist("$ROOT_TEMP_DIR"))
        {
            # There's a flaky case in Windows that can occur. If the focus is on the
            # temp directory in Windows Explorer, the directory may get deleted but
            # then may not be able to be created. Try to catch this error and issue
            # a specific message about it.
            rmtree("$ROOT_TEMP_DIR") != 0 or die("\n\nError: could not delete $ROOT_TEMP_DIR. " .
                                                 "Make sure you do not have the focus on this " .
                                                 "directory in Windows Explorer when " .
                                                 "running this script!");
        }

        mkdir("$ROOT_TEMP_DIR") == 1 or die("Error: could not create $ROOT_TEMP_DIR");

        CopyFilesToTempDir();

        CopyToolkits($toolkitsDir);

		FixupExifTagsFile();

        FixupAllMakeFiles();
    }


    # Create and test zip files
    if ($binarySourceBoth eq "-binary")
    {
        CreateAndTestBinaryZipFile($noBuild, $buildClean, $noZip, $noTest, $projectToBuildAndTest);
    }
    elsif ($binarySourceBoth eq "-source")
    {
        CreateAndTestSourceZipFile($noBuild, $buildClean, $noZip, $noTest, $projectToBuildAndTest);
    }
    elsif ($binarySourceBoth eq "-both")
    {
        CreateAndTestBinaryZipFile($noBuild, $buildClean, $noZip, $noTest, $projectToBuildAndTest);
        CreateAndTestSourceZipFile($noBuild, $buildClean, $noZip, $noTest, $projectToBuildAndTest);
    }


    print "\n\nEntire script has run successfully...end\n\n\n";
}


Main();
