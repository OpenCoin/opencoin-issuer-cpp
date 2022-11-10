#ifndef MODEL_HPP
#define MODEL_HPP

#include <memory>
#include <string>
#include <chrono>

#include "crow/json.h"

#include "tl/expected.hpp"

struct PublicKey {
  std::string modulus; //: "daaa63ddda38c189b8c49020c8276adbe0a695685a...",
  std::string public_exponent;//: 65537,

  crow::json::wvalue to_json() const;  
};

struct WeightedUrl {
  uint32_t weight;
  std::string url;

  crow::json::wvalue to_json() const;
};


/** currency description document */
struct CDD {

  std::string additional_info;
  time_t cdd_expiry_date;//: 2023-07-22T15:45:53.164685
  std::string  cdd_location;//: https://opencent.org,
  size_t cdd_serial;//: 1,
  time_t  cdd_signing_date;//: 2022-07-22T15:45:53.164685,
  size_t  currency_divisor;//: 100,
  std::string  currency_name;//: OpenCent,
  std::vector<unsigned>  denominations;//: [1, 2, 5],
  std::string id;//: 23ed956e629ba35f0002eaf833ea436aea7db5c2,
  std::vector<WeightedUrl> info_service;
  /* eCipherSuite*/ std::string issuer_cipher_suite;//: RSA-SHA256-PSS-CHAUM82,
  PublicKey issuer_public_master_key;//: {
	//  modulus: daaa63ddda38c189b8c49020c8276adbe0a695685a...,
	// public_exponent: 65537,
	// type: rsa public key
  //},
  std::vector<WeightedUrl> mint_service;
  std::string protocol_version;   //: https://opencoin.org/1.0,
  std::vector<WeightedUrl> redeem_service;
  std::vector<WeightedUrl> renew_service; 

  crow::json::wvalue to_json() const;
};

struct CDDC {
  CDD cdd;
  std::string signature;

  crow::json::wvalue to_json() const;  
};

struct MintKey {
  unsigned int cdd_serial;
  std::string coins_expiry_date; //": "2023-10-30T15:45:53.164685",
  unsigned int denomination; //": 1,
  std::string id;   // "1ceb977bb531c65f133ab8b0d60862b17369d96",
  std::string issuer_id; //": "23ed956e629ba35f0002eaf833ea436aea7db5c2",
  PublicKey public_mint_key;
  
  std::string sign_coins_not_after;
  std::string sign_coins_not_before;
  //    "type": "mint key"
  crow::json::wvalue to_json() const;  
};
  
struct MintKeyCert {
  MintKey mint_key;
  std::string signature;

  crow::json::wvalue to_json() const;  
};

struct RequestCDDC {
  unsigned int cdd_serial;/// The version of the CDD. (Int)
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  static std::optional<RequestCDDC> from_json(const crow::json::rvalue & json); 
};

struct ResponseCDDC {
  CDDC cddc;
  unsigned int message_reference;
  unsigned int status_code;
  std::string status_description;

  crow::json::wvalue to_json() const;  
};

  
class Model {
public:
  virtual ~Model(){};

  virtual const CDDC& getCDDC() = 0;
  virtual void mint() = 0;

  static std::unique_ptr<Model> getModel(const std::string& backend_name);
private:
};

#endif // #ifndef MODEL_HPP

