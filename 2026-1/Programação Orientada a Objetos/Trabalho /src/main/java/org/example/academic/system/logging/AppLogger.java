package org.example.academic.system.logging;

import java.util.logging.Logger;

/**
 * TUS-2390 - ponto único para pegar um Logger no sistema.
 * Garante que a configuração foi carregada antes de devolver o logger.
 */
public final class AppLogger {

    static {
        LoggingConfig.init();
    }

    private AppLogger() {
    }

    public static Logger get(Class<?> type) {
        return Logger.getLogger(type.getName());
    }

    public static Logger get(String name) {
        return Logger.getLogger(name);
    }
}
