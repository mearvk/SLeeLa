# Backup: java-jdk28-tier-2026-11-12

Pre-change original of `impl/frontend/driver.cpp` before the JDK 28 tier
loader rule (SLEELA.syntax 43): when a program imports `java.jdk-28`, a JDK 28
reference unit wins over any other unit of the same name.

Restore: `cp impl/backup/java-jdk28-tier-2026-11-12/driver.cpp impl/frontend/driver.cpp`
Verify:  `(cd impl/backup/java-jdk28-tier-2026-11-12 && sha256sum -c SHA256SUMS)`
