#pragma once
#include <string>
#include <vector>
#include <utility>
namespace sleela::annotation {
struct Annotation { std::string name; std::string value; };
class DocumentAnnotations { std::vector<Annotation> items_; public: void add(std::string n,std::string v){items_.push_back({std::move(n),std::move(v)});} const std::vector<Annotation>& all() const noexcept{return items_;} bool has(const std::string& n) const {for(const auto& a:items_) if(a.name==n)return true; return false;} };
}