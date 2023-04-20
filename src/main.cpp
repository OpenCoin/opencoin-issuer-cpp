#include "crow.h"
#include "crow/common.h"
#include "crow/http_response.h"
#include "model.hpp"

int main() {
  crow::SimpleApp app;
  std::shared_ptr<Model> model = Model::get_model("simple");

  CROW_ROUTE(app, "/cddc")
      .methods(crow::HTTPMethod::POST)
    ([&model](const crow::request &req) {
        auto req_cddc = RequestCDDC::from_string(req.body);
        if (!req_cddc) {
          return crow::response(crow::status::BAD_REQUEST);
        } else {
          ResponseCDDC res;
          res.message_reference = req_cddc->message_reference;
          auto cddc = model->get_cddc(req_cddc->cdd_serial);
          if (!cddc) {
            res.status_code = crow::status::NOT_FOUND;
          } else {
            res.cddc = *cddc.value();
            res.status_code = crow::status::OK;
          }
          return crow::response(res.to_json());
        }
      });

  CROW_ROUTE(app, "/cddc/serial")
      .methods(crow::HTTPMethod::POST)([&model](const crow::request &request) {
        auto req = RequestCDDCSerial::from_string(request.body);
        if (!req) {
          return crow::response(crow::status::BAD_REQUEST);
        } else {
          ResponseCDDCSerial res;
          res.message_reference = req->message_reference;

          auto cddc = model->get_current_cddc();
          if (!cddc) {
            res.status_code = crow::status::NOT_FOUND;
          } else {
            res.cdd_serial = (*cddc)->cdd.cdd_serial;
            res.status_code = crow::status::OK;
          }
          return crow::response(res.to_json());
        }
      });

  CROW_ROUTE(app, "/mkcs").methods(crow::HTTPMethod::POST)
    ([&model](const crow::request &request) {
        auto req = RequestMKCs::from_string(request.body);
        if (!req) {
          return crow::response(crow::status::BAD_REQUEST);
        } else {
          ResponseMKCs res;
          res.message_reference = req->message_reference;
          res.keys = model->getMKCs(req->denominations, req->mint_key_ids);
          res.status_code = crow::status::OK;
          return crow::response(res.to_json());
        }
      });

  CROW_ROUTE(app, "/mint").methods(crow::HTTPMethod::POST)
    ([&model](const crow::request &request) {
        auto req = RequestMint::from_string(request.body);
        if (!req) {
          return crow::response(crow::status::BAD_REQUEST);
        } else {
          ResponseMint res;
          res.message_reference = req->message_reference;

          /// \todo change argument transaction_reference to bigint
          auto minted =
              model->mint(req->transaction_reference.to_string(), req->blinds);

          res.blind_signatures = minted;
          res.status_code = crow::status::OK;

          return crow::response(res.to_json());
        }
      });

  CROW_ROUTE(app, "/renew")
      .methods(crow::HTTPMethod::POST)([&model](const crow::request &request) {
        auto req = RequestRenew::from_string(request.body);
        if (!req) {
          return crow::response(crow::status::BAD_REQUEST);
        } else {
          // \todo implement ResponseDelay
          ResponseMint res;
          res.message_reference = req->message_reference;
          res.status_code = crow::status::OK;
          res.blind_signatures =
              model->mint(req->transaction_reference, req->blinds);
          return crow::response(res.to_json());
        }
      });

  CROW_ROUTE(app, "/resume")
      .methods(crow::HTTPMethod::POST)([](const crow::request &request) {
        auto req = RequestResume::from_string(request.body);
        if (!req) {
          return crow::response(crow::status::BAD_REQUEST);
        } else {
          // \todo implement request
          ResponseMint res;
          res.message_reference = req->message_reference;
          res.status_code = crow::status::NOT_IMPLEMENTED; // crow::status::OK;
          return crow::response(crow::status::NOT_IMPLEMENTED);
        }
      });

  CROW_ROUTE(app, "/redeem")
      .methods(crow::HTTPMethod::POST)([&model](const crow::request &request) {
        auto req = RequestRedeem::from_string(request.body);
        if (!req) {
          return crow::response(crow::status::BAD_REQUEST);
        } else {
          ResponseRedeem res;
          res.message_reference = req->message_reference;
          bool success = model->redeem(req->coins);
          res.status_code =
              success ? crow::status::OK : crow::status::NOT_FOUND;

          return crow::response(res.to_json());
        }
      });

  app.port(18080).run();
}
