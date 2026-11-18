# Java identifiers that collide with SLeeLa (reviewed 2026-11-18)

Each of 192 candidate words (SLeeLa keywords, the compiler's built-in names in
`impl/frontend/semantic.cpp`, `next`, and the two converters' rename lists) was
used as a Java identifier in a `#sleela 3.1` / `spark;` program and run with
`impl/build/sleela`: as a local, a parameter and a field (`var`), and as a
method declared and called only through `this.m()` / `obj.m()` (`methodq`).

The `qualify` inventory is in section 9d of `conversion.model`; the shared
`rename` inventory is in section 9e. Both the C++ model loader and native
`JConvModel` read these rows. The C++ converter remains fail-closed for reserved
member declarations, while the native converter applies the replacement per
member; the shared model determines the collision and replacement policy.

## Replacement and qualification policy

| Word | Shared model policy | Notes |
|---|---|---|
| `close` | `closeResource` | Special Java resource-method mapping; the VM's `close` is receiver-independent |
| `int128`, `int64`, `print`, `require`, `struct`, `usualgc`, `usualmem` | trailing `_` | Shared `rename ... all ...` rows |
| `next` | `next_` in value context only | `next()` and `it.next()` remain ordinary Java method calls |
| `connect`, `send` | qualified call when unqualified | Shared `qualify` rows; not globally renamed |
| `main` | preserved for the entry class | Entry-point contract |
| `String` | no rename | Java type name; handled by type conversion |

The `rename` records carry an explicit context (`all` or `value`). This
prevents the earlier failure where globally renaming `next` also rewrote
library calls such as `it.next()`. Built-in call qualification is a separate
rule: it applies to method calls, not variable identifiers.

C++ currently refuses reserved Java member declarations that would collide
with SLeeLa names, rather than emitting a potentially inconsistent renamed
member; the diagnostic lookup uses the same `rename` model row. Local/value
renaming and call qualification are model-driven. The native converter applies
the shared member replacement in its per-member output policy.

## Fine as a variable; as a method only when every call is qualified (173)

An unqualified call `m(...)` reaches the VM built-in instead. Both converters
now qualify these calls from the shared `qualify` rows in
`impl/subjects/conversion/conversion.model` (model 1.5.0).

`accept` `arrayGet` `arrayLength` `arrayNew` `arrayPush` `arraySet` `audioAdd` `audioClose` `audioControls` `audioNew` `audioPlatform` `audioRender` `audioValidate` `bestOfArch` `bestOfArchParam` `bestOfArchRealized` `bestOfArchState` `bestOfBest` `bestOfCandidate` `bestOfCandidateArch` `bestOfCertainty` `bestOfChoice` `bestOfClose` `bestOfCostBudget` `bestOfJitter` `bestOfLoss` `bestOfMean` `bestOfMinVersion` `bestOfNew` `bestOfRecord` `bestOfReport` `bestOfScore` `bestOfWeight` `bigadd` `bigdiv` `bigdivmod` `bigisqrt` `bigmul` `bigsub` `close` `conduct` `congruent` `connect` `degreemax` `fifoCreate` `ideaCanvasActiveBackend` `ideaCanvasAdvance` `ideaCanvasCols` `ideaCanvasCount` `ideaCanvasCreate` `ideaCanvasDestroy` `ideaCanvasExcite` `ideaCanvasGpuAvailable` `ideaCanvasHasRenderer` `ideaCanvasIntensityAt` `ideaCanvasMeasuredHz` `ideaCanvasMigrate` `ideaCanvasOpenWindow` `ideaCanvasPolicy` `ideaCanvasPresent` `ideaCanvasRows` `ideaCanvasRun` `ideaCanvasSetBackendPolicy` `ideaCanvasSetParams` `ideaCanvasShouldClose` `ideaCanvasStep` `ideaCanvasTimestep` `insight` `javaStringCharAt` `javaStringCodePointAt` `javaStringCodePointCount` `javaStringCompareTo` `javaStringConcat` `javaStringFromCodeUnit` `javaStringFromCodeUnits` `javaStringFromUtf8` `javaStringHashCode` `javaStringLength` `javaStringOffsetByCodePoints` `javaStringSubstring` `javaStringToUtf8` `join` `listen` `lock` `nativeCall` `nativeClose` `nativeOpen` `npfsCreate` `openFile` `osCapability` `osChangeDir` `osCurrentDir` `osExists` `osFileSize` `osGetEnv` `osHostName` `osIsDir` `osKill` `osMakeDir` `osPlatform` `osProcessClose` `osProcessId` `osRemove` `osRename` `osRun` `osSetEnv` `osSpawn` `osTempDir` `osUserName` `osWait` `oslsCronNextAfter` `oslsCronValidate` `oslsEnumerateEnvironment` `oslsEnumerateProcesses` `oslsGetAvailableMemory` `oslsGetCPUInfo` `oslsGetCapabilities` `oslsGetDiskSpace` `oslsGetEnvironmentValue` `oslsGetFileInfo` `oslsGetMemoryInfo` `oslsGetNativeError` `oslsGetNetworkInterfaces` `oslsGetPathInfo` `oslsGetRuntimeInfo` `oslsGetSystemLoad` `oslsGetTimezoneInfo` `oslsGetVolumeInfo` `oslsListDirectory` `oslsNetworkResolve` `oslsPathJoin` `oslsPathNormalize` `oslsPathResolve` `oslsProcessInfo` `oslsSystemInfo` `pipe` `pipePeer` `read` `recv` `role` `route` `send` `sleep` `sockclose` `sockread` `sockwrite` `spawn` `structPack` `structUnpack` `synchroClose` `synchroDispatch` `synchroLoss` `synchroMax` `synchroMean` `synchroMin` `synchroOpen` `synchroP95` `synchroReceived` `synchroReport` `synchroSent` `sysdepth` `timeHttpDate` `timeJson` `timeLocation` `timeMonotonicNanos` `timeNtp` `timePrecisionMillis` `timeSetLocation` `timeUtcMillis` `timeUtcNanos` `unlinkFile` `unlock` `write`

## No additional mapping required

`gc` `length` `mem`
