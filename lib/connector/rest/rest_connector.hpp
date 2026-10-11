#ifndef SLEELA_REST_CONNECTOR_HPP
#define SLEELA_REST_CONNECTOR_HPP
#include "rest_connector.h"
#include <stdexcept>
#include <string>
namespace sleela::connector {
struct HttpResponse { long status; std::string body; };
class RestClient {
 long timeout_;
public:
 explicit RestClient(long timeoutSeconds=30):timeout_(timeoutSeconds){}
 HttpResponse get(const std::string& url)const{sleela_http_response r{};if(!sleela_http_get(url.c_str(),timeout_,&r)){std::string e=r.error;sleela_http_response_free(&r);throw std::runtime_error(e);}HttpResponse o{r.status,r.body?r.body:""};sleela_http_response_free(&r);return o;}
 HttpResponse postJson(const std::string& url,const std::string& json)const{sleela_http_response r{};if(!sleela_http_post_json(url.c_str(),json.c_str(),timeout_,&r)){std::string e=r.error;sleela_http_response_free(&r);throw std::runtime_error(e);}HttpResponse o{r.status,r.body?r.body:""};sleela_http_response_free(&r);return o;}
};
}
#endif
