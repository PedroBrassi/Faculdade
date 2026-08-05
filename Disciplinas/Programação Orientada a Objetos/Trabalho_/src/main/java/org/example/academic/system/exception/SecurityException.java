package org.example.academic.system.exception;

/** Exceção base para todos os erros de segurança (US-2369). */
public class SecurityException extends RuntimeException {
    public SecurityException(String message) { super(message); }
}
