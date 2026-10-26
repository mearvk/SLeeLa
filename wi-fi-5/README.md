<img align="right" src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://raw.githubusercontent.com/mearvk/SLeeLa/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">







# Wi-Fi 5

**SLeeLa Wi-Fi 5 Foundation**

The `/wi-fi-5` directory is reserved for SLeeLa's Wi-Fi 5 work: a common place to document the wireless networking standard, supported capabilities, interoperability considerations, security requirements, and how SLeeLa components may use compatible wireless networks.

The directory name **Wi-Fi 5** identifies the requested generation. Specifications and implementation details must distinguish verified standard requirements from proposed SLeeLa design choices.

## Scope

- Protocol and capability overview for Wi-Fi 5.
- Compatibility and interoperability notes.
- Security and configuration guidance.
- Implementation plans, test cases, and platform considerations.

## Source Series

- [Technical Specifications](TECHNICAL-SPECIFICATIONS.md)
- [Build Instructions](BUILDING.md)
- [C API and Example](src/c/)
- [C++ API and Example](src/cpp/)
- [Java Reference Class and Demo](src/java/)

The C, C++, and Java files provide static reference metadata and simple capability-profile checks. They do not implement the radio PHY/MAC, scan for wireless networks, or configure adapters. The listed rates and capabilities are standard-level reference values, not promises about connected hardware or application throughput.

## Status

Research and reference-source foundation. Compile examples with the commands in [BUILDING.md](BUILDING.md). Hardware-level implementation requires operating-system and driver integration, compatible certified radios, regulatory controls, and conformance testing.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
