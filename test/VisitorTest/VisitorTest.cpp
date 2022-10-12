// VisitorTest.cpp : Defines the entry point for the console application.
//


#include "PmtDefinitionInterpreter.h"
#include "PmtAccessor.h"
#include "PmtAllAccessors.h"
#include "GetMetadataValue.h"

// Works when PMT_REQUIRE is commented out
//PMT_REQUIRE(PmtAllAccessors)
int main(int argc, char *argv[])
{
    // signal the caller if we had any problems
    int return_value = 0;
    
    GetMetadataValue mdVisitor;

    // load metadata definition
    PmtLogicalDefinitionInterpreter interpreter;
    interpreter.load();

    char inExif[] = "KodakDC260.jpg";

    cout << endl << "--- Testing PmtVisitor ---" << endl;

    try
    {
        // ----- testing read from an Exif file and write to an xml file
        PmtAccessorPtr inAcc = PmtAccessor::getAccessor(inExif);

        // verify that an accessor was returned.
        if (inAcc)
        {

            // Read in the metadata and close the file
            PmtMetadataPtr mdRoot = inAcc->readMetadata();
            inAcc->close();

            // Make sure that we got a valid root pointer
            if (mdRoot.ptr())
            {
                // create an iterator to go through the data
                PmtMetadataIterator mdIter = mdRoot->getMetadata("");

                // pull the first piece of metadata off
                PmtMetadataPtr md = mdIter.start();

                // Loop through all the metadata
                while (md)
                {
                    mdVisitor.goGetIt(md);
                    
                    cout << md << endl;

                    // Get the next piece of metadata
                    md = mdIter.next();
                }
            }
            else
                cout << "Could not find metadata in the file "
                    << inExif << " .\n";
        }
        else
            cout << "Could not find an accessor for the file "
                << inExif << " .\n";
    }

    // Catch exceptions explicitely thrown by PMT
    catch(PmtError & e)
    {
        cout << e << endl;
        return_value = 1;
    }

    // Catch any other exceptions
    catch(...)
    {
        cout << "**** Some Exception caught." << endl;
        return_value = 1;
    }

    return return_value;
}

