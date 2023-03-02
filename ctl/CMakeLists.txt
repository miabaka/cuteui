project(ctl)

add_library(ctl INTERFACE
        include/ctl/memory/RcObject.hpp
        include/ctl/memory/RcPtr.hpp
        include/ctl/memory/WeakRcPtr.hpp
        include/ctl/Signal.hpp)

target_include_directories(ctl INTERFACE include)