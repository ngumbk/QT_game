cmake_minimum_required(VERSION 3.5)
 
project(CityCity_Guess VERSION 0.1 LANGUAGES CXX)
 
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
 
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
set(CMAKE_PREFIX_PATH "D:/Qt/6.2.2/msvc2019_64")
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Sql)
 
 
set(PROJECT_SOURCES
      main.cpp
      mainwindow.h
      mainwindow.cpp
      mainwindow.ui
      scoreboard.h
      scoreboard.cpp
      scoreboard.ui
      gamewindow.h
      gamewindow.cpp
      gamewindow.ui
      gameover.h
      gameover.cpp
      gameover.ui
      pics.qrc
      src.qrc
      )
 
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/CityGuessDB.db 
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
 
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CityCity_Guess
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(CityCity_Guess
        ${PROJECT_SOURCES}
    )
endif()
 
target_link_libraries(CityCity_Guess PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::Sql)
 
set_target_properties(CityCity_Guess PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)
 
install(TARGETS
            CityCity_Guess
        DESTINATION
            .
)
 
get_property(QT_CORE_INCLUDE_DIRS TARGET Qt${QT_VERSION_MAJOR}::Core PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
list(GET QT_CORE_INCLUDE_DIRS 0 QT_CORE_MAIN_INCLUDE_DIR)
get_filename_component(QT_MAIN_DIR ${QT_CORE_MAIN_INCLUDE_DIR}/../.. ABSOLUTE)
 
install(FILES $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Core>
              $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Gui>
              $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Widgets>
              $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Sql>
              DESTINATION .)
 
install(DIRECTORY ${QT_MAIN_DIR}/plugins/platforms/
                  DESTINATION ./platforms USE_SOURCE_PERMISSIONS)
 
install(DIRECTORY
    ${QT_MAIN_DIR}/plugins/sqldrivers/
        DESTINATION ./sqldrivers USE_SOURCE_PERMISSIONS)
 
install(FILES CityGuessDB.db DESTINATION .)
 
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CityCity_Guess)
endif()
 
