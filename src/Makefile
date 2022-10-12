AUTOMAKE_OPTIONS = subdir-objects

if INCLUDE_KIESEXIF
ExifSupportSource = PmtAccessor/PmtExifAccessor.cpp
ExifSupportHeader = PmtAccessor/PmtExifAccessor.h
ExifInclude = -I$(KIESEXIFROOT)/inc
JpegInclude = -I$(JPEGROOT)/inc
else
if INCLUDE_OPENEXIF
ExifSupportSource = PmtAccessor/PmtExifAccessor.cpp
ExifSupportHeader = PmtAccessor/PmtExifAccessor.h
ExifInclude = -I$(EXIFROOT)/include
JpegInclude = 
else
ExifSupportSource = 
ExifSupportHeader =
ExifInclude =
JpegInclude =
endif
endif

if INCLUDE_OPENTIFF
TiffInclude = -I$(TIFFROOT)/inc -I$(TIFFROOT)/include
TiffSupportSource = PmtAccessor/PmtTiffAccessor.cpp
TiffSupportHeader = PmtAccessor/PmtTiffAccessor.h
else
if INCLUDE_EKTIFF
TiffInclude = -I$(TIFFROOT)/inc -I$(TIFFROOT)/include
TiffSupportSource = PmtAccessor/PmtTiffAccessor.cpp
TiffSupportHeader = PmtAccessor/PmtTiffAccessor.h
else
TiffInclude =
TiffSupportSource =
TiffSupportHeader =
endif
endif

if USE_IMAGE_ACCESSORS
ImageAccSupportSource = PmtAccessor/PmtImageRect.cpp
ImageAccSupportHeader = PmtAccessor/PmtImageAccessor.h PmtAccessor/PmtImageRect.h
if INCLUDE_OPENTIFF
TiffImageAccSupportSource = PmtAccessor/PmtTiffImageAccessor.cpp
TiffImageAccSupportHeader = PmtAccessor/PmtTiffImageAccessor.h
else
TiffImageAccSupportSouce =
TiffImageAccSupportHeader =
endif
if INCLUDE_OPENEXIF
ExifImageAccSupportSource = PmtAccessor/PmtExifImageAccessor.cpp
ExifImageAccSupportHeader = PmtAccessor/PmtExifImageAccessor.h
else
ExifImageAccSupportSouce =
ExifImageAccSupportHeader =
endif
else
TiffImageAccSupportSource = 
ExifImageAccSupportSource =
TiffImageAccSupportHeader =
ExifImageAccSupportHeader =
ImageAccSupportSource =
ImageAccSupportHeader =
endif

if USE_SOUND_ACCESSORS
SoundAccSupportHeader = PmtAccessor/PmtSoundAccessor.h
endif 

if USE_DIG35
DIG35SupportSource = PmtAccessor/PmtDIG35Accessor.cpp
DIG35SupportHeader = PmtAccessor/PmtDIG35Accessor.h
endif

if USE_JPEG2000
JPEG2000Include = -I$(KAKADUROOT)/include
JPEG2000SupportSource = PmtAccessor/PmtJ2KAccessor.cpp
JPEG2000SupportHeader = PmtAccessor/PmtJ2KAccessor.h
endif

if USE_PNG_ACCESSORS
PNGInclude = -I$(PNGROOT)/inc -I$(PNGROOT)/include
PNGSupportSource = PmtAccessor/PmtPngAccessor.cpp
PNGSupportHeader = PmtAccessor/PmtPngAccessor.h
endif

if USE_QT_ACCESSORS
QTSupportSource = \
	PmtAccessor/PmtQtAtoms.cpp \
	PmtAccessor/PmtQtSoundAccessor.cpp \
	PmtAccessor/PmtQtAccessor.cpp 
QTSupportHeader = \
	PmtAccessor/PmtQtAtoms.h \
	PmtAccessor/PmtQtAccessor.h 
endif

if USE_XMP_ACCESSORS
XMPSupportSource = \
	PmtAccessor/PmtXMPAccessor.cpp \
	PmtAccessor/PmtXMPSAXParser.cpp \
	PmtAccessor/XMLEncoder.cpp \
	PmtAccessor/XMPBlockManager.cpp \
	PmtAccessor/XMPDataNode.cpp \
	PmtAccessor/XMPNameSpaceManager.cpp \
	PmtAccessor/XMPSpecialMetadatas.cpp \
	PmtAccessor/XMPTypeTranslater.cpp 
XMPSupportHeader = \
	PmtAccessor/PmtXMPAccessor.h \
	PmtAccessor/PmtXMPSAXParser.h \
	PmtAccessor/XMPBlockManager.h \
	PmtAccessor/XMPDataNode.h \
	PmtAccessor/XMPNameSpaceManager.h \
	PmtAccessor/XMPSpecialMetadatas.h \
	PmtAccessor/XMPTypeTranslater.h \
	PmtAccessor/XMPCommonTags.h 
endif

if INCLUDE_XERCES_DOM
XMLInclude = -I$(XERCESCROOT)/include -I$(XERCESCROOT)/include/xercesc
else
if INCLUDE_GENERIC_DOM
XMLInclude = -I$(XERCESCROOT)/include
endif
endif

EXTRA_DIST = PmtAccessor/PmtExifAccessor.cpp PmtAccessor/PmtExifAccessor.h

INCLUDES = -I$(srcdir)/Ek -I$(srcdir)/PmtUtils \
	-I$(srcdir)/PmtCore -I$(srcdir)/PmtInterpreter \
	-I$(srcdir)/PmtTranslator -I$(srcdir)/PmtAccessor \
	$(ExifInclude) $(TiffInclude) $(PNGInclude) $(JPEG2000Include) \
	$(XMLInclude) \
	$(JpegInclude)

lib_LTLIBRARIES = libEk.la libPmtCore.la libPmtAccessor.la

libEk_la_SOURCES = Ek/EkCompiler.cpp \
	Ek/EkError.cpp \
	Ek/EkMutex.cpp \
	Ek/EkNullMutex.cpp \
	Ek/EkPrecompiled.cpp \
	Ek/EkRefCount.cpp \
	Ek/EkStrings.cpp \
	Ek/EkTemplate.cpp \
	Ek/ptstring.cpp 

libPmtCore_la_SOURCES = PmtCore/PmtAliases.cpp \
	PmtCore/PmtAttribute.cpp \
	PmtCore/PmtCompositeMetadata.cpp \
	PmtCore/PmtElement.cpp \
	PmtCore/PmtFacet.cpp \
	PmtCore/PmtKeyParser.cpp \
	PmtCore/PmtMetadata.cpp \
	PmtCore/PmtModelGroup.cpp \
	PmtCore/PmtOccurrence.cpp \
	PmtInterpreter/PmtAliasesParser.cpp \
	PmtInterpreter/PmtDefinitionInterpreter.cpp \
	PmtInterpreter/PmtInstanceInterpreter.cpp \
	PmtInterpreter/PmtSchemaInfoParser.cpp \
	PmtInterpreter/PmtSchemaSAXHandler.cpp \
	PmtInterpreter/PmtSchemaSAXParser.cpp \
	PmtInterpreter/PmtXMLInstanceParser.cpp \
	PmtUtils/PmtError.cpp \
	PmtUtils/PmtMisc.cpp \
	PmtAccessor/PmtTranslationTable.cpp \
	PmtTranslator/PmtTranslator.cpp \
	PmtAccessor/PmtAtom.cpp \
	PmtAccessor/PmtFaceAtoms.cpp \
	PmtAccessor/PmtFaceTranslator.cpp 


libPmtAccessor_la_SOURCES = PmtAccessor/PmtAccessor.cpp \
	PmtAccessor/PmtAccessorCreate.cpp \
	PmtAccessor/PmtFaceAtoms.cpp \
	$(ImageAccSupportSource) \
	$(ExifSupportSource) \
	$(ExifImageAccSupportSource) \
	$(TiffSupportSource) \
	$(TiffImageAccSupportSource) \
	$(PNGSupportSource) \
	$(QTSupportSource) \
	$(XMPSupportSource) \
	$(DIG35SupportSource) \
	$(JPEG2000SupportSource) \
	PmtAccessor/PmtXmlAccessor.cpp



include_HEADERS = Ek/EkBool.h \
	Ek/EkCompiler.h \
	Ek/EkError.h \
	Ek/EkFactoryT.h \
	Ek/EkGPtr.h \
	Ek/EkGuard.h \
	Ek/EkLocking.h \
	Ek/EkMapT.h \
	Ek/EkMutex.h \
	Ek/EkNullMutex.h \
	Ek/EkPrecompiled.h \
	Ek/EkRational.h \
	Ek/EkRefCount.h \
	Ek/EkCompSmartPtr.h \
	Ek/EkSmartPtr.h \
	Ek/EkStrings.h \
	Ek/EkTemplate.h \
	Ek/EkThread.h \
	Ek/ptstring.h \
	PmtCore/PmtAliases.h \
	PmtCore/PmtAttribute.h \
	PmtCore/PmtAttributeT.h \
	PmtCore/PmtCompositeMetadata.h \
	PmtCore/PmtConfig.h \
	PmtCore/PmtCoreDefs.h \
	PmtCore/PmtCoreCommon.h \
	PmtCore/PmtDoubleMdVisitorT.h \
	PmtCore/PmtElement.h \
	PmtCore/PmtFacet.h \
	PmtCore/PmtFacetT.h \
	PmtCore/PmtKeyParser.h \
	PmtCore/PmtMdVisitorImpl.h \
	PmtCore/PmtMetadata.h \
	PmtCore/PmtMetadataT.h \
	PmtCore/PmtMetadataVisitor.h \
	PmtCore/PmtModelGroup.h \
	PmtCore/PmtOccurrence.h \
	PmtCore/PmtRootMetadata.h \
	PmtCore/PmtSingleMdVisitorT.h \
	PmtInterpreter/PmtAliasesParser.h \
	PmtInterpreter/PmtDefaultPreparsedSchema.h \
	PmtInterpreter/PmtDefinitionInterpreter.h \
	PmtInterpreter/PmtDefinitionTypes.h \
	PmtInterpreter/PmtDomString.h \
	PmtInterpreter/PmtInstanceInterpreter.h \
	PmtInterpreter/PmtOrder.h \
	PmtInterpreter/PmtSAX2.h \
	PmtInterpreter/PmtSchemaInfoParser.h \
	PmtInterpreter/PmtSchemaSAXHandler.h \
	PmtInterpreter/PmtSchemaSAXParser.h \
	PmtInterpreter/PmtXMLInstanceParser.h \
	PmtUtils/PmtCommon.h  \
	PmtUtils/PmtError.h \
	PmtUtils/PmtIoFunctions.h \
	PmtUtils/PmtMisc.h   \
	PmtUtils/PmtVersionNumber.h \
	PmtAccessor/PmtAccessor.h \
	PmtAccessor/PmtAccrImplDefs.h \
	PmtAccessor/PmtAllAccessors.h \
	PmtAccessor/PmtAtom.h \
	$(ImageAccSupportHeader) \
	$(SoundAccSupportHeader) \
	$(ExifSupportHeader) \
	$(ExifImageAccSupportHeader) \
	$(TiffSupportHeader) \
	$(TiffImageAccSupportHeader) \
	$(PNGSupportHeader) \
	$(QTSupportHeader) \
	$(XMPSupportHeader) \
	$(DIG35SupportHeader) \
	$(JPEG2000SupportHeader) \
	PmtAccessor/PmtTranslationTable.h \
	PmtAccessor/PmtXmlAccessor.h \
	PmtTranslator/PmtTranslationTbl.h \
	PmtTranslator/PmtTranslator.h

clean-compile:
	-rm -f $(OBJECTS)









