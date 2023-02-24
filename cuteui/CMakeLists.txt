project(cute)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(cuteui STATIC
        src/system/WindowManager.cpp
        src/Application.cpp
        src/Button.cpp
        src/Widget.cpp
        src/Window.cpp)

target_include_directories(cuteui PUBLIC include)

target_link_libraries(cuteui PUBLIC glm cutegfx)