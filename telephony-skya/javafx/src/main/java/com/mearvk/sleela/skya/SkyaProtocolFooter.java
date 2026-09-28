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
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.util.Duration;
import javax.xml.parsers.DocumentBuilderFactory;

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
    private final VBox root=new VBox();
    private final VBox scroller=new VBox(2);
    private final HBox bevel=new HBox();
    private final StackPane container=new StackPane();
    private final Label message=new Label();
    private final TranslateTransition scroll=new TranslateTransition();
    private int index;
    private String bodiUiStatus="BODI UI: unavailable";

    SkyaProtocolFooter(){
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
    void callback(String event){
        int selected=switch(event){
            case "GUI.CREATE"->0;case "CLIENT.CREATE"->1;case "SESSION.CREATE"->2;case "CLIENT.CONNECT"->3;
            case "LISTENER.START"->4;case "SESSION.OPEN","SESSION.CLOSE"->5;case "SESSION.AUTHENTICATE"->6;
            case "CIRCUIT.LOAD"->7;case "CIRCUIT.START","CIRCUIT.PAUSE"->8;case "EVENT.EMIT"->9;
            case "COMMAND.INVOKE"->10;case "DATA.UPDATE"->11;case "LISTENER.RECEIVE"->12;case "LISTENER.ACK"->13;
            case "MONITOR.STATUS","CIRCUIT.STOP"->14;case "CONTROL.UPDATE"->15;default->index;};
        index=selected;javafx.application.Platform.runLater(this::restart);
    }
    String bodiUiStatus(){return bodiUiStatus;}
    private void loadBodiUi(){
        try(var in=getClass().getResourceAsStream("/skya-ui.xml")){
            if(in==null)return;var doc=DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(in);var r=doc.getDocumentElement();
            if("bodi-ui".equals(r.getTagName())&&"skya-ui".equals(r.getAttribute("id")))bodiUiStatus="BODI UI: loaded "+r.getAttribute("id");
        }catch(Exception ignored){}
    }
    private void restart(){message.setText(MESSAGES[index]);scroll.stop();scroll.setFromX(container.getWidth()+20);scroll.setToX(-message.prefWidth(-1)-20);scroll.playFromStart();}
}
