#!/bin/bash

# The contents of this file are subject to the IBM Common Public
# License Version 0.5 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy of
# the License at http://oss.software.ibm.com/developerworks/
# opensource/license-cpl.html
#
# The Original Code is the Picture Metadata Toolkit,
# released November 13, 2000.
#
# The Initial Developer of the Original Code is Eastman
# Kodak Company. Portions created by Kodak are
# Copyright (C) 2003 Eastman Kodak Company. All
# Rights Reserved.
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
 
 
# To run: execute this script while in the Pmt/src directory.
 
# This script expects that the configure script has been run with the
# following options: --enable-debug --enable-profile .  Other options
# may be applied as needed.  The script cleans and builds the entire
# project, and then runs our three main tests, determining which
# lines of executable code were not run during each test.
# At the end, the lines are compared for each test to determine overall
# which lines had not been executed at all.
 
# Note that lines that were disabled due to the set of configure options
# that were passed in (such as KIES vs. OpenExif code) will NOT show up.
 
# The list of tests to run 
TESTS="PmtInterpreterTest AccessorTest VisitorTest"

PMTBASEDIR="$PWD/.."

# Set where we're going to store some stuff
cd $PMTBASEDIR/src
if [[ ! -d tmp ]] ; then 
    mkdir tmp
fi

# Some basic cleanup...
rm -f *.compiled compiled.*txt

# run for each test
for TEST in $TESTS ; do

    # Some basic cleanup...
    rm -f *.out *.gcov tmp/* *.da

    # Determine the executable name for each test
    if [[ $TEST == "PmtInterpreterTest" ]] ; then 
        TESTEXE="interpTest"
    elif [[ $TEST == "AccessorTest" ]] ; then 
        TESTEXE="accrTest"
    elif [[ $TEST == "VisitorTest" ]] ; then 
        TESTEXE="visitorTest"
    fi

    # CD to the test directory and run the test
    cd $PMTBASEDIR/test/$TEST
    $TESTEXE ;
    cd $PMTBASEDIR/src

    # Now, determine which lines haven't been run....
    # First, find the possible lines that MAY NOT have been executed
    for FILE in */*.cpp ; do

        CPPFILE=`basename $FILE `;

        gcov $CPPFILE 

        mv [EPp]*.gcov tmp

        grep -n "######" tmp/*.gcov | awk '{ print $1 ;}' > $CPPFILE.out

        rm *.gcov
        rm -f tmp/*

    done


    # Second, do a little analysis to determine exactly which
    # lines WERE NOT executed in this TEST
    for FILE in *.out ; do

        echo Working on $TEST:$FILE 

        for TLINE in `cat $FILE ` ; do

            FILENAME=`echo $TLINE | sed 's/:/ /g' | awk '{ print $1 ; }'`

            TIMESMENTIONED=`grep -l $FILENAME *.out | wc -l`
            LINEMENTIONED=`grep "$TLINE" *.out | wc -l`

            if [[ $LINEMENTIONED == $TIMESMENTIONED ]] ; then
                echo $TLINE >> $TEST.compiled
            fi

        done

    done

    sort -u $TEST.compiled > $TEST.sorted.compiled

done

# Some basic cleanup...
rm -rf tmp *.out *.gcov

#Lastly, combine ALL the results to determine what was REALLY NOT EXECUTED!
for FILE in *.sorted.compiled ; do

    echo Working on $FILE 

    for TLINE in `cat $FILE ` ; do

        LINE=`echo $TLINE | sed 's/:/ /g'`

        FILENAME=`echo $LINE | awk '{ print $1 ; }'`

        TIMESMENTIONED=`grep -l $FILENAME *.sorted.compiled | wc -l`
        LINEMENTIONED=`grep "$TLINE" *.sorted.compiled | wc -l`

        if [[ $LINEMENTIONED == $TIMESMENTIONED ]] ; then
            echo $LINE >> x.txt
        fi

    done

done

sort -u x.txt | sed 's/tmp\///g' | sed 's/\.gcov//g' > compiled.txt
rm -f x.txt


