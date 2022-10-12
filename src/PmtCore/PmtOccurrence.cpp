/*
 * Copyright (c) 2000-2009, Eastman Kodak Company
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification,are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice, 
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the 
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Eastman Kodak Company nor the names of its 
 *       contributors may be used to endorse or promote products derived from 
 *       this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * 
 * Creation Date: 04/11/2001
 *
 * Original Author: 
 * Dan Rupe  rupe@kodak.com
 * Copyright (C) 2001 Eastman Kodak Company. All Rights Reserved.
 * 
 * Contributor(s): 
 * Sam Fryer  <samuel.fryer@kodak.com>
 *
*/

#include <ctype.h>
#include "PmtOccurrence.h"
#include "stdlib.h"

#define PMT_OCCURRENCE_RANGE_CHAR '-'
#define PMT_OCCURRANCE_WILDCARD_CHAR '*'
#define PMT_OCCURRENCE_DELIMS " ,"


bool PmtOccurrence::parseKeyForOccurrences(PmtKeyType & key)
{
    bool returnValue = false;
    PmtKeyType::size_type pos;
    pos = key.find("[");
    if ((pos != PmtKeyType::npos) && (pos > 0))
    {
      // Perform some syntax checking for the occurrence specification.
      PmtKeyType::size_type pos2;
      pos2 = key.find("]");

      // If no ']' found; or ']' comes before '['; or nothing between '[' & ']'
      if ((pos2 != PmtKeyType::npos) && (pos2 > pos) && (pos2 != pos + 1))
      {
        // save off the occurrence
        SPmtString occurrenceString = key.substr(pos + 1, pos2 - pos - 1); 
        // strip occurrence specification off of key
        key = key.substr(0, pos);   

        // Now that we have the occurrence information separated, parse 
        // through it.
        returnValue = parseOccurrenceSpecification(occurrenceString);
      }
    }
    return returnValue;
}


bool PmtOccurrence::
parseOccurrenceSpecification(const SPmtString & occurrenceString)
{
    bool result = true;
    
    // If wildcard found
    if (occurrenceString == PMT_OCCURRANCE_WILDCARD_CHAR)
    {
        mIsUnspecifiedOccurrence = false;
        mIsWildcard = true;
        mIsExplicitlySpecifiedSingleOccurrence = false;
    }
    else
    {
        SPmtString::size_type pos;
        SPmtString::size_type pos2;
        SPmtString myString = occurrenceString;
        SPmtString tempString;

        // Loop through space and comma-seperated arguments.
        pos = myString.find_first_not_of(PMT_OCCURRENCE_DELIMS);
        while ((result) && (pos != SPmtString::npos))
        {
            // skip over leading delimeters
            myString = myString.substr(pos);    
            pos2 = myString.find_first_of(PMT_OCCURRENCE_DELIMS);
            if (pos2 != SPmtString::npos)
            {
                tempString = myString.substr(0, pos2);
                myString = myString.substr(pos2 + 1);   
                pos = myString.find_first_not_of(PMT_OCCURRENCE_DELIMS);
            }
            else
            {
                tempString = myString;  
                pos = pos2;
            }
                  
            result = parseArgument(tempString);
        }
    }

    return result;
}


bool PmtOccurrence::parseArgument(SPmtString argument)
{
    bool returnValue = true;
    
    // If range of occurrences
    SPmtString::size_type pos = argument.find(PMT_OCCURRENCE_RANGE_CHAR);
    if (pos == SPmtString::npos)
    {
        const char * num = argument.c_str();
        
        // Verify it's a number
        if (isNumber(num))
        {
            // Convert to number and put on our list for future reference.
            int result = atoi(num);

            addOccurrence(result);
        }
        else
            returnValue = false;
    }
    else
        returnValue = parseRange(argument,pos);

    return returnValue;
}


bool PmtOccurrence::parseRange(SPmtString argument,SPmtString::size_type pos)
{
    bool returnValue = true;
    SPmtString partOne = argument.substr(0, pos);
    SPmtString partTwo = argument.substr(pos + 1);

    if ((isNumber(partOne.c_str())) && (isNumber(partTwo.c_str())))
    {
        int numOne, numTwo;

        numOne = atoi(partOne.c_str());
        numTwo = atoi(partTwo.c_str());

        for (;numOne <= numTwo;numOne++)
        {
            addOccurrence(numOne);
        }
    }
    else
        returnValue = false;

    return returnValue;
}


bool PmtOccurrence::isNumber(const char *numberString)
{
    bool returnValue = true;
    const char * digit = numberString;
    for (unsigned int i = 0; returnValue && (i < strlen(numberString)); 
         digit++,i++)
    {
        if ( (*digit < '0') || (*digit > '9') )
            returnValue = false;
    }

    return returnValue;
}


void PmtOccurrence::addOccurrence(int occurrence)
{
    mOccurrenceList.push_back(occurrence);

    if (mOccurrenceList.size() == 1)
        mIsExplicitlySpecifiedSingleOccurrence = true;
    else
        mIsExplicitlySpecifiedSingleOccurrence = false;

    mIsUnspecifiedOccurrence = false;
}


int PmtOccurrence::start() const
{
    int returnValue = 0;
    if (!isUnspecifiedOccurrence())
    {
        mIndex = 0;
        returnValue = next();
    }
    return returnValue;
}


int PmtOccurrence::next() const
{
    int returnValue = OCCURRENCE_END;
    if (!isUnspecifiedOccurrence())
    {
        if ((mOccurrenceList.size() != 0)
            && (mIndex <= (mOccurrenceList.size() - 1)))
            returnValue = mOccurrenceList[mIndex++];
    }
    return returnValue;
}


void PmtOccurrence::show() const
{
    for (unsigned int counter = 0; counter < mOccurrenceList.size(); counter++)
        cout << "Occurrence[" << counter << "] = " << mOccurrenceList[counter]
            << endl;
}

