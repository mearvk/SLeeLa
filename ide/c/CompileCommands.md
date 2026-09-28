# C Compile Commands Contract

Version: 0.2.0-dev

The C integration consumes a standard `compile_commands.json` database.

Each compilation entry supplies:
- source file;
- working directory;
- compiler executable;
- language standard;
- include directories;
- macro definitions;
- target architecture;
- sysroot and other compiler flags.

The IDE must normalize paths without changing compiler semantics. Generated diagnostics must retain original source locations.

When no compilation database exists, the project model may construct a command from explicit module configuration, but it must record that the configuration was synthesized.