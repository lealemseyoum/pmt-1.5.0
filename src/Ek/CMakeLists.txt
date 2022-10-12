set(EK_SRCS     EkCompiler.cpp
                EkError.cpp
                EkMutex.cpp
                EkNullMutex.cpp 
                EkPrecompiled.cpp 
                EkRefCount.cpp 
                EkStrings.cpp 
                EkTemplate.cpp 
                ptstring.cpp )

add_library(Ek STATIC ${EK_SRCS})

target_include_directories(
    Ek PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/PmtCore>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/PmtUtils>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/PmtInterpreter>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/PmtTranslator>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/PmtAccessor>

    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>

)

# Install targets
install(
    TARGETS Ek
    EXPORT PmtTargets 
    INCLUDES 
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    ARCHIVE 
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install header files
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h*"
)
