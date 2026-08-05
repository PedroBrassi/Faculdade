package org.example.academic.system.exception;

/** Lançada quando os dados de uma turma falham na validação (US-2367, US-2363 AC3). */
public class InvalidClassException extends AcademicSystemException {
    public InvalidClassException(String message) { super(message); }
}
