qt_add_executable(tests
    TestCrypto.cpp
    TestApi.cpp
    main.cpp
)

target_link_libraries(tests
    PRIVATE Catch2 SeregaLib
)

target_include_directories(tests
    PRIVATE ../include
)

include(CTest)
include(Catch)

catch_discover_tests(tests)
