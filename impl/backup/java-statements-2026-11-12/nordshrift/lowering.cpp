// ===========================================================================
// lowering.cpp -- Shared Sleela AST -> Nordshrift intermediate representation.
// ===========================================================================
#include "lowering.h"

#include <sstream>
#include <typeinfo>

namespace nordshrift {
namespace {

using namespace sleela;

struct Lowerer {
    LoweredProgram out;

    void fail(const std::string& message) {
        if (out.error.empty()) out.error = message;
    }

    void expr(const Expr* e) {
        if (!e) { fail("null expression"); return; }
        if (auto x=dynamic_cast<const IntLit*>(e)) { out.ops.push_back("expr.int:"+std::to_string(x->value)); return; }
        if (auto x=dynamic_cast<const DoubleLit*>(e)) { out.ops.push_back("expr.double:"+std::to_string(x->value)); return; }
        if (auto x=dynamic_cast<const BoolLit*>(e)) { out.ops.push_back(std::string("expr.bool:")+(x->value?"true":"false")); return; }
        if (auto x=dynamic_cast<const StrLit*>(e)) { out.ops.push_back("expr.string:"+x->value); return; }
        if (dynamic_cast<const NullLit*>(e)) { out.ops.push_back("expr.null"); return; }
        if (auto x=dynamic_cast<const VarExpr*>(e)) { out.ops.push_back("expr.var:"+x->name); return; }
        if (auto x=dynamic_cast<const Unary*>(e)) { out.ops.push_back("expr.unary:"+x->op); expr(x->operand.get()); out.ops.push_back("expr.unary.end"); return; }
        if (auto x=dynamic_cast<const Binary*>(e)) { out.ops.push_back("expr.binary:"+x->op); expr(x->lhs.get()); expr(x->rhs.get()); out.ops.push_back("expr.binary.end"); return; }
        if (auto x=dynamic_cast<const Call*>(e)) {
            out.ops.push_back("expr.call:"+x->callee+":"+std::to_string(x->args.size()));
            for (const auto& a:x->args) expr(a.get());
            out.ops.push_back("expr.call.end"); return;
        }
        if (auto x=dynamic_cast<const MethodCall*>(e)) {
            out.ops.push_back("expr.method:"+x->method+":"+std::to_string(x->args.size()));
            expr(x->receiver.get()); for (const auto& a:x->args) expr(a.get());
            out.ops.push_back("expr.method.end"); return;
        }
        if (auto x=dynamic_cast<const NewExpr*>(e)) { out.ops.push_back("expr.new:"+x->typeName); return; }
        if (auto x=dynamic_cast<const MemberAccess*>(e)) { out.ops.push_back("expr.member:"+x->field); expr(x->base.get()); out.ops.push_back("expr.member.end"); return; }
        fail(std::string("unsupported expression node: ")+typeid(*e).name());
    }

    void stmt(const Stmt* s) {
        if (!s) { fail("null statement"); return; }
        if (auto x=dynamic_cast<const Block*>(s)) {
            out.ops.push_back("stmt.block:"+std::to_string(x->stmts.size()));
            for (const auto& v:x->stmts) stmt(v.get());
            out.ops.push_back("stmt.block.end"); return;
        }
        if (auto x=dynamic_cast<const VarDecl*>(s)) {
            out.ops.push_back("stmt.var:"+x->type+":"+x->name); if(x->init) expr(x->init.get()); out.ops.push_back("stmt.var.end"); return;
        }
        if (auto x=dynamic_cast<const Assign*>(s)) { out.ops.push_back("stmt.assign:"+x->name); expr(x->value.get()); out.ops.push_back("stmt.assign.end"); return; }
        if (auto x=dynamic_cast<const FieldAssign*>(s)) { out.ops.push_back("stmt.fieldassign:"+x->field); expr(x->base.get()); expr(x->value.get()); out.ops.push_back("stmt.fieldassign.end"); return; }
        if (auto x=dynamic_cast<const ExprStmt*>(s)) { out.ops.push_back("stmt.expr"); expr(x->expr.get()); out.ops.push_back("stmt.expr.end"); return; }
        if (auto x=dynamic_cast<const PrintStmt*>(s)) { out.ops.push_back("stmt.print"); expr(x->expr.get()); out.ops.push_back("stmt.print.end"); return; }
        if (auto x=dynamic_cast<const ReturnStmt*>(s)) { out.ops.push_back("stmt.return"); if(x->value) expr(x->value.get()); out.ops.push_back("stmt.return.end"); return; }
        if (auto x=dynamic_cast<const IfStmt*>(s)) { out.ops.push_back("stmt.if"); expr(x->cond.get()); stmt(x->thenS.get()); if(x->elseS) stmt(x->elseS.get()); out.ops.push_back("stmt.if.end"); return; }
        if (auto x=dynamic_cast<const WhileStmt*>(s)) { out.ops.push_back("stmt.while"); expr(x->cond.get()); stmt(x->body.get()); out.ops.push_back("stmt.while.end"); return; }
        if (auto x=dynamic_cast<const ForStmt*>(s)) { out.ops.push_back("stmt.for"); if(x->init) stmt(x->init.get()); if(x->cond) expr(x->cond.get()); if(x->update) stmt(x->update.get()); stmt(x->body.get()); out.ops.push_back("stmt.for.end"); return; }
        fail(std::string("unsupported statement node: ")+typeid(*s).name());
    }

    void run(const Program& p) {
        out.ops.push_back("program");
        for (const auto& i:p.imports) out.ops.push_back("import:"+i);
        for (const auto& s:p.structs) {
            out.ops.push_back("struct:"+s.name+":"+std::to_string(s.fields.size()));
            for (const auto& f:s.fields) out.ops.push_back("field:"+f.type+":"+f.name);
            out.ops.push_back("struct.end");
        }
        for (const auto& c:p.classes) {
            out.ops.push_back("class:"+c.name+":"+std::to_string(c.fields.size())+":"+std::to_string(c.methods.size()));
            for (const auto& f:c.fields) {
                out.ops.push_back("field:"+f.type+":"+f.name+":"+(f.isStatic?"static":"instance")+":"+(f.isProtected?"protected":"public"));
                if(f.init) expr(f.init.get());
                out.ops.push_back("field.end");
            }
            for (const auto& m:c.methods) {
                out.ops.push_back("method:"+m.retType+":"+m.name+":"+std::to_string(m.params.size()));
                for(const auto& p:m.params) out.ops.push_back("param:"+p.type+":"+p.name);
                stmt(m.body.get());
                out.ops.push_back("method.end");
            }
            out.ops.push_back("class.end");
        }
        out.ops.push_back("program.end");
    }
};

} // namespace

LoweredProgram lowerProgram(const sleela::Program& program) {
    Lowerer l; l.run(program); return l.out;
}

std::string serializeLoweredProgram(const LoweredProgram& lowered) {
    std::ostringstream out;
    for (const auto& op:lowered.ops) out << op << '\n';
    if (!lowered.error.empty()) out << "ERROR:" << lowered.error << '\n';
    return out.str();
}

} // namespace nordshrift
