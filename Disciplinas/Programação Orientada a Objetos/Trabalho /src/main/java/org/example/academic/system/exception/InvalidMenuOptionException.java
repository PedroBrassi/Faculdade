package org.example.academic.system.exception;

/** Lançada quando o usuário seleciona uma opção de menu inválida (US-2368 AC2). */
public class InvalidMenuOptionException extends KeyboardInputException {
    public InvalidMenuOptionException(String option) {
        super("Invalid menu option: '" + option + "'. Please select a valid option.");
    }
}
