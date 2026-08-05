package org.example.academic.system.logging;

import org.example.academic.system.model.PersistenceType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * TUS-2393 — log das operações de persistência.
 */
public final class PersistenceAuditLogger {

    private static final Logger log = LoggerFactory.getLogger(PersistenceAuditLogger.class);

    private PersistenceAuditLogger() {}

    public static void saved(PersistenceType type, String fileName) {
        log.info("Academic data saved using {} persistence to file '{}'", type.name(), fileName);
    }
}
