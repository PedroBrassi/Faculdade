package org.example.academic.system.exception;

/** Lançada quando um código de turma não é encontrado no sistema (US-2367, US-2361 AC4). */
public class ClassNotFoundException extends AcademicSystemException {
    public ClassNotFoundException(String code) {
        super("Academic class not found with code: " + code);
    }
}
