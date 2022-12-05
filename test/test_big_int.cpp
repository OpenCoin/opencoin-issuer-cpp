#include "big_int.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("BigInt::from_string", "[big_int]") {
  std::string VALID [] = {
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

  for (size_t i= 0; i<sizeof(VALID)/sizeof(VALID[0]);i++) {
    auto b = BigInt::from_string(VALID[i]);
    REQUIRE(b->to_string() == VALID[i]);
    REQUIRE(b.has_value());
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
