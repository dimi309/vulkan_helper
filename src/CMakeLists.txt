file(GLOB SOURCE_FILES *.c)

file(GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.h)

file(GLOB TEST_SOURCE_AND_HEADER_FILES test/*.*)

add_library(vulkan_helper ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(vulkan_helper PUBLIC ${Vulkan_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)


target_link_libraries(vulkan_helper PUBLIC ${Vulkan_LIBRARIES})

if(VH_BUILD_TESTS)
  if(MSVC)
    add_executable(test WIN32 ${TEST_SOURCE_AND_HEADER_FILES})
  else()
    add_executable(test ${TEST_SOURCE_AND_HEADER_FILES})
  endif()

  target_include_directories(test PUBLIC ${CMAKE_SOURCE_DIR}/include)

  target_link_libraries(test PUBLIC vulkan_helper)
  
  if(UNIX)
    target_link_libraries(test PUBLIC xcb xcb-randr)
  endif()
  
endif()
