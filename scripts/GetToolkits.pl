#! c:\perl\bin\perl.exe -w


use strict;
use Getopt::Std;


my ($dirFrom);


sub FixupExifTagsFile()
{
    my $BEGIN_STRING = "/** Following Are Tags defined  for DeltaImage support **/\n";
    my $END_STRING = "#define EXIFTAG_DELTAIMAGE_DATA				19\n";
    my $EXIF_TAGS_FILE_PATH = "Pmt/Toolkits/Kies/inc/ExifTags.h";
    my $TEMP_FILENAME = "temp.tmp";

    open(INFILE, $EXIF_TAGS_FILE_PATH) or die("Error: could not open $EXIF_TAGS_FILE_PATH");
    open(NEWFILE, ">$TEMP_FILENAME") or die("Error: could not open $TEMP_FILENAME");

    # First get rid of several #define's.
    while (<INFILE>)
    {
        if ($_ eq $BEGIN_STRING)
        {
            # Go into loop to skip over the lines dealing with DeltaImage.
            while (<INFILE>)
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
            # There are some lines that we don't remove, but we do change
            # all instances of DELTAIMAGE to EKSPECIMAGE.
            my $line = $_;

            $line =~ s/DELTAIMAGE/EKSPECIMAGE/;
            
            print NEWFILE $line;
        }
    }

    close(INFILE) or die("Error: could not close $EXIF_TAGS_FILE_PATH");
    close(NEWFILE) or die("Error: could not close $TEMP_FILENAME");
    unlink($EXIF_TAGS_FILE_PATH) or die ("Error: could not delete $EXIF_TAGS_FILE_PATH");
    rename($TEMP_FILENAME, $EXIF_TAGS_FILE_PATH) or die("Error: could not rename file $TEMP_FILENAME");

    print("\nExifTags.h file fixed up successfully...\n");
}


sub Main()
{
    getopts('i:');

	# Override input and output arguments, if available.
	if ($Getopt::Std::opt_i)
    {
		$dirFrom = $Getopt::Std::opt_i;
	}
    else
    {
        print("Directory path to copy Toolkits from is required as a command-line argument\n");
        print("via the '-i' option.\n");
        die("Error: Argument required!");
    }

    # Get up to just above the Pmt directory.
    chdir "../..";

    # Copy entire Ijl
    system("xcopy $dirFrom\\Ijl\\*.* Pmt\\Toolkits\\Ijl\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Ijl directory");

    # Copy Jpeg/inc and Jpeg/lib
    system("xcopy $dirFrom\\Jpeg\\inc\\*.* Pmt\\Toolkits\\Jpeg\\inc\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Jpeg directory");
    system("xcopy $dirFrom\\Jpeg\\lib\\*.* Pmt\\Toolkits\\Jpeg\\lib\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Jpeg directory");

    # Copy Kies/inc and certain Kies/lib files
    system("xcopy $dirFrom\\Kies\\inc\\*.* Pmt\\Toolkits\\Kies\\inc\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Kies directory");
    system("xcopy $dirFrom\\Kies\\lib\\debug\\ekexifio140d.* Pmt\\Toolkits\\Kies\\lib\\debug\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Kies/lib directory");
    system("xcopy $dirFrom\\Kies\\lib\\debug\\ekfpixjpeg140d.* Pmt\\Toolkits\\Kies\\lib\\debug\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Kies/lib directory");
    system("xcopy $dirFrom\\Kies\\lib\\debug\\exifio.lib Pmt\\Toolkits\\Kies\\lib\\debug\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Kies/lib directory");
    system("xcopy $dirFrom\\Kies\\lib\\release\\ekexifio140.* Pmt\\Toolkits\\Kies\\lib\\release\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Kies/lib directory");
    system("xcopy $dirFrom\\Kies\\lib\\release\\ekfpixjpeg140.* Pmt\\Toolkits\\Kies\\lib\\release\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Kies/lib directory");
    system("xcopy $dirFrom\\Kies\\lib\\release\\exifio.lib Pmt\\Toolkits\\Kies\\lib\\release\\*.* /s /e") == 0 or die("Error: could not xcopy the Toolkits/Kies/lib directory");

    FixupExifTagsFile();

    print "\n\nEntire script has run successfully...end\n\n\n";
}


Main();