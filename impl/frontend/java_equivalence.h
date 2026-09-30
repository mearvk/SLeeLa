#ifndef SLEELA_JAVA_EQUIVALENCE_H
#define SLEELA_JAVA_EQUIVALENCE_H
#include <string>
#include <vector>
#include <map>
namespace sleela {
enum class JavaEquivalenceLayer { Lexical=1, TypeSystem=2, Declarations=3, Expressions=4, Statements=5, SemanticConstraints=6, ApiCounterparts=7, SourceEquivalenceTesting=8 };
enum class JavaSymbolDomain { NativeSLeeLa, JavaCompatibility, JavaApiCounterpart };
enum class JavaPrimitiveType { Boolean, Byte, Short, Int, Long, Char, Float, Double, Void };
enum class JavaReferenceKind { Class, Interface, Array, TypeVariable, Parameterized, Wildcard, Null };
struct JavaTypeDescriptor { bool primitive=false; JavaPrimitiveType primitiveType=JavaPrimitiveType::Void; JavaReferenceKind referenceKind=JavaReferenceKind::Class; std::string qualifiedName; std::vector<JavaTypeDescriptor> arguments; std::vector<std::string> bounds; int arrayDimensions=0; std::string normalized() const; };
enum class JavaDeclarationKind { Package, Import, Class, Interface, Enum, Record, AnnotationInterface, Field, Method, Constructor, Parameter, TypeParameter, RecordComponent, Initializer, NestedType, LocalType, AnonymousType };
struct JavaDeclarationDescriptor { JavaDeclarationKind kind=JavaDeclarationKind::Class; std::string qualifiedName,owner,name,returnType; std::vector<JavaTypeDescriptor> parameterTypes; std::vector<std::string> typeParameters,thrownTypes,annotations; unsigned modifiers=0; };
enum class JavaExpressionKind { Literal, Name, This, Super, MemberAccess, MethodInvocation, ConstructorInvocation, ArrayCreation, ArrayAccess, Assignment, CompoundAssignment, Unary, Binary, Conditional, Cast, InstanceOf, Lambda, MethodReference, ClassLiteral, SwitchExpression, Pattern, Parenthesized };
struct JavaExpressionDescriptor { JavaExpressionKind kind=JavaExpressionKind::Name; std::string operatorText,typeText; std::vector<JavaExpressionDescriptor> children; };
enum class JavaStatementKind { Empty, Block, LocalDeclaration, Expression, If, Switch, While, Do, For, EnhancedFor, Break, Continue, Return, Throw, Assert, Synchronized, Try, Catch, Finally, TryWithResources, Yield, Labeled, ExplicitConstructorInvocation };
struct JavaStatementDescriptor { JavaStatementKind kind=JavaStatementKind::Empty; std::vector<JavaStatementDescriptor> children; std::vector<std::string> thrownTypes; };
enum class JavaSemanticRuleKind { NameResolution, Scope, AccessControl, TypeChecking, Conversion, NumericPromotion, Boxing, Unboxing, GenericInference, CaptureConversion, OverloadResolution, OverrideResolution, ConstructorInvocation, DefiniteAssignment, Reachability, ExceptionChecking, Initialization, SealedHierarchy, RecordConstraints, EnumConstraints, AnnotationConstraints };
struct JavaApiMemberDescriptor { std::string owner,qualifiedName,memberName,descriptor,returnType; std::vector<std::string> parameterTypes,thrownTypes,annotations; };
struct JavaEquivalenceDiagnostic { std::string layer,code,sourceLocation,message; bool error=false; };
struct JavaEquivalenceInventory { std::vector<JavaDeclarationDescriptor> declarations; std::vector<JavaExpressionDescriptor> expressions; std::vector<JavaStatementDescriptor> statements; std::vector<JavaApiMemberDescriptor> apiMembers; std::vector<JavaEquivalenceDiagnostic> diagnostics; std::map<JavaSymbolDomain,std::size_t> symbolCounts; };
const char* javaEquivalenceLayerName(JavaEquivalenceLayer);
const char* javaSymbolDomainName(JavaSymbolDomain);
const char* javaDeclarationKindName(JavaDeclarationKind);
const char* javaExpressionKindName(JavaExpressionKind);
const char* javaStatementKindName(JavaStatementKind);
const char* javaSemanticRuleName(JavaSemanticRuleKind);
}
#endif
