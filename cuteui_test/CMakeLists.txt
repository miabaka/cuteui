project(cuteui_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(cuteui_test src/main.cpp)

target_link_libraries(cuteui_test PRIVATE cuteui)