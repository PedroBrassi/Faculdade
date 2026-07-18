package org.example.academic.system;

import org.example.academic.system.model.PersistenceType;
import org.example.academic.system.repository.AcademicClassRepository;
import org.example.academic.system.repository.InMemoryAcademicClassRepository;
import org.example.academic.system.service.ClassService;
import org.example.academic.system.service.PersistenceService;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.*;

@DisplayName("TUS-2403 - PersistenceService behavior")
class PersistenceServiceTest {

    @TempDir
    Path tempDir;

    private InMemoryAcademicClassRepository memoryRepo;
    private PersistenceService persistenceService;

    @BeforeEach
    void setUp() {
        memoryRepo = new InMemoryAcademicClassRepository();
        persistenceService = new PersistenceService(memoryRepo);

        ClassService classService = new ClassService(memoryRepo);
        classService.registerClass("CS101", "Introduction to OOP");
    }

    private String persistToTempFile(String fileName) throws IOException {
        AcademicClassRepository repo = persistenceService.buildRepository();
        Path file = tempDir.resolve(fileName);
        repo.persistAll(memoryRepo.findAll(), file.toString());
        assertTrue(Files.exists(file), "Arquivo de persistência deve existir");
        return Files.readString(file);
    }

    @Test
    @DisplayName("salva dados usando o repositório TXT padrão e o arquivo contém os dados")
    void shouldSaveUsingDefaultTxtRepository() throws IOException {
        assertEquals(PersistenceType.TXT, persistenceService.getCurrentType());

        String content = persistToTempFile("data.txt");
        assertTrue(content.contains("CS101"));
        assertTrue(content.contains("Introduction to OOP"));
    }

    @Test
    @DisplayName("troca o tipo de persistência para XML e salva os dados")
    void shouldChangePersistenceTypeToXml() throws IOException {
        persistenceService.setPersistenceType(PersistenceType.XML);
        assertEquals(PersistenceType.XML, persistenceService.getCurrentType());

        String content = persistToTempFile("data.xml");
        assertTrue(content.contains("CS101"));
    }

    @Test
    @DisplayName("troca o tipo de persistência para JSON e salva os dados")
    void shouldChangePersistenceTypeToJson() throws IOException {
        persistenceService.setPersistenceType(PersistenceType.JSON);
        assertEquals(PersistenceType.JSON, persistenceService.getCurrentType());

        String content = persistToTempFile("data.json");
        assertTrue(content.contains("CS101"));
    }
}
