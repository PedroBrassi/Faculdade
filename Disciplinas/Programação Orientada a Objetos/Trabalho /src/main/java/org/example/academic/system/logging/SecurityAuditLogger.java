package org.example.academic.system.logging;

import org.example.academic.system.model.Role;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * TUS-2391 / TUS-2392 — log de eventos de segurança.
 * A senha NUNCA é registrada, apenas o usuário e o perfil.
 */
public final class SecurityAuditLogger {

    private static final Logger log = LoggerFactory.getLogger(SecurityAuditLogger.class);

    private SecurityAuditLogger() {}

    public static void loginSuccess(String username) {
        log.info("Login succeeded for user '{}'", safe(username));
    }

    public static void loginFailure(String username) {
        log.warn("Login failed for user '{}'", safe(username));
    }

    public static void logout(String username) {
        log.info("Logout for user '{}'", safe(username));
    }

    public static void authorizationFailure(Role role, String operation) {
        String r = (role == null) ? "UNKNOWN" : role.name();
        log.warn("Authorization denied for role {} on operation '{}'", r, operation);
    }

    private static String safe(String username) {
        return (username == null || username.isBlank()) ? "unknown" : username;
    }
}
