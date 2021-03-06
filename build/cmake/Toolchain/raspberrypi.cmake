SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)

# Check parameters
if (NOT DEFINED ENV{RASPBERRY_PI_TOOLCHAIN})
	MESSAGE(FATAL_ERROR "RASPBERRY_PI_TOOLCHAIN is not set")
endif()
if (NOT DEFINED ENV{RASPBERRY_PI_ROOTFS})
	MESSAGE(FATAL_ERROR "RASPBERRY_PI_ROOTFS is not set")
endif()

# Configure toolchain
SET(CMAKE_C_COMPILER $ENV{RASPBERRY_PI_TOOLCHAIN}/bin/$ENV{RASPBERRY_PI_TOOLCHAIN_PREFIX}-gcc)
SET(CMAKE_CXX_COMPILER $ENV{RASPBERRY_PI_TOOLCHAIN}/bin/$ENV{RASPBERRY_PI_TOOLCHAIN_PREFIX}-g++)
SET(CMAKE_FIND_ROOT_PATH $ENV{RASPBERRY_PI_ROOTFS})
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /usr/lib/$ENV{RASPBERRY_PI_TOOLCHAIN_PREFIX})
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} /usr/include/$ENV{RASPBERRY_PI_TOOLCHAIN_PREFIX})
