
AUTOMAKE_OPTIONS = foreign

if USE_IMAGE_ACCESSORS

if INCLUDE_KIESEXIF
ExifInclude = -I$(KIESEXIFROOT)/inc
ExifLib = -lexif
ExifLibPath = -L$(KIESEXIFROOT)/lib
JpegInclude = -I$(JPEGROOT)/inc
JpegLibPath = -L$(JPEGROOT)/lib
#Modern libtool has a problem with -ljpeg, so we must specify the full path
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

if USE_JPEG2000
JPEG2000Include = -I$(KAKADUROOT)/include -I$(KAKADUROOT)/inc
JPEG2000LibPath = -L$(KAKADUROOT)/lib
JPEG2000Lib = -lkdu_ek -lkdu_ek_jp2
else
JPEG2000Include =
JPEG2000LibPath =
JPEG2000Lib =
endif

if USE_PNG_ACCESSORS
PNGInclude = -I$(PNGROOT)/inc -I$(PNGROOT)/include
PNGLib = -lpng
else
PNGInclude = 
PNGLib =
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
	$(ExifInclude) $(TiffInclude) $(PNGInclude) $(DOMInclude) \
	$(JpegInclude) $(JPEG2000Include)

noinst_PROGRAMS = pmtDataView

pmtDataView_SOURCES = DataView.cpp CViewImage.cpp

pmtDataView_LDADD = $(top_srcdir)/src/libPmtAccessor.la \
		$(top_srcdir)/src/libPmtCore.la \
		$(top_srcdir)/src/libEk.la \
		$(ExifLibPath) $(TiffLibPath) \
		$(JpegLibPath) $(ExifLib) $(JpegLib) $(TiffLib) \
		$(PNGLib) $(DOMLib) \
		-L/usr/X11R6/lib -lXm -lXt -lX11 -lpthread \
		$(JPEG2000LibPath) $(JPEG2000Lib)

include_HEADERS = CViewImage.h

clean-compile:
	-rm -f $(OBJECTS)

endif







