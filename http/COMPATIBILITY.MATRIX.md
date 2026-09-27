# SLeeLa HTTP Compatibility Matrix

| Target | HTTP 1.0-9.0 | Native packaging | Service integration |
|---|---|---|---|
| Linux | Source/build validation | .deb/.rpm release pipeline | systemd adapter |
| Windows 10+ | Source/build validation | signed MSI/EXE release pipeline | Windows Service adapter |
| Windows 11 | Source/build validation | signed MSI/EXE release pipeline | Windows Service adapter |
| Future Windows 12* | capability/build detection | future package target | capability detection |
| macOS | Source/build validation | signed .pkg release pipeline | launchd adapter |

* Future-compatible architecture; not a claim that Microsoft has released a product named Windows 12.
