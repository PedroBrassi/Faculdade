package org.example.academic.system.logging;

import java.io.InputStream;
import java.util.logging.LogManager;
import java.util.logging.Logger;

/**
 * TUS-2390 - infraestrutura de logging.
 * Carrega o logging.properties (formato, nível e destino) uma única vez.
 * A ideia é deixar a configuração separada das classes de negócio.
 */
public final class LoggingConfig {

    private static boolean initialized = false;

    private LoggingConfig() {
    }

    public static synchronized void init() {
        if (initialized) {
            return;
        }

        try (InputStream in = LoggingConfig.class.getResourceAsStream("/logging.properties")) {
            if (in != null) {
                LogManager.getLogManager().readConfiguration(in);
            }
        } catch (Exception e) {
            // se o arquivo não estiver no classpath o JUL usa a config padrão,
            // então não vale a pena derrubar a aplicação por causa disso
            Logger.getLogger(LoggingConfig.class.getName())
                    .warning("Could not load logging.properties, using defaults");
        }

        initialized = true;
    }
}
