import java.util.concurrent.Semaphore;

/**
 * EXPERIMENTO 3b - Jantar dos Filósofos com a solução do "SALEIRO"
 * (slide 28, Aula 10).
 *
 * Um semáforo extra (saleiro) garante que no máximo 1 filósofo por vez
 * tenta pegar os dois garfos. Isso quebra a condição de ESPERA CIRCULAR:
 * não é mais possível que todos peguem o garfo da esquerda ao mesmo tempo.
 */
public class FilosofosComSaleiro {

    static final int N = 5;
    static final Object[] garfos = new Object[N];
    static final Semaphore saleiro = new Semaphore(1); // token de controle
    static volatile long ultimaRefeicao = System.currentTimeMillis();
    static volatile int refeicoesTotais = 0;

    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== EXPERIMENTO 3b: Jantar dos Filósofos com o saleiro ===");
        for (int i = 0; i < N; i++) garfos[i] = new Object();

        for (int i = 0; i < N; i++) {
            final int id = i;
            new Thread(() -> filosofo(id), "Filosofo-" + id).start();
        }

        // Roda por 8 segundos e reporta quantas refeições ocorreram (prova de progresso, sem deadlock)
        Thread.sleep(8000);
        System.out.println("\n--- RESULTADO APÓS 8s ---");
        System.out.println("Total de refeições concluídas por todos os filósofos: " + refeicoesTotais);
        System.out.println("(Se este número for > 0 e crescente, não há deadlock.)");
        System.exit(0);
    }

    static void filosofo(int id) {
        int esquerda = id;
        int direita = (id + 1) % N;
        while (true) {
            pensar(id);
            try {
                saleiro.acquire(); // só 1 filósofo por vez tenta pegar os garfos
                synchronized (garfos[esquerda]) {
                    synchronized (garfos[direita]) {
                        saleiro.release(); // já garantiu os dois garfos, libera o saleiro
                        System.out.println("Filosofo-" + id + " está COMENDO (garfos " + esquerda + " e " + direita + ")");
                        ultimaRefeicao = System.currentTimeMillis();
                        refeicoesTotais++;
                        dormir(50);
                    }
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }

    static void pensar(int id) { dormir(10); }
    static void dormir(long ms) {
        try { Thread.sleep(ms); } catch (InterruptedException e) { Thread.currentThread().interrupt(); }
    }
}
