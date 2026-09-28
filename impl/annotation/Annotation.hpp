#pragma once
#include <string>
#include <vector>
#include <utility>
namespace sleela::annotation {
struct Annotation { std::string name; std::string value; };
class DocumentAnnotations {
    std::vector<Annotation> items_;
public:
    void add(std::string n,std::string v);
    void add(Annotation a);
    const std::vector<Annotation>& all() const noexcept;
    bool has(const std::string& n) const noexcept;
    std::size_t count(const std::string& n) const noexcept;
    const Annotation* first(const std::string& n) const noexcept;
};
}