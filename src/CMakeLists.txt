if(APPLE AND NOT DEFINED PLATFORM)
  file(GLOB SOURCE_FILES *.c *.m)
else()
  file(GLOB SOURCE_FILES *.c)
endif()

file(GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.h)

if(NOT DEFINED ANDROID_PLATFORM)
  list(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/vulkan_helper_android.c)
  list(REMOVE_ITEM HEADER_FILES ${CMAKE_SOURCE_DIR}/include/vulkan_helper_android.h)
endif()

if(NOT APPLE OR DEFINED PLATFORM)
  list(REMOVE_ITEM HEADER_FILES ${CMAKE_SOURCE_DIR}/include/vulkan_helper_macos.h)
endif()

file(GLOB TEST_SOURCE_AND_HEADER_FILES test/*.*)

add_library(vulkan_helper ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(vulkan_helper PUBLIC ${Vulkan_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include ${Vulkan_INCLUDE_DIRS}/../../MoltenVK/include)

if(NOT DEFINED ANDROID_PLATFORM)
  target_link_libraries(vulkan_helper PUBLIC ${Vulkan_LIBRARIES})
else()
  add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
  target_include_directories(native_app_glue PUBLIC
    ${ANDROID_NDK}/sources/android/native_app_glue)

  target_link_libraries(vulkan_helper
    android
    native_app_glue)
endif()

if(VH_BUILD_TESTS)
  if(MSVC)
    add_executable(test WIN32 ${TEST_SOURCE_AND_HEADER_FILES})
  else()
    add_executable(test ${TEST_SOURCE_AND_HEADER_FILES})
  endif()

  target_include_directories(test PUBLIC ${CMAKE_SOURCE_DIR}/include)

  target_link_libraries(test PUBLIC vulkan_helper)
  
  if(UNIX AND NOT APPLE)
    target_link_libraries(test PUBLIC xcb xcb-randr)
  endif()
  
  if(APPLE) # MacOS
    set_target_properties(test PROPERTIES LINK_FLAGS "-framework QuartzCore")
  endif()
  
endif()
