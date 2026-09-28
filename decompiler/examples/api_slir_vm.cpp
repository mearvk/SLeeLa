#include "sleela/decompiler/slir.hpp"
#include "sleela/decompiler/vm.hpp"
#include <iostream>

using namespace sleela::decompiler;

int main() {
    Module module;
    module.name = "slir-example";
    module.format = Format::Raw;
    module.architecture = Architecture::X86_64;

    Function function;
    function.name = "constant_add";
    function.address = 0x1000;

    SlirBlock block;
    block.id = 0;

    Operation a;
    a.opcode = OpCode::Const;
    a.immediate = 40;
    a.outputs.push_back({0, "u64"});

    Operation b;
    b.opcode = OpCode::Const;
    b.immediate = 2;
    b.outputs.push_back({1, "u64"});

    Operation add;
    add.opcode = OpCode::Add;
    add.inputs = {{0, "u64"}, {1, "u64"}};
    add.outputs.push_back({2, "u64"});

    Operation ret;
    ret.opcode = OpCode::Return;

    block.operations = {a, b, add, ret};
    function.blocks.push_back(block);
    module.functions.push_back(function);

    Vm vm({.memory_size = 4096, .max_steps = 32, .deterministic = true});
    vm.load(module);

    if (!vm.run()) {
        std::cerr << "VM trap: " << vm.trap().message << "\n";
        return 1;
    }

    std::cout << "result: " << vm.register_value(2) << "\n";
    std::cout << "steps: " << vm.steps() << "\n";
}
