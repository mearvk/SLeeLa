package com.mearvk.sleela.skya;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.*;
import javafx.stage.Stage;
import java.io.*;
import java.nio.file.*;

public final class SkyaApp extends Application {
    private Label engineStatus, circuitStatus;
    private Process sleelaProcess;
    private volatile boolean monitoringPaused;

    @Override public void start(Stage stage) {
        stage.setTitle("Skya — SLeeLa Telephony");
        engineStatus = new Label("Client: stopped");
        circuitStatus = new Label("Monitoring circuit: ready");
        ListView<String> peers = new ListView<>();
        peers.getItems().addAll("Monitoring circuit initialized","Awaiting connected peers");

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
        root.setPadding(new Insets(12));
        root.setTop(new HBox(8, start, pause, stop));
        root.setCenter(center);
        stage.setScene(new Scene(root,760,520));
        stage.show();
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

    @Override public void stop(){stopAdminTask();}

    private Path locateCircuit() {
        String configured=System.getenv("SKYA_SLEEELA_CIRCUIT");
        if(configured!=null&&!configured.isBlank()){Path p=Paths.get(configured);if(Files.isRegularFile(p))return p;}
        Path cwd=Paths.get("").toAbsolutePath();
        Path p=cwd.resolve("telephony-skya/sleela/SkyaClient.sleela");
        if(Files.isRegularFile(p))return p;
        p=cwd.resolve("sleela/SkyaClient.sleela");
        return Files.isRegularFile(p)?p:null;
    }
    public static void main(String[] args){launch(args);}
}
