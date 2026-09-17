import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * EXPERIMENTO 2a - Leitores-Escritores com MUTEX SIMPLES e FILA JUSTA
 * (slide 12, Aula 10).
 *
 * Um único semáforo binário e JUSTO (FIFO) protege o acesso ao recurso
 * compartilhado. Leitores e escritores disputam exatamente o mesmo
 * semáforo, na ordem em que chegaram: nunca há leitura simultânea, e um
 * escritor que já está na fila NUNCA é ultrapassado por um leitor que
 * chegou depois dele.
 *
 * Esta versão inclui leitores E escritores de verdade, para que a
 * comparação com o Experimento 2b (ReadWriteLock) seja justa.
 */
public class LeitoresEscritoresMutexSimples {

    static final int N_LEITORES = 8;
    static final Semaphore mutex = new Semaphore(1, true); // true = fila FIFO justa

    static volatile boolean ativo = true;
    static volatile boolean escritorAguardando = false;
    static final AtomicInteger ativosAgora = new AtomicInteger(0);
    static final AtomicInteger maxSimultaneos = new AtomicInteger(0);
    static final AtomicInteger leiturasTotais = new AtomicInteger(0);
    static final AtomicInteger leiturasDuranteEsperaEscritor = new AtomicInteger(0);

    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== EXPERIMENTO 2a: Leitores-Escritores com mutex simples (fila justa) ===");

        Thread[] leitores = new Thread[N_LEITORES];
        for (int i = 0; i < N_LEITORES; i++) {
            final int id = i;
            leitores[i] = new Thread(() -> leitor(id), "Leitor-" + id);
        }

        long inicio = System.currentTimeMillis();
        for (Thread t : leitores) t.start();

        Thread escritor = new Thread(LeitoresEscritoresMutexSimples::escritor, "Escritor-0");
        escritor.start();

        escritor.join();
        for (Thread t : leitores) t.join();
        long fim = System.currentTimeMillis();

        System.out.println("\n--- RESULTADO FINAL ---");
        System.out.println("Máximo de leitores simultâneos observado: " + maxSimultaneos.get() + " (de " + N_LEITORES + ")");
        System.out.println("Leituras totais concluídas: " + leiturasTotais.get());
        System.out.println("Tempo total do experimento: " + (fim - inicio) + " ms");
    }

    static void leitor(int id) {
        while (ativo) {
            try {
                mutex.acquire();
                if (escritorAguardando) leiturasDuranteEsperaEscritor.incrementAndGet();
                int agora = ativosAgora.incrementAndGet();
                maxSimultaneos.updateAndGet(max -> Math.max(max, agora));
                leiturasTotais.incrementAndGet();
                Thread.sleep(50); // simula tempo de leitura
                ativosAgora.decrementAndGet();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            } finally {
                mutex.release();
            }
            try { Thread.sleep(20); } catch (InterruptedException e) { Thread.currentThread().interrupt(); }
        }
    }

    static void escritor() {
        try {
            Thread.sleep(300); // dá tempo para os leitores começarem a circular
            long pedidoEm = System.currentTimeMillis();
            escritorAguardando = true;
            System.out.println("[" + pedidoEm % 100000 + " ms] Escritor-0 SOLICITOU acesso exclusivo");

            mutex.acquire();
            long obtidoEm = System.currentTimeMillis();
            escritorAguardando = false;

            System.out.println("[" + obtidoEm % 100000 + " ms] Escritor-0 OBTEVE acesso exclusivo (esperou " + (obtidoEm - pedidoEm) + " ms)");
            Thread.sleep(100); // simula tempo de escrita
            System.out.println("Escritor-0 concluiu a escrita.");

            System.out.println("\n--- IMPACTO DA ESPERA DO ESCRITOR ---");
            System.out.println("Tempo de espera do escritor: " + (obtidoEm - pedidoEm) + " ms");
            System.out.println("Leituras concluídas ENQUANTO o escritor esperava: " + leiturasDuranteEsperaEscritor.get());
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } finally {
            mutex.release();
            ativo = false; // sinaliza aos leitores para encerrarem
        }
    }
}
