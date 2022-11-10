cmake_minimum_required(VERSION 3.1.3)

enable_language(C)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)

if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

project(oc-mint VERSION 0.0.1 LANGUAGES CXX)

include(FetchContent)
#
# crow project Configuration variables
#
set(CROW_BUILD_EXAMPLES Off)
set(CROW_BUILD_TOOLS Off)
set(CROW_BUILD_TESTS Off)
set(CROW_BUILD_DOCS Off)

# add crow project to the build
FetchContent_Declare(crow
  		 GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
		 GIT_TAG v1.0+5
		 )

if(NOT crow_POPULATED)
  FetchContent_Populate(crow)
  add_subdirectory(${crow_SOURCE_DIR} ${crow_BINARY_DIR})
endif(NOT crow_POPULATED)

# add tartan lamas expected library
FetchContent_Declare(expected
  		 GIT_REPOSITORY https://github.com/TartanLlama/expected.git
		 GIT_TAG master
		 )
if(NOT expected_POPULATED)
  FetchContent_Populate(expected)
endif(NOT expected_POPULATED)
 
set(API_SOURCES src/main.cpp src/model.cpp src/model.hpp)
add_executable(oc-mint ${API_SOURCES})
target_link_libraries(oc-mint PRIVATE Crow::Crow INTERFACE tl::expected::expected)
target_include_directories(oc-mint PRIVATE ${expected_SOURCE_DIR}/include) 
