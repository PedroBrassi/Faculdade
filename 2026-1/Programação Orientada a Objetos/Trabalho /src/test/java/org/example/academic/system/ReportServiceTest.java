package org.example.academic.system;

import org.example.academic.system.model.AcademicClass;
import org.example.academic.system.model.Exam;
import org.example.academic.system.model.PersistenceType;
import org.example.academic.system.model.Role;
import org.example.academic.system.model.Seminar;
import org.example.academic.system.service.ReportService;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

@DisplayName("TUS-2404 - ReportService behavior")
class ReportServiceTest {

    private ReportService reportService;
    private AcademicClass academicClass;

    @BeforeEach
    void setUp() {
        reportService = new ReportService();
        academicClass = new AcademicClass("CS101", "Introduction to OOP");
        academicClass.addAssessment(new Exam(8.0, 0.4));
        academicClass.addAssessment(new Seminar(9.0, 0.6));
    }

    @Test
    @DisplayName("gera o sumário de avaliações da turma")
    void shouldGenerateClassAssessmentSummary() {
        String summary = reportService.generateClassAssessmentSummary(academicClass, Role.ADMIN);

        assertNotNull(summary);
        assertTrue(summary.contains("CS101"));
        assertTrue(summary.contains("Introduction to OOP"));
        assertTrue(summary.contains("Exam"));
        assertTrue(summary.contains("Seminar"));
    }

    @Test
    @DisplayName("gera o relatório de peso das avaliações")
    void shouldGenerateAssessmentWeightReport() {
        double total = reportService.generateAssessmentWeightReport(academicClass, Role.ADMIN);

        assertEquals(1.0, total, 1e-9);
    }

    @Test
    @DisplayName("gera o relatório da configuração de persistência")
    void shouldGeneratePersistenceConfigurationReport() {
        String report = reportService.generatePersistenceConfigurationReport(PersistenceType.XML, Role.ADMIN);

        assertNotNull(report);
        assertTrue(report.contains("XML"));
    }
}
