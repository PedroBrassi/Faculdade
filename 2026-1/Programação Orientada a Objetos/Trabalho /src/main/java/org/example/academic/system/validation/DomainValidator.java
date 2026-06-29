package org.example.academic.system.validation;

import jakarta.validation.ConstraintViolation;
import jakarta.validation.Validation;
import jakarta.validation.Validator;
import jakarta.validation.ValidatorFactory;
import org.example.academic.system.exception.AcademicSystemException;

import java.util.Set;
import java.util.stream.Collectors;

/**
 * Validador de domínio centralizado usando Jakarta Bean Validation (TUS-2371).
 * Converte violações em AcademicSystemException (AC7).
 */
public class DomainValidator {

    private static final Validator VALIDATOR;

    static {
        try (ValidatorFactory factory = Validation.buildDefaultValidatorFactory()) {
            VALIDATOR = factory.getValidator();
        }
    }

    private DomainValidator() {}

    public static <T> void validate(T object) {
        Set<ConstraintViolation<T>> violations = VALIDATOR.validate(object);
        if (!violations.isEmpty()) {
            String message = violations.stream()
                    .map(ConstraintViolation::getMessage)
                    .collect(Collectors.joining("; "));
            throw new AcademicSystemException("Validation failed: " + message);
        }
    }
}
