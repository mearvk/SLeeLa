#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>

namespace sleela::reflection {
enum class Kind { Void, Boolean, Integer, Unsigned, Floating, String, Struct, Class, Array, Pointer, Function, Enum, Unknown };
struct Field { std::string name,type; std::size_t offset=0,size=0; Kind kind=Kind::Unknown; bool isStatic=false; };
struct Parameter { std::string name,type; };
struct Method { std::string name,signature; std::vector<Parameter> parameters; std::string returnType; bool isStatic=false; };
struct Constructor { std::string signature; std::vector<Parameter> parameters; };
struct Property { std::string name,type; bool readable=true,writable=false; };
struct Attribute { std::string name,value; };
struct Type {
  std::string name,namespaceName;
  std::size_t size=0,alignment=0;
  Kind kind=Kind::Unknown;
  std::string typeId,version;
  std::vector<std::string> bases;
  std::vector<Field> fields;
  std::vector<Method> methods;
  std::vector<Constructor> constructors;
  std::vector<Property> properties;
  std::vector<Attribute> attributes;
};
class Registry {
  mutable std::mutex mutex_;
  std::unordered_map<std::string,Type> types_;
public:
  bool add(Type);
  bool remove(const std::string&);
  const Type* find(const std::string&) const;
  std::vector<std::string> names() const;
  void clear();
};
std::string stableTypeId(const std::string& fullyQualifiedName,const std::string& version);
template<class T> Type describe(const char* n){Type t;t.name=n;t.size=sizeof(T);t.alignment=alignof(T);t.kind=Kind::Struct;return t;}
}
