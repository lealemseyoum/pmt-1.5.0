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
 * Creation Date: 08/15/2000
 *
 * Original Author: 
 * Dan Rupe  rupe@kodak.com
 * 
 * Contributor(s): 
*/

#include <iostream>
#include "PmtMisc.h"
#include "EkStrings.h"
#include "PmtError.h"

#ifdef _MSC_VER
#include <direct.h>
#pragma warning (disable:4800)

#elif defined(macintosh)

// This should really check whether we're linking against the Metrowerks Standard Libraries
// or actual POSIX libraries. This could make a difference for MacOS X, which is built on
// a Unix core.

#include <stdio.h>
#include <stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <Navigation.h>
#include <XMLString.hpp>
#include <util/PlatformUtils.hpp>
#include <util/Platforms/MacOS/MacOSPlatformUtils.hpp>

// The Metrowerks standard libraries (MSL) don't implement the POSIX unlink function,
// and it would be very complicated to try and convert the POSIX style pathnames into
// MacOS file descriptors. As such, a copy file function is implemented natively to Pmt.
#define MACOSFILECOPYBUFFERSIZE    102400   // 100KB

#else                           // #else assumes Unix for now.

#include <stdio.h>

#endif



#ifdef _MSC_VER
bool PmtCopyFile(const char *fromPath, const char *toPath,
                 bool overwriteIfExists)
{
#ifdef _UNICODE
    wstring wfrom = EkAToW(fromPath);
    wstring wto = EkAToW(toPath);
    return CopyFile(wfrom.c_str(), wto.c_str(), !overwriteIfExists);
#else
    return CopyFile(fromPath, toPath, !overwriteIfExists);
#endif
}


#elif defined(macintosh)


bool PmtCopyFile(const char *fromPath, const char *toPath,
                 bool overwriteIfExists)
{
    char *buf;                  // Intermediate buffer for file data
    int fromFd, toFd;           // Input and output file descriptors
    int exclusive;              // Exclusive flag for opening output file
    long numRead;               // Number of bytes read from the source file
    // in the current chunk

    // If we're not supposed to overwrite the existing file, and the file already exists,
    // then return false.
    if (!overwriteIfExists)
    {
        if ((toFd = open(toPath, O_RDONLY)) == -1)
        {
            return false;
        }
    }

    // Allocate memory for the intermediate buffer
    if ((buf = (char *) malloc(MACOSFILECOPYBUFFERSIZE)) == NULL)
        return false;

    // Open the source and destination files
    if ((fromFd = open(fromPath, O_RDONLY)) == -1)
    {
        free(buf);
        return false;
    }

    // We already checked above, to make sure we will not incorrectly overwrite
    // the file, so here we can open it without the O_EXCL flag.
    if ((toFd = open(toPath, O_WRONLY | O_CREAT)) == -1)
    {
        free(buf);
        close(fromFd);
        return false;
    }

    // Read the input file, one buffer at a time, and write the data to the output file
    while (1)
    {                           // Try and read the next chunk
        if ((numRead = read(fromFd, buf, MACOSFILECOPYBUFFERSIZE)) < 1)
            break;

        // Write the data we just read, and break out on an error
        if ((numRead = write(toFd, buf, numRead)) == -1)
            break;
    }

    // Close the files and dispose of the buffer
    close(fromFd);
    close(toFd);
    free(buf);

    // If an error occurred (numRead = -1) return false, othewise, return true
    if (numRead == -1)
        return false;
    else
        return true;
}


#else                           // #else assumes Unix for now.


bool PmtCopyFile(const char *fromPath, const char *toPath,
                 bool overwriteIfExists)
{
    bool retVal = false;
    FILE *myFile1;
    FILE *myFile2;
    const int BUF_SIZE = 102400;

    // If we're not supposed to overwrite the existing file, and the file already exists,
    // then return false.
    if (!overwriteIfExists)
    {
        // Check for existance of file.
        myFile2 = PmtFOpen(toPath, "r");
        if (myFile2)
        {
            fclose(myFile2);
            return false;
        }
    }

    myFile1 = PmtFOpen(fromPath, "r");
    if (myFile1)
    {
        myFile2 = PmtFOpen(toPath, "w+");
        if (myFile2)
        {
            char *buf = new char[BUF_SIZE];
            if (buf)
            {
                size_t sizeRead;
                while ((sizeRead =
                        fread(buf, sizeof(char), BUF_SIZE - 1, myFile1)))
                {
                    fwrite(buf, sizeof(char), sizeRead, myFile2);
                }

                delete[]buf;
            }

            fclose(myFile2);

            retVal = true;
        }

        fclose(myFile1);
    }

    return retVal;
}


#endif


#if macintosh
void PmtChangePathStyleFromUnixToMac(SPmtString & filePath)
{
    // If the path starts with a relative directory (i.e. no / or .)
    // then place a colon in front, since colons are required to start
    // a relative path on the Mac.
    if ((filePath[0] != '/') && (filePath[0] != '.'))
        filePath = ':' + filePath;

    // Change any ./ at beginning of path to :
    if ((filePath[0] == '.') && (filePath[1] == '/'))
        filePath = ':' + filePath.substr(2);

    // Change any ../ at the beginning of path to ::
    if ((filePath[0] == '.') && (filePath[1] == '.') && (filePath[2] == '/'))
        filePath = "::" + filePath.substr(3);

    // Change ../ to :
    SPmtString tempPath;
    SPmtString::size_type pos = 0;
    while ((pos = filePath.find("../", pos)) != SPmtString::npos)
    {
        tempPath = filePath.substr(0, pos);
        tempPath += ":";
        tempPath += filePath.substr(pos + 3);
        filePath = tempPath;
    }

    // Change ./ to nothing
    pos = 0;
    while ((pos = filePath.find("./"), pos) != SPmtString::npos)
    {
        tempPath = filePath.substr(0, pos);
        tempPath += filePath.substr(pos + 2);
        filePath = tempPath;
    }

    // If the first character is a slash, remove it.  On the Mac,
    // we want absolute paths to start with the volumn id.
    if (filePath[0] == '/')
        filePath = filePath.substr(1);

    // Do a simple slash to colon conversion, to convert any slashes directly
    // between directory names and/or file names.
    pos = 0;
    while ((pos = filePath.find('/', pos)) != SPmtString::npos)
    {
        filePath.replace(pos, 1, ":");
        pos++;
    }
}
#endif


FILE *PmtFOpen(const char *filename, const char *mode)
{
    FILE *file;
    SPmtString stringFileName = filename;

#ifndef _MSC_VER
    stringFileName = PmtChangePathSlashes(stringFileName.c_str());
#endif

#if macintosh
    PmtChangePathStyleFromUnixToMac(stringFileName);
#endif

    file = fopen(stringFileName.c_str(), mode);

    return file;
}


bool PmtFileExists(const char *filename)
{
    FILE *file;
    file = PmtFOpen(filename, "rb");
    if (file == NULL)
    {
        return false;
    }
    else
    {
        fclose(file);
        return true;
    }
}


SPmtString PmtChangePathSlashes(const char *path)
{
    SPmtString filePath = path;

    // Change any backslashes to forward slashes if on non-Windows platform.
#ifndef _MSC_VER
    SPmtString::size_type pos = 0;
    while ((pos = filePath.find('\\', pos)) != SPmtString::npos)
    {
        filePath.replace(pos, 1, "/");
        pos++;
    }
#endif

    return filePath;
}


#if macintosh
static char *c2passtr(char *str)
{
    int len = strlen(str);
    if (len > 255)
        len = 255;

    for (int i = len; i >= 1; i--)
        str[i] = str[i - 1];

    str[0] = len;               /* Record the SPmtString length */
    return str;
}
#endif


bool PmtGetOSSpecificFilePathForXmlParser(SPmtString & filePath)
{
#ifndef _MSC_VER
    filePath = PmtChangePathSlashes(filePath.c_str());
#endif

#if macintosh
    FSSpec fsspec;
    short volRef = 0;
    long dirID = 0;
    OSErr osRetVal = noErr;

    PmtChangePathStyleFromUnixToMac(filePath);
    char tempBuf[MAX_PATH_LEN];
    strcpy(tempBuf, filePath.c_str());
    c2passtr(tempBuf);

    osRetVal =
        FSMakeFSSpec(volRef, dirID, (const unsigned char *) tempBuf, &fsspec);
    if (osRetVal != noErr)
        return false;

    XMLCh *myPath = XMLCreateFullPathFromFSSpec(fsspec);
    if (!myPath)
        return false;

    filePath = XMLString::transcode(myPath);
#endif

    return true;
}

