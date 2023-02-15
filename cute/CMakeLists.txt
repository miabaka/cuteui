project(cute)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(cute STATIC
        src/platform/Win32Platform.cpp
        src/platform/Win32PlatformWindow.cpp
        src/Application.cpp
        src/Box.cpp
        src/Button.cpp
        src/EventManager.cpp
        src/Spacer.cpp
        src/Window.cpp)

target_include_directories(cute PUBLIC include)