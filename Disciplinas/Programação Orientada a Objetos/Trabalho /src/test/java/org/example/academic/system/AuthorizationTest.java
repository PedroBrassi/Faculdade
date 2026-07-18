package org.example.academic.system;

import org.example.academic.system.exception.AuthorizationException;
import org.example.academic.system.model.Administrator;
import org.example.academic.system.model.Professor;
import org.example.academic.system.model.Role;
import org.example.academic.system.security.AuthorizationService;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

@DisplayName("US-2387 - Comportamento da autorização")
class AuthorizationTest {

    private AuthorizationService authorization;

    @BeforeEach
    void setUp() {
        authorization = new AuthorizationService();
    }

    @Test
    @DisplayName("ADMIN é autorizado em operações de administrador")
    void adminShouldBeAuthorized() {
        Administrator admin = new Administrator();
        assertTrue(authorization.isAuthorized(admin, Role.ADMIN));
        assertDoesNotThrow(() -> authorization.requireRole(admin, Role.ADMIN, "configure persistence"));
    }

    @Test
    @DisplayName("PROFESSOR não é autorizado em operações de administrador")
    void professorShouldNotBeAuthorized() {
        Professor professor = new Professor();
        assertThrows(AuthorizationException.class,
                () -> authorization.requireRole(professor, Role.ADMIN, "configure persistence"));
    }

    @Test
    @DisplayName("acesso não autorizado lança AuthorizationException")
    void unauthorizedAccessShouldThrow() {
        assertThrows(AuthorizationException.class,
                () -> authorization.requireRole(new Professor(), Role.ADMIN, "register class"));
    }
}
