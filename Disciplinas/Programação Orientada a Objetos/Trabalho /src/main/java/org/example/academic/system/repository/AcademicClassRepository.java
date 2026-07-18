package org.example.academic.system.repository;

import org.example.academic.system.model.AcademicClass;

import java.util.List;
import java.util.Optional;

/**
 * Abstração de repositório para persistência de AcademicClass (TUS-2362 AC5).
 */
public interface AcademicClassRepository {

    void save(AcademicClass academicClass);

    Optional<AcademicClass> findByCode(String code);

    List<AcademicClass> findAll();

    /**
     * Persiste todas as turmas no mecanismo configurado.
     *
     * @param classes lista de turmas a persistir
     * @param fileName nome do arquivo destino (usado por TXT, XML e JSON)
     */
    void persistAll(List<AcademicClass> classes, String fileName);
}
