project(cutegfx)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(cutegfx STATIC
        src/Platform.cpp
        src/Renderer.cpp)

target_include_directories(cutegfx PUBLIC include)

target_link_libraries(cutegfx PUBLIC
        glm
        cuteutil)

if (WIN32)
    target_sources(cutegfx PRIVATE
            src/Direct3D11Device.cpp
            src/Direct3D11Viewport.cpp
            src/Win32HelperWindow.cpp
            src/Win32Platform.cpp
            src/Win32Window.cpp)

    target_link_libraries(cutegfx PRIVATE
            dwmapi
            d3d11
            dxgi
            dcomp)
else ()
    message(WARNING "Unsupported platform")
endif ()