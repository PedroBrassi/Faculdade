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
 * Persistência em arquivo XML para turmas e avaliações (US-2372 AC2).
 */
public class XmlAcademicClassRepository implements AcademicClassRepository {

    private static final Logger log = LoggerFactory.getLogger(XmlAcademicClassRepository.class);

    private final InMemoryAcademicClassRepository memoryDelegate;

    public XmlAcademicClassRepository(InMemoryAcademicClassRepository memoryDelegate) {
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
        log.info("Persisting {} class(es) to XML file: {}", classes.size(), fileName);
        try (FileWriter writer = new FileWriter(fileName)) {
            writer.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
            writer.write("<classes>\n");
            for (AcademicClass cls : classes) {
                writer.write("  <class code=\"" + escape(cls.getCode())
                        + "\" title=\"" + escape(cls.getTitle()) + "\">\n");
                for (Assessment a : cls.getAssessments()) {
                    writer.write("    <assessment type=\"" + a.getType()
                            + "\" value=\"" + a.getValue()
                            + "\" weight=\"" + a.getWeight() + "\"/>\n");
                }
                writer.write("  </class>\n");
            }
            writer.write("</classes>\n");
            log.info("XML persistence completed successfully.");
        } catch (IOException e) {
            log.error("Failed to persist classes to XML file: {}", fileName, e);
            throw new AcademicSystemException("Failed to save data to XML file: " + e.getMessage(), e);
        }
        PersistenceAuditLogger.saved(PersistenceType.XML, fileName);
    }

    private String escape(String value) {
        if (value == null) return "";
        return value.replace("&", "&amp;")
                    .replace("<", "&lt;")
                    .replace(">", "&gt;")
                    .replace("\"", "&quot;");
    }
}
