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
 *   standard-N    idea-vm-001..010 under virtual-machine/vm-version/NNN. The CLI can select them but cannot boot a
 *                 program on them (`sleela vm run N` is refused), so they are listed and never runnable here.
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
    /** Premium only: the generation binary the CLI boots, relative to the SLeeLa home. */
    public final String binaryPath;
    /** null when the VM can run on this server, else why not. */
    public final String unavailable;

    private VmChoice(Tier tier, int number, String id, String vmName, String label, String binaryPath, String unavailable) {
        this.tier = tier; this.number = number; this.id = id; this.vmName = vmName; this.label = label;
        this.binaryPath = binaryPath; this.unavailable = unavailable;
    }

    public String getId() { return id; }
    public String getTier() { return tier.name(); }
    public String getVmName() { return vmName; }
    public String getLabel() { return label; }
    public String getUnavailable() { return unavailable; }
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
        all.add(new VmChoice(Tier.NATIVE, 0, DEFAULT_ID, "native", "default", null, null));
        for (int n = 1; n <= STANDARD.length; n++) {
            all.add(new VmChoice(Tier.STANDARD, n, "standard-" + n, String.format("idea-vm-%03d", n), STANDARD[n - 1], null,
                    "Standard VMs cannot run programs yet: the SLeeLa CLI can select them but not boot a program on them."));
        }
        for (int n = 1; n <= PREMIUM.length; n++) {
            String bin = String.format("sleela-virtual-machine/%03d/vm-build/slvm-%02d", n, n);
            String why = null;
            if (home == null) why = "The SLeeLa source tree was not found.";
            else if (!Files.isExecutable(home.resolve(bin))) why = "Not built on this server (" + bin + " is missing; run make premium-vms).";
            all.add(new VmChoice(Tier.PREMIUM, n, "premium-" + n, "slvm-" + n, PREMIUM[n - 1], bin, why));
        }
        return Collections.unmodifiableList(all);
    }

    /** Looks an id up in the catalog; unknown or missing ids give the native VM. */
    public static VmChoice find(List<VmChoice> catalog, String id) {
        if (id != null) for (VmChoice v : catalog) if (v.id.equals(id)) return v;
        return catalog.get(0);
    }
}
