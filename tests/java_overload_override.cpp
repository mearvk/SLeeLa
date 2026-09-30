#include "../impl/frontend/java_equivalence.h"
#include <cassert>
#include <iostream>
using namespace sleela;
static JavaTypeDescriptor ref(const char* n,JavaReferenceKind k=JavaReferenceKind::Class){JavaTypeDescriptor t;t.qualifiedName=n;t.referenceKind=k;return t;}
static JavaTypeDescriptor prim(JavaPrimitiveType p){JavaTypeDescriptor t;t.primitive=true;t.primitiveType=p;return t;}
static JavaDeclarationDescriptor method(const char* n,JavaTypeDescriptor a){JavaDeclarationDescriptor d;d.kind=JavaDeclarationKind::Method;d.name=n;d.parameterTypes.push_back(a);d.returnType="void";return d;}
int main(){
 JavaMethodCandidate a;a.method=method("m",prim(JavaPrimitiveType::Int));
 JavaMethodCandidate b;b.method=method("m",prim(JavaPrimitiveType::Long));
 auto r=resolveOverload({a,b},{prim(JavaPrimitiveType::Int)},JavaApplicabilityPhase::Strict);
 assert(r.status==JavaResolutionStatus::Selected && r.selectedIndex==0);
 JavaMethodCandidate generic;generic.method.name="identity";generic.method.kind=JavaDeclarationKind::Method;generic.method.typeParameters={"T"};generic.method.parameterTypes={ref("T",JavaReferenceKind::TypeVariable)};generic.method.returnType="T";
 auto inf=inferGenericMethodTypes(generic,{ref("java.lang.String")}); assert(inf.success && inf.inferred["T"].qualifiedName=="java.lang.String");
 auto inv=inferInvocationType(generic,{ref("java.lang.String")}); assert(inv.success && inv.invocationType.returnType=="java.lang.String");
 JavaExpressionDescriptor lambda;lambda.kind=JavaExpressionKind::Lambda; auto pert=assessArgumentPertinence(lambda,ref("java.util.function.Function")); assert(!pert.pertinent);
 JavaMethodCandidate specific,general;specific.method=method("pick",ref("java.lang.String"));general.method=method("pick",ref("java.lang.Object"));
 auto ms=compareMostSpecific(specific,general,{ref("java.lang.String")});assert(ms.firstMoreSpecific);
 auto set=chooseMostSpecific({specific,general},{0,1},{ref("java.lang.String")});assert(set.selectedIndex==0 && !set.ambiguous);
 JavaMethodCandidate var;var.isVarargs=true;var.method.parameterTypes={prim(JavaPrimitiveType::Int),ref("java.lang.String")};assert(variableArityApplicable(var,{prim(JavaPrimitiveType::Int),ref("java.lang.String"),ref("java.lang.String")}));
 JavaMethodCandidate d1,d2;d1.isDefault=d2.isDefault=true;d1.isInterfaceMethod=d2.isInterfaceMethod=true;d1.method=method("x",ref("int"));d2.method=method("x",ref("int"));d1.method.owner="I1";d2.method.owner="I2";
 auto ir=resolveInterfaceDefaults({d1,d2});assert(!ir.conflicts.empty());
 auto base=method("read",ref("int"));auto derived=method("read",ref("int"));base.returnType="java.lang.Object";derived.returnType="java.lang.String";base.thrownTypes={"java.io.IOException"};assert(isOverrideCompatible(base,derived,JavaAccessLevel::Protected,JavaAccessLevel::Public,false,false,false,false));
 derived.thrownTypes={"java.sql.SQLException"};assert(!checkedExceptionsCompatible(base,derived));
 std::cout<<"java overload/override refinement tests passed\n";return 0;
}