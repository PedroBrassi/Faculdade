package org.example.academic.system;

import org.example.academic.system.logging.AppLogger;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import java.util.logging.Logger;

import static org.junit.jupiter.api.Assertions.assertDoesNotThrow;
import static org.junit.jupiter.api.Assertions.assertNotNull;

@DisplayName("TUS-2395 - Comportamento da infraestrutura de logging")
class LoggingInfrastructureTest {

    @Test
    @DisplayName("é possível criar uma instância de logger")
    void loggerCanBeCreated() {
        Logger log = AppLogger.get(LoggingInfrastructureTest.class);
        assertNotNull(log);
    }

    @Test
    @DisplayName("escrever mensagens não lança exceção")
    void logMessagesShouldNotThrow() {
        Logger log = AppLogger.get(LoggingInfrastructureTest.class);
        // sem checar conteudo nem destino - so garante que a infra nao quebra (TUS-2395)
        assertDoesNotThrow(() -> {
            log.info("infra de logging ok");
            log.warning("evento de aviso");
            log.fine("detalhe");
        });
    }
}
