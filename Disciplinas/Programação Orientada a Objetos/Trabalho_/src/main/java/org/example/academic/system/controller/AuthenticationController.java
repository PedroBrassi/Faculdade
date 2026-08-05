package org.example.academic.system.controller;

import org.example.academic.system.model.User;
import org.example.academic.system.security.AuthenticationService;

/**
 * Controller de autenticação para a camada de apresentação (TUS-2414).
 *
 * <p>Desacopla a GUI JavaFX da implementação do {@link AuthenticationService},
 * seguindo o mesmo padrão arquitetural usado pelo restante da aplicação.
 * Não contém regra de negócio: apenas delega e propaga as exceções de
 * autenticação existentes. Reutilizável por qualquer tela JavaFX ou outra
 * camada de apresentação.</p>
 */
public class AuthenticationController {

    private final AuthenticationService authenticationService;

    public AuthenticationController(AuthenticationService authenticationService) {
        this.authenticationService = authenticationService;
    }

    /**
     * Delega a autenticação ao {@link AuthenticationService} e devolve o
     * usuário autenticado em caso de sucesso. Exceções de autenticação são
     * propagadas sem alteração.
     */
    public User authenticate(String username, String password) {
        return authenticationService.authenticate(username, password);
    }

    /** Encerra a sessão do usuário atual. */
    public void logout() {
        authenticationService.logout();
    }

    /** Retorna o usuário atualmente autenticado, ou {@code null}. */
    public User getCurrentUser() {
        return authenticationService.getCurrentUser();
    }
}
