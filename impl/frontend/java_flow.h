// ===========================================================================
// java_flow.h -- Java source-level definite-assignment/reachability model.
// ===========================================================================
// Source-semantic analysis only. No JVM/SLVM runtime is implied.
#ifndef SLEELA_JAVA_FLOW_H
#define SLEELA_JAVA_FLOW_H

#include <map>
#include <set>
#include <string>
#include <vector>

namespace sleela {

enum class JavaFlowDiagnosticKind {
    UseBeforeAssignment,
    FinalReassignment,
    UnreachableStatement,
    InvalidBreak,
    InvalidContinue
};

struct JavaFlowDiagnostic {
    JavaFlowDiagnosticKind kind = JavaFlowDiagnosticKind::UseBeforeAssignment;
    std::string variable;
    std::string message;
    bool error = true;
};

struct JavaFlowFacts {
    std::set<std::string> definitelyAssigned;
    std::set<std::string> definitelyUnassigned;
    bool reachable = true;
};

enum class JavaFlowExprKind {
    Literal, Variable, Assignment, CompoundAssignment, Unary,
    Binary, Conditional, Other
};

struct JavaFlowExpr {
    JavaFlowExprKind kind = JavaFlowExprKind::Literal;
    std::string name;
    std::string op;
    std::vector<JavaFlowExpr> children;
};

enum class JavaFlowStmtKind {
    Empty, Block, LocalDeclaration, Expression, Assignment,
    If, While, Do, For, Switch, Break, Continue, Return, Throw,
    Assert, Try, Synchronized, Yield, Labeled
};

struct JavaFlowStmt {
    JavaFlowStmtKind kind = JavaFlowStmtKind::Empty;
    std::string variable;
    bool finalVariable = false;
    bool hasInitializer = false;
    bool conditionConstant = false;
    bool conditionValue = false;
    JavaFlowExpr expression;
    JavaFlowExpr condition;
    std::vector<JavaFlowStmt> children;
    std::vector<JavaFlowStmt> elseChildren;
    std::vector<std::vector<JavaFlowStmt>> switchCases;
    std::vector<std::vector<JavaFlowStmt>> catchBlocks;
    std::vector<JavaFlowStmt> finallyBlock;
    std::string label;
};

struct JavaFlowResult {
    JavaFlowFacts after;
    std::vector<JavaFlowDiagnostic> diagnostics;
};

JavaFlowResult analyzeJavaFlow(
    const std::vector<JavaFlowStmt>& statements,
    const std::set<std::string>& initiallyAssigned = {},
    const std::set<std::string>& initiallyUnassigned = {});

const char* javaFlowDiagnosticName(JavaFlowDiagnosticKind kind);

} // namespace sleela
#endif
