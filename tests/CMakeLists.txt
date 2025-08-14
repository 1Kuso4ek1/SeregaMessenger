enable_testing()

qt_add_executable(tests
    TestCrypto.cpp
)

target_link_libraries(tests
    PRIVATE Catch2WithMain SeregaLib
)

target_include_directories(tests
    PRIVATE ../include
)

include(CTest)
include(Catch)
catch_discover_tests(tests)
