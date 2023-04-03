cmake_minimum_required(VERSION 3.0.2)

project(vulkan_helper C)

option(VH_BUILD_TESTS "Build test program" ON)
option(VH_NO_NATIVE_INITS "Do not include native initialisation functions" OFF)

if(IOS AND DEFINED ANDROID_PLATFORM)
  message(FATAL_ERROR "Variables defined for both iOS and Android (IOS is set and ANDROID_PLATFORM exists)")
endif()

if(IOS)
  add_compile_definitions(VULKAN_HELPER_IOS)
endif()

if(VH_NO_NATIVE_INITS)
  add_compile_definitions(VULKAN_HELPER_NO_NATIVE)
endif()

file(COPY "include" DESTINATION ".")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

if(MSVC)
  add_definitions("-D_CRT_SECURE_NO_WARNINGS")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/bin")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/bin")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/lib")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/lib")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/lib")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/lib")
endif()

if(NOT IOS) # Linking to MoltenVK explicitly on iOS
find_package(Vulkan REQUIRED)
endif()

subdirs(src)
