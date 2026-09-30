#include "../impl/frontend/java_equivalence.h"
#include <cassert>
using namespace sleela;
static JavaTypeDescriptor p(JavaPrimitiveType x){JavaTypeDescriptor t;t.primitive=true;t.primitiveType=x;return t;}
static JavaDeclarationDescriptor method(const char* n,JavaTypeDescriptor a){JavaDeclarationDescriptor d;d.kind=JavaDeclarationKind::Method;d.name=n;d.parameterTypes.push_back(a);d.returnType="void";return d;}
int main(){
 JavaMethodCandidate a;a.method=method("m",p(JavaPrimitiveType::Int));
 JavaMethodCandidate b;b.method=method("m",p(JavaPrimitiveType::Long));
 auto r=resolveOverload({a,b},{p(JavaPrimitiveType::Int)},JavaApplicabilityPhase::Strict);
 assert(r.status==JavaResolutionStatus::Selected && r.selectedIndex==0);
 auto amb=resolveOverload({a,b},{p(JavaPrimitiveType::Short)},JavaApplicabilityPhase::Strict);
 assert(amb.status==JavaResolutionStatus::Ambiguous || amb.status==JavaResolutionStatus::Selected);
 auto base=method("m",p(JavaPrimitiveType::Int));auto derived=method("m",p(JavaPrimitiveType::Int));
 assert(isOverrideEquivalent(base,derived)); assert(isOverrideCompatible(base,derived));
 return 0;
}