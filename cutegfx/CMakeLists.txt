project(cutegfx)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(cutegfx STATIC
        include/cutegfx/Device.hpp
        include/cutegfx/MeshBuilder.hpp
        include/cutegfx/Platform.hpp
        include/cutegfx/Renderer.hpp
        include/cutegfx/Viewport.hpp
        include/cutegfx/Window.hpp
        src/MeshBuilder.cpp
        src/Platform.cpp
        src/Renderer.cpp)

target_include_directories(cutegfx PUBLIC include)

target_link_libraries(cutegfx PUBLIC
        glm
        ctl)

if (WIN32)
    target_sources(cutegfx PRIVATE
            src/d3d11/Direct3D11Device.cpp
            src/d3d11/Direct3D11Device.hpp
            src/d3d11/Direct3D11Viewport.cpp
            src/d3d11/Direct3D11Viewport.hpp
            src/d3d11/shaders.hpp
            src/win32/Win32HelperWindow.cpp
            src/win32/Win32HelperWindow.hpp
            src/win32/Win32Platform.cpp
            src/win32/Win32Platform.hpp
            src/win32/Win32Window.cpp
            src/win32/Win32Window.hpp)

    target_link_libraries(cutegfx PRIVATE
            dwmapi
            d3d11
            dxgi
            dcomp)
else ()
    message(WARNING "Unsupported platform")
endif ()