package org.example.academic.system.exception;

/** Lançada quando um usuário tenta uma operação fora do seu perfil (US-2369 AC2). */
public class AuthorizationException extends SecurityException {
    public AuthorizationException(String detail) {
        super("Access denied: " + detail);
    }
}
