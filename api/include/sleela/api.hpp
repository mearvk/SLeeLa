#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include <utility>

namespace sleela::api {

enum class EvidenceKind { Observed, Decoded, Derived, Inferred, Hypothesized, Unknown, Unsupported, Conflicting };

struct Provenance {
    std::uint64_t file_offset{0};
    std::optional<std::uint64_t> virtual_address;
    std::optional<std::uint64_t> runtime_address;
    std::string source;
    EvidenceKind evidence{EvidenceKind::Observed};
};

enum class TypeKind { Void, Bool, Char, Integer, Floating, Pointer, Reference, Array, Struct, Union, Enum, Function, FunctionPointer, Class, Opaque, Unknown };

class Type {
public:
    Type() = default;
    explicit Type(std::string name, TypeKind kind = TypeKind::Unknown) : name_(std::move(name)), kind_(kind) {}
    const std::string& name() const noexcept { return name_; }
    TypeKind kind() const noexcept { return kind_; }
    void add_evidence(Provenance p) { evidence_.push_back(std::move(p)); }
    const std::vector<Provenance>& evidence() const noexcept { return evidence_; }
private:
    std::string name_;
    TypeKind kind_{TypeKind::Unknown};
    std::vector<Provenance> evidence_;
};

class Field {
public:
    Field(std::string name, Type type, std::uint64_t offset = 0) : name_(std::move(name)), type_(std::move(type)), offset_(offset) {}
    const std::string& name() const noexcept { return name_; }
    const Type& type() const noexcept { return type_; }
    std::uint64_t offset() const noexcept { return offset_; }
    void add_evidence(Provenance p) { evidence_.push_back(std::move(p)); }
    const std::vector<Provenance>& evidence() const noexcept { return evidence_; }
private:
    std::string name_;
    Type type_;
    std::uint64_t offset_{0};
    std::vector<Provenance> evidence_;
};

class CallingConvention {
public:
    explicit CallingConvention(std::string name = "unknown") : name_(std::move(name)) {}
    const std::string& name() const noexcept { return name_; }
    void set_confidence(double value) noexcept { confidence_ = value; }
    double confidence() const noexcept { return confidence_; }
private:
    std::string name_;
    double confidence_{0.0};
};

class FunctionSignature {
public:
    explicit FunctionSignature(std::string name = {}) : name_(std::move(name)) {}
    const std::string& name() const noexcept { return name_; }
    void set_return_type(Type type) { return_type_ = std::move(type); }
    const std::optional<Type>& return_type() const noexcept { return return_type_; }
    void add_parameter(Type type) { parameters_.push_back(std::move(type)); }
    const std::vector<Type>& parameters() const noexcept { return parameters_; }
    void set_calling_convention(CallingConvention cc) { calling_convention_ = std::move(cc); }
    const CallingConvention& calling_convention() const noexcept { return calling_convention_; }
private:
    std::string name_;
    std::optional<Type> return_type_;
    std::vector<Type> parameters_;
    CallingConvention calling_convention_;
};

class Method {
public:
    explicit Method(std::string name = {}) : name_(std::move(name)) {}
    const std::string& name() const noexcept { return name_; }
    void set_signature(FunctionSignature signature) { signature_ = std::move(signature); }
    const std::optional<FunctionSignature>& signature() const noexcept { return signature_; }
    void set_virtual(bool value) noexcept { virtual_ = value; }
    bool is_virtual() const noexcept { return virtual_; }
    void set_static(bool value) noexcept { static_ = value; }
    bool is_static() const noexcept { return static_; }
    void add_evidence(Provenance p) { evidence_.push_back(std::move(p)); }
    const std::vector<Provenance>& evidence() const noexcept { return evidence_; }
private:
    std::string name_;
    std::optional<FunctionSignature> signature_;
    bool virtual_{false};
    bool static_{false};
    std::vector<Provenance> evidence_;
};

class CppClass {
public:
    explicit CppClass(std::string name = {}) : name_(std::move(name)) {}
    const std::string& name() const noexcept { return name_; }
    void add_base_class(std::string name) { bases_.push_back(std::move(name)); }
    const std::vector<std::string>& base_classes() const noexcept { return bases_; }
    void add_field(Field field) { fields_.push_back(std::move(field)); }
    const std::vector<Field>& fields() const noexcept { return fields_; }
    void add_method(Method method) { methods_.push_back(std::move(method)); }
    const std::vector<Method>& methods() const noexcept { return methods_; }
    void set_vtable_address(std::uint64_t address) noexcept { vtable_address_ = address; }
    const std::optional<std::uint64_t>& vtable_address() const noexcept { return vtable_address_; }
    void set_rtti_type_name(std::string name) { rtti_type_name_ = std::move(name); }
    const std::optional<std::string>& rtti_type_name() const noexcept { return rtti_type_name_; }
    void add_evidence(Provenance p) { evidence_.push_back(std::move(p)); }
    const std::vector<Provenance>& evidence() const noexcept { return evidence_; }
private:
    std::string name_;
    std::vector<std::string> bases_;
    std::vector<Field> fields_;
    std::vector<Method> methods_;
    std::optional<std::uint64_t> vtable_address_;
    std::optional<std::string> rtti_type_name_;
    std::vector<Provenance> evidence_;
};

class CStruct {
public:
    explicit CStruct(std::string name = {}) : name_(std::move(name)) {}
    const std::string& name() const noexcept { return name_; }
    void add_field(Field field) { fields_.push_back(std::move(field)); }
    const std::vector<Field>& fields() const noexcept { return fields_; }
private:
    std::string name_;
    std::vector<Field> fields_;
};

class CUnion : public CStruct { public: using CStruct::CStruct; };

class CEnum {
public:
    explicit CEnum(std::string name = {}) : name_(std::move(name)) {}
    void add_value(std::string name, std::int64_t value) { values_.emplace_back(std::move(name), value); }
    const std::string& name() const noexcept { return name_; }
    const std::vector<std::pair<std::string, std::int64_t>>& values() const noexcept { return values_; }
private:
    std::string name_;
    std::vector<std::pair<std::string, std::int64_t>> values_;
};

class CppNamespace {
public:
    explicit CppNamespace(std::string name = {}) : name_(std::move(name)) {}
    const std::string& name() const noexcept { return name_; }
    void add_class(CppClass value) { classes_.push_back(std::move(value)); }
    const std::vector<CppClass>& classes() const noexcept { return classes_; }
private:
    std::string name_;
    std::vector<CppClass> classes_;
};

class Address {
public:
    explicit Address(std::uint64_t value = 0) : value_(value) {}
    std::uint64_t value() const noexcept { return value_; }
private:
    std::uint64_t value_{0};
};

class Variable {
public:
    Variable(std::string name, Type type) : name_(std::move(name)), type_(std::move(type)) {}
    const std::string& name() const noexcept { return name_; }
    const Type& type() const noexcept { return type_; }
private:
    std::string name_;
    Type type_;
};

class Instruction {
public:
    Instruction(Address address, std::string mnemonic) : address_(address), mnemonic_(std::move(mnemonic)) {}
    const Address& address() const noexcept { return address_; }
    const std::string& mnemonic() const noexcept { return mnemonic_; }
private:
    Address address_;
    std::string mnemonic_;
};

class BasicBlock {
public:
    explicit BasicBlock(std::uint64_t id = 0) : id_(id) {}
    std::uint64_t id() const noexcept { return id_; }
    void add_instruction(Instruction instruction) { instructions_.push_back(std::move(instruction)); }
    const std::vector<Instruction>& instructions() const noexcept { return instructions_; }
private:
    std::uint64_t id_{0};
    std::vector<Instruction> instructions_;
};

class ControlFlowGraph {
public:
    void add_block(BasicBlock block) { blocks_.push_back(std::move(block)); }
    const std::vector<BasicBlock>& blocks() const noexcept { return blocks_; }
private:
    std::vector<BasicBlock> blocks_;
};

class Artifact {
public:
    explicit Artifact(std::string identity = {}) : identity_(std::move(identity)) {}
    const std::string& identity() const noexcept { return identity_; }
    void set_format(std::string format) { format_ = std::move(format); }
    const std::string& format() const noexcept { return format_; }
private:
    std::string identity_;
    std::string format_;
};

class ReconstructionUnit {
public:
    void add_type(Type type) { types_.push_back(std::move(type)); }
    void add_class(CppClass value) { classes_.push_back(std::move(value)); }
    void add_struct(CStruct value) { structs_.push_back(std::move(value)); }
    void add_union(CUnion value) { unions_.push_back(std::move(value)); }
    void add_enum(CEnum value) { enums_.push_back(std::move(value)); }
    void add_namespace(CppNamespace value) { namespaces_.push_back(std::move(value)); }
    const std::vector<Type>& types() const noexcept { return types_; }
    const std::vector<CppClass>& classes() const noexcept { return classes_; }
    const std::vector<CStruct>& structs() const noexcept { return structs_; }
    const std::vector<CUnion>& unions() const noexcept { return unions_; }
    const std::vector<CEnum>& enums() const noexcept { return enums_; }
    const std::vector<CppNamespace>& namespaces() const noexcept { return namespaces_; }
private:
    std::vector<Type> types_;
    std::vector<CppClass> classes_;
    std::vector<CStruct> structs_;
    std::vector<CUnion> unions_;
    std::vector<CEnum> enums_;
    std::vector<CppNamespace> namespaces_;
};

class CApi {
public:
    virtual ~CApi() = default;
    virtual std::string language() const { return "C"; }
};

class CppApi : public CApi {
public:
    std::string language() const override { return "C++"; }
};

} // namespace sleela::api
