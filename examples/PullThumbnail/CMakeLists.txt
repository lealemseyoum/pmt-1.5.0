include_guard()

add_executable(pullThumb pullThumb.cpp)

target_link_libraries(pullThumb    PmtAccessor 
                                    Ek 
                                    PmtCore
                                    ${XercesC_LIBRARIES}
                                    ${Opentiff_LIBRARIES}
                                    ${Openexif_LIBRARIES})