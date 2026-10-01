cmake_minimum_required(VERSION 3.10.0)
project(cpp_list_entry VERSION 0.1.0 LANGUAGES C CXX)

# cpp20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(cpp_list_entry INTERFACE)
target_include_directories(cpp_list_entry INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(cpp_list_entry INTERFACE cxx_std_11)

include(CTest)
if(BUILD_TESTING)
	add_executable(cpp_list_entry_tests tests/cpp_list_entry_tests.cpp)
	target_link_libraries(cpp_list_entry_tests PRIVATE cpp_list_entry)
	set_property(TARGET cpp_list_entry_tests PROPERTY VS_GLOBAL_VcpkgApplocalDeps "false")
	add_test(NAME cpp_list_entry_tests COMMAND cpp_list_entry_tests)
endif()

