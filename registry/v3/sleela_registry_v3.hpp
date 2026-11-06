#ifndef SLEELA_REGISTRY_V3_HPP
#define SLEELA_REGISTRY_V3_HPP
#include "sleela_registry_v3.h"
#include <string>
#include <utility>
namespace reg3 {
class Registry {
public:
    Registry(const std::string& id, size_t maxRecords=4096)
        : value_(slr3_registry_create(id.c_str(),maxRecords)) {}
    ~Registry(){ if(value_) slr3_registry_destroy(value_); }
    Registry(const Registry&)=delete;
    Registry& operator=(const Registry&)=delete;
    Registry(Registry&& other) noexcept : value_(other.value_){other.value_=nullptr;}
    Registry& operator=(Registry&& other) noexcept {
        if(this!=&other){if(value_)slr3_registry_destroy(value_);value_=other.value_;other.value_=nullptr;}
        return *this;
    }
    bool valid() const { return value_!=nullptr; }
    size_t count() const { return slr3_registry_count(value_); }
    const slr3_record* find(const std::string& id) const { return slr3_registry_get(value_,id.c_str()); }
    const slr3_record* identify(const std::string& ns,const std::string& nameOrAlias) const {
        return slr3_registry_identify(value_,ns.c_str(),nameOrAlias.c_str());
    }
    slr3_status add(const slr3_record& record){return slr3_registry_add(value_,&record);}
    slr3_status relate(const std::string& a,const std::string& type,const std::string& b){
        return slr3_registry_relate(value_,a.c_str(),type.c_str(),b.c_str());
    }
    slr3_status transition(const std::string& id,const std::string& state,uint64_t revision){
        return slr3_registry_transition(value_,id.c_str(),state.c_str(),revision);
    }
    slr3_status save(const std::string& path) const {return slr3_registry_save_v2(value_,path.c_str());}
    slr3_status load(const std::string& path){return slr3_registry_load_v2(value_,path.c_str());}
    slr3_registry* raw() const {return value_;}
private:
    slr3_registry* value_;
};
}
#endif
