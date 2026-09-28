#include "native_add.h"
#include "native_builders.h"
namespace sleela { namespace native {
using namespace sleela::native::builders;
void addSociology(Program&p){
 ClassDecl c;c.name="__NativeSociology";
 auto m=M2("double","__native_sociology_proportion","double","part","double","total");
 m.body->stmts.push_back(Ret(Bn("/",V("part"),V("total"))));c.methods.push_back(std::move(m));
 m=M3("double","__native_sociology_rate","double","events","double","population","double","scale");
 m.body->stmts.push_back(Ret(Bn("*",Bn("/",V("events"),V("population")),V("scale"))));c.methods.push_back(std::move(m));
 m=M2("double","__native_sociology_growth_rate","double","oldValue","double","newValue");
 m.body->stmts.push_back(Ret(Bn("/",Bn("-",V("newValue"),V("oldValue")),V("oldValue"))));c.methods.push_back(std::move(m));
 m=M2("double","__native_sociology_transition_probability","double","transitions","double","origin");
 m.body->stmts.push_back(Ret(Bn("/",V("transitions"),V("origin"))));c.methods.push_back(std::move(m));
 p.classes.push_back(std::move(c));
}
}}
