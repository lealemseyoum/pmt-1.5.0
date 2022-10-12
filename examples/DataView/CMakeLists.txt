include_guard()

add_executable(pmtDataView DataView.cpp CViewImage.cpp)

target_link_libraries(pmtDataView   PmtAccessor 
                                    Ek 
                                    PmtCore
                                    ${XercesC_LIBRARIES}
                                    ${Opentiff_LIBRARIES}
                                    ${Openexif_LIBRARIES})