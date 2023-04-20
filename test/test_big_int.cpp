#include "big_int.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("BigInt::from_string", "[big_int]") {
  const std::string VALID [] = {
    "1",
    "12",
    "123",
    "1234",
    "12345",
    "123456",
    "120456",
    "123056",
    "120345",
    "1234560abc",
    "aabbcc",
    "abcdef1"
  };

  for (const auto & i : VALID) {
    auto b = BigInt::from_string(i);
    REQUIRE(b.has_value());
    REQUIRE(b->to_string() == i);
  }
  
  auto invalid_hex = "aabbcc..";
  {
    auto b = BigInt::from_string(invalid_hex);
    REQUIRE(!b);
  }
  
}

TEST_CASE("BigInt::from_int", "[big_int]") {
  auto b = BigInt::from_int(0xaabbccdd);
  REQUIRE(b.to_string()=="aabbccdd");
}
