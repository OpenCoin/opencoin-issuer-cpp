![build](https://github.com/OpenCoin/oc-mint-cpp/actions/workflows/cmake.yaml/badge.svg)

# oc mint sample

this is a C++ implementation of the opencoin protocol 
as mapping to some kind of REST interface 

actually it is a work in progress.

## Protocol Questions

+ What is signed as cdd - only the content of the cdd item with curly braces 
or also the key?
+ the weighted URL as array has a different js encoding as other elements
+ Clarify PSS usage (see https://crypto.stackexchange.com/questions/12707/usability-of-padding-scheme-in-blinded-rsa-signature)

## TODO

+ [x] bigint type for big integers encoded as string
+ [ ] blinding utilizing crypto++
- [x] complete from_json conversions
- [x] complete tests
- [x] drone config
- [ ] select crypto library
   + https://en.wikipedia.org/wiki/Comparison_of_cryptography_libraries

### Blinding Notes

https://stackoverflow.com/questions/47860570/how-to-create-and-verify-blind-rsa-signatures-with-crypto

https://crypto.stackexchange.com/questions/12707/usability-of-padding-scheme-in-blinded-rsa-signature

https://crypto.stackexchange.com/questions/54085/is-there-a-standard-padding-format-for-rsa-blind-signatures/60728#60728
https://crypto.stackexchange.com/questions/5626/rsa-blind-signatures-in-practice
