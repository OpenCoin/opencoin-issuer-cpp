cmake_minimum_required(VERSION 3.1.3)

enable_language(C)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)

if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

project(oc-issuer VERSION 0.0.2 LANGUAGES CXX)

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

include(CTest)
enable_testing()

set(CATCH_INSTALL_DOCS Off)
set(CATCH_INSTALL_EXTRAS Off)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.3.1
)

FetchContent_MakeAvailable(Catch2)

#add doxygen documentation
find_package(Doxygen
             REQUIRED dot
             OPTIONAL_COMPONENTS mscgen dia)
set(DOXYGEN_HAVE_DOT YES)
doxygen_add_docs( doc
                  README.md 
                  src
		  COMMENT "Generate documentation"
)

# build common library
set(LIB_SOURCES
  src/model.cpp src/model.hpp
  src/json_serialisation.cpp
  src/big_int.hpp src/big_int.cpp )

add_library(oc-mint-lib ${LIB_SOURCES})
target_link_libraries(oc-mint-lib PUBLIC Crow::Crow)
target_include_directories(oc-mint-lib PUBLIC ${expected_SOURCE_DIR}/include src) 

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE oc-mint-lib INTERFACE tl::expected::expected)

## these are unittests that can be run on any platform
add_executable(tests  test/test_big_int.cpp test/test.cpp)
target_link_libraries(tests
                      oc-mint-lib
		      Catch2::Catch2WithMain)
add_test(tests tests)
