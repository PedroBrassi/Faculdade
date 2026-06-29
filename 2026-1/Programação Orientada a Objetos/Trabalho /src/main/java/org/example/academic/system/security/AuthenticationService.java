package org.example.academic.system.security;

import org.example.academic.system.exception.AuthenticationException;
import org.example.academic.system.exception.AuthorizationException;
import org.example.academic.system.logging.SecurityAuditLogger;
import org.example.academic.system.model.Role;
import org.example.academic.system.model.User;
import org.example.academic.system.repository.TxtUserRepository;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Optional;

/**
 * Gerencia autenticação e autorização baseada em perfil (US-2366, US-2369).
 * Completamente separado da lógica do domínio acadêmico (US-2366 AC8).
 * Senhas nunca são logadas (US-2366 AC6).
 */
public class AuthenticationService {

    private static final Logger log = LoggerFactory.getLogger(AuthenticationService.class);

    private final TxtUserRepository userRepository;
    private User currentUser;

    public AuthenticationService(TxtUserRepository userRepository) {
        this.userRepository = userRepository;
    }

    /**
     * Construtor de conveniência: usa o repositório de usuários padrão
     * ({@code users.txt}; se ausente, carrega admin/professor padrão).
     */
    public AuthenticationService() {
        this(new TxtUserRepository("users.txt"));
    }

    /**
     * Autentica o usuário. Lança AuthenticationException em caso de falha.
     */
    public User authenticate(String username, String password) {
        log.info("Authentication attempt for username: '{}'", username);
        Optional<User> found = userRepository.findByUsername(username);
        if (found.isEmpty() || !found.get().getPassword().equals(password)) {
            SecurityAuditLogger.loginFailure(username);
            throw new AuthenticationException();
        }
        currentUser = found.get();
        SecurityAuditLogger.loginSuccess(username);
        log.info("Authentication successful: {}", currentUser);
        return currentUser;
    }

    /** Encerra a sessão do usuário atual. */
    public void logout() {
        if (currentUser != null) {
            SecurityAuditLogger.logout(currentUser.getUsername());
            log.info("User '{}' logged out.", currentUser.getUsername());
        }
        currentUser = null;
    }

    public User getCurrentUser() {
        return currentUser;
    }

    public boolean isAuthenticated() {
        return currentUser != null;
    }

    /**
     * Verifica que o usuário atual possui o perfil exigido.
     * Lança AuthorizationException em caso negativo (US-2369 AC2).
     */
    public void requireRole(Role requiredRole) {
        if (currentUser == null) {
            SecurityAuditLogger.authorizationFailure(null, requiredRole.name());
            throw new AuthorizationException("operation requires role [" + requiredRole + "].");
        }
        if (currentUser.getRole() != requiredRole) {
            SecurityAuditLogger.authorizationFailure(currentUser.getRole(), requiredRole.name());
            log.warn("Authorization denied. User '{}' has role [{}], required [{}].",
                    currentUser.getUsername(), currentUser.getRole(), requiredRole);
            throw new AuthorizationException("operation requires role [" + requiredRole + "].");
        }
    }
}
