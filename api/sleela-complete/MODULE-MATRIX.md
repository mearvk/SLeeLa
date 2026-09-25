# Sleela-Complete Module Matrix

| Existing SLeeLa area | Sleela-Complete designer surface |
|---|---|
| Core / runtime | ProjectSpecification, ModuleSpecification, ClassSpecification, Component, Configuration, EventBus, Result, Thread, Timer, Lock |
| API / foundational API | TypeSpecification, InterfaceSpecification, MethodSpecification, FieldSpecification, ParameterSpecification |
| AI | Model, Inference, DataSet, KnowledgeUnit |
| Audio / media | MediaStream plus existing telephony-skya/sleela audio classes |
| BODI | ModuleSpecification, MessageSpecification, EventSpecification, WorkflowSpecification |
| Data analytics | DataSet, Query, DataModelSpecification, WorkflowSpecification |
| Database | DatabaseConnection, Query, DataSet, ServiceSpecification |
| Decompiler | Decompiler |
| Email | EmailService, MessageSpecification, EndpointSpecification |
| HTTP multiplexing / HTTP servers | HttpService, Server, Router, Listener, NetworkEndpoint |
| Listener / posting / router | Listener, EventSpecification, Router, EndpointSpecification |
| Media | MediaStream, Stream, EventBus |
| Memory management | MemoryManager, MemoryBlock, MemoryStructure, StructMemory, Leech, MemoryFlow, MemoryCondition, MemoryPolicy, MemoryTelemetry, MemoryAllocator, MemoryArena, MemoryPool, MemoryGuard, MemoryValidation |
| Reflection / runtime type metadata | ReflectionRegistry, TypeInfo, FieldInfo, MethodInfo, ParameterInfo, EnumInfo, BaseTypeInfo, AttributeInfo, ConstructorInfo, PropertyInfo, Invocation, Serializer |
| Regex | Pattern |
| Server | Server, Router, Listener, ServiceSpecification |
| Subjects / science libraries | Subject, KnowledgeUnit, DataModelSpecification |
| Webserver | HttpService, Server, Router, EndpointSpecification |
| Telephony-Skya | Call, SipTransaction, RtpStream, MediaStream plus the existing VoIP classes |
| Platform support | PlatformSpecification, BackendSpecification, AdapterSpecification, ConnectorSpecification |
| Security | Credential, SecureChannel, Certificate, SecuritySpecification |
| UI / application design | Window, Widget, Component, EventBus |
| I/O | File, Stream, MessageSpecification |
| XML | XmlDocument, XmlElement |
| Synchronization | EventBus, Thread, Lock, Timer |
| Terminal | Terminal, Stream, Configuration |
| Native implementation | BackendSpecification, AdapterSpecification, BuildSpecification, TestSpecification |

## Designer contract

A designer may start with ProjectSpecification and ModuleSpecification, define every class and contract with the specification classes, select platform/backend adapters, then define security, build, test, documentation and examples before native implementation is selected.

For memory-aware applications, the designer can additionally specify ownership, structure, observation or Leech relationships, memory policy, telemetry and deterministic condition behavior.

For reflection-aware applications, the designer can specify type metadata, fields, methods, parameters, properties, constructors, attributes, serialization and native interoperability requirements.

## Native boundary

The C/C++ packages under api/native/memory and api/native/reflection are native implementation substrates for these designer-facing contracts. Native availability does not by itself mean that a Sleela-facing class or every advanced production feature has been implemented.

## Completeness rule

A module is not considered fully specified merely because a native implementation exists. The Sleela-facing specification must describe its public classes, contracts, data, events, lifecycle, dependencies, platform constraints, security boundary, build target and tests.

For Memory Manager and Reflection specifically, completeness also requires that ownership and lifetime, metadata consistency, native ABI boundaries, error behavior and platform constraints are explicitly defined.

Sleela-Complete therefore names the authoring state, not a claim that every native backend is already production-complete.