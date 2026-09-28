#include "expressions.hpp"
#include <cctype>
namespace sleela::debugger::engine {
ExpressionResult ExpressionEngine::evaluate(const std::string&e,const std::unordered_map<std::string,std::uint64_t>&v)const{if(e.empty()||e.size()>4096)return{false,0,"expression length invalid"};auto it=v.find(e);if(it!=v.end())return{true,it->second,{}};std::size_t i=0;while(i<e.size()&&std::isspace((unsigned char)e[i]))++i;std::uint64_t n=0;bool any=false;for(;i<e.size()&&std::isxdigit((unsigned char)e[i]);++i){any=true;n=n*16+(std::isdigit((unsigned char)e[i])?e[i]-'0':std::tolower((unsigned char)e[i])-'a'+10);}while(i<e.size()&&std::isspace((unsigned char)e[i]))++i;return any&&i==e.size()?ExpressionResult{true,n,{}}:ExpressionResult{false,0,"unsupported expression"};}
}