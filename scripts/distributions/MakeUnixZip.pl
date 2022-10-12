#! c:\perl\bin\perl.exe -w


use strict;
use Cwd;         # For cwd()
use File::Path;  # For rmtree()

 
my $PMT_DIR;                 # Is setup in beginning of Main();
my $PMT_DISTRIBUTIONS_DIR;   # Is setup in beginning of Main();
my $ROOT_TEMP_DIR = "$ENV{HOME}/UnixZipTemp";
my $PMT_TEMP_DIR = "$ROOT_TEMP_DIR/pmt";
my $DIST_FILENAME = "pmt-*";
my $PMT_TEMP_DIST = "pmt-temp-dist";
my $PMT_ACC_TEST_DIR = "test/AccessorTest";
my $PMT_INT_TEST_DIR = "test/PmtInterpreterTest";
my $PMT_VISITOR_TEST_DIR = "test/VisitorTest";
my $PMT_ACC_TEST_EXE = "accrTest";
my $PMT_INT_TEST_EXE = "interpTest";
my $PMT_VISITOR_TEST_EXE = "visitorTest";
my $EKTIFF_ROOT;
my $OPENTIFF_ROOT;


sub DoesRelativeDirExist($)
{
    my ($directory) = @_;
    my $curDir = cwd();

    return -d "$curDir/$directory";
}


sub DoesRelativeFileExist($)
{
    my ($file) = @_;
    my $curDir = cwd();

    return -e "$curDir/$file";
}


sub DoesAbsoluteDirExist($)
{
    my ($directory) = @_;

    return -d "$directory";
}


sub DoesAbsoluteFileExist($)
{
    my ($file) = @_;

    return -e "$file";
}


sub CreateTempResultsFile($$)
{
    my ($resultsFilePath, $tempResultsFilePath) = @_;

    open(INFILE, $resultsFilePath)
        or die("Error: could not create temporary results file - Could not open $resultsFilePath");
    open(OUTFILE, ">$tempResultsFilePath")
        or die("Error: could not create temporary results file - Could not create $tempResultsFilePath");

    my @lines = <INFILE> ;
    # Leave first (Time and Date) and last (timing) lines out
    for ( my $i = 1; $i < $#lines; $i++ )
    {
        print OUTFILE $lines[$i] ;
    }

    close(INFILE) or die("Error: could not create temporary results file");
    close(OUTFILE) or die("Error: could not create temporary results file");
}

# This function runs the accessor and interpreter test and compares results to the GroundTrue files
sub RunTests($$)
{
    my ($rootdir,$toolkits) = @_;

    my $BASELINE_FILE           = "GroundTrue.txt";
    my $BASELINE_FILE_TEMP      = "GroundTrue.tmp";
    my $BASELINE_FILE_RUN       = "GroundTrueRun.txt";
    my $BASELINE_FILE_THREAD_RUN = "GroundTrueThreadRun.txt";
    my $BASELINE_FILE_RUN_TEMP  = "GroundTrueRun.tmp";

    # Pmt interpreter test
    chdir "$rootdir/$PMT_INT_TEST_DIR" or die("Error: could not cd to $rootdir/$PMT_INT_TEST_DIR");

    system("$PMT_INT_TEST_EXE > $BASELINE_FILE_RUN") == 0
        or die("Error: unable to execute test program $PMT_INT_TEST_EXE");

    CreateTempResultsFile($BASELINE_FILE, $BASELINE_FILE_TEMP);
    CreateTempResultsFile($BASELINE_FILE_RUN, $BASELINE_FILE_RUN_TEMP);

    # PMT Interpreter threading test
    system("$PMT_INT_TEST_EXE -t 5 -i 10 > $BASELINE_FILE_THREAD_RUN") == 0
        or die("Error: unable to execute thread test program $PMT_INT_TEST_EXE");

    # Cmp the results of each test program against a baseline text file that has the correct output.
    system("cmp $BASELINE_FILE_TEMP $BASELINE_FILE_RUN_TEMP") == 0
        or die("Error: Results of test file did not compare to baseline results");

    print("\nPmtInterpreter tested successfully...\n");

    # Pmt accessor test
    chdir "$rootdir/$PMT_ACC_TEST_DIR" or die("Error: could not cd to $rootdir/$PMT_ACC_TEST_DIR");

    if ( $toolkits eq "open" )
    {
        $BASELINE_FILE = "GroundTrueOpenExif.txt" ;
    }

    system("$PMT_ACC_TEST_EXE > $BASELINE_FILE_RUN") == 0
        or die("Error: unable to execute test program $PMT_ACC_TEST_EXE");

    if ( $toolkits eq "open" )
    {
        # PMT Accessor threading test
        system("$PMT_ACC_TEST_EXE -t 5 -i 10 > $BASELINE_FILE_THREAD_RUN") == 0
            or die("Error: unable to execute thread test program $PMT_ACC_TEST_EXE");
    }

    CreateTempResultsFile($BASELINE_FILE, $BASELINE_FILE_TEMP);
    CreateTempResultsFile($BASELINE_FILE_RUN, $BASELINE_FILE_RUN_TEMP);

    system("diff -a --ignore-space-change $BASELINE_FILE_TEMP $BASELINE_FILE_RUN_TEMP") == 0
    	or die("Error: Results of test file did not compare to baseline results") ;

    print("\nPmtAccessor tested successfully...\n");

    # Pmt Visitor test
    chdir "$rootdir/$PMT_VISITOR_TEST_DIR" or die("Error: could not cd to $rootdir/$PMT_VISITOR_TEST_DIR");

    system("$PMT_VISITOR_TEST_EXE > $BASELINE_FILE_RUN") == 0
        or die("Error: unable to execute test program $PMT_VISITOR_TEST_EXE");

    CreateTempResultsFile($BASELINE_FILE, $BASELINE_FILE_TEMP);
    CreateTempResultsFile($BASELINE_FILE_RUN, $BASELINE_FILE_RUN_TEMP);

    # Cmp the results of each test program against a baseline text file that has the correct output.
    system("cmp $BASELINE_FILE_TEMP $BASELINE_FILE_RUN_TEMP") == 0
        or die("Error: Results of test file did not compare to baseline results");

    print("\nVisitor tested successfully...\n");


    chdir $PMT_DIR;
}

# This function unzips and untars specified file to the test dir
sub UnzipUnTarFileToTestDir($$)
{
    my ($dir, $file) = @_;

    chdir $dir;

    # Unzip into tar.
    system("gunzip -cd $file.tar.gz > $file.tar") == 0 or die("Error: unable to unzip $file.tar.gz");

    print("\nFile unzipped successfully...\n\n");

    system("tar xvf $file.tar") == 0 or die("Error: unable to un-tar $file.tar");

    # Restore to Pmt directory.
    chdir $PMT_DIR;

    print("\nFile un-tarred successfully...\n");
}


sub MakeDistribution()
{
    chdir $PMT_TEMP_DIR;
    system("./configure --with-exif=open") == 0 or die("Error: could not configure $PMT_TEMP_DIR");
    system("make dist") == 0 or die("Error: could not make distribution in $PMT_TEMP_DIR");
    
    chdir $PMT_DIR;
}

# This funciton makes a copy of the distribution to the specified path and filename
sub CopyDistribution($)
{
    my ($copy_to) = @_;

    chdir $PMT_TEMP_DIR;
    system("cp $DIST_FILENAME.tar.gz $copy_to.tar.gz") == 0 or die("Error: could not copy $DIST_FILENAME.tar.gz to $copy_to.tar.gz");

    # Restore to Pmt Directory
    chdir $PMT_DIR;
}

sub ConfigureAndMake($$)
{
    my ($root_dir, $option) = @_;

    chdir $root_dir or die("Error: could not cd to $root_dir");

    system("./configure $option") == 0 or die("Error: could not configure $root_dir");

    system("make") == 0 or die("Error: could not make $root_dir");

    # Restore to Pmt Directory
    chdir $PMT_DIR;
}


sub CopyFilesToTempDir()
{
    print "copying files...\n";
    system("cp -r * $PMT_TEMP_DIR") == 0 or die("Error: could not copy to temporary directory $PMT_TEMP_DIR");

    print("\nCopied files to $PMT_TEMP_DIR successfully...\n");
}

		
sub ShowHelpScreen()
{
    print "\nUsage for MakeUnixZip.pl:\n\n";
    print "perl MakeUnixZip.pl [options]\n\n";
    print "-h --help                -- Shows this help screen (following options ignored)\n";
    print "-openToolkitsPath <path> -- Specify absolute path to OpenSource toolkits.\n";
    print "                            It will be assumed that the toolkits will be in the\n";
    print "                            jpeg, exif, tiff, xerces-c directories.\n";
    print "                            If not specified, will not build with open toolkits.\n";
    print "-swioToolkitsPath <path> -- Specify absolute path to SWIO toolkits.\n";
    print "                            It will be assumed that the toolkits will be in the\n";
    print "                            jpeg, exif, tiff, xerces-c directories.\n";
    print "                            If not specified, will not build with swio toolkits.\n";
    print "-buildAndTestDir <path>  -- (Optional) Specify directory where testing will be done.\n";
    print "                            Default: ~/UnixZipTemp\n";
    print "Example usage:\n";
    print "1- Build/Test with OpenSource Toolkits -\n";
    print "   perl MakeUnixZip.pl -openToolkitsPath /pmttools/toolkits/open\n";
    print "2- Build/Test with SWIO Toolkits -\n";
    print "   perl MakeUnixZip.pl -swioToolkitsPath /pmttools/toolkits/swio\n";
    print "3- Build/Test with both OpenSource and SWIO Toolkits\n";
    print "   perl MakeUnixZip.pl -openToolkitsPath /pmttools/toolkits/open -swioToolkitsPath /pmttools/toolkits/swio\n";
}


sub ProcessCommandLineArgs($$)
{
    # "$#ARGV" is 1 less than the actual number of command line arguments passed in.
    
    if ($#ARGV == -1 or $ARGV[0] eq "-h" or $ARGV[0] eq "--help")
    {
	    ShowHelpScreen();
	    exit;
    }

    for (my $argNum = 0; $#ARGV >= $argNum; $argNum++)
    {
	    if ($ARGV[$argNum] eq "-openToolkitsPath") 
	    { 
	        $argNum++;
	        $_[0] = $ARGV[$argNum];
	    }
  	    elsif ($ARGV[$argNum] eq "-swioToolkitsPath") 
	    { 
	        $argNum++;
	        $_[1] = $ARGV[$argNum];
	    }    
	    elsif ($ARGV[$argNum] eq "-buildAndTestDir")
	    {
	        $argNum++;
	        $ROOT_TEMP_DIR = $ARGV[$argNum];
	        $PMT_TEMP_DIR = "$ROOT_TEMP_DIR/pmt";
	    }
	    else
	    {
	        print "\nERROR: INVALID ARGUMENTS!\n";
	        ShowHelpScreen();
	        exit;
	    }
    }
}


sub Main()
{
	# Have all output (even errors) go to STDOUT
	*STDERR = *STDOUT;

    system("clear");
    print "perl MakeUnixZip @ARGV\n";
	system("date");

    # All the subroutines called directly from Main() assume the
    # current working directory is set to Pmt.
    # Get up to Pmt directory.
    chdir "../..";

    # Set the globals permanently.
    $PMT_DIR = cwd();
    $PMT_DISTRIBUTIONS_DIR = "$PMT_DIR/scripts/distributions";
    $OPENTIFF_ROOT = $ENV{TIFFROOT};

    my $openToolkits = 0; # 1 means build with open Toolkits, 0 means don't
    my $openToolkitsPath = "";
    my $swioToolkits = 0; # 1 means build with swio Toolkits, 0 means don't
    my $swioToolkitsPath = "";

    ProcessCommandLineArgs($openToolkitsPath,$swioToolkitsPath);

    if ( not $openToolkitsPath eq "" )
    {
        $openToolkits = 1 ;
    }
    if ( not $swioToolkitsPath eq "" )
    {
        $swioToolkits = 1 ;
    }
    
    if (DoesAbsoluteDirExist($ROOT_TEMP_DIR))
    {
        rmtree("$ROOT_TEMP_DIR") != 0 or die("\n\nError: could not delete $ROOT_TEMP_DIR.");
    }
	
    mkdir("$ROOT_TEMP_DIR", 0777) == 1 or die("Error: could not create $ROOT_TEMP_DIR");
    mkdir("$PMT_TEMP_DIR", 0777) == 1 or die("Error: could not create $PMT_TEMP_DIR");

    CopyFilesToTempDir();

    
    my @configure_options = ("disable-shared", "disable-static") ;
    my $option;

    MakeDistribution();

	foreach $option (@configure_options)
	{
	    if (DoesAbsoluteDirExist("$ROOT_TEMP_DIR/$option"))
	    {
	        rmtree("$ROOT_TEMP_DIR/$option") != 0 or die("\n\nError: could not delete $ROOT_TEMP_DIR/$option.");
	    }
    
	    mkdir("$ROOT_TEMP_DIR/$option", 0777) == 1 or die("Error: could not create $ROOT_TEMP_DIR/$option");

	    chdir "$ROOT_TEMP_DIR/$option" or die("Error: could not cd to $ROOT_TEMP_DIR/$option");
    
        if ( $openToolkits )
        {
            mkdir("$ROOT_TEMP_DIR/$option/open", 0777) == 1
                or die("Error: could not create $ROOT_TEMP_DIR/$option/open");
            CopyDistribution("$ROOT_TEMP_DIR/$option/open/$PMT_TEMP_DIST");
		    UnzipUnTarFileToTestDir("$ROOT_TEMP_DIR/$option/open",$PMT_TEMP_DIST);
        }
        if ( $swioToolkits )
        {
            mkdir("$ROOT_TEMP_DIR/$option/swio", 0777) == 1
                or die("Error: could not create $ROOT_TEMP_DIR/$option/swio");
            CopyDistribution("$ROOT_TEMP_DIR/$option/swio/$PMT_TEMP_DIST");
		    UnzipUnTarFileToTestDir("$ROOT_TEMP_DIR/$option/swio",$PMT_TEMP_DIST);
        }
    
	    chdir $PMT_DIR or die("Error: could not cd to $PMT_DIR");
	}

    # Get the name of the distribution and assign it to $DIST_FILENAME
    if ( $openToolkits )
    {
        chdir "$ROOT_TEMP_DIR/$configure_options[0]/open"
            or die("Error: could not cd to $ROOT_TEMP_DIR/$configure_options[0]/open");
    }
    else
    {
        chdir "$ROOT_TEMP_DIR/$configure_options[0]/swio"
            or die("Error: could not cd to $ROOT_TEMP_DIR/$configure_options[0]/swio");
    }
    my @files_in_dir = <pmt-*> ;
    my $file;
    foreach $file (@files_in_dir)
    {
        if (-d $file)
        {
      	    $DIST_FILENAME = $file;
      	    last;
        }
    }
    chdir $PMT_DIR or die("Error: could not cd to $PMT_DIR");


    foreach $option (@configure_options)
    {
        if ( $openToolkits )
        {
            $ENV{JPEGROOT} = "$openToolkitsPath/jpeg" ;
            $ENV{EXIFROOT} = "$openToolkitsPath/exif" ;
            $ENV{TIFFROOT} = "$openToolkitsPath/tiff" ;
            $ENV{XERCESCROOT} = "$openToolkitsPath/xerces-c" ;
            $ENV{LD_LIBRARY_PATH} = "$openToolkitsPath/exif/lib:$openToolkitsPath/tiff/lib:$openToolkitsPath/xerces-c/lib:$ENV{LD_LIBRARY_PATH}" ;
            
            ConfigureAndMake("$ROOT_TEMP_DIR/$option/open/$DIST_FILENAME", "--$option --with-exif=open --with-tiff=open");
            RunTests("$ROOT_TEMP_DIR/$option/open/$DIST_FILENAME","open");
        }
        if ( $swioToolkits )
        {
            $ENV{JPEGROOT} = "$swioToolkitsPath/jpeg" ;
            $ENV{KIESEXIFROOT} = "$swioToolkitsPath/exif" ;
            $ENV{TIFFROOT} = "$swioToolkitsPath/tiff" ;
            $ENV{XERCESCROOT} = "$swioToolkitsPath/xerces-c" ;
            
            ConfigureAndMake("$ROOT_TEMP_DIR/$option/swio/$DIST_FILENAME", "--$option --with-exif=kies --with-tiff=ektiff --with-image-accessor=no");
            RunTests("$ROOT_TEMP_DIR/$option/swio/$DIST_FILENAME","swio");
        }
    }

    # copy distribution to the distributions directory (where this script was called from)
    CopyDistribution("$PMT_DISTRIBUTIONS_DIR/$DIST_FILENAME");

    print "\n\nEntire script has run successfully...end\n\n\n";
}

Main();
