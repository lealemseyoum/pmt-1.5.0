#!/bin/bash

#
# Creation Date: July 2003
#
# Original Author:
# Sam Fryer <samuel.fryer@kodak.com>
#
# Contributor(s):
#
#
# Known bugs:
#
 
 
# To run: execute this script while in the Pmt/scripts directory.
 
PMTBASEDIR="$PWD/.."

# Set where we're going to store some stuff
cd $PMTBASEDIR/test/PmtInterpreterTest

mv PmtInterpreterTest.cpp PmtInterpreterTest.cpp.orig

cat << MYEOF1 > PmtInterpreterTest.cpp

#include "PmtDefinitionInterpreter.h"

// Global Variables
int gnThreads = 1;              // Number of threads running
int gnIterations = 1;           // Number of iterations per thread
int gnExitedWithError = 0;      // Number of exceptions caught

int main()
{
    PmtLogicalDefinitionInterpreter interp;
    interp.load("PmtDefaultSchemaInfo.xml");
    
    return 0;
}

MYEOF1

cat << MYEOF2 > PmtDefaultSchemaInfo.xml
<?xml version="1.0" encoding="UTF-8"?>
<MetadataDefinitionBindings>
	<MetadataDefinitionBinding>
		<MetadataDefnFileURI>../../DefaultDefinitions/PmtDefaultDefinitions.xsd</MetadataDefnFileURI>
		<RootElementName>PmtRootElement</RootElementName>
		<AssociatedTranslationTableURI>default</AssociatedTranslationTableURI>
	</MetadataDefinitionBinding>
</MetadataDefinitionBindings>

MYEOF2

cd $PMTBASEDIR

if [[ -f Makefile ]] ; then
    make distclean
fi

./configure --disable-shared --enable-debug --with-xml=xerces --with-write-out-schema --with-embedded-docs

make all

cat << MYEOF3 > $PMTBASEDIR/src/PmtInterpreter/PmtDefaultPreparsedSchema.h
/* 
 * The contents of this file are subject to the IBM Common Public
 * License Version 0.5 (the "License"); you may not use this file 
 * except in compliance with the License. You may obtain a copy of 
 * the License at http://oss.software.ibm.com/developerworks/
 * opensource/license-cpl.html
 *  
 * The Original Code is the Picture Metadata Toolkit,
 * released November 13, 2000.
 *
 * The Initial Developer of the Original Code is Eastman 
 * Kodak Company. Portions created by Kodak are 
 * Copyright (C) 1999-2003 Eastman Kodak Company. All 
 * Rights Reserved. 
 * 
*/

/***************************************************
 * This is a generated file.  DO NOT EDIT!         *
 ***************************************************/

#ifndef PMT_DEFAULT_PREPARSED_SCHEMA
#define PMT_DEFAULT_PREPARSED_SCHEMA

static const char * PmtDefaultPreparsedSchema[] = {
MYEOF3


cd $PMTBASEDIR/test/PmtInterpreterTest

./interpTest >> $PMTBASEDIR/src/PmtInterpreter/PmtDefaultPreparsedSchema.h

mv PmtInterpreterTest.cpp.orig PmtInterpreterTest.cpp
rm PmtDefaultSchemaInfo.xml

cd $PMTBASEDIR

cat << MYEOF4 >> $PMTBASEDIR/src/PmtInterpreter/PmtDefaultPreparsedSchema.h
};

#endif

MYEOF4

make distclean

echo Script completed successfully!

