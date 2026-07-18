package org.example.academic.system.gui;

import org.example.academic.system.controller.AcademicSystemController;
import org.example.academic.system.controller.AuthenticationController;

/**
 * Contexto compartilhado da aplicação JavaFX.
 *
 * <p>Os controllers de tela são instanciados pelo {@code FXMLLoader} (via
 * {@code fx:controller}) com construtor sem argumentos, portanto não podem
 * receber as dependências por injeção. Esta classe expõe os controllers de
 * negócio já conectados (mesmo wiring da CLI) para que as telas possam delegar
 * as operações ao backend sem conhecer a montagem das dependências.</p>
 */
public final class AppContext {

    private static AcademicSystemController academicSystemController;
    private static AuthenticationController authenticationController;

    private AppContext() {
    }

    /** Inicializa o contexto com os controllers já conectados (chamado no startup). */
    public static void init(AcademicSystemController academic, AuthenticationController authentication) {
        academicSystemController = academic;
        authenticationController = authentication;
    }

    public static AcademicSystemController academic() {
        return academicSystemController;
    }

    public static AuthenticationController auth() {
        return authenticationController;
    }
}
