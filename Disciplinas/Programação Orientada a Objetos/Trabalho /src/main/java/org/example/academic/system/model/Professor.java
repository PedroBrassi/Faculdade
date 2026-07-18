package org.example.academic.system.model;

/** Usuário com perfil PROFESSOR. */
public class Professor extends User {

    public Professor(String username, String password) {
        super(username, password, Role.PROFESSOR);
    }

    /** Professor padrão (usado em checagens de perfil/testes). */
    public Professor() {
        super("professor", "prof123", Role.PROFESSOR);
    }
}
