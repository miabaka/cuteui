project(cutegfx)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(cutegfx STATIC
        include/cutegfx/Buffer.hpp
        include/cutegfx/Device.hpp
        include/cutegfx/MeshBuilder.hpp
        include/cutegfx/Platform.hpp
        include/cutegfx/Renderer.hpp
        include/cutegfx/Shader.hpp
        include/cutegfx/Viewport.hpp
        include/cutegfx/Window.hpp
        src/MeshBuilder.cpp
        src/Platform.cpp
        src/Renderer.cpp)

target_include_directories(cutegfx PUBLIC include)

target_link_libraries(cutegfx PUBLIC
        glm
        cuteutil)

if (WIN32)
    target_sources(cutegfx PRIVATE
            src/impl/Direct3D11Buffer.cpp
            src/impl/Direct3D11Buffer.hpp
            src/impl/Direct3D11Device.cpp
            src/impl/Direct3D11Device.hpp
            src/impl/Direct3D11Shader.cpp
            src/impl/Direct3D11Shader.hpp
            src/impl/Direct3D11Viewport.cpp
            src/impl/Direct3D11Viewport.hpp
            src/impl/Win32HelperWindow.cpp
            src/impl/Win32HelperWindow.hpp
            src/impl/Win32Platform.cpp
            src/impl/Win32Platform.hpp
            src/impl/Win32Window.cpp
            src/impl/Win32Window.hpp)

    target_link_libraries(cutegfx PRIVATE
            dwmapi
            d3d11
            dxgi
            dcomp)
else ()
    message(WARNING "Unsupported platform")
endif ()