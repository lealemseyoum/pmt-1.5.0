unset(Openexif_FOUND)
unset(Openexif_INCLUDE_DIRS)
unset(Openexif_LIBRARIES)

find_path(Openexif_INCLUDE_DIRS NAMES 
    ExifImageFile.h
    PATHS
    /usr/include/openexif
    /usr/local/include/openexif 
    /opt/openexif/lib
    /home/lms/workspace/exif_ws/openexif-2_1_4-src/src)



find_path(Openexif_INCLUDE_DIRS2 NAMES 
    OpenExifJpegLib.h
    PATHS
    /usr/include/openexif
    /usr/local/include/openexif 
    /opt/openexif/lib
    /home/lms/workspace/exif_ws/openexif-2_1_4-src/src/OpenExifJpeg)


list(APPEND Openexif_INCLUDE_DIRS "${Openexif_INCLUDE_DIRS2}")

find_library(Openexif_LIBRARIES NAMES openexif
    PATHS 
    /usr/lib
    /usr/local/lib
    /opt/openexif/lib
    /home/lms/workspace/exif_ws/openexif-2_1_4-src/build)


if(Openexif_INCLUDE_DIRS AND Openexif_LIBRARIES)
    set(Openexif_FOUND 1)
endif(Openexif_INCLUDE_DIRS AND Openexif_LIBRARIES)