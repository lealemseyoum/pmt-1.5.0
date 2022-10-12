include_guard()

add_executable(createExif createExif.cpp)

target_link_libraries(createExif    PmtAccessor 
                                    Ek 
                                    PmtCore
                                    ${XercesC_LIBRARIES}
                                    ${Opentiff_LIBRARIES}
                                    ${Openexif_LIBRARIES})