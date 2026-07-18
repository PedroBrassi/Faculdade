package org.example.academic.system.exception;

/** Lançada quando os dados de uma avaliação são inválidos (US-2367, US-2361 AC5/AC6). */
public class InvalidAssessmentException extends AcademicSystemException {
    public InvalidAssessmentException(String message) { super(message); }
}
