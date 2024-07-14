
#include <catch2/catch_test_macros.hpp>

//#include "cryptlib.h"
//#include "integer.h"
//#include "nbtheory.h"
//#include "osrng.h"
//#include "rsa.h"
//#include "sha.h"

#include <iostream>
#include <stdexcept>

/*
CryptoPP::Integer blind_signature(const CryptoPP::SecByteBlock &orig,
                                  const CryptoPP::Integer &r,
                                  const CryptoPP::RSA::PublicKey &pub_key,
                                  const CryptoPP::RSA::PrivateKey &priv_key) {
    using namespace CryptoPP;
    using std::cout;
    using std::endl;

    // Convenience
    const Integer &n = pub_key.GetModulus();
    const Integer &e = pub_key.GetPublicExponent();
//  const Integer &d = priv_key.GetPrivateExponent();

// For sizing the hashed message buffer. This should be SHA256 size.
    const size_t sig_size = UnsignedMin(SHA256::BLOCKSIZE, n.ByteCount());
// Scratch
    SecByteBlock buff_1, buff_2, buff_3;

    Integer m(orig.data(), orig.size());
    cout << "Message: " << std::hex << m << endl;

    // Hash message per Rabin (1979)
    buff_1.resize(sig_size);
    SHA256 hash_1;
    hash_1.CalculateTruncatedDigest(buff_1, buff_1.size(), orig, orig.size());

    // H(m) as Integer
    Integer hm(buff_1.data(), buff_1.size());
    cout << "H(m): " << std::hex << hm << endl;

    // Blinding factor
    Integer b = a_exp_b_mod_c(r, e, n);
    cout << "Random: " << std::hex << b << endl;

    // Alice blinded message
    Integer mm = a_times_b_mod_c(hm, b, n);
    cout << "Blind msg: " << std::hex << mm << endl;

    AutoSeededRandomPool prng;

    // Bob sign
    Integer ss = priv_key.CalculateInverse(prng, mm);
    cout << "Blind sign: " << ss << endl;

    return ss;
}

CryptoPP::Integer unblind_signature(const CryptoPP::Integer &ss,
                                    const CryptoPP::Integer &r,
                                    const CryptoPP::RSA::PublicKey &pub_key) {

    const CryptoPP::Integer &n = pub_key.GetModulus();
    CryptoPP::Integer s = a_times_b_mod_c(ss, r.InverseMod(n), n);
    return s;
}

CryptoPP::Integer verify(CryptoPP::Integer const &ss,
                         const CryptoPP::Integer &r,
                         const CryptoPP::RSA::PublicKey &pub_key) {
    CryptoPP::Integer s = unblind_signature(ss, r, pub_key);
    CryptoPP::Integer v = pub_key.ApplyFunction(s);
    return v;
}

TEST_CASE("cryptopp1", "[crypto]") {
    using namespace CryptoPP;
    using std::cout;
    using std::endl;
    using std::runtime_error;

    // Bob artificially small key pair
    AutoSeededRandomPool prng;
    RSA::PrivateKey priv_key;

    priv_key.GenerateRandomWithKeySize(prng, 64U);
    RSA::PublicKey pub_key(priv_key);

    // Convenience
    const Integer &n = pub_key.GetModulus();
    const Integer &e = pub_key.GetPublicExponent();
    const Integer &d = priv_key.GetPrivateExponent();

    // Print params
    cout << "Pub mod: " << std::hex << pub_key.GetModulus() << endl;
    cout << "Pub exp: " << std::hex << e << endl;
    cout << "Priv mod: " << std::hex << priv_key.GetModulus() << endl;
    cout << "Priv exp: " << std::hex << d << endl;
    const char *MESSAGE = "secret";
    SecByteBlock orig((const byte *) MESSAGE, 6U);

    // Alice blinding
    Integer r;
    do {
        r.Randomize(prng, Integer::One(), n - Integer::One());
    } while (!RelativelyPrime(r, n));
    CryptoPP::Integer ss = blind_signature(orig,
                                           r,
                                           pub_key,
                                           priv_key);
    // Alice checks s(s'(x)) = x. This is from Chaum's paper
    Integer c = pub_key.ApplyFunction(ss);
    cout << "Check sign: " << c << endl;
    //if (c != mm) {
    //   throw runtime_error("Alice cross-check failed");
    // }
    // Alice remove blinding
    Integer s = unblind_signature(ss, r, pub_key);
    cout << "Unblind sign: " << s << endl;

    // Eve verifies
    Integer v = verify(ss, r, pub_key);
    cout << "Verify: " << std::hex << v << endl;

    // Scratch
    SecByteBlock buff_2, buff_3;

    // Convert to a string
    size_t req = v.MinEncodedSize();
    buff_2.resize(req);
    v.Encode(&buff_2[0], buff_2.size());

    // Hash message per Rabin (1979)
    const size_t sig_size = UnsignedMin(SHA256::BLOCKSIZE, n.ByteCount());
    buff_3.resize(sig_size);
    SHA256 hash_2;
    hash_2.CalculateTruncatedDigest(buff_3, buff_3.size(), orig, orig.size());

    // Constant time compare
    bool equal = buff_2.size() == buff_3.size() &&
                 VerifyBufsEqual(buff_2.data(), buff_3.data(), buff_3.size());

    if (!equal) {
        throw runtime_error("Eve verified failed");
    }
    cout << "Verified signature" << endl;


}
*/
/*
TEST_CASE("cryptopp", "[crypto]") {
    using namespace CryptoPP;
    using std::cout;
    using std::endl;
    using std::runtime_error;

    // Bob artificially small key pair
    AutoSeededRandomPool prng;
    RSA::PrivateKey priv_key;

    priv_key.GenerateRandomWithKeySize(prng, 64U);
    RSA::PublicKey pub_key(priv_key);

    // Convenience
    const Integer &n = pub_key.GetModulus();
    const Integer &e = pub_key.GetPublicExponent();
    const Integer &d = priv_key.GetPrivateExponent();

    // Print params
    cout << "Pub mod: " << std::hex << pub_key.GetModulus() << endl;
    cout << "Pub exp: " << std::hex << e << endl;
    cout << "Priv mod: " << std::hex << priv_key.GetModulus() << endl;
    cout << "Priv exp: " << std::hex << d << endl;

    // For sizing the hashed message buffer. This should be SHA256 size.
    const size_t sig_size = UnsignedMin(SHA256::BLOCKSIZE, n.ByteCount());

    // Scratch
    SecByteBlock buff_1, buff_2, buff_3;

    // Alice original message to be signed by Bob
    SecByteBlock orig((const byte *) "secret", 6U);
    Integer m(orig.data(), orig.size());
    cout << "Message: " << std::hex << m << endl;

    // Hash message per Rabin (1979)
    buff_1.resize(sig_size);
    SHA256 hash_1;
    hash_1.CalculateTruncatedDigest(buff_1, buff_1.size(), orig, orig.size());

    // H(m) as Integer
    Integer hm(buff_1.data(), buff_1.size());
    cout << "H(m): " << std::hex << hm << endl;

    // Alice blinding
    Integer r;
    do {
        r.Randomize(prng, Integer::One(), n - Integer::One());
    } while (!RelativelyPrime(r, n));

    // Blinding factor
    Integer b = a_exp_b_mod_c(r, e, n);
    cout << "Random: " << std::hex << b << endl;

    // Alice blinded message
    Integer mm = a_times_b_mod_c(hm, b, n);
    cout << "Blind msg: " << std::hex << mm << endl;

    // Bob sign
    Integer ss = priv_key.CalculateInverse(prng, mm);
    cout << "Blind sign: " << ss << endl;

    // Alice checks s(s'(x)) = x. This is from Chaum's paper
    Integer c = pub_key.ApplyFunction(ss);
    cout << "Check sign: " << c << endl;
    if (c != mm) {
        throw runtime_error("Alice cross-check failed");
    }
    // Alice remove blinding
    Integer s = a_times_b_mod_c(ss, r.InverseMod(n), n);
    cout << "Unblind sign: " << s << endl;

    // Eve verifies
    Integer v = pub_key.ApplyFunction(s);
    cout << "Verify: " << std::hex << v << endl;

    // Convert to a string
    size_t req = v.MinEncodedSize();
    buff_2.resize(req);
    v.Encode(&buff_2[0], buff_2.size());

    // Hash message per Rabin (1979)
    buff_3.resize(sig_size);
    SHA256 hash_2;
    hash_2.CalculateTruncatedDigest(buff_3, buff_3.size(), orig, orig.size());

    // Constant time compare
    bool equal = buff_2.size() == buff_3.size() &&
                 VerifyBufsEqual(buff_2.data(), buff_3.data(), buff_3.size());

    if (!equal) {
        throw runtime_error("Eve verified failed");
    }
    cout << "Verified signature" << endl;
}
*/
