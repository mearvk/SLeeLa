#include "native_add.h"
#include "native_builders.h"
namespace sleela { namespace native {
using namespace sleela::native::builders;
void addAstrophysics(Program&p){
 ClassDecl c;c.name="__NativeAstrophysics";
 auto m=M2("double","__native_astrophysics_flux_from_luminosity","double","L","double","r");
 m.body->stmts.push_back(Ret(Bn("/",V("L"),Bn("*",D(12.566370614359172),Bn("*",V("r"),V("r"))))));c.methods.push_back(std::move(m));
 m=M2("double","__native_astrophysics_redshift","double","observed","double","rest");
 m.body->stmts.push_back(Ret(Bn("-",Bn("/",V("observed"),V("rest")),D(1))));c.methods.push_back(std::move(m));
 m=M2("double","__native_astrophysics_observed_wavelength","double","rest","double","z");
 m.body->stmts.push_back(Ret(Bn("*",V("rest"),Bn("+",D(1),V("z")))));c.methods.push_back(std::move(m));
 m=M1("double","__native_astrophysics_wien_peak","double","temperature");
 m.body->stmts.push_back(Ret(Bn("/",D(2.897771955e-3),V("temperature"))));c.methods.push_back(std::move(m));
 m=M2("double","__native_astrophysics_kepler_period","double","a","double","mass");
 m.body->stmts.push_back(Ret(Bn("*",D(6.283185307179586),C1("__native_math_sqrt",Bn("/",Bn("*",V("a"),Bn("*",V("a"),V("a"))),Bn("*",D(6.67430e-11),V("mass")))))));c.methods.push_back(std::move(m));
 m=M2("double","__native_astrophysics_escape_velocity","double","mass","double","radius");
 m.body->stmts.push_back(Ret(C1("__native_math_sqrt",Bn("/",Bn("*",D(1.33486e-10),V("mass")),V("radius")))));c.methods.push_back(std::move(m));
 p.classes.push_back(std::move(c));
}
}}
