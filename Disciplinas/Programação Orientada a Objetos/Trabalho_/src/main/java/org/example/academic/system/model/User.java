package org.example.academic.system.model;

import lombok.EqualsAndHashCode;
import lombok.Getter;

/**
 * Representa um usuário do sistema com controle de acesso baseado em perfil.
 * Senhas nunca são expostas no toString (US-2366 AC6).
 */
@Getter
@EqualsAndHashCode(of = "username")
public abstract class User {

    private final String username;
    private final String password;
    private final Role role;

    protected User(String username, String password, Role role) {
        this.username = username;
        this.password = password;
        this.role = role;
    }

    /** Nunca expõe a senha no toString (AC6 — passwords não devem ser logadas). */
    @Override
    public String toString() {
        return "User{username='" + username + "', role=" + role + "}";
    }
}
