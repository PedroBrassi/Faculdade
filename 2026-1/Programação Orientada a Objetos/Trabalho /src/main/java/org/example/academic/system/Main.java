package org.example.academic.system;

import org.example.academic.system.exception.AcademicSystemException;
import org.example.academic.system.exception.KeyboardInputException;
import org.example.academic.system.exception.SecurityException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Ponto de entrada da aplicação.
 * Responsável apenas pela inicialização, tratamento de exceções de topo e saída (TUS-2370).
 */
public class Main {

    private static final Logger log = LoggerFactory.getLogger(Main.class);

    public static void main(String[] args) {
        try {
            AcademicSystem.run();
        } catch (AcademicSystemException e) {
            System.out.println("[ACADEMIC ERROR] " + e.getMessage());
            log.error("Unhandled academic error: {}", e.getMessage(), e);
        } catch (KeyboardInputException e) {
            System.out.println("[INPUT ERROR] " + e.getMessage());
            log.error("Unhandled input error: {}", e.getMessage(), e);
        } catch (SecurityException e) {
            // Não expõe detalhes sensíveis (US-2369 AC6)
            System.out.println("[SECURITY ERROR] " + e.getMessage());
            log.error("Unhandled security error: {}", e.getMessage(), e);
        } catch (Exception e) {
            System.out.println("[ERROR] An unexpected error occurred. Please contact support.");
            log.error("Unexpected error: {}", e.getMessage(), e);
        }
    }
}
