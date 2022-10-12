include_guard()

add_executable(pmtDataList DataList.cpp)

target_link_libraries(pmtDataList   PmtAccessor 
                                    Ek 
                                    PmtCore
                                    ${XercesC_LIBRARIES}
                                    ${Opentiff_LIBRARIES}
                                    ${Openexif_LIBRARIES})