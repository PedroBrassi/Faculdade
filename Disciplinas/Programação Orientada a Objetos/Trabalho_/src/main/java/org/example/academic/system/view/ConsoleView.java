package org.example.academic.system.view;

import org.example.academic.system.controller.AcademicSystemController;
import org.example.academic.system.exception.*;
import org.example.academic.system.model.AcademicClass;
import org.example.academic.system.model.Assessment;
import org.example.academic.system.model.PersistenceType;
import org.example.academic.system.model.Role;
import org.example.academic.system.security.AuthenticationService;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.List;
import java.util.Optional;
import java.util.Scanner;

/**
 * Interface de linha de comando e toda a E/S via teclado (US-2364).
 * Responsável apenas por exibir menus e ler entradas.
 * Toda lógica acadêmica é delegada ao AcademicSystemController (TUS-2370).
 */
public class ConsoleView {

    private static final Logger log = LoggerFactory.getLogger(ConsoleView.class);
    private static final String DATA_FILE = "academic-data";

    private final Scanner scanner;
    private final AuthenticationService authService;
    private final AcademicSystemController controller;

    public ConsoleView(AuthenticationService authService, AcademicSystemController controller) {
        this.authService = authService;
        this.controller = controller;
        this.scanner = new Scanner(System.in);
    }

    // -----------------------------------------------------------------------
    // Entrada da aplicação
    // -----------------------------------------------------------------------

    public void start() {
        System.out.println("===========================================");
        System.out.println("       ACADEMIC SYSTEM — Welcome           ");
        System.out.println("===========================================");

        while (true) {
            if (!loginFlow()) {
                System.out.println("Exiting application. Goodbye!");
                break;
            }
            boolean running = true;
            while (running && authService.isAuthenticated()) {
                running = menuFlow();
            }
        }
    }

    // -----------------------------------------------------------------------
    // Fluxo de login (US-2366)
    // -----------------------------------------------------------------------

    private boolean loginFlow() {
        System.out.println("\n--- Login ---");
        System.out.print("Username (or 'exit' to quit): ");
        String username = scanner.nextLine().trim();
        if ("exit".equalsIgnoreCase(username)) return false;

        System.out.print("Password: ");
        String password = scanner.nextLine().trim();

        try {
            authService.authenticate(username, password);
            System.out.println("Welcome, " + authService.getCurrentUser().getUsername()
                    + " [" + authService.getCurrentUser().getRole() + "]");
            return true;
        } catch (AuthenticationException e) {
            System.out.println("[ERROR] " + e.getMessage());
            return loginFlow();
        }
    }

    // -----------------------------------------------------------------------
    // Menu por perfil (US-2364)
    // -----------------------------------------------------------------------

    private boolean menuFlow() {
        Role role = authService.getCurrentUser().getRole();
        return role == Role.ADMIN ? adminMenu() : professorMenu();
    }

    private boolean adminMenu() {
        System.out.println("\n========== ADMIN MENU ==========");
        System.out.println("1. Register Class");
        System.out.println("2. Register Assessment");
        System.out.println("3. List Classes");
        System.out.println("4. Search Class by Code");
        System.out.println("5. Class Assessment Summary");
        System.out.println("6. Save Data");
        System.out.println("7. Configure Persistence Type");
        System.out.println("8. Persistence Configuration Report");
        System.out.println("9. Logout");
        System.out.println("0. Exit");
        System.out.print("Choose an option: ");

        return handleMenuInput(true);
    }

    private boolean professorMenu() {
        System.out.println("\n========== PROFESSOR MENU ==========");
        System.out.println("1. Register Assessment");
        System.out.println("2. List Classes");
        System.out.println("3. Search Class by Code");
        System.out.println("4. Class Assessment Summary");
        System.out.println("5. Logout");
        System.out.println("0. Exit");
        System.out.print("Choose an option: ");

        return handleMenuInput(false);
    }

    private boolean handleMenuInput(boolean isAdmin) {
        String input = scanner.nextLine().trim();
        try {
            int option = parseInteger(input);
            if (isAdmin) {
                switch (option) {
                    case 1 -> registerClassFlow();
                    case 2 -> registerAssessmentFlow();
                    case 3 -> listClassesFlow();
                    case 4 -> searchClassFlow();
                    case 5 -> classAssessmentSummaryFlow();
                    case 6 -> saveDataFlow();
                    case 7 -> configurePersistenceFlow();
                    case 8 -> persistenceReportFlow();
                    case 9 -> { authService.logout(); return true; }
                    case 0 -> { return false; }
                    default -> throw new InvalidMenuOptionException(input);
                }
            } else {
                switch (option) {
                    case 1 -> registerAssessmentFlow();
                    case 2 -> listClassesFlow();
                    case 3 -> searchClassFlow();
                    case 4 -> classAssessmentSummaryFlow();
                    case 5 -> { authService.logout(); return true; }
                    case 0 -> { return false; }
                    default -> throw new InvalidMenuOptionException(input);
                }
            }
        } catch (InvalidMenuOptionException | InvalidNumericInputException e) {
            System.out.println("[INPUT ERROR] " + e.getMessage());
            log.warn("Keyboard input error: {}", e.getMessage());
        } catch (AcademicSystemException e) {
            System.out.println("[ACADEMIC ERROR] " + e.getMessage());
            log.error("Academic error: {}", e.getMessage());
        } catch (AuthorizationException e) {
            System.out.println("[ACCESS DENIED] " + e.getMessage());
            log.warn("Authorization error: {}", e.getMessage());
        }
        return true;
    }

    // -----------------------------------------------------------------------
    // Operações
    // -----------------------------------------------------------------------

    private void registerClassFlow() {
        System.out.println("\n--- Register Class ---");
        System.out.print("Class code (e.g. CS101): ");
        String code = scanner.nextLine().trim();
        System.out.print("Class title: ");
        String title = scanner.nextLine().trim();
        controller.registerClass(code, title);
        System.out.println("Class '" + code + "' registered successfully.");
        log.info("Audit: class '{}' registered by '{}'", code,
                authService.getCurrentUser().getUsername());
    }

    private void registerAssessmentFlow() {
        System.out.println("\n--- Register Assessment ---");
        System.out.print("Class code: ");
        String code = scanner.nextLine().trim();
        System.out.println("Assessment type:");
        System.out.println("  1 - Exam");
        System.out.println("  2 - PracticalAssignment");
        System.out.println("  3 - Seminar");
        System.out.println("  4 - Assignment");
        System.out.print("Select type: ");
        int type = parseInteger(scanner.nextLine().trim());
        System.out.print("Value (0.0 - 10.0): ");
        double value = parseDouble(scanner.nextLine().trim());
        System.out.print("Weight (0.0 - 1.0): ");
        double weight = parseDouble(scanner.nextLine().trim());
        controller.registerAssessment(code, type, value, weight);
        System.out.println("Assessment registered successfully in class '" + code + "'.");
    }

    private void listClassesFlow() {
        System.out.println("\n--- Registered Classes ---");
        List<AcademicClass> classes = controller.listClasses();
        if (classes.isEmpty()) {
            System.out.println("No classes registered.");
            return;
        }
        for (AcademicClass cls : classes) {
            System.out.printf("  [%s] %s%n", cls.getCode(), cls.getTitle());
            for (Assessment a : cls.getAssessments()) {
                System.out.printf("      %-22s value=%.1f  weight=%.2f%n",
                        a.getType(), a.getValue(), a.getWeight());
            }
        }
    }

    private void searchClassFlow() {
        System.out.println("\n--- Search Class ---");
        System.out.print("Class code: ");
        String code = scanner.nextLine().trim();
        Optional<AcademicClass> result = controller.findClassByCode(code);
        if (result.isPresent()) {
            AcademicClass cls = result.get();
            System.out.printf("Found: [%s] %s%n", cls.getCode(), cls.getTitle());
            for (Assessment a : cls.getAssessments()) {
                System.out.printf("  %-22s value=%.1f  weight=%.2f%n",
                        a.getType(), a.getValue(), a.getWeight());
            }
        } else {
            System.out.println("Class not found: " + code);
        }
    }

    private void classAssessmentSummaryFlow() {
        System.out.println("\n--- Class Assessment Summary ---");
        System.out.print("Class code: ");
        String code = scanner.nextLine().trim();
        System.out.println(controller.generateClassSummary(code));
    }

    private void saveDataFlow() {
        String fileName = fileNameFor(controller.getCurrentPersistenceType());
        controller.saveData(fileName);
        System.out.println("Data saved successfully (type=" + controller.getCurrentPersistenceType()
                + ", file=" + fileName + ").");
        log.info("Audit: data saved by '{}'", authService.getCurrentUser().getUsername());
    }

    private void configurePersistenceFlow() {
        System.out.println("\n--- Configure Persistence Type ---");
        System.out.println("  1 - TXT");
        System.out.println("  2 - XML");
        System.out.println("  3 - JSON");
        System.out.print("Select type: ");
        int option = parseInteger(scanner.nextLine().trim());
        PersistenceType type = switch (option) {
            case 1 -> PersistenceType.TXT;
            case 2 -> PersistenceType.XML;
            case 3 -> PersistenceType.JSON;
            default -> throw new InvalidMenuOptionException(String.valueOf(option));
        };
        controller.configurePersistence(type);
        System.out.println("Persistence type configured to: " + type);
        log.info("Audit: persistence type set to '{}' by '{}'", type,
                authService.getCurrentUser().getUsername());
    }

    private void persistenceReportFlow() {
        System.out.println("\n--- Persistence Configuration Report ---");
        System.out.print(controller.generatePersistenceReport());
    }

    // -----------------------------------------------------------------------
    // Helpers
    // -----------------------------------------------------------------------

    private String fileNameFor(PersistenceType type) {
        return switch (type) {
            case TXT  -> DATA_FILE + ".txt";
            case XML  -> DATA_FILE + ".xml";
            case JSON -> DATA_FILE + ".json";
        };
    }

    private int parseInteger(String input) {
        try {
            return Integer.parseInt(input);
        } catch (NumberFormatException e) {
            throw new InvalidNumericInputException(input);
        }
    }

    private double parseDouble(String input) {
        try {
            return Double.parseDouble(input);
        } catch (NumberFormatException e) {
            throw new InvalidNumericInputException(input);
        }
    }
}
