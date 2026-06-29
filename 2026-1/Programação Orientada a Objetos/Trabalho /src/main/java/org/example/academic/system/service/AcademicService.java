package org.example.academic.system.service;

import org.example.academic.system.model.AcademicClass;
import org.example.academic.system.repository.AcademicClassRepository;

import java.util.List;
import java.util.Optional;

/**
 * Fachada de negócio para turmas e avaliações.
 *
 * <p>Delega às responsabilidades coesas de {@link ClassService} e
 * {@link AssessmentService} (TUS-2400), mantendo uma API única e estável para
 * o {@code AcademicSystemController} e para a camada de apresentação (CLI/GUI).</p>
 */
public class AcademicService {

    private final ClassService classService;
    private final AssessmentService assessmentService;
    private final AcademicClassRepository repository;

    public AcademicService(AcademicClassRepository repository) {
        this.repository = repository;
        this.classService = new ClassService(repository);
        this.assessmentService = new AssessmentService(repository);
    }

    /** Cadastra uma nova turma após validação (US-2363). */
    public void registerClass(String code, String title) {
        classService.registerClass(code, title);
    }

    /**
     * Registra uma avaliação em uma turma existente (US-2361).
     *
     * @param classCode      código da turma
     * @param assessmentType 1=Exam, 2=PracticalAssignment, 3=Seminar, 4=Assignment
     * @param value          nota (0-10)
     * @param weight         peso (0-1)
     */
    public void registerAssessment(String classCode, int assessmentType, double value, double weight) {
        assessmentService.registerAssessment(classCode, assessmentType, value, weight);
    }

    public Optional<AcademicClass> findByCode(String code) {
        return repository.findByCode(code);
    }

    public List<AcademicClass> listAllClasses() {
        return repository.findAll();
    }
}
