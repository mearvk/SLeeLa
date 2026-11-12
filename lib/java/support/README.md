# /lib/java/support: shared helper classes for the Java container

One central home for code that several `/lib/java` types need, so no Java type
carries a private copy.

| Class | What it holds | Used by |
|---|---|---|
| `SLJavaBits` | 32-bit and/or/xor/not, rotate, unsigned halves of a 64-bit long (`lo`, `hi`, `make`), non-negative `mod`. The VM has no bitwise operators, so these are arithmetic. | `UUID`, `SLJavaMd5` |
| `SLJavaHex` | Fixed-width lower-case hex text, hex digit value. | `UUID` |
| `SLJavaMd5` | MD5 (RFC 1321) of a byte array, 16 digest bytes. Reference-grade, not constant-time. | `UUID.nameUUIDFromBytes` |

Call them as static methods through the class name, for example
`SLJavaBits.xor32(a, b)`. The loader finds each class by file name. They are
not Java types, so they have no `javaType` field and do not count toward JDK
coverage. New shared helpers (CRC, Base64 alphabet tables, digest padding) go
here, not inside the Java type that first needed them.

Rule for authors: array elements start out `null`, and `null` in arithmetic
makes the result a double; zero-fill a numeric array before summing it.
