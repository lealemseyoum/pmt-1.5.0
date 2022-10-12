
add_executable(accrTest AccessorTest.cpp)

target_link_libraries(accrTest PRIVATE  Openexif::Openexif 
                                        Opentiff::Opentiff 
                                        Ek
                                        PmtCore 
                                        PmtAccessor
                                        ${XercesC_LIBRARIES}
                                        pthread)

# use ctest
# ctest accr_test --verbose
add_test(
    NAME accr_test
    COMMAND accrTest 
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)