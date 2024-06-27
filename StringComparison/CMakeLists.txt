# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../CMake")

project ("StringComparison" LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(xxHash_DIR "C:/Vcpkg/installed/x64-windows/share/xxhash")
add_executable("StringComparisonExe" "main.cpp")

target_link_options("StringComparisonExe" PUBLIC 
	"$<$<CXX_COMPILER_ID:MSVC>:/DEBUG>"
	"$<$<CXX_COMPILER_ID:MSVC>:/OPT:REF>"
	"$<$<CXX_COMPILER_ID:MSVC>:/OPT:ICF>"
	"$<$<CXX_COMPILER_ID:MSVC>:/PROFILE>"
)
find_package(xxHash CONFIG REQUIRED)
target_link_libraries(
	"StringComparisonExe"
	PUBLIC
	"xxHash::xxhash"
	"BenchmarkSuite"
	"glaze::glaze"
)

include("JsonifierDetectArchitecture")
message("${AVX_FLAG}")
target_compile_options (
	"StringComparisonExe" PUBLIC
	"$<$<CXX_COMPILER_ID:MSVC>:$<$<STREQUAL:${ASAN_ENABLED},TRUE>:/fsanitize=address>>"
	"$<$<CXX_COMPILER_ID:MSVC>:/constexpr:steps 2097152>"
	"$<$<CXX_COMPILER_ID:GNU>:$<$<STREQUAL:${ASAN_ENABLED},TRUE>:-fsanitize=address>>"
	
	"${AVX_FLAG}"
)

target_compile_definitions(
	"StringComparisonExe" PUBLIC
	"JSONIFIER_CPU_INSTRUCTIONS=${JSONIFIER_CPU_INSTRUCTIONS}"
	"$<$<STREQUAL:${ASAN_ENABLED},TRUE>:ASAN>"
	"$<$<STREQUAL:${DEV},TRUE>:DEV>"
)

target_include_directories("StringComparisonExe" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")

target_compile_definitions(
	"StringComparisonExe"
	PUBLIC 
	"INSTALL_PATH=\"${CMAKE_INSTALL_PREFIX}\""
)

install(
	FILES 
	"$<TARGET_FILE:StringComparisonExe>"
	DESTINATION "${CMAKE_INSTALL_PREFIX}/"
	OPTIONAL
)

install(
	FILES 
	"${CMAKE_SOURCE_DIR}/TwitterData-Prettified.json"
	DESTINATION "${CMAKE_INSTALL_PREFIX}/"
	OPTIONAL
)