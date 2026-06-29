package org.example.academic.system;

import jakarta.validation.Validation;
import jakarta.validation.Validator;
import jakarta.validation.ValidatorFactory;
import org.example.academic.system.model.Assessment;
import org.example.academic.system.model.Exam;
import org.example.academic.system.model.AcademicClass;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

@DisplayName("TUS-2385 - Validação do domínio acadêmico")
class DomainValidationTest {

    private static ValidatorFactory factory;
    private static Validator validator;

    @BeforeAll
    static void setUp() {
        factory = Validation.buildDefaultValidatorFactory();
        validator = factory.getValidator();
    }

    @AfterAll
    static void tearDown() {
        factory.close();
    }

    @Test
    @DisplayName("turma válida passa na validação")
    void validClassShouldPass() {
        AcademicClass academicClass = new AcademicClass("CC101", "Introdução a POO");
        assertTrue(validator.validate(academicClass).isEmpty());
    }

    @Test
    @DisplayName("turma com código em branco falha")
    void blankCodeShouldFail() {
        AcademicClass academicClass = new AcademicClass("   ", "Introdução a POO");
        assertFalse(validator.validate(academicClass).isEmpty());
    }

    @Test
    @DisplayName("turma com título em branco falha")
    void blankTitleShouldFail() {
        AcademicClass academicClass = new AcademicClass("CC101", "");
        assertFalse(validator.validate(academicClass).isEmpty());
    }

    @Test
    @DisplayName("avaliação com valor inválido falha")
    void invalidValueShouldFail() {
        Assessment assessment = new Exam(11.0, 0.5);
        assertFalse(validator.validate(assessment).isEmpty());
    }

    @Test
    @DisplayName("avaliação com peso inválido falha")
    void invalidWeightShouldFail() {
        Assessment assessment = new Exam(8.0, 2.0);
        assertFalse(validator.validate(assessment).isEmpty());
    }
}
