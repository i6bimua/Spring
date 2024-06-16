cmake_minimum_required(VERSION 3.5)

project(Gwidget VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS WebSockets)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS MultimediaWidgets)
set(PROJECT_SOURCES
        main.cpp
        gwidget.cpp
        gwidget.h
        gwidget.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_big_resources(PROJECT_SOURCES
        source.qrc
    )
    qt_add_executable(Gwidget
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        comp.h comp.cpp
        exer.h exer.cpp
        rank.h rank.cpp
        explanation.h explanation.cpp
        image.qrc
        tcpserver.h tcpserver.cpp tcpserver.ui
        tcpclient.h tcpclient.cpp tcpclient.ui

        pramodedialog.h pramodedialog.cpp pramodedialog.ui
        ico.rc
        pasword.h pasword.cpp

        getmap.h getmap.cpp getmap.ui
        dragbutton.h dragbutton.cpp
        getmap2.h getmap2.cpp getmap2.ui

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Gwidget APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Gwidget SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Gwidget
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Gwidget PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Gwidget PRIVATE Qt6::WebSockets)
target_link_libraries(Gwidget PRIVATE Qt6::Network)
target_link_libraries(Gwidget PRIVATE Qt6::Multimedia)
target_link_libraries(Gwidget PUBLIC Qt::MultimediaWidgets)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Gwidget)
endif()
set_target_properties(Gwidget PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Gwidget
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Gwidget)
endif()
