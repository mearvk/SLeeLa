// ===========================================================================
// java_equivalence.h -- Source-level Java -> SLeeLa equivalence model.
// ===========================================================================
// This model describes the eight source/API equivalence layers. It is not a
// JVM, bytecode, or SLVM interface. Java compatibility symbols remain distinct
// from native SLeeLa symbols.
#ifndef SLEELA_JAVA_EQUIVALENCE_H
#define SLEELA_JAVA_EQUIVALENCE_H

#include <string>
#include <vector>
#include <map>

namespace sleela {

enum class JavaEquivalenceLayer {
    Lexical = 1, TypeSystem = 2, Declarations = 3, Expressions = 4,
    Statements = 5, SemanticConstraints = 6, ApiCounterparts = 7,
    SourceEquivalenceTesting = 8
};

enum class JavaSymbolDomain { NativeSLeeLa, JavaCompatibility, JavaApiCounterpart };
enum class JavaPrimitiveType { Boolean, Byte, Short, Int, Long, Char, Float, Double, Void };
enum class JavaReferenceKind { Class, Interface, Array, TypeVariable, Parameterized, Wildcard, Null };



struct JavaTypeDescriptor {
    bool primitive = false;
    JavaPrimitiveType primitiveType = JavaPrimitiveType::Void;
    JavaReferenceKind referenceKind = JavaReferenceKind::Class;
    std::string qualifiedName;
    std::vector<JavaTypeDescriptor> arguments;
    std::vector<std::string> bounds;
    int arrayDimensions = 0;
    std::string normalized() const;
};

enum class JavaConversionKind {
    Identity, WideningPrimitive, NarrowingPrimitive, WideningAndNarrowingPrimitive,
    WideningReference, NarrowingReference, Boxing, Unboxing, Unchecked,
    Capture, String, ValueSet, Forbidden
};
enum class JavaConversionContext {
    Assignment, StrictInvocation, LooseInvocation, StringContext, Casting,
    Numeric, Testing
};
struct JavaConversionResult {
    JavaConversionKind kind = JavaConversionKind::Forbidden;
    JavaTypeDescriptor target;
    bool permitted = false;
    bool compileTimeOnly = true;
    std::string reason;
};
JavaConversionResult classifyJavaConversion(const JavaTypeDescriptor& source,
                                           const JavaTypeDescriptor& target,
                                           JavaConversionContext context);
JavaTypeDescriptor unaryNumericPromotion(const JavaTypeDescriptor& source);
JavaTypeDescriptor binaryNumericPromotion(const JavaTypeDescriptor& left,
                                          const JavaTypeDescriptor& right);

enum class JavaDeclarationKind {
    Package, Import, Class, Interface, Enum, Record, AnnotationInterface,
    Field, Method, Constructor, Parameter, TypeParameter, RecordComponent,
    Initializer, NestedType, LocalType, AnonymousType
};

struct JavaDeclarationDescriptor {
    JavaDeclarationKind kind = JavaDeclarationKind::Class;
    std::string qualifiedName;
    std::string owner;
    std::string name;
    std::string returnType;
    std::vector<JavaTypeDescriptor> parameterTypes;
    std::vector<std::string> typeParameters;
    std::vector<std::string> thrownTypes;
    unsigned modifiers = 0;
    std::vector<std::string> annotations;
};

enum class JavaExpressionKind {
    Literal, Name, This, Super, MemberAccess, MethodInvocation,
    ConstructorInvocation, ArrayCreation, ArrayAccess, Assignment,
    CompoundAssignment, Unary, Binary, Conditional, Cast, InstanceOf,
    Lambda, MethodReference, ClassLiteral, SwitchExpression, Pattern,
    Parenthesized
};

struct JavaExpressionDescriptor {
    JavaExpressionKind kind = JavaExpressionKind::Name;
    std::string operatorText;
    std::string typeText;
    std::vector<JavaExpressionDescriptor> children;
};

enum class JavaStatementKind {
    Empty, Block, LocalDeclaration, Expression, If, Switch, While, Do,
    For, EnhancedFor, Break, Continue, Return, Throw, Assert,
    Synchronized, Try, Catch, Finally, TryWithResources, Yield,
    Labeled, ExplicitConstructorInvocation
};

struct JavaStatementDescriptor {
    JavaStatementKind kind = JavaStatementKind::Empty;
    std::vector<JavaStatementDescriptor> children;
    std::vector<std::string> thrownTypes;
};

enum class JavaSemanticRuleKind {
    NameResolution, Scope, AccessControl, TypeChecking, Conversion,
    NumericPromotion, Boxing, Unboxing, GenericInference, CaptureConversion,
    OverloadResolution, OverrideResolution, ConstructorInvocation,
    DefiniteAssignment, Reachability, ExceptionChecking, Initialization,
    SealedHierarchy, RecordConstraints, EnumConstraints, AnnotationConstraints
};

enum class JavaApplicabilityPhase { Strict, Loose, VariableArity };
enum class JavaResolutionStatus { NotApplicable, Applicable, Ambiguous, Selected };

struct JavaMethodCandidate {
    JavaDeclarationDescriptor method;
    bool isVarargs = false;
    bool isStatic = false;
    bool isGeneric = false;
    std::string declaringType;
};

struct JavaMethodResolution {
    JavaResolutionStatus status = JavaResolutionStatus::NotApplicable;
    JavaApplicabilityPhase phase = JavaApplicabilityPhase::Strict;
    int selectedIndex = -1;
    std::vector<int> applicableIndices;
    std::vector<std::string> diagnostics;
};

JavaMethodResolution resolveOverload(const std::vector<JavaMethodCandidate>& candidates,
                                     const std::vector<JavaTypeDescriptor>& argumentTypes,
                                     JavaApplicabilityPhase phase);
bool isOverrideEquivalent(const JavaDeclarationDescriptor& base,
                          const JavaDeclarationDescriptor& derived);
bool isOverrideCompatible(const JavaDeclarationDescriptor& base,
                          const JavaDeclarationDescriptor& derived);

struct JavaApiMemberDescriptor {
    std::string owner;
    std::string qualifiedName;
    std::string memberName;
    std::string descriptor;
    std::string returnType;
    std::vector<std::string> parameterTypes;
    std::vector<std::string> thrownTypes;
    std::vector<std::string> annotations;
};

struct JavaEquivalenceDiagnostic {
    std::string layer;
    std::string code;
    std::string sourceLocation;
    std::string message;
    bool error = false;
};

struct JavaEquivalenceInventory {
    std::vector<JavaDeclarationDescriptor> declarations;
    std::vector<JavaExpressionDescriptor> expressions;
    std::vector<JavaStatementDescriptor> statements;
    std::vector<JavaApiMemberDescriptor> apiMembers;
    std::vector<JavaEquivalenceDiagnostic> diagnostics;
    std::map<JavaSymbolDomain, std::size_t> symbolCounts;
};

const char* javaEquivalenceLayerName(JavaEquivalenceLayer layer);
const char* javaSymbolDomainName(JavaSymbolDomain domain);
const char* javaDeclarationKindName(JavaDeclarationKind kind);
const char* javaExpressionKindName(JavaExpressionKind kind);
const char* javaStatementKindName(JavaStatementKind kind);
const char* javaSemanticRuleName(JavaSemanticRuleKind kind);

} // namespace sleela
#endif
