import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * EXPERIMENTO 2b - Leitores-Escritores com ReadWriteLock
 * (resposta à provocação do slide 13, Aula 10).
 *
 * Usa um ReentrantReadWriteLock no modo padrão (NÃO justo): vários
 * leitores podem entrar simultaneamente na seção crítica (lock de
 * leitura compartilhado), enquanto um escritor precisa de acesso
 * exclusivo (lock de escrita).
 *
 * IMPORTANTE: o modo não-justo do Java NÃO é uma política formal de
 * "preferência ao leitor" (reader-preference) — ele só significa que a
 * ordem de atendimento entre leitores e escritor NÃO é garantida ser
 * FIFO. Na prática, a implementação do JDK tem uma proteção interna
 * que costuma impedir novos leitores de "furarem a fila" depois que um
 * escritor já está esperando (por isso não é incomum observar ZERO
 * leituras concluídas durante a espera do escritor, mesmo neste modo
 * não-justo). Não assuma starvation do escritor como resultado
 * garantido: meça e compare com a versão com fila justa (Experimento
 * 2a) para tirar conclusões.
 *
 * Mesma estrutura de leitores/escritor do Experimento 2a, para permitir
 * comparação direta.
 */
public class LeitoresEscritoresComPrioridade {

    static final int N_LEITORES = 8;
    static final ReadWriteLock lock = new ReentrantReadWriteLock(); // não-justo (padrão)

    static volatile boolean ativo = true;
    static volatile boolean escritorAguardando = false;
    static final AtomicInteger ativosAgora = new AtomicInteger(0);
    static final AtomicInteger maxSimultaneos = new AtomicInteger(0);
    static final AtomicInteger leiturasTotais = new AtomicInteger(0);
    static final AtomicInteger leiturasDuranteEsperaEscritor = new AtomicInteger(0);

    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== EXPERIMENTO 2b: Leitores-Escritores com ReadWriteLock ===");

        Thread[] leitores = new Thread[N_LEITORES];
        for (int i = 0; i < N_LEITORES; i++) {
            final int id = i;
            leitores[i] = new Thread(() -> leitor(id), "Leitor-" + id);
        }

        long inicio = System.currentTimeMillis();
        for (Thread t : leitores) t.start();

        Thread escritor = new Thread(LeitoresEscritoresComPrioridade::escritor, "Escritor-0");
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
            lock.readLock().lock();
            try {
                if (escritorAguardando) leiturasDuranteEsperaEscritor.incrementAndGet();
                int agora = ativosAgora.incrementAndGet();
                maxSimultaneos.updateAndGet(max -> Math.max(max, agora));
                leiturasTotais.incrementAndGet();
                Thread.sleep(50); // simula tempo de leitura
                ativosAgora.decrementAndGet();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            } finally {
                lock.readLock().unlock();
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

            lock.writeLock().lock();
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
            lock.writeLock().unlock();
            ativo = false; // sinaliza aos leitores para encerrarem
        }
    }
}
