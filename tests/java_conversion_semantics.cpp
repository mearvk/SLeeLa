#include "../impl/frontend/java_equivalence.h"
#include <cassert>
using namespace sleela;
int main(){
  JavaTypeDescriptor i; i.primitive=true; i.primitiveType=JavaPrimitiveType::Int;
  JavaTypeDescriptor l; l.primitive=true; l.primitiveType=JavaPrimitiveType::Long;
  auto w=classifyJavaConversion(i,l,JavaConversionContext::Assignment);
  assert(w.permitted && w.kind==JavaConversionKind::WideningPrimitive);
  auto n=classifyJavaConversion(l,i,JavaConversionContext::Assignment);
  assert(!n.permitted);
  auto cast=classifyJavaConversion(l,i,JavaConversionContext::Casting);
  assert(cast.permitted && cast.kind==JavaConversionKind::NarrowingPrimitive);
  auto p=unaryNumericPromotion(i);
  assert(p.normalized()=="int");
  JavaTypeDescriptor b; b.primitive=true; b.primitiveType=JavaPrimitiveType::Byte;
  assert(binaryNumericPromotion(b,i).normalized()=="int");
  auto boxedI=classifyJavaConversion(i,JavaTypeDescriptor{false,JavaPrimitiveType::Void,JavaReferenceKind::Class,"java.lang.Integer",{}, {},0},JavaConversionContext::Assignment);
  assert(boxedI.permitted && boxedI.kind==JavaConversionKind::Boxing);
  return 0;
}
