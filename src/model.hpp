#ifndef OC_ISSUER_MODEL_HPP
#define OC_ISSUER_MODEL_HPP

#include <chrono>
#include <memory>
#include <optional>
#include <string>

#include "crow/json.h"

#include "tl/expected.hpp"
#include "big_int.hpp"

struct PublicKey {
  BigInt modulus; //: "daaa63ddda38c189b8c49020c8276adbe0a695685a...",
  BigInt public_exponent;//: 65537,

    [[nodiscard]] crow::json::wvalue to_json() const;
};

struct WeightedUrl {
  uint32_t weight;
  std::string url;

    [[nodiscard]] crow::json::wvalue to_json() const;
};

/** @brief currency description document 
 *
 * this structure describes a currency based on opencoin protocol. 
 * It can be converted to json to provide the specified currency 
 * description document. 
 */
struct CDD {

  std::string additional_info;
  time_t cdd_expiry_date;    /// expiry date of this document (e.g.
                             ///2023-07-22T15:45:53.164685)
  std::string cdd_location;  /// URL of location of this document (e.g
                             ///https://opencent.org)
  size_t cdd_serial;         /// serial number of currency description document
  time_t cdd_signing_date;   /// date of signing this document (e.g.
                             ///2022-07-22T15:45:53.164685)
  size_t currency_divisor;   /// divisor used for coins of this currency
  std::string currency_name; /// name of the currency (e.g. OpenCent)
  std::vector<unsigned>
      denominations; /// the available denominations of this currency
  BigInt id;         /// an identity for this currency

  std::vector<WeightedUrl> info_service;
  /* eCipherSuite*/
  std::string issuer_cipher_suite; /// the cipher suite used for this currencey
                                   /// (currently only RSA-SHA256-PSS-CHAUM82 
                                   ///  is supported)
  PublicKey issuer_public_master_key; /// the public key of this currency
  std::vector<WeightedUrl> mint_service;
  std::string protocol_version; // e.g. https://opencoin.org/1.0
  std::vector<WeightedUrl> redeem_service;
  std::vector<WeightedUrl> renew_service;

  [[nodiscard]] crow::json::wvalue to_json() const;
};

struct CDDC {
  CDD cdd;
  std::string signature;

    [[nodiscard]] crow::json::wvalue to_json() const;
};

struct MintKey {
  unsigned int cdd_serial;
  std::string coins_expiry_date; //": "2023-10-30T15:45:53.164685",
  unsigned int denomination; //": 1,
  BigInt id;   // "1ceb977bb531c65f133ab8b0d60862b17369d96",
  BigInt issuer_id; //": "23ed956e629ba35f0002eaf833ea436aea7db5c2",
  PublicKey public_mint_key;

  std::string sign_coins_not_after;
  std::string sign_coins_not_before;
  //    "type": "mint key"
  [[nodiscard]] crow::json::wvalue to_json() const;
};

struct MintKeyCert {
  MintKey mint_key;
  std::string signature;

    [[nodiscard]] crow::json::wvalue to_json() const;
};

enum class eError {
  JSON_PARSE_ERROR,
  JSON_MISSING_KEY,
  JSON_WRONG_REQUEST_TYPE,
  JSON_WRONG_VALUE_TYPE,
  JSON_ERROR,
  NOT_IMPLEMENTED
};

struct Response {
  unsigned int message_reference;
  unsigned int status_code;
  std::string status_description;

    [[nodiscard]] virtual crow::json::wvalue to_json() const=0;
};

struct RequestCDDCSerial {
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  static tl::expected<RequestCDDCSerial, eError>
  from_string(const std::string &str);
};

struct ResponseCDDCSerial : Response {
  unsigned int cdd_serial{0U};

    [[nodiscard]] crow::json::wvalue to_json() const override;
};

struct RequestCDDC {
  unsigned int cdd_serial;        /// The version of the CDD. (Int)
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  static tl::expected<RequestCDDC, eError> from_string(const std::string &str);
};

struct ResponseCDDC : Response {
  CDDC cddc;

    [[nodiscard]] crow::json::wvalue to_json() const override;
};

struct RequestMKCs {
  std::vector<unsigned int> denominations;
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  std::vector<BigInt> mint_key_ids;
  //  "type": "request mint key certificates"
  static tl::expected<RequestMKCs, eError> from_string(const std::string &str);
};

struct ResponseMKCs : Response {
  std::vector<MintKeyCert> keys;

    [[nodiscard]] crow::json::wvalue to_json() const override;
};

struct Blind {
  BigInt blinded_payload_hash; //bigint
  BigInt mint_key_id; //bigint
  std::string reference;
    [[nodiscard]] crow::json::wvalue to_json() const;
  static tl::expected<Blind, eError> from_json(const crow::json::rvalue &json);
};

struct BlindSignature {
  BigInt blind_signature;
  std::string reference;
    [[nodiscard]] crow::json::wvalue to_json() const;
};

struct RequestMint {
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  BigInt transaction_reference; 
  std::vector<Blind> blinds;
  //  "type": "request mint"
  static tl::expected<RequestMint, eError> from_string(const std::string &str);
};

struct ResponseMint : Response {
  std::vector<BlindSignature> blind_signatures;

    [[nodiscard]] crow::json::wvalue to_json() const override;
};

struct Coin {
  struct Payload {
    std::string cdd_location;
    unsigned int denomination;
    BigInt issuer_id;
    BigInt mint_key_id;
    std::string protocol_version;
    BigInt serial;

      [[nodiscard]] crow::json::wvalue to_json() const;
    static tl::expected<Payload,eError> from_json(const crow::json::rvalue& json);
  };

  Payload payload;
  std::string signature;

    [[nodiscard]] crow::json::wvalue to_json() const;
  static tl::expected<Coin, eError> from_json(const crow::json::rvalue &json);
};

struct CoinStack {
  std::vector<Coin> coins;
  std::string subject;
  //  "type": "coinstack"
  [[nodiscard]]  crow::json::wvalue to_json() const;
};

struct RequestRenew {
  std::vector<Blind> blinds;
  std::vector<Coin> coins;
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  std::string transaction_reference;
  //  "type": "request renew"
  static tl::expected<RequestRenew, eError> from_string(const std::string &str);
};

struct ResponseDelay : Response {
    [[nodiscard]] crow::json::wvalue to_json() const override;
};

struct RequestResume {
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  BigInt transaction_reference; 
  //  "type": "request resume"
  static tl::expected<RequestResume, eError>
  from_string(const std::string &str);
};

struct RequestRedeem {
  std::vector<Coin> coins;
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  //  "type": "request redeem"
  static tl::expected<RequestRedeem, eError>
  from_string(const std::string &str);
};

struct ResponseRedeem : Response {
    [[nodiscard]] crow::json::wvalue to_json() const override;
};

class Model {
public:
  virtual ~Model()=default;

  /** 
   * return the CurrencyDocumentDescription certifikate for a specific 
   * serial version number of it.
   * [see spec](https://opencoin.org/0.4/schemata.html#cddc)
   * @return returns a pointer to the CDDC if successful, false otherwise
   */
  virtual tl::expected<CDDC *, bool> get_cddc(unsigned int cdd_serial) = 0;

  /** 
   * return the CurrencyDocumentDescription certifikate 
   * [see spec](https://opencoin.org/0.4/schemata.html#cddc)
   * @return returns a pointer to the CDDC if successful, false otherwise
   */virtual tl::expected<CDDC *, bool> get_current_cddc() = 0;

  /** 
   * return the MintKey certificates for a given list of denominations 
   * and mint key ids
   *
   * @param denominations 
   * @param mint_key_ids 
   *
   * @return mint key certificates for given denominations and mint_key_ids
   */
  virtual std::vector<MintKeyCert>
  getMKCs(const std::vector<unsigned int> &denominations,
          const std::vector<BigInt> &mint_key_ids) = 0;


  /** 
   * returns the vector of blind signatures for a given vector of blinds
   *
   * @param transaction_reference reference to a transaction (send from client)
   * @param blinds the vector of blinds to sign
   *
   * @return 
   */
  virtual std::vector<BlindSignature>
  mint(std::string const& transaction_reference,
       const std::vector<Blind> &blinds) = 0;

  /** 
   * redeem valid coins into real money
   *
   * @param coins the coins to redeem
   *
   * @return true if successful, false on error
   */
  virtual bool redeem(const std::vector<Coin> &coins) = 0;

  /** 
   * factory function returning a concrete backend for Opencoin API handling.
   * based on backend_name a concrete backend will be returned 
   * or in case of error null.
   * @param backend_name 
   *
   * @return pointer to backend instance or null on invalid backend name
   */
  static std::unique_ptr<Model> get_model(const std::string &backend_name);

private:
};

#endif // #ifndef OC_ISSUER_MODEL_HPP
