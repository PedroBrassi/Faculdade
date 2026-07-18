package org.example.academic.system.gui;

import javafx.fxml.FXML;
import javafx.scene.control.TextArea;
import org.example.academic.system.controller.AcademicSystemController;
import org.example.academic.system.model.AcademicClass;

import java.util.List;

/**
 * Tela de relatórios (TUS-2411).
 *
 * <p>Monta um relatório consolidado a partir do {@code ReportService} (via
 * {@code AcademicSystemController}): configuração de persistência atual e o
 * sumário de avaliações de cada turma cadastrada.</p>
 */
public class ReportController {

    @FXML private TextArea txtReport;

    @FXML
    private void generateReport() {
        AcademicSystemController controller = AppContext.academic();
        StringBuilder sb = new StringBuilder();

        sb.append("=== Configuração de Persistência ===\n");
        sb.append(controller.generatePersistenceReport()).append("\n");

        sb.append("=== Turmas e Avaliações ===\n");
        List<AcademicClass> classes = controller.listClasses();
        if (classes.isEmpty()) {
            sb.append("Nenhuma turma cadastrada.\n");
        } else {
            for (AcademicClass cls : classes) {
                sb.append(controller.generateClassSummary(cls.getCode())).append("\n");
            }
        }

        txtReport.setText(sb.toString());
    }

    @FXML
    private void goBack() {
        SceneManager.switchScene("MainScreen.fxml", "Sistema Acadêmico");
    }
}
