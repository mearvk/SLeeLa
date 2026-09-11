package com.mearvk.sleela.gui;

/** Minimal launcher for the Swing/JavaFX GUI boundary. */
public final class SleelaGuiDemo {
    private SleelaGuiDemo() {
    }

    public static void main(String[] args) {
        String backend = args.length == 0 ? "swing" : args[0];
        SleelaGui gui = SleelaGui.create(backend);
        gui.show("SLeeLa GUI", 640, 400);
        gui.setText("SLeeLa is running on " + backend);
        gui.onAction(() -> gui.setText("SLeeLa action dispatched"));
    }
}
