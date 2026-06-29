package org.example.academic.system.gui;

import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.TextField;

/**
 * Tela de cadastro de turmas (TUS-2409).
 *
 * <p>Delega o cadastro ao {@code AcademicSystemController}. A operação exige
 * perfil ADMIN; caso o usuário não tenha permissão, a exceção de autorização é
 * exibida ao usuário.</p>
 */
public class ClassRegistrationController {

    @FXML private TextField txtCode;
    @FXML private TextField txtName;
    @FXML private TextField txtProfessor;
    @FXML private TextField txtSemester;

    @FXML
    private void registerClass() {
        String code = txtCode.getText() == null ? "" : txtCode.getText().trim();
        String name = txtName.getText() == null ? "" : txtName.getText().trim();

        try {
            AppContext.academic().registerClass(code, name);

            Alert alert = new Alert(Alert.AlertType.INFORMATION);
            alert.setHeaderText(null);
            alert.setContentText("Turma '" + code + "' cadastrada com sucesso!");
            alert.showAndWait();

            clearFields();
        } catch (Exception e) {
            showError(e);
        }
    }

    private void clearFields() {
        txtCode.clear();
        txtName.clear();
        if (txtProfessor != null) txtProfessor.clear();
        if (txtSemester != null) txtSemester.clear();
    }

    private void showError(Exception e) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setHeaderText(null);
        alert.setContentText(e.getMessage());
        alert.showAndWait();
    }

    @FXML
    private void goBack() {
        SceneManager.switchScene("MainScreen.fxml", "Sistema Acadêmico");
    }
}
