#include "crow/http_response.h"
#include "crow/json.h"
#include <optional>

#include "crow.h"

#include "model.hpp"


int main()
{
    crow::SimpleApp app;
    std::shared_ptr<Model> model = Model::getModel("simple");

    CROW_ROUTE(app, "/cddc")//.methods(crow::HTTPMethod::GET)
      ([&model](const crow::request& req){
	auto json = crow::json::load(req.body);
	if (!json) {
	  return crow::response(crow::status::BAD_REQUEST,
				"json parse error");
	} else {
	  auto req_cddc = RequestCDDC::from_json(json);
	  if (!req_cddc)
	    return crow::response(crow::status::BAD_REQUEST,
				  "json decode error ");
	  else {
	    ResponseCDDC res;
	    // TBD use serial from req
	    res.cddc = model->getCDDC();
	    res.message_reference = req_cddc->message_reference;
	    res.status_code = 200;
	    return crow::response(res.to_json());
	  }
	}
      });

    CROW_ROUTE(app, "/cddc/serial").methods(crow::HTTPMethod::GET)
      ([](){
	return crow::response(crow::status::NOT_IMPLEMENTED);
    });

    CROW_ROUTE(app, "/mkcs").methods(crow::HTTPMethod::GET)
      ([](){
	return crow::response(crow::status::NOT_IMPLEMENTED);
    });

    CROW_ROUTE(app, "/mint").methods(crow::HTTPMethod::GET)
      ([](){
	return crow::response(crow::status::NOT_IMPLEMENTED);
    });


    CROW_ROUTE(app, "/renew").methods(crow::HTTPMethod::GET)
      ([](){
	return crow::response(crow::status::NOT_IMPLEMENTED);
    });

    CROW_ROUTE(app, "/resume").methods(crow::HTTPMethod::GET)
      ([](){
	return crow::response(crow::status::NOT_IMPLEMENTED);
    });

    CROW_ROUTE(app, "/redeem").methods(crow::HTTPMethod::GET)
      ([](){
	return crow::response(crow::status::NOT_IMPLEMENTED);
    });

    
    

    app.port(18080).run();
}
