# Tutorial 02 — Moderate VM Source

A native VM with concurrency, asynchronous I/O, resolver support, GC, and feature bits.

```sleela
class ModerateVMExample {
    SleelaVMOptions options;
    SleelaVMExecutionOptions execution;
    SleelaVMMemoryOptions memory;
    SleelaVMConcurrencyOptions concurrency;
    SleelaVMIOOptions io;
    SleelaVMSecurityOptions security;
    SleelaVMSource source;

    function build() {
        options.target = 1;
        options.architecture = 11;
        options.operatingSystem = 20;
        options.abi = 30;
        options.executionMode = 43;
        options.garbageCollection = 52;
        options.threadingModel = 62;
        options.ioModel = 72;
        options.securityModel = 82;
        options.featureMaskLow = 16777216 + 33554432 + 67108864 + 134217728;

        execution.executionMode = 43;
        execution.instructionBudget = 1000000;
        execution.callDepth = 1024;
        execution.deterministicMode = 1;

        memory.memoryLimit = 536870912;
        memory.heapLimit = 402653184;
        memory.stackLimit = 16777216;
        memory.pointerBits = 64;
        memory.garbageCollector = 52;
        memory.guardPages = 1;

        concurrency.threadingModel = 62;
        concurrency.workerThreads = 4;
        concurrency.maxThreads = 16;
        concurrency.asyncModel = 1;

        io.ioModel = 72;
        io.resolverMode = 1;
        io.tlsMode = 1;
        io.networkBudget = 104857600;

        security.securityModel = 82;
        security.capabilityModel = 1;
        security.failClosed = 1;
        security.secretRedaction = 1;
        security.replayProtection = 1;

        source.sourceName = "moderate-vm";
        source.targetKind = "SLVM";
        source.architecture = "arm64";
        source.operatingSystem = "linux";
        source.abi = "C";
        source.memoryLimit = 536870912;
        source.cpuLimit = 8;
        source.stackLimit = 16777216;
        source.options = options;

        source.validate();
        source.analyzePhysicalLimits();
        source.planModules();
        source.emitC();
        source.emitCPP();
    }
}
```

Required features must fit the selected architecture, OS, resources, and policy. Optional features can be omitted only when explicitly optional.
