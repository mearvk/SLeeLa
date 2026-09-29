# SLeeLa /lib Library Index

**Library revision:** 0.1  
**Initial front-end objects:** 69  
**Target standard-library objects:** 2,048  
**Verified `.sleela` source files before `/lib`:** 160  
**Verified `.sleela` source files after `/lib`:** 229 on `master`; 238 on `main`

| Family | Objects |
|---|---:|
| core | 8 |
| collections | 8 |
| text | 8 |
| io | 8 |
| vm | 13 |
| os | 10 |
| net | 8 |
| security | 6 |
| **Total** | **69** |

The VM and OS families are language-facing contracts for facilities implemented below the SLeeLa layer. Native execution belongs behind an explicit bridge.

**SLeeLa — MEARVK LLC — 2026**
# SLeeLa /lib Library Index

**Revision:** 0.3  
**Packages:** 23  
**SLeeLa source units:** 917  
**Module-facade symbols:** 53  
**Symbol manifest:** `LIBRARY.SYMBOLS.md`

| Family | Sources |
|---|---:|
| collections | 8 |
| compiler | 16 |
| core | 83 |
| crypto | 16 |
| database | 16 |
| debugger | 16 |
| deliberation | 295 |
| filesystem | 16 |
| http | 16 |
| io | 8 |
| math | 243 |
| memory | 16 |
| net | 8 |
| os | 10 |
| process | 16 |
| reflection | 16 |
| regex | 27 |
| runtime | 16 |
| security | 6 |
| text | 8 |
| ui | 16 |
| video | 16 |
| vm | 13 |

The compiler and Nordshrift loader recursively discover `/lib`. Module packages that previously existed only in their subsystem trees now have SLeeLa facade sources under `/lib` and remain traceable to their original source roots.

**Max Rupplin — MEARVK LLC — 2026**
