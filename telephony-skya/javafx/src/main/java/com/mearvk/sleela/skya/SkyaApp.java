package com.mearvk.sleela.skya;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.*;
import javafx.stage.Stage;
import java.io.*;
import java.nio.file.*;
import java.util.Properties;

public final class SkyaApp extends Application {
    private Label engineStatus, circuitStatus, status;
    private Process sleelaProcess;
    private volatile boolean monitoringPaused;
    private final Properties config = new Properties();
    private String configName = "default";
    private final SkyaProtocolFooter protocolFooter = new SkyaProtocolFooter();

    @Override public void start(Stage stage) {
        stage.setTitle("Skya — SLeeLa Telephony — Admin");
        engineStatus = new Label("Client: stopped");
        circuitStatus = new Label("Monitoring circuit: ready");
        status = new Label("Admin: ready");
        ListView<String> peers = new ListView<>();
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
        start.setOnAction(e -> startAdminTask());
        pause.setOnAction(e -> pauseAdminTask());
        stop.setOnAction(e -> stopAdminTask());

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
        MenuItem restart = new MenuItem("Restart Monitor");
        restart.setOnAction(e -> { stopAdminTask(); startAdminTask(); });
        service.getItems().add(restart);

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

    private synchronized void startAdminTask() {
        if (sleelaProcess != null && sleelaProcess.isAlive()) {
            monitoringPaused=false; engineStatus.setText("Client: running");
            circuitStatus.setText("Monitoring circuit: resumed"); return;
        }
        Path circuit=locateCircuit();
        if(circuit==null){circuitStatus.setText("Monitoring circuit: not found");return;}
        String command=System.getenv("SLEELA_COMMAND");
        if(command==null||command.isBlank()) command="sleela";
        try {
            sleelaProcess=new ProcessBuilder(command,"run",circuit.toString()).redirectErrorStream(true).start();
            monitoringPaused=false; engineStatus.setText("Client: running");
            circuitStatus.setText("Monitoring circuit: running — "+circuit);
            Process active=sleelaProcess;
            Thread readerThread=new Thread(() -> {
                try(BufferedReader reader=new BufferedReader(new InputStreamReader(active.getInputStream()))) {
                    while(reader.readLine()!=null) { if(monitoringPaused) continue; }
                } catch(IOException ignored) {
                } finally {
                    Platform.runLater(() -> { if(sleelaProcess==active&&!active.isAlive()){
                        sleelaProcess=null; engineStatus.setText("Client: stopped"); circuitStatus.setText("Monitoring circuit: stopped"); }});
                }
            },"skya-sleela-output");
            readerThread.setDaemon(true); readerThread.start();
        } catch(IOException e) {
            sleelaProcess=null; engineStatus.setText("Client: unavailable");
            circuitStatus.setText("SLeeLa runner unavailable: "+e.getMessage());
        }
    }

    private synchronized void pauseAdminTask() {
        if(sleelaProcess==null||!sleelaProcess.isAlive()){
            engineStatus.setText("Client: stopped"); circuitStatus.setText("Monitoring circuit: not running"); return;
        }
        monitoringPaused=true; engineStatus.setText("Client: paused"); circuitStatus.setText("Monitoring circuit: paused");
    }

    private synchronized void stopAdminTask() {
        monitoringPaused=false; Process process=sleelaProcess; sleelaProcess=null;
        if(process!=null&&process.isAlive()) process.destroy();
        engineStatus.setText("Client: stopped"); circuitStatus.setText("Monitoring circuit: stopped");
    }

    private Path locateCircuit() {
        String configured=System.getenv("SKYA_SLEEELA_CIRCUIT");
        if(configured!=null&&!configured.isBlank()){Path p=Paths.get(configured);if(Files.isRegularFile(p))return p;}
        Path cwd=Paths.get("").toAbsolutePath();
        Path p=cwd.resolve("telephony-skya/sleela/SkyaClient.sleela");
        if(Files.isRegularFile(p))return p;
        p=cwd.resolve("sleela/SkyaClient.sleela");
        return Files.isRegularFile(p)?p:null;
    }
    @Override public void stop() { protocolFooter.stop(); stopAdminTask(); }

    public static void main(String[] args){launch(args);}
}
