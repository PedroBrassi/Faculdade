package org.example.academic.system.controller;

import org.example.academic.system.model.AcademicClass;
import org.example.academic.system.model.PersistenceType;
import org.example.academic.system.model.Role;
import org.example.academic.system.security.AuthenticationService;
import org.example.academic.system.service.AcademicService;
import org.example.academic.system.service.PersistenceService;
import org.example.academic.system.service.ReportService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.List;
import java.util.Optional;

/**
 * Controller responsável por todas as operações acadêmicas (TUS-2370).
 * Separado do modelo de domínio, da camada de segurança e da entrada via teclado.
 */
public class AcademicSystemController {

    private static final Logger log = LoggerFactory.getLogger(AcademicSystemController.class);

    private final AcademicService academicService;
    private final AuthenticationService authService;
    private final PersistenceService persistenceService;
    private final ReportService reportService;

    public AcademicSystemController(AcademicService academicService,
                                    AuthenticationService authService,
                                    PersistenceService persistenceService,
                                    ReportService reportService) {
        this.academicService = academicService;
        this.authService = authService;
        this.persistenceService = persistenceService;
        this.reportService = reportService;
    }

    /** Cadastra uma nova turma. Requer perfil ADMIN (US-2363 AC5). */
    public void registerClass(String code, String title) {
        authService.requireRole(Role.ADMIN);
        academicService.registerClass(code, title);
        log.info("Class registration delegated to service: code='{}', title='{}'", code, title);
    }

    /** Registra uma avaliação em uma turma. Acessível por ADMIN e PROFESSOR. */
    public void registerAssessment(String classCode, int type, double value, double weight) {
        academicService.registerAssessment(classCode, type, value, weight);
        log.info("Assessment registration delegated to service for class '{}'", classCode);
    }

    /**
     * Persiste todas as turmas usando o mecanismo de persistência configurado.
     * Requer perfil ADMIN.
     */
    public void saveData(String fileName) {
        authService.requireRole(Role.ADMIN);
        var repo = persistenceService.buildRepository();
        repo.persistAll(academicService.listAllClasses(), fileName);
        log.info("Data persistence delegated (type={})", persistenceService.getCurrentType());
    }

    public Optional<AcademicClass> findClassByCode(String code) {
        return academicService.findByCode(code);
    }

    public List<AcademicClass> listClasses() {
        return academicService.listAllClasses();
    }

    /** Altera o tipo de persistência. Requer perfil ADMIN (US-2372 AC4). */
    public void configurePersistence(PersistenceType type) {
        authService.requireRole(Role.ADMIN);
        persistenceService.setPersistenceType(type);
        log.info("Persistence configuration updated to '{}' by user '{}'",
                type, authService.getCurrentUser().getUsername());
    }

    /** Retorna o tipo de persistência atualmente configurado (US-2372 AC8). */
    public PersistenceType getCurrentPersistenceType() {
        return persistenceService.getCurrentType();
    }

    /** Gera o relatório da configuração de persistência. */
    public String generatePersistenceReport() {
        Role role = authService.getCurrentUser() != null
                ? authService.getCurrentUser().getRole()
                : null;
        return reportService.generatePersistenceConfigurationReport(
                persistenceService.getCurrentType(), role);
    }

    /** Gera o sumário de avaliações de uma turma. */
    public String generateClassSummary(String classCode) {
        Role role = authService.getCurrentUser() != null
                ? authService.getCurrentUser().getRole()
                : null;
        return academicService.findByCode(classCode)
                .map(cls -> reportService.generateClassAssessmentSummary(cls, role))
                .orElse("Class not found: " + classCode);
    }
}
