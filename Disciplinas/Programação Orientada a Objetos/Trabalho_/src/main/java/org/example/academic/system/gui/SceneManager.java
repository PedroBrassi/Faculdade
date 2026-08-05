package org.example.academic.system.gui;

import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

/**
 * Gerencia a troca de cenas da aplicação JavaFX, mantendo um único
 * {@link Stage} para todas as telas (TUS-2406).
 */
public class SceneManager {

    private static final String FXML_BASE = "/org/example/academic/system/gui/";

    private static Stage stage;

    public static void setStage(Stage primaryStage) {
        stage = primaryStage;
    }

    public static void switchScene(String fxml, String title) {
        try {
            FXMLLoader loader = new FXMLLoader(
                    SceneManager.class.getResource(FXML_BASE + fxml)
            );

            Parent root = loader.load();

            stage.setTitle(title);
            stage.setScene(new Scene(root));
            stage.show();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
