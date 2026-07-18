package org.example.academic.system.gui;

import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.ComboBox;
import org.example.academic.system.model.PersistenceType;

/**
 * Tela de configuração do tipo de persistência (TUS-2412).
 *
 * <p>Delega ao {@code AcademicSystemController#configurePersistence}, que exige
 * perfil ADMIN. O combo é inicializado com o tipo atualmente configurado.</p>
 */
public class PersistenceConfigController {

    @FXML private ComboBox<String> cbPersistence;

    @FXML
    public void initialize() {
        cbPersistence.getItems().addAll("TXT", "XML", "JSON");
        // Reflete o tipo atualmente configurado no backend.
        cbPersistence.setValue(AppContext.academic().getCurrentPersistenceType().name());
    }

    @FXML
    private void saveConfiguration() {
        try {
            PersistenceType type = PersistenceType.valueOf(cbPersistence.getValue());
            AppContext.academic().configurePersistence(type);

            Alert alert = new Alert(Alert.AlertType.INFORMATION);
            alert.setHeaderText(null);
            alert.setContentText("Persistência alterada para " + type + ".");
            alert.showAndWait();
        } catch (Exception e) {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setHeaderText(null);
            alert.setContentText(e.getMessage());
            alert.showAndWait();
        }
    }

    @FXML
    private void goBack() {
        SceneManager.switchScene("MainScreen.fxml", "Sistema Acadêmico");
    }
}
