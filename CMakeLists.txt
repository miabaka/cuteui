cmake_minimum_required(VERSION 3.20)

project(CuteUI)

add_subdirectory(third_party/glm)
add_subdirectory(third_party/stb)

add_subdirectory(ctl)
add_subdirectory(cutegfx)
add_subdirectory(cuteui)

add_subdirectory(cuteui_test)