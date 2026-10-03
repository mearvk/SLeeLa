#include "semantic.h"
#include "annotation_pipeline.h"
#include <map>
#include <set>
#include <string>
namespace sleela {
namespace {
enum class Kind { Void, Int, Double, Bool, String, Null, Struct, Unknown, Error };
struct Type { Kind kind=Kind::Unknown; std::string name; };
static Type typeOf(const std::string& n){
 // Accept the Java-style keyword names the lexer produces (int, double,
 // boolean, String, void) as well as the lowercase aliases (bool, string).
 if(n=="void"){return{Kind::Void,{}};}
 if(n=="int"){return{Kind::Int,{}};}
 if(n=="double"){return{Kind::Double,{}};}
 if(n=="bool"||n=="boolean"){return{Kind::Bool,{}};}
 if(n=="string"||n=="String"){return{Kind::String,{}};}
 return{Kind::Struct,n};
}
static std::string nameOf(const Type&t){
 switch(t.kind){case Kind::Void:return"void";case Kind::Int:return"int";case Kind::Double:return"double";case Kind::Bool:return"bool";case Kind::String:return"string";case Kind::Null:return"null";case Kind::Struct:return t.name;case Kind::Unknown:return"unknown";default:return"error";}
}
static bool same(const Type&a,const Type&b){return a.kind==b.kind&&(a.kind!=Kind::Struct||a.name==b.name);}
static bool numeric(const Type&t){return t.kind==Kind::Int||t.kind==Kind::Double;}
static bool assignable(const Type&to,const Type&from){
 return to.kind==Kind::Unknown||from.kind==Kind::Unknown||(to.kind==Kind::Double&&from.kind==Kind::Int)||(from.kind==Kind::Null&&(to.kind==Kind::Struct||to.kind==Kind::String))||same(to,from);
}
static bool builtin(const std::string&n){
 static const std::set<std::string> s={
 "spawn","join","lock","unlock","send","recv","structPack","structUnpack","listen","accept","connect","sockread","sockwrite","sockclose",
 "pipe","pipePeer","fifoCreate","npfsCreate","openFile","read","write","close","unlinkFile",
 "timeUtcMillis","timeUtcNanos","timeMonotonicNanos","timePrecisionMillis","timeLocation","timeHttpDate","timeJson","timeNtp","timeSetLocation",
 "Munction.start","synchroOpen","synchroDispatch","synchroReport","synchroClose","synchroSent","synchroReceived","synchroMean","synchroMin","synchroMax","synchroP95","synchroLoss",
 "bestOfNew","bestOfWeight","bestOfMinVersion","bestOfCostBudget","bestOfCandidate","bestOfRecord","bestOfScore","bestOfBest","bestOfChoice","bestOfReport","bestOfClose",
 "bestOfMean","bestOfLoss","bestOfJitter","bestOfCertainty","bestOfCandidateArch","bestOfArchRealized","bestOfArch","bestOfArchParam","bestOfArchState",
 "audioNew","audioAdd","audioControls","audioValidate","audioRender","audioClose","audioPlatform",
 "conduct","role","insight","congruent","route","sysdepth","degreemax"};
 return s.count(n)!=0;
}
struct MethodSig{const Method* m=nullptr;std::string owner;};
class Analyzer{
 const Program&p; const SyntaxVersion&syntax; SemanticResult r;
 std::set<std::string> structs; std::set<std::string> classNames; std::map<std::string,std::map<std::string,Type>> fields;
 std::map<std::string,MethodSig> methods; std::map<std::string,Type> globals; std::map<std::string,std::string> owners;
 struct Scope{std::map<std::string,Type> vars;}; std::vector<Scope> scopes; const Method* cur=nullptr; std::string cls;
 void err(const std::string&s){r.errors.push_back("Semantic error: "+s);}
 // A type is known if it is a scalar keyword (Java or lowercase alias), a
 // declared struct, or a declared class/interface name (so a constructor's
 // return type and object-typed parameters resolve).
 bool known(const std::string&t)const{return t=="void"||t=="int"||t=="double"||t=="bool"||t=="boolean"||t=="string"||t=="String"||structs.count(t)||classNames.count(t);}
 Type tn(const std::string&t)const{return known(t)?typeOf(t):Type{Kind::Error,t};}
 void collect(){
  for(const auto&s:p.structs)if(!structs.insert(s.name).second)err("duplicate struct '"+s.name+"'");
  for(const auto&s:p.structs){std::map<std::string,Type> f;if(s.fields.size()>64)err("struct '"+s.name+"' exceeds 64 fields");
   for(const auto&x:s.fields){if(f.count(x.name))err("duplicate field '"+x.name+"' in struct '"+s.name+"'");
    Type t=tn(x.type);if(t.kind==Kind::Error)err("unknown type '"+x.type+"' for field '"+x.name+"'");if(t.kind==Kind::Void)err("field '"+x.name+"' cannot be void");f[x.name]=t;}fields[s.name]=f;}
  for(const auto&c:p.classes)classNames.insert(c.name);
  for(const auto&c:p.classes){for(const auto&f:c.fields){if(globals.count(f.name))err("duplicate global field '"+f.name+"'");
    Type t=tn(f.type);if(t.kind==Kind::Error)err("unknown type '"+f.type+"' for field '"+f.name+"'");if(t.kind==Kind::Void)err("field '"+f.name+"' cannot be void");globals[f.name]=t;owners[f.name]=c.name;}
   for(const auto&m:c.methods){if(methods.count(m.name))err("duplicate method '"+m.name+"'");
    if(!known(m.retType))err("unknown return type '"+m.retType+"' for method '"+m.name+"'");
    if(m.isProtected&&!m.isStatic){err("protected method '"+m.name+"' must also be static");}
    methods[m.name]={&m,c.name};}}
 }
 void push(){scopes.push_back({});} void pop(){scopes.pop_back();}
 bool declare(const std::string&n,const Type&t){auto&v=scopes.back().vars;if(v.count(n)){err("duplicate local variable '"+n+"'");return false;}v[n]=t;return true;}
 Type lookup(const std::string&n)const{for(auto i=scopes.rbegin();i!=scopes.rend();++i){auto x=i->vars.find(n);if(x!=i->vars.end())return x->second;}auto g=globals.find(n);if(g!=globals.end())return g->second;if(n=="next")return{Kind::Int,{}};return{Kind::Error,n};}
 void declarations(){
  if(!methods.count("main"))err("no 'main' method found");
  for(const auto&c:p.classes){for(const auto&f:c.fields){if(f.isProtected&&!f.isStatic)err("protected field '"+f.name+"' must also be static");}
   for(const auto&m:c.methods){std::set<std::string> seen;for(const auto&x:m.params){if(!known(x.type))err("unknown parameter type '"+x.type+"'");if(x.type=="void")err("parameter '"+x.name+"' cannot be void");if(!seen.insert(x.name).second)err("duplicate parameter '"+x.name+"' in method '"+m.name+"'");}}}
 }
 void method(const ClassDecl&owner,const Method&m){cur=&m;cls=owner.name;scopes.clear();push();for(const auto&x:m.params)declare(x.name,tn(x.type));block(*m.body);if(m.name=="main"&&m.retType!="void")err("main must return void");pop();}
 void block(const Block&b){for(const auto&s:b.stmts)stmt(*s);}
 void stmt(const Stmt&s){
  if(auto b=dynamic_cast<const Block*>(&s)){push();block(*b);pop();return;}
  if(auto d=dynamic_cast<const VarDecl*>(&s)){Type t=tn(d->type);if(t.kind==Kind::Error){err("unknown variable type '"+d->type+"'");return;}if(t.kind==Kind::Void){err("variable '"+d->name+"' cannot be void");return;}if(d->init){Type g=expr(*d->init);if(!assignable(t,g))err("initializer for variable '"+d->name+"' has type "+nameOf(g)+", expected "+nameOf(t));}declare(d->name,t);return;}
  if(auto a=dynamic_cast<const Assign*>(&s)){Type t=lookup(a->name);if(t.kind==Kind::Error){err("assignment to undeclared variable '"+a->name+"'");return;}Type g=expr(*a->value);if(!assignable(t,g))err("cannot assign "+nameOf(g)+" to '"+a->name+"' of type "+nameOf(t));return;}
  if(auto f=dynamic_cast<const FieldAssign*>(&s)){Type t=member(*f->base,f->field),g=expr(*f->value);if(t.kind!=Kind::Error&&!assignable(t,g))err("cannot assign "+nameOf(g)+" to field '"+f->field+"' of type "+nameOf(t));return;}
  if(auto e=dynamic_cast<const ExprStmt*>(&s)){expr(*e->expr);return;} if(auto p=dynamic_cast<const PrintStmt*>(&s)){Type t=expr(*p->expr);if(t.kind==Kind::Void)err("print() cannot print void");return;}
  if(auto x=dynamic_cast<const ReturnStmt*>(&s)){Type want=tn(cur->retType);
   if(want.kind==Kind::Void){if(x->value){expr(*x->value);err("void method '"+cur->name+"' cannot return a value");}return;}
   if(!x->value){err("non-void method '"+cur->name+"' must return a value");return;}
   Type g=expr(*x->value);if(!assignable(want,g))err("method '"+cur->name+"' returns "+nameOf(g)+", expected "+nameOf(want));return;}
  if(auto i=dynamic_cast<const IfStmt*>(&s)){requireBool(expr(*i->cond),"if condition");stmt(*i->thenS);if(i->elseS)stmt(*i->elseS);return;}
  if(auto w=dynamic_cast<const WhileStmt*>(&s)){requireBool(expr(*w->cond),"while condition");stmt(*w->body);return;}
  if(auto f=dynamic_cast<const ForStmt*>(&s)){push();if(f->init)stmt(*f->init);if(f->cond)requireBool(expr(*f->cond),"for condition");if(f->update)stmt(*f->update);stmt(*f->body);pop();return;}
  if(auto d=dynamic_cast<const DoStmt*>(&s)){stmt(*d->body);requireBool(expr(*d->cond),"do-while condition");return;}
  if(dynamic_cast<const BreakStmt*>(&s))return;
  if(dynamic_cast<const ContinueStmt*>(&s))return;
  if(auto t=dynamic_cast<const ThrowStmt*>(&s)){expr(*t->value);return;}
  if(auto a=dynamic_cast<const AssertStmt*>(&s)){requireBool(expr(*a->cond),"assert condition");if(a->message)expr(*a->message);return;}
  if(auto y=dynamic_cast<const YieldStmt*>(&s)){expr(*y->value);return;}
  if(auto sy=dynamic_cast<const SynchronizedStmt*>(&s)){expr(*sy->monitor);push();block(*sy->body);pop();return;}
  if(auto tr=dynamic_cast<const TryStmt*>(&s)){push();block(*tr->body);pop();for(const auto&c:tr->catches){push();declare(c.variable,c.type.empty()?Type{Kind::Unknown,{}}:tn(c.type));block(*c.body);pop();}if(tr->finallyBlock){push();block(*tr->finallyBlock);pop();}return;}
  if(auto sw=dynamic_cast<const SwitchStmt*>(&s)){expr(*sw->selector);push();for(const auto&c:sw->cases){for(const auto&l:c.labels)expr(*l);for(const auto&st:c.statements)stmt(*st);}pop();return;}
  err("unknown statement kind");
 }
 void requireBool(const Type&t,const char*w){if(t.kind!=Kind::Bool&&t.kind!=Kind::Unknown)err(std::string(w)+" must be bool, got "+nameOf(t));}
 // The `next` / `next.next` / `next.next.next.next` system-degree idiom (see
 // compiler emitVar/emitMember): `next` is Degree 1 and each `.next` step is a
 // bounded symbolic relation, not a struct field access. Recognize an all-`next`
 // chain so the analyzer treats it as an integer degree rather than a member.
 static bool isNextChain(const Expr&e){
  if(auto v=dynamic_cast<const VarExpr*>(&e))return v->name=="next";
  if(auto m=dynamic_cast<const MemberAccess*>(&e))return m->field=="next"&&isNextChain(*m->base);
  return false;
 }
 Type member(const Expr&e,const std::string&f){Type b=expr(e);if(b.kind==Kind::Unknown)return b;if(b.kind!=Kind::Struct){err("member '"+f+"' requires a struct value, got "+nameOf(b));return{Kind::Error,{}};}auto s=fields.find(b.name);if(s==fields.end()){err("unknown struct '"+b.name+"'");return{Kind::Error,{}};}auto x=s->second.find(f);if(x==s->second.end()){err("struct '"+b.name+"' has no field '"+f+"'");return{Kind::Error,{}};}return x->second;}
 Type expr(const Expr&e){
  if(dynamic_cast<const IntLit*>(&e)){return{Kind::Int,{}};}
  if(dynamic_cast<const DoubleLit*>(&e)){return{Kind::Double,{}};}
  if(dynamic_cast<const BoolLit*>(&e)){return{Kind::Bool,{}};}
  if(dynamic_cast<const StrLit*>(&e)){return{Kind::String,{}};}
  if(dynamic_cast<const NullLit*>(&e)){return{Kind::Null,{}};}
  if(auto v=dynamic_cast<const VarExpr*>(&e)){Type t=lookup(v->name);if(t.kind==Kind::Error)err("use of undeclared variable '"+v->name+"'");return t;}
  if(auto n=dynamic_cast<const NewExpr*>(&e)){if(!structs.count(n->typeName)){err("new of unknown struct '"+n->typeName+"'");return{Kind::Error,{}};}return{Kind::Struct,n->typeName};}
  if(auto m=dynamic_cast<const MemberAccess*>(&e)){if(m->field=="next"&&isNextChain(*m->base))return{Kind::Int,{}};return member(*m->base,m->field);}
  if(auto u=dynamic_cast<const Unary*>(&e)){Type t=expr(*u->operand);if(u->op=="-"&&!numeric(t)&&t.kind!=Kind::Unknown)err("unary '-' requires numeric operand, got "+nameOf(t));if(u->op=="!"&&t.kind!=Kind::Bool&&t.kind!=Kind::Unknown)err("unary '!' requires bool operand, got "+nameOf(t));return t;}
  if(auto b=dynamic_cast<const Binary*>(&e)){return binary(*b);}
  if(auto c=dynamic_cast<const Call*>(&e)){return call(*c);}
  if(auto m=dynamic_cast<const MethodCall*>(&e)){return fluent(*m);}
  if(auto a=dynamic_cast<const AssignmentExpr*>(&e)){
   Type g=expr(*a->value);
   if(auto v=dynamic_cast<const VarExpr*>(a->target.get())){Type t=lookup(v->name);if(t.kind==Kind::Error){err("assignment to undeclared variable '"+v->name+"'");return{Kind::Error,{}};}if(!assignable(t,g))err("cannot assign "+nameOf(g)+" to '"+v->name+"' of type "+nameOf(t));return t;}
   if(auto mm=dynamic_cast<const MemberAccess*>(a->target.get())){Type t=member(*mm->base,mm->field);if(t.kind!=Kind::Error&&!assignable(t,g))err("cannot assign "+nameOf(g)+" to field '"+mm->field+"'");return t;}
   err("assignment target must be a variable or a struct field");return{Kind::Error,{}};
  }
  if(auto c=dynamic_cast<const ConditionalExpr*>(&e)){requireBool(expr(*c->cond),"conditional '?:' condition");Type a=expr(*c->thenE),b=expr(*c->elseE);if(assignable(a,b))return a;if(assignable(b,a))return b;return{Kind::Unknown,{}};}
  if(auto io=dynamic_cast<const InstanceOfExpr*>(&e)){expr(*io->value);return{Kind::Bool,{}};}
  if(auto ca=dynamic_cast<const CastExpr*>(&e)){expr(*ca->operand);return tn(ca->typeName);}
  if(dynamic_cast<const SuperExpr*>(&e))return{Kind::Unknown,{}};
  if(dynamic_cast<const ThisExpr*>(&e))return{Kind::Unknown,{}};
  if(auto aa=dynamic_cast<const ArrayAccess*>(&e)){expr(*aa->base);Type idx=expr(*aa->index);if(idx.kind!=Kind::Int&&idx.kind!=Kind::Unknown)err("array index must be int, got "+nameOf(idx));return{Kind::Unknown,{}};}
  if(auto mr=dynamic_cast<const MethodReferenceExpr*>(&e)){expr(*mr->base);return{Kind::Unknown,{}};}
  err("unknown expression kind");return{Kind::Error,{}};
 }
 Type binary(const Binary&b){Type l=expr(*b.lhs),r=expr(*b.rhs);if(b.op=="&&"||b.op=="||"){if(l.kind!=Kind::Bool&&l.kind!=Kind::Unknown)err("operator '"+b.op+"' requires bool operands");if(r.kind!=Kind::Bool&&r.kind!=Kind::Unknown)err("operator '"+b.op+"' requires bool operands");return{Kind::Bool,{}};}
  if(b.op=="+"||b.op=="-"||b.op=="*"||b.op=="/"||b.op=="%"){
   // Java-style: `+` with a String operand is string concatenation; the VM's
   // OP_ADD already renders either operand to text. Only `+` concatenates.
   if(b.op=="+"&&(l.kind==Kind::String||r.kind==Kind::String))return{Kind::String,{}};
   if(!numeric(l)&&l.kind!=Kind::Unknown){err("operator '"+b.op+"' requires numeric operands");}
   if(!numeric(r)&&r.kind!=Kind::Unknown){err("operator '"+b.op+"' requires numeric operands");}
   return{(l.kind==Kind::Double||r.kind==Kind::Double)?Kind::Double:Kind::Int,{}};}
  if(b.op=="<"||b.op=="<="||b.op==">"||b.op==">="){if((!numeric(l)||!numeric(r))&&l.kind!=Kind::Unknown&&r.kind!=Kind::Unknown)err("comparison '"+b.op+"' requires numeric operands");return{Kind::Bool,{}};}
  if(b.op=="=="||b.op=="!="){if(!assignable(l,r)&&!assignable(r,l)&&l.kind!=Kind::Unknown&&r.kind!=Kind::Unknown)err("equality operands have incompatible types "+nameOf(l)+" and "+nameOf(r));return{Kind::Bool,{}};}err("unknown binary operator '"+b.op+"'");return{Kind::Error,{}};
 }
 Type call(const Call&c){auto it=methods.find(c.callee);if(it!=methods.end()){const Method&m=*it->second.m;if(c.args.size()!=m.params.size())err("method '"+c.callee+"' expects "+std::to_string(m.params.size())+" argument(s), got "+std::to_string(c.args.size()));size_t n=c.args.size()<m.params.size()?c.args.size():m.params.size();for(size_t i=0;i<n;i++){Type g=expr(*c.args[i]),w=tn(m.params[i].type);if(!assignable(w,g))err("argument "+std::to_string(i+1)+" to '"+c.callee+"' has type "+nameOf(g)+", expected "+nameOf(w));}if(m.isProtected&&it->second.owner!=cls)err("protected method access denied for '"+c.callee+"'");return tn(m.retType);}
  // `spawn(method)` names a zero-arg method to run on a new thread; its single
  // argument is a method name, not a value, so it is not resolved as a variable.
  if(c.callee=="spawn"){if(c.args.size()!=1){err("spawn(method) takes exactly one argument");return{Kind::Unknown,{}};}auto v=dynamic_cast<const VarExpr*>(c.args[0].get());if(!v||!methods.count(v->name))err("spawn(method) argument must be a declared method name");return{Kind::Unknown,{}};}
  // `structUnpack(TypeName, json)` -- the first argument is a struct type name,
  // not a value; the result is an instance of that struct type.
  if(c.callee=="structUnpack"){if(c.args.size()!=2){err("structUnpack(TypeName, json) takes exactly two arguments");return{Kind::Error,{}};}auto v=dynamic_cast<const VarExpr*>(c.args[0].get());if(!v||!structs.count(v->name)){err("structUnpack first argument must be a declared struct type name");return{Kind::Error,{}};}expr(*c.args[1]);return{Kind::Struct,v->name};}
  for(const auto&a:c.args){expr(*a);}
  if(!builtin(c.callee)){err("call to unknown method '"+c.callee+"'");}
  const std::string&n=c.callee;if(n=="conduct"||n=="congruent")return{Kind::Bool,{}};if(n=="role"||n=="insight"||n=="route"||n=="timeLocation"||n=="timeHttpDate"||n=="timeJson")return{Kind::String,{}};if(n=="sysdepth"||n=="degreemax"||n=="timeUtcMillis"||n=="timeUtcNanos"||n=="timeMonotonicNanos"||n=="timePrecisionMillis")return{Kind::Int,{}};if(n=="synchroMean"||n=="synchroMin"||n=="synchroMax"||n=="synchroP95"||n=="synchroLoss"||n=="bestOfMean"||n=="bestOfLoss"||n=="bestOfJitter"||n=="bestOfCertainty")return{Kind::Double,{}};if(n=="Munction.start"||n=="read"||n=="recv"||n=="sockread"||n=="timeNtp")return{Kind::Unknown,{}};return{Kind::Unknown,{}};
 }
 Type fluent(const MethodCall&m){
  // `Munction.start(name)` opens a reach and yields a reach handle. The receiver
  // is the `Munction` opener identifier, not a declared variable, so handle it
  // before trying to resolve the receiver as a value.
  if(m.method=="start"){if(auto r=dynamic_cast<const VarExpr*>(m.receiver.get())){if(r->name=="Munction"){if(m.args.size()!=1)err("Munction.start(name) takes exactly one argument");else{Type t=expr(*m.args[0]);if(t.kind!=Kind::String&&t.kind!=Kind::Unknown)err("Munction.start name must be string");}return{Kind::Unknown,{}};}}}
  Type rcv=expr(*m.receiver);static const std::set<std::string>v={"connect","enable","send","thatch","consume","latch","closeWithReceipt","close","reception"};if(!v.count(m.method)){err("unknown fluent method '"+m.method+"'");return{Kind::Error,{}};}if(m.method=="consume"||m.method=="latch"||m.method=="close"||m.method=="reception"||m.method=="closeWithReceipt"){if(!m.args.empty())err("Munction "+m.method+"() takes no arguments");}else{if(m.args.size()!=1)err("Munction "+m.method+"() takes exactly one argument");if(!m.args.empty()){Type t=expr(*m.args[0]);if(t.kind!=Kind::String&&t.kind!=Kind::Unknown)err("Munction "+m.method+" argument must be string");}}return m.method=="closeWithReceipt"?Type{Kind::String,{}}:rcv;}
public:
 Analyzer(const Program&x,const SyntaxVersion&s):p(x),syntax(s){}
 SemanticResult run(){
  collect();
  declarations();
  for(const auto&d:frontend::validateLanguageAnnotations(p.annotations)){
    if(d.severity==frontend::AnnotationDiagnostic::Severity::Error) err(d.message);
    else r.warnings.push_back("Semantic warning: "+d.message);
  }
  for(const auto&c:p.classes)for(const auto&m:c.methods)method(c,m);
  return r;
}
};
}
SemanticResult analyzeSemantics(const Program&p,const SyntaxVersion&s){return Analyzer(p,s).run();}
}
