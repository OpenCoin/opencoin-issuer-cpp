#ifndef MODEL_HPP
#define MODEL_HPP

#include <memory>
#include <string>
#include <chrono>
#include <optional>

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

enum class eError {
  JSON_PARSE_ERROR,
  JSON_ERROR,
  NOT_IMPLEMENTED
};

struct Response {
  unsigned int message_reference;
  unsigned int status_code;
  std::string status_description;

  virtual crow::json::wvalue to_json() const;  
};
  
struct RequestCDDSerial {
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  static tl::expected<RequestCDDSerial,eError> from_string(const std::string& str);
};

struct ResponseCDDSerial: Response {
  unsigned int cdd_serial;

  crow::json::wvalue to_json() const override;  
};

struct RequestCDDC {
  unsigned int cdd_serial;/// The version of the CDD. (Int)
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  static tl::expected<RequestCDDC,eError> from_string(const std::string& str);
};

struct ResponseCDDC : Response {
  CDDC cddc;

  crow::json::wvalue to_json() const override;  
};

struct RequestMKCs {
  std::vector<unsigned int> denominations;
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  std::vector<unsigned int> mint_key_ids;
  //  "type": "request mint key certificates"
  static tl::expected<RequestMKCs,eError> from_string(const std::string& str);
};

struct ResponseMKCs: Response {
  std::vector<MintKeyCert> keys;

  crow::json::wvalue to_json() const override;  
};

struct Blind {
  std::string blinded_payload_hash;
  std::string mint_key_id;
  std::string reference;
  crow::json::wvalue to_json() const;  
};

struct BlindSignature {
  std::string blind_signature;
  std::string reference;
  crow::json::wvalue to_json() const;  
};

struct RequestMint {
  std::vector<Blind> blinds;
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  //  "type": "request mint"
  static tl::expected<RequestMint,eError> from_string(const std::string& str);
};

struct ResponseMint : Response {
  std::vector<BlindSignature> blind_signatures;

  crow::json::wvalue to_json() const override;  
};


struct Coin {
  struct Payload {
    std::string cdd_location;
    unsigned int denomination;
    std::string issuer_id;
    std::string mint_key_id;
    std::string protocol_version;
    std::string serial;
    
    crow::json::wvalue to_json() const;  
};

  Payload payload;
  std::string signature;

  crow::json::wvalue to_json() const;  
};

struct CoinStack {
  std::vector<Coin> coins;
  std::string subject;
  //  "type": "coinstack"
  crow::json::wvalue to_json() const;  
};

struct RequestRenew {
  std::vector<Blind> blinds;
  std::vector<Coin> coins;
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  unsigned int transaction_reference; 
  //  "type": "request renew"
  static tl::expected<RequestRenew,eError> from_string(const std::string& str);
};

struct ResponseDelay : Response {
  crow::json::wvalue to_json() const override;  
};

struct RequestResume {
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  unsigned int transaction_reference; 
  //  "type": "request resume"
  static tl::expected<RequestResume,eError> from_string(const std::string& str);
};

struct RequestRedeem {
  std::vector<Coin> coins;
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  unsigned int transaction_reference; 
  //  "type": "request redeem"
  static tl::expected<RequestRedeem,eError> from_string(const std::string& str);
};

struct ResponseReedem : Response {
  crow::json::wvalue to_json() const override;  
};

class Model {
public:
  virtual ~Model(){};

  virtual const CDDC& getCDDC() = 0;
  virtual const CDDC& getCurrentCDDC() = 0;
  virtual void mint() = 0;

  static std::unique_ptr<Model> getModel(const std::string& backend_name);
private:
};

#endif // #ifndef MODEL_HPP

