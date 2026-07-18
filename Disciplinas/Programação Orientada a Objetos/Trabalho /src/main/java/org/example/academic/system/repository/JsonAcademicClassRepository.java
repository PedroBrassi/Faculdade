package org.example.academic.system.repository;

import org.example.academic.system.exception.AcademicSystemException;
import org.example.academic.system.logging.PersistenceAuditLogger;
import org.example.academic.system.model.AcademicClass;
import org.example.academic.system.model.Assessment;
import org.example.academic.system.model.PersistenceType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.FileWriter;
import java.io.IOException;
import java.util.List;
import java.util.Optional;

/**
 * Persistência em arquivo JSON para turmas e avaliações (US-2372 AC2).
 */
public class JsonAcademicClassRepository implements AcademicClassRepository {

    private static final Logger log = LoggerFactory.getLogger(JsonAcademicClassRepository.class);

    private final InMemoryAcademicClassRepository memoryDelegate;

    public JsonAcademicClassRepository(InMemoryAcademicClassRepository memoryDelegate) {
        this.memoryDelegate = memoryDelegate;
    }

    @Override
    public void save(AcademicClass academicClass) {
        memoryDelegate.save(academicClass);
    }

    @Override
    public Optional<AcademicClass> findByCode(String code) {
        return memoryDelegate.findByCode(code);
    }

    @Override
    public List<AcademicClass> findAll() {
        return memoryDelegate.findAll();
    }

    @Override
    public void persistAll(List<AcademicClass> classes, String fileName) {
        log.info("Persisting {} class(es) to JSON file: {}", classes.size(), fileName);
        try (FileWriter writer = new FileWriter(fileName)) {
            writer.write("[\n");
            for (int i = 0; i < classes.size(); i++) {
                AcademicClass cls = classes.get(i);
                writer.write("  {\n");
                writer.write("    \"code\": \"" + cls.getCode() + "\",\n");
                writer.write("    \"title\": \"" + cls.getTitle() + "\",\n");
                writer.write("    \"assessments\": [\n");
                List<Assessment> assessments = cls.getAssessments();
                for (int j = 0; j < assessments.size(); j++) {
                    Assessment a = assessments.get(j);
                    writer.write("      {\"type\": \"" + a.getType()
                            + "\", \"value\": " + a.getValue()
                            + ", \"weight\": " + a.getWeight() + "}");
                    writer.write(j < assessments.size() - 1 ? ",\n" : "\n");
                }
                writer.write("    ]\n");
                writer.write("  }");
                writer.write(i < classes.size() - 1 ? ",\n" : "\n");
            }
            writer.write("]\n");
            log.info("JSON persistence completed successfully.");
        } catch (IOException e) {
            log.error("Failed to persist classes to JSON file: {}", fileName, e);
            throw new AcademicSystemException("Failed to save data to JSON file: " + e.getMessage(), e);
        }
        PersistenceAuditLogger.saved(PersistenceType.JSON, fileName);
    }
}
