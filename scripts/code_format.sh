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
# Creation Date: March 2003
#
# Original Author: 
# Sam Fryer <samuel.fryer@kodak.com>
# 
# Contributor(s): 
#
#
# Known bugs:
# 1. indent collapses spaces in templates -- i.e.:
#      x< y< z > > becomes x< y< z >>
#      which may cause problems with some compilers.
#


for FILE in `find . -iname "*.h" ; find . -iname "*.cpp"`; do
    echo working on $FILE ...
    indent $FILE -o tmp -bap -bbo -bl -bli0 -bls -ci4 -cli0 -cp33 -cs -d0 -di1 -hnl -i4 -ip0 -l79 -lp  -nbc -ncdb -ncdw -nce -nfc1 -nfca -npcs -nprs -npsl -nsc -nsob -nss -nut -saf -sai -saw -ts4
    cat tmp | grep -v "##ModelId" > $FILE
    echo "" >> $FILE
    rm tmp
done

