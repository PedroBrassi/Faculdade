package org.example.academic.system;

import org.example.academic.system.controller.AcademicSystemController;
import org.example.academic.system.exception.AuthorizationException;
import org.example.academic.system.model.AcademicClass;
import org.example.academic.system.repository.InMemoryAcademicClassRepository;
import org.example.academic.system.repository.TxtUserRepository;
import org.example.academic.system.security.AuthenticationService;
import org.example.academic.system.service.AcademicService;
import org.example.academic.system.service.PersistenceService;
import org.example.academic.system.service.ReportService;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

@DisplayName("TUS-2405 - AcademicSystemController delegation behavior")
class AcademicSystemControllerTest {

    private InMemoryAcademicClassRepository repository;
    private AuthenticationService authService;
    private AcademicSystemController controller;

    @BeforeEach
    void setUp() {
        repository = new InMemoryAcademicClassRepository();
        AcademicService academicService = new AcademicService(repository);
        // Arquivo inexistente -> carrega usuários padrão: admin/admin123 e professor/prof123
        authService = new AuthenticationService(new TxtUserRepository("nonexistent-users.txt"));
        PersistenceService persistenceService = new PersistenceService(repository);
        ReportService reportService = new ReportService();

        controller = new AcademicSystemController(
                academicService, authService, persistenceService, reportService);
    }

    @Test
    @DisplayName("registra turmas por delegação ao service")
    void shouldRegisterClassThroughServiceDelegation() {
        authService.authenticate("admin", "admin123");

        controller.registerClass("CS101", "Introduction to OOP");

        assertTrue(repository.findByCode("CS101").isPresent());
    }

    @Test
    @DisplayName("registra avaliações por delegação ao service")
    void shouldRegisterAssessmentThroughServiceDelegation() {
        authService.authenticate("admin", "admin123");
        controller.registerClass("CS101", "Introduction to OOP");

        controller.registerAssessment("CS101", 1, 8.0, 0.4);

        AcademicClass cls = repository.findByCode("CS101").orElseThrow();
        assertEquals(1, cls.getAssessments().size());
        assertEquals("Exam", cls.getAssessments().get(0).getType());
    }

    @Test
    @DisplayName("preserva as regras de autorização: PROFESSOR não pode cadastrar turma")
    void shouldPreserveAuthorizationRulesForProfessor() {
        authService.authenticate("professor", "prof123");

        assertThrows(AuthorizationException.class,
                () -> controller.registerClass("CS101", "Introduction to OOP"));
        assertTrue(repository.findAll().isEmpty());
    }

    @Test
    @DisplayName("preserva as regras de autorização: usuário não autenticado não pode cadastrar turma")
    void shouldPreserveAuthorizationRulesForAnonymous() {
        assertThrows(AuthorizationException.class,
                () -> controller.registerClass("CS101", "Introduction to OOP"));
    }
}
