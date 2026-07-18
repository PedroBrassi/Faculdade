package org.example.academic.system.repository;

import org.example.academic.system.model.Administrator;
import org.example.academic.system.model.Professor;
import org.example.academic.system.model.Role;
import org.example.academic.system.model.User;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

/**
 * Carrega usuários de um arquivo TXT (US-2366 AC5).
 * Formato: username|password|ROLE
 * Em caso de arquivo ausente, usa um conjunto padrão de usuários.
 */
public class TxtUserRepository {

    private static final Logger log = LoggerFactory.getLogger(TxtUserRepository.class);
    private final List<User> users = new ArrayList<>();

    public TxtUserRepository(String filePath) {
        loadUsers(filePath);
    }

    private void loadUsers(String filePath) {
        File file = new File(filePath);
        if (!file.exists()) {
            log.warn("User file '{}' not found. Loading default users.", filePath);
            users.add(new Administrator("admin", "admin123"));
            users.add(new Professor("professor", "prof123"));
            return;
        }
        try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
            String line;
            while ((line = reader.readLine()) != null) {
                line = line.trim();
                if (line.isEmpty() || line.startsWith("#")) continue;
                String[] parts = line.split("\\|");
                if (parts.length == 3) {
                    Role role = Role.valueOf(parts[2].toUpperCase());
                    User user = role == Role.ADMIN
                            ? new Administrator(parts[0], parts[1])
                            : new Professor(parts[0], parts[1]);
                    users.add(user);
                }
            }
            log.info("Loaded {} user(s) from '{}'.", users.size(), filePath);
        } catch (IOException e) {
            log.error("Failed to read user file: {}", filePath, e);
        }
    }

    public Optional<User> findByUsername(String username) {
        return users.stream()
                .filter(u -> u.getUsername().equals(username))
                .findFirst();
    }
}
