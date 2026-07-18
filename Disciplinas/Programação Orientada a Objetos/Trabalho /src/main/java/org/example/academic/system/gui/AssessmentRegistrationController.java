package org.example.academic.system.gui;

import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.ComboBox;
import javafx.scene.control.TextField;
import org.example.academic.system.exception.InvalidNumericInputException;

/**
 * Tela de cadastro de avaliações (US-2410).
 *
 * <p>Delega o registro ao {@code AcademicSystemController}, que cria a avaliação
 * do tipo escolhido e a associa à turma informada. Acessível por ADMIN e
 * PROFESSOR.</p>
 */
public class AssessmentRegistrationController {

    @FXML private TextField txtClassCode;
    @FXML private ComboBox<String> cbType;
    @FXML private TextField txtName;
    @FXML private TextField txtWeight;
    @FXML private TextField txtMaximumScore;

    @FXML
    public void initialize() {
        cbType.getItems().addAll(
                "Exam",
                "Practical Assignment",
                "Seminar",
                "Assignment"
        );
        cbType.getSelectionModel().selectFirst();
    }

    @FXML
    private void registerAssessment() {
        try {
            String classCode = txtClassCode.getText() == null ? "" : txtClassCode.getText().trim();
            int type = typeIndexOf(cbType.getValue());
            double value = parseDouble(txtMaximumScore.getText(), "Nota");
            double weight = parseDouble(txtWeight.getText(), "Peso");

            AppContext.academic().registerAssessment(classCode, type, value, weight);

            Alert alert = new Alert(Alert.AlertType.INFORMATION);
            alert.setHeaderText(null);
            alert.setContentText("Avaliação cadastrada na turma '" + classCode + "'!");
            alert.showAndWait();

            txtName.clear();
            txtWeight.clear();
            txtMaximumScore.clear();
        } catch (Exception e) {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setHeaderText(null);
            alert.setContentText(e.getMessage());
            alert.showAndWait();
        }
    }

    /** Mapeia o tipo selecionado para o código esperado pelo backend (1=Exam ... 4=Assignment). */
    private int typeIndexOf(String type) {
        if (type == null) return 0;
        return switch (type) {
            case "Exam" -> 1;
            case "Practical Assignment" -> 2;
            case "Seminar" -> 3;
            case "Assignment" -> 4;
            default -> 0;
        };
    }

    private double parseDouble(String input, String fieldName) {
        try {
            return Double.parseDouble(input == null ? "" : input.trim());
        } catch (NumberFormatException e) {
            throw new InvalidNumericInputException(fieldName + ": " + input);
        }
    }

    @FXML
    private void goBack() {
        SceneManager.switchScene("MainScreen.fxml", "Sistema Acadêmico");
    }
}
