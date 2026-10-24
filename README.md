# KANE Browser

KANE is an experimental Windows 10/11 x64 browser project. The current goal is a lightweight native Windows shell around Chromium, with tab lifecycle and memory-pressure management while retaining the browser engine's sandbox.

## Current implementation status

- **Chromium engine:** pinned Chromium Embedded Framework (CEF) binary distribution is downloaded and built by CMake.
- **Sandbox:** Windows sandbox build is enabled in the CEF integration configuration; CI must pass before this build is considered validated.
- **Windows package:** GitHub Actions builds and uploads the executable, CEF module, DLLs, resource files, and locales together as `KANE-Windows-x64.zip`.
- **Memory policy:** a separate, dependency-free C++ module and unit tests define when inactive tabs may be frozen/discarded. It is not yet wired into Chromium tab lifecycle.
- **UI:** the first CEF integration temporarily uses CEF's minimal sample UI so real Chromium page rendering and process startup can be tested. The custom KANE tab strip/address bar is not integrated yet.
- **Not complete:** this is an early browser integration, not a production-ready browser. Do not use it for sensitive browsing until security, sandbox startup, navigation, permissions, and update behavior have been verified.

## Build on Windows 10/11

Install Visual Studio 2022 with the Desktop development with C++ workload, Windows SDK, CMake, and Python. The first CMake configure downloads the pinned official CEF standard distribution (roughly 350 MB) and verifies it against the checksum published by the CEF build server.

From a **Developer PowerShell for VS 2022**:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --parallel 2
ctest --test-dir build -C Release --output-on-failure
```

Run the packaged `kane_browser.exe` from its complete output folder. Keep the matching `kane_browser.dll`, CEF DLLs, resource files, and `Locales` directory beside it; Chromium needs these runtime files.

## Memory policy principles

- Never automatically discard the active tab.
- Protect tabs that are audible, capturing, downloading, pinned, or contain unsaved form state.
- Use available system memory as a signal, not as a hard promise of browser RAM usage.
- Apply freeze/discard decisions only through supported Chromium lifecycle APIs after integration and restore tests are in place.

## Next integration milestones

1. Replace the temporary CEF sample chrome with KANE's native tab strip, address/search bar, back/forward/reload, and new-tab controls.
2. Connect KANE tabs to CEF browser instances and lifecycle events.
3. Wire the tested memory policy to safe tab freeze/discard and restore behavior.
4. Test 10/30/50-tab workloads and verify audio, WebRTC, forms, downloads, permissions, and session restore.
5. Add a signed installer and a reviewed Chromium/CEF security update process.

CEF is based on Chromium and is distributed under its own license. Keep its license and third-party notices with the packaged application. The CEF version is deliberately pinned; security updates should be reviewed and updated explicitly rather than silently tracking a moving latest build.
