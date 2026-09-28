# SLeeLa Heuristic System Monitor

The Heuristic System Monitor (HSM) is SLeeLa's system-awareness and policy-observation layer. It does not maintain a giant table of kernel syscall numbers. Instead, it reasons about operations SLeeLa actually exposes at its platform boundaries.

## 1. OS + Version + Capability

SLeeLa maintains a compact host description:

OS family -> version/build evidence -> architecture -> runtime -> capability profile

The profile covers memory, file I/O, networking, threads, dynamic libraries, terminal, paths, process creation, IPC and time.

Version is primarily diagnostic. Capability is the operational compatibility test. This avoids making an unfamiliar version string a brittle startup failure.

## 2. Three Decisions

At an OS-facing boundary SLeeLa reasons in three states:

1. Known and supported — use the native abstraction.
2. Known but unavailable — report a capability failure and use a documented fallback when one exists.
3. Unknown/custom — route through a controlled native-provider boundary and have HSM observe it.

This is deliberately different from pretending that every raw kernel call is known.

## 3. Heuristic Monitoring

HSM observes unsupported capability requests, platform mismatches, unknown SLeeLa-level operations, native/FFI escape operations, privilege-sensitive operations, resource bursts and resource imbalance.

The monitor accumulates a risk score. It starts in advisory mode. Strict mode can turn review-level findings into a block decision.

Resource imbalance means a workload consuming resources outside its declared or accounted envelope. It is a technical fairness and security concept.

## 4. Unknown Operations

If custom code asks for something SLeeLa does not have a named abstraction for, SLeeLa should not guess and should not silently pretend the operation is portable.

The intended path is:

custom operation -> native provider boundary -> HSM observation -> capability/policy decision -> execute or refuse

On Linux, direct system-call invocation can report ENOSYS when a requested system call is not implemented, and direct invocation is architecture-sensitive. SLeeLa therefore uses a native-provider/extension boundary as its portable contract rather than a cross-platform raw syscall-number dispatcher.

## 5. Security Relationship

HSM is not presented as a complete sandbox. It is an observation and policy layer. Stronger deployments can add a real OS sandbox or syscall filter where appropriate.

SLeeLa therefore uses layered controls:

HSM observation
-> Memory Manager
-> Security Supervisor
-> native/platform abstraction
-> OS

## 6. Grade Integration

- Grade I: memory accounting + OS identity/capability awareness + HSM observation.
- Grade II: Grade I + 1–3 process session accounting.
- Grade III: Grade II + resource accounting at principal SLeeLa OS abstraction boundaries.

HSM is common to all three grades.

## 7. Excellent Operating Rule

The monitor prefers:

detect -> explain -> contain -> recover

over:

guess -> execute -> discover failure later

The runtime remains usable on unfamiliar but compatible systems while unusual behavior becomes visible and policy-controlled.
