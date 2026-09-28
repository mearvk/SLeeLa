package com.mearvk.sleela.gui;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;
import java.awt.BorderLayout;

/** Swing backend for the SLeeLa GUI boundary. */
public final class SwingGui implements SleelaGui {
    private JFrame frame;
    private JLabel label;
    private JButton actionButton;

    @Override
    public void show(String title, int width, int height) {
        runOnEdt(() -> {
            frame = new JFrame(title);
            frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
            frame.setSize(width, height);
            frame.setLocationByPlatform(true);

            label = new JLabel("SLeeLa", JLabel.CENTER);
            actionButton = new JButton("Action");
            JPanel panel = new JPanel(new BorderLayout());
            panel.add(label, BorderLayout.CENTER);
            panel.add(actionButton, BorderLayout.SOUTH);
            frame.setContentPane(panel);
            frame.setVisible(true);
        });
    }

    @Override
    public void setText(String text) {
        runOnEdt(() -> {
            if (label != null) label.setText(text);
        });
    }

    @Override
    public void onAction(Runnable action) {
        if (action == null) throw new NullPointerException("action");
        runOnEdt(() -> {
            if (actionButton != null) actionButton.addActionListener(event -> action.run());
        });
    }

    @Override
    public void close() {
        runOnEdt(() -> {
            if (frame != null) frame.dispose();
            frame = null;
        });
    }

    private static void runOnEdt(Runnable task) {
        if (SwingUtilities.isEventDispatchThread()) task.run();
        else SwingUtilities.invokeLater(task);
    }
}
