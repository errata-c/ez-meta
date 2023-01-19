cmake_minimum_required(VERSION 3.24)
project(EZ_META_TESTS)

FetchContent_Declare(
	fmt
	GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
	GIT_TAG "9.1.0"
	FIND_PACKAGE_ARGS CONFIG
)
FetchContent_MakeAvailable(fmt)


add_subdirectory("basic")