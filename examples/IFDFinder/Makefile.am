
AUTOMAKE_OPTIONS = foreign

if INCLUDE_KIESEXIF
ExifInclude = -I$(KIESEXIFROOT)/inc
ExifLib = -lexif
ExifLibPath = -L$(KIESEXIFROOT)/lib
JpegInclude = -I$(JPEGROOT)/inc
JpegLibPath = -L$(JPEGROOT)/lib
#Modern libtool has a problem with -ljpeg, so we have to include the full path.
JpegLib = $(JPEGROOT)/lib/libekjpeg.a
else
if INCLUDE_OPENEXIF
ExifInclude = -I$(EXIFROOT)/include
ExifLib = -lopenexif
ExifLibPath = -L$(EXIFROOT)/lib
JpegInclude = 
JpegLibPath = 
JpegLib = 
else
ExifInclude =
ExifLib =
ExifLibPath =
JpegInclude =
JpegLibPath =
JpegLib =
endif
endif

if INCLUDE_OPENTIFF
TiffInclude = -I$(TIFFROOT)/inc -I$(TIFFROOT)/include
TiffLibPath = -L$(TIFFROOT)/lib
TiffLib = -lopentiff
else
if INCLUDE_EKTIFF
TiffInclude = -I$(TIFFROOT)/inc -I$(TIFFROOT)/include
TiffLibPath = -L$(TIFFROOT)/lib
TiffLib = -lektiff
else
TiffInclude =
TiffLibPath =
TiffLib =
endif
endif

if INCLUDE_GENERIC_DOM
DOMLib = -L$(XERCESCROOT)/lib -lxerces-c
DOMInclude = -I$(XERCESCROOT)/include -I$(XERCESCROOT)/include/xercesc
else
if INCLUDE_XERCES_DOM
DOMLib = -L$(XERCESCROOT)/lib -lxerces-c
DOMInclude = -I$(XERCESCROOT)/include -I$(XERCESCROOT)/include/xercesc
endif
endif

INCLUDES = -I$(top_srcdir)/src/Ek -I$(top_srcdir)/src/PmtUtils \
	-I$(top_srcdir)/src/PmtCore -I$(top_srcdir)/src/PmtInterpreter \
	-I$(top_srcdir)/src/PmtAccessor \
	$(ExifInclude) $(TiffInclude) $(PNGInclude) $(DOMInclude) $(JpegInclude) 

noinst_PROGRAMS = IFDFinder

IFDFinder_SOURCES = main.cpp

IFDFinder_LDADD = $(top_srcdir)/src/libPmtAccessor.la \
		$(top_srcdir)/src/libPmtCore.la \
		$(top_srcdir)/src/libEk.la \
		$(ExifLibPath) $(TiffLibPath) $(JpegLibPath) \
		$(ExifLib) $(JpegLib) $(TiffLib) $(DOMLib)

include_HEADERS = IFDFinder.h

clean-compile:
	-rm -f $(OBJECTS)









