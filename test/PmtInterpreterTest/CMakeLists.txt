add_executable(interpTest   PmtInterpreterTest.cpp 
                            PmtAPITestPmtMetadata.cpp 
                            PmtAPITestPmtMetadataT.cpp PmtAPITestPmtCompMetadata.cpp)

target_link_libraries(interpTest PRIVATE    Openexif::Openexif 
                                            Opentiff::Opentiff 
                                            Ek 
                                            PmtCore 
                                            PmtAccessor 
                                            ${XercesC_LIBRARIES}
                                            pthread)

target_include_directories(interpTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# use ctest
# ctest interp_test --verbose
add_test(
    NAME interp_test 
    COMMAND interpTest 
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)