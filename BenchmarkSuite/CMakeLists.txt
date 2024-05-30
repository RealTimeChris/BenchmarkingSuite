cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAME "BenchmarkSuite")
set(CMAKE_CXX_STANDARD 20)

project("${PROJECT_NAME}" LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../CMake;")

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Include/benchmarksuite/*.hpp")

include(FetchContent)

FetchContent_Declare(
  nanobench
  GIT_REPOSITORY https://github.com/realtimechris/nanobench.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(nanobench)

set_target_properties(nanobench PROPERTIES INTERFACE_INCLUDE_DIRECTORIES  ${CMAKE_INSTALL_PREFIX}/include)

add_library("${PROJECT_NAME}" INTERFACE "${HEADERS}")

add_library("BenchmarkSuite::BenchmarkSuite" ALIAS "${PROJECT_NAME}")

if (MSVC)
	string(REGEX MATCH "\/cl(.exe)?$" matched_cl ${CMAKE_CXX_COMPILER})
	if (matched_cl)

		target_compile_options(
			"${PROJECT_NAME}" INTERFACE
			$<$<CONFIG:Release>:/GL>
			$<$<CONFIG:MinSizeRel>:/GL>
			"/Zc:preprocessor"
			"/permissive-"
			"/Zc:lambda"
		)

		target_link_options(
			"${PROJECT_NAME}" INTERFACE
			$<$<CONFIG:Release>:/LTCG /INCREMENTAL:NO>
			$<$<CONFIG:MinSizeRel>:/LTCG /INCREMENTAL:NO>
		)
	endif()
endif()

if (WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /FA")
endif()

set_target_properties(
	"${PROJECT_NAME}" PROPERTIES
	OUTPUT_NAME "benchmarksuite"
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

target_include_directories(
	"${PROJECT_NAME}" INTERFACE
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include/>"
)

target_compile_options("${PROJECT_NAME}" INTERFACE "$<$<COMPILE_LANGUAGE:CXX>:${AVX_FLAG}>")

target_compile_definitions(
	"${PROJECT_NAME}" INTERFACE
	"$<$<STREQUAL:${ASAN_ENABLED},TRUE>:/fsanitize=address>"
	"$<$<STREQUAL:${DEV},TRUE>:DEV>"
)

set(CONFIG_FILE_NAME "BenchmarkSuiteConfig.cmake")
set(EXPORTED_TARGETS_NAME "BenchmarkSuiteTargets")
set(EXPORTED_TARGETS_FILE_NAME "${EXPORTED_TARGETS_NAME}.cmake")
set(EXPORTED_TARGETS_FILE_PATH "share/benchmarksuite/${EXPORTED_TARGETS_FILE_NAME}")
set(VERSION_FILE_NAME "BenchmarkSuiteConfigVersion.cmake")

include(CMakePackageConfigHelpers)
configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/../CMake/${CONFIG_FILE_NAME}.in"
	"${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME}"
	INSTALL_DESTINATION "share/benchmarksuite"
	PATH_VARS
	EXPORTED_TARGETS_FILE_PATH
)

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/${VERSION_FILE_NAME}"
	VERSION "1.0.0"
	COMPATIBILITY AnyNewerVersion
)

install(
	FILES
	"${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME}"
	"${CMAKE_CURRENT_BINARY_DIR}/${VERSION_FILE_NAME}"
	DESTINATION "share/benchmarksuite"
)

install(
	DIRECTORY
	"${CMAKE_CURRENT_SOURCE_DIR}/Include/"
	DESTINATION "include"
)

install(
	TARGETS "${PROJECT_NAME}"
	EXPORT "${EXPORTED_TARGETS_NAME}"
	RUNTIME DESTINATION "bin"
	ARCHIVE DESTINATION "lib"
)

install(
	EXPORT "${EXPORTED_TARGETS_NAME}"
	FILE "${EXPORTED_TARGETS_FILE_NAME}"
	NAMESPACE "BenchmarkSuite::"
	DESTINATION "share/benchmarksuite"
)

if (WIN32 AND "${BUILD_SHARED_LIBS}")
	install(
		FILES 
		"$<TARGET_PDB_FILE:BenchmarkSuite>"
		DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
		OPTIONAL
	)
endif()
