# Nordshrift Complete Module Capability Matrix

| Module | Authoring | Native substrate | Platform adapters | Security | Tests |
|---|---|---|---|---|---|
| Core/runtime | Yes | Required | Linux/Windows/macOS | Required | Required |
| API/types | Yes | Shared compiler | N/A | Required | Required |
| Network | Yes | C/C++ networking | Linux/Windows/macOS | TLS/SecureChannel | Required |
| HTTP/web | Yes | HTTP implementations | Linux/Windows/macOS | TLS/auth | Required |
| Server/service | Yes | Runtime/event loop | Linux/Windows/macOS | Ports/firewall | Required |
| I/O | Yes | OS I/O | Linux/Windows/macOS | Permission policy | Required |
| Synchronization | Yes | C/C++ runtime | Linux/Windows/macOS | Lifetime safety | Required |
| Database | Yes | DB connector | Platform-specific | Credentials/TLS | Required |
| Email | Yes | SMTP/transport | Platform-specific | TLS/auth | Required |
| XML | Yes | Parser/serializer | N/A | Input limits | Required |
| UI | Yes | UI backend | Linux/Windows/macOS | Permission policy | Required |
| Media | Yes | Audio/media backend | Linux/Windows/macOS | Device policy | Required |
| Telephony/VoIP | Yes | Skya/native transport | Linux/Windows/macOS | SIP-TLS/SRTP | Required |
| Memory | Yes | C/C++ memory manager | OS allocator | Corruption checks | Required |
| Reflection | Yes | C++ metadata registry | ABI-sensitive | Invocation policy | Required |
| AI/inference | Yes | Model/runtime backend | Backend-specific | Input/model policy | Required |
| Analytics | Yes | Data/runtime backend | N/A | Data policy | Required |
| Subjects/science | Yes | Math/domain runtime | N/A | Validation | Required |
| Terminal | Yes | OS terminal backend | Linux/Windows/macOS | Permission policy | Required |
| Decompiler/tooling | Yes | Toolchain | Platform-specific | Untrusted-input isolation | Required |

"Yes" under Authoring means the module can be represented at the Sleela/Nordshrift authoring layer. It does not claim every native backend is already production-complete.
