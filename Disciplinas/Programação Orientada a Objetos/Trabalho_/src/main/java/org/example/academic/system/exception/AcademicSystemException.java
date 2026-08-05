package org.example.academic.system.exception;

/**
 * Exceção base para todos os erros do domínio acadêmico (US-2367).
 */
public class AcademicSystemException extends RuntimeException {

    public AcademicSystemException(String message) {
        super(message);
    }

    public AcademicSystemException(String message, Throwable cause) {
        super(message, cause);
    }
}
