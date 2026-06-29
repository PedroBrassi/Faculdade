package org.example.academic.system.gui;

import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import org.example.academic.system.exception.AuthenticationException;
import org.example.academic.system.model.User;

/**
 * Controller JavaFX da tela de login (TUS-2407).
 *
 * <p>Não contém lógica de autenticação: delega ao
 * {@link org.example.academic.system.controller.AuthenticationController}
 * (via {@link AppContext}) e, em caso de sucesso, navega para a tela principal.
 * Em caso de falha, exibe uma mensagem de erro. A senha nunca trafega em texto
 * plano na tela (PasswordField).</p>
 */
public class LoginController {

    @FXML private TextField usernameField;
    @FXML private PasswordField passwordField;
    @FXML private Label errorLabel;

    @FXML
    private void handleLogin() {
        errorLabel.setText("");
        String username = usernameField.getText() == null ? "" : usernameField.getText().trim();
        String password = passwordField.getText() == null ? "" : passwordField.getText();

        try {
            User user = AppContext.auth().authenticate(username, password);
            SceneManager.switchScene("MainScreen.fxml",
                    "Sistema Acadêmico - " + user.getUsername() + " [" + user.getRole() + "]");
        } catch (AuthenticationException e) {
            errorLabel.setText("Usuário ou senha inválidos.");
        }
    }
}
