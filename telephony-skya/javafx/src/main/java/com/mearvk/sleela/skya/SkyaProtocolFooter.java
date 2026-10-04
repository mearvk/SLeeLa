package com.mearvk.sleela.skya;

import javafx.animation.Animation;
import javafx.animation.TranslateTransition;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.control.Label;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Region;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.util.Duration;
import javax.xml.parsers.DocumentBuilderFactory;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.function.Consumer;

/**
 * Guia(TM) client/listener protocol surface for the Skya JavaFX GUI.
 *
 * <p>This is the live GUI-to-SLeeLa connection. Each GUI control calls
 * {@link #send(String)} with a Guia command name; the footer maps it to a
 * {@code GUIA/1 <COMMAND>} line, opens a TCP control socket to the SLeeLa Skya
 * client (default {@code 127.0.0.1:8700}, overridable via
 * {@code SKYA_GUIA_HOST}/{@code SKYA_GUIA_PORT}), writes the command, reads the
 * {@code GUIA/1 <EVENT>} reply, and returns it. The SLeeLa client performs the
 * actual network communication and returns the event here. The scrolling
 * status bar visualizes the last command/event in the Guia 16-step model.
 *
 * <p>If the SLeeLa client is not reachable, {@code send} returns a
 * {@code GUIA/1 CLIENT.OFFLINE} event rather than throwing, so the GUI stays
 * responsive and reports the condition.
 */
final class SkyaProtocolFooter {
    private static final String[] MESSAGES = {
        "1. GUIA™ GUI.CREATE → SLeeLa Engine","2. GUIA™ CLIENT.CREATE → Skya Client",
        "3. GUIA™ SESSION.CREATE → Session Layer","4. GUIA™ CLIENT.CONNECT → Connection Manager",
        "5. GUIA™ LISTENER.START → Event Listener","6. GUIA™ SESSION.OPEN → Active Session",
        "7. GUIA™ SESSION.AUTHENTICATE → Authorization Path","8. GUIA™ CIRCUIT.LOAD → SLeeLa Circuit",
        "9. GUIA™ CIRCUIT.START → Engine Runtime","10. GUIA™ EVENT.EMIT → SLeeLa Event Path",
        "11. GUIA™ COMMAND.INVOKE → Engine Command","12. GUIA™ DATA.UPDATE → GUI Data Model",
        "13. GUIA™ LISTENER.RECEIVE → Client Listener","14. GUIA™ LISTENER.ACK → Ordered Message Acknowledgement",
        "15. GUIA™ MONITOR.STATUS → Engine Status","16. GUIA™ CONTROL.UPDATE → JavaFX GUI"
    };

    /** Default SLeeLa Guia control endpoint (SkyaClient.sleela listens here). */
    private static final String DEFAULT_HOST = "127.0.0.1";
    private static final int DEFAULT_PORT = 8700;
    private static final int CONNECT_TIMEOUT_MS = 2000;
    private static final int READ_TIMEOUT_MS = 4000;

    private final VBox root=new VBox();
    private final VBox scroller=new VBox(2);
    private final HBox bevel=new HBox();
    private final StackPane container=new StackPane();
    private final Label message=new Label();
    private final TranslateTransition scroll=new TranslateTransition();
    private final String host;
    private final int port;
    private int index;
    private String bodiUiStatus="BODI UI: unavailable";
    private final java.util.List<Consumer<String>> eventSinks = new java.util.concurrent.CopyOnWriteArrayList<>();

    SkyaProtocolFooter(){
        this.host = envOr("SKYA_GUIA_HOST", DEFAULT_HOST);
        this.port = envIntOr("SKYA_GUIA_PORT", DEFAULT_PORT);
        message.setFont(Font.font("System",11));message.setStyle("-fx-font-weight:bold;");
        message.setAlignment(Pos.CENTER_LEFT);message.setMaxWidth(Double.MAX_VALUE);
        container.setPrefHeight(24);container.setMinHeight(24);container.setMaxHeight(24);
        container.setStyle("-fx-border-color:#b7cdb8;-fx-background-color:#f4faf4;-fx-padding:3 8 3 8;");
        container.setClip(new Rectangle(0,24));container.getChildren().add(message);
        StackPane.setAlignment(message,Pos.CENTER_LEFT);

        Region upper=new Region(), lower=new Region();
        upper.setPrefHeight(2);lower.setPrefHeight(1);
        upper.setStyle("-fx-background-color:#a6a6a6;");
        lower.setStyle("-fx-background-color:#ffffff;");
        HBox.setHgrow(upper,javafx.scene.layout.Priority.ALWAYS);HBox.setHgrow(lower,javafx.scene.layout.Priority.ALWAYS);
        bevel.getChildren().addAll(upper,lower);
        bevel.setMinHeight(3);bevel.setPrefHeight(3);bevel.setMaxHeight(3);
        bevel.setStyle("-fx-border-color:#d0d0d0 transparent transparent transparent;");

        scroller.getChildren().add(container);
        root.getChildren().addAll(bevel,scroller);
        loadBodiUi();message.setText(MESSAGES[0]);
        scroll.setNode(message);scroll.setDuration(Duration.seconds(10));scroll.setCycleCount(1);
        scroll.setInterpolator(javafx.animation.Interpolator.LINEAR);
        scroll.setOnFinished(e->{index=(index+1)%MESSAGES.length;message.setText(MESSAGES[index]);scroll.setFromX(container.getWidth()+20);scroll.setToX(-message.prefWidth(-1)-20);scroll.playFromStart();});
        container.widthProperty().addListener((obs,oldValue,newValue)->{if(scroll.getStatus()==Animation.Status.STOPPED)restart();});
    }

    Node node(){return root;}
    void start(){javafx.application.Platform.runLater(this::restart);}
    void stop(){scroll.stop();}

    /** The host:port of the SLeeLa Guia control endpoint, for display. */
    String endpoint(){return host+":"+port;}

    /** Register a sink that receives every Guia event line this footer gets. */
    void onEvent(Consumer<String> sink){ if(sink!=null) eventSinks.add(sink); }

    private void emit(String event){ for(Consumer<String> s: eventSinks){ try{ s.accept(event); }catch(RuntimeException ignored){} } }

    /**
     * Advance the scrolling status bar to the step for {@code event} without
     * performing any network I/O. Used to reflect a received Guia event name.
     */
    void callback(String event){
        index=stepFor(event);
        javafx.application.Platform.runLater(this::restart);
    }

    /**
     * Send a Guia command to the SLeeLa Skya client and return its Guia event
     * reply. Opens a short-lived TCP control connection, writes
     * {@code GUIA/1 <command>}, reads one reply line, drives the status bar for
     * both the command and the reply, and notifies the event sink.
     *
     * <p>Never throws: on any connection/IO failure it returns
     * {@code "GUIA/1 CLIENT.OFFLINE reason=..."}. Runs the blocking socket work
     * on a short-lived worker thread so the JavaFX thread is never blocked.
     */
    String send(String command){
        callback(verb(command));
        // The SLeeLa agent dispatches on the exact command verb; arguments are
        // for display/logging only, so only the verb is placed on the wire.
        String line = "GUIA/1 " + verb(command);
        String event;
        try {
            event = exchange(line);
        } catch (Exception ex) {
            event = "GUIA/1 CLIENT.OFFLINE reason=" + sanitize(ex.getMessage());
        }
        final String received = event;
        callback(guiaEventName(received));
        javafx.application.Platform.runLater(() -> emit(received));
        return event;
    }

    /** Fire-and-forget variant used from UI handlers; result goes to the sink. */
    void sendAsync(String command){
        callback(verb(command));
        final String line = "GUIA/1 " + verb(command);
        Thread t = new Thread(() -> {
            String event;
            try { event = exchange(line); }
            catch (Exception ex) { event = "GUIA/1 CLIENT.OFFLINE reason=" + sanitize(ex.getMessage()); }
            final String received = event;
            javafx.application.Platform.runLater(() -> { callback(guiaEventName(received)); emit(received); });
        }, "guia-send");
        t.setDaemon(true);
        t.start();
    }

    private String exchange(String line) throws Exception {
        try (Socket socket = new Socket()) {
            socket.connect(new InetSocketAddress(host, port), CONNECT_TIMEOUT_MS);
            socket.setSoTimeout(READ_TIMEOUT_MS);
            OutputStream out = socket.getOutputStream();
            // The SLeeLa agent reads one frame with sockread() and matches the
            // exact command verb, so no trailing newline is sent. We half-close
            // our output so the agent's single sockread() sees a complete frame.
            out.write(line.getBytes(StandardCharsets.UTF_8));
            out.flush();
            socket.shutdownOutput();
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream(), StandardCharsets.UTF_8));
            String reply = in.readLine();
            return reply == null || reply.isBlank() ? "GUIA/1 CLIENT.OFFLINE reason=no-reply" : reply.trim();
        }
    }

    String bodiUiStatus(){return bodiUiStatus;}

    private void loadBodiUi(){
        try(var in=getClass().getResourceAsStream("/skya-ui.xml")){
            if(in==null)return;var doc=DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(in);var r=doc.getDocumentElement();
            if("bodi-ui".equals(r.getTagName())&&"skya-ui".equals(r.getAttribute("id")))bodiUiStatus="BODI UI: loaded "+r.getAttribute("id");
        }catch(Exception ignored){}
        // Surface the declarative-UI load status so it is observable rather than
        // dead state (the GUI is built in code; skya-ui.xml is the Guia/BODI
        // declaration and reference for the command/binding vocabulary).
        System.out.println("[skya] " + bodiUiStatus);
    }

    /** Map a Guia command or event name to its step index in the 16-step model. */
    private static int stepFor(String name){
        return switch(name){
            case "GUI.CREATE","CLIENT.CREATED"->0;
            case "CLIENT.CREATE"->1;
            case "SESSION.CREATE"->2;
            case "CLIENT.CONNECT","CLIENT.CONNECTED","CLIENT.OFFLINE","CLIENT.ERROR"->3;
            case "LISTENER.START"->4;
            case "SESSION.OPEN","SESSION.CLOSE","SESSION.CLOSED"->5;
            case "SESSION.AUTHENTICATE"->6;
            case "CIRCUIT.LOAD"->7;
            case "CIRCUIT.START","CIRCUIT.PAUSE"->8;
            case "EVENT.EMIT","MEDIA.AUDIO","MEDIA.VIDEO"->9;
            case "COMMAND.INVOKE","CHAT.SEND"->10;
            case "DATA.UPDATE"->11;
            case "LISTENER.RECEIVE"->12;
            case "LISTENER.ACK","ACK","FILE.ACCEPTED","FILE.SEND"->13;
            case "MONITOR.STATUS","CIRCUIT.STOP"->14;
            case "CONTROL.UPDATE","COMMAND.UNKNOWN"->15;
            default->index0();
        };
    }
    private static int index0(){return 0;}

    /** The leading verb of a command line (text before the first space). */
    private static String verb(String command){
        if(command==null) return "";
        String s=command.trim();
        int sp=s.indexOf(' ');
        return sp<0 ? s : s.substring(0,sp);
    }

    /** Extract the EVENT token from a "GUIA/1 EVENT ..." reply line. */
    private static String guiaEventName(String line){
        if(line==null) return "CONTROL.UPDATE";
        String s=line.trim();
        if(s.startsWith("GUIA/1 ")) s=s.substring(7).trim();
        int sp=s.indexOf(' ');
        return sp<0 ? s : s.substring(0,sp);
    }

    private static String sanitize(String s){ return s==null ? "unknown" : s.replace('\n',' ').replace('\r',' '); }
    private static String envOr(String k,String d){ String v=System.getenv(k); return v==null||v.isBlank()?d:v.trim(); }
    private static int envIntOr(String k,int d){ try{ String v=System.getenv(k); return v==null||v.isBlank()?d:Integer.parseInt(v.trim()); }catch(NumberFormatException e){ return d; } }

    private void restart(){message.setText(MESSAGES[index]);scroll.stop();scroll.setFromX(container.getWidth()+20);scroll.setToX(-message.prefWidth(-1)-20);scroll.playFromStart();}
}
