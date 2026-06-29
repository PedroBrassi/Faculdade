package org.example.academic.system.report;

import org.example.academic.system.model.PersistenceType;

/**
 * Relatório do tipo de persistência ativo (US-2372 AC8).
 */
public class PersistenceConfigReport {

    private final PersistenceType currentType;

    public PersistenceConfigReport(PersistenceType currentType) {
        this.currentType = currentType;
    }

    public void print() {
        System.out.println("=== Persistence Configuration Report ===");
        System.out.println("Active persistence type : " + currentType);
        System.out.println("========================================");
    }
}
