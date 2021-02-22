cmake_minimum_required(VERSION 3.15)

project(ez-template
VERSION 1.0.0
LANGUAGES C CXX)

include(GNUInstallDirs)
find_package(ez-cmake CONFIG REQUIRED)

# Find deps
#find_package(glm CONFIG REQUIRED)

# Create main target.
add_library(${CMAKE_PROJECT_NAME} INTERFACE
# Add source files
#"src/IntersectFunctions.cpp"
)

# Add the include directory
target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE 
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Select the c++ version to use.
target_compile_features(${CMAKE_PROJECT_NAME} INTERFACE cxx_std_17)

# Build the tests once the option is enabled.
if(${BUILD_TESTS})
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif()


# Set the proper installation path
set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/${CMAKE_PROJECT_NAME})


install(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
	TYPE INCLUDE
	FILES_MATCHING
	PATTERN "*.h" PATTERN "*.hpp"
)

install(TARGETS ${CMAKE_PROJECT_NAME}
	EXPORT ${CMAKE_PROJECT_NAME}-targets
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/$<CONFIG>"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
)

install_package(
	NAME "${CMAKE_PROJECT_NAME}"
	EXPORT "${CMAKE_PROJECT_NAME}-targets"
	VERSION "${CMAKE_PROJECT_VERSION}"

	# Version compatiblity
	COMPATIBILITY "SameMajorVersion"

	# Relative install location of the package config files.
	DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${CMAKE_PROJECT_NAME}"

	# Pre and Post configuration files for the packages. Files are run as scripts when the package is found.
	#PRECONFIG "${CMAKE_CURRENT_SOURCE_DIR}/preconfig.cmake"
	#POSTCONFIG "${CMAKE_CURRENT_SOURCE_DIR}/postconfig.cmake"

	# Only add this option if the library is header only, or an interface library.
	ARCH_INDEPENDENT
)