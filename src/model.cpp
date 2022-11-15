#include "model.hpp"
#include "crow/json.h"

#define TO_JSON(name) r[#name]=name
#define TO_JSON_JSON(name) r[#name]=name.to_json()
#define SEQ_TO_JSON(name) r[#name]=list_to_json(name)

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
  SEQ_TO_JSON(denominations);
  TO_JSON(id);
  SEQ_TO_JSON(info_service);
  TO_JSON(issuer_cipher_suite);
  TO_JSON_JSON(issuer_public_master_key);
  SEQ_TO_JSON(mint_service);
  TO_JSON(protocol_version); 
  SEQ_TO_JSON(redeem_service);
  SEQ_TO_JSON(renew_service); 

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

crow::json::wvalue ResponseCDDC::to_json() const
{
  crow::json::wvalue r;
  TO_JSON_JSON(cddc);
  TO_JSON(message_reference);
  TO_JSON(status_code);
  TO_JSON(status_description);
  
  r["type"]= "response cddc";
  return r;
}
std::optional<RequestCDDC> RequestCDDC::from_json(const crow::json::rvalue & json) {

  if (json.has("cdd_serial")&&json.has("message_reference")) {
    auto r = std::make_optional<RequestCDDC>();
    r->cdd_serial=json["cdd_serial"].u();
    r->message_reference= json["message_reference"].u();
    return r;
  }
  else {
    return std::nullopt;
  }
} 

/** blafasel */
class DefaultModel : public Model {
public:
  
  DefaultModel() {}
  const CDDC& getCDDC() override {return m_cddc; };
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
