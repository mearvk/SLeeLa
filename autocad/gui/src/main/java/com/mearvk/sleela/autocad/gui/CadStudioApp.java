package com.mearvk.sleela.autocad.gui;

import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

import java.util.List;

/**
 * SLeeLa AutoCAD Studio — a JavaFX plan-entry + geometry preview surface.
 *
 * <p>The user types a plan (description, dimensions, plan, notes); the Studio
 * asks the SLeeLa renderer to evaluate it, draws the returned geometry in a
 * live canvas preview, and can export the renderer's AutoCAD DXF.</p>
 *
 * <p><strong>JavaFX presents; SLeeLa decides.</strong> This class computes no
 * geometry and writes no DXF — it displays what {@link CadPlanModel} returns
 * from the renderer.</p>
 */
public final class CadStudioApp extends Application {

    private final CadPlanModel model = new CadPlanModel();
    private final Canvas canvas = new Canvas(760, 620);
    private final Label status = new Label();
    private final TextArea dxfOut = new TextArea();

    @Override
    public void start(Stage stage) {
        stage.setTitle("SLeeLa \u2022 AutoCAD Studio");

        BorderPane root = new BorderPane();
        root.setPadding(new Insets(16));
        root.setTop(header());
        root.setLeft(planPanel());
        root.setCenter(previewPanel());
        root.setBottom(statusBar());

        Scene scene = new Scene(root, 1280, 820);
        stage.setScene(scene);
        stage.show();

        refresh();
    }

    private VBox header() {
        Label title = new Label("SLeeLa AutoCAD Studio");
        Label subtitle = new Label(
                "Descriptions \u2022 dimensions \u2022 plans \u2022 notes \u2192 AutoCAD DXF  \u2022  JavaFX presents, SLeeLa decides");
        VBox box = new VBox(4, title, subtitle);
        box.setPadding(new Insets(0, 0, 14, 0));
        return box;
    }

    /** Left: the plan text editor plus render/export actions. */
    private VBox planPanel() {
        Label heading = new Label("Plan");
        TextArea editor = new TextArea(model.planText());
        editor.setPrefRowCount(22);
        editor.setPrefColumnCount(34);
        editor.textProperty().addListener((obs, old, val) -> {
            model.setPlanText(val);
            refresh();
        });

        Button render = new Button("Render via SLeeLa");
        render.setOnAction(e -> refresh());
        Button export = new Button("Show DXF");
        export.setOnAction(e -> dxfOut.setText(model.render()));
        HBox actions = new HBox(8, render, export);

        dxfOut.setEditable(false);
        dxfOut.setPrefRowCount(8);
        dxfOut.setPromptText("Generated AutoCAD DXF appears here (from the SLeeLa renderer).");

        VBox box = new VBox(10, heading, editor, actions, new Label("DXF output"), dxfOut);
        box.setPadding(new Insets(0, 16, 0, 0));
        box.setPrefWidth(420);
        return box;
    }

    /** Center: the geometry preview drawn from the renderer's entity list. */
    private VBox previewPanel() {
        Label heading = new Label("Geometry Preview");
        VBox box = new VBox(8, heading, canvas);
        VBox.setVgrow(canvas, Priority.ALWAYS);
        return box;
    }

    private VBox statusBar() {
        VBox box = new VBox(status);
        box.setPadding(new Insets(12, 0, 0, 0));
        return box;
    }

    /** Pull entities from the renderer (via the model) and paint them. */
    private void refresh() {
        List<Entity> entities = model.previewEntities();
        draw(entities);
        status.setText(model.summary()
                + "  \u2022  rendered via SLeeLa connector (local preview until transport is wired)");
    }

    /** Paint the entity list, fitting the drawing extents into the canvas. */
    private void draw(List<Entity> entities) {
        GraphicsContext g = canvas.getGraphicsContext2D();
        double w = canvas.getWidth();
        double h = canvas.getHeight();
        g.setFill(Color.web("#0f172a"));
        g.fillRect(0, 0, w, h);

        if (entities.isEmpty()) { return; }

        // Compute drawing extents.
        double minX = Double.MAX_VALUE, minY = Double.MAX_VALUE;
        double maxX = -Double.MAX_VALUE, maxY = -Double.MAX_VALUE;
        for (Entity e : entities) {
            if (e instanceof Entity.Line l) {
                minX = Math.min(minX, Math.min(l.x1(), l.x2()));
                minY = Math.min(minY, Math.min(l.y1(), l.y2()));
                maxX = Math.max(maxX, Math.max(l.x1(), l.x2()));
                maxY = Math.max(maxY, Math.max(l.y1(), l.y2()));
            } else if (e instanceof Entity.Text t) {
                minX = Math.min(minX, t.x()); minY = Math.min(minY, t.y());
                maxX = Math.max(maxX, t.x()); maxY = Math.max(maxY, t.y());
            }
        }
        double pad = 40;
        double sx = (w - 2 * pad) / Math.max(1, maxX - minX);
        double sy = (h - 2 * pad) / Math.max(1, maxY - minY);
        double scale = Math.min(sx, sy);

        // DXF Y is up; canvas Y is down — flip when projecting.
        for (Entity e : entities) {
            if (e instanceof Entity.Line l) {
                g.setStroke(l.layer().equals("dims") ? Color.web("#f87171") : Color.web("#e5e7eb"));
                g.setLineWidth(l.layer().equals("dims") ? 1.0 : 2.0);
                g.strokeLine(px(l.x1(), minX, scale, pad), py(l.y1(), minY, maxY, scale, pad),
                             px(l.x2(), minX, scale, pad), py(l.y2(), minY, maxY, scale, pad));
            } else if (e instanceof Entity.Text t) {
                g.setFill(t.layer().equals("dims") ? Color.web("#fca5a5") : Color.web("#86efac"));
                g.fillText(t.value(), px(t.x(), minX, scale, pad), py(t.y(), minY, maxY, scale, pad));
            }
        }
    }

    private static double px(double x, double minX, double scale, double pad) {
        return pad + (x - minX) * scale;
    }

    private static double py(double y, double minY, double maxY, double scale, double pad) {
        // Flip vertically so the drawing reads the right way up.
        return pad + (maxY - y) * scale;
    }

    public static void main(String[] args) {
        launch(args);
    }
}
