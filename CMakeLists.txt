cmake_minimum_required(VERSION 3.10)
project(pmt VERSION 1.5.0)

# where to find dependencies
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

list(APPEND CMAKE_PREFIX_PATH "/opt/")
set(CMAKE_INSTALL_PREFIX "/opt/pmt")

add_compile_options(-Wno-write-strings -Wno-deprecated)


find_package(Openexif)
find_package(Opentiff)
find_package(XercesC)
#find_package(ICU COMPONENTS uc data)
find_package(Threads)


if(Openexif_FOUND)
	message("Openexif IS FOUND")
endif()

if(Opentiff_FOUND)
	message("Opentiff IS FOUND")
endif()

if(XercesC_FOUND)
	message("Xerces IS FOUND")
	message(STATUS "XERCESC FOUND AT ${XercesC_LIBRARIES}")
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CMakePrintHelpers)

option(USE_OPENEXIF "OpenExif" ON)

option(USE_OPENTIFF "OpenTiff" ON)

option(USE_XERCES "Xerces" ON)

option(USE_IMAGE_ACCESSORS "Image accessors" ON)

option(USE_DIG35 "Enable DIG35 standard" ON)

option(USE_JPEG2000 "Use JPEG2000 standard" ON)

option(USE_PNG_ACCESSORS "Enable PNG file access" ON)

option(USE_QT_ACCESSORS "Enable QT accessors" ON)

option(USE_XMP_ACCESSORS "Enable XML accessors" ON)


add_subdirectory(src)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
	add_subdirectory(test)
endif()

option(BUILD_EXAMPLES "Build examples " ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


# Generate and install package configuration files
configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/PmtConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/PmtConfig.cmake"
    INSTALL_DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/Pmt
)

write_basic_package_version_file(
    "PmtConfigVersion.cmake"
    COMPATIBILITY  SameMajorVersion
)

install(
    FILES 
        ${CMAKE_CURRENT_BINARY_DIR}/PmtConfig.cmake 
        ${CMAKE_CURRENT_BINARY_DIR}/PmtConfigVersion.cmake 
    DESTINATION 
        ${CMAKE_INSTALL_LIBDIR}/cmake/Pmt
)

# Install package configuration files
install(
    EXPORT PmtTargets
    FILE PmtTargets.cmake
    NAMESPACE Pmt::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Pmt
)


#[=[
Export Targets from the Build Tree:
In some cases, it is desirable to export targets directly from a build tree. The targets may then be used by an outside project that references the build tree with no installation involved. 

If we want our project to also be used from a build directory we only have to add the following. 
]=]

export(
    EXPORT PmtTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/PmtTargets.cmake"
    NAMESPACE Pmt::
)

