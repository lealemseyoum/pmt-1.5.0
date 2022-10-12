/* 
 * The contents of this file are subject to the IBM Common Public
 * License Version 0.5 (the "License"); you may not use this file 
 * except in compliance with the License. You may obtain a copy of 
 * the License at http://oss.software.ibm.com/developerworks/
 * opensource/license-cpl.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Initial Developer of the Original Code is Eastman
 * Kodak Company. Portions created by Kodak are
 * Copyright (C) 2003 Eastman Kodak Company. All
 * Rights Reserved.
 *
 * Original Author:
 * Sam Fryer  samuel.fryer@kodak.com
 *
 * Contributor(s):
 * Don Brown
 *
 * Date: February 18, 2003
*/

/*
 * Program Usage:
 *   DataList [-s | -x] <filename> [Key List]
 *     where -s signifies usage of the PmtMetadata.show() method
 *     -x signifies using XML output
 *     <filename> is the name of the file to get the metadata from
 *     And Key list are an optional list of keys to print
 *        (instead of all)
*/

// The core PMT includes ...
#include "PmtMetadataT.h"
#include "PmtDefinitionInterpreter.h"

// Each accessor that we might need has to be included so that it gets 
// set up correctly in the factory.
#include "PmtAllAccessors.h"

// Only needed to print out the version of PMT we're using
#include "PmtVersionNumber.h"

// Here is a list of the error codes we return.
#define PMTDATALIST_OK 0
#define PMTDATALIST_ERROR 1
#define PMTDATALIST_PMT_EXCEPTION_THROWN 2
#define PMTDATALIST_PMT_UNKNOWN_EXCEPTION_THROWN 3
#define PMTDATALIST_PMT_ACCESSOR_NOT_FOUND 4
#define PMTDATALIST_NO_METADATA 5


// A convience class to help pass around all the command line configurations.
class ConfigOptions
{
    public:
        // The constructor -- initialize everything to "false"
        ConfigOptions()
            : mdComma(false),mdDeleteItems(false),
              mdSchema(NULL),mdShow(false),mdShowAll(false),
              mdShowBriefDesc(false), mdShowFullDesc(false),
              mdShowOnlyMetadata(false),mdShowType(false),mdShowXML(false),
              mdShowURI(false),mdWriteItems(false)
            {};
        
        bool mdComma;         // whether to use CSV style or not
        bool mdDeleteItems;   // Set a metadata item (requires write access)
        char * mdSchema;      // Additional schema info URI to use
        bool mdShow;          // whether to use the default "show" method or not
        bool mdShowAll;       // get all metadata items -- not just ones present
        bool mdShowBriefDesc; // print a brief description of the metadata item
        bool mdShowFullDesc;  // print a full description of the metadata item
        bool mdShowOnlyMetadata;  // show metadata value only
        bool mdShowType;      // Show the metadata type
        bool mdShowXML;       // whether to output default XML or not
        bool mdShowURI;       // Replace the key string with a URI
        bool mdWriteItems;    // Set a metadata item (requires write access)
};

// Routine to set the output spacing between key and data
string getSpacing(ConfigOptions * co, PmtMetadataPtr md)
{
    // set our spacing string
    string spacing = ",";
    
    // If we're not doing CSV style prints then determine the # of tabs
    if (!co->mdComma)
    {
        // Adjust the number of tabs based on the length of the key
		int len = 0;
		if (!(co->mdShowURI))
			len = md->fullKey().length();
		else
			len = md->getURI().length();

        if (len > 52) spacing = " ";
        else if (len > 39) spacing = "\t";
        else if (len > 31) spacing = "\t\t";
        else if (len > 22) spacing = "\t\t\t";
        else if (len > 12) spacing = "\t\t\t\t";
        else spacing = "\t\t\t\t\t";
    }
    return spacing;
}

// DSB: modified for output for -m option
// DSB: and to handle output for specified keys not in file
// Create our own method for showing metadata
//   (Note that we assume md to be a valid pointer.)
void UseMyShow(ConfigOptions * co, 
               PmtMetadataPtr md, 
               bool isDefault = false)
{
    SPmtString valueString("**Not in file**");
    
    // determine our separator (Tabs or Commas).
    char separator = '\t';
    if (co->mdComma) separator = ',';

    if ( (!isDefault) || (isDefault && co->mdWriteItems) )
        valueString = EK_WIDE2STR(md->getValueStr());

    if ( !(co->mdShowOnlyMetadata) )
    {
        // Print the Fullkey, then some separation
		if (!(co->mdShowURI))
			cout << md->fullKey();
		else
			cout << md->getURI();
		cout << getSpacing(co,md);
    }   
    cout << valueString;    

    // Did we ask to show the typing of the metadata?
    if (co->mdShowType)
        cout << separator << md->trueType();

    // Did we ask to see a brief description of the metadata?
    if (co->mdShowBriefDesc)
        cout << separator << md->briefDocumentation();

    // Did we ask to see a full description?
    if (co->mdShowFullDesc)
        cout << separator << md->fullDocumentation();

    // Finally, end the line
    cout << endl;
}

// This is a convience routine to convert a text string to
// a vector of integers like one would see straight from the
// file for certain data types (like byte or undefined).
// It is exactly that -- a convience routine, and will not
// perform optimally in all circumstances.
SPmtString convertToVector(SPmtString value, PmtMDType mdType)
{
	SPmtString ret;
	if ( (mdType < PMT_MD_TYPE_STRING) && 
		(value.length() > 0) && (value[0] >= 'A'))
	{
		char buf[5];
		for (int i = 0; i< value.length(); i++)
		{
			sprintf(buf,"%d",value[i]);
			ret += buf;
			ret += " 00 ";
		}
	}
	else
		ret = value;
	return ret;
}


// Procedure: readFileAndList
// Purpose:   read an image file and dump the metadata to standard out
// Inputs:    filename -- the name of the file to read
//            co -- configuration options
//            keys -- the user-specified keys to display or write
//            valus -- the user-specified values to write
//            nkeys -- the number of user-specified keys/values
// Returns:   Above listed error codes.
int readFileAndList(char *filename, ConfigOptions * co,
                    std::vector<SPmtString>&keys,std::vector<SPmtString>&values, int nkeys)
{
    // signal the caller if we had any problems
    int return_value = PMTDATALIST_OK;

    // The PMT may throw exceptions.  We need to be able to handle them
    // to exit a little more gracefully.
    try
    {
        // load the default metadata definition
        PmtLogicalDefinitionInterpreter interpreter;
        interpreter.load();
        
        // Load the specified schema as well as the builtin (if specified).
        // This requires an XML parser to be configured in during the build.
        // (this can be used to load & test new schemas, but be careful --
        //  PMT may not be the most robust tool for testing!)
        if (co->mdSchema != NULL)
            interpreter.load(co->mdSchema);

        // We don't normally have to specify a mode -- it defaults
        // to read only.  However, since we might want to change 
        // some metadata, we have to specify PMT_READWRITE
        PmtOpenMode openMode = PMT_READONLY;
        
        if (co->mdWriteItems || co->mdDeleteItems)
            openMode = PMT_READWRITE;
            
        // Create the appropriate file accessor
        PmtAccessorPtr inAcc = PmtAccessor::getAccessor(filename,openMode);

        // verify that an accessor was returned.
        if (inAcc.ptr())
        {

            // Read in the metadata
            PmtMetadataPtr mdRoot = inAcc->readMetadata();

            // Make sure that we got a valid root pointer
            if (mdRoot.ptr())
            {
                // if no keys were specified, dump them all
                if (nkeys == 0)
                {
                    if (co->mdShowXML)
                    {
                        // The default streaming output of a PmtMetadataPtr
                        // is XML.
                        cout << mdRoot << endl;
                    }
                    else
                    {
                        PmtKeyType mdKey;
                        
                        // To show all metadata items -- not just the ones
                        // present in the file, we have to set the key to 
                        // "*[*]".  Otherwise, we leave it empty.
                        if (co->mdShowAll)
                            mdKey = "*[*]";
                            
                        // create an iterator to go through the data
                        PmtMetadataIterator mdIter = mdRoot->getMetadata(mdKey);

                        // pull the first piece of metadata off
                        PmtMetadataPtr md = mdIter.start();

                        // Loop through all the metadata
                        while (md)
                        {
                            // Do we want to use the default "show" method?
                            if (co->mdShow)
                                md->show();
                            else
                                UseMyShow(co,md);

                            // Get the next piece of metadata
                            md = mdIter.next();
                        }
                    }
                }
                else // The user specified some keys keys to display so 
                {    // loop through and display/write each.
                
                    // Create a root pointer
                    PmtMetadataPtr keysOnlyRoot = PmtMetadata::create();
                    
                    // Create a vector for XML warning comments
                    vector < string > xmlComments;

                    int i = 0;
                    for (i = 0; i < nkeys ; i++ )
                    {
                        // DSB: changed 2nd param to mdShowAll and added 
                        // DSB: appropriate logic
                        // Get each metadata key
                        // Note if second parameter in getMetadatum is true,
                        // PMT will create the the metadata object with a 
                        // default value if it does not exist in the file.                  
                        PmtMetadataPtr md = 
                                mdRoot->getMetadatum(keys[i],co->mdShowAll);

                        bool isDefaultMD = false;
                        
                        // see if key is bad or just not in file
                        if (!md) 
                        {    
                            md = mdRoot->getMetadatum(keys[i],true);
                            isDefaultMD = true;
                        }
                        
                        if (md)
                        {
                            // Do we want to change the values before we
                            // display anything?
                            if (co->mdWriteItems)
                            {
								// Try to be nice and convert the data 
								// if necessary ...
								SPmtString newvalue = convertToVector(values[i],md->getMDType());

                                // Then, we have to set the correct value
                                md->setValueStr(EK_STR2WIDE(newvalue));
                                
                                // Then, we have to tell the accessor that
                                // we want the value changed in the file.
                                inAcc->writeMetadata(md);
                            }

                            // Do we want to delete the values instead of
                            // displaying them?
                            if (co->mdDeleteItems)
                            {
                                // Then, we have to tell the accessor that
                                // we want the value deleted in the file.
                                inAcc->removeMetadata(md);
                            }
                            else // we're going to display it
                            {
                                // Do we want to use the default "show" method?
                                if (co->mdShow)
                                {
                                    // This should be just md->show(),
                                    // but it might *appear* that metadata
                                    // NOT in the file really is.
                                    if (isDefaultMD) 
                                    {
                                        UseMyShow(co,md,isDefaultMD);
                                    }
                                    else 
                                    {
                                       md->show();
                                    }
                                }
                                
                                // Do we want to show the XML?
                                if (co->mdShowXML)
                                {
                                    if (!isDefaultMD) 
                                    {
                                        // If we're showing XML, create an md object
                                        PmtMetadataPtr tmpMd = 
                                            keysOnlyRoot->getMetadatum(
                                            md->fullKey(),true);

                                        // now set the new object's value
                                        // (There's more effecient ways of doing
                                        // this, but none are easier...)
                                        tmpMd->setValueStr(md->getValueStr());
                                    }
                                    else // key is not in file so add comment
                                    {
                                        string warn = "<!-- Key ";
                                        warn += md->fullKey();
                                        warn += " is not in the file. -->\n";
                                        xmlComments.push_back(warn);    
                                    }
                                }

                                // if none of the above ...
                                if ((!co->mdShow)&&(!co->mdShowXML))
                                    UseMyShow(co,md,isDefaultMD);
                            }
                        }
                        else
                        { 
                            // The key given on the command line must 
                            // not be in the schema.  Create a warning message.
                            string warn;
                            if (co->mdShowXML) warn += "<!-- ";
                            warn += "Key ";
                            warn +=  keys[i]; 
                            warn += " is not in the schema";
                            if (co->mdShowXML) 
                            {
                                warn += " -->\n";
                                xmlComments.push_back(warn);
                            }
                            else 
                            {
                                cerr << warn << "\n";
                            }     
                        }
                    }
                    
                    // if we're outputting XML, we haven't output anything yet.
                    // So, let's do it here.
                    if (co->mdShowXML)
                    {
                        // The default streaming output of a 
                        // PmtMetadataPtr is XML.
                        cout << keysOnlyRoot << endl;
                        
                        // Also, output all of our warnings about keys not
                        // present.
                        for (unsigned int i=0; i<xmlComments.size(); i++)
                        {
                            cout << xmlComments[i];
                        }
                    }
                }
            }
            else
            {
                cout << "Could not find metadata in the file "
                    << filename << " .\n";
                return_value = PMTDATALIST_NO_METADATA;
            }
            
            // Finally, close the file
            inAcc->close();
        }
        else
        {
            cout << "Could not find an accessor for the file "
                << filename << " .\n";
            return_value = PMTDATALIST_PMT_ACCESSOR_NOT_FOUND;
        }
    }

    // Catch exceptions explicitely thrown by PMT
    catch(PmtError & e)
    {
        cout << e << endl;
        return_value = PMTDATALIST_PMT_EXCEPTION_THROWN;
    }

    // Catch any other exceptions
    catch(...)
    {
        cout << "**** Some Exception caught." << endl;
        return_value = PMTDATALIST_PMT_UNKNOWN_EXCEPTION_THROWN;
    }

    return return_value;
}



// Read the keys (and values) from a file.
// Note: no key nor value may be greater than BUFFERSIZE large.
#define BUFFERSIZE 2048
void readKeys(char * filename,
              std::vector<SPmtString>&keys,std::vector<SPmtString>&values,int &nkeys, 
              ConfigOptions &co)
{
	FILE * input = fopen(filename,"r");
	if (input != NULL)
	{
        std::vector<SPmtString> newargv;
        char buf[BUFFERSIZE];
        int bufLength = 0;
		while ((bufLength = fscanf(input,"%s",buf)) != EOF)
		{
            if (buf[0] == '\"')
            {
                char c[2];
                char * a = buf;
                char * b = &buf[1];
                while ( *a != 0 ) { *a = *b; a++; b++; bufLength++; }
                a--;
                while ((!feof(input)) && (bufLength < BUFFERSIZE))
                {
                    bufLength += fread(a,1,1,input);
                    if (*a == '\"') 
                    {
                        *a = 0;
                        break;
                    }
                    a++;
                }
            }
		    newargv.push_back(buf);
		}
		fclose(input);
		if (newargv.size()>0)
        {
            std::vector<SPmtString>::iterator iter = newargv.begin();
            while (iter != newargv.end())
            {
                if (!co.mdWriteItems)
                {
                    keys.push_back(*iter);
                    nkeys++;
                }
                else 
                {
                    SPmtString argVal = (*iter);
                    iter++;
                    if (iter != newargv.end())
                    {
                        keys.push_back(argVal);
                        values.push_back(*iter);
                        nkeys++;
                    }
                    else
                    {
                        cerr << "Missing metadata key or value\n";
                    }
                }
                iter++;
            }
        }
	}
	else
		std::cerr << "ERROR: Could not open command file : " << filename << std::endl;
}


// Routine to print out all the options...
void print_usage(char * progname)
{
    cout << " Usage:\n"
         << progname << " [ options ] <filename> [key list]\n\n"
         << "Where options can be:\n"
         << "-a \t\t print items even if they don't exist in the file\n"
         << "-b \t\t print a brief description of the metadata items\n"
         << "-c \t\t print the data in a comma separated format\n"
         << "-d \t\t delete the items instead of printing them\n"
         << "   \t\t (this requires the key list)\n"
         << "-e <Schema URI>  enhance the built-in schema with the specified URI\n"
         << "-f \t\t print a full description of the metadata items\n"
         << "-h \t\t print this message and exit\n"
         << "-k <filename> load the text file of keys (or key value pairs)\n"
         << "-m \t\t print only the metadata value\n"
         << "-s \t\t print using PMT's default printing method\n"
         << "-t \t\t print the metadata type after its value\n"
         << "-u \t\t print the URI instead of the key\n"
         << "-v \t\t print the version number of the PMT library we're using\n"
         << "-w \t\t write the items instead of printing them\n"
         << "   \t\t (this requires <key keyvalue> pairs in the key list)\n"
         << "-x \t\t print the output in an XML format\n\n"
         << "filename (required) must be a file type understood by PMT\n\n"
         << "key list: A list of keys to print out\n\n"
         << "NOTE: Operations that destroy the integrity of the file will not\n"
         << "      be allowed! (Such as deleting the width in a JPEG file)\n\n";
    
}


// The main entry point.
int main(int argc, char *argv[])
{
    int return_value = -1;
    ConfigOptions co;

    // Read the input arguements
    if (argc >= 2)
    {
        // our placeholder for which parameter we're on
        int i = 0;

        // The name of the file we're dumping the metadata from
        char *filename = NULL;

        // The user specified keys to show
        std::vector<SPmtString> keys;
        std::vector<SPmtString> values;
        int nkeys = 0;
		
		// Some parameter helpers
		// Do we need to print how to use this?
		bool printUsage = false;

        // Loop through the parameters
        for (i = 1 ; i < argc ; i++)
        {
            // if it doesn't have a '-' then we assume it's the file name
            if (argv[i][0] != '-')
            {
                if (filename == NULL)
                    filename = argv[i];
                else 
                {
                    // if we already have a filename, then assume it's
                    // a key (or a (key,value) pair for writing)
                 
                    if (!co.mdWriteItems)
                    {
                        keys.push_back(argv[i]);
                        nkeys++;
                    }
                    else 
                    {
                        if (i < argc - 1)
                        {
                            keys.push_back(argv[i++]);
                            values.push_back(argv[i]);
                            nkeys++;
                        }
                        else
                        {
                        	cerr << "Missing metadata key or value\n";
                        	return_value = PMTDATALIST_ERROR;
                        }
                    }
                }
            }
            else 
            {
                switch(argv[i][1])
                {
                    // Display all PMT knows about metadata 
                    // (not just what's present in the metadata file)
                    case 'a':
                        co.mdShowAll = true;
                        break;
                        
                    // Display a brief description of the metadata
                    case 'b':
                        co.mdShowBriefDesc = true;
                        break;

                    // Display in a Comma Separated Value (CSV) style 
                    case 'c':
                        co.mdComma = true;
                        break;

                    // Delete the listed metadata items
                    case 'd':
                        co.mdDeleteItems = true;
                        break;

                    // Specify an additional schema URI to use
                    case 'e':
                        i++;
                        co.mdSchema = argv[i];
                        break;

                    // Display a full description of the metadata
                    case 'f':
                        co.mdShowFullDesc = true;
                        break;
                        
                    // They asked for help, so display the options
                    // and exit.
                    case 'h':
						printUsage = true;
						if (return_value < PMTDATALIST_OK)
							return_value = PMTDATALIST_OK;
                        break;
                        
                     // load the keys (and values) from a file
                    case 'k':
                        if (i < argc - 1)
                           readKeys(argv[++i],keys,values,nkeys,co);
                        break;
                        
                   // DSB: added -m option 
                    // If next character is an 'm' then assume "show metadata"
                    case 'm':
                        co.mdShowOnlyMetadata = true;
                        break;
                        
                    // If the next character is an 's' then assume "show"
                    case 's':
                        co.mdShow = true;
                        break;
                        
                    // If the next character is an 's' then assume "show type"
                    case 't':
                        co.mdShowType = true;
                        break;
 
					// If the next character is an 's' then assume "show type"
                    case 'u':
                        co.mdShowURI = true;
                        break;
                       
                    // If the next character is an 'v' then show the PMT
                    // version number and exit
                    case 'v':
                        cout << argv[0] << " using PMT Version " 
                             << PmtVersionNumber << endl;
						if (return_value < PMTDATALIST_OK)
							return_value = PMTDATALIST_OK;
                        break;

                    // If the next character is an 'w' then write the items
                    case 'w':
                        co.mdWriteItems = true;
                        break;
                                                
                    // If the next character is an 'x' then we're showing XML
                    case 'x':
                        co.mdShowXML = true;
                        break;
                        
                    // Otherwise, say we don't understand
                    default:
                        cout << "Unknown Option: " << argv[i] << endl;
						printUsage = true;
						if (return_value < PMTDATALIST_ERROR)
							return_value = PMTDATALIST_ERROR;
                }
            }
        }

		// if we need to print out how to use the program, do so here.
		if (printUsage)
			print_usage(argv[0]);

        // if we have a filename, dump the metadata from the file
        if ((filename != NULL)&&(return_value == -1))
            return_value = readFileAndList(filename, &co,
                                           keys, values, nkeys);

		// if we don't have a valid return value, set it to error
		if (return_value == -1)
			return_value = PMTDATALIST_ERROR;
    }
    else
    {
        print_usage(argv[0]);
        return_value = PMTDATALIST_ERROR;
    }
    
    // return the status
    return return_value;
}

