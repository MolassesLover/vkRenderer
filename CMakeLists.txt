cmake_minimum_required(VERSION 3.8)
project(TEMPORARYNAME)

include(FindX11)

find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)

set(SOURCE_FILES
	src/main.cpp
	src/rendering.cpp
	src/select_gpu.cpp
)

set(HEADER_FILES
	src/include/rendering.h
	src/include/select_gpu.h
	src/include/TEMPORARYNAME.h
)

include_directories(src/include ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

add_executable(TEMPORARYNAME ${SOURCE_FILES})
add_library(TEMPORARYNAMELIB SHARED ${SOURCE_FILES})

set_target_properties(TEMPORARYNAMELIB PROPERTIES OUTPUT_NAME "TEMPORARYNAME")

target_link_libraries(TEMPORARYNAME ${Vulkan_LIBRARIES})
target_link_libraries(TEMPORARYNAME ${SDL2_LIBRARIES})
target_link_libraries(TEMPORARYNAME ${X11_LIBRARIES})

install(TARGETS TEMPORARYNAME 
		DESTINATION bin)
install(TARGETS TEMPORARYNAMELIB
		DESTINATION lib)
install(FILES ${HEADER_FILES} 
		DESTINATION include/TEMPORARYNAME)