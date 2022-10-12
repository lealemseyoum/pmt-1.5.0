
set(PMT_CORE_SRCS   PmtAliases.cpp 
                    PmtAttribute.cpp 
                    PmtCompositeMetadata.cpp 
                    PmtElement.cpp 
                    PmtFacet.cpp 
                    PmtKeyParser.cpp 
                    PmtMetadata.cpp 
                    PmtModelGroup.cpp 
                    PmtOccurrence.cpp 
                    ../PmtInterpreter/PmtAliasesParser.cpp 
	                ../PmtInterpreter/PmtDefinitionInterpreter.cpp 
	                ../PmtInterpreter/PmtInstanceInterpreter.cpp 
	                ../PmtInterpreter/PmtSchemaInfoParser.cpp 
	                ../PmtInterpreter/PmtSchemaSAXHandler.cpp 
	                ../PmtInterpreter/PmtSchemaSAXParser.cpp 
	                ../PmtInterpreter/PmtXMLInstanceParser.cpp 
	                ../PmtUtils/PmtError.cpp 
	                ../PmtUtils/PmtMisc.cpp 
	                ../PmtAccessor/PmtTranslationTable.cpp 
	                ../PmtTranslator/PmtTranslator.cpp 
	                ../PmtAccessor/PmtAtom.cpp 
	                ../PmtAccessor/PmtFaceAtoms.cpp 
	                ../PmtAccessor/PmtFaceTranslator.cpp) 

add_library(PmtCore ${PMT_CORE_SRCS})

message(STATUS "Source dir is: ${CMAKE_SOURCE_DIR}")

target_include_directories(
    PmtCore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Ek>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/PmtUtils>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/PmtInterpreter>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/PmtTranslator>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/PmtAccessor>

    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Install targets
install(
    TARGETS PmtCore
    EXPORT PmtTargets
    INCLUDES 
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install header files
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h*"
)
