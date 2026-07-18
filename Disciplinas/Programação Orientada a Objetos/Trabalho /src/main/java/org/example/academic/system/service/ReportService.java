package org.example.academic.system.service;

import org.example.academic.system.logging.ReportAuditLogger;
import org.example.academic.system.model.AcademicClass;
import org.example.academic.system.model.Assessment;
import org.example.academic.system.model.PersistenceType;
import org.example.academic.system.model.Role;

/**
 * Gera relatórios sobre turmas e configuração de persistência (TUS-2394).
 */
public class ReportService {

    /**
     * Gera um sumário das avaliações de uma turma.
     */
    public String generateClassAssessmentSummary(AcademicClass academicClass, Role role) {
        StringBuilder sb = new StringBuilder();
        sb.append("Class Code : ").append(academicClass.getCode()).append("\n");
        sb.append("Class Title: ").append(academicClass.getTitle()).append("\n");
        for (Assessment a : academicClass.getAssessments()) {
            sb.append("  Assessment: ").append(a.getType())
              .append(" | value=").append(a.getValue())
              .append(" | weight=").append(a.getWeight())
              .append("\n");
        }
        ReportAuditLogger.generated("class assessment summary", role);
        return sb.toString();
    }

    /**
     * Calcula o peso total das avaliações de uma turma.
     */
    public double generateAssessmentWeightReport(AcademicClass academicClass, Role role) {
        double total = academicClass.getAssessments().stream()
                .mapToDouble(Assessment::getWeight)
                .sum();
        ReportAuditLogger.generated("assessment weight", role);
        return total;
    }

    /**
     * Retorna o relatório da configuração de persistência atual.
     */
    public String generatePersistenceConfigurationReport(PersistenceType type, Role role) {
        ReportAuditLogger.generated("persistence configuration", role);
        return "Active persistence type: " + type.name() + "\n";
    }
}
