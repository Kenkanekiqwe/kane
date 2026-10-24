# KANE Browser

KANE is an experimental Windows browser project focused on low memory use, a native Win32/Direct2D interface, and a Chromium-based web engine.

## Current status

The `bootstrap/native-shell` branch contains the first native UI shell and a separately testable memory-policy module. **It is not a complete browser yet**: Chromium is not integrated in this starter commit, so web navigation and site rendering are not available.

## Architecture

- `src/main.cpp` — Win32 application window and Direct2D-painted starter interface.
- `include/kane/memory_policy.h` / `src/memory_policy.cpp` — engine-independent decisions for prioritizing idle tabs under memory pressure.
- `tests/memory_policy_tests.cpp` — lightweight unit tests without third-party test dependencies.
- `CMakeLists.txt` — builds KANE's own code. Chromium itself uses GN/Ninja and must be built separately.

## Build on Windows 10/11

Install Visual Studio 2022 with the Desktop development with C++ workload and a Windows 10/11 SDK, plus CMake.

From a **Developer PowerShell for VS 2022**:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
.\build\Release\kane_browser.exe
```

If using a Visual Studio generator, the executable is normally under `build\Release`.

## Memory policy principles

- Never automatically discard the active tab.
- Protect tabs that are audible, capturing, downloading, or contain unsaved form state.
- Use available system memory as a signal, not as a hard promise of browser RAM usage.
- Make decisions only; the Chromium integration layer must perform freeze/discard through supported browser lifecycle APIs.

## Chromium integration plan

1. Pin and build a known Chromium revision with the official Windows toolchain.
2. Add KANE's browser UI and integration targets to the Chromium GN build.
3. Connect native tab UI to Chromium's browser/tab lifecycle.
4. Integrate memory-policy decisions with Chromium lifecycle APIs without disabling the sandbox.
5. Benchmark repeatable 10/30/50-tab workloads and verify audio, WebRTC, forms, downloads, and restore behavior.

Chromium is a large source tree and is not vendored into this repository. Follow the official [Windows build instructions](https://chromium.googlesource.com/chromium/src/+/main/docs/windows_build_instructions.md) and respect Chromium's licensing and security update requirements.
