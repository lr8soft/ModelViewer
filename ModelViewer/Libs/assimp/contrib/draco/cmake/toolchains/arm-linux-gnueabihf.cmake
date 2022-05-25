if(DRACO_CMAKE_TOOLCHAINS_ARM_LINUX_GNUEABIHF_CMAKE_)
  return()
endif() # DRACO_CMAKE_TOOLCHAINS_ARM_LINUX_GNUEABIHF_CMAKE_
set(DRACO_CMAKE_TOOLCHAINS_ARM_LINUX_GNUEABIHF_CMAKE_ 1)

set(CMAKE_SYSTEM_NAME "Linux")

if("${CROSS}" STREQUAL "")
  set(CROSS arm-linux-gnueabihf-)
endif()

set(CMAKE_CXX_COMPILER ${CROSS}g++)
set(CMAKE_CXX_FLAGS_INIT "-march=armv7-a -marm")
set(CMAKE_SYSTEM_PROCESSOR "armv7")
set(DRACO_NEON_INTRINSICS_FLAG "-mfpu=neon")
