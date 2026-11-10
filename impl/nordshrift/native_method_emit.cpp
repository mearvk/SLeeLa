#include "native_method_emit.h"

#include <cctype>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace nordshrift {
namespace {
using namespace sleela;

static bool markedNative(const Method& m) {
    for (const auto& a : m.java.annotations)
        if (a.name == "native" && a.marker) return true;
    return false;
}
static bool scalarType(const std::string& t) {
    return t == "int" || t == "double" || t == "boolean" || t == "String";
}
static std::string cType(const std::string& t) {
    if (t == "int") return "int64_t";
    if (t == "double") return "double";
    if (t == "boolean") return "int32_t";
    if (t == "String") return "SLNativeString";
    if (t == "void") return "void";
    throw std::runtime_error("unsupported native ABI type '" + t + "'");
}
static std::string cVar(const std::string& name) { return "slv_" + name; }
static std::string cString(const std::string& s) {
    std::ostringstream o;
    for (unsigned char c : s) {
        switch (c) {
            case '\\': o << "\\\\"; break;
            case '"': o << "\\\""; break;
            case '\n': o << "\\n"; break;
            case '\r': o << "\\r"; break;
            case '\t': o << "\\t"; break;
            default:
                if (c < 32 || c >= 127) {
                    const char* hex = "0123456789abcdef";
                    o << "\\x" << hex[c >> 4] << hex[c & 15] << "\"\"";
                } else o << static_cast<char>(c);
        }
    }
    return o.str();
}
static std::string signature(const ClassDecl& cls, const Method& m) {
    std::string key = cls.name + "::" + m.name;
    std::string s = key + "|static|argc=" + std::to_string(m.params.size()) + "|";
    for (const auto& p : m.params) s += std::to_string(p.type.size()) + ":" + p.type;
    s += "|return=" + std::to_string(m.retType.size()) + ":" + m.retType + "|abi=1.0";
    return s;
}

class MethodBodyEmitter {
    std::ostringstream out;
    int depth = 0;
    void ind() { for (int i=0;i<depth;i++) out << "    "; }
    void expr(const Expr* e) {
        if (auto x=dynamic_cast<const IntLit*>(e)) { out << "INT64_C(" << x->value << ")"; return; }
        if (auto x=dynamic_cast<const DoubleLit*>(e)) { out << std::setprecision(17) << x->value; return; }
        if (auto x=dynamic_cast<const BoolLit*>(e)) { out << (x->value ? "1" : "0"); return; }
        if (auto x=dynamic_cast<const VarExpr*>(e)) { out << cVar(x->name); return; }
        if (auto x=dynamic_cast<const Unary*>(e)) { out << "(" << x->op; expr(x->operand.get()); out << ")"; return; }
        if (auto x=dynamic_cast<const Binary*>(e)) { out << "("; expr(x->lhs.get()); out << " " << x->op << " "; expr(x->rhs.get()); out << ")"; return; }
        if (auto x=dynamic_cast<const ConditionalExpr*>(e)) { out << "("; expr(x->cond.get()); out << " ? "; expr(x->thenE.get()); out << " : "; expr(x->elseE.get()); out << ")"; return; }
        if (auto x=dynamic_cast<const AssignmentExpr*>(e)) {
            auto v=dynamic_cast<const VarExpr*>(x->target.get());
            if (!v || x->op != "=") throw std::runtime_error("native emitter supports only simple local assignment expressions");
            out << "(" << cVar(v->name) << " = "; expr(x->value.get()); out << ")"; return;
        }
        throw std::runtime_error("native emitter encountered an unsupported expression");
    }
    void inlineStmt(const Stmt* s) {
        if (auto d=dynamic_cast<const VarDecl*>(s)) {
            if (!scalarType(d->type)) throw std::runtime_error("unsupported local type '" + d->type + "'");
            out << cType(d->type) << " " << cVar(d->name);
            if (d->init) { out << " = "; expr(d->init.get()); }
            else if (d->type == "String") out << " = (SLNativeString){0}";
            else out << " = 0";
            return;
        }
        if (auto a=dynamic_cast<const Assign*>(s)) { out << cVar(a->name) << " = "; expr(a->value.get()); return; }
        if (auto e=dynamic_cast<const ExprStmt*>(s)) { expr(e->expr.get()); return; }
        throw std::runtime_error("unsupported for-loop initializer/update in native method");
    }
    void stmt(const Stmt* s) {
        if (auto b=dynamic_cast<const Block*>(s)) { ind(); out << "{\n"; ++depth; for (const auto& x:b->stmts) stmt(x.get()); --depth; ind(); out << "}\n"; return; }
        if (auto d=dynamic_cast<const VarDecl*>(s)) { ind(); inlineStmt(d); out << ";\n"; return; }
        if (auto a=dynamic_cast<const Assign*>(s)) { ind(); inlineStmt(a); out << ";\n"; return; }
        if (auto e=dynamic_cast<const ExprStmt*>(s)) { ind(); inlineStmt(e); out << ";\n"; return; }
        if (auto r=dynamic_cast<const ReturnStmt*>(s)) { ind(); out << "return"; if (r->value) { out << " "; expr(r->value.get()); } out << ";\n"; return; }
        if (auto i=dynamic_cast<const IfStmt*>(s)) {
            ind(); out << "if ("; expr(i->cond.get()); out << ")\n"; stmt(i->thenS.get());
            if (i->elseS) { ind(); out << "else\n"; stmt(i->elseS.get()); } return;
        }
        if (auto w=dynamic_cast<const WhileStmt*>(s)) { ind(); out << "while ("; expr(w->cond.get()); out << ")\n"; stmt(w->body.get()); return; }
        if (auto f=dynamic_cast<const ForStmt*>(s)) {
            ind(); out << "for ("; if (f->init) inlineStmt(f->init.get()); out << "; ";
            if (f->cond) expr(f->cond.get()); out << "; "; if (f->update) inlineStmt(f->update.get());
            out << ")\n"; stmt(f->body.get()); return;
        }
        throw std::runtime_error("native emitter encountered an unsupported statement");
    }
public:
    std::string emit(const Method& m, const std::string& symbol) {
        out << "static " << cType(m.retType) << " " << symbol << "(";
        if (m.params.empty()) out << "void";
        for (size_t i=0;i<m.params.size();++i) {
            if (i) out << ", ";
            out << cType(m.params[i].type) << " " << cVar(m.params[i].name);
        }
        out << ") {\n";
        depth=1;
        if (m.body) for (const auto& s:m.body->stmts) stmt(s.get());
        ind();
        if (m.retType == "void") out << "return;\n";
        else out << "return (" << cType(m.retType) << ")0; /* unreachable fallback; semantic compiler validates source */\n";
        out << "}\n\n";
        return out.str();
    }
};

struct NativeMethod {
    const ClassDecl* cls;
    const Method* method;
    std::string signature;
    std::string symbol;
};

static void validateMethod(const ClassDecl& cls, const Method& m) {
    if (!m.isStatic) throw std::runtime_error(cls.name+"."+m.name+": only static @native methods have an ABI-v1 receiver contract");
    if (!scalarType(m.retType) && m.retType != "void")
        throw std::runtime_error(cls.name+"."+m.name+": unsupported return type '"+m.retType+"'");
    for (const auto& p:m.params)
        if (!scalarType(p.type)) throw std::runtime_error(cls.name+"."+m.name+": unsupported parameter type '"+p.type+"'");
    if (!m.body) throw std::runtime_error(cls.name+"."+m.name+": method has no body");
    for (const auto& s:m.body->stmts) {
        if (auto v=dynamic_cast<const VarDecl*>(s.get()))
            if (!scalarType(v->type)) throw std::runtime_error(cls.name+"."+m.name+": unsupported local type '"+v->type+"'");
    }
}

} // namespace

std::string emitNativeMethods(const sleela::Program& program) {
    std::vector<NativeMethod> methods;
    for (const auto& cls:program.classes) for (const auto& method:cls.methods) {
        if (!markedNative(method)) continue;
        validateMethod(cls,method);
        methods.push_back({&cls,&method,signature(cls,method),"sleela_native_method_"+std::to_string(methods.size())});
    }
    if (methods.empty()) throw std::runtime_error("no @native methods to emit");
    std::ostringstream out;
    out << "/* Generated by SLeeLa: independently addressable @native method plugin. */\n"
           "#include <stdint.h>\n#include <stddef.h>\n#include <string.h>\n"
           "#include \"native_loader.h\"\n"
           "typedef struct { const uint8_t *data; uint32_t length; } SLNativeString;\n\n";
    for (const auto& nm:methods) {
        MethodBodyEmitter body;
        out << body.emit(*nm.method,nm.symbol);
    }
    out << "SL_NATIVE_EXPORT uint32_t sl_native_abi_version(void) { return SL_NATIVE_ABI_VERSION; }\n"
           "SL_NATIVE_EXPORT uint32_t sl_native_typed_abi_version(void) { return SL_NATIVE_TYPED_ABI_VERSION; }\n"
           "SL_NATIVE_EXPORT int sl_native_run(int argc, const char *const argv[]) { (void)argc; (void)argv; return -1; }\n"
           "SL_NATIVE_EXPORT int sl_native_call(const char *method, uint32_t argc, const SLNativeValue *argv, SLNativeValue *out_result) {\n"
           "    if (!method || !out_result) return SL_NATIVE_CALL_INVALID_ARGUMENT;\n"
           "    memset(out_result, 0, sizeof(*out_result));\n";
    for (const auto& nm:methods) {
        const Method& m=*nm.method;
        out << "    if (strcmp(method, \"" << cString(nm.signature) << "\") == 0) {\n"
               "        if (argc != " << m.params.size() << "u || (argc && !argv)) return SL_NATIVE_CALL_INVALID_ARGUMENT;\n";
        for (size_t i=0;i<m.params.size();++i) {
            const auto& p=m.params[i];
            const char* tag=p.type=="int"?"SL_NATIVE_VALUE_I64":p.type=="double"?"SL_NATIVE_VALUE_F64":p.type=="boolean"?"SL_NATIVE_VALUE_BOOL":"SL_NATIVE_VALUE_UTF8";
            out << "        if (argv[" << i << "].tag != " << tag << ") return SL_NATIVE_CALL_INVALID_ARGUMENT;\n";
            if (p.type=="String")
                out << "        SLNativeString slv_arg_" << i << " = { argv[" << i << "].as.utf8.data, argv[" << i << "].as.utf8.length };\n";
            else if (p.type=="int") out << "        int64_t slv_arg_" << i << " = argv[" << i << "].as.i64;\n";
            else if (p.type=="double") out << "        double slv_arg_" << i << " = argv[" << i << "].as.f64;\n";
            else out << "        int32_t slv_arg_" << i << " = argv[" << i << "].as.boolean;\n";
        }
        out << "        ";
        if (m.retType=="void") {
            out << nm.symbol << "(";
        } else {
            out << cType(m.retType) << " slv_result = " << nm.symbol << "(";
        }
        for (size_t i=0;i<m.params.size();++i) { if (i) out << ", "; out << "slv_arg_" << i; }
        out << ");\n";
        if (m.retType=="void") out << "        out_result->tag = SL_NATIVE_VALUE_NULL;\n";
        else if (m.retType=="int") out << "        out_result->tag = SL_NATIVE_VALUE_I64; out_result->as.i64 = slv_result;\n";
        else if (m.retType=="double") out << "        out_result->tag = SL_NATIVE_VALUE_F64; out_result->as.f64 = slv_result;\n";
        else if (m.retType=="boolean") out << "        out_result->tag = SL_NATIVE_VALUE_BOOL; out_result->as.boolean = slv_result ? 1 : 0;\n";
        else out << "        out_result->tag = SL_NATIVE_VALUE_UTF8; out_result->as.utf8.data = slv_result.data; out_result->as.utf8.length = slv_result.length;\n";
        out << "        return SL_NATIVE_CALL_OK;\n    }\n";
    }
    out << "    return SL_NATIVE_CALL_NOT_HANDLED;\n}\n";
    return out.str();
}

} // namespace nordshrift
