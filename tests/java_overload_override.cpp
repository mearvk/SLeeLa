#include "../impl/frontend/java_equivalence.h"
#include <cassert>
#include <iostream>
using namespace sleela;
static JavaTypeDescriptor ref(const char* n, JavaReferenceKind k=JavaReferenceKind::Class){JavaTypeDescriptor t;t.qualifiedName=n;t.referenceKind=k;return t;}
static JavaTypeDescriptor prim(JavaPrimitiveType p){JavaTypeDescriptor t;t.primitive=true;t.primitiveType=p;return t;}
static JavaDeclarationDescriptor method(const char* n,JavaTypeDescriptor a){JavaDeclarationDescriptor d;d.kind=JavaDeclarationKind::Method;d.name=n;d.parameterTypes.push_back(a);d.returnType="void";return d;}
int main(){
 JavaMethodCandidate a;a.method=method("m",prim(JavaPrimitiveType::Int));
 JavaMethodCandidate b;b.method=method("m",prim(JavaPrimitiveType::Long));
 auto r=resolveOverload({a,b},{prim(JavaPrimitiveType::Int)},JavaApplicabilityPhase::Strict);
 assert(r.status==JavaResolutionStatus::Selected && r.selectedIndex==0);

 JavaMethodCandidate generic;generic.method.name="identity";generic.method.kind=JavaDeclarationKind::Method;
 generic.method.typeParameters={"T"};generic.method.parameterTypes={ref("T",JavaReferenceKind::TypeVariable)};
 auto inf=inferGenericMethodTypes(generic,{ref("java.lang.String")});
 assert(inf.success && inf.inferred["T"].qualifiedName=="java.lang.String");

 JavaExpressionDescriptor lambda;lambda.kind=JavaExpressionKind::Lambda;
 auto pert=assessArgumentPertinence(lambda,ref("java.util.function.Function"));
 assert(!pert.pertinent && pert.potentiallyCompatible);

 JavaMethodCandidate specific,general;
 specific.method=method("pick",ref("java.lang.String"));
 general.method=method("pick",ref("java.lang.Object"));
 auto ms=compareMostSpecific(specific,general,{ref("java.lang.String")});
 assert(ms.firstMoreSpecific && !ms.secondMoreSpecific);

 auto base=method("read",ref("int"));auto derived=method("read",ref("int"));
 base.returnType="java.lang.Object";derived.returnType="java.lang.String";
 base.thrownTypes={"java.io.IOException"};derived.thrownTypes={};
 assert(isOverrideCompatible(base,derived,JavaAccessLevel::Protected,JavaAccessLevel::Public,false,false,false,false));
 derived.thrownTypes={"java.sql.SQLException"};
 assert(!checkedExceptionsCompatible(base,derived));

 JavaMethodCandidate var;var.isVarargs=true;var.method.parameterTypes={prim(JavaPrimitiveType::Int),ref("java.lang.String")};
 assert(variableArityApplicable(var,{prim(JavaPrimitiveType::Int),ref("java.lang.String"),ref("java.lang.String")}));
 std::cout<<"java overload/override refinement tests passed\n";
 return 0;
}