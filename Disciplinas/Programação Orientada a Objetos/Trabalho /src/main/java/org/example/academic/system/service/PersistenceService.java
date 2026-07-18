package org.example.academic.system.service;

import org.example.academic.system.model.PersistenceType;
import org.example.academic.system.repository.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Gerencia a configuração de persistência ativa (US-2372).
 * Permite trocar entre TXT, XML e JSON em tempo de execução.
 */
public class PersistenceService {

    private static final Logger log = LoggerFactory.getLogger(PersistenceService.class);

    private PersistenceType currentType;
    private final InMemoryAcademicClassRepository memoryRepository;

    public PersistenceService(InMemoryAcademicClassRepository memoryRepository) {
        this.memoryRepository = memoryRepository;
        this.currentType = PersistenceType.TXT; // padrão
    }

    public PersistenceType getCurrentType() {
        return currentType;
    }

    /** Altera o tipo de persistência ativo (US-2372 AC1). */
    public void setPersistenceType(PersistenceType type) {
        this.currentType = type;
        log.info("Persistence type changed to: {}", type);
    }

    /** Retorna o repositório configurado para o tipo de persistência atual. */
    public AcademicClassRepository buildRepository() {
        return switch (currentType) {
            case TXT  -> new TxtAcademicClassRepository(memoryRepository);
            case XML  -> new XmlAcademicClassRepository(memoryRepository);
            case JSON -> new JsonAcademicClassRepository(memoryRepository);
        };
    }
}
