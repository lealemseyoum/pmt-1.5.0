
AUTOMAKE_OPTIONS = foreign

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
	$(DOMInclude) 

CPPFLAGS = -D BUILD_LOGICAL_DEFINITION_AND_LOGICAL_INSTANCE_TEST

EXTRA_DIST = Aliases.xml AllTypes.xml AllTypes.xsd AllTypesInfo.xml \
	Facet.xml Facet.xsd FacetInfo.xml GroundTrue.txt \
	MinMaxOccurs.xml MinMaxOccurs.xsd MinMaxOccursInfo.xml \
	Occurrences.xsd OccurrencesInfo.xml \
	PmtInterpreterTest.xml PmtMetadataSchemaInfo.xml \
	Ref.xsd RefInfo.xml SimpleType.xsd SimpleTypeInfo.xml \
	PrintQuantity.xsd PrintQuantityInfo.xml AttrFacet.xsd \
	AttrFacetInfo.xml CompTypeSimpleContent.xsd \
	CompTypeSimpleContentInfo.xml ModelGroups.xsd \
	ModelGroupsInfo.xml LocalInstanceDoc.xml ExifPersistAliases.xml \
	CompTypeComplexContent.xsd CompTypeComplexContentInfo.xml \
	AttFacet.xml IncludeInfo.xml Order.xsd Payment.xsd \
        PersonDetails.xsd Customer.xsd ImportInfo.xml \
        OrderImport.xsd Prod.xsd

noinst_PROGRAMS = interpTest

interpTest_SOURCES = PmtInterpreterTest.cpp PmtAPITestPmtCompMetadata.h \
	PmtAPITestPmtCompMetadata.cpp PmtAPITestPmtMetadata.h \
	PmtAPITestPmtMetadata.cpp PmtAPITestPmtMetadataT.h \
	PmtAPITestPmtMetadataT.cpp


interpTest_LDADD = $(top_srcdir)/src/libPmtCore.la $(top_srcdir)/src/libEk.la \
	$(DOMLib) -lpthread

clean-compile:
	-rm -f $(OBJECTS)


