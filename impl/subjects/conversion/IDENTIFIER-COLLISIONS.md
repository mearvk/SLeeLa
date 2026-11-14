# Java identifiers that collide with SLeeLa (measured 2026-11-14)

Each of 192 candidate words (SLeeLa keywords, the compiler's built-in names in
`impl/frontend/semantic.cpp`, `next`, and the two converters' rename lists) was
used as a Java identifier in a `#sleela 3.1` / `spark;` program and run with
`impl/build/sleela`: as a local, a parameter and a field (`var`), and as a
method declared and called only through `this.m()` / `obj.m()` (`methodq`).

This is data for `conversion.model`. It is not yet a model row kind: the
sleelactl loader (`sleelactl/convert/conv_model.c`) rejects unknown kinds, so
adding `rename` / `qualify` rows needs a one-line entry in its table first.

## Must be renamed (13)

| Word | var | method, qualified call |
|---|---|---|
| `String` | FAIL | FAIL |
| `close` | ok | FAIL |
| `connect` | ok | FAIL |
| `int128` | FAIL | FAIL |
| `int64` | FAIL | FAIL |
| `main` | ok | FAIL |
| `next` | FAIL | ok |
| `print` | FAIL | FAIL |
| `require` | FAIL | FAIL |
| `send` | ok | FAIL |
| `struct` | FAIL | FAIL |
| `usualgc` | FAIL | FAIL |
| `usualmem` | FAIL | FAIL |

`next` is the silent one: as a variable it reads as the integer 1 (the
next-degree idiom, `impl/frontend/compiler.cpp` `emitVar`). `main` must not be
renamed on an entry class. Neither converter handles `next`, `connect` or `send`
yet, and the sleelactl list (`kSleelaReserved`) also misses `require`,
`usualgc`, `usualmem`, `int64`, `int128`.

Do not add `next`, `connect` or `send` to `JConvRules.isRenamedIdent`: that list
renames every use, method calls included, so `it.next()` on a library Iterator
became `it.next_()` (tried 2026-11-14, reverted). `next` needs a rename of
variables, parameters and fields only; `connect` and `send` need a rename of
declared methods of the converted tree only, not of library calls.

## Fine as a variable; as a method only when every call is qualified (170)

An unqualified call `m(...)` reaches the VM built-in instead. lib/java/converter
qualifies only about 40 of these (`JConvRules.isBuiltinCallName`).

`accept` `arrayGet` `arrayLength` `arrayNew` `arrayPush` `arraySet` `audioAdd` `audioClose` `audioControls` `audioNew` `audioPlatform` `audioRender` `audioValidate` `bestOfArch` `bestOfArchParam` `bestOfArchRealized` `bestOfArchState` `bestOfBest` `bestOfCandidate` `bestOfCandidateArch` `bestOfCertainty` `bestOfChoice` `bestOfClose` `bestOfCostBudget` `bestOfJitter` `bestOfLoss` `bestOfMean` `bestOfMinVersion` `bestOfNew` `bestOfRecord` `bestOfReport` `bestOfScore` `bestOfWeight` `bigadd` `bigdiv` `bigdivmod` `bigisqrt` `bigmul` `bigsub` `conduct` `congruent` `degreemax` `fifoCreate` `ideaCanvasActiveBackend` `ideaCanvasAdvance` `ideaCanvasCols` `ideaCanvasCount` `ideaCanvasCreate` `ideaCanvasDestroy` `ideaCanvasExcite` `ideaCanvasGpuAvailable` `ideaCanvasHasRenderer` `ideaCanvasIntensityAt` `ideaCanvasMeasuredHz` `ideaCanvasMigrate` `ideaCanvasOpenWindow` `ideaCanvasPolicy` `ideaCanvasPresent` `ideaCanvasRows` `ideaCanvasRun` `ideaCanvasSetBackendPolicy` `ideaCanvasSetParams` `ideaCanvasShouldClose` `ideaCanvasStep` `ideaCanvasTimestep` `insight` `javaStringCharAt` `javaStringCodePointAt` `javaStringCodePointCount` `javaStringCompareTo` `javaStringConcat` `javaStringFromCodeUnit` `javaStringFromCodeUnits` `javaStringFromUtf8` `javaStringHashCode` `javaStringLength` `javaStringOffsetByCodePoints` `javaStringSubstring` `javaStringToUtf8` `join` `listen` `lock` `nativeCall` `nativeClose` `nativeOpen` `npfsCreate` `openFile` `osCapability` `osChangeDir` `osCurrentDir` `osExists` `osFileSize` `osGetEnv` `osHostName` `osIsDir` `osKill` `osMakeDir` `osPlatform` `osProcessClose` `osProcessId` `osRemove` `osRename` `osRun` `osSetEnv` `osSpawn` `osTempDir` `osUserName` `osWait` `oslsCronNextAfter` `oslsCronValidate` `oslsEnumerateEnvironment` `oslsEnumerateProcesses` `oslsGetAvailableMemory` `oslsGetCPUInfo` `oslsGetCapabilities` `oslsGetDiskSpace` `oslsGetEnvironmentValue` `oslsGetFileInfo` `oslsGetMemoryInfo` `oslsGetNativeError` `oslsGetNetworkInterfaces` `oslsGetPathInfo` `oslsGetRuntimeInfo` `oslsGetSystemLoad` `oslsGetTimezoneInfo` `oslsGetVolumeInfo` `oslsListDirectory` `oslsNetworkResolve` `oslsPathJoin` `oslsPathNormalize` `oslsPathResolve` `oslsProcessInfo` `oslsSystemInfo` `pipe` `pipePeer` `read` `recv` `role` `route` `sleep` `sockclose` `sockread` `sockwrite` `spawn` `structPack` `structUnpack` `synchroClose` `synchroDispatch` `synchroLoss` `synchroMax` `synchroMean` `synchroMin` `synchroOpen` `synchroP95` `synchroReceived` `synchroReport` `synchroSent` `sysdepth` `timeHttpDate` `timeJson` `timeLocation` `timeMonotonicNanos` `timeNtp` `timePrecisionMillis` `timeSetLocation` `timeUtcMillis` `timeUtcNanos` `unlinkFile` `unlock` `write`

## Fine in every use (9)

`Any` `Sleela` `gc` `int32` `length` `material` `mem` `spark` `utf16`
