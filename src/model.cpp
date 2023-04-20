#include "model.hpp"
#include "tl/expected.hpp"

using std::cout;
using std::endl;
#define UNUSED(s) /* s */

/** this is for now our sample model */
class DefaultModel : public Model {
public:
    DefaultModel() = default;

    tl::expected<CDDC *, bool> get_cddc(unsigned int cdd_serial) override {
        cout << __FUNCTION__ << "(" << cdd_serial << ")" << endl; // NOLINT(clion-misra-cpp2008-5-2-12)
        return &m_cddc;
    };

    tl::expected<CDDC *, bool> get_current_cddc() override {
        cout << __FUNCTION__ << "()" << endl;

        return &m_cddc;
    }

    std::vector<BlindSignature>
    mint(const std::string & UNUSED(transaction_reference),
         const std::vector<Blind> &UNUSED(blinds)) override {
        std::vector<BlindSignature> res;
        cout << __FUNCTION__ << "("
             << ")" << endl;

        return res;
    }

    std::vector<MintKeyCert>
    getMKCs(const std::vector<unsigned int> & UNUSED(denominations),
            const std::vector<BigInt> & UNUSED(mint_key_ids)) override {
        std::vector<MintKeyCert> res;
        cout << __FUNCTION__ << endl;
        return res;
    }

    bool redeem(const std::vector<Coin> &UNUSED(coins)) override {
        cout << __FUNCTION__ << endl;
        return false;
    }

private:
    CDDC m_cddc;
};

std::unique_ptr<Model> Model::get_model(const std::string & /*backend_name*/) {
    cout << __FUNCTION__ << endl;
    //:wq
    // if (backend_name=="default")
    return std::make_unique<DefaultModel>();
}
