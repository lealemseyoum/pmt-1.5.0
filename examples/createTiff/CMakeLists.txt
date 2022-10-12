include_guard()

add_executable(createTiff createTiff.cpp)

target_link_libraries(createTiff    PmtAccessor 
                                    Ek 
                                    PmtCore
                                    ${XercesC_LIBRARIES}
                                    ${Opentiff_LIBRARIES}
                                    ${Openexif_LIBRARIES})