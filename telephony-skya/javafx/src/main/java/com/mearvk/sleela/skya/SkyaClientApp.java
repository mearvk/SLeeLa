package com.mearvk.sleela.skya;

import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.*;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

import java.io.File;

public final class SkyaClientApp extends Application {
    private final Label status = new Label("Client: ready");
    private final Label selectedFile = new Label("No file selected");

    @Override
    public void start(Stage stage) {
        stage.setTitle("Skya — Audio / Video / File");
        stage.setMinWidth(820);
        stage.setMinHeight(560);

        TabPane tabs = new TabPane();
        tabs.getTabs().add(audioTab());
        tabs.getTabs().add(videoTab());
        tabs.getTabs().add(fileTab());

        BorderPane root = new BorderPane(tabs);
        root.setPadding(new Insets(12));
        root.setBottom(new VBox(6, new Separator(), status));
        BorderPane.setMargin(status, new Insets(8, 0, 0, 0));

        stage.setScene(new Scene(root, 900, 600));
        stage.show();
    }

    private Tab audioTab() {
        Button start = new Button("Start Audio");
        Button mute = new Button("Mute");
        Button stop = new Button("Stop Audio");
        start.setOnAction(e -> status.setText("Audio: capture/playback requested"));
        mute.setOnAction(e -> status.setText("Audio: muted"));
        stop.setOnAction(e -> status.setText("Audio: stopped"));

        VBox box = new VBox(12,
                new Label("Audio"),
                new Label("Audio capture, playback, codec and connection controls."),
                new HBox(8, start, mute, stop),
                new Separator(),
                new Label("Codec: automatic"),
                new Label("Connection: not connected"));
        box.setPadding(new Insets(18));
        return new Tab("Audio", box);
    }

    private Tab videoTab() {
        Button start = new Button("Start Video");
        Button stop = new Button("Stop Video");
        start.setOnAction(e -> status.setText("Video: capture/display requested"));
        stop.setOnAction(e -> status.setText("Video: stopped"));

        VBox box = new VBox(12,
                new Label("Video"),
                new Label("Video capture, display, codec and connection controls."),
                new HBox(8, start, stop),
                new Separator(),
                new Label("Codec: automatic"),
                new Label("Connection: not connected"));
        box.setPadding(new Insets(18));
        return new Tab("Video", box);
    }

    private Tab fileTab() {
        Button choose = new Button("Choose File");
        Button send = new Button("Send File");
        Button cancel = new Button("Cancel Transfer");

        choose.setOnAction(e -> {
            FileChooser chooser = new FileChooser();
            chooser.setTitle("Select a file for Skya transfer");
            File file = chooser.showOpenDialog(null);
            if (file != null) {
                selectedFile.setText(file.getAbsolutePath());
                status.setText("File selected: " + file.getName());
            }
        });
        send.setOnAction(e -> {
            if ("No file selected".equals(selectedFile.getText())) {
                status.setText("File transfer: select a file first");
            } else {
                status.setText("File transfer: requested for " + selectedFile.getText());
            }
        });
        cancel.setOnAction(e -> status.setText("File transfer: cancelled"));

        VBox box = new VBox(12,
                new Label("File Transfer"),
                new Label("Select and manage files for future Skya peer transfer."),
                selectedFile,
                new HBox(8, choose, send, cancel),
                new Separator(),
                new Label("Transfer state: idle"));
        box.setPadding(new Insets(18));
        return new Tab("Files", box);
    }

    public static void main(String[] args) {
        launch(args);
    }
}
