project(cutegfx)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(cutegfx STATIC
        src/CuteGfx.cpp
        src/Direct3D11Device.cpp
        src/Direct3D11Viewport.cpp
        src/Win32HelperWindow.cpp
        src/Win32Platform.cpp
        src/Win32PlatformWindow.cpp)

target_include_directories(cutegfx PUBLIC include)

target_link_libraries(cutegfx
        PUBLIC glm cuteutil
        PRIVATE dwmapi d3d11 dxgi dcomp)