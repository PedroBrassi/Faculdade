/**
 * EXPERIMENTO 3a - Jantar dos Filósofos SEM controle (ingênuo).
 *
 * Cada filósofo tenta pegar o garfo da esquerda e depois o da direita, na
 * mesma ordem, sem nenhum mecanismo adicional. Uma barreira (CyclicBarrier)
 * força todos os 5 filósofos a pegarem o garfo da esquerda praticamente ao
 * mesmo tempo, tornando o DEADLOCK reprodutível de forma confiável (as 4
 * condições de Coffman ficam presentes simultaneamente: exclusão mútua,
 * posse e espera, não preempção e espera circular).
 *
 * Um "vigia" (watchdog) imprime um aviso se nenhum filósofo comer por 5s.
 */
public class FilosofosSemControle {

    static final int N = 5;
    static final Object[] garfos = new Object[N];
    static final java.util.concurrent.CyclicBarrier barreira = new java.util.concurrent.CyclicBarrier(N);
    static volatile long ultimaRefeicao = System.currentTimeMillis();

    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== EXPERIMENTO 3a: Jantar dos Filósofos SEM controle ===");
        for (int i = 0; i < N; i++) garfos[i] = new Object();

        for (int i = 0; i < N; i++) {
            final int id = i;
            new Thread(() -> filosofo(id), "Filosofo-" + id).start();
        }

        // Vigia: se ninguém comer por 5s, é sinal forte de deadlock
        while (true) {
            Thread.sleep(1000);
            long semComer = System.currentTimeMillis() - ultimaRefeicao;
            if (semComer > 5000) {
                System.out.println("\n*** POSSÍVEL DEADLOCK DETECTADO: nenhum filósofo come há " + semComer + " ms ***");
                System.out.println("*** (interrompa o programa com Ctrl+C e registre esta observação) ***");
            }
        }
    }

    static void filosofo(int id) {
        int esquerda = id;
        int direita = (id + 1) % N;
        while (true) {
            pensar(id);
            try { barreira.await(); } catch (Exception e) { /* ignora */ } // força largada simultânea
            synchronized (garfos[esquerda]) {
                System.out.println("Filosofo-" + id + " pegou o garfo " + esquerda + " (esquerda)");
                synchronized (garfos[direita]) {
                    System.out.println("Filosofo-" + id + " pegou o garfo " + direita + " (direita) -> COMENDO");
                    ultimaRefeicao = System.currentTimeMillis();
                    dormir(50);
                }
            }
        }
    }

    static void pensar(int id) { dormir(10); }
    static void dormir(long ms) {
        try { Thread.sleep(ms); } catch (InterruptedException e) { Thread.currentThread().interrupt(); }
    }
}
