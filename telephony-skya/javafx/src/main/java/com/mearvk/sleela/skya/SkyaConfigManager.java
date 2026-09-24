package com.mearvk.sleela.skya;

import javafx.scene.control.Alert;
import javafx.scene.control.ChoiceDialog;
import javafx.scene.control.TextInputDialog;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.*;
import java.util.*;
import java.util.stream.Collectors;

final class SkyaConfigManager {
    private static final Path CONFIG_DIR =
            Paths.get(System.getProperty("user.home"), ".sleela", "skya");

    private SkyaConfigManager() {}

    static Path directory() throws IOException {
        Files.createDirectories(CONFIG_DIR);
        return CONFIG_DIR;
    }

    static List<String> names() throws IOException {
        if (!Files.isDirectory(CONFIG_DIR)) return List.of();
        try (var stream = Files.list(CONFIG_DIR)) {
            return stream.filter(p -> p.getFileName().toString().endsWith(".properties"))
                    .map(p -> p.getFileName().toString().replaceFirst("\\.properties$", ""))
                    .sorted()
                    .collect(Collectors.toList());
        }
    }

    static Properties load(String name) throws IOException {
        Properties p = new Properties();
        Path file = safePath(name);
        if (Files.isRegularFile(file)) {
            try (InputStream in = Files.newInputStream(file)) {
                p.load(in);
            }
        }
        return p;
    }

    static void save(String name, Properties properties) throws IOException {
        Path file = safePath(name);
        try (OutputStream out = Files.newOutputStream(file)) {
            properties.store(out, "Skya configuration");
        }
    }

    static void delete(String name) throws IOException {
        Files.deleteIfExists(safePath(name));
    }

    static String promptName(javafx.stage.Window owner, String initial) {
        TextInputDialog dialog = new TextInputDialog(initial);
        dialog.setTitle("Skya Configuration");
        dialog.setHeaderText("Configuration name");
        dialog.setContentText("Name:");
        return dialog.showAndWait().map(String::trim)
                .filter(s -> !s.isBlank() && s.matches("[A-Za-z0-9._-]+"))
                .orElse(null);
    }

    static void error(javafx.stage.Window owner, String message) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.initOwner(owner);
        alert.setTitle("Skya Configuration");
        alert.setHeaderText("Configuration operation failed");
        alert.setContentText(message);
        alert.showAndWait();
    }

    private static Path safePath(String name) {
        if (name == null || !name.matches("[A-Za-z0-9._-]+")) {
            throw new IllegalArgumentException("Invalid configuration name");
        }
        return CONFIG_DIR.resolve(name + ".properties").normalize();
    }
}
