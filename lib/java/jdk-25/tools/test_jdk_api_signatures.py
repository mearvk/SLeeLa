#!/usr/bin/env python3
"""Focused regression tests for JDK signature indexing and generic erasure."""

from __future__ import annotations

import unittest

import jdk_api_signatures as sig


def member(name, params=(), ret="void", tparams="", mods="public", abstract=False, static=False, throws=()):
    return {
        "name": name,
        "mods": mods,
        "tparams": tparams,
        "ret": ret,
        "params": [[t, f"arg{i}"] for i, t in enumerate(params)],
        "throws": list(throws),
        "static": static,
        "abstract": abstract,
        "protected": "protected" in mods.split(),
    }


class SignatureErasureTests(unittest.TestCase):
    def test_generic_nested_and_wildcard_types_erase(self):
        self.assertEqual(sig.erased("java.util.Map<java.lang.String, ? extends T>", {"T"}), "Any")
        self.assertEqual(sig.erased("T[]", {"T"}), "Any[]")
        self.assertEqual(sig.erased("java.lang.String...", set()), "String[]")
        self.assertEqual(sig.erased("int[][]", set()), "int[][]")
        self.assertEqual(sig.erased("java.lang.String", set()), "String")

    def test_generic_method_keeps_metadata_and_emits_erased_stub(self):
        m = member(
            "copy",
            ("java.util.List<? extends T>", "T[]"),
            "T[]",
            "<T extends java.lang.Number>",
            throws=("java.io.IOException",),
        )
        rendered = sig.render("example.Copy", m, {"E"})
        self.assertIn('@generic("<T extends java.lang.Number>")', rendered)
        self.assertIn("Any[] copy(Any arg0, Any[] arg1)", rendered)
        self.assertIn("throws java.io.IOException", rendered)
        self.assertIn("UnsupportedOperationException", rendered)
        self.assertNotIn("java.util.List", rendered)
        self.assertNotIn("abstract ", rendered)

    def test_abstract_method_remains_fail_loud(self):
        rendered = sig.render(
            "example.Contract",
            member("read", ("int",), "java.lang.String", mods="public abstract", abstract=True),
        )
        self.assertIn("AbstractMethodError", rendered)
        self.assertNotIn("abstract ", rendered)

    def test_constructor_generic_parameters_are_erased(self):
        rendered = sig.render_ctor(
            "example.Box",
            "Box",
            member("<init>", ("T", "java.util.List<? super T>"), tparams="<T extends java.lang.Number>"),
            {"E"},
        )
        self.assertIn('@generic("<T extends java.lang.Number>")', rendered)
        self.assertIn("Box(Any arg0, Any arg1)", rendered)
        self.assertIn("UnsupportedOperationException", rendered)

    def test_signature_index_retains_erasure_collisions(self):
        record = {
            "tparams": "<E>",
            "members": [
                member("put", ("java.util.List<E>",), "void"),
                member("put", ("java.util.Set<E>",), "void"),
                member("put", ("int",), "void"),
            ],
        }
        index = sig.build_signature_index(record)
        self.assertEqual(len(index[("put", ("Any",))]), 2)
        self.assertEqual(len(index[("put", ("int",))]), 1)

    def test_inherited_index_walks_superclasses_and_interfaces(self):
        api = {
            "Child": {"kind": "class", "super": "Base", "interfaces": ["First"]},
            "Base": {"kind": "class", "super": None, "interfaces": []},
            "First": {"kind": "interface", "super": None, "interfaces": ["Second"]},
            "Second": {"kind": "interface", "super": None, "interfaces": []},
        }
        impl = {
            "Base": "  public void base(int value) { }",
            "First": "  public default void defaulted(int value) { }\n  public static void helper(int value) { }",
            "Second": "  public void inherited(int value) { }",
        }
        found = sig.inherited_implementation_methods("Child", api, impl)
        self.assertIn("base", found)
        self.assertIn("defaulted", found)
        self.assertIn("inherited", found)
        self.assertNotIn("helper", found)

    def test_constructor_assignability_distinguishes_same_arity(self):
        self.assertFalse(sig.accepts(("R",), ("I",)))
        self.assertTrue(sig.accepts(("A",), ("I",)))
        self.assertTrue(sig.accepts(("D",), ("I",)))


if __name__ == "__main__":
    unittest.main()
