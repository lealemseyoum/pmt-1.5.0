add_executable(nsTest NSTest.cpp)

target_link_libraries(nsTest PRIVATE    Ek 
                                        PmtCore 
                                        PmtAccessor 
                                        ${XercesC_LIBRARIES}
                                        pthread)

#use ctest
enable_testing()
add_test(
    NAME ns_test
    COMMAND nsTest
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)