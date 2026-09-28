#include "../include/sleela_regex.hpp"
namespace sleela::regex {
Pattern::Pattern(const std::string& p,std::regex_constants::syntax_option_type f){try{expression_=std::regex(p,f);valid_=true;}catch(const std::regex_error& e){valid_=false;error_=e.what();}}
Pattern Pattern::ere(const std::string& p,bool icase){auto f=std::regex_constants::extended;if(icase)f|=std::regex_constants::icase;return Pattern(p,f);}
static Span sp(const std::smatch& m,std::size_t i){Span s;if(i<m.size()&&m[i].matched){s.start=(std::size_t)m.position(i);s.end=s.start+(std::size_t)m.length(i);s.matched=true;}return s;}
static Match mm(const std::smatch& m){Match x;x.matched=m.size()>0&&m[0].matched;x.whole=sp(m,0);for(std::size_t i=1;i<m.size();++i){Capture c;c.span=sp(m,i);if(c.span.matched)c.value=m[i].str();x.captures.push_back(c);}return x;}
Match Pattern::match(const std::string& t)const{if(!valid_)return{};std::smatch m;if(!std::regex_match(t,m,expression_))return{};return mm(m);}
Match Pattern::search(const std::string& t)const{if(!valid_)return{};std::smatch m;if(!std::regex_search(t,m,expression_))return{};return mm(m);}
std::string Pattern::replace_first(const std::string& t,const std::string& r)const{if(!valid_)return t;return std::regex_replace(t,expression_,r,std::regex_constants::format_first_only);}
std::vector<std::string> Pattern::split(const std::string& t)const{std::vector<std::string> o;if(!valid_){o.push_back(t);return o;}std::sregex_token_iterator i(t.begin(),t.end(),expression_,-1),e;for(;i!=e;++i)o.push_back(*i);return o;}
std::string Pattern::escape(const std::string& l){static const std::string m=".^$|()[]*+?{}\\";std::string o;for(char c:l){if(m.find(c)!=std::string::npos)o+='\\';o+=c;}return o;}
}
