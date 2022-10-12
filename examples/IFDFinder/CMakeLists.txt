include_guard()

add_executable(IFDFinder main.cpp)

target_link_libraries(IFDFinder     PmtAccessor 
                                    Ek 
                                    PmtCore
                                    ${XercesC_LIBRARIES}
                                    ${Opentiff_LIBRARIES}
                                    ${Openexif_LIBRARIES})