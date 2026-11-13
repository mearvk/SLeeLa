// ===========================================================================
// java_equivalence.cpp -- Names and normalization for Java equivalence model.
// ===========================================================================
#include "java_equivalence.h"
#include <sstream>
namespace sleela {
std::string JavaTypeDescriptor::normalized() const {
    std::ostringstream out;
    if (primitive) {
        switch (primitiveType) {
            case JavaPrimitiveType::Boolean: out<<"boolean"; break;
            case JavaPrimitiveType::Byte: out<<"byte"; break;
            case JavaPrimitiveType::Short: out<<"short"; break;
            case JavaPrimitiveType::Int: out<<"int"; break;
            case JavaPrimitiveType::Long: out<<"long"; break;
            case JavaPrimitiveType::Char: out<<"char"; break;
            case JavaPrimitiveType::Float: out<<"float"; break;
            case JavaPrimitiveType::Double: out<<"double"; break;
            case JavaPrimitiveType::Void: out<<"void"; break;
        }
    } else {
        if (referenceKind==JavaReferenceKind::Wildcard) out<<"?";
        else out<<qualifiedName;
        if (!arguments.empty()) {
            out<<"<";
            for (std::size_t i=0;i<arguments.size();++i) { if(i) out<<","; out<<arguments[i].normalized(); }
            out<<">";
        }
    }
    for(int i=0;i<arrayDimensions;++i) out<<"[]";
    return out.str();
}

static bool sameType(const JavaTypeDescriptor& a,const JavaTypeDescriptor& b){return a.normalized()==b.normalized();}
static bool numericPrimitive(JavaPrimitiveType p){return p==JavaPrimitiveType::Byte||p==JavaPrimitiveType::Short||p==JavaPrimitiveType::Char||p==JavaPrimitiveType::Int||p==JavaPrimitiveType::Long||p==JavaPrimitiveType::Float||p==JavaPrimitiveType::Double;}
static int numericRank(JavaPrimitiveType p){switch(p){case JavaPrimitiveType::Byte:return 1;case JavaPrimitiveType::Short:return 2;case JavaPrimitiveType::Char:return 2;case JavaPrimitiveType::Int:return 3;case JavaPrimitiveType::Long:return 4;case JavaPrimitiveType::Float:return 5;case JavaPrimitiveType::Double:return 6;default:return 0;}}
static JavaTypeDescriptor primitive(JavaPrimitiveType p){JavaTypeDescriptor t;t.primitive=true;t.primitiveType=p;return t;}
static JavaTypeDescriptor boxed(JavaPrimitiveType p){JavaTypeDescriptor t;t.primitive=false;t.referenceKind=JavaReferenceKind::Class;switch(p){case JavaPrimitiveType::Boolean:t.qualifiedName="java.lang.Boolean";break;case JavaPrimitiveType::Byte:t.qualifiedName="java.lang.Byte";break;case JavaPrimitiveType::Short:t.qualifiedName="java.lang.Short";break;case JavaPrimitiveType::Int:t.qualifiedName="java.lang.Integer";break;case JavaPrimitiveType::Long:t.qualifiedName="java.lang.Long";break;case JavaPrimitiveType::Char:t.qualifiedName="java.lang.Character";break;case JavaPrimitiveType::Float:t.qualifiedName="java.lang.Float";break;case JavaPrimitiveType::Double:t.qualifiedName="java.lang.Double";break;default:break;}return t;}
static bool unboxName(const std::string& n,JavaPrimitiveType& p){if(n=="java.lang.Boolean"){p=JavaPrimitiveType::Boolean;return true;}if(n=="java.lang.Byte"){p=JavaPrimitiveType::Byte;return true;}if(n=="java.lang.Short"){p=JavaPrimitiveType::Short;return true;}if(n=="java.lang.Integer"){p=JavaPrimitiveType::Int;return true;}if(n=="java.lang.Long"){p=JavaPrimitiveType::Long;return true;}if(n=="java.lang.Character"){p=JavaPrimitiveType::Char;return true;}if(n=="java.lang.Float"){p=JavaPrimitiveType::Float;return true;}if(n=="java.lang.Double"){p=JavaPrimitiveType::Double;return true;}return false;}
JavaConversionResult classifyJavaConversion(const JavaTypeDescriptor& s,const JavaTypeDescriptor& t,JavaConversionContext ctx){
 JavaConversionResult r;r.target=t;
 if(sameType(s,t)){r.kind=JavaConversionKind::Identity;r.permitted=true;r.reason="identity conversion";return r;}
 if(s.primitive&&t.primitive&&numericPrimitive(s.primitiveType)&&numericPrimitive(t.primitiveType)){
  int a=numericRank(s.primitiveType),b=numericRank(t.primitiveType);
  if(a<b){r.kind=JavaConversionKind::WideningPrimitive;r.permitted=true;r.reason="widening primitive conversion";}
  else if(ctx==JavaConversionContext::Casting){r.kind=JavaConversionKind::NarrowingPrimitive;r.permitted=true;r.reason="narrowing primitive conversion in cast context";}
  else r.reason="narrowing primitive conversion is not generally permitted in this context";
  return r;
 }
 if(s.primitive&&!t.primitive&&numericPrimitive(s.primitiveType)&&t.referenceKind==JavaReferenceKind::Class){
  if(sameType(boxed(s.primitiveType),t)){r.kind=JavaConversionKind::Boxing;r.permitted=ctx!=JavaConversionContext::Numeric;r.reason="boxing conversion";return r;}
  JavaTypeDescriptor b=boxed(s.primitiveType);
  if(t.qualifiedName=="java.lang.Object"){r.kind=JavaConversionKind::Boxing;r.permitted=ctx==JavaConversionContext::Assignment||ctx==JavaConversionContext::LooseInvocation;r.reason="boxing followed by widening reference conversion";return r;}
 }
 if(!s.primitive&&t.primitive){JavaPrimitiveType p; if(unboxName(s.qualifiedName,p)){if(p==t.primitiveType){r.kind=JavaConversionKind::Unboxing;r.permitted=ctx==JavaConversionContext::Assignment||ctx==JavaConversionContext::LooseInvocation||ctx==JavaConversionContext::Casting;r.reason="unboxing conversion";return r;}if(numericPrimitive(p)&&numericPrimitive(t.primitiveType)&&numericRank(p)<numericRank(t.primitiveType)){r.kind=JavaConversionKind::Unboxing;r.permitted=ctx==JavaConversionContext::Assignment||ctx==JavaConversionContext::LooseInvocation||ctx==JavaConversionContext::Numeric;r.reason="unboxing followed by widening primitive conversion";return r;}}}
 if(!s.primitive&&!t.primitive){
  if(t.qualifiedName=="java.lang.Object"&&s.referenceKind!=JavaReferenceKind::Null){r.kind=JavaConversionKind::WideningReference;r.permitted=true;r.reason="widening reference conversion";return r;}
  if(ctx==JavaConversionContext::Casting){r.kind=JavaConversionKind::NarrowingReference;r.permitted=true;r.compileTimeOnly=false;r.reason="narrowing reference conversion; runtime check may apply";return r;}
  if(ctx==JavaConversionContext::StringContext){r.kind=JavaConversionKind::String;r.permitted=true;r.reason="string conversion";return r;}
 }
 r.reason="no permitted conversion classified";
 return r;
}
JavaTypeDescriptor unaryNumericPromotion(const JavaTypeDescriptor& s){
 if(!s.primitive)return s;
 if(s.primitiveType==JavaPrimitiveType::Byte||s.primitiveType==JavaPrimitiveType::Short||s.primitiveType==JavaPrimitiveType::Char)return primitive(JavaPrimitiveType::Int);
 return s;
}
JavaTypeDescriptor binaryNumericPromotion(const JavaTypeDescriptor& l,const JavaTypeDescriptor& rr){
 JavaTypeDescriptor a=unaryNumericPromotion(l),b=unaryNumericPromotion(rr);
 if(a.primitive&&b.primitive){int ar=numericRank(a.primitiveType),br=numericRank(b.primitiveType);return primitive(ar>=br?a.primitiveType:b.primitiveType);}
 return a;
}

static bool applicablePhase(const JavaMethodCandidate& m,const std::vector<JavaTypeDescriptor>& a,JavaApplicabilityPhase p){
 if(m.method.parameterTypes.size()!=a.size() && !(m.isVarargs&&p==JavaApplicabilityPhase::VariableArity)) return false;
 std::size_t n=m.method.parameterTypes.size();
 for(std::size_t i=0;i<a.size();++i){
  std::size_t pi=(m.isVarargs&&i>=n-1)?n-1:i;if(pi>=n)return false;
  auto ctx=(p==JavaApplicabilityPhase::Strict)?JavaConversionContext::StrictInvocation:
           (p==JavaApplicabilityPhase::Loose)?JavaConversionContext::LooseInvocation:JavaConversionContext::LooseInvocation;
  if(!classifyJavaConversion(a[i],m.method.parameterTypes[pi],ctx).permitted)return false;
 }
 return true;
}
JavaMethodResolution resolveOverload(const std::vector<JavaMethodCandidate>& c,const std::vector<JavaTypeDescriptor>& a,JavaApplicabilityPhase p){
 JavaMethodResolution r;r.phase=p;
 for(std::size_t i=0;i<c.size();++i)if(applicablePhase(c[i],a,p))r.applicableIndices.push_back((int)i);
 if(r.applicableIndices.empty()){r.status=JavaResolutionStatus::NotApplicable;return r;}
 if(r.applicableIndices.size()==1){r.status=JavaResolutionStatus::Selected;r.selectedIndex=r.applicableIndices[0];return r;}
 int best=r.applicableIndices[0];bool tie=false;
 for(std::size_t k=1;k<r.applicableIndices.size();++k){int cur=r.applicableIndices[k];const auto& bm=c[best].method.parameterTypes;const auto& cm=c[cur].method.parameterTypes;if(bm.size()!=cm.size()){tie=true;continue;}bool curMore=true,bestMore=true;for(std::size_t i=0;i<bm.size();++i){curMore &= classifyJavaConversion(cm[i],bm[i],JavaConversionContext::StrictInvocation).permitted;bestMore &= classifyJavaConversion(bm[i],cm[i],JavaConversionContext::StrictInvocation).permitted;}if(curMore&&!bestMore){best=cur;tie=false;}else if(curMore==bestMore)tie=true;}
 if(tie){r.status=JavaResolutionStatus::Ambiguous;r.diagnostics.push_back("multiple applicable methods are not uniquely most specific");}else{r.status=JavaResolutionStatus::Selected;r.selectedIndex=best;}return r;
}
bool isOverrideEquivalent(const JavaDeclarationDescriptor& a,const JavaDeclarationDescriptor& b){
 return a.kind==JavaDeclarationKind::Method&&b.kind==JavaDeclarationKind::Method&&a.name==b.name&&a.parameterTypes.size()==b.parameterTypes.size()&&[&](){for(std::size_t i=0;i<a.parameterTypes.size();++i)if(a.parameterTypes[i].normalized()!=b.parameterTypes[i].normalized())return false;return true;}();
}
bool isOverrideCompatible(const JavaDeclarationDescriptor& base,const JavaDeclarationDescriptor& derived){
 if(!isOverrideEquivalent(base,derived))return false;
 if(base.returnType.empty()||derived.returnType.empty())return true;
 return base.returnType==derived.returnType;
}
const char* javaEquivalenceLayerName(JavaEquivalenceLayer x) {
    switch(x) {
        case JavaEquivalenceLayer::Lexical:return "Q1-Lexical";
        case JavaEquivalenceLayer::TypeSystem:return "Q2-TypeSystem";
        case JavaEquivalenceLayer::Declarations:return "Q3-Declarations";
        case JavaEquivalenceLayer::Expressions:return "Q4-Expressions";
        case JavaEquivalenceLayer::Statements:return "Q5-Statements";
        case JavaEquivalenceLayer::SemanticConstraints:return "Q6-SemanticConstraints";
        case JavaEquivalenceLayer::ApiCounterparts:return "Q7-ApiCounterparts";
        case JavaEquivalenceLayer::SourceEquivalenceTesting:return "Q8-SourceEquivalenceTesting";
    }
    return "Unknown";
}
const char* javaSymbolDomainName(JavaSymbolDomain x) {
    switch(x) {
        case JavaSymbolDomain::NativeSLeeLa:return "native-sleela";
        case JavaSymbolDomain::JavaCompatibility:return "java-compatibility";
        case JavaSymbolDomain::JavaApiCounterpart:return "java-api-counterpart";
    }
    return "unknown";
}
const char* javaDeclarationKindName(JavaDeclarationKind x) {
    switch(x) {
        case JavaDeclarationKind::Package:return "package";
        case JavaDeclarationKind::Import:return "import";
        case JavaDeclarationKind::Class:return "class";
        case JavaDeclarationKind::Interface:return "interface";
        case JavaDeclarationKind::Enum:return "enum";
        case JavaDeclarationKind::Record:return "record";
        case JavaDeclarationKind::AnnotationInterface:return "annotation-interface";
        case JavaDeclarationKind::Field:return "field";
        case JavaDeclarationKind::Method:return "method";
        case JavaDeclarationKind::Constructor:return "constructor";
        case JavaDeclarationKind::Parameter:return "parameter";
        case JavaDeclarationKind::TypeParameter:return "type-parameter";
        case JavaDeclarationKind::RecordComponent:return "record-component";
        case JavaDeclarationKind::Initializer:return "initializer";
        case JavaDeclarationKind::NestedType:return "nested-type";
        case JavaDeclarationKind::LocalType:return "local-type";
        case JavaDeclarationKind::AnonymousType:return "anonymous-type";
    }
    return "unknown";
}
const char* javaExpressionKindName(JavaExpressionKind x) {
    switch(x) {
        case JavaExpressionKind::Literal:return "literal";
        case JavaExpressionKind::Name:return "name";
        case JavaExpressionKind::This:return "this";
        case JavaExpressionKind::Super:return "super";
        case JavaExpressionKind::MemberAccess:return "member-access";
        case JavaExpressionKind::MethodInvocation:return "method-invocation";
        case JavaExpressionKind::ConstructorInvocation:return "constructor-invocation";
        case JavaExpressionKind::ArrayCreation:return "array-creation";
        case JavaExpressionKind::ArrayAccess:return "array-access";
        case JavaExpressionKind::Assignment:return "assignment";
        case JavaExpressionKind::CompoundAssignment:return "compound-assignment";
        case JavaExpressionKind::Unary:return "unary";
        case JavaExpressionKind::Binary:return "binary";
        case JavaExpressionKind::Conditional:return "conditional";
        case JavaExpressionKind::Cast:return "cast";
        case JavaExpressionKind::InstanceOf:return "instanceof";
        case JavaExpressionKind::Lambda:return "lambda";
        case JavaExpressionKind::MethodReference:return "method-reference";
        case JavaExpressionKind::ClassLiteral:return "class-literal";
        case JavaExpressionKind::SwitchExpression:return "switch-expression";
        case JavaExpressionKind::Pattern:return "pattern";
        case JavaExpressionKind::Parenthesized:return "parenthesized";
    }
    return "unknown";
}
const char* javaStatementKindName(JavaStatementKind x) {
    switch(x) {
        case JavaStatementKind::Empty:return "empty";
        case JavaStatementKind::Block:return "block";
        case JavaStatementKind::LocalDeclaration:return "local-declaration";
        case JavaStatementKind::Expression:return "expression";
        case JavaStatementKind::If:return "if";
        case JavaStatementKind::Switch:return "switch";
        case JavaStatementKind::While:return "while";
        case JavaStatementKind::Do:return "do";
        case JavaStatementKind::For:return "for";
        case JavaStatementKind::EnhancedFor:return "enhanced-for";
        case JavaStatementKind::Break:return "break";
        case JavaStatementKind::Continue:return "continue";
        case JavaStatementKind::Return:return "return";
        case JavaStatementKind::Throw:return "throw";
        case JavaStatementKind::Assert:return "assert";
        case JavaStatementKind::Synchronized:return "synchronized";
        case JavaStatementKind::Try:return "try";
        case JavaStatementKind::Catch:return "catch";
        case JavaStatementKind::Finally:return "finally";
        case JavaStatementKind::TryWithResources:return "try-with-resources";
        case JavaStatementKind::Yield:return "yield";
        case JavaStatementKind::Labeled:return "labeled";
        case JavaStatementKind::ExplicitConstructorInvocation:return "explicit-constructor-invocation";
    }
    return "unknown";
}
const char* javaSemanticRuleName(JavaSemanticRuleKind x) {
    switch(x) {
        case JavaSemanticRuleKind::NameResolution:return "name-resolution";
        case JavaSemanticRuleKind::Scope:return "scope";
        case JavaSemanticRuleKind::AccessControl:return "access-control";
        case JavaSemanticRuleKind::TypeChecking:return "type-checking";
        case JavaSemanticRuleKind::Conversion:return "conversion";
        case JavaSemanticRuleKind::NumericPromotion:return "numeric-promotion";
        case JavaSemanticRuleKind::Boxing:return "boxing";
        case JavaSemanticRuleKind::Unboxing:return "unboxing";
        case JavaSemanticRuleKind::GenericInference:return "generic-inference";
        case JavaSemanticRuleKind::CaptureConversion:return "capture-conversion";
        case JavaSemanticRuleKind::OverloadResolution:return "overload-resolution";
        case JavaSemanticRuleKind::OverrideResolution:return "override-resolution";
        case JavaSemanticRuleKind::ConstructorInvocation:return "constructor-invocation";
        case JavaSemanticRuleKind::DefiniteAssignment:return "definite-assignment";
        case JavaSemanticRuleKind::Reachability:return "reachability";
        case JavaSemanticRuleKind::ExceptionChecking:return "exception-checking";
        case JavaSemanticRuleKind::Initialization:return "initialization";
        case JavaSemanticRuleKind::SealedHierarchy:return "sealed-hierarchy";
        case JavaSemanticRuleKind::RecordConstraints:return "record-constraints";
        case JavaSemanticRuleKind::EnumConstraints:return "enum-constraints";
        case JavaSemanticRuleKind::AnnotationConstraints:return "annotation-constraints";
    }
    return "unknown";
}

JavaGenericInferenceResult inferGenericMethodTypes(const JavaMethodCandidate& c,const std::vector<JavaTypeDescriptor>& args){
 JavaGenericInferenceResult r;
 if(c.method.typeParameters.empty()){r.success=true;return r;}
 if(c.method.parameterTypes.size()!=args.size() && !c.isVarargs){r.diagnostics.push_back("arity does not permit generic inference");return r;}
 for(std::size_t i=0;i<args.size() && i<c.method.parameterTypes.size();++i){
  const auto& p=c.method.parameterTypes[i];
  if(!p.primitive && p.referenceKind==JavaReferenceKind::TypeVariable && !p.qualifiedName.empty()){
   auto it=r.inferred.find(p.qualifiedName);
   if(it==r.inferred.end()) r.inferred[p.qualifiedName]=args[i];
   else if(it->second.normalized()!=args[i].normalized()) r.diagnostics.push_back("conflicting inferred type for "+p.qualifiedName);
  }
 }
 r.success=r.diagnostics.empty();
 if(r.success && r.inferred.size()<c.method.typeParameters.size()) r.diagnostics.push_back("insufficient constraints for all method type parameters");
 r.success=r.diagnostics.empty();
 return r;
}
JavaPertinenceResult assessArgumentPertinence(const JavaExpressionDescriptor& e,const JavaTypeDescriptor& formal){
 JavaPertinenceResult r;r.potentiallyCompatible=true;
 if(e.kind==JavaExpressionKind::Lambda || e.kind==JavaExpressionKind::MethodReference){
  r.pertinent=false;
  r.reason="implicitly typed lambda or inexact method reference is not pertinent until target typing";
 }
 if(formal.referenceKind==JavaReferenceKind::TypeVariable) r.potentiallyCompatible=true;
 return r;
}
static bool refSubtype(const JavaTypeDescriptor& a,const JavaTypeDescriptor& b){
 if(a.normalized()==b.normalized()) return true;
 if(!a.primitive&&!b.primitive&&b.qualifiedName=="java.lang.Object") return true;
 return false;
}
JavaMostSpecificResult compareMostSpecific(const JavaMethodCandidate& a,const JavaMethodCandidate& b,const std::vector<JavaTypeDescriptor>&){
 JavaMostSpecificResult r;
 if(a.method.parameterTypes.size()!=b.method.parameterTypes.size()){r.ambiguous=true;r.reason="different arity cannot establish specificity";return r;}
 bool ab=true,ba=true;
 for(std::size_t i=0;i<a.method.parameterTypes.size();++i){
  ab &= refSubtype(a.method.parameterTypes[i],b.method.parameterTypes[i]) ||
        classifyJavaConversion(a.method.parameterTypes[i],b.method.parameterTypes[i],JavaConversionContext::StrictInvocation).permitted;
  ba &= refSubtype(b.method.parameterTypes[i],a.method.parameterTypes[i]) ||
        classifyJavaConversion(b.method.parameterTypes[i],a.method.parameterTypes[i],JavaConversionContext::StrictInvocation).permitted;
 }
 r.firstMoreSpecific=ab&&!ba;r.secondMoreSpecific=ba&&!ab;r.ambiguous=!r.firstMoreSpecific&&!r.secondMoreSpecific;
 r.reason=r.ambiguous?"neither candidate is uniquely more specific":"unique most-specific relation established";
 return r;
}
bool checkedExceptionsCompatible(const JavaDeclarationDescriptor& base,const JavaDeclarationDescriptor& derived){
 for(const auto& d:derived.thrownTypes){
  bool covered=false;
  for(const auto& b:base.thrownTypes) if(d==b){covered=true;break;}
  if(!covered) return false;
 }
 return true;
}
bool isOverrideCompatible(const JavaDeclarationDescriptor& base,const JavaDeclarationDescriptor& derived,JavaAccessLevel baseAccess,JavaAccessLevel derivedAccess,bool baseStatic,bool derivedStatic,bool baseFinal,bool basePrivate){
 if(!isOverrideEquivalent(base,derived)) return false;
 if(baseStatic || derivedStatic || baseFinal || basePrivate) return false;
 if(static_cast<int>(derivedAccess)>static_cast<int>(baseAccess)) return false;
 if(!checkedExceptionsCompatible(base,derived)) return false;
 if(base.returnType.empty()||derived.returnType.empty()) return true;
 if(base.returnType==derived.returnType) return true;
 JavaTypeDescriptor b; b.qualifiedName=base.returnType; b.referenceKind=JavaReferenceKind::Class;
 JavaTypeDescriptor d; d.qualifiedName=derived.returnType; d.referenceKind=JavaReferenceKind::Class;
 return refSubtype(d,b);
}
bool variableArityApplicable(const JavaMethodCandidate& c,const std::vector<JavaTypeDescriptor>& args){
 if(!c.isVarargs||c.method.parameterTypes.empty()) return false;
 const std::size_t fixed=c.method.parameterTypes.size()-1;
 if(args.size()<fixed) return false;
 for(std::size_t i=0;i<fixed;++i)
  if(!classifyJavaConversion(args[i],c.method.parameterTypes[i],JavaConversionContext::LooseInvocation).permitted) return false;
 return true;
}


[[maybe_unused]] static std::string eraseTypeVariables(const JavaTypeDescriptor& t){
 if(t.referenceKind==JavaReferenceKind::TypeVariable) return "java.lang.Object";
 if(t.referenceKind==JavaReferenceKind::Parameterized){
  JavaTypeDescriptor e=t;e.arguments.clear();e.referenceKind=JavaReferenceKind::Class;return e.normalized();
 }
 return t.normalized();
}
JavaMostSpecificSetResult chooseMostSpecific(const std::vector<JavaMethodCandidate>& c,const std::vector<int>& applicable,const std::vector<JavaTypeDescriptor>& args){
 JavaMostSpecificSetResult r;
 for(int i:applicable){
  bool maximal=true;
  for(int j:applicable) if(i!=j){
   auto cmp=compareMostSpecific(c[j],c[i],args);
   if(cmp.firstMoreSpecific){maximal=false;break;}
  }
  if(maximal) r.maximallySpecific.push_back(i);
 }
 if(r.maximallySpecific.size()==1){r.selectedIndex=r.maximallySpecific[0];return r;}
 if(r.maximallySpecific.empty()){r.ambiguous=true;r.diagnostics.push_back("no maximally specific method");return r;}
 bool sameSig=true; for(std::size_t k=1;k<r.maximallySpecific.size();++k){
  if(!isOverrideEquivalent(c[r.maximallySpecific[0]].method,c[r.maximallySpecific[k]].method)){sameSig=false;break;}
 }
 if(sameSig){
  int concrete=-1;
  for(int i:r.maximallySpecific) if(!c[i].isAbstract && !c[i].isDefault){if(concrete!=-1){concrete=-2;break;}concrete=i;}
  if(concrete>=0){r.selectedIndex=concrete;return r;}
  int preferred=-1;
  for(int i:r.maximallySpecific){
   bool preferredHere=true;
   for(int j:r.maximallySpecific) if(i!=j && !c[i].method.returnType.empty() && c[i].method.returnType!=c[j].method.returnType){
    JavaTypeDescriptor a;a.qualifiedName=c[i].method.returnType;
    JavaTypeDescriptor b;b.qualifiedName=c[j].method.returnType;
    if(!refSubtype(a,b)) preferredHere=false;
   }
   if(preferredHere){if(preferred!=-1){preferred=-2;break;}preferred=i;}
  }
  if(preferred>=0){r.selectedIndex=preferred;return r;}
 }
 r.ambiguous=true;r.diagnostics.push_back("multiple maximally specific methods remain");
 return r;
}
JavaInvocationTypeResult inferInvocationType(const JavaMethodCandidate& c,const std::vector<JavaTypeDescriptor>& args,const JavaTypeDescriptor* target){
 JavaInvocationTypeResult r;
 auto inf=inferGenericMethodTypes(c,args); r.inferred=inf.inferred;
 if(!inf.success){r.diagnostics=inf.diagnostics;return r;}
 r.invocationType=c.method;
 auto substitute=[&](const std::string& n){
  auto it=r.inferred.find(n);return it==r.inferred.end()?std::string():it->second.normalized();
 };
 for(auto& p:r.invocationType.parameterTypes) if(p.referenceKind==JavaReferenceKind::TypeVariable){auto x=substitute(p.qualifiedName);if(!x.empty()){p.qualifiedName=x;p.referenceKind=JavaReferenceKind::Class;}}
 if(r.invocationType.returnType.size() && r.inferred.count(r.invocationType.returnType)) r.invocationType.returnType=r.inferred[r.invocationType.returnType].normalized();
 if(target && !r.invocationType.returnType.empty()){
  JavaTypeDescriptor actual;actual.qualifiedName=r.invocationType.returnType;
  auto conv=classifyJavaConversion(actual,*target,JavaConversionContext::Assignment);
  if(!conv.permitted){r.diagnostics.push_back("inferred invocation type is incompatible with target type");return r;}
 }
 r.success=true;return r;
}
JavaInterfaceInheritanceResult resolveInterfaceDefaults(const std::vector<JavaMethodCandidate>& methods){
 JavaInterfaceInheritanceResult r;
 for(std::size_t i=0;i<methods.size();++i){
  bool shadowed=false;
  for(std::size_t j=0;j<methods.size();++j) if(i!=j && methods[j].isInterfaceMethod && methods[j].method.owner!=methods[i].method.owner){
   if(isOverrideEquivalent(methods[j].method,methods[i].method) && methods[j].isDefault && !methods[i].isDefault){shadowed=true;break;}
  }
  if(!shadowed) r.inherited.push_back((int)i);
 }
 for(std::size_t a=0;a<r.inherited.size();++a) for(std::size_t b=a+1;b<r.inherited.size();++b){
  int i=r.inherited[a],j=r.inherited[b];
  if(methods[i].isDefault&&methods[j].isDefault&&isOverrideEquivalent(methods[i].method,methods[j].method)){
   r.conflicts.push_back(i);r.conflicts.push_back(j);
  }
 }
 if(!r.conflicts.empty()) r.diagnostics.push_back("conflicting inherited default methods require explicit resolution");
 return r;
}

} // namespace sleela
