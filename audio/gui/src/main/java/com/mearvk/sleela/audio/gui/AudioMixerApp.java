package com.mearvk.sleela.audio.gui;

import javafx.application.Application;
import javafx.beans.value.ObservableValue;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.chart.LineChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

import java.util.Locale;

public final class AudioMixerApp extends Application {
    private final AudioMixerModel model = new AudioMixerModel();
    private final Label status = new Label();

    @Override
    public void start(Stage stage) {
        stage.setTitle("SLeeLa • Audio / Video Mixer");

        BorderPane root = new BorderPane();
        root.setPadding(new Insets(18));
        root.setTop(header());
        root.setCenter(workspace());
        root.setBottom(statusBar());

        Scene scene = new Scene(root, 1120, 720);
        stage.setScene(scene);
        stage.show();

        status.setText(model.summary() + " • synchronized output ready");
    }

    private VBox header() {
        Label title = new Label("SLeeLa Audio / Video");
        title.getStyleClass().add("title");
        Label subtitle = new Label(
                "JavaFX presentation • synchronized native mixer • future SLeeLa mapping");
        subtitle.getStyleClass().add("subtitle");
        VBox box = new VBox(4, title, subtitle);
        box.setPadding(new Insets(0, 0, 16, 0));
        return box;
    }

    private HBox workspace() {
        VBox tracks = trackPanel();
        VBox controls = mixerPanel();
        VBox preview = previewPanel();
        HBox box = new HBox(14, tracks, controls, preview);
        HBox.setHgrow(tracks, Priority.ALWAYS);
        HBox.setHgrow(preview, Priority.ALWAYS);
        return box;
    }

    private VBox trackPanel() {
        Label heading = new Label("Synchronized Inputs");
        heading.getStyleClass().add("heading");

        TableView<AudioMixerModel.Track> table = new TableView<>();
        table.setItems(javafx.collections.FXCollections.observableArrayList(model.tracks()));
        table.setColumnResizePolicy(TableView.CONSTRAINED_RESIZE_POLICY_ALL_COLUMNS);

        TableColumn<AudioMixerModel.Track, String> role = new TableColumn<>("Role");
        role.setCellValueFactory(new PropertyValueFactory<>("role"));
        TableColumn<AudioMixerModel.Track, String> source = new TableColumn<>("Source");
        source.setCellValueFactory(new PropertyValueFactory<>("source"));
        TableColumn<AudioMixerModel.Track, Double> start = new TableColumn<>("Start (s)");
        start.setCellValueFactory(new PropertyValueFactory<>("startSeconds"));
        TableColumn<AudioMixerModel.Track, Double> quality = new TableColumn<>("Quality");
        quality.setCellValueFactory(new PropertyValueFactory<>("quality"));
        table.getColumns().addAll(role, source, start, quality);
        table.setPrefHeight(360);

        Button addFile = new Button("Add Audio File");
        Button addLive = new Button("Add Live Input");
        Button process = new Button("Process Synchronized Output");

        addFile.setOnAction(e -> status.setText(
                "Audio-file input requested • native provider admission is the next integration point."));
        addLive.setOnAction(e -> status.setText(
                "Live input requested • native device provider admission is the next integration point."));
        process.setOnAction(e -> status.setText(
                "Processing synchronized blocks • output/audio/mix-output.wav"));

        HBox actions = new HBox(8, addFile, addLive);
        VBox box = new VBox(10, heading, table, actions, process);
        box.getStyleClass().add("panel");
        return box;
    }

    private VBox mixerPanel() {
        Label heading = new Label("Mixer");
        heading.getStyleClass().add("heading");

        GridPane grid = new GridPane();
        grid.setHgap(12);
        grid.setVgap(12);

        Slider bass = slider(-12, 12, model.bassDb());
        Slider mid = slider(-12, 12, model.midDb());
        Slider treble = slider(-12, 12, model.trebleDb());
        Slider gain = slider(-18, 12, model.masterGainDb());
        Slider pan = slider(-1, 1, model.pan());

        addControl(grid, 0, "Bass", bass, model::setBassDb);
        addControl(grid, 1, "Mid", mid, model::setMidDb);
        addControl(grid, 2, "Treble", treble, model::setTrebleDb);
        addControl(grid, 3, "Master", gain, model::setMasterGainDb);
        addControl(grid, 4, "Pan", pan, model::setPan);

        Label channels = new Label("Channels");
        channels.getStyleClass().add("subheading");

        Slider left = slider(0, 1, 1);
        Slider right = slider(0, 1, 1);
        grid.add(new Label("L"), 0, 6);
        grid.add(left, 1, 6);
        grid.add(new Label("R"), 0, 7);
        grid.add(right, 1, 7);

        VBox box = new VBox(12, heading, grid, channels,
                new Label("Output: synchronized timeline → audio/mix-output.wav"));
        box.getStyleClass().add("panel");
        return box;
    }

    private VBox previewPanel() {
        Label heading = new Label("A/V Preview");
        heading.getStyleClass().add("heading");

        NumberAxis x = new NumberAxis();
        NumberAxis y = new NumberAxis();
        x.setLabel("Timeline");
        y.setLabel("Level");

        LineChart<Number, Number> waveform = new LineChart<>(x, y);
        waveform.setTitle("Processed Audio Level");
        waveform.setLegendVisible(false);
        waveform.setCreateSymbols(false);
        waveform.setAnimated(false);

        XYChart.Series<Number, Number> series = new XYChart.Series<>();
        for (int i = 0; i < 120; i++) {
            double t = i / 24.0;
            double level = 0.55 * Math.sin(t * 2.1) + 0.18 * Math.sin(t * 7.0);
            series.getData().add(new XYChart.Data<>(t, level));
        }
        waveform.getData().add(series);
        waveform.setPrefHeight(350);

        Label video = new Label("Video frame surface\nReady for RGB / RGBA frame preview");
        video.setAlignment(Pos.CENTER);
        video.getStyleClass().add("video-surface");
        video.setMaxWidth(Double.MAX_VALUE);
        video.setPrefHeight(120);

        VBox box = new VBox(10, heading, waveform, video);
        box.getStyleClass().add("panel");
        return box;
    }

    private VBox statusBar() {
        status.getStyleClass().add("status");
        VBox box = new VBox(status);
        box.setPadding(new Insets(14, 0, 0, 0));
        return box;
    }

    private Slider slider(double min, double max, double value) {
        Slider slider = new Slider(min, max, value);
        slider.setShowTickMarks(true);
        slider.setPrefWidth(210);
        return slider;
    }

    private void addControl(GridPane grid, int row, String name, Slider slider,
                            java.util.function.DoubleConsumer update) {
        Label label = new Label(name);
        Label value = new Label(format(slider.getValue()));
        slider.valueProperty().addListener((ObservableValue<? extends Number> ignored,
                                             Number oldValue, Number newValue) -> {
            update.accept(newValue.doubleValue());
            value.setText(format(newValue.doubleValue()));
            status.setText(model.summary());
        });
        grid.add(label, 0, row);
        grid.add(slider, 1, row);
        grid.add(value, 2, row);
    }

    private static String format(double value) {
        return String.format(Locale.ROOT, "%+.1f", value);
    }

    public static void main(String[] args) {
        launch(args);
    }
}
