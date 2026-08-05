package org.example.academic.system;

import org.example.academic.system.exception.AcademicSystemException;
import org.example.academic.system.model.AcademicClass;
import org.example.academic.system.repository.InMemoryAcademicClassRepository;
import org.example.academic.system.service.ClassService;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;

@DisplayName("TUS-2401 - ClassService behavior")
class ClassServiceTest {

    private InMemoryAcademicClassRepository repository;
    private ClassService classService;

    @BeforeEach
    void setUp() {
        repository = new InMemoryAcademicClassRepository();
        classService = new ClassService(repository);
    }

    @Test
    @DisplayName("registra uma turma válida e a armazena no repositório")
    void shouldRegisterValidClass() {
        classService.registerClass("CS101", "Introduction to OOP");

        Optional<AcademicClass> stored = repository.findByCode("CS101");
        assertTrue(stored.isPresent(), "Turma deve ficar armazenada no AcademicSystem");
        assertEquals("CS101", stored.get().getCode());
        assertEquals("Introduction to OOP", stored.get().getTitle());
    }

    @Test
    @DisplayName("código de turma inválido lança AcademicSystemException")
    void shouldRejectInvalidClassCode() {
        assertThrows(AcademicSystemException.class,
                () -> classService.registerClass("invalid code", "Some title"));
    }

    @Test
    @DisplayName("título de turma em branco lança AcademicSystemException")
    void shouldRejectBlankTitle() {
        assertThrows(AcademicSystemException.class,
                () -> classService.registerClass("CS101", ""));
    }

    @Test
    @DisplayName("turma inválida não é armazenada")
    void invalidClassMustNotBeStored() {
        assertThrows(AcademicSystemException.class,
                () -> classService.registerClass("bad", ""));
        assertTrue(repository.findAll().isEmpty());
    }
}
