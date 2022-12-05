#include "crow/json.h"
#include "model.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE( "PublicKey::to_json", "[to_json]" ) {
  
  PublicKey k //{ BigInt::from_string("daaa63ddda38c189b8c49020c8276adbe0a695685a...").value(),
    //    BigInt::from_int(65537UL) };
    ;
  auto json = k.to_json();

  REQUIRE( json["modulus"].dump() == "\"" + k.modulus.to_string() + "\""  );
  REQUIRE( json["public_exponent"].dump() == "\"" + k.public_exponent.to_string()+"\"" );
  REQUIRE( json["type"].dump() == "\"rsa public key\"" );
  REQUIRE( json.keys().size() == 3 );
}

TEST_CASE("RequestCDDSerial::from_string", "[from_string]") {
  // good case
  std::string good = "{"
                     "\"message_reference\": 100000,"
                     "\"type\": \"request cdd serial\""
                     "}";

  auto res = RequestCDDSerial::from_string(good);
  REQUIRE(res.has_value() == true);
  REQUIRE(res->message_reference == 100000);

  // bad cases
  res = RequestCDDSerial::from_string("");
  REQUIRE(res.has_value() == false);

  // invalid type
  res = RequestCDDSerial::from_string("{"
                                      "\"message_reference\": 100000,"
                                      "\"type\": \"request something wrong\""
                                      "}");
  REQUIRE(res.has_value() == false);

  // invalid attribute name
  res = RequestCDDSerial::from_string("{"
                                      "\"x_message_reference\": 100000,"
                                      "}");
  REQUIRE(res.has_value() == false);
}

TEST_CASE("RequestCDDC::from_string", "[from_string]") {
  // good case
  auto res = RequestCDDC::from_string("{"
                                      "\"cdd_serial\": 1,"
                                      "\"message_reference\": 100001,"
                                      "\"type\": \"request cddc\""
                                      "}");
  REQUIRE(res.has_value() == true);
  REQUIRE(res->cdd_serial == 1);
  REQUIRE(res->message_reference == 100001);

  // bad cases
  res = RequestCDDC::from_string("");
  REQUIRE(res.has_value() == false);

  // invalid type
  res = RequestCDDC::from_string("{"
                                 "\"cdd_serial\": 1,"
                                 "\"message_reference\": 100001,"
                                 "\"type\": \"wrong type\","
                                 "}");
  REQUIRE(res.has_value() == false);

  // invalid attribute name
  res = RequestCDDC::from_string("{"
                                 "\"cdd_serial\": 1,"
                                 "\"x_ message_reference\": 100001,"
                                 "\"type\": \"request cddc\","
                                 "}");
  REQUIRE(res.has_value() == false);
}

TEST_CASE("RequestMKCs::from_string", "[from_string]") {
  // good case
  auto res = RequestMKCs::from_string( "{"
				       "\"denominations\": [1, 2, 5],"
				       "\"message_reference\": 100002,"
				       "\"mint_key_ids\": [],"
				       "\"type\": \"request mint key certificates\""
				       "}");
  const std::vector<uint32_t> EXPECTED_DENOMINATIONS = {1,2,5};
  const std::vector<BigInt> EXPECTED_MINT_KEY_IDS = {};

  REQUIRE(res.has_value() == true);
  REQUIRE(res->denominations == EXPECTED_DENOMINATIONS);
  REQUIRE(res->message_reference == 100002);
  REQUIRE(res->mint_key_ids == EXPECTED_MINT_KEY_IDS);

  // bad cases
  res = RequestMKCs::from_string("");
  REQUIRE(res.has_value() == false);

  // invalid type
  res = RequestMKCs::from_string("{"
                                 "\"denominations\": [1, 2, 5],"
                                 "\"message_reference\": 100002,"
                                 "\"mint_key_ids\": [],"
                                 "\"type\": \"request wrong type\""
                                 "}");
  REQUIRE(res.has_value() == false);

  // invalid attribute name
  res =
      RequestMKCs::from_string("{"
                               "\"denominations\": [1, 2, 5],"
                               "\"x_message_reference\": 100002," // WRONG NAME
                               "\"mint_key_ids\": [],"
                               "\"type\": \"request wrong type\""
                               "}");
  REQUIRE(res.has_value() == false);

  // invalid attribute type (not an array as expected)
  res = RequestMKCs::from_string("{"
                                 "\"denominations\": 1,"
                                 "\"message_reference\": 100002," // WRONG NAME
                                 "\"mint_key_ids\": [],"
                                 "\"type\": \"request wrong type\""
                                 "}");
  REQUIRE(res.has_value() == false);
}

TEST_CASE("Blind::from_json", "[from_string]") {

  auto good = crow::json::load(
      "{"
      "\"blinded_payload_hash\": "
      "\"924edb672c3345492f38341ff86b57181da4c673efd5fd76c0f64e369974c678f89ffcb1c5f77bf04911a9a63831b52675b70c06548a4d945ec5dd1d825ab08\","
      "\"mint_key_id\": \"1ceb977bb531c65f133ab8b0d60862b17369d96\","
      "\"reference\": \"a0\","
      "\"type\": \"blinded payload hash\""
      "}");

  auto res = Blind::from_json(good);
  REQUIRE(res.has_value()==true);
  REQUIRE(res->blinded_payload_hash.to_string()==
	  "924edb672c3345492f38341ff86b57181da4c673efd5fd76c0f64e369974c678f89ffcb1c5f77bf04911a9a63831b52675b70c06548a4d945ec5dd1d825ab08");
  REQUIRE(res->mint_key_id.to_string()=="1ceb977bb531c65f133ab8b0d60862b17369d96");
  REQUIRE(res->reference=="a0");
  // bad cases 
  // wrong_type["blinded_payload_hash"]= "924edb672c3345492f38341ff86b57181da4c673ef...";
  // wrong_type["mint_key_id"]= "1ceb977bb531c65f133ab8b0d60862b17369d96";
  // wrong_type["reference"] = "a0";
  // wrong_type["type"]= "wrong type";

  // res = Blind::from_json(wrong_type);
}

TEST_CASE("RequestMint::from_string", "[from_string]") {

  // good case
  auto res = RequestMint::from_string( "{"
				       "\"blinds\": ["
				       "{"
				       "\"blinded_payload_hash\": \"924edb672c3345492f38341ff86b57181da4c673efd5fd76c0f64e369974c678f89ffcb1c5f77bf04911a9a63831b52675b70c06548a4d945ec5dd1d825ab08\","
				       "\"mint_key_id\": \"1ceb977bb531c65f133ab8b0d60862b17369d96\","
				       "\"reference\": \"a0\","
				       "\"type\": \"blinded payload hash\""
				       "},"
				       "{"
				       "\"blinded_payload_hash\": \"95db92e1c46ebea5edec5e508a831263de6fb78b4cf9187593f6af815b51db9db35ad5eaf2c0c83bd7e13c999df4f0f1af65b367eb7c2b6addb9735dce156b5\","
				       "\"mint_key_id\": \"f2864e5cd937dbaa4825e73a81062de162143682\","
				       "\"reference\": \"a1\","
				       "\"type\": \"blinded payload hash\""
				       "},"
				       "{"
				       "\"blinded_payload_hash\": \"10afac98ac43eb40e996c621d5db4d2238348e3f74850856d940955da0fd24fa4d3aee79da1e9da24e85cf9cefd96feaca5b26a9353a3d9fcb4bd34145046ce8\","
				       "\"mint_key_id\": \"897a16bf12bd9ba474ef7be0e3a53553a7b4ece8\","
				       "\"reference\": \"a2\","
				       "\"type\": \"blinded payload hash\""
				       "}"
				       "],"
				       "\"message_reference\": 100003,"
				       "\"transaction_reference\": \"b2221a58008a05a6c4647159c324c985\","
				       "\"type\": \"request mint\""
				      "}");

  REQUIRE(res.has_value()==true);
  REQUIRE(res->message_reference==100003);
  REQUIRE(res->transaction_reference.to_string() == "b2221a58008a05a6c4647159c324c985");
  /// \todo check blinds

  // bad cases
  res = RequestMint::from_string("");
  REQUIRE(res.has_value() == false);

  // invalid type
  res = RequestMint::from_string(
      "{"
      "\"blinds\": [],"
      "\"message_reference\": 100003,"
      "\"transaction_reference\": \"b2221a58008a05a6c4647159c324c985\","
      "\"type\": \"request wrong type\""
      "}");
  REQUIRE(res.has_value() == false);

  // invalid attribute name
  res = RequestMint::from_string(
      "{"
      "\"blinds\": [],"
      "\"x_message_reference\": 100003,"
      "\"transaction_reference\": \"b2221a58008a05a6c4647159c324c985\","
      "\"type\": \"request mint\""
      "}");
  REQUIRE(res.has_value() == false);

  // invalid attribute type (not an array as expected)
  res = RequestMint::from_string(
      "{"
      "\"blinds\": 1,"
      "\"message_reference\": 100003,"
      "\"transaction_reference\": \"b2221a58008a05a6c4647159c324c985\","
      "\"type\": \"request mint\""
      "}");
  REQUIRE(res.has_value() == false);
}

TEST_CASE("RequestRenew::from_string", "[from_string]") {
  auto res = RequestRenew::from_string(
      "{"
      "  \"blinds\": ["
      "    {"
      "      \"blinded_payload_hash\": "
      "\"7ed0cda1c1b36f544514b12848b8436974b7b9f6c7231ebcc566678e3478d3279db03872e4710413ad20a55d56ef12fb1800ac187195322535a626e178931cf9\","
      "      \"mint_key_id\": \"f2864e5cd937dbaa4825e73a81062de162143682\","
      "      \"reference\": \"b0\","
      "      \"type\": \"blinded payload hash\""
      "    }"
      "  ],"
      "  \"coins\": ["
      "    {"
      "      \"payload\": {"
      "        \"cdd_location\": \"https://opencent.org\","
      "        \"denomination\": 1,"
      "        \"issuer_id\": \"23ed956e629ba35f0002eaf833ea436aea7db5c2\","
      "        \"mint_key_id\": \"1ceb977bb531c65f133ab8b0d60862b17369d96\","
      "        \"protocol_version\": \"https://opencoin.org/1.0\","
      "        \"serial\": \"cd613e30d8f16adf91b7584a2265b1f5\","
      "        \"type\": \"payload\""
      "      },"
      "      \"signature\": \"2ec0af339566b19fb9867b491ce58025dcefcab649...\","
      "      \"type\": \"coin\""
      "    }"
      "  ],"
      "  \"message_reference\": 100004,"
      "  \"transaction_reference\": \"ad45f23d3b1a11df587fd2803bab6c39\","
      "  \"type\": \"request renew\""
      "}");
  REQUIRE(res.has_value() == true);

  // invalid attribute name
  res = RequestRenew::from_string(
      "{"
      "\"blinds\": [],"
      "\"coins\": [],"
      "\"x_message_reference\": 100003,"
      "\"transaction_reference\": \"b2221a58008a05a6c4647159c324c985\","
      "\"type\": \"request mint\""
      "}");
  REQUIRE(res.has_value() == false);

  // invalid attribute type (not an array as expected)
  res = RequestRenew::from_string(
      "{"
      "\"blinds\": 1,"
      "\"coins\": [],"
      "\"message_reference\": 100003,"
      "\"transaction_reference\": \"b2221a58008a05a6c4647159c324c985\","
      "\"type\": \"request mint\""
      "}");
  REQUIRE(res.has_value() == false);
}

TEST_CASE("RequestResume::from_string", "[from_string]") {
  // good case
  auto res = RequestResume::from_string( "{"
				       "\"message_reference\": 100005,"
				       "\"transaction_reference\": \"ad45f23d3b1a11df587fd2803bab6c39\","
				       "\"type\": \"request resume\""
				       "}");

  REQUIRE(res.has_value()==true);
  REQUIRE(res->message_reference==100005);
  REQUIRE(res->transaction_reference.to_string()=="ad45f23d3b1a11df587fd2803bab6c39");
  
  // bad cases
  res = RequestResume::from_string("");
  REQUIRE(res.has_value() == false);

  // invalid type
  res = RequestResume::from_string(
      "{"
      "\"message_reference\": 100005,"
      "\"transaction_reference\": \"ad45f23d3b1a11df587fd2803bab6c39\","
      "\"type\": \"Wrong type\""
      "}");
  REQUIRE(res.has_value() == false);

  // invalid attribute name
  res = RequestResume::from_string("{"
                                   "\"message_reference\": 100005,"
                                   "\"WRONG_ATTR_transaction_reference\": "
                                   "\"ad45f23d3b1a11df587fd2803bab6c39\","
                                   "\"type\": \"request resume\""
                                   "}");
  REQUIRE(res.has_value() == false);
}

TEST_CASE("RequestRedeem::from_string", "[from_string]") {

  auto res = RequestRedeem::from_string(
      "{"
      "\"coins\": ["
      "{"
      "\"payload\": {"
      "\"cdd_location\": \"https://opencent.org\","
      "\"denomination\": 2,"
      "\"issuer_id\": \"23ed956e629ba35f0002eaf833ea436aea7db5c2\","
      "\"mint_key_id\": \"f2864e5cd937dbaa4825e73a81062de162143682\","
      "\"protocol_version\": \"https://opencoin.org/1.0\","
      "\"serial\": \"cd447e35b8b6d8fe442e3d437204e52d\","
      "\"type\": \"payload\""
      "},"
      "\"signature\": \"11b6bfa18134c300f4440df1db17a08fa71a071b71...\","
      "\"type\": \"coin\""
      "},"
      "{"
      "\"payload\": {"
      "\"cdd_location\": \"https://opencent.org\","
      "\"denomination\": 2,"
      "\"issuer_id\": \"23ed956e629ba35f0002eaf833ea436aea7db5c2\","
      "\"mint_key_id\": \"f2864e5cd937dbaa4825e73a81062de162143682\","
      "\"protocol_version\": \"https://opencoin.org/1.0\","
      "\"serial\": \"5b6e6e307d4bedc51431193e6c3f339\","
      "\"type\": \"payload\""
      "},"
      "\"signature\": \"a6dd7b7f1f12c4e411289e8ea0355f24a8597bbc38...\","
      "\"type\": \"coin\""
      "}"
      "],"
      "\"message_reference\": 100006,"
      "\"type\": \"request redeem\""
      "}");

  REQUIRE(res.has_value() == true);
  REQUIRE(res->message_reference == 100006);

  // bad cases
  res = RequestRedeem::from_string("");
  REQUIRE(res.has_value() == false);

  // invalid type
  res = RequestRedeem::from_string("{"
                                   "\"coins\": [],"
                                   "\"message_reference\": 100006,"
                                   "\"type\": \"WRONG TYPE\""
                                   "}");
  REQUIRE(res.has_value() == false);

  // invalid attribute name
  res = RequestRedeem::from_string("{"
                                   "\"INVALID_coins\": [],"
                                   "\"message_reference\": 100006,"
                                   "\"type\": \"request redeem\""
                                   "}");
  REQUIRE(res.has_value() == false);
  // invalid coin type
  res = RequestRedeem::from_string("{"
                                   "\"coins\": 1,"
                                   "\"message_reference\": 100006,"
                                   "\"type\": \"request redeem\""
                                   "}");
  REQUIRE(res.has_value() == false);
}
