package org.example.academic.system.model;

import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Pattern;
import jakarta.validation.constraints.Size;
import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Representa uma turma acadêmica.
 * Lombok reduz boilerplate (TUS-2365).
 * Validado via Jakarta Bean Validation (TUS-2371).
 */
@Getter
@Setter
@EqualsAndHashCode(of = "code")
@ToString
public class AcademicClass {

    @NotBlank(message = "Class code must not be blank")
    @Pattern(
        regexp = "^[A-Z]{2,5}\\d{3,6}$",
        message = "Class code must follow the pattern: 2-5 uppercase letters followed by 3-6 digits (e.g. CS101)"
    )
    private String code;

    @NotBlank(message = "Class title must not be blank")
    @Size(min = 3, max = 100, message = "Class title must be between 3 and 100 characters")
    private String title;

    @Valid
    private final List<Assessment> assessments = new ArrayList<>();

    public AcademicClass(String code, String title) {
        this.code = code;
        this.title = title;
    }

    public void addAssessment(Assessment assessment) {
        assessments.add(assessment);
    }

    public List<Assessment> getAssessments() {
        return Collections.unmodifiableList(assessments);
    }
}
