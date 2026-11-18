#!/usr/bin/env python3
"""JDK 28 generator signature-index and generic-resolution tests."""

from __future__ import annotations

import unittest

import generate_jdk28 as gen


def method(name, params=(), ret="void", tparams="", mods="public"):
    return {
        "name": name,
        "mods": mods,
        "tparams": tparams,
        "ret": ret,
        "params": [[t, f"arg{i}"] for i, t in enumerate(params)],
        "throws": [],
        "protected": "protected" in mods.split(),
        "static": "static" in mods.split(),
        "abstract": "abstract" in mods.split(),
    }


class JDK28SignatureIndexTests(unittest.TestCase):
    def test_varargs_are_indexed_as_array_parameters(self):
        self.assertEqual(gen.erase("java.lang.String..."), "String[]")
        self.assertEqual(gen.erase("T..."), "Any[]")
        self.assertEqual(gen.erase("java.util.List<? extends T>"), "Any")

    def test_erasure_index_retains_colliding_declarations(self):
        sig = {
            "members": [
                method("put", ("java.util.List<T>",)),
                method("put", ("java.util.Set<T>",)),
                method("put", ("int",)),
            ]
        }
        groups, collisions = gen.member_signature_index(sig)
        self.assertEqual(len(groups[("put", ("Any",))]), 2)
        self.assertEqual(len(groups[("put", ("int",))]), 1)
        self.assertEqual(collisions, 1)

    def test_same_arity_different_constructor_is_not_suppressed(self):
        t = {"name": "example.Widget", "kind": "class", "fields": [], "components": []}
        sig = {
            "tparams": "",
            "members": [
                method("<init>", ("int",)),
                method("<init>", ("java.lang.String",)),
            ],
        }
        lines, constructors, methods, collisions = gen.member_blocks(
            t, sig, "Widget", "  public Widget(int value) { }"
        )
        rendered = "\n".join(lines)
        self.assertEqual(constructors, 1)
        self.assertEqual(methods, 0)
        self.assertIn("Widget(String arg0)", rendered)
        self.assertNotIn("Widget(int arg0)", rendered)

    def test_generic_class_constructor_parameter_is_resolved_as_type_variable(self):
        t = {"name": "example.Box", "kind": "class", "fields": [], "components": []}
        sig = {
            "tparams": "<T>",
            "members": [method("<init>", ("T",))],
        }
        lines, constructors, methods, collisions = gen.member_blocks(
            t, sig, "Box", "  public Box(Any value) { }"
        )
        self.assertEqual(constructors, 0)
        self.assertEqual(methods, 0)

    def test_inherited_interface_implementation_prevents_redundant_stub(self):
        t = {"name": "example.Child", "kind": "class", "fields": [], "components": []}
        sig = {"tparams": "", "members": [method("ready", ("int",), "boolean"), method("hidden", ("int",), "void"), method("abstractHelper", ("int",), "void")]}
        api = {
            "example.Child": {"name": "example.Child", "kind": "class", "super": None, "interfaces": ["example.Ready"]},
            "example.Ready": {"name": "example.Ready", "kind": "interface", "super": None, "interfaces": []},
        }
        impl_texts = {"example.Ready": "  public default boolean ready(int value) { return true; }\n  private void hidden(int value) { }\n  public abstract void abstractHelper(int value);"}
        lines, constructors, methods, collisions = gen.member_blocks(
            t, sig, "Child", "", api, impl_texts
        )
        self.assertEqual(methods, 2)
        self.assertNotIn("ready(", "\n".join(lines))
        self.assertIn("hidden(", "\n".join(lines))
        self.assertIn("abstractHelper(", "\n".join(lines))


if __name__ == "__main__":
    unittest.main()
