#include "../impl/frontend/java_equivalence.h"
#include <cassert>
#include <iostream>
using namespace sleela;

static JavaTypeDescriptor ref(const char* n, JavaReferenceKind k=JavaReferenceKind::Class){ JavaTypeDescriptor t; t.qualifiedName=n; t.referenceKind=k; return t; }
static JavaTypeDescriptor prim(JavaPrimitiveType p){ JavaTypeDescriptor t; t.primitive=true; t.primitiveType=p; return t; }

int main(){
  JavaMethodCandidate generic;
  generic.method.name="identity";
  generic.method.kind=JavaDeclarationKind::Method;
  generic.method.typeParameters={"T"};
  generic.method.parameterTypes={ref("T",JavaReferenceKind::TypeVariable)};
  auto inf=inferGenericMethodTypes(generic,{ref("java.lang.String")});
  assert(inf.success && inf.inferred["T"].qualifiedName=="java.lang.String");

  JavaExpressionDescriptor lambda; lambda.kind=JavaExpressionKind::Lambda;
  auto pert=assessArgumentPertinence(lambda,ref("java.util.function.Function"));
  assert(!pert.pertinent && pert.potentiallyCompatible);

  JavaMethodCandidate a,b;
  a.method.name=b.method.name="m";
  a.method.kind=b.method.kind=JavaDeclarationKind::Method;
  a.method.parameterTypes={ref("java.lang.String")};
  b.method.parameterTypes={ref("java.lang.Object")};
  auto ms=compareMostSpecific(a,b,{ref("java.lang.String")});
  assert(ms.firstMoreSpecific && !ms.secondMoreSpecific);

  JavaDeclarationDescriptor base, derived;
  base.kind=derived.kind=JavaDeclarationKind::Method;
  base.name=derived.name="read";
  base.parameterTypes={};
  base.returnType="java.lang.Object";
  derived.returnType="java.lang.String";
  base.thrownTypes={"java.io.IOException"};
  derived.thrownTypes={};
  assert(isOverrideCompatible(base,derived,JavaAccessLevel::Protected,JavaAccessLevel::Public,false,false,false,false));

  derived.thrownTypes={"java.sql.SQLException"};
  assert(!checkedExceptionsCompatible(base,derived));

  JavaMethodCandidate var;
  var.isVarargs=true;
  var.method.parameterTypes={prim(JavaPrimitiveType::Int),ref("java.lang.String")};
  assert(variableArityApplicable(var,{prim(JavaPrimitiveType::Int),ref("java.lang.String"),ref("java.lang.String")}));

  std::cout<<"java overload/override refinement tests passed\n";
  return 0;
}
