unset(Opentiff_FOUND)
unset(Opentiff_INCLUDE_DIRS)
unset(Opentiff_LIBRARIES)

find_path(Opentiff_INCLUDE_DIRS NAMES 
    TiffImageFile.h
    PATHS
    /usr/include/Opentiff
    /usr/local/include/Opentiff 
    /opt/Opentiff/include
    /home/lms/workspace/exif_ws/opentiff-1_3_3-src/inc)



find_library(Opentiff_LIBRARIES NAMES opentiff
    PATHS 
    /usr/lib
    /usr/local/lib
    /opt/Opentiff/lib
    /home/lms/workspace/exif_ws/opentiff-1_3_3-src/build)


if(Opentiff_INCLUDE_DIRS AND Opentiff_LIBRARIES)
    set(Opentiff_FOUND 1)
endif(Opentiff_INCLUDE_DIRS AND Opentiff_LIBRARIES)