package com.mearvk.sleela.skya;

import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.*;
import javafx.stage.Stage;

import java.io.*;
import java.nio.file.*;

public final class SkyaApp extends Application {
    private Label engineStatus;
    private Label circuitStatus;

    @Override
    public void start(Stage stage) {
        stage.setTitle("Skya — SLeeLa Telephony");
        engineStatus = new Label("Client: starting");
        circuitStatus = new Label("Monitoring circuit: loading");

        ListView<String> peers = new ListView<>();
        peers.getItems().addAll(
            "Monitoring circuit initialized",
            "Awaiting connected peers"
        );

        Button start = new Button("Start Client");
        Button stop = new Button("Stop Monitor");
        start.setOnAction(e -> engineStatus.setText("Client: running"));
        stop.setOnAction(e -> engineStatus.setText("Client: monitor stopped"));

        VBox center = new VBox(
            10,
            new Label("Skya Client Monitor"),
            new Separator(),
            new Label("Connection / Peer Monitor"),
            peers,
            new Label("Engine"),
            engineStatus,
            new Label("SLeeLa .sleela circuit"),
            circuitStatus
        );

        root(stage, new HBox(8, start, stop), center);
        runSleelaMonitoringCircuit();
    }

    private void root(Stage stage, HBox top, VBox center) {
        BorderPane root = new BorderPane();
        root.setPadding(new Insets(12));
        root.setTop(top);
        root.setCenter(center);
        stage.setScene(new Scene(root, 760, 520));
        stage.show();
    }

    private void runSleelaMonitoringCircuit() {
        Path circuit = locateCircuit();
        if (circuit == null) {
            circuitStatus.setText("Monitoring circuit: not found");
            return;
        }

        circuitStatus.setText("Monitoring circuit: " + circuit);

        String command = System.getenv("SLEELA_COMMAND");
        if (command == null || command.isBlank()) {
            command = "sleela";
        }

        try {
            Process process = new ProcessBuilder(
                command, "run", circuit.toString()
            ).redirectErrorStream(true).start();

            Thread outputReader = new Thread(() -> {
                try (BufferedReader reader = new BufferedReader(
                        new InputStreamReader(process.getInputStream()))) {
                    while (reader.readLine() != null) {
                        // The SLeeLa circuit remains authoritative.
                        // GUI output is consumed without blocking the JavaFX UI thread.
                    }
                } catch (IOException ignored) {
                    // Process termination is handled by the SLeeLa circuit lifecycle.
                }
            }, "skya-sleela-output");

            // Java 17-compatible replacement for Thread.ofVirtual().
            outputReader.setDaemon(true);
            outputReader.start();
        } catch (IOException e) {
            circuitStatus.setText("Circuit found; SLeeLa runner unavailable");
        }
    }

    private Path locateCircuit() {
        String configured = System.getenv("SKYA_SLEEELA_CIRCUIT");
        if (configured != null && !configured.isBlank()) {
            Path path = Paths.get(configured);
            if (Files.isRegularFile(path)) {
                return path;
            }
        }

        Path cwd = Paths.get("").toAbsolutePath();
        Path path = cwd.resolve("telephony-skya/sleela/SkyaClient.sleela");
        if (Files.isRegularFile(path)) {
            return path;
        }

        path = cwd.resolve("sleela/SkyaClient.sleela");
        return Files.isRegularFile(path) ? path : null;
    }

    public static void main(String[] args) {
        launch(args);
    }
}
