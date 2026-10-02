# Nordshrift Tutorial 03 — The /lib Compiler Bridge

SST and Nordshrift use the same library index as the SLeeLa compiler.

~~~text
                 /lib
                  |
        +---------+---------+
        |                   |
   Nordshrift           SLeeLa Compiler
        |                   |
        +---------+---------+
                  |
             same symbols
~~~

This prevents a class from being visible to the compiler but invisible to Nordshrift.

An SST document referencing vm.SleelaVMCompilerManager is resolved through the shared index; Nordshrift does not copy the VM package into a private registry.

## Exercise

Trace one /lib class from source path to package-qualified symbol and generated SLeeLa.
