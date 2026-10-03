package com.mearvk.sleela.website.gui;

import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.control.Slider;
import javafx.scene.control.TextField;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.layout.VBox;
import javafx.scene.web.WebView;
import javafx.stage.Stage;

/**
 * SLeeLa Website Studio — a JavaFX authoring + preview surface.
 *
 * <p>The Studio lets a user/developer design a site by hand: pick a design
 * preset, drive the "energy" (exciting) dial, adjust the palette, and compose
 * an ordered list of signs. It then asks the SLeeLa generator for the HTML and
 * renders it in a live preview.</p>
 *
 * <p><strong>JavaFX presents; SLeeLa decides.</strong> This class contains no
 * markup generation of its own — it collects intent and displays what
 * {@link WebsiteDesignModel#generate()} returns.</p>
 */
public final class WebsiteStudioApp extends Application {

    private final WebsiteDesignModel model = new WebsiteDesignModel();
    private final Label status = new Label();
    private final WebView preview = new WebView();
    private final ListView<Sign> signList = new ListView<>();

    @Override
    public void start(Stage stage) {
        stage.setTitle("SLeeLa \u2022 Website Studio");

        BorderPane root = new BorderPane();
        root.setPadding(new Insets(16));
        root.setTop(header());
        root.setLeft(designPanel());
        root.setCenter(previewPanel());
        root.setRight(signPanel());
        root.setBottom(statusBar());

        Scene scene = new Scene(root, 1280, 820);
        stage.setScene(scene);
        stage.show();

        refresh();
    }

    private VBox header() {
        Label title = new Label("SLeeLa Website Studio");
        Label subtitle = new Label(
                "Build exciting sites from designs + signs \u2022 JavaFX presents, SLeeLa decides");
        VBox box = new VBox(4, title, subtitle);
        box.setPadding(new Insets(0, 0, 14, 0));
        return box;
    }

    /** Left: the global design controls (preset, energy dial, palette). */
    private VBox designPanel() {
        Label heading = new Label("Design");

        ChoiceBox<WebsiteDesignModel.Preset> presets =
                new ChoiceBox<>(FXCollections.observableArrayList(WebsiteDesignModel.Preset.values()));
        presets.setValue(model.preset());
        presets.valueProperty().addListener((obs, old, val) -> {
            if (val != null) { model.setPreset(val); syncPaletteFields(); refresh(); }
        });

        Slider energy = new Slider(0, 100, model.energy());
        energy.setShowTickMarks(true);
        energy.setShowTickLabels(true);
        energy.setMajorTickUnit(25);
        energy.valueProperty().addListener((obs, old, val) -> {
            model.setEnergy(val.intValue());
            refresh();
        });

        brandField = paletteField("Brand", model.brand(), model::setBrand);
        baseField = paletteField("Base", model.base(), model::setBase);
        accentField = paletteField("Accent", model.accent(), model::setAccent);

        VBox box = new VBox(10,
                heading,
                new Label("Preset"), presets,
                new Label("Energy (exciting dial)"), energy,
                brandField.row, baseField.row, accentField.row);
        box.setPadding(new Insets(0, 16, 0, 0));
        box.setPrefWidth(260);
        return box;
    }

    /** Center: the live preview rendered from the generator output. */
    private VBox previewPanel() {
        Label heading = new Label("Live Preview");
        VBox.setVgrow(preview, Priority.ALWAYS);
        Button regenerate = new Button("Regenerate from SLeeLa");
        regenerate.setOnAction(e -> refresh());
        VBox box = new VBox(8, heading, preview, regenerate);
        box.setPadding(new Insets(0, 16, 0, 0));
        return box;
    }

    /** Right: the ordered sign list and its editor controls. */
    private VBox signPanel() {
        Label heading = new Label("Signs");
        signList.setItems(FXCollections.observableArrayList(model.signs()));
        signList.setCellFactory(v -> new javafx.scene.control.ListCell<>() {
            @Override
            protected void updateItem(Sign item, boolean empty) {
                super.updateItem(item, empty);
                setText(empty || item == null ? null : item.label());
            }
        });
        signList.setPrefHeight(360);

        ChoiceBox<Sign.Kind> kind =
                new ChoiceBox<>(FXCollections.observableArrayList(Sign.Kind.values()));
        kind.setValue(Sign.Kind.HERO);
        TextField a = new TextField();
        TextField b = new TextField();
        TextField c = new TextField();
        kind.valueProperty().addListener((obs, old, k) -> {
            if (k != null) {
                a.setPromptText(k.slotA());
                b.setPromptText(k.slotB());
                c.setPromptText(k.slotC());
            }
        });
        a.setPromptText("Headline");
        b.setPromptText("Subhead");
        c.setPromptText("CTA label");

        Button add = new Button("Add Sign");
        add.setOnAction(e -> {
            model.addSign(new Sign(kind.getValue(), a.getText(), b.getText(), c.getText()));
            a.clear(); b.clear(); c.clear();
            reloadSigns();
            refresh();
        });

        Button remove = new Button("Remove");
        remove.setOnAction(e -> {
            model.removeSign(signList.getSelectionModel().getSelectedIndex());
            reloadSigns();
            refresh();
        });
        Button up = new Button("\u2191");
        up.setOnAction(e -> move(-1));
        Button down = new Button("\u2193");
        down.setOnAction(e -> move(1));
        HBox order = new HBox(8, remove, up, down);

        VBox box = new VBox(10, heading, signList, order,
                new Label("New sign"), kind, a, b, c, add);
        box.setPrefWidth(320);
        return box;
    }

    private VBox statusBar() {
        VBox box = new VBox(status);
        box.setPadding(new Insets(12, 0, 0, 0));
        return box;
    }

    // --- helpers ---------------------------------------------------------

    private void move(int delta) {
        int i = signList.getSelectionModel().getSelectedIndex();
        model.moveSign(i, delta);
        reloadSigns();
        signList.getSelectionModel().select(i + delta);
        refresh();
    }

    private void reloadSigns() {
        signList.setItems(FXCollections.observableArrayList(model.signs()));
    }

    /** Ask SLeeLa (via the model) for HTML and show it; update the status. */
    private void refresh() {
        String html = model.generate();
        preview.getEngine().loadContent(html, "text/html");
        status.setText(model.summary() + "  \u2022  generated via SLeeLa connector (local preview until transport is wired)");
    }

    private void syncPaletteFields() {
        if (brandField != null) { brandField.field.setText(model.brand()); }
        if (baseField != null) { baseField.field.setText(model.base()); }
        if (accentField != null) { accentField.field.setText(model.accent()); }
    }

    private PaletteField brandField;
    private PaletteField baseField;
    private PaletteField accentField;

    private PaletteField paletteField(String name, String value, java.util.function.Consumer<String> apply) {
        Label label = new Label(name);
        TextField field = new TextField(value);
        field.textProperty().addListener((obs, old, val) -> { apply.accept(val); refresh(); });
        HBox row = new HBox(8, label, field);
        HBox.setHgrow(field, Priority.ALWAYS);
        return new PaletteField(row, field);
    }

    /** Small struct pairing a labelled row with its editable field. */
    private static final class PaletteField {
        final HBox row;
        final TextField field;
        PaletteField(HBox row, TextField field) { this.row = row; this.field = field; }
    }

    public static void main(String[] args) {
        launch(args);
    }
}
