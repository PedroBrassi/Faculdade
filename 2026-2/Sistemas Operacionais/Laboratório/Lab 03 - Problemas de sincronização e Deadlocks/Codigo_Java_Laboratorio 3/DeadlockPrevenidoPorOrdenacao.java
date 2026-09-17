/**
 * EXPERIMENTO 4b - Prevenção de deadlock atacando a ESPERA CIRCULAR
 * (slide 37, Aula 11): impor uma ordem global de aquisição de recursos.
 *
 * Agora as DUAS threads pegam sempre recurso1 primeiro e recurso2 depois.
 * Sem ciclo possível no grafo de alocação, o deadlock não pode ocorrer,
 * mesmo com os mesmos "sleep" usados no Experimento 4a.
 */
public class DeadlockPrevenidoPorOrdenacao {

    static final Object recurso1 = new Object();
    static final Object recurso2 = new Object();

    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== EXPERIMENTO 4b: Prevenção por ordenação global de recursos ===");

        Thread a = new Thread(() -> {
            synchronized (recurso1) {
                log("A", "obteve recurso1");
                dormir(500);
                log("A", "tentando obter recurso2...");
                synchronized (recurso2) {
                    log("A", "obteve recurso2 -> concluiu");
                }
            }
        }, "Thread-A");

        // ATENÇÃO: B agora também pede recurso1 ANTES de recurso2 (mesma ordem de A)
        Thread b = new Thread(() -> {
            synchronized (recurso1) {
                log("B", "obteve recurso1");
                dormir(500);
                log("B", "tentando obter recurso2...");
                synchronized (recurso2) {
                    log("B", "obteve recurso2 -> concluiu");
                }
            }
        }, "Thread-B");

        a.start();
        b.start();

        a.join(5000);
        b.join(5000);

        if (a.isAlive() || b.isAlive()) {
            System.out.println("\n*** DEADLOCK OCORREU (não deveria!) ***");
        } else {
            System.out.println("\nAmbas as threads terminaram SEM deadlock -- a ordenação global funcionou.");
        }
    }

    static void log(String quem, String msg) {
        System.out.println("[" + System.currentTimeMillis() % 100000 + " ms] Thread-" + quem + ": " + msg);
    }

    static void dormir(long ms) {
        try { Thread.sleep(ms); } catch (InterruptedException e) { Thread.currentThread().interrupt(); }
    }
}
