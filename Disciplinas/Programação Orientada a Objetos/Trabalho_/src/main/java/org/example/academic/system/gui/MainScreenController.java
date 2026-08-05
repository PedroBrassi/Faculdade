package org.example.academic.system.gui;

import javafx.fxml.FXML;
import javafx.scene.control.Label;
import org.example.academic.system.model.User;

/**
 * Tela principal por perfil (TUS-2408).
 *
 * <p>Exibe o usuário autenticado e dá acesso às demais telas. O controle de
 * acesso por perfil é garantido pelo backend: operações restritas a ADMIN
 * (cadastrar turma, configurar persistência, salvar dados) lançam
 * {@code AuthorizationException} quando executadas por um professor, e a tela
 * correspondente exibe a mensagem de erro.</p>
 */
public class MainScreenController {

    @FXML private Label lblWelcome;

    @FXML
    public void initialize() {
        if (lblWelcome != null) {
            User user = AppContext.auth().getCurrentUser();
            if (user != null) {
                lblWelcome.setText("Bem-vindo, " + user.getUsername() + " [" + user.getRole() + "]");
            }
        }
    }

    @FXML
    private void openClassRegistration() {
        SceneManager.switchScene("ClassRegistration.fxml", "Cadastro de Turmas");
    }

    @FXML
    private void openAssessmentRegistration() {
        SceneManager.switchScene("AssessmentRegistration.fxml", "Cadastro de Avaliações");
    }

    @FXML
    private void openPersistence() {
        SceneManager.switchScene("PersistenceConfig.fxml", "Configuração de Persistência");
    }

    @FXML
    private void openVisualization() {
        SceneManager.switchScene("DataVisualization.fxml", "Visualização");
    }

    @FXML
    private void openReports() {
        SceneManager.switchScene("Report.fxml", "Relatórios");
    }

    @FXML
    private void logout() {
        AppContext.auth().logout();
        SceneManager.switchScene("Login.fxml", "Sistema Acadêmico - Login");
    }
}
