cmake_minimum_required(VERSION 3.16)

project(SeregaMessenger VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)

set(QT_QML_GENERATE_QMLLS_INI ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.9.1)

set(SEREGA_HEADERS
    include/core/SeregaApp.hpp
    include/crypto/CryptoManager.hpp
    include/storage/SecureStorage.hpp
    include/api/Api.hpp
    include/api/AuthApi.hpp
    include/api/ChatsApi.hpp
    include/api/UpdatesApi.hpp
    include/api/UsersApi.hpp
    include/utils/Config.hpp
    include/utils/RequestHandler.hpp
)

set(SEREGA_SOURCES
    src/core/SeregaApp.cpp
    src/crypto/CryptoManager.cpp
    src/storage/SecureStorage.cpp
    src/api/Api.cpp
    src/api/AuthApi.cpp
    src/api/ChatsApi.cpp
    src/api/UpdatesApi.cpp
    src/api/UsersApi.cpp
    src/utils/RequestHandler.cpp
)

if(ANDROID)
    set(ANDROID_MANIFEST android/AndroidManifest.xml)
endif()

set_source_files_properties(qml/Navigation.qml
    PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_library(SeregaLib
    ${SEREGA_HEADERS}
    ${SEREGA_SOURCES}
)

qt_add_executable(Serega
    MANUAL_FINALIZATION

    src/main.cpp

    ${ANDROID_MANIFEST}
)

qt_add_qml_module(Serega
    URI SeregaMessenger
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/Navigation.qml
        qml/Login.qml
        qml/Chats.qml
        qml/ChatDelegate.qml
        qml/Avatar.qml
        qml/Chat.qml
        qml/Message.qml
)

if(ANDROID)
    include(libraries/android_openssl/android_openssl.cmake)

    add_android_openssl_libraries(Serega)
endif()

set(CONFIG_FILE "config/config.json")

qt_add_resources(Serega
    "config"
    PREFIX
        "/"
    FILES
        ${CONFIG_FILE}
)

qt_add_resources(Serega
    "icons"
    PREFIX
        "/"
    FILES
        resources/icons/profile-pic.png
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
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
)

set(SODIUM_DISABLE_TESTS ON)
set(BUILD_WITH_QT6 ON)

add_subdirectory(libraries/libsodium)
add_subdirectory(libraries/qtkeychain)

if(BUILD_TESTS)
    add_subdirectory(libraries/Catch2)
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

qt_finalize_executable(Serega)
