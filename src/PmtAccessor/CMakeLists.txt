set(PMT_ACCESSOR_SRCS   PmtAccessor.cpp
	                    PmtAccessorCreate.cpp
	                    PmtFaceAtoms.cpp
	                    PmtXmlAccessor.cpp )

IF(USE_OPENEXIF)
    list(APPEND PMT_ACCESSOR_SRCS PmtExifAccessor.cpp)
endif()

if(USE_OPENTIFF)
    list(APPEND PMT_ACCESSOR_SRCS PmtTiffAccessor.cpp)
endif()

if(USE_IMAGE_ACCESSORS)
    list(APPEND PMT_ACCESSOR_SRCS PmtImageRect.cpp)
    if(USE_OPENTIFF)
        list(APPEND PMT_ACCESSOR_SRCS PmtTiffImageAccessor.cpp)
    endif(USE_OPENTIFF)
    if(USE_OPENEXIF)
        list(APPEND PMT_ACCESSOR_SRCS PmtExifImageAccessor.cpp)
    endif(USE_OPENEXIF)
endif(USE_IMAGE_ACCESSORS)


if(USE_DIG35)
    list(APPEND PMT_ACCESSOR_SRCS PmtDIG35Accessor.cpp)
endif(USE_DIG35)

if(USE_JPEG2000)
    list(APPEND PMT_ACCESSOR_SRCS PmtJ2KAccessor.cpp)
endif(USE_JPEG2000)

if(USE_PNG_ACCESSORS)
    list(APPEND PMT_ACCESSOR_SRCS PmtPngAccessor.cpp)
endif(USE_PNG_ACCESSORS)

if(USE_QT_ACCESSORS)
    list(APPEND PMT_ACCESSOR_SRCS   PmtQtAtoms.cpp 
                                    PmtQtSoundAccessor.cpp
                                    PmtQtAccessor.cpp)
endif(USE_QT_ACCESSORS)

if(USE_XMP_ACCESSORS)
    list(APPEND PMT_ACCESSOR_SRCS   PmtXMPAccessor.cpp
                                    PmtXMPSAXParser.cpp 
                                    XMLEncoder.cpp 
                                    XMPBlockManager.cpp 
                                    XMPDataNode.cpp 
                                    XMPNameSpaceManager.cpp 
                                    XMPSpecialMetadatas.cpp 
                                    XMPTypeTranslater.cpp)
endif(USE_XMP_ACCESSORS)

add_library(PmtAccessor STATIC ${PMT_ACCESSOR_SRCS})

target_link_libraries( PmtAccessor  PmtCore 
                                    Ek
                                    Openexif::Openexif 
                                    Opentiff::Opentiff
                                    XercesC::XercesC)

target_include_directories(
    PmtAccessor PUBLIC  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>   
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/Ek>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/PmtCore>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/PmtUtils>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/PmtInterpreter>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/PmtTranslator>
                
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)           

# Install targets
install(
    TARGETS PmtAccessor
    EXPORT PmtTargets
    INCLUDES 
        DESTIANTION ${CMAKE_INSTALL_INCLUDEDIR})

# Install header files
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h*")
