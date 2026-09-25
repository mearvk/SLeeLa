#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <cstddef>
namespace sleela::reflection { enum class Kind{Void,Boolean,Integer,Unsigned,Floating,String,Struct,Class,Array,Pointer,Function,Unknown}; struct Field{std::string name,type;size_t offset=0,size=0;Kind kind=Kind::Unknown;}; struct Method{std::string name,signature;}; struct Type{std::string name;size_t size=0,alignment=0;Kind kind=Kind::Unknown;std::vector<Field> fields;std::vector<Method> methods;}; class Registry{std::unordered_map<std::string,Type> types;public:bool add(Type);const Type* find(const std::string&)const;std::vector<std::string> names()const;}; template<class T> Type describe(const char*n){Type t;t.name=n;t.size=sizeof(T);t.alignment=alignof(T);t.kind=Kind::Struct;return t;} }
