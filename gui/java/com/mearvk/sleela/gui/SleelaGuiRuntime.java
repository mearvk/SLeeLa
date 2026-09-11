package com.mearvk.sleela.gui;

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Objects;

/**
 * Path 2: SLeeLa owns GUI intent and Java provides the desktop implementation.
 * A SLeeLa-side dispatcher can bind named operations to this host.
 */
public final class SleelaGuiRuntime {
    private final SleelaGui gui;
    private final Map<String, Runnable> actions = new LinkedHashMap<>();

    public SleelaGuiRuntime(SleelaGui gui) {
        this.gui = Objects.requireNonNull(gui, "gui");
    }

    public void window(String title, int width, int height) {
        gui.show(title, width, height);
    }

    public void text(String text) {
        gui.setText(text);
    }

    public void action(String name, Runnable operation) {
        actions.put(Objects.requireNonNull(name, "name"), Objects.requireNonNull(operation, "operation"));
        gui.onAction(() -> {
            Runnable selected = actions.get(name);
            if (selected != null) selected.run();
        });
    }

    public void close() {
        gui.close();
    }
}
