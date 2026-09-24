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
    private final Label status = new Label("Skya: ready");
    private final Label connection = new Label("Connection: not connected");
    private final Label selectedFile = new Label("No file selected");

    @Override
    public void start(Stage stage) {
        stage.setTitle("Skya — SLeeLa Telephony");
        stage.setMinWidth(900);
        stage.setMinHeight(620);

        TextField host = new TextField("localhost");
        host.setPrefColumnCount(16);
        TextField port = new TextField("8443");
        port.setPrefColumnCount(6);
        TextField room = new TextField("lobby");
        room.setPrefColumnCount(10);
        Button connect = new Button("Connect");
        Button disconnect = new Button("Disconnect");

        connect.setOnAction(e -> {
            connection.setText("Connection: requested to " + host.getText() + ":" + port.getText()
                    + " / " + room.getText());
            status.setText("Skya: connection requested");
        });
        disconnect.setOnAction(e -> {
            connection.setText("Connection: disconnected");
            status.setText("Skya: disconnected");
        });

        HBox connectionBar = new HBox(8,
                new Label("Host"), host,
                new Label("Port"), port,
                new Label("Room"), room,
                connect, disconnect);
        connectionBar.setPadding(new Insets(0, 0, 10, 0));

        TabPane tabs = new TabPane();
        tabs.getTabs().add(chatTab());
        tabs.getTabs().add(videoTab());
        tabs.getTabs().add(audioTab());
        tabs.getTabs().add(fileTab());

        BorderPane root = new BorderPane(tabs);
        root.setPadding(new Insets(12));
        root.setTop(connectionBar);
        root.setBottom(new VBox(6, new Separator(), connection, status));
        BorderPane.setMargin(connection, new Insets(8, 0, 0, 0));

        stage.setScene(new Scene(root, 1000, 680));
        stage.show();
    }

    private Tab chatTab() {
        ListView<String> messages = new ListView<>();
        messages.getItems().add("Skya Chat — conversation ready");

        TextField message = new TextField();
        message.setPromptText("Type a message...");
        Button send = new Button("Send");
        Button clear = new Button("Clear");

        Runnable sendMessage = () -> {
            String text = message.getText().trim();
            if (text.isEmpty()) return;
            messages.getItems().add("You: " + text);
            message.clear();
            status.setText("Chat: message queued");
        };
        send.setOnAction(e -> sendMessage.run());
        message.setOnAction(e -> sendMessage.run());
        clear.setOnAction(e -> messages.getItems().clear());

        HBox compose = new HBox(8, message, send, clear);
        HBox.setHgrow(message, Priority.ALWAYS);

        VBox box = new VBox(10,
                new Label("Chat"),
                new Label("Peer and room messaging."),
                messages,
                compose);
        VBox.setVgrow(messages, Priority.ALWAYS);
        box.setPadding(new Insets(18));
        return new Tab("Chat", box);
    }

    private Tab videoTab() {
        Button start = new Button("Start Video");
        Button camera = new Button("Camera");
        Button stop = new Button("Stop Video");

        start.setOnAction(e -> status.setText("Video: call requested"));
        camera.setOnAction(e -> status.setText("Video: camera requested"));
        stop.setOnAction(e -> status.setText("Video: stopped"));

        VBox box = new VBox(12,
                new Label("Video"),
                new Label("Video calling, camera and display controls."),
                new HBox(8, start, camera, stop),
                new Separator(),
                new Label("Codec: automatic"),
                new Label("Peer media: ready for negotiated transport"));
        box.setPadding(new Insets(18));
        return new Tab("Video", box);
    }

    private Tab audioTab() {
        Button start = new Button("Start Audio");
        Button mute = new Button("Mute");
        Button stop = new Button("Stop Audio");

        start.setOnAction(e -> status.setText("Audio: call requested"));
        mute.setOnAction(e -> status.setText("Audio: muted"));
        stop.setOnAction(e -> status.setText("Audio: stopped"));

        VBox box = new VBox(12,
                new Label("Audio"),
                new Label("Voice calling, microphone and playback controls."),
                new HBox(8, start, mute, stop),
                new Separator(),
                new Label("Codec: automatic"),
                new Label("Peer media: ready for negotiated transport"));
        box.setPadding(new Insets(18));
        return new Tab("Audio", box);
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
                new Label("Select and manage files for peer transfer."),
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
