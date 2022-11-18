#include "model.hpp"
#include "crow/json.h"

#define TO_JSON(name) r[#name]=name
#define TO_JSON_JSON(name) r[#name]=name.to_json()
#define TO_JSON_ARRAY(name) r[#name]=list_to_json(name)

template <class T>
crow::json::wvalue list_to_json(const std::vector<T>& array) {
  crow::json::wvalue::list l;
  for(auto item:array)
    l.push_back(item.to_json());
  return crow::json::wvalue(l);
}

crow::json::wvalue list_to_json(const std::vector<unsigned int>& array) {
  crow::json::wvalue::list l;
  for(auto item:array)
    l.push_back(item);
  return crow::json::wvalue(l);
}


crow::json::wvalue PublicKey::to_json() const {
  crow::json::wvalue r;
  TO_JSON(modulus);
  TO_JSON(public_exponent);
  r["type"]="rsa public key";
  return r;
}

crow::json::wvalue WeightedUrl::to_json() const
{
  crow::json::wvalue::list l;
  crow::json::wvalue w(weight);

  l.push_back(w);
  l.push_back(url);
  return l;
}

crow::json::wvalue CDD::to_json() const {

  crow::json::wvalue r;
  TO_JSON(additional_info);
  TO_JSON(cdd_expiry_date);
  TO_JSON(cdd_location);
  TO_JSON(cdd_serial);
  TO_JSON(cdd_signing_date);
  TO_JSON(currency_divisor);
  TO_JSON( currency_name);
  TO_JSON_ARRAY(denominations);
  TO_JSON(id);
  TO_JSON_ARRAY(info_service);
  TO_JSON(issuer_cipher_suite);
  TO_JSON_JSON(issuer_public_master_key);
  TO_JSON_ARRAY(mint_service);
  TO_JSON(protocol_version); 
  TO_JSON_ARRAY(redeem_service);
  TO_JSON_ARRAY(renew_service); 

  r["type"]= "cdd";
  return r;
}

crow::json::wvalue CDDC::to_json() const{  
  crow::json::wvalue r;
  TO_JSON_JSON(cdd);
  TO_JSON(signature);
  r["type"]= "cdd certificate";
  return r;
}

crow::json::wvalue MintKey::to_json() const {
  crow::json::wvalue r;

  TO_JSON(cdd_serial);
  TO_JSON(coins_expiry_date);
  TO_JSON(denomination);
  TO_JSON(id);
  TO_JSON(issuer_id);
  TO_JSON_JSON(public_mint_key);
  
  TO_JSON(sign_coins_not_after);
  TO_JSON(sign_coins_not_before);
  
  r["type"]= "mint key";
  return r;
}  


crow::json::wvalue MintKeyCert::to_json() const {  
  crow::json::wvalue r;
  TO_JSON_JSON(mint_key);
  TO_JSON(signature);
  r["type"]= "mint key certificate";
  return r;
}

crow::json::wvalue Response::to_json() const
{
  crow::json::wvalue r;
  TO_JSON(message_reference);
  TO_JSON(status_code);
  TO_JSON(status_description);
  return r;
}


crow::json::wvalue ResponseCDDSerial::to_json() const
{
  crow::json::wvalue r = Response::to_json();
  TO_JSON(cdd_serial);
  
  r["type"]= "response cdd serial";
  return r;
}

tl::expected<RequestCDDSerial,eError>
RequestCDDSerial::from_string(const std::string& str)
{
  auto json = crow::json::load(str);
  if (!json) {
    return tl::make_unexpected(eError::JSON_PARSE_ERROR);
  } else if (!json.has("message_reference")) {
    return tl::make_unexpected(eError::JSON_ERROR);
  } else {
    RequestCDDSerial r;
    r.message_reference= json["message_reference"].u();
    return r;
  }
}

crow::json::wvalue ResponseCDDC::to_json() const
{
  crow::json::wvalue r = Response::to_json();
  TO_JSON_JSON(cddc);
  r["type"]= "response cdd serial";
  return r;
}

tl::expected<RequestCDDC,eError>
RequestCDDC::from_string(const std::string& str)
{
  auto json = crow::json::load(str);
  if (!json) {
    return tl::make_unexpected(eError::JSON_PARSE_ERROR);
  } else if (!(json.has("cdd_serial")&&json.has("message_reference"))) {
    return tl::make_unexpected(eError::JSON_ERROR);
  } else {
    RequestCDDC r;
    r.cdd_serial=json["cdd_serial"].u();
    r.message_reference= json["message_reference"].u();
    return r;
  }
}

tl::expected<RequestMKCs,eError>
RequestMKCs::from_string(const std::string& str) {
  std::vector<unsigned int> denominations;
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  std::vector<unsigned int> mint_key_ids;
  //  "type": "request mint key certificates"

  auto json = crow::json::load(str);
  if (!json) {
    return tl::make_unexpected(eError::JSON_PARSE_ERROR);
  } else {
    return tl::make_unexpected(eError::NOT_IMPLEMENTED);
  }
}


crow::json::wvalue ResponseMKCs::to_json() const{  
  crow::json::wvalue r = Response::to_json();
  TO_JSON_ARRAY(keys);
  r["type"]= "response mint key certificates";
  return r;
}

crow::json::wvalue Blind::to_json() const {
  crow::json::wvalue r;
  TO_JSON(blinded_payload_hash);
  TO_JSON(mint_key_id);
  TO_JSON(reference);
  r["type"]= "blinded payload hash";
  return r;
} 

crow::json::wvalue BlindSignature::to_json() const {
  crow::json::wvalue r;
  TO_JSON(blind_signature);
  TO_JSON(reference);
  r["type"]= "blind signature";
  return r;
  
}

tl::expected<RequestMint,eError>
RequestMint::from_string(const std::string& str){
  std::vector<Blind> blinds;
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  //  "type": "request mint"
  auto json = crow::json::load(str);
  if (!json) {
    return tl::make_unexpected(eError::JSON_PARSE_ERROR);
  } else {
    return tl::make_unexpected(eError::NOT_IMPLEMENTED);
  }
}

crow::json::wvalue ResponseMint::to_json() const {
  crow::json::wvalue r = Response::to_json();
  TO_JSON_ARRAY(blind_signatures);
  r["type"]= "response mint";
  return r;
}

crow::json::wvalue Coin::Payload::to_json() const {
  crow::json::wvalue r;
  TO_JSON(cdd_location);
  TO_JSON(denomination);
  TO_JSON(issuer_id);
  TO_JSON(mint_key_id);
  TO_JSON(protocol_version);
  TO_JSON(serial);

  r["type"]= "payload";
  return r;
}

crow::json::wvalue Coin::to_json() const
{
  crow::json::wvalue r;
  TO_JSON_JSON(payload);
  TO_JSON(signature);
  r["type"]= "coin";
  return r;
}

crow::json::wvalue CoinStack::to_json() const {
  crow::json::wvalue r;
  TO_JSON_ARRAY(coins);
  TO_JSON(subject);
  r["type"]= "coinstack";
  return r;
}  


tl::expected<RequestRenew,eError>
RequestRenew::from_string(const std::string& str) {
  std::vector<Blind> blinds;
  std::vector<Coin> coins;
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  unsigned int transaction_reference; 
  //  "type": "request renew"
  auto json = crow::json::load(str);
  if (!json) {
    return tl::make_unexpected(eError::JSON_PARSE_ERROR);
  } else {
    return tl::make_unexpected(eError::NOT_IMPLEMENTED);
  }
}

crow::json::wvalue ResponseDelay::to_json() const {  
  crow::json::wvalue r = Response::to_json();
  r["type"]= "response delay";
  return r;
}

tl::expected<RequestResume,eError>
RequestResume::from_string(const std::string& str) {
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  unsigned int transaction_reference; 
  //  "type": "request resume"
  auto json = crow::json::load(str);
  if (!json) {
    return tl::make_unexpected(eError::JSON_PARSE_ERROR);
  } else {
    return tl::make_unexpected(eError::NOT_IMPLEMENTED);
  }
}

tl::expected<RequestRedeem,eError>
RequestRedeem::from_string(const std::string& str) {
  std::vector<Coin> coins;
  unsigned int message_reference; /// Client internal message reference.
                                  /// (Integer)
  unsigned int transaction_reference; 
  //  "type": "request redeem"
  auto json = crow::json::load(str);
  if (!json) {
    return tl::make_unexpected(eError::JSON_PARSE_ERROR);
  } else {
    return tl::make_unexpected(eError::NOT_IMPLEMENTED);
  }
}

crow::json::wvalue ResponseReedem::to_json() const {
  crow::json::wvalue r = Response::to_json();
  r["type"]= "response redeem";
  return r;
}  


/** this is for now our sample model */
class DefaultModel : public Model {
public:
  
  DefaultModel() {}
  const CDDC& getCDDC() override {return m_cddc; };
  const CDDC& getCurrentCDDC() override {return m_cddc; };
  void mint() override {};

private:
  CDDC m_cddc;
};

std::unique_ptr<Model> Model::getModel(const std::string& /*backend_name*/)
{
//:wq
//if (backend_name=="default")
    return std::make_unique<DefaultModel>();
}
