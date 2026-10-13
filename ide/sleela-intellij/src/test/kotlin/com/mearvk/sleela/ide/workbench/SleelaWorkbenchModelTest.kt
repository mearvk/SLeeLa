package com.mearvk.sleela.ide.workbench

import com.mearvk.sleela.ide.Severity
import org.junit.jupiter.api.Assertions.assertEquals
import org.junit.jupiter.api.Assertions.assertFalse
import org.junit.jupiter.api.Assertions.assertTrue
import org.junit.jupiter.api.Test

/**
 * Verifies the host-side implementation of the lib/ide-workbench contracts:
 * editor dirty tracking, the tab group's 1..14 watch-set bound, diagnostics'
 * failure rule, and the .sst/.sleela language routing.
 */
class SleelaWorkbenchModelTest {

    @Test
    fun editorTracksDirtyAndSavedState() {
        val ed = WorkbenchEditor("doc", "/p/a.sleela", "sleela")
        ed.loadText("class A {}", "100:9")
        assertFalse(ed.dirty)
        ed.setText("class A {}")          // same text -> not dirty
        assertFalse(ed.dirty)
        ed.setText("class A { int x; }")  // changed -> dirty
        assertTrue(ed.dirty)
        ed.markSaved("200:17")
        assertFalse(ed.dirty)
        assertTrue(ed.staleAgainst("300:20")) // disk moved past the saved revision
    }

    @Test
    fun tabGroupOpensFocusesAndClosesWithoutDuplicates() {
        val tabs = WorkbenchTabGroup()
        tabs.open(WorkbenchEditor("a", "/p/a.sleela", "sleela"))
        tabs.open(WorkbenchEditor("b", "/p/b.sst", "sst"))
        assertEquals(2, tabs.size)
        // Re-opening an existing id focuses, does not duplicate.
        val idx = tabs.open(WorkbenchEditor("a", "/p/a.sleela", "sleela"))
        assertEquals(2, tabs.size)
        assertEquals(idx, tabs.activeIndex)
        tabs.close(tabs.activeIndex)
        assertEquals(1, tabs.size)
    }

    @Test
    fun watchSetStaysWithinGuiBound() {
        val tabs = WorkbenchTabGroup()
        assertEquals(0, tabs.watchSetSize()) // nothing open
        for (i in 0 until 20) {
            tabs.open(WorkbenchEditor("doc$i", "/p/f$i.sleela", "sleela"))
        }
        assertEquals(20, tabs.size)
        // 20 open, but the OS watch registration is capped at 14 (lib/gui bound).
        assertEquals(14, tabs.watchSetSize())
        assertTrue(tabs.watchSetWithinGuiBound())
    }

    @Test
    fun diagnosticsEnforceFailureRule() {
        val d = WorkbenchDiagnostics()
        assertTrue(d.compileSucceeded())
        d.publish("doc", Severity.WARNING, 1, 1, "unused")
        assertTrue(d.compileSucceeded())          // warnings do not fail
        d.publish("doc", Severity.ERROR, 3, 5, "boom")
        assertFalse(d.compileSucceeded())          // an error fails the run
        assertEquals("1 error(s), 1 warning(s)", d.statusLine())
        d.clearDocument("doc")
        assertEquals(0, d.size)
        assertTrue(d.compileSucceeded())
    }

    @Test
    fun languageRoutingClassifiesSources() {
        assertEquals("sst", WorkbenchLanguage.forPath("/p/main.sst"))
        assertEquals("sleela", WorkbenchLanguage.forPath("/p/util.sleela"))
        assertEquals("cpp", WorkbenchLanguage.forPath("/p/core.cpp"))
        assertEquals("text", WorkbenchLanguage.forPath("/p/notes.txt"))
        assertTrue(WorkbenchLanguage.isSource("/p/main.sst"))
        assertFalse(WorkbenchLanguage.isSource("/p/notes.txt"))
    }
}
