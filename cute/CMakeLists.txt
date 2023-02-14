project(cute)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(cute STATIC
        src/Application.cpp
        src/Window.cpp)

target_include_directories(cute PUBLIC include)