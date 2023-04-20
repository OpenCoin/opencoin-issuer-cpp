#include "big_int.hpp"
#include "tl/expected.hpp"
#include <charconv>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "clion-misra-cpp2008-6-4-5"
inline uint8_t hex(char c) {
  switch(c) {
    case '0': return 0U;
    case '1': return 1U;
    case '2': return 2U;
    case '3': return 3U;
    case '4': return 4U;
    case '5': return 5U;
    case '6': return 6U;
    case '7': return 7U;
    case '8': return 8U;
    case '9': return 9U;
    case 'a': return 10U;
    case 'b': return 11U;
    case 'c': return 12U;
    case 'd': return 13U;
    case 'e': return 14U;
    case 'f': return 15U;
    case 'A': return 10U;
    case 'B': return 11U;
    case 'C': return 12U;
    case 'D': return 13U;
    case 'E': return 14U;
    case 'F': return 15U;
    default:
      return 0xffU;
  }
}
#pragma clang diagnostic pop
  
tl::expected<BigInt,BigInt::eError>
BigInt::from_string(const std::string& str) {
  BigInt b;

  uint8_t hval=0U;
  uint8_t nibble;
  size_t i = str.size()+1U;

  for(auto c : str) {
    nibble = hex(c);
    if (nibble ==0xFFU) {
      return tl::make_unexpected(eError::PARSE_ERROR);
    }
    if ( i%2U != 0U ) {
      hval = nibble << 4U;
    } else {
      hval |= nibble;
      b.data[256U - (i/2U)]= hval;
      hval = 0U;
    }
    i--;
  }

  return b;
}

BigInt BigInt::from_int(uint64_t value)
{
  BigInt b;
  b.data[248U]=static_cast<uint8_t>(value >> 56 & 0xffU);
  b.data[249U]=static_cast<uint8_t>(value >> 48 & 0xffU);
  b.data[250U]=static_cast<uint8_t>(value >> 40U & 0xffU);
  b.data[251U]=static_cast<uint8_t>(value >> 32U & 0xffU);
  b.data[252U]=static_cast<uint8_t>(value >> 24U & 0xffU);
  b.data[253U]=static_cast<uint8_t>(value >> 16U & 0xffU);
  b.data[254U]=static_cast<uint8_t>(value >> 8U & 0xffU);
  b.data[255U]=static_cast<uint8_t>(value & 0xffU);
  return b;
}

constexpr char hex_char [] = "0123456789abcdef";
std::string BigInt::to_string() const
{
  std::string s;
  uint8_t b;
  uint8_t first_digit = 0U;
  for (size_t i = 0U; i<256U;i++) {
    b = data[i];
    if (first_digit==0U) {
      if (b==0U) {
	      continue;
      } else if (b>0xfU) {
	      s.push_back(hex_char[b >> 4]);
      } else {
        /* nothing to do here */
      }
      s.push_back(hex_char[b & 0xf]);
      first_digit = b;
    } else {
      s.push_back(hex_char[b >> 4]);
      s.push_back(hex_char[b & 0xf]);
    }
  }
  return s;
}

bool operator == (const BigInt& rhs, const BigInt& lhs)
{ return rhs.data == lhs.data; }
