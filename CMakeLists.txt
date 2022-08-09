cmake_minimum_required(VERSION 3.8)
project(TEMPORARYNAME)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

set(SOURCE_FILES
	src/main.cpp
)

set(HEADER_FILES
	src/include/TEMPORARYNAME.h
	src/include/interface_vulkan.hpp
)

include_directories(src/include ${glfw3_INCLUDE_DIRS} )

add_executable(TEMPORARYNAME ${SOURCE_FILES})
add_library(TEMPORARYNAMELIB SHARED ${SOURCE_FILES})

set_target_properties(TEMPORARYNAMELIB PROPERTIES OUTPUT_NAME "TEMPORARYNAME")

target_link_libraries(TEMPORARYNAME ${Vulkan_LIBRARIES})
target_link_libraries(TEMPORARYNAME glfw3)

install(TARGETS TEMPORARYNAME 
		DESTINATION bin)
install(TARGETS TEMPORARYNAMELIB
		DESTINATION lib)
install(FILES ${HEADER_FILES} 
		DESTINATION include/TEMPORARYNAME)