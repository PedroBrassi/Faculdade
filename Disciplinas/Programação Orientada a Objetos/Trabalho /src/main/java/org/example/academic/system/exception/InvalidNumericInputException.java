package org.example.academic.system.exception;

/** Lançada quando o sistema espera um número mas recebe entrada inválida (US-2368 AC1). */
public class InvalidNumericInputException extends KeyboardInputException {
    public InvalidNumericInputException(String input) {
        super("Invalid numeric input: '" + input + "'. Please enter a valid number.");
    }
}
