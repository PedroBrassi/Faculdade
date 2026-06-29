package org.example.academic.system.gui;

import javafx.application.Application;
import javafx.stage.Stage;
import org.example.academic.system.controller.AcademicSystemController;
import org.example.academic.system.controller.AuthenticationController;
import org.example.academic.system.repository.InMemoryAcademicClassRepository;
import org.example.academic.system.repository.TxtUserRepository;
import org.example.academic.system.security.AuthenticationService;
import org.example.academic.system.service.AcademicService;
import org.example.academic.system.service.PersistenceService;
import org.example.academic.system.service.ReportService;

/**
 * Ponto de entrada da interface gráfica JavaFX (TUS-2406).
 *
 * <p>Reutiliza exatamente o mesmo wiring de services/controllers da aplicação
 * de linha de comando ({@code org.example.academic.system.Main}), que continua
 * funcionando de forma independente. A GUI não contém regra de negócio: as
 * telas delegam ao {@link AcademicSystemController} e ao
 * {@link AuthenticationController} via {@link AppContext}.</p>
 */
public class JavaFXMain extends Application {

    @Override
    public void init() {
        // Infraestrutura (mesmo wiring de AcademicSystem.run()).
        InMemoryAcademicClassRepository memoryRepo = new InMemoryAcademicClassRepository();
        TxtUserRepository userRepository = new TxtUserRepository("users.txt");

        // Segurança.
        AuthenticationService authService = new AuthenticationService(userRepository);

        // Serviços.
        PersistenceService persistenceService = new PersistenceService(memoryRepo);
        AcademicService academicService = new AcademicService(memoryRepo);
        ReportService reportService = new ReportService();

        // Controllers reutilizados pela GUI.
        AcademicSystemController academicSystemController = new AcademicSystemController(
                academicService, authService, persistenceService, reportService);
        AuthenticationController authenticationController = new AuthenticationController(authService);

        AppContext.init(academicSystemController, authenticationController);
    }

    @Override
    public void start(Stage stage) {
        SceneManager.setStage(stage);
        SceneManager.switchScene("Login.fxml", "Sistema Acadêmico - Login");
        stage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}
