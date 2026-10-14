/*
 * registry/sleela_registry.hpp -- Sleela Registry C++ facade (RAII).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * A thin, exception-light C++ wrapper over the C registry ABI so embedders use
 * an owned handle (reg::Registry) instead of raw pointers. SPDX: MIT-compatible
 * with the repo's distributive terms.
 */
#ifndef SLEELA_REGISTRY_HPP
#define SLEELA_REGISTRY_HPP

#include "sleela_registry.h"

#include <string>

namespace reg {

class Registry {
public:
    Registry() : r_(sl_reg_create()) {}
    ~Registry() { if (r_) sl_reg_destroy(r_); }
    Registry(Registry&& o) noexcept : r_(o.r_) { o.r_ = nullptr; }
    Registry& operator=(Registry&& o) noexcept { if (this!=&o){ if(r_)sl_reg_destroy(r_); r_=o.r_; o.r_=nullptr; } return *this; }
    Registry(const Registry&) = delete;
    Registry& operator=(const Registry&) = delete;

    bool valid() const { return r_ != nullptr; }
    sl_registry* raw() const { return r_; }

    void seed()                 { sl_reg_seed_defaults(r_); }
    bool keyExists(const std::string& p) const { return sl_reg_key_exists(r_, p.c_str()) != 0; }
    void keyCreate(const std::string& p)       { sl_reg_key_create(r_, p.c_str()); }

    void setString(const std::string& p,const std::string& n,const std::string& v){ sl_reg_set_string(r_,p.c_str(),n.c_str(),v.c_str()); }
    void setInt(const std::string& p,const std::string& n,long v)                 { sl_reg_set_int(r_,p.c_str(),n.c_str(),v); }
    void setBool(const std::string& p,const std::string& n,bool v)                { sl_reg_set_bool(r_,p.c_str(),n.c_str(),v?1:0); }

    std::string getString(const std::string& p,const std::string& n) const { char b[256]; return sl_reg_get_string(r_,p.c_str(),n.c_str(),b,sizeof b)==SL_REG_OK?std::string(b):std::string(); }
    long getInt(const std::string& p,const std::string& n) const { long v=0; sl_reg_get_int(r_,p.c_str(),n.c_str(),&v); return v; }
    bool getBool(const std::string& p,const std::string& n) const { int v=0; sl_reg_get_bool(r_,p.c_str(),n.c_str(),&v); return v!=0; }

    void featureSet(const std::string& n,bool on){ sl_reg_feature_set(r_,n.c_str(),on?1:0); }
    bool feature(const std::string& n) const      { return sl_reg_feature_enabled(r_,n.c_str())!=0; }

    void provenanceSet(const std::string& p,const sl_reg_provenance& pr){ sl_reg_provenance_set(r_,p.c_str(),&pr); }
    bool provenanceGet(const std::string& p,sl_reg_provenance& out) const { return sl_reg_provenance_get(r_,p.c_str(),&out)==SL_REG_OK; }

    sl_reg_status save(const std::string& path) const { return sl_reg_save(r_,path.c_str()); }
    sl_reg_status load(const std::string& path)       { return sl_reg_load(r_,path.c_str()); }

    std::string review(const std::string& path="") const {
        std::string out; out.resize(1<<16);
        long n = sl_reg_review(r_, path.empty()?nullptr:path.c_str(), &out[0], out.size());
        if (n < 0) return std::string();
        out.resize((size_t)n); return out;
    }

private:
    sl_registry* r_;
};

} // namespace reg

#endif /* SLEELA_REGISTRY_HPP */
