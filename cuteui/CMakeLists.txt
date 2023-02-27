project(cute)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(cuteui STATIC
        include/cuteui/system/WindowManager.hpp
        include/cuteui/Application.hpp
        include/cuteui/Box.hpp
        include/cuteui/Button.hpp
        include/cuteui/Grid.hpp
        include/cuteui/LayerBox.hpp
        include/cuteui/Spacer.hpp
        include/cuteui/Widget.hpp
        include/cuteui/Window.hpp
        src/system/WindowManager.cpp
        src/Application.cpp
        src/Box.cpp
        src/Button.cpp
        src/Widget.cpp
        src/Window.cpp)

target_include_directories(cuteui PUBLIC include)

target_link_libraries(cuteui PUBLIC glm cutegfx)