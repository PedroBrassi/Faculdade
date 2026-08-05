package org.example.academic.system;

import org.example.academic.system.exception.AcademicSystemException;
import org.example.academic.system.model.AcademicClass;
import org.example.academic.system.repository.InMemoryAcademicClassRepository;
import org.example.academic.system.service.AssessmentService;
import org.example.academic.system.service.ClassService;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

@DisplayName("TUS-2402 - AssessmentService behavior")
class AssessmentServiceTest {

    private InMemoryAcademicClassRepository repository;
    private ClassService classService;
    private AssessmentService assessmentService;

    @BeforeEach
    void setUp() {
        repository = new InMemoryAcademicClassRepository();
        classService = new ClassService(repository);
        assessmentService = new AssessmentService(repository);
        classService.registerClass("CS101", "Introduction to OOP");
    }

    @Test
    @DisplayName("registra uma avaliação em uma turma existente e a armazena na turma")
    void shouldRegisterAssessmentInExistingClass() {
        assessmentService.registerAssessment("CS101", 1, 8.0, 0.4); // Exam

        AcademicClass cls = repository.findByCode("CS101").orElseThrow();
        assertEquals(1, cls.getAssessments().size());
        assertEquals("Exam", cls.getAssessments().get(0).getType());
        assertEquals(8.0, cls.getAssessments().get(0).getValue());
        assertEquals(0.4, cls.getAssessments().get(0).getWeight());
    }

    @Test
    @DisplayName("tipo de avaliação inválido não adiciona avaliação")
    void invalidAssessmentTypeMustNotAddAssessment() {
        assertThrows(AcademicSystemException.class,
                () -> assessmentService.registerAssessment("CS101", 99, 8.0, 0.4));

        AcademicClass cls = repository.findByCode("CS101").orElseThrow();
        assertTrue(cls.getAssessments().isEmpty());
    }

    @Test
    @DisplayName("código de turma inexistente não adiciona avaliação")
    void nonexistentClassCodeMustNotAddAssessment() {
        assertThrows(AcademicSystemException.class,
                () -> assessmentService.registerAssessment("ZZ999", 1, 8.0, 0.4));

        assertTrue(repository.findByCode("ZZ999").isEmpty());
    }

    @Test
    @DisplayName("valores fora da faixa não adicionam avaliação (validação de domínio)")
    void outOfRangeValueMustNotAddAssessment() {
        assertThrows(AcademicSystemException.class,
                () -> assessmentService.registerAssessment("CS101", 1, 50.0, 0.4));

        AcademicClass cls = repository.findByCode("CS101").orElseThrow();
        assertTrue(cls.getAssessments().isEmpty());
    }
}
