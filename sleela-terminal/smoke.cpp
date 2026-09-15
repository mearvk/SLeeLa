// SleelaTerminal(TM) shell -- layered smoke test.
// Original SLeeLa work (see NOTICE). Exercises L1..L5.

#include "core.hpp"
#include "arith.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "expand.hpp"
#include "executor.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace sleela::sh;

static long A(const std::string& e) {
    ArithResult r = evalArith(e);
    assert(r.ok);
    return r.value;
}

int main() {
    // --- L1: Value ---------------------------------------------------------
    assert(Value("42").asInt() == 42);
    assert(Value("-7").asInt() == -7);
    assert(Value("abc").asInt() == 0);
    assert(Value::fromInt(99).text == "99");

    // --- L1: arithmetic engine (precedence, assoc, comparisons, div0) ------
    assert(A("1 + 2 * 3") == 7);           // precedence
    assert(A("(1 + 2) * 3") == 9);         // parens
    assert(A("10 - 3 - 2") == 5);          // left assoc
    assert(A("2 * 3 + 4 * 5") == 26);
    assert(A("7 % 3") == 1);
    assert(A("-3 + 5") == 2);              // unary minus
    assert(A("!0") == 1 && A("!5") == 0);  // logical not
    assert(A("2 < 3") == 1 && A("3 < 2") == 0);
    assert(A("4 == 4") == 1 && A("4 != 4") == 0);
    assert(A("1 && 0") == 0 && A("1 || 0") == 1);
    assert(A("2 <= 2 && 3 >= 1") == 1);
    {
        ArithResult r = evalArith("5 / 0");
        assert(!r.ok && r.error.find("zero") != std::string::npos);
    }
    // variable lookup
    {
        VarLookup lk = [](const std::string& n) -> long { return n == "x" ? 6 : 0; };
        assert(evalArith("x * x + 1", lk).value == 37);
    }

    // --- L2: lexer ---------------------------------------------------------
    {
        std::vector<Token> t; LexError e;
        assert(lex("echo hi | tr a-z A-Z > out.txt", t, e));
        assert(t[0].kind == Tok::Word && t[0].text == "echo");
        bool hasPipe = false, hasGreat = false;
        for (auto& tk : t) { if (tk.kind == Tok::Pipe) hasPipe = true; if (tk.kind == Tok::Great) hasGreat = true; }
        assert(hasPipe && hasGreat);
    }
    {
        std::vector<Token> t; LexError e;
        assert(lex("x=3", t, e));
        assert(t[0].kind == Tok::Assignment && t[0].text == "x=3");
    }
    {
        std::vector<Token> t; LexError e;
        assert(lex("echo 'a b'  \"c d\"", t, e));
        // quoted spans become single words
        assert(t[0].text == "echo" && t[1].text == "a b" && t[2].text == "c d");
    }
    {
        std::vector<Token> t; LexError e;
        assert(lex(">>", t, e) && t[0].kind == Tok::DGreat);
        assert(lex("a && b || c", t, e));
    }

    // --- L3: parser --------------------------------------------------------
    auto parseOK = [](const std::string& s) -> NodePtr {
        std::vector<Token> t; LexError le;
        bool ok = lex(s, t, le); assert(ok);
        ParseError pe; NodePtr n = parse(t, pe);
        assert(n != nullptr);
        return n;
    };
    { auto n = parseOK("echo hello"); assert(n->kind == NodeKind::List); }
    { auto n = parseOK("a | b | c");
      assert(n->children[0]->kind == NodeKind::Pipeline);
      assert(n->children[0]->children.size() == 3); }
    { auto n = parseOK("true && echo yes");
      assert(n->children[0]->kind == NodeKind::AndOr); }
    { auto n = parseOK("if true; then echo t; else echo f; fi");
      assert(n->children[0]->kind == NodeKind::If); }
    { auto n = parseOK("while false; do echo x; done");
      assert(n->children[0]->kind == NodeKind::While); }
    { auto n = parseOK("cat < in.txt > out.txt");
      assert(n->children[0]->redirs.size() == 2); }
    // parse error is reported, not crashed
    { std::vector<Token> t; LexError le; lex("if true then", t, le);
      ParseError pe; NodePtr n = parse(t, pe);
      assert(n == nullptr && !pe.message.empty()); }

    // --- L4: expansion -----------------------------------------------------
    {
        Environment env;
        CommandRunner none = nullptr;
        auto single = [&](const std::string& w) { return expandWordSingle(w, env, none); };
        env.set("name", "world");
        assert(single("hello $name") == "hello world");
        assert(single("${name}!") == "world!");
        env.set("x", "6");
        assert(single("$(( x * x + 1 ))") == "37");
        env.setLastStatus(3);
        assert(single("status=$?") == "status=3");
        assert(single("no $undefined here") == "no  here");

        // multi-field: field splitting from an expansion
        env.set("list", "a b c");
        auto fields = expandWord("$list", env, none);
        assert(fields.size() == 3 && fields[0] == "a" && fields[2] == "c");

        // positional parameters
        env.setPositionals({"one", "two", "three"});
        assert(single("$1-$2-$3") == "one-two-three");
        assert(single("count=$#") == "count=3");
    }

    // --- L4: glob matching -------------------------------------------------
    assert(globMatch("*.txt", "notes.txt"));
    assert(!globMatch("*.txt", "notes.md"));
    assert(globMatch("a?c", "abc") && !globMatch("a?c", "ac"));
    assert(globMatch("[a-c]x", "bx") && !globMatch("[a-c]x", "dx"));
    assert(globMatch("[!0-9]", "z") && !globMatch("[!0-9]", "5"));
    assert(globMatch("f*", "foobar"));

    // --- L3/L2: parse M2 constructs ----------------------------------------
    { auto n = parseOK("for i in a b c; do echo $i; done");
      assert(n->children[0]->kind == NodeKind::For);
      assert(n->children[0]->for_var == "i");
      assert(n->children[0]->for_words.size() == 3); }
    { auto n = parseOK("case $x in a) echo A;; b|c) echo BC;; *) echo other;; esac");
      assert(n->children[0]->kind == NodeKind::Case);
      assert(n->children[0]->case_items.size() == 3); }
    { auto n = parseOK("greet() { echo hi; }");
      assert(n->children[0]->kind == NodeKind::FunctionDef);
      assert(n->children[0]->func_name == "greet"); }

    // --- L5: executor (end to end) -----------------------------------------
    auto run = [](const std::string& s, Environment& env) -> int {
        std::vector<Token> t; LexError le; bool ok = lex(s, t, le); assert(ok);
        ParseError pe; NodePtr n = parse(t, pe); assert(n);
        return execute(*n, env);
    };
    {
        Environment env;
        assert(run("true", env) == 0);
        assert(run("false", env) == 1);
        assert(run("true && false", env) == 1);
        assert(run("false || true", env) == 0);
        assert(run("false && echo nope", env) == 1);  // short-circuit
        // assignment + arithmetic + $?
        run("x=4", env);
        assert(env.get("x") == "4");
        run("y=$(( x * 5 ))", env);
        assert(env.get("y") == "20");
        // if uses exit status
        assert(run("if true; then :; else false; fi", env) == 0);
        // while with a guard-terminating condition
        run("n=0", env);
        // (condition false immediately -> body never runs)
        assert(run("while false; do echo loop; done", env) == 0);
    }

    // --- L5: M2 execution (for / case / functions / command subst) ---------
    {
        Environment env;
        // for-loop leaves the last iterated value in the loop var
        run("for i in a b c; do last=$i; done", env);
        assert(env.get("last") == "c");

        // for-loop accumulation via arithmetic
        run("sum=0", env);
        run("for k in 1 2 3 4; do sum=$(( sum + k )); done", env);
        assert(env.get("sum") == "10");

        // case: matching arm runs, sets a variable
        run("x=b", env);
        run("case $x in a) r=A;; b|c) r=BC;; *) r=other;; esac", env);
        assert(env.get("r") == "BC");
        run("x=zzz", env);
        run("case $x in a) r=A;; *) r=other;; esac", env);
        assert(env.get("r") == "other");

        // function definition + call with positional params
        run("setname() { who=$1; }", env);
        run("setname alice", env);
        assert(env.get("who") == "alice");

        // command substitution captures stdout
        run("greeting=$(echo hello)", env);
        assert(env.get("greeting") == "hello");
        run("n=$(echo 6); sq=$(( n * n ))", env);
        assert(env.get("sq") == "36");
    }

    std::cout << "sleela-terminal smoke: OK (M2)\n";
    return 0;
}
