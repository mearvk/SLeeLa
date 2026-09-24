package com.mearvk.sleela.skya;

import javafx.animation.Animation;
import javafx.animation.TranslateTransition;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.control.Label;
import javafx.scene.layout.StackPane;
import javafx.scene.text.Font;
import javafx.util.Duration;

/**
 * Scrolling GUI-to-engine protocol status footer for Skya.
 * The messages describe the Guia™ protocol path; they are status messaging,
 * not an independent claim that a production transport has been negotiated.
 */
final class SkyaProtocolFooter {
    private static final String[] MESSAGES = {
        "1. GUIA™ GUI.CREATE → SLeeLa Engine",
        "2. GUIA™ CLIENT.CREATE → Skya Client",
        "3. GUIA™ SESSION.CREATE → Session Layer",
        "4. GUIA™ CLIENT.CONNECT → Connection Manager",
        "5. GUIA™ LISTENER.START → Event Listener",
        "6. GUIA™ SESSION.OPEN → Active Session",
        "7. GUIA™ SESSION.AUTHENTICATE → Authorization Path",
        "8. GUIA™ CIRCUIT.LOAD → SLeeLa Circuit",
        "9. GUIA™ CIRCUIT.START → Engine Runtime",
        "10. GUIA™ EVENT.EMIT → SLeeLa Event Path",
        "11. GUIA™ COMMAND.INVOKE → Engine Command",
        "12. GUIA™ DATA.UPDATE → GUI Data Model",
        "13. GUIA™ LISTENER.RECEIVE → Client Listener",
        "14. GUIA™ LISTENER.ACK → Ordered Message Acknowledgement",
        "15. GUIA™ MONITOR.STATUS → Engine Status",
        "16. GUIA™ CONTROL.UPDATE → JavaFX GUI"
    };

    private final StackPane container = new StackPane();
    private final Label message = new Label();
    private final TranslateTransition scroll = new TranslateTransition();
    private int index;

    SkyaProtocolFooter() {
        message.setFont(Font.font("System", 11));
        message.setStyle("-fx-font-weight: bold;");
        message.setAlignment(Pos.CENTER_LEFT);
        message.setMaxWidth(Double.MAX_VALUE);
        container.setPrefHeight(24);
        container.setMinHeight(24);
        container.setMaxHeight(24);
        container.setStyle("-fx-border-color: #b7cdb8; -fx-background-color: #f4faf4; -fx-padding: 3 8 3 8;");
        container.setClip(new javafx.scene.shape.Rectangle(0, 24));
        container.getChildren().add(message);
        StackPane.setAlignment(message, Pos.CENTER_LEFT);
        message.setText(MESSAGES[0]);

        scroll.setNode(message);
        scroll.setDuration(Duration.seconds(10));
        scroll.setCycleCount(1);
        scroll.setInterpolator(javafx.animation.Interpolator.LINEAR);
        scroll.setOnFinished(e -> {
            index = (index + 1) % MESSAGES.length;
            message.setText(MESSAGES[index]);
            scroll.setFromX(container.getWidth() + 20);
            scroll.setToX(-message.prefWidth(-1) - 20);
            scroll.playFromStart();
        });
        container.widthProperty().addListener((obs, oldValue, newValue) -> {
            if (scroll.getStatus() == Animation.Status.STOPPED) restart();
        });
    }

    Node node() { return container; }

    void start() {
        javafx.application.Platform.runLater(this::restart);
    }

    void stop() { scroll.stop(); }

    private void restart() {
        message.setText(MESSAGES[index]);
        scroll.stop();
        scroll.setFromX(container.getWidth() + 20);
        scroll.setToX(-message.prefWidth(-1) - 20);
        scroll.playFromStart();
    }
}
