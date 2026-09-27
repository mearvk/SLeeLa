# SLeeLa HTTP Troubleshooting

Missing module: run the installer from the repository root and verify http-X.Y exists.

Build failure: install the host C/C++ toolchain and inspect the module build/Makefile. The installer does not silently bypass a failed syntax check.

Windows detection: product/build information is recorded, but support decisions are capability based rather than dependent on a hard-coded marketing-version list.

Service failure: installation does not imply service activation. Check the native service manager separately.

Network failure: check application configuration, listener state, TLS policy and the host firewall. Installation does not automatically open ports.
