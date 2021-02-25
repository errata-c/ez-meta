cmake_minimum_required(VERSION 3.15)

project(EZ_META
VERSION 1.0.0
LANGUAGES C CXX)

option(BUILD_TESTS "Build the library tests" ON)

include(GNUInstallDirs)
find_package(ez-cmake CONFIG REQUIRED)

# Find deps
find_package(glm CONFIG REQUIRED)

# Create main target.
add_library(ez-meta INTERFACE)
target_link_libraries(ez-meta INTERFACE glm)

# Add the include directory
target_include_directories(ez-meta INTERFACE 
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Select the c++ version to use.
target_compile_features(ez-meta INTERFACE cxx_std_17)
set_target_properties(ez-meta PROPERTIES EXPORT_NAME "meta")

# Build the tests once the option is enabled.
if(${BUILD_TESTS})
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif()


# Set the proper installation path
set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/ez-meta)


install(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
	TYPE INCLUDE
	FILES_MATCHING
	PATTERN "*.h" PATTERN "*.hpp"
)

install(TARGETS ez-meta
	EXPORT ez-meta-targets
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/$<CONFIG>"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
)

install_package(
	NAME "ez-meta"
	NAMESPACE "ez::"

	EXPORT "ez-meta-targets"
	VERSION "${CMAKE_PROJECT_VERSION}"

	# Version compatiblity
	COMPATIBILITY "SameMajorVersion"

	# Relative install location of the package config files.
	DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/ez-meta"

	# Pre and Post configuration files for the packages. Files are run as scripts when the package is found.
	PRECONFIG "${CMAKE_CURRENT_SOURCE_DIR}/preconfig.cmake"
	#POSTCONFIG "${CMAKE_CURRENT_SOURCE_DIR}/postconfig.cmake"

	# Only add this option if the library is header only, or an interface library.
	ARCH_INDEPENDENT
)