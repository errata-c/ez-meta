cmake_minimum_required(VERSION 3.24)

project(EZ_META
	VERSION 1.1.0
	LANGUAGES C CXX
	DESCRIPTION "C++ metaprogramming utilites, primarily for determining iterator categories and linear algebra types"
)

set(EZ_META_CONFIG_DIR "share/ez-meta" CACHE STRING "The relative directory to install package config files.")

include(FetchContent)

FetchContent_Declare(
	ez-cmake
	GIT_REPOSITORY "https://github.com/errata-c/ez-cmake.git"
	GIT_TAG "main"
)
FetchContent_Declare(
	glm
	GIT_REPOSITORY "https://github.com/g-truc/glm.git"
	GIT_TAG "0.9.9.8"
	FIND_PACKAGE_ARGS CONFIG
)
FetchContent_MakeAvailable(ez-cmake glm)


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


if(PROJECT_IS_TOP_LEVEL)
	include(CTest)
	if(BUILD_TESTING)
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
endif()