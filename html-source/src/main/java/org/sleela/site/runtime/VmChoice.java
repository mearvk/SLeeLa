package org.sleela.site.runtime;

import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * One virtual machine a visitor can pick on /runtime. Mirrors the three tiers the sleela CLI knows
 * (impl/frontend/driver.cpp vmCmd, .kiro/steering/vm-tiers.md, virtual-machine/USER-GUIDE.md):
 *
 *   native        the Native VM in impl/core, used by plain `sleela run`. Always the default.
 *   standard-N    idea-vm-001..010 under virtual-machine/vm-version/NNN. They are smoke/demo programs, not engines: the
 *                 CLI cannot boot a program on them. When the server has built
 *                 virtual-machine/vm-version/build/NNN/idea-vm-NNN (make standard-vms) the visitor can select one and
 *                 the page runs the visitor program on the native core they are built on, then that VM's own
 *                 self-check, and shows both outputs.
 *   premium-N     slvm-1..11 under sleela-virtual-machine/NNN. Real VMs, run with `sleela vm run premium N`. They are
 *                 runnable only when the server has built sleela-virtual-machine/NNN/vm-build/slvm-NN (make premium-vms);
 *                 the sandbox is read-only, so the CLI cannot build one on demand during a visitor run.
 *
 * Visitors send only the id; every id is looked up in this fixed table, so nothing a visitor types reaches the command line.
 */
public final class VmChoice {

    public enum Tier { NATIVE, STANDARD, PREMIUM }

    private static final String[] STANDARD = {
        "ground floor", "memory hardening", "typed results", "telemetry", "gc quality",
        "dispatch profiler", "capability enforcement", "platform parity", "audit trail", "consolidated",
    };
    /** One line per standard VM: what it adds over the previous version (from virtual-machine/vm-version/NNN/VERSION.md). */
    private static final String[] STANDARD_WHY = {
        "Baseline. Adds the user-level ground floor: one config surface (memory limit, thread cap, verbosity, capability bits), uniform diagnostics and a status banner.",
        "Improves on 001 with a memory-pressure layer: warning and critical watermarks, a NORMAL/PRESSURE/CRITICAL/LIMIT condition, and one notice per change.",
        "Improves on 002 with one typed result shared by the whole VM (stable result codes plus subsystem and message) instead of raw status integers.",
        "Improves on 003 with lock-free telemetry: VM-wide counters and per-table usage gauges with high-water marks.",
        "Improves on 004 with garbage-collection quality: reclaim ratio and collection rate give a HEALTHY/BUSY/PRESSURE/THRASH condition and pacing advice.",
        "Improves on 005 with a dispatch profiler: per-opcode-family counts and time, a hot-family query and a ranked report.",
        "Improves on 006 with capability enforcement: one choke point turns the capability mask into real allow/deny decisions that are reported and counted.",
        "Improves on 007 with platform parity: detects the host OS and reports, per capability, whether the OS supports it and the config permits it.",
        "Improves on 008 with an audit trail: a 256-entry ring of structured events, each linked by a running hash so the order is tamper-evident.",
        "Capstone. Wires versions 001 to 009 behind one facade (init, poll, permit, report, shutdown) so the whole stack runs in dependency order.",
    };
    private static final String[] PREMIUM_WHY = {
        "Dedicated VM foundation: broker, capabilities, security supervisor and OS boundary.",
        "Cryptographic and VM-link evolution over the Foundation generation.",
        "Verified execution: isolation, deterministic policy and supply-chain integrity.",
        "Authenticated and distributed execution.",
        "Reproducible, policy-verifiable and fault-tolerant execution.",
        "Continuously verified, migratable execution: lineage, attestation and leases.",
        "Hardened management and recovery: logging, health, checkpoints and resource limits.",
        "Supervised execution: admission, leases, transactions and audit.",
        "Portable, capability-controlled filesystem substrate with OS adapters.",
        "Verified storage execution and an execution-assurance layer.",
        "Filesystem-module host above the generation 9 and 10 storage contracts.",
    };
    private static final String[] PREMIUM = {
        "Foundation", "Operator", "Specialist", "Supervisor", "Manager", "Director",
        "Administrator", "Executive", "Authority", "Principal", "Sovereign",
    };

    public static final String DEFAULT_ID = "native";

    public final Tier tier;
    public final int number;
    public final String id;
    public final String vmName;
    public final String label;
    /** Premium: the generation binary the CLI boots. Standard: the built self-check program. Relative to the SLeeLa home. */
    public final String binaryPath;
    /** One line on why this VM differs from, or improves on, the others. */
    public final String why;
    /** null when the VM can run on this server, else why not. */
    public final String unavailable;

    private VmChoice(Tier tier, int number, String id, String vmName, String label, String binaryPath, String unavailable, String why) {
        this.tier = tier; this.number = number; this.id = id; this.vmName = vmName; this.label = label;
        this.binaryPath = binaryPath; this.unavailable = unavailable; this.why = why;
    }

    public String getId() { return id; }
    public String getTier() { return tier.name(); }
    public String getVmName() { return vmName; }
    public String getLabel() { return label; }
    public String getUnavailable() { return unavailable; }
    public String getWhy() { return why; }
    public boolean isAvailable() { return unavailable == null; }

    /** Text shown to the visitor, e.g. "Premium 6: slvm-6 (Director)". */
    public String getDisplay() {
        switch (tier) {
            case NATIVE: return "Native VM (impl/core, the default engine)";
            case STANDARD: return "Standard " + number + ": " + vmName + " (" + label + ")";
            default: return "Premium " + number + ": " + vmName + " (" + label + ")";
        }
    }

    /** Every VM, native first, with availability checked against this server. */
    public static List<VmChoice> catalog(Path home) {
        List<VmChoice> all = new ArrayList<>();
        all.add(new VmChoice(Tier.NATIVE, 0, DEFAULT_ID, "native", "default", null, null,
                "The default engine in impl/core: runs your program directly. Every other VM is layered on or built beside it."));
        for (int n = 1; n <= STANDARD.length; n++) {
            String bin = String.format("virtual-machine/vm-version/build/%03d/idea-vm-%03d", n, n);
            String why = null;
            if (home == null) why = "The SLeeLa source tree was not found.";
            else if (!Files.isExecutable(home.resolve(bin))) why = "Not built on this server (" + bin + " is missing; run make standard-vms).";
            all.add(new VmChoice(Tier.STANDARD, n, "standard-" + n, String.format("idea-vm-%03d", n), STANDARD[n - 1], bin, why, STANDARD_WHY[n - 1]));
        }
        for (int n = 1; n <= PREMIUM.length; n++) {
            String bin = String.format("sleela-virtual-machine/%03d/vm-build/slvm-%02d", n, n);
            String why = null;
            if (home == null) why = "The SLeeLa source tree was not found.";
            else if (!Files.isExecutable(home.resolve(bin))) why = "Not built on this server (" + bin + " is missing; run make premium-vms).";
            all.add(new VmChoice(Tier.PREMIUM, n, "premium-" + n, "slvm-" + n, PREMIUM[n - 1], bin, why, PREMIUM_WHY[n - 1]));
        }
        return Collections.unmodifiableList(all);
    }

    /** Looks an id up in the catalog; unknown or missing ids give the native VM. */
    public static VmChoice find(List<VmChoice> catalog, String id) {
        if (id != null) for (VmChoice v : catalog) if (v.id.equals(id)) return v;
        return catalog.get(0);
    }
}
