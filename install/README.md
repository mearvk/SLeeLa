# SLeeLa Quick and Safe Install

Cross-platform installer for Linux, macOS, and Windows 10+.

It prompts for SLeeLa System startup, SLeeLa Server, HTTP Servers 1-9, VM Edition, Port Authority startup, pause, and shutdown behavior. It builds first, stages the installation, backs up an existing user-local installation, copies files, writes startup configuration, and can update the user PATH.

Normal installation is user-local and does not require administrator or root access.

Linux/macOS: quick-safe-install.sh
Windows 10+: quick-safe-install.ps1

The installer does not silently enable network listeners or change system-wide service configuration.
