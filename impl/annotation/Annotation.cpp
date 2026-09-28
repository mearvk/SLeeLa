#include "Annotation.hpp"
namespace sleela::annotation {
void DocumentAnnotations::add(std::string n,std::string v){items_.push_back({std::move(n),std::move(v)});}
void DocumentAnnotations::add(Annotation a){items_.push_back(std::move(a));}
const std::vector<Annotation>& DocumentAnnotations::all() const noexcept{return items_;}
bool DocumentAnnotations::has(const std::string& n) const noexcept{return first(n)!=nullptr;}
std::size_t DocumentAnnotations::count(const std::string& n) const noexcept{std::size_t c=0;for(const auto& a:items_)if(a.name==n)++c;return c;}
const Annotation* DocumentAnnotations::first(const std::string& n) const noexcept{for(const auto& a:items_)if(a.name==n)return &a;return nullptr;}
}