cmake_minimum_required(VERSION 3.16)
project(MusicPlayerOS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS 
    Core 
    Gui 
    Widgets 
    Multimedia
    REQUIRED
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
    src/main.cpp
    src/app.cpp
    src/home_screen.cpp
    src/music_player_app.cpp
    src/touch_calibrator_app.cpp
    src/settings_app.cpp
    src/file_explorer_app.cpp
    src/sysinfo_app.cpp
    src/system.cpp
)

set(HEADERS
    include/app.h
    include/keyboard_ui.h
    include/system.h
    include/home_screen.h
    include/music_player_app.h
    include/touch_calibrator_app.h
    include/settings_app.h
    include/file_explorer_app.h
    include/sysinfo_app.h
)

add_executable(MusicPlayerOS ${SOURCES} ${HEADERS})

target_link_libraries(MusicPlayerOS
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Multimedia
)

# Install the executable to /usr/local/bin
install(TARGETS MusicPlayerOS DESTINATION /usr/local/bin)

# Install systemd service file
install(FILES MusicPlayerOS.service DESTINATION /etc/systemd/system)

# Print build info
message(STATUS "Music Player OS Build Configuration:")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  CXX Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "  Qt5 Version: ${Qt5_VERSION}")

