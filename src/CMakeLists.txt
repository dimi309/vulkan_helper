file(GLOB SOURCE_FILES *.c)
file(GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB TEST_SOURCE_AND_HEADER_FILES test/*.*)

add_library(vkzos ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(vkzos PUBLIC ${Vulkan_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(vkzos PUBLIC ${Vulkan_LIBRARIES})

if(MSVC)
  add_executable(test WIN32 ${TEST_SOURCE_AND_HEADER_FILES})
else()
  add_executable(test ${TEST_SOURCE_AND_HEADER_FILES})
endif()

target_include_directories(test PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(test PUBLIC vkzos)
