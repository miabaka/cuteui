project(stb)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(stb_image STATIC
        include/stb/image.hpp
        src/image.h
        src/image.cpp)

target_include_directories(stb_image PUBLIC include)
target_link_libraries(stb_image PUBLIC glm)