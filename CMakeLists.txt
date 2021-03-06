cmake_minimum_required(VERSION 3.15)

project(EZ_META
VERSION 1.0.1
LANGUAGES C CXX)

include(GNUInstallDirs)
include("cmake/ez-cmake/install_package.cmake")

option(BUILD_TESTS "Build the test executables" ON)
option(SEPARATE_DIRECTORY "Install the package into its own directory" ON)
set(CONFIG_DIR "${CMAKE_INSTALL_DATAROOTDIR}/ez-meta" CACHE STRING "The relative directory to install package config files.")




# Find deps
find_package(glm CONFIG REQUIRED)

# Create main target.
add_library(meta INTERFACE)
target_link_libraries(meta INTERFACE glm::glm)

# Add the include directory
target_include_directories(meta INTERFACE 
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Select the c++ version to use.
target_compile_features(meta INTERFACE cxx_std_17)
add_library(ez::meta ALIAS meta)



if(BUILD_TESTS)
	add_subdirectory("tests")
endif()

if(SEPARATE_DIRECTORY)
	set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/ez-meta)
endif()



install(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
	TYPE INCLUDE
	FILES_MATCHING
	PATTERN "*.h" PATTERN "*.hpp"
)

install(TARGETS meta
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
	COMPATIBILITY "SameMajorVersion"
	DESTINATION "${CONFIG_DIR}"

	PRECONFIG "cmake/preconfig.cmake"

	# Only add this option if the library is header only, or an interface library.
	ARCH_INDEPENDENT
)