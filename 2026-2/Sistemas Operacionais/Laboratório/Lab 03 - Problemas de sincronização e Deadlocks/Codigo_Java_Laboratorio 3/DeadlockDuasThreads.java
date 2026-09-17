/**
 * EXPERIMENTO 4a - Deadlock determinístico com 2 processos e 2 recursos
 * (reproduz exatamente o cenário do slide 10, Aula 11).
 *
 * Thread A: pega recurso1, espera um pouco, tenta pegar recurso2.
 * Thread B: pega recurso2, espera um pouco, tenta pegar recurso1.
 * Os "sleep" forçam o intercalamento exato do slide, tornando o deadlock
 * 100% reprodutível (ao contrário de uma condição de corrida comum).
 */
public class DeadlockDuasThreads {

    static final Object recurso1 = new Object();
    static final Object recurso2 = new Object();

    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== EXPERIMENTO 4a: Deadlock com 2 recursos (A e B) ===");

        Thread a = new Thread(() -> {
            synchronized (recurso1) {
                log("A", "obteve recurso1");
                dormir(500); // dá tempo para B pegar recurso2
                log("A", "tentando obter recurso2...");
                synchronized (recurso2) {
                    log("A", "obteve recurso2 -> concluiu");
                }
            }
        }, "Thread-A");

        Thread b = new Thread(() -> {
            synchronized (recurso2) {
                log("B", "obteve recurso2");
                dormir(500); // dá tempo para A pegar recurso1
                log("B", "tentando obter recurso1...");
                synchronized (recurso1) {
                    log("B", "obteve recurso1 -> concluiu");
                }
            }
        }, "Thread-B");

        a.start();
        b.start();

        // Espera no máximo 5s; se não terminar, é deadlock confirmado
        a.join(5000);
        b.join(5000);

        if (a.isAlive() || b.isAlive()) {
            System.out.println("\n*** DEADLOCK CONFIRMADO: as threads não terminaram em 5s. ***");
            System.out.println("*** Estado: A segura recurso1 e espera recurso2; B segura recurso2 e espera recurso1. ***");
            // Encerra o programa de propósito: as duas threads ficarão bloqueadas
            // para sempre (esse É o resultado esperado do experimento, não um
            // erro de execução), então usamos código de saída 0.
            System.exit(0);
        } else {
            System.out.println("\nAmbas as threads terminaram sem deadlock.");
        }
    }

    static void log(String quem, String msg) {
        System.out.println("[" + System.currentTimeMillis() % 100000 + " ms] Thread-" + quem + ": " + msg);
    }

    static void dormir(long ms) {
        try { Thread.sleep(ms); } catch (InterruptedException e) { Thread.currentThread().interrupt(); }
    }
}
