file(GLOB SOURCE_FILES *.c)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/unit_tests.c)

file(GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.h)

add_library(vkzos ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(vkzos PUBLIC ${Vulkan_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(vkzos PUBLIC ${Vulkan_LIBRARIES})

add_executable(unittests unit_tests.c)

target_include_directories(unittests PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(unittests PUBLIC vkzos)
