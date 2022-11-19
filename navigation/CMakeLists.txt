project (navigation)


# Tools for using NK parser.
include (platform/nk)

# Set compile flags.
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wno-deprecated-declarations")

add_executable (NavigationController "src/controller.cpp")
add_dependencies (NavigationController echo_nav_idl_files echo_nav_edl_files)
