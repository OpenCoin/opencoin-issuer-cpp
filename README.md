opencoin-issuer-cpp - a C++ OpenCoin Issuer REST-API implementation {#mainpage}
=============================================================

![build](https://github.com/OpenCoin/opencoin-issuer-cpp/actions/workflows/cmake.yaml/badge.svg)
[![Documentation](https://github.com/OpenCoin/opencoin-issuer-cpp/actions/workflows/doxygen-gh-pages.yml/badge.svg)](https://github.com/OpenCoin/opencoin-issuer-cpp/actions/workflows/doxygen-gh-pages.yml)

# opencoin issuer

this is a C++ implementation of the [opencoin protocol](https://opencoin.org/OpenCoin.html)
done as mapping to some kind of REST interface.

As all issuer related interactions of the protocol follows a request/response mechanism we are able to map all of them to Http-Requests.

We decided to use POST-Requests in all cases, 
as the protocol uses JSON-formatted messages for requests.

The following table gives an overview of the mapping of requests to URLs:

| Request           | URL          | Response                      | C++ Interface Method                               |
|:------------------|:-------------|:------------------------------|:---------------------------------------------------|
| RequestCDDCSerial | /cddc/serial | ResponseCDDCSerial            | cdd.cdd_serial of Model::get_current_cddc() |
| RequestCDDC       | /cddc        | ResponseCDDC                  | Model::get_current_cddc()                            |
| RequestMKCs       | /mkcs        | ResponseMKCs                  | Model::getMKCs                                     |
| RequestMint       | /mint        | ResponseMint                  | Model::mint                                        |
| RequestRenew      | /renew       | ResponseMint or ResponseDelay | ?                                                  |
| RequestResume     | /resume      | ResponseMint or ResponseDelay | ?                                                  |
| RequestRedeem     | /redeem      | ResponseRedeem                | Model::redeem                                      |

actually the implementation is a work in progress.

## TODO

- [ ] select crypto library
   + https://en.wikipedia.org/wiki/Comparison_of_cryptography_libraries
+ [ ] blinding utilizing crypto++
+ [ ] integrate session and login to make transactions account based
+ [ ] bookkeeping for accounts

## Protocol Questions

+ What is signed as cdd - only the content of the cdd item with curly braces 
or also the key?
+ the weighted URL as array has a different js encoding as other elements
+ Clarify PSS usage (see https://crypto.stackexchange.com/questions/12707/usability-of-padding-scheme-in-blinded-rsa-signature) 

### Blinding Notes

https://stackoverflow.com/questions/47860570/how-to-create-and-verify-blind-rsa-signatures-with-crypto

https://crypto.stackexchange.com/questions/12707/usability-of-padding-scheme-in-blinded-rsa-signature

https://crypto.stackexchange.com/questions/54085/is-there-a-standard-padding-format-for-rsa-blind-signatures/60728#60728
https://crypto.stackexchange.com/questions/5626/rsa-blind-signatures-in-practice

<https://cfrg.github.io/draft-irtf-cfrg-blind-signatures/draft-irtf-cfrg-rsa-blind-signatures.html>

alternative implementation using openssl
https://github.com/jedisct1/blind-rsa-signatures
