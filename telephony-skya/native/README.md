<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# Skya Native C/C++ Layer

This directory is the native execution boundary for Skya. Keep protocol and operating-system work here or in the existing SLeeLa native subsystems; do not duplicate the SLeeLa VM/runtime.

Components:
- skya_engine.h: stable C ABI.
- skya_engine.cpp: engine state and lifecycle implementation.
- main.cpp: command-line native launcher.
- Makefile: portable C++17 build.
- SKYA-SLEEELA-ABI.md: binding contract between native engine and SLeeLa.
- SKYA-CXX-INTEGRATION.md: C/C++ integration rules and ownership model.

SLeeLa owns the language runtime, threading model, sockets, files, and platform abstraction. Skya owns telephony session state. JavaFX is presentation only.