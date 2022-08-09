cmake_minimum_required (VERSION 3.8)

project (TEMPORARYNAME)

find_package (glfw3 REQUIRED)
find_package (glm REQUIRED)
find_package (Vulkan REQUIRED)
find_package (tinyobjloader REQUIRED)

find_package (PkgConfig)
pkg_get_variable (STB_INCLUDEDIR stb includedir)
if (NOT STB_INCLUDEDIR)
  unset (STB_INCLUDEDIR)
  find_path (STB_INCLUDEDIR stb_image.h PATH_SUFFIXES stb)
endif ()
if (NOT STB_INCLUDEDIR)
  message (FATAL_ERROR "Unable to find the stb_image.h header!")
endif ()

set(SOURCE_FILES
	src/main.cpp
)

set(HEADER_FILES
  src/include/vulkan_interface.hpp
	src/include/TEMPORARYNAME.h
)

include_directories(src/include 
  ${glfw_INCLUDE_DIRS} 
  ${glm_INCLUDE_DIRS} 
  ${tinyobjloader_INCLUDE_DIRS}
)

add_executable(TEMPORARYNAME ${SOURCE_FILES})
add_library(TEMPORARYNAMELIB SHARED ${SOURCE_FILES})
set_target_properties(TEMPORARYNAMELIB PROPERTIES OUTPUT_NAME "TEMPORARYNAME")

target_link_libraries(TEMPORARYNAME 
  glfw3
  tinyobjloader
  ${Vulkan_LIBRARIES}
)

install(TARGETS TEMPORARYNAME 
		DESTINATION bin)
install(TARGETS TEMPORARYNAMELIB
		DESTINATION lib)
install(FILES ${HEADER_FILES} 
		DESTINATION include/TEMPORARYNAME)