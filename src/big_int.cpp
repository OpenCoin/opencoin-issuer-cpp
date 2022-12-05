#include "big_int.hpp"
#include "tl/expected.hpp"
#include <iostream>
#include <charconv>
#include <vector>

inline uint8_t hex(char c) {
  switch(c) {
    case '0': return 0; 
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'a': return 10;
    case 'b': return 11;
    case 'c': return 12;
    case 'd': return 13;
    case 'e': return 14;
    case 'f': return 15;
    case 'A': return 10;
    case 'B': return 11;
    case 'C': return 12;
    case 'D': return 13;
    case 'E': return 14;
    case 'F': return 15;
    default:
      return 0xff;
  }
}
  
tl::expected<BigInt,BigInt::eError>
BigInt::from_string(const std::string& str) {
  BigInt b;
  // std::cout << str << std::endl;

  uint8_t hval=0;
  uint8_t nibble;
  size_t i = str.size()+1;

  for(auto c : str) {
    nibble = hex(c);
    if (nibble ==0xFF) {
      return tl::make_unexpected(eError::PARSE_ERROR);
    }
    if (i%2) {
      hval = nibble << 4;
    } else {
      hval |= nibble;
      b.data[256 - (i/2)]= hval;
      hval = 0;
    }
    i--;
  }

  // std::cout << std::hex;
  // for (auto b: b.data)
  //   {
  //     std::cout << (int)b;
  //   }
  // std::cout << std::dec << std::endl;
  return b;
}

BigInt BigInt::from_int(uint64_t value)
{
  BigInt b;
  b.data[248]=(value >> 56 & 0xff);
  b.data[249]=(value >> 48 & 0xff);
  b.data[250]=(value >> 40 & 0xff);
  b.data[251]=(value >> 32 & 0xff);
  b.data[252]=(value >> 24 & 0xff);
  b.data[253]=(value >> 16 & 0xff);
  b.data[254]=(value >> 8 & 0xff);
  b.data[255]=(value & 0xff);
  return b;
}

constexpr char hex_char [] = "0123456789abcdef";
std::string BigInt::to_string() const
{
  std::string s;
  uint8_t b;
  uint8_t first_digit = 0;
  for (size_t i = 0; i<256;i++) {
    b = data[i];
    if (first_digit==0) {
      if (b==0) {
	continue;
      } else if (b>0xf) {
	s.push_back(hex_char[b >> 4]);
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
