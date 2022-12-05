#ifndef BIG_INT_HPP
#define BIG_INT_HPP

#include <string>
#include <array>

#include "tl/expected.hpp"

struct BigInt {

  BigInt() : data() {}
  virtual ~BigInt() {}

  enum class eError : uint8_t { PARSE_ERROR };
  static tl::expected<BigInt, eError> from_string(const std::string &str);

  static BigInt from_int(uint64_t value);
  std::string to_string() const;

  friend bool operator == (const BigInt& rhs, const BigInt& lhs);
private:
  std::array<uint8_t,256> data;
};

bool operator==(const BigInt &rhs, const BigInt &lhs);

#endif // #ifndef #ifndef BIG_INT_HPP
