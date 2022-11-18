#include "crow.h"
#include "model.hpp"

int main()
{
    crow::SimpleApp app;
    std::shared_ptr<Model> model = Model::getModel("simple");

    CROW_ROUTE(app, "/cddc")
      ([&model](const crow::request& req){
	auto req_cddc = RequestCDDC::from_string(req.body);
	if (!req_cddc) {
	  return crow::response(crow::status::BAD_REQUEST);
	} else {
	  ResponseCDDC res;
	  // TBD use serial from req
	  res.cddc = model->getCDDC();
	  res.message_reference = req_cddc->message_reference;
	  res.status_code = crow::status::OK;
	  return crow::response(res.to_json());
	}
      });

    CROW_ROUTE(app, "/cddc/serial")
      ([&model](const crow::request& request){
	auto req = RequestCDDSerial::from_string(request.body);
	if (!req) {
	  return crow::response(crow::status::BAD_REQUEST);
	} else {
	  // \todo check serial input
	  ResponseCDDSerial res;
	  res.cdd_serial = model->getCurrentCDDC().cdd.cdd_serial;
	  res.message_reference = req->message_reference;
	  res.status_code = crow::status::OK;
	  return crow::response(res.to_json());
	}
    });

    CROW_ROUTE(app, "/mkcs")
      ([&model](const crow::request& request){
	auto req = RequestMKCs::from_string(request.body);
	if (!req) {
	  return crow::response(crow::status::BAD_REQUEST);
	} else {
	  // \todo implement request
	  return crow::response(crow::status::NOT_IMPLEMENTED);
	}
      });

    CROW_ROUTE(app, "/mint").methods(crow::HTTPMethod::GET)
      ([&model](const crow::request& request){
	auto req = RequestMint::from_string(request.body);
	if (!req) {
	  return crow::response(crow::status::BAD_REQUEST);
	} else {
	  // \todo implement request
	  return crow::response(crow::status::NOT_IMPLEMENTED);
	}
      });


    CROW_ROUTE(app, "/renew").methods(crow::HTTPMethod::GET)
      ([&model](const crow::request& request){
	auto req = RequestMint::from_string(request.body);
	if (!req) {
	  return crow::response(crow::status::BAD_REQUEST);
	} else {
	  // \todo implement request
	  return crow::response(crow::status::NOT_IMPLEMENTED);
	}
      });

    CROW_ROUTE(app, "/resume").methods(crow::HTTPMethod::GET)
      ([&model](const crow::request& request){
	auto req = RequestResume::from_string(request.body);
	if (!req) {
	  return crow::response(crow::status::BAD_REQUEST);
	} else {
	  // \todo implement request
	  return crow::response(crow::status::NOT_IMPLEMENTED);
	}
      });

    CROW_ROUTE(app, "/redeem").methods(crow::HTTPMethod::GET)
      ([&model](const crow::request& request){
	auto req = RequestRedeem::from_string(request.body);
	if (!req) {
	  return crow::response(crow::status::BAD_REQUEST);
	} else {
	  // \todo implement request
	  return crow::response(crow::status::NOT_IMPLEMENTED);
	}
      });

    app.port(18080).run();
}
