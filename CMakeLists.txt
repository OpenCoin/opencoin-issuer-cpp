cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)

project(oc-issuer VERSION 0.0.2 LANGUAGES CXX)

enable_language(C)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

#
# crow project Configuration variables
#
set(CROW_BUILD_EXAMPLES Off)
set(CROW_BUILD_TOOLS Off)
set(CROW_BUILD_TESTS Off)
set(CROW_BUILD_DOCS Off)
set(CROW_FEATURES "ssl;compression")

# add crow project to the build
FetchContent_Declare(crow
  		 GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
		 GIT_TAG v1.2.0
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

# add crypt++ (+cmake) library
#set(CRYPTOPP_BUILD_TESTING Off)
#set(CRYPTOPP_INSTALL Off)

#if(NOT cryptopp_POPULATED)
#  FetchContent_Declare(cryptopp
#    GIT_REPOSITORY https://github.com/abdes/cryptopp-cmake.git
#    GIT_TAG CRYPTOPP_8_7_0_1)
#  FetchContent_Populate(cryptopp)
#  add_subdirectory(${cryptopp_SOURCE_DIR} ${cryptopp_BINARY_DIR})
#endif(NOT cryptopp_POPULATED)

include(CTest)
enable_testing()

set(CATCH_INSTALL_DOCS Off)
set(CATCH_INSTALL_EXTRAS Off)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.6.0
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
target_link_libraries(oc-mint-lib PUBLIC Crow::Crow) # cryptopp::cryptopp)
target_include_directories(oc-mint-lib PUBLIC ${expected_SOURCE_DIR}/include src) 

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE oc-mint-lib INTERFACE tl::expected::expected)

## these are unittests that can be run on any platform
add_executable(tests test/test_big_int.cpp
                     test/test_json_s8n.cpp
                     test/test_crypto.cpp)
                     
target_link_libraries(tests
                      oc-mint-lib
		      Catch2::Catch2WithMain)
add_test(tests tests)
