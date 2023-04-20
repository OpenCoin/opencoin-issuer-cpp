#ifndef OC_ISSUER_BIG_INT_HPP
#define OC_ISSUER_BIG_INT_HPP

#include <string>
#include <array>

#include "tl/expected.hpp"

struct BigInt {

  BigInt() : data() {}
  virtual ~BigInt() = default;

  enum class eError : uint8_t { PARSE_ERROR };
  static tl::expected<BigInt, eError> from_string(const std::string &str);

  static BigInt from_int(uint64_t value);
  [[nodiscard]] std::string to_string() const;

  friend bool operator == (const BigInt& rhs, const BigInt& lhs);
private:
  std::array<uint8_t,256U> data;
};

bool operator==(const BigInt &rhs, const BigInt &lhs);

#endif // #ifndef #ifndef OC_ISSUER_BIG_INT_HPP
