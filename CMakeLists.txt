cmake_minimum_required (VERSION 3.12)

project (echo)

# Initialize CMake library for KasperskyOS SDK.
include (platform)
initialize_platform ()

include (platform/install)

# Tools for using NK parser.
include (platform/nk)

nk_build_idl_files (echo_idl_files NK_MODULE "echo" IDL "resources/configuration/Configuration.idl")
nk_build_cdl_files (echo_cdl_files IDL_TARGET echo_idl_files NK_MODULE "echo" CDL "resources/configuration/Configuration.cdl")
nk_build_edl_files (echo_server_edl_files CDL_TARGET echo_cdl_files NK_MODULE "echo" EDL "resources/configuration/ConfigurationServer.edl")

nk_build_edl_files (echo_controller_edl_files NK_MODULE "echo" EDL "resources/comm_controller/Client.edl")

nk_build_idl_files (echo_nav_idl_files NK_MODULE "echo" IDL "resources/navigation/NavigationCommand.idl")
nk_build_cdl_files (echo_nav_cdl_files IDL_TARGET echo_nav_idl_files NK_MODULE "echo" CDL "resources/navigation/NavigationCommand.cdl")
nk_build_edl_files (echo_nav_edl_files CDL_TARGET echo_nav_cdl_files NK_MODULE "echo" EDL "resources/navigation/NavigationController.edl")


find_package (mosquitto REQUIRED COMPONENTS CXX_LIB)
include_directories (${mosquitto_INCLUDE})

# Add a package for working with the virtual file system.
find_package (vfs REQUIRED)
include_directories (${vfs_INCLUDE})
# Add a package with the dhcpcd program implementation.
find_package (rump REQUIRED COMPONENTS DHCPCD_ENTITY)
include_directories (${rump_INCLUDE})

# Add a package with the VFS program implementations
find_package (precompiled_vfs REQUIRED)
include_directories (${precompiled_vfs_INCLUDE})

# (BSP_HW_ENTITY, BSP_QEMU_ENTITY and BSP_CLIENT_LIB)
find_package (bsp REQUIRED)
include_directories (${bsp_INCLUDE})

include_directories ("communication/deps/rapidjson/include")

find_package (gpio REQUIRED COMPONENTS CLIENT_LIB ENTITY)
include_directories (${gpio_INCLUDE})

# Choose the storage devices for the configuration files.
# If none of the options are selected, the ramfs device is used.
option (ROOTFS_SDCARD "Use SD card to mount root filesystem" OFF)
message (STATUS "Use SD card to mount root filesystem - ${ROOTFS_SDCARD}")

# Attach the directory with the generated config header files
include_directories (${CMAKE_CURRENT_BINARY_DIR})

# Attach the directories with programs.
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/resources/include)

# Recommended flags against vulnerabilities
set(VULN_LDFLAGS "\
    -Wl,-z,noexecstack \
    -Wl,-z,separate-code \
    -Wl,-z,now")
set(VULN_COMPILER_FLAGS "\
    -Wall -Wextra -Wconversion \
    -fPIE -pie -D_FORTIFY_SOURCE=2 -O2 \
    -fstack-protector-strong -fstack-clash-protection \
    -mbranch-protection=standard \
    -Wsign-conversion -Wformat=2 -Wformat-security -Werror=format-security \
    -fsanitize=undefined -fsanitize-undefined-trap-on-error")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VULN_COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VULN_COMPILER_FLAGS}")

add_subdirectory (configuration_server)
add_subdirectory (navigation)
add_subdirectory (communication)
add_subdirectory (einit)
