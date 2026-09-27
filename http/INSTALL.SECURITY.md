# SLeeLa HTTP Installation Security

Installation is a privileged boundary. Verify package/source digests when a release manifest exists, verify the requested module, build with warnings enabled, run syntax/self-tests, install with least privilege, and never silently downgrade protocol security.

Do not automatically enable listeners, change firewall rules, or install drivers/kernel extensions.

Windows production packages should be Authenticode signed. macOS production packages should be signed and notarized. Linux releases should use native package-signing conventions.

Windows support is capability/build based. Windows 10/11 are current targets; a future Windows 12, if Microsoft releases one, must be admitted by supported APIs/build capabilities rather than an invented build number.
