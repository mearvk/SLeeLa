# SLeeLa IDE and Language-Server Direction

## Purpose

SLeeLa tooling should expose compiler intelligence without requiring users to invoke low-level build commands.

## Core capabilities

- syntax highlighting;
- diagnostics;
- autocomplete;
- hover/type information;
- go-to-definition;
- symbol search;
- references;
- rename/refactoring;
- formatting;
- project navigation;
- test discovery;
- build/run;
- debugger integration;
- profiler integration;
- documentation/API browser.

## Language server

A future SLeeLa language server should reuse the compiler lexer, parser, AST and semantic model rather than maintaining a second parser.

## Debugging

Debug information should map generated/core instructions back to source locations and preserve class/method/variable identity where possible.

## Editor independence

The protocol layer should remain editor-neutral so VS Code, JetBrains, Vim/Neovim and other clients can consume it.

## Current foundation

The repository's C++ compiler and diagnostics are the authoritative semantic source. IDE tooling should be a client of that implementation.

**Max Rupplin — MEARVK LLC — 2026**
