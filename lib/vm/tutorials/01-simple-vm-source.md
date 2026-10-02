# Tutorial 01 — Simple VM Source

A minimal SLVM construction request.

```sleela
class SimpleVMExample {
    SleelaVMOptions options;
    SleelaVMSource source;

    function build() {
        options.target = 1;
        options.architecture = 10;
        options.operatingSystem = 20;
        options.abi = 30;
        options.executionMode = 40;
        options.threadingModel = 60;
        options.ioModel = 70;
        options.securityModel = 81;

        source.sourceName = "simple-vm";
        source.targetKind = "SLVM";
        source.architecture = "x86_64";
        source.operatingSystem = "linux";
        source.abi = "C";
        source.memoryLimit = 67108864;
        source.cpuLimit = 1;
        source.stackLimit = 1048576;
        source.options = options;

        source.validate();
        source.analyzePhysicalLimits();
        source.planModules();
        source.emitC();
    }
}
```

The compiler resolves target, architecture, ABI, memory, and security requirements before emitting the C construction pieces.
