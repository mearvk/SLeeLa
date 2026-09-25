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

## Completeness rule

A module is not considered fully specified merely because a native implementation exists. The Sleela-facing specification must describe its public classes, contracts, data, events, lifecycle, dependencies, platform constraints, security boundary, build target and tests.

Sleela-Complete therefore names the authoring state, not a claim that every native backend is already production-complete.
