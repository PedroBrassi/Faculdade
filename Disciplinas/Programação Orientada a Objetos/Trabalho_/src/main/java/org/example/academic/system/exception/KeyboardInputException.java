package org.example.academic.system.exception;

/** Exceção base para todos os erros de entrada via teclado (US-2368). */
public class KeyboardInputException extends RuntimeException {
    public KeyboardInputException(String message) { super(message); }
    public KeyboardInputException(String message, Throwable cause) { super(message, cause); }
}
