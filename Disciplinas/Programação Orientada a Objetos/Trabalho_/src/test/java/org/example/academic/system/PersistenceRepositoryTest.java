package org.example.academic.system;

import org.example.academic.system.model.AcademicClass;
import org.example.academic.system.model.Exam;
import org.example.academic.system.repository.AcademicClassRepository;
import org.example.academic.system.repository.InMemoryAcademicClassRepository;
import org.example.academic.system.repository.JsonAcademicClassRepository;
import org.example.academic.system.repository.TxtAcademicClassRepository;
import org.example.academic.system.repository.XmlAcademicClassRepository;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.assertTrue;

@DisplayName("US-2389 - Repositórios de persistência (TXT/XML/JSON)")
class PersistenceRepositoryTest {

    @TempDir
    Path tempDir;

    private InMemoryAcademicClassRepository memory;

    @BeforeEach
    void setUp() {
        memory = new InMemoryAcademicClassRepository();
        AcademicClass academicClass = new AcademicClass("CC101", "Introdução a POO");
        academicClass.addAssessment(new Exam(8.0, 0.4));
        memory.save(academicClass);
    }

    @Test
    @DisplayName("repositório TXT gera arquivo com os dados")
    void txtRepositoryShouldGenerateFile() throws IOException {
        AcademicClassRepository repository = new TxtAcademicClassRepository(memory);
        Path file = tempDir.resolve("classes.txt");
        repository.persistAll(memory.findAll(), file.toString());

        assertTrue(Files.exists(file));
        String content = Files.readString(file);
        assertTrue(content.contains("CC101"));
        assertTrue(content.contains("Exam"));
    }

    @Test
    @DisplayName("repositório XML gera arquivo com os dados")
    void xmlRepositoryShouldGenerateFile() throws IOException {
        AcademicClassRepository repository = new XmlAcademicClassRepository(memory);
        Path file = tempDir.resolve("classes.xml");
        repository.persistAll(memory.findAll(), file.toString());

        assertTrue(Files.exists(file));
        String content = Files.readString(file);
        assertTrue(content.contains("CC101"));
        assertTrue(content.contains("Exam"));
    }

    @Test
    @DisplayName("repositório JSON gera arquivo com os dados")
    void jsonRepositoryShouldGenerateFile() throws IOException {
        AcademicClassRepository repository = new JsonAcademicClassRepository(memory);
        Path file = tempDir.resolve("classes.json");
        repository.persistAll(memory.findAll(), file.toString());

        assertTrue(Files.exists(file));
        String content = Files.readString(file);
        assertTrue(content.contains("CC101"));
        assertTrue(content.contains("Exam"));
    }
}
