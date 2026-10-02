# Tutorial 03 — Advanced VM Source

An SLJVM construction request combining broker, resolver, attestation, migration, checkpointing, deterministic replay, and signed packaging.

```sleela
class AdvancedSLJVMExample {
    SleelaVMOptions options;
    SleelaVMRuntimeOptions runtime;
    SleelaVMSecurityOptions security;
    SleelaVMJVMOptions jvm;
    SleelaVMBuildOptions build;
    SleelaVMSource source;

    function build() {
        options.target = 2;
        options.architecture = 10;
        options.operatingSystem = 22;
        options.abi = 32;
        options.executionMode = 43;
        options.garbageCollection = 53;
        options.threadingModel = 62;
        options.ioModel = 72;
        options.securityModel = 83;
        options.brokerModel = 1;
        options.jvmModel = 1;
        options.featureMaskLow = 134217727;

        runtime.runtimeMode = 1;
        runtime.compatibilityMode = 1;
        runtime.resolverModel = 1;
        runtime.checkpointModel = 1;
        runtime.migrationModel = 1;
        runtime.recoveryModel = 1;

        security.securityModel = 83;
        security.capabilityModel = 1;
        security.cryptoModel = 1;
        security.certificateModel = 1;
        security.attestationModel = 1;
        security.auditModel = 1;
        security.failClosed = 1;
        security.policySnapshot = 1;
        security.replayProtection = 1;

        jvm.jvmModel = 1;
        jvm.bytecodeLevel = 21;
        jvm.brokerModel = 1;
        jvm.brokerSecurity = 1;
        jvm.nativeBridge = 1;
        jvm.moduleSystem = 1;

        build.compilerMode = 1;
        build.optimization = 2;
        build.reproducibleBuild = 1;
        build.cStandard = 17;
        build.cppStandard = 20;
        build.abi = 32;
        build.packageFormat = 102;
        build.certificateFormat = 1;
        build.artifactHash = 256;

        source.sourceName = "advanced-sljvm";
        source.targetKind = "SLJVM";
        source.architecture = "x86_64";
        source.operatingSystem = "macos";
        source.abi = "JVM";
        source.memoryLimit = 536870912;
        source.cpuLimit = 8;
        source.stackLimit = 33554432;
        source.options = options;

        source.validate();
        source.analyzePhysicalLimits();
        source.planModules();
        source.generateC();
        source.generateCPP();
        source.buildSLJVM();
    }
}
```

The source expresses requirements; the compiler selects concrete modules after architecture, capability, security, ABI, and physical-limit analysis.
