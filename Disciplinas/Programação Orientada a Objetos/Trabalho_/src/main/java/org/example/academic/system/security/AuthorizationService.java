package org.example.academic.system.security;

import org.example.academic.system.exception.AuthorizationException;
import org.example.academic.system.logging.SecurityAuditLogger;
import org.example.academic.system.model.Role;
import org.example.academic.system.model.User;

/**
 * Serviço de autorização independente (TUS-2392).
 * Permite checagens de perfil sem depender do estado de sessão.
 */
public class AuthorizationService {

    public boolean isAuthorized(User user, Role requiredRole) {
        return user != null && user.getRole() == requiredRole;
    }

    /**
     * Exige que o usuário possua o perfil necessário para a operação.
     * O log é registrado antes de lançar a exceção (TUS-2392).
     */
    public void requireRole(User user, Role requiredRole, String operation) {
        if (!isAuthorized(user, requiredRole)) {
            Role role = (user == null) ? null : user.getRole();
            SecurityAuditLogger.authorizationFailure(role, operation);
            throw new AuthorizationException("Access denied for operation: " + operation);
        }
    }
}
