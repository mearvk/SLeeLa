package com.mearvk.sleela.skya;

import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.*;
import javafx.stage.Stage;
import java.io.*;
import java.util.Properties;

public final class SkyaApp extends Application {
    private Label engineStatus, circuitStatus, status;
    private final Properties config = new Properties();
    private String configName = "default";
    private final SkyaProtocolFooter protocolFooter = new SkyaProtocolFooter();
    private ListView<String> peers;

    @Override public void start(Stage stage) {
        stage.setTitle("Skya — SLeeLa Telephony — Admin");
        SkyaLog.info("admin", "SkyaApp (monitor) starting; Guia endpoint " + protocolFooter.endpoint());
        // The admin monitor is a Guia client of the SAME SLeeLa agent the user
        // client talks to (started by sleela-up.sh / client_monitor.sh). It
        // does not spawn its own circuit — doing so would collide on the Guia
        // control port. Every event the agent returns updates the monitor.
        protocolFooter.onEvent(this::onEvent);
        protocolFooter.sendAsync("GUI.CREATE");
        engineStatus = new Label("Client: querying agent at " + protocolFooter.endpoint());
        circuitStatus = new Label("Monitoring circuit: ready");
        status = new Label("Admin: ready");
        peers = new ListView<>();
        peers.getItems().addAll("Monitoring circuit initialized","Awaiting connected peers");

        ImageView logo = logoView();
        Label title = new Label("Skya Admin");
        title.setStyle("-fx-font-size: 22px; -fx-font-weight: bold;");
        Label subtitle = new Label("SLeeLa Telephony Monitor");
        VBox brand = new VBox(2, title, subtitle);
        HBox header = new HBox(18, logo, brand);
        header.setAlignment(Pos.CENTER_LEFT);
        header.setPadding(new Insets(10, 4, 14, 4));

        MenuBar menuBar = buildMenuBar(stage);
        Button start = new Button("Start");
        Button pause = new Button("Pause");
        Button stop = new Button("Stop");
        start.setOnAction(e -> protocolFooter.sendAsync("CIRCUIT.START"));
        pause.setOnAction(e -> protocolFooter.sendAsync("CIRCUIT.PAUSE"));
        stop.setOnAction(e -> protocolFooter.sendAsync("CIRCUIT.STOP"));

        VBox center = new VBox(10, new Label("Skya Client Monitor"), new Separator(),
            new Label("Connection / Peer Monitor"), peers, new Label("Engine"), engineStatus,
            new Label("SLeeLa .sleela circuit"), circuitStatus);
        BorderPane root = new BorderPane();
        root.setPadding(new Insets(0, 12, 12, 12));
        root.setTop(new VBox(menuBar, header, new HBox(8, start, pause, stop)));
        root.setCenter(center);
        root.setBottom(new VBox(6, new Separator(), status, protocolFooter.node()));
        stage.setScene(new Scene(root, 900, 620));
        stage.show();
        protocolFooter.start();
    }

    private MenuBar buildMenuBar(Stage stage) {
        Menu settings = new Menu("Settings");
        settings.getItems().addAll(
            menuItem(stage, "Chat Settings…", "chat"),
            menuItem(stage, "Audio Settings…", "audio"),
            menuItem(stage, "Video Settings…", "video"),
            menuItem(stage, "File Transfer Settings…", "file"));

        Menu configMenu = new Menu("Config");
        MenuItem load = new MenuItem("Load Config…");
        MenuItem save = new MenuItem("Save Config");
        MenuItem saveAs = new MenuItem("Save Config As…");
        MenuItem delete = new MenuItem("Delete Old Config…");
        load.setOnAction(e -> loadConfig(stage));
        save.setOnAction(e -> saveConfig(stage, false));
        saveAs.setOnAction(e -> saveConfig(stage, true));
        delete.setOnAction(e -> deleteConfig(stage));
        configMenu.getItems().addAll(load, save, saveAs, new SeparatorMenuItem(), delete);

        Menu service = new Menu("Service");
        MenuItem refresh = new MenuItem("Refresh Status");
        refresh.setOnAction(e -> protocolFooter.sendAsync("MONITOR.STATUS"));
        MenuItem restart = new MenuItem("Restart Monitor");
        restart.setOnAction(e -> { protocolFooter.sendAsync("CIRCUIT.STOP"); protocolFooter.sendAsync("CIRCUIT.START"); });
        service.getItems().addAll(refresh, restart);

        Menu help = new Menu("Help");
        MenuItem about = new MenuItem("About Skya Admin");
        about.setOnAction(e -> {
            Alert a = new Alert(Alert.AlertType.INFORMATION);
            a.initOwner(stage);
            a.setTitle("About Skya Admin");
            a.setHeaderText("Skya — SLeeLa Telephony — Admin");
            a.setContentText("Administrative monitoring surface\nConfiguration: " + configName);
            a.showAndWait();
        });
        help.getItems().add(about);
        return new MenuBar(settings, configMenu, service, help);
    }

    private MenuItem menuItem(Stage stage, String text, String key) {
        MenuItem item = new MenuItem(text);
        item.setOnAction(e -> {
            TextInputDialog d = new TextInputDialog(config.getProperty(key + ".settings", ""));
            d.setTitle("Skya Settings");
            d.setHeaderText(text);
            d.setContentText("Value:");
            d.showAndWait().ifPresent(v -> { config.setProperty(key + ".settings", v); status.setText(text + " changed"); });
        });
        return item;
    }

    private void loadConfig(Stage stage) {
        try {
            var names = SkyaConfigManager.names();
            if (names.isEmpty()) { status.setText("Config: no saved configurations"); return; }
            ChoiceDialog<String> d = new ChoiceDialog<>(configName, names);
            d.setTitle("Load Config");
            d.setHeaderText("Choose a Skya configuration");
            d.showAndWait().ifPresent(name -> {
                try { configName = name; config.clear(); config.putAll(SkyaConfigManager.load(name)); status.setText("Config loaded: " + name); }
                catch (IOException ex) { SkyaConfigManager.error(stage, ex.getMessage()); }
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
                try { SkyaConfigManager.delete(name); status.setText("Config deleted: " + name); }
                catch (IOException | RuntimeException ex) { SkyaConfigManager.error(stage, ex.getMessage()); }
            });
        } catch (IOException | RuntimeException ex) { SkyaConfigManager.error(stage, ex.getMessage()); }
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

    /**
     * Apply a Guia event from the agent to the monitor surface. CIRCUIT.*
     * commands and MONITOR.STATUS return a MONITOR.STATUS event carrying the
     * circuit state; CLIENT.OFFLINE means the SLeeLa agent is not running.
     */
    private void onEvent(String event) {
        status.setText("Guia: " + event);
        if (event.contains("CLIENT.OFFLINE")) {
            engineStatus.setText("Client: agent offline (" + protocolFooter.endpoint() + ")");
            circuitStatus.setText("Monitoring circuit: no agent — start it with sleela-up.sh / client_monitor.sh");
            return;
        }
        if (event.contains("state=running")) {
            engineStatus.setText("Client: running");
            circuitStatus.setText("Monitoring circuit: running");
            setPeers("Agent reachable", "Circuit running on " + protocolFooter.endpoint());
        } else if (event.contains("state=paused")) {
            engineStatus.setText("Client: paused");
            circuitStatus.setText("Monitoring circuit: paused");
        } else if (event.contains("state=stopped")) {
            engineStatus.setText("Client: stopped");
            circuitStatus.setText("Monitoring circuit: stopped");
        } else if (event.contains("CLIENT.CREATED")) {
            engineStatus.setText("Client: agent reachable");
        }
    }

    private void setPeers(String... lines) {
        peers.getItems().setAll(lines);
    }

    @Override public void stop() { SkyaLog.info("admin", "SkyaApp (monitor) stopping"); protocolFooter.stop(); }

    public static void main(String[] args){launch(args);}
}
