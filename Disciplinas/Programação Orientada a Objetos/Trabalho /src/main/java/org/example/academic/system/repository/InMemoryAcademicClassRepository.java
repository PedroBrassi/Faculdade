package org.example.academic.system.repository;

import org.example.academic.system.model.AcademicClass;

import java.util.*;

/**
 * Repositório em memória — armazenamento primário em runtime.
 */
public class InMemoryAcademicClassRepository implements AcademicClassRepository {

    private final Map<String, AcademicClass> store = new LinkedHashMap<>();

    @Override
    public void save(AcademicClass academicClass) {
        store.put(academicClass.getCode(), academicClass);
    }

    @Override
    public Optional<AcademicClass> findByCode(String code) {
        return Optional.ofNullable(store.get(code));
    }

    @Override
    public List<AcademicClass> findAll() {
        return new ArrayList<>(store.values());
    }

    /** Operação sem efeito para in-memory; a persistência é feita pelos repositórios de arquivo. */
    @Override
    public void persistAll(List<AcademicClass> classes, String fileName) {
        // no-op
    }
}
