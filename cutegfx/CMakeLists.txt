project(cutegfx)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(cutegfx STATIC
        src/MeshBuilder.cpp
        src/Platform.cpp
        src/Renderer.cpp)

target_include_directories(cutegfx PUBLIC include)

target_link_libraries(cutegfx PUBLIC
        glm
        cuteutil)

if (WIN32)
    target_sources(cutegfx PRIVATE
            src/impl/Direct3D11Device.cpp
            src/impl/Direct3D11Viewport.cpp
            src/impl/Win32HelperWindow.cpp
            src/impl/Win32Platform.cpp
            src/impl/Win32Window.cpp)

    target_link_libraries(cutegfx PRIVATE
            dwmapi
            d3d11
            dxgi
            dcomp)
else ()
    message(WARNING "Unsupported platform")
endif ()