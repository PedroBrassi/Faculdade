package org.example.academic.system.exception;

/** Lançada quando a autenticação falha por credenciais inválidas (US-2369 AC1). */
public class AuthenticationException extends SecurityException {
    public AuthenticationException() {
        super("Authentication failed: invalid username or password.");
    }
    public AuthenticationException(String message) {
        super(message);
    }
}
