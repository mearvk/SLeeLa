package com.mearvk.sleela.skya;

import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.*;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

import java.io.File;
import java.io.IOException;
import java.util.Properties;

public final class SkyaClientApp extends Application {
    private final Label status = new Label("Skya: ready");
    private final Label connection = new Label("Connection: not connected");
    private final Label selectedFile = new Label("No file selected");
    private final TextField host = new TextField("localhost");
    private final TextField port = new TextField("8443");
    private final TextField room = new TextField("lobby");
    private Properties config = new Properties();
    private String configName = "default";

    @Override
    public void start(Stage stage) {
        stage.setTitle("Skya — SLeeLa Telephony");
        stage.setMinWidth(980);
        stage.setMinHeight(700);

        MenuBar menuBar = buildMenuBar(stage);
        ImageView logo = logoView();
        Label title = new Label("Skya");
        title.setStyle("-fx-font-size: 22px; -fx-font-weight: bold;");
        Label subtitle = new Label("SLeeLa Telephony Client");
        subtitle.setStyle("-fx-font-size: 12px;");
        VBox brand = new VBox(2, title, subtitle);
        HBox header = new HBox(18, logo, brand);
        header.setAlignment(Pos.CENTER_LEFT);
        header.setPadding(new Insets(10, 4, 14, 4));

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

        host.setPrefColumnCount(14);
        port.setPrefColumnCount(6);
        room.setPrefColumnCount(10);
        HBox connectionBar = new HBox(8, new Label("Host"), host, new Label("Port"), port,
                new Label("Room"), room, connect, disconnect);
        connectionBar.setAlignment(Pos.CENTER_LEFT);

        VBox top = new VBox(menuBar, header, connectionBar);
        top.setPadding(new Insets(0, 12, 10, 12));

        TabPane tabs = new TabPane(chatTab(), videoTab(), audioTab(), fileTab());
        tabs.setTabClosingPolicy(TabPane.TabClosingPolicy.UNAVAILABLE);

        BorderPane root = new BorderPane(tabs);
        root.setPadding(new Insets(0, 12, 12, 12));
        root.setTop(top);
        root.setBottom(new VBox(6, new Separator(), connection, status));
        BorderPane.setMargin(connection, new Insets(8, 0, 0, 0));

        stage.setScene(new Scene(root, 1100, 760));
        stage.show();
    }

    private MenuBar buildMenuBar(Stage stage) {
        Menu file = new Menu("File");
        MenuItem load = new MenuItem("Load Config…");
        MenuItem save = new MenuItem("Save Config");
        MenuItem saveAs = new MenuItem("Save Config As…");
        MenuItem delete = new MenuItem("Delete Old Config…");
        MenuItem exit = new MenuItem("Exit");
        load.setOnAction(e -> loadConfig(stage));
        save.setOnAction(e -> saveConfig(stage, false));
        saveAs.setOnAction(e -> saveConfig(stage, true));
        delete.setOnAction(e -> deleteConfig(stage));
        exit.setOnAction(e -> stage.close());
        file.getItems().addAll(load, save, saveAs, new SeparatorMenuItem(), delete,
                new SeparatorMenuItem(), exit);

        Menu settings = new Menu("Settings");
        MenuItem chat = new MenuItem("Chat Settings…");
        MenuItem audio = new MenuItem("Audio Settings…");
        MenuItem video = new MenuItem("Video Settings…");
        MenuItem files = new MenuItem("File Transfer Settings…");
        chat.setOnAction(e -> settingsDialog(stage, "Chat", "chat"));
        audio.setOnAction(e -> settingsDialog(stage, "Audio", "audio"));
        video.setOnAction(e -> settingsDialog(stage, "Video", "video"));
        files.setOnAction(e -> settingsDialog(stage, "File Transfer", "file"));
        settings.getItems().addAll(chat, audio, video, files);

        Menu configMenu = new Menu("Config");
        MenuItem edit = new MenuItem("Edit Current Config…");
        MenuItem folder = new MenuItem("Open Config Folder");
        edit.setOnAction(e -> settingsDialog(stage, "Configuration", "general"));
        folder.setOnAction(e -> {
            try {
                Runtime.getRuntime().exec(new String[]{"xdg-open", SkyaConfigManager.directory().toString()});
            } catch (Exception ignored) {
                status.setText("Config folder: " + SkyaConfigManager.directorySafe());
            }
        });
        configMenu.getItems().addAll(edit, folder);

        Menu help = new Menu("Help");
        MenuItem about = new MenuItem("About Skya");
        about.setOnAction(e -> {
            Alert a = new Alert(Alert.AlertType.INFORMATION);
            a.initOwner(stage);
            a.setTitle("About Skya");
            a.setHeaderText("Skya — SLeeLa Telephony");
            a.setContentText("Guia™ JavaFX client surface\nConfiguration: " + configName);
            a.showAndWait();
        });
        help.getItems().add(about);
        return new MenuBar(file, settings, configMenu, help);
    }

    private void loadConfig(Stage stage) {
        try {
            var names = SkyaConfigManager.names();
            if (names.isEmpty()) { status.setText("Config: no saved configurations"); return; }
            ChoiceDialog<String> d = new ChoiceDialog<>(configName, names);
            d.setTitle("Load Config");
            d.setHeaderText("Choose a Skya configuration");
            d.showAndWait().ifPresent(name -> {
                try {
                    configName = name;
                    config = SkyaConfigManager.load(name);
                    host.setText(config.getProperty("host", "localhost"));
                    port.setText(config.getProperty("port", "8443"));
                    room.setText(config.getProperty("room", "lobby"));
                    status.setText("Config loaded: " + name);
                } catch (IOException ex) { SkyaConfigManager.error(stage, ex.getMessage()); }
            });
        } catch (IOException | RuntimeException ex) { SkyaConfigManager.error(stage, ex.getMessage()); }
    }

    private void saveConfig(Stage stage, boolean chooseName) {
        try {
            if (chooseName) {
                String name = SkyaConfigManager.promptName(stage, configName);
                if (name == null) return;
                configName = name;
            }
            config.setProperty("host", host.getText());
            config.setProperty("port", port.getText());
            config.setProperty("room", room.getText());
            SkyaConfigManager.save(configName, config);
            status.setText("Config saved: " + configName);
        } catch (IOException | RuntimeException ex) { SkyaConfigManager.error(stage, ex.getMessage()); }
    }

    private void deleteConfig(Stage stage) {
        try {
            var names = SkyaConfigManager.names();
            if (names.isEmpty()) { status.setText("Config: no saved configurations"); return; }
            ChoiceDialog<String> d = new ChoiceDialog<>(configName, names);
            d.setTitle("Delete Old Config");
            d.setHeaderText("Delete a saved configuration");
            d.showAndWait().ifPresent(name -> {
                try {
                    SkyaConfigManager.delete(name);
                    status.setText("Config deleted: " + name);
                } catch (IOException | RuntimeException ex) { SkyaConfigManager.error(stage, ex.getMessage()); }
            });
        } catch (IOException | RuntimeException ex) { SkyaConfigManager.error(stage, ex.getMessage()); }
    }

    private void settingsDialog(Stage stage, String title, String prefix) {
        TextInputDialog d = new TextInputDialog(config.getProperty(prefix + ".settings", ""));
        d.setTitle("Skya " + title + " Settings");
        d.setHeaderText(title + " configuration");
        d.setContentText("Value:");
        d.showAndWait().ifPresent(v -> {
            config.setProperty(prefix + ".settings", v);
            status.setText(title + " settings changed");
        });
    }

    private ImageView logoView() {
        var url = getClass().getResource("/images/skya-logo-blue.jpeg");
        Image image = url == null
                ? new Image("https://raw.githubusercontent.com/mearvk/SLeeLa/master/images/skya-logo-blue.jpeg", true)
                : new Image(url.toExternalForm(), true);
        ImageView view = new ImageView(image);
        view.setFitWidth(180);
        view.setFitHeight(72);
        view.setPreserveRatio(true);
        view.setSmooth(true);
        return view;
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
        VBox box = new VBox(10, new Label("Chat"), new Label("Peer and room messaging."),
                messages, compose);
        VBox.setVgrow(messages, Priority.ALWAYS);
        box.setPadding(new Insets(18));
        return new Tab("Chat", box);
    }

    private Tab videoTab() {
        Button start = new Button("Start Video"), camera = new Button("Camera"), stop = new Button("Stop Video");
        start.setOnAction(e -> status.setText("Video: call requested"));
        camera.setOnAction(e -> status.setText("Video: camera requested"));
        stop.setOnAction(e -> status.setText("Video: stopped"));
        VBox box = new VBox(12, new Label("Video"),
                new Label("Video calling, camera and display controls."),
                new HBox(8, start, camera, stop), new Separator(),
                new Label("Codec: automatic"),
                new Label("Peer media: ready for negotiated transport"));
        box.setPadding(new Insets(18));
        return new Tab("Video", box);
    }

    private Tab audioTab() {
        Button start = new Button("Start Audio"), mute = new Button("Mute"), stop = new Button("Stop Audio");
        start.setOnAction(e -> status.setText("Audio: call requested"));
        mute.setOnAction(e -> status.setText("Audio: muted"));
        stop.setOnAction(e -> status.setText("Audio: stopped"));
        VBox box = new VBox(12, new Label("Audio"),
                new Label("Voice calling, microphone and playback controls."),
                new HBox(8, start, mute, stop), new Separator(),
                new Label("Codec: automatic"),
                new Label("Peer media: ready for negotiated transport"));
        box.setPadding(new Insets(18));
        return new Tab("Audio", box);
    }

    private Tab fileTab() {
        Button choose = new Button("Choose File"), send = new Button("Send File"), cancel = new Button("Cancel Transfer");
        choose.setOnAction(e -> {
            FileChooser chooser = new FileChooser();
            chooser.setTitle("Select a file for Skya transfer");
            File file = chooser.showOpenDialog(null);
            if (file != null) {
                selectedFile.setText(file.getAbsolutePath());
                status.setText("File selected: " + file.getName());
            }
        });
        send.setOnAction(e -> status.setText("No file selected".equals(selectedFile.getText())
                ? "File transfer: select a file first"
                : "File transfer: requested for " + selectedFile.getText()));
        cancel.setOnAction(e -> status.setText("File transfer: cancelled"));
        VBox box = new VBox(12, new Label("File Transfer"),
                new Label("Select and manage files for peer transfer."), selectedFile,
                new HBox(8, choose, send, cancel), new Separator(),
                new Label("Transfer state: idle"));
        box.setPadding(new Insets(18));
        return new Tab("Files", box);
    }

    public static void main(String[] args) { launch(args); }
}
