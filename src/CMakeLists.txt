if(APPLE)
  file(GLOB SOURCE_FILES *.c *.m)
else()
  file(GLOB SOURCE_FILES *.c)
endif()


file(GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.h)
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

if(VULKAN_HELPER_TESTS)
  if(MSVC)
    add_executable(test WIN32 ${TEST_SOURCE_AND_HEADER_FILES})
  else()
    add_executable(test ${TEST_SOURCE_AND_HEADER_FILES})
  endif()

  target_include_directories(test PUBLIC ${CMAKE_SOURCE_DIR}/include)

  target_link_libraries(test PUBLIC vulkan_helper)
  
  if(APPLE) # MacOS
    set_target_properties(test PROPERTIES LINK_FLAGS "-framework QuartzCore")
  endif()
  
endif()
