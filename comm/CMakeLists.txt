project (comm)

# Tools for using NK parser.
include (platform/nk)

# Set compile flags.
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wno-deprecated-declarations")

add_executable (Controller "src/controller.cpp" "src/subscriber.cpp")
add_dependencies (Controller echo_idl_files echo_controller_edl_files)

target_link_libraries(Controller 
                      ${mosquitto_CXX_LIB}
                      ${vfs_CLIENT_LIB}
)

set_target_properties (Controller PROPERTIES ${vfs_ENTITY}_REPLACEMENT "")
set_target_properties (Controller PROPERTIES LINK_FLAGS ${VULN_LDFLAGS})

