#!/usr/bin/env python3
"""Synthetic tests for the Java source implementation-depth audit."""
from __future__ import annotations

import unittest

from java_source_implementation_audit import analyze_text


class ImplementationAuditTests(unittest.TestCase):
    def test_dispatch_placeholder_is_not_implementation(self):
        result = analyze_text('''
            class Sample {
              String invoke(String operation) {
                return "java.invoke:java.lang.String:" + operation;
              }
            }
        ''')
        self.assertEqual(result["classifications"].get("generic-dispatch"), 1)

    def test_accessor_and_constant_are_separated(self):
        result = analyze_text('''
            class Sample {
              int getValue() { return value; }
              boolean available() { return false; }
              int value;
            }
        ''')
        self.assertEqual(result["classifications"].get("accessor-or-field-copy"), 1)
        self.assertEqual(result["classifications"].get("constant-placeholder"), 1)

    def test_nested_braces_and_strings_do_not_truncate_body(self):
        result = analyze_text('''
            class Sample {
              String format(String input) {
                if (input == "") { return "{}"; }
                return input;
              }
            }
        ''')
        self.assertEqual(result["classifications"].get("substantive-candidate"), 1)

    def test_empty_method(self):
        result = analyze_text("class Sample { void todo() { } }")
        self.assertEqual(result["classifications"].get("empty"), 1)


if __name__ == "__main__":
    unittest.main()
