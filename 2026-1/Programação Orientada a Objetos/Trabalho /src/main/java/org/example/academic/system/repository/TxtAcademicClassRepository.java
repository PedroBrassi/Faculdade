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
 * Persistência em arquivo TXT para turmas e avaliações (TUS-2362).
 *
 * Formato por linha:
 *   Class Code: &lt;code&gt;
 *   Class Title: &lt;title&gt;
 *   Assessment: &lt;type&gt; | value=&lt;v&gt; | weight=&lt;w&gt;
 *   (linha em branco entre turmas)
 */
public class TxtAcademicClassRepository implements AcademicClassRepository {

    private static final Logger log = LoggerFactory.getLogger(TxtAcademicClassRepository.class);

    private final InMemoryAcademicClassRepository memoryDelegate;

    public TxtAcademicClassRepository(InMemoryAcademicClassRepository memoryDelegate) {
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
        log.info("Persisting {} class(es) to TXT file: {}", classes.size(), fileName);
        try (FileWriter writer = new FileWriter(fileName)) {
            for (AcademicClass cls : classes) {
                writer.write("Class Code: " + cls.getCode() + "\n");
                writer.write("Class Title: " + cls.getTitle() + "\n");
                for (Assessment a : cls.getAssessments()) {
                    writer.write("Assessment: " + a.getType()
                            + " | value=" + a.getValue()
                            + " | weight=" + a.getWeight() + "\n");
                }
                writer.write("\n");
            }
            log.info("TXT persistence completed successfully.");
        } catch (IOException e) {
            log.error("Failed to persist classes to TXT file: {}", fileName, e);
            throw new AcademicSystemException("Failed to save data to TXT file: " + e.getMessage(), e);
        }
        PersistenceAuditLogger.saved(PersistenceType.TXT, fileName);
    }
}
