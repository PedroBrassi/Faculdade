package org.example.academic.system;

import org.example.academic.system.model.AcademicClass;
import org.example.academic.system.model.Administrator;
import org.example.academic.system.model.User;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotEquals;

/**
 * TUS-2382 / TUS-2384 - Igualdade de objetos de domínio identificáveis.
 *
 * <p>Turmas são identificadas pelo código; usuários, pelo username. Dois objetos
 * com o mesmo identificador devem ser iguais e ter o mesmo hashCode.</p>
 */
@DisplayName("TUS-2384 - Igualdade de objetos de domínio identificáveis")
class EqualityTest {

    @Test
    @DisplayName("turmas com o mesmo código são iguais (independente do título)")
    void academicClassesWithSameCodeAreEqual() {
        AcademicClass a = new AcademicClass("CS101", "Introdução a POO");
        AcademicClass b = new AcademicClass("CS101", "Outro título");

        assertEquals(a, b);
        assertEquals(a.hashCode(), b.hashCode());
    }

    @Test
    @DisplayName("turmas com códigos diferentes não são iguais")
    void academicClassesWithDifferentCodeAreNotEqual() {
        AcademicClass a = new AcademicClass("CS101", "Introdução a POO");
        AcademicClass b = new AcademicClass("CS102", "Introdução a POO");

        assertNotEquals(a, b);
    }

    @Test
    @DisplayName("usuários com o mesmo username são iguais (independente da senha)")
    void usersWithSameUsernameAreEqual() {
        User a = new Administrator("admin", "admin123");
        User b = new Administrator("admin", "senha-diferente");

        assertEquals(a, b);
        assertEquals(a.hashCode(), b.hashCode());
    }

    @Test
    @DisplayName("usuários com usernames diferentes não são iguais")
    void usersWithDifferentUsernameAreNotEqual() {
        User a = new Administrator("admin", "admin123");
        User b = new Administrator("outro", "admin123");

        assertNotEquals(a, b);
    }
}
