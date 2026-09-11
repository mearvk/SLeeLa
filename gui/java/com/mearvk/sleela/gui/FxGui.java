package com.mearvk.sleela.gui;

import javafx.application.Platform;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

/** JavaFX backend for the SLeeLa GUI boundary. */
public final class FxGui implements SleelaGui {
    private Stage stage;
    private Label label;
    private Button actionButton;

    @Override
    public void show(String title, int width, int height) {
        runOnFx(() -> {
            stage = new Stage();
            stage.setTitle(title);
            label = new Label("SLeeLa");
            actionButton = new Button("Action");
            VBox root = new VBox(10.0, label, actionButton);
            root.setPrefSize(width, height);
            Scene scene = new Scene(root);
            stage.setScene(scene);
            stage.show();
        });
    }

    @Override
    public void setText(String text) {
        runOnFx(() -> {
            if (label != null) label.setText(text);
        });
    }

    @Override
    public void onAction(Runnable action) {
        if (action == null) throw new NullPointerException("action");
        runOnFx(() -> {
            if (actionButton != null) actionButton.setOnAction(event -> action.run());
        });
    }

    @Override
    public void close() {
        runOnFx(() -> {
            if (stage != null) stage.close();
            stage = null;
        });
    }

    private static void runOnFx(Runnable task) {
        if (Platform.isFxApplicationThread()) task.run();
        else Platform.runLater(task);
    }
}
