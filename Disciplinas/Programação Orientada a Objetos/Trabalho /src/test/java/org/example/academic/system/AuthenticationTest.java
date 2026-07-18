package org.example.academic.system;

import org.example.academic.system.exception.AuthenticationException;
import org.example.academic.system.model.Role;
import org.example.academic.system.model.User;
import org.example.academic.system.security.AuthenticationService;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

@DisplayName("US-2386 - Comportamento da autenticação")
class AuthenticationTest {

    private AuthenticationService authentication;

    @BeforeEach
    void setUp() {
        authentication = new AuthenticationService();
    }

    @Test
    @DisplayName("usuário e senha válidos autenticam")
    void validCredentialsShouldAuthenticate() {
        User user = authentication.authenticate("admin", "admin123");
        assertEquals(Role.ADMIN, user.getRole());
    }

    @Test
    @DisplayName("usuário inválido lança AuthenticationException")
    void invalidUsernameShouldThrow() {
        assertThrows(AuthenticationException.class,
                () -> authentication.authenticate("naoexiste", "admin123"));
    }

    @Test
    @DisplayName("senha inválida lança AuthenticationException")
    void invalidPasswordShouldThrow() {
        assertThrows(AuthenticationException.class,
                () -> authentication.authenticate("admin", "senhaerrada"));
    }
}
