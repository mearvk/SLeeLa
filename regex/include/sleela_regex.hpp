#ifndef SLEELA_REGEX_HPP
#define SLEELA_REGEX_HPP
#include <cstddef>
#include <regex>
#include <string>
#include <vector>
namespace sleela::regex {
struct Span { std::size_t start=0,end=0; bool matched=false; };
struct Capture { Span span; std::string value; };
struct Match { bool matched=false; Span whole; std::vector<Capture> captures; };
class Pattern {
public:
 Pattern()=default;
 explicit Pattern(const std::string& pattern,std::regex_constants::syntax_option_type flags=std::regex_constants::ECMAScript);
 static Pattern ere(const std::string& pattern,bool icase=false);
 bool valid() const noexcept { return valid_; }
 const std::string& error() const noexcept { return error_; }
 Match match(const std::string& text) const;
 Match search(const std::string& text) const;
 std::string replace_first(const std::string& text,const std::string& replacement) const;
 std::vector<std::string> split(const std::string& text) const;
 static std::string escape(const std::string& literal);
private:
 std::regex expression_;
 bool valid_=false;
 std::string error_;
};
}
#endif
