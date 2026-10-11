#ifndef SLEELA_CONNECTOR_REGISTRY_HPP
#define SLEELA_CONNECTOR_REGISTRY_HPP
#include "connector_registry.h"
#include <new>
#include <stdexcept>
#include <string>
namespace sleela::connector {
class ConnectorRegistry {
 sleela_connector_registry*r_=sleela_connector_registry_create();
public:
 ConnectorRegistry(){if(!r_)throw std::bad_alloc();}
 ~ConnectorRegistry(){sleela_connector_registry_destroy(r_);}
 ConnectorRegistry(const ConnectorRegistry&)=delete;ConnectorRegistry&operator=(const ConnectorRegistry&)=delete;
 void add(const std::string&ref,const std::string&kind,void*handle=nullptr){char e[256]{};if(!sleela_connector_register(r_,ref.c_str(),kind.c_str(),handle,e,sizeof e))throw std::runtime_error(e);}
 sleela_connector_view resolve(const std::string&ref)const{sleela_connector_view v{};char e[256]{};if(!sleela_connector_resolve(r_,ref.c_str(),&v,e,sizeof e))throw std::runtime_error(e);return v;}
 sleela_connector_view resolveUnnamed()const{sleela_connector_view v{};char e[256]{};if(!sleela_connector_resolve_unnamed(r_,&v,e,sizeof e))throw std::runtime_error(e);return v;}
 void setProperty(const std::string&ref,const std::string&key,const std::string&value){char e[256]{};if(!sleela_connector_property_set(r_,ref.c_str(),key.c_str(),value.c_str(),e,sizeof e))throw std::runtime_error(e);}
 std::string property(const std::string&ref,const std::string&key)const{const char*v=sleela_connector_property_get(r_,ref.c_str(),key.c_str());return v?v:"";}
 size_t size()const noexcept{return sleela_connector_count(r_);}
};
}
#endif
