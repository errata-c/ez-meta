cmake_minimum_required(VERSION 3.16)

project(EZ_META
	VERSION 1.0.1
	LANGUAGES C CXX
	DESCRIPTION "C++ metaprogramming utilites, primarily for determining iterator categories and linear algebra types"
)

option(EZ_META_BUILD_TESTS "Build the test executables" OFF)
set(EZ_META_CONFIG_DIR "share/ez-meta" CACHE STRING "The relative directory to install package config files.")

include(FetchContent)

FetchContent_Declare(
	ez-cmake
	GIT_REPOSITORY "https://github.com/errata-c/ez-cmake.git"
	GIT_TAG "main"
)
FetchContent_MakeAvailable(ez-cmake)


# Find deps
find_package(glm CONFIG REQUIRED)

# Create main target.
add_library(meta INTERFACE)
target_link_libraries(meta INTERFACE glm::glm)

# Add the include directory
target_include_directories(meta INTERFACE 
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
	"$<INSTALL_INTERFACE:include>"
)

# Select the c++ version to use.
target_compile_features(meta INTERFACE cxx_std_17)
add_library(ez::meta ALIAS meta)

if(EZ_META_BUILD_TESTS)
	add_subdirectory("tests")
endif()

install(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
	TYPE INCLUDE
	FILES_MATCHING
	PATTERN "*.h" PATTERN "*.hpp"
)

install(TARGETS meta
	EXPORT ez-meta-targets
	RUNTIME DESTINATION "bin/$<CONFIG>"
	ARCHIVE DESTINATION "lib/$<CONFIG>"
	LIBRARY DESTINATION "lib/$<CONFIG>"
)

install_package(
	NAME "ez-meta"
	NAMESPACE "ez::"

	EXPORT "ez-meta-targets"
	VERSION "${EZ_META_VERSION}"
	COMPATIBILITY "SameMajorVersion"
	DESTINATION "${EZ_META_CONFIG_DIR}"

	PRECONFIG "cmake/preconfig.cmake"

	# Only add this option if the library is header only, or an interface library.
	ARCH_INDEPENDENT
)

set(CPACK_PACKAGE_NAME "ez-meta")
set(CPACK_PACKAGE_VERSION "${EZ_META_VERSION}")
set(CPACK_PACKAGE_VENDOR "errata-c")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")

include(CPack)