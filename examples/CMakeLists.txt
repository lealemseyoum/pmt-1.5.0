include_guard()

set(EXAMPLE_DIRS createExif createTiff DataList DataView IFDFinder PmtPcdCreator PullThumbnail)

foreach(loopVar IN LISTS EXAMPLE_DIRS)
    message(STATUS ${CMAKE_CURRENT_SOURCE_DIR}/${loopVar})    
    add_subdirectory(${loopVar})
endforeach()