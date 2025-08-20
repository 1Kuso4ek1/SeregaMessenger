cmake_minimum_required(VERSION 3.16)

project(SeregaMessenger VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)

set(BUILD_SHARED_LIBS OFF)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.9.1)

qt_add_library(SeregaLib
    include/core/SeregaApp.hpp
    include/crypto/CryptoManager.hpp
    include/storage/CryptoStorage.hpp

    src/core/SeregaApp.cpp
    src/crypto/CryptoManager.cpp
    src/storage/CryptoStorage.cpp
)

qt_add_executable(Serega
    src/main.cpp
)

qt_add_qml_module(Serega
    URI SeregaMessenger
    VERSION 1.0
    QML_FILES
    qml/Main.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(Serega PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Serega
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

set(SODIUM_DISABLE_TESTS ON)
set(BUILD_WITH_QT6 ON)

add_subdirectory(libraries/libsodium)
add_subdirectory(libraries/qtkeychain)
add_subdirectory(libraries/Catch2)

message(INFO ${QTKEYCHAIN_TARGET_NAME})

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

target_link_libraries(SeregaLib
    PUBLIC Qt6::Core Qt6::Quick qt6keychain sodium
)
target_link_libraries(Serega
    PRIVATE SeregaLib
)

include_directories(include include/core)

include(GNUInstallDirs)
install(TARGETS Serega
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
