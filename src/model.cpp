#include "model.hpp"
#include "crow/json.h"
#include "tl/expected.hpp"

#define TO_JSON(name) r[#name]=name
#define BIGINT_TO_JSON(name) r[#name]=name.to_string()
#define TO_JSON_JSON(name) r[#name]=name.to_json()
#define TO_JSON_ARRAY(name) r[#name]=list_to_json(name)

template <class T>
crow::json::wvalue list_to_json(const std::vector<T> &array) {
  crow::json::wvalue::list l;
  for (auto item : array)
    l.push_back(item.to_json());
  return crow::json::wvalue(l);
}

crow::json::wvalue list_to_json(const std::vector<unsigned int> &array) {
  crow::json::wvalue::list l;
  for (auto item : array)
    l.push_back(item);
  return crow::json::wvalue(l);
}

crow::json::wvalue PublicKey::to_json() const {
  crow::json::wvalue r;
  BIGINT_TO_JSON(modulus);
  BIGINT_TO_JSON(public_exponent);
  r["type"]="rsa public key";
  return r;
}

crow::json::wvalue WeightedUrl::to_json() const {
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
  TO_JSON(currency_name);
  TO_JSON_ARRAY(denominations);
  BIGINT_TO_JSON(id);
  TO_JSON_ARRAY(info_service);
  TO_JSON(issuer_cipher_suite);
  TO_JSON_JSON(issuer_public_master_key);
  TO_JSON_ARRAY(mint_service);
  TO_JSON(protocol_version);
  TO_JSON_ARRAY(redeem_service);
  TO_JSON_ARRAY(renew_service);

  r["type"] = "cdd";
  return r;
}

crow::json::wvalue CDDC::to_json() const {
  crow::json::wvalue r;
  TO_JSON_JSON(cdd);
  TO_JSON(signature);
  r["type"] = "cdd certificate";
  return r;
}

crow::json::wvalue MintKey::to_json() const {
  crow::json::wvalue r;

  TO_JSON(cdd_serial);
  TO_JSON(coins_expiry_date);
  TO_JSON(denomination);
  BIGINT_TO_JSON(id);
  BIGINT_TO_JSON(issuer_id);
  TO_JSON_JSON(public_mint_key);

  TO_JSON(sign_coins_not_after);
  TO_JSON(sign_coins_not_before);

  r["type"] = "mint key";
  return r;
}

crow::json::wvalue MintKeyCert::to_json() const {
  crow::json::wvalue r;
  TO_JSON_JSON(mint_key);
  TO_JSON(signature);
  r["type"] = "mint key certificate";
  return r;
}

crow::json::wvalue Response::to_json() const {
  crow::json::wvalue r;
  TO_JSON(message_reference);
  TO_JSON(status_code);
  TO_JSON(status_description);
  return r;
}

crow::json::wvalue ResponseCDDSerial::to_json() const {
  crow::json::wvalue r = Response::to_json();
  TO_JSON(cdd_serial);

  r["type"] = "response cdd serial";
  return r;
}

tl::expected<RequestCDDSerial, eError>
RequestCDDSerial::from_string(const std::string &str) {
  auto json = crow::json::load(str);
  if (!json) {
    return tl::make_unexpected(eError::JSON_PARSE_ERROR);
  } else if (!json.has("type") || !json.has("message_reference")) {
    return tl::make_unexpected(eError::JSON_ERROR);
  } else if (json["type"] != "request cdd serial") {
    return tl::make_unexpected(eError::JSON_ERROR);
  } else {
    RequestCDDSerial r;
    r.message_reference = json["message_reference"].u();
    return r;
  }
}

crow::json::wvalue ResponseCDDC::to_json() const {
  crow::json::wvalue r = Response::to_json();
  TO_JSON_JSON(cddc);
  r["type"] = "response cdd serial";
  return r;
}

tl::expected<RequestCDDC, eError>
RequestCDDC::from_string(const std::string &str) {
  auto json = crow::json::load(str);
  if (!json) {
    return tl::make_unexpected(eError::JSON_PARSE_ERROR);
  } else if (!(json.has("type") && json.has("message_reference") &&
               json.has("cdd_serial"))) {
    return tl::make_unexpected(eError::JSON_MISSING_KEY);
  } else if (json["type"] != "request cddc") {
    return tl::make_unexpected(eError::JSON_WRONG_REQUEST_TYPE);
  } else {
    RequestCDDC r;
    r.cdd_serial = json["cdd_serial"].u();
    r.message_reference = json["message_reference"].u();
    return r;
  }
}

tl::expected<RequestMKCs, eError>
RequestMKCs::from_string(const std::string &str) {
  auto json = crow::json::load(str);
  if (!json) {
    return tl::make_unexpected(eError::JSON_PARSE_ERROR);
  } else if (!(json.has("denominations") && json.has("message_reference") &&
               json.has("mint_key_ids") && json.has("type"))) {
    return tl::make_unexpected(eError::JSON_MISSING_KEY);
  } else if (json["type"] != "request mint key certificates") {
    return tl::make_unexpected(eError::JSON_WRONG_REQUEST_TYPE);
  } else {
    RequestMKCs r;
    r.message_reference = json["message_reference"].u();

    auto denominations = json["denominations"];
    if (denominations.t() != crow::json::type::List) {
      return tl::make_unexpected(eError::JSON_WRONG_REQUEST_TYPE);
    } else {
      for (auto d : denominations.lo()) {
        r.denominations.push_back(d.u());
      }
    }
    auto mint_key_ids = json["mint_key_ids"];
    if (mint_key_ids.t() != crow::json::type::List) {
      return tl::make_unexpected(eError::JSON_WRONG_REQUEST_TYPE);
    } else {
      for (auto k: mint_key_ids.lo()) {
	auto kv = BigInt::from_string(k.s());
	if (!kv.has_value()) {
	  return tl::make_unexpected(eError::JSON_PARSE_ERROR);
	} else {
	  r.mint_key_ids.push_back(*kv);
	}
      }
    }
    return r;
  }
}

crow::json::wvalue ResponseMKCs::to_json() const {
  crow::json::wvalue r = Response::to_json();
  TO_JSON_ARRAY(keys);
  r["type"] = "response mint key certificates";
  return r;
}

crow::json::wvalue Blind::to_json() const {
  crow::json::wvalue r;
  BIGINT_TO_JSON(blinded_payload_hash);
  BIGINT_TO_JSON(mint_key_id);
  TO_JSON(reference);
  r["type"] = "blinded payload hash";
  return r;
}

tl::expected<Blind, eError> Blind::from_json(const crow::json::rvalue &json) {
  if (!(json.has("type")
	&& json.has("blinded_payload_hash")
	&& json.has("mint_key_id")
	&& json.has("reference"))) {
    return tl::make_unexpected(eError::JSON_ERROR);
  } else if (json["type"] != "blinded payload hash") {
    return tl::make_unexpected(eError::JSON_ERROR);
  } else {
    Blind r;

    auto hash = BigInt::from_string(json["blinded_payload_hash"].s());
    if (!hash) {
      // std::cout << "invalid hash : " <<  json["blinded_payload_hash"].s()
      // 		<< std::endl;
      return tl::make_unexpected(eError::JSON_PARSE_ERROR);
    }
    r.blinded_payload_hash = hash.value();

    auto key_id = BigInt::from_string(json["mint_key_id"].s());
    if (!key_id) {
      return tl::make_unexpected(eError::JSON_PARSE_ERROR);
    }
    r.mint_key_id = key_id.value();
    r.reference = json["reference"].s();
    return r;
  }
}

crow::json::wvalue BlindSignature::to_json() const {
  crow::json::wvalue r;
  BIGINT_TO_JSON(blind_signature);
  TO_JSON(reference);
  r["type"] = "blind signature";
  return r;
}

tl::expected<RequestMint, eError>
RequestMint::from_string(const std::string &str) {
  std::vector<Blind> blinds;
  //  "type": "request mint"
  auto json = crow::json::load(str);
  if (!json) {
    return tl::make_unexpected(eError::JSON_PARSE_ERROR);
  } else if (!(json.has("type") && json.has("message_reference") &&
               json.has("transaction_reference") && json.has("blinds"))) {
    return tl::make_unexpected(eError::JSON_ERROR);
  } else if (json["type"] != "request mint") {
    return tl::make_unexpected(eError::JSON_ERROR);
  } else {
    RequestMint r;
    r.message_reference= json["message_reference"].u();
    auto tr = BigInt::from_string(json["transaction_reference"].s());
    if (!tr)
      return tl::make_unexpected(eError::JSON_PARSE_ERROR);
      
    r.transaction_reference = *tr;
    if (json["blinds"].t()!=crow::json::type::List) {
      return tl::make_unexpected(eError::JSON_WRONG_VALUE_TYPE);
    }

    for (auto item : json["blinds"]) {
      auto b = Blind::from_json(item);
      if (!b.has_value()) {
        return tl::make_unexpected(b.error());
      } else {
        r.blinds.push_back(b.value());
      }
    }
    return r;
  }
}

crow::json::wvalue ResponseMint::to_json() const {
  crow::json::wvalue r = Response::to_json();
  TO_JSON_ARRAY(blind_signatures);
  r["type"] = "response mint";
  return r;
}

crow::json::wvalue Coin::Payload::to_json() const {
  crow::json::wvalue r;
  TO_JSON(cdd_location);
  TO_JSON(denomination);
  BIGINT_TO_JSON(issuer_id);
  BIGINT_TO_JSON(mint_key_id);
  TO_JSON(protocol_version);
  BIGINT_TO_JSON(serial);

  r["type"] = "payload";
  return r;
}

crow::json::wvalue Coin::to_json() const {
  crow::json::wvalue r;
  TO_JSON_JSON(payload);
  TO_JSON(signature);
  r["type"] = "coin";
  return r;
}

tl::expected<Coin::Payload, eError>
Coin::Payload::from_json(const crow::json::rvalue &json) {
  if (!(json.has("cdd_location") && json.has("denomination") &&
        json.has("issuer_id") && json.has("mint_key_id") &&
        json.has("protocol_version") && json.has("serial") &&
        json.has("type"))) {
    return tl::make_unexpected(eError::JSON_ERROR);
  } else if (json["type"] != "payload") {
    return tl::make_unexpected(eError::JSON_ERROR);
  } else {
    Coin::Payload payload;
    payload.cdd_location = json["cdd_location"].s();
    payload.denomination = json["denomination"].u();
    auto id = BigInt::from_string(json["issuer_id"].s());
    if (!id)
      tl::make_unexpected(eError::JSON_PARSE_ERROR);
    payload.issuer_id    = *id;

    id = BigInt::from_string(json["mint_key_id"].s());
    if (!id)
      tl::make_unexpected(eError::JSON_PARSE_ERROR);
    payload.mint_key_id  = *id;
    payload.protocol_version = json["protocol_version"].s();
    id = BigInt::from_string(json["serial"].s());
    if (!id)
      tl::make_unexpected(eError::JSON_PARSE_ERROR);
    payload.serial      = *id;
    return payload;
  }
}

tl::expected<Coin, eError> Coin::from_json(const crow::json::rvalue &json) {
  if (!(json.has("type") && json.has("payload") && json.has("signature"))) {
    return tl::make_unexpected(eError::JSON_ERROR);
  } else if (json["type"] != "coin") {
    return tl::make_unexpected(eError::JSON_ERROR);
  } else {
    auto pl = Payload::from_json(json["payload"]);
    if (!pl.has_value()) {
      return tl::make_unexpected(pl.error());
    } else {
      Coin c;
      c.payload = pl.value();
      c.signature = json["signature"].s();
      return c;
    }
  }
}

crow::json::wvalue CoinStack::to_json() const {
  crow::json::wvalue r;
  TO_JSON_ARRAY(coins);
  TO_JSON(subject);
  r["type"] = "coinstack";
  return r;
}

tl::expected<RequestRenew, eError>
RequestRenew::from_string(const std::string &str) {
  auto json = crow::json::load(str);

  if (!json) {
    return tl::make_unexpected(eError::JSON_PARSE_ERROR);
  } else if (!(json.has("blinds") && json.has("coins") &&
               json.has("transaction_reference") &&
               json.has("message_reference") && json.has("type"))) {
    return tl::make_unexpected(eError::JSON_MISSING_KEY);
  } else if (json["type"] != "request renew") {
    return tl::make_unexpected(eError::JSON_WRONG_REQUEST_TYPE);
  } else if ((json["coins"].t() != crow::json::type::List) ||
             (json["blinds"].t() != crow::json::type::List)) {
    return tl::make_unexpected(eError::JSON_WRONG_VALUE_TYPE);
  } else {
    RequestRenew r;

    for (auto item : json["coins"]) {
      auto coin = Coin::from_json(item);
      if (!coin.has_value()) {
        return tl::make_unexpected(coin.error());
      } else {
        r.coins.push_back(coin.value());
      }
    }

    for (auto item : json["blinds"]) {
      auto blind = Blind::from_json(item);
      if (!blind.has_value()) {
        return tl::make_unexpected(blind.error());
      } else {
        r.blinds.push_back(blind.value());
      }
    }

    r.message_reference = json["message_reference"].u();
    r.transaction_reference = json["transaction_reference"].s();
    return r;
  }
}

crow::json::wvalue ResponseDelay::to_json() const {
  crow::json::wvalue r = Response::to_json();
  r["type"] = "response delay";
  return r;
}

tl::expected<RequestResume, eError>
RequestResume::from_string(const std::string &str) {
  auto json = crow::json::load(str);
  if (!json) {
    return tl::make_unexpected(eError::JSON_PARSE_ERROR);
  } else if (!(json.has("transaction_reference") &&
               json.has("message_reference") && json.has("type"))) {
    return tl::make_unexpected(eError::JSON_MISSING_KEY);
  } else if (json["type"] != "request resume") {
    return tl::make_unexpected(eError::JSON_WRONG_REQUEST_TYPE);
  } else {
    RequestResume r;
    r.message_reference = json["message_reference"].u();
    auto tr = BigInt::from_string(json["transaction_reference"].s());
    if (!tr)
      return tl::make_unexpected(eError::JSON_PARSE_ERROR);
    r.transaction_reference = *tr;
    return r;
  }
}

tl::expected<RequestRedeem, eError>
RequestRedeem::from_string(const std::string &str) {
  //  "type":
  auto json = crow::json::load(str);
  if (!json) {
    return tl::make_unexpected(eError::JSON_PARSE_ERROR);
  } else if (!(json.has("coins") && json.has("message_reference") &&
               json.has("type"))) {
    return tl::make_unexpected(eError::JSON_MISSING_KEY);
  } else if (json["type"] != "request redeem") {
    return tl::make_unexpected(eError::JSON_WRONG_REQUEST_TYPE);
  } else {
    RequestRedeem r;
    r.message_reference = json["message_reference"].u();
    if (json["coins"].t() != crow::json::type::List) {
      return tl::make_unexpected(eError::JSON_WRONG_VALUE_TYPE);
    }

    for (auto item : json["coins"]) {
      auto coin = Coin::from_json(item);
      if (!coin.has_value()) {
        return tl::make_unexpected(coin.error());
      } else {
        r.coins.push_back(coin.value());
      }
    }
    return r;
  }
}

crow::json::wvalue ResponseRedeem::to_json() const {
  crow::json::wvalue r = Response::to_json();
  r["type"] = "response redeem";
  return r;
}

using std::cout;
using std::endl;

/** this is for now our sample model */
class DefaultModel : public Model {
public:
  DefaultModel() {}
  tl::expected<CDDC *, bool> getCDDC(unsigned int cdd_serial) override {
    cout << __FUNCTION__ << "(" << cdd_serial << ")" << endl;
    return &m_cddc;
  };

  tl::expected<CDDC *, bool> getCurrentCDDC() override {
    cout << __FUNCTION__ << "()" << endl;

    return &m_cddc;
  }

  std::vector<BlindSignature> mint(const std::string& transaction_reference,
                                   const std::vector<Blind> &blinds) override {
    std::vector<BlindSignature> res;
    cout << __FUNCTION__ << "("
         << ")" << endl;

    return res;
  }

  const std::vector<MintKeyCert>
  getMKCs(const std::vector<unsigned int> &denominations,
          const std::vector<BigInt> &mint_key_ids) override {
    std::vector<MintKeyCert> res;
    cout << __FUNCTION__ << endl;
    return res;
  }
  bool redeem(const std::vector<Coin> &coins) override {
    cout << __FUNCTION__ << endl;
    return false;
  }

private:
  CDDC m_cddc;
};

std::unique_ptr<Model> Model::getModel(const std::string & /*backend_name*/) {
  cout << __FUNCTION__ << endl;
  //:wq
  // if (backend_name=="default")
  return std::make_unique<DefaultModel>();
}
