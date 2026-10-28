<#
.SYNOPSIS
    Build the Sleelavirin(TM) native antivirus engine on Windows.

.DESCRIPTION
    The engine is portable C11 / C++17 with a thin portability shim
    (src/sleelavirin_compat.h) that covers MSVC and MinGW differences. This
    script auto-detects a toolchain and builds the C library, the C++ library,
    and the sleelavirin-scan CLI into .\build.

    Toolchains, in order of preference:
      1. CMake + any generator (MSVC / Ninja / MinGW)  -- recommended
      2. MinGW-w64 GCC/G++ directly (gcc/g++ on PATH)   -- matches the repo's
         Windows 10+ convention (MSYS2: pacman -S mingw-w64-ucrt-x86_64-gcc)

    Windows version floor:
      -WinXP   target the Windows XP API floor (0x0501). Requires an XP-capable
               toolchain (see PLATFORMS.md). Without it, the engine defaults to a
               Windows 7 floor (0x0601) that also runs on 8/10/11.

.PARAMETER WinXP
    Build with the Windows XP API floor.

.PARAMETER Test
    Run the test suite after building (CMake path uses ctest).

.NOTES
    Max Rupplin - MEARVK LLC - 2026.
#>
param(
    [switch]$WinXP,
    [switch]$Test
)

$ErrorActionPreference = "Stop"
Set-Location -Path $PSScriptRoot

function Have($name) { return [bool](Get-Command $name -ErrorAction SilentlyContinue) }

$winntFlag = if ($WinXP) { "0x0501" } else { "" }

if (Have "cmake") {
    Write-Host "== Building with CMake =="
    $args = @("-S", ".", "-B", "build")
    if ($WinXP) { $args += "-DSLEELAVIRIN_WINXP=ON" }
    & cmake @args
    & cmake --build build --config Release
    if ($Test) { Push-Location build; & ctest --output-on-failure -C Release; Pop-Location }
    Write-Host "Artifacts in .\build (libsleelavirin, libsleelavirin_cpp, sleelavirin-scan)."
    exit 0
}

if ((Have "gcc") -and (Have "g++")) {
    Write-Host "== Building with MinGW-w64 GCC/G++ =="
    New-Item -ItemType Directory -Force -Path build | Out-Null
    $cflags   = @("-O2","-Wall","-Wextra","-Wpedantic","-std=c11","-Isrc")
    $cxxflags = @("-O2","-Wall","-Wextra","-Wpedantic","-std=c++17","-Isrc")
    if ($WinXP) { $cflags += "-D_WIN32_WINNT=$winntFlag"; $cxxflags += "-D_WIN32_WINNT=$winntFlag" }

    $cobj = @("scan","heuristic","native","ac","sig","bridge")
    $objs = @()
    foreach ($m in $cobj) {
        & gcc @cflags -c "src/sleelavirin_$m.c" -o "build/sleelavirin_$m.o"
        $objs += "build/sleelavirin_$m.o"
    }
    & ar rcs build/libsleelavirin.a @objs
    & g++ @cxxflags -c src/sleelavirin_cpp.cpp -o build/sleelavirin_cpp.o
    & ar rcs build/libsleelavirin_cpp.a build/sleelavirin_cpp.o
    & gcc @cflags src/sleelavirin_native_main.c build/libsleelavirin.a -o build/sleelavirin-scan.exe

    if ($Test) {
        & gcc @cflags tests/test_ac.c build/libsleelavirin.a -o build/test_ac.exe
        & .\build\test_ac.exe
        & gcc @cflags tests/test_bridge.c build/libsleelavirin.a -o build/test_bridge.exe
        & .\build\test_bridge.exe
        & g++ @cxxflags tests/test_cpp_scan.cpp build/libsleelavirin_cpp.a build/libsleelavirin.a -o build/test_cpp_scan.exe
        & .\build\test_cpp_scan.exe
    }
    Write-Host "Artifacts in .\build (libsleelavirin.a, libsleelavirin_cpp.a, sleelavirin-scan.exe)."
    exit 0
}

Write-Error "No toolchain found. Install CMake (recommended) or MinGW-w64 GCC/G++ (MSYS2: pacman -S mingw-w64-ucrt-x86_64-gcc), or use Makefile.win with MSVC's nmake."
