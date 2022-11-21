#include <catch2/catch_test_macros.hpp>
#include "model.hpp"

TEST_CASE( "PublicKey::to_json", "[to_json]" ) {
  PublicKey k {"daaa63ddda38c189b8c49020c8276adbe0a695685a...",
	       "65537" };

  auto json = k.to_json();

  REQUIRE( json["modulus"].dump() == "\"" + k.modulus + "\""  );
  REQUIRE( json["public_exponent"].dump() == "\"" + k.public_exponent+"\"" );
  REQUIRE( json["type"].dump() == "\"rsa public key\"" );
  REQUIRE( json.keys().size() == 3 );
}
