#include "sleela/decompiler/slir.hpp"
#include <algorithm>
#include <sstream>
#include <unordered_map>

namespace sleela::decompiler {
// SLIR construction passes live here. Keeping the IR independent of a native ISA allows
// the x86/x64/ARM/ARM64 lifters to share analysis, VM validation, and refactoring code.
namespace {

// A tiny value-numbering table: each distinct register/operand label gets a stable SLIR
// value id within a function, so operations reference consistent Values.
struct ValueTable {
    std::unordered_map<std::string, std::uint32_t> ids;
    std::uint32_t next{0};
    Value get(const std::string& label, const std::string& type = "u64") {
        auto it = ids.find(label);
        if (it == ids.end()) { it = ids.emplace(label, next++).first; }
        return Value{it->second, type, label};
    }
};

// Map a decoded instruction to an SLIR opcode by its observed, flagged effect first
// (branch/call/return are authoritative) and otherwise by mnemonic family.
OpCode classify(const Instruction& i) {
    if (i.is_return) return OpCode::Return;
    if (i.is_call) return OpCode::Call;
    if (i.is_branch) return OpCode::Branch;
    const std::string& m = i.mnemonic;
    if (m == "mov" || m == "lea" || m == "movn" || m == "movz") return OpCode::Move;
    if (m == "add" || m == "adc") return OpCode::Add;
    if (m == "sub" || m == "sbb") return OpCode::Sub;
    if (m == "mul" || m == "imul") return OpCode::Mul;
    if (m == "div" || m == "idiv") return OpCode::Div;
    if (m == "cmp" || m == "test" || m == "cbz" || m == "cbnz" || m == "tst") return OpCode::Compare;
    if (m == "push") return OpCode::Store;
    if (m == "pop") return OpCode::Load;
    if (m == "nop") return OpCode::Nop;
    return OpCode::Nop;
}

// Render an architecture-neutral statement for the operation, derived from the recovered
// operands. This is intentionally readable (assignment / call / branch form) rather than a
// faithful native mnemonic; the native form stays available on the Instruction.
std::string statement(const Instruction& i, OpCode op) {
    auto opnd = [&](std::size_t n) -> std::string {
        return n < i.operands.size() ? (i.operands[n].text.empty() ? "?" : i.operands[n].text) : "?";
    };
    switch (op) {
        case OpCode::Return: return "return;";
        case OpCode::Call:   return "call " + (i.operands.empty() ? std::string("<indirect>") : opnd(0)) +
                                    (i.relocation_symbol.empty() ? "" : " /* " + i.relocation_symbol + " */") + ";";
        case OpCode::Branch: return (i.is_conditional ? "if (cc) goto " : "goto ") +
                                    (i.operands.empty() ? std::string("<indirect>") : opnd(0)) + ";";
        case OpCode::Move:   return i.operands.size() >= 2 ? opnd(0) + " = " + opnd(1) + ";" : (i.text + ";");
        case OpCode::Add:    return i.operands.size() >= 2 ? opnd(0) + " += " + opnd(1) + ";" : (i.text + ";");
        case OpCode::Sub:    return i.operands.size() >= 2 ? opnd(0) + " -= " + opnd(1) + ";" : (i.text + ";");
        case OpCode::Mul:    return i.operands.size() >= 2 ? opnd(0) + " *= " + opnd(1) + ";" : (i.text + ";");
        case OpCode::Div:    return i.operands.size() >= 2 ? opnd(0) + " /= " + opnd(1) + ";" : (i.text + ";");
        case OpCode::Compare:return "compare(" + opnd(0) + (i.operands.size() >= 2 ? ", " + opnd(1) : "") + ");";
        case OpCode::Store:  return "push(" + opnd(0) + ");";
        case OpCode::Load:   return "pop(" + opnd(0) + ");";
        case OpCode::Nop:    return "/* " + (i.text.empty() ? i.mnemonic : i.text) + " */";
        default:             return "/* " + (i.text.empty() ? i.mnemonic : i.text) + " */";
    }
}

// Populate an operation's SLIR inputs/outputs from the recovered operands, following the
// Intel dest-first ordering already produced by the decoder.
void wire_operands(Operation& o, const Instruction& i, OpCode op, ValueTable& vt) {
    auto reg_like = [](const Operand& op2) {
        return op2.kind == Operand::Kind::Register || op2.kind == Operand::Kind::Memory;
    };
    switch (op) {
        case OpCode::Move:
            if (i.operands.size() >= 2) { o.outputs.push_back(vt.get(i.operands[0].text)); o.inputs.push_back(vt.get(i.operands[1].text)); }
            break;
        case OpCode::Add: case OpCode::Sub: case OpCode::Mul: case OpCode::Div:
            if (i.operands.size() >= 2) { auto d = vt.get(i.operands[0].text); o.outputs.push_back(d); o.inputs.push_back(d); o.inputs.push_back(vt.get(i.operands[1].text)); }
            break;
        case OpCode::Compare:
            for (const auto& x : i.operands) if (reg_like(x)) o.inputs.push_back(vt.get(x.text));
            break;
        case OpCode::Store: case OpCode::Load:
            if (!i.operands.empty()) { auto v = vt.get(i.operands[0].text); (op == OpCode::Store ? o.inputs : o.outputs).push_back(v); }
            break;
        default: break;
    }
    for (const auto& x : i.operands) if (x.kind == Operand::Kind::Immediate) { o.immediate = x.value; break; }
}
} // namespace

Module Lifter::lift(const Artifact& a, const std::vector<Instruction>& ins,
                    const ControlFlowGraph& cfg, const std::vector<Symbol>& functions) const {
    Module m;
    m.name = a.name();
    m.format = a.format();
    m.architecture = a.architecture();
    m.artifact_class = a.artifact_class();
    m.library_metadata = a.library_metadata();
    m.kernel_module_metadata = a.kernel_module_metadata();
    m.sections = a.interfaces().sections;

    // Sort function entry points and compute each function's instruction window as the
    // range up to the next entry (or the first return within it).
    std::vector<Symbol> fns = functions;
    std::sort(fns.begin(), fns.end(), [](const Symbol& x, const Symbol& y) { return x.address < y.address; });

    std::vector<const Instruction*> all;
    for (const auto& i : ins) all.push_back(&i);
    std::sort(all.begin(), all.end(), [](const Instruction* x, const Instruction* y) { return x->address < y->address; });

    for (std::size_t fi = 0; fi < fns.size(); ++fi) {
        const auto start = fns[fi].address;
        const auto limit = (fi + 1 < fns.size()) ? fns[fi + 1].address : UINT64_MAX;
        Function fn;
        fn.name = fns[fi].name;
        fn.address = start;
        fn.provenance.push_back(fns[fi].provenance);

        SlirBlock blk;
        blk.id = 0;
        ValueTable vt;
        bool stop = false;
        for (const auto* ip : all) {
            if (ip->address < start || ip->address >= limit) continue;
            const auto& i = *ip;
            const OpCode op = classify(i);
            Operation o;
            o.opcode = op;
            o.address = i.address;
            o.provenance = i.provenance;
            wire_operands(o, i, op, vt);
            o.text = statement(i, op);
            blk.operations.push_back(std::move(o));
            if (i.is_return) { stop = true; break; }
        }
        (void)stop;
        fn.blocks.push_back(std::move(blk));
        m.functions.push_back(std::move(fn));
    }
    (void)cfg;
    return m;
}
} // namespace sleela::decompiler
