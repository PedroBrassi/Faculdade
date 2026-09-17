import java.util.concurrent.Semaphore;

/**
 * EXPERIMENTO 1b - Produtor-Consumidor COM sincronização correta.
 *
 * Mesma estrutura do Experimento 1a (mesmo CAPACIDADE, mesmo número de
 * tentativas), mas agora três semáforos coordenam o acesso ao "buffer",
 * exatamente como descrito na Aula 10 (slide 7):
 *   - mutex     : exclusão mútua no acesso ao contador (down/up)
 *   - espacos   : conta posições livres (produtor espera se buffer cheio)
 *   - itens     : conta posições ocupadas (consumidor espera se buffer vazio)
 *
 * Compare o resultado final com o do Experimento 1a.
 */
public class ProdutorConsumidorComSemaforo {

    static final int CAPACIDADE = 5;
    static final int ITENS_POR_PRODUTOR = 5000;

    static int itensNoBuffer = 0;

    static final Semaphore mutex = new Semaphore(1);
    static final Semaphore espacos = new Semaphore(CAPACIDADE);
    static final Semaphore itens = new Semaphore(0);

    static int totalProduzido = 0;
    static int totalConsumido = 0;

    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== EXPERIMENTO 1b: Produtor-Consumidor COM semáforos ===");
        System.out.println("(O buffer limitado é representado, de forma simplificada, por um contador inteiro: 'itensNoBuffer'.)");
        System.out.println("Capacidade do buffer: " + CAPACIDADE);

        Thread p1 = new Thread(ProdutorConsumidorComSemaforo::produzir, "Produtor-1");
        Thread p2 = new Thread(ProdutorConsumidorComSemaforo::produzir, "Produtor-2");
        Thread c1 = new Thread(ProdutorConsumidorComSemaforo::consumir, "Consumidor-1");
        Thread c2 = new Thread(ProdutorConsumidorComSemaforo::consumir, "Consumidor-2");

        long inicio = System.currentTimeMillis();
        p1.start(); p2.start(); c1.start(); c2.start();
        p1.join(); p2.join(); c1.join(); c2.join();
        long fim = System.currentTimeMillis();

        System.out.println("\n--- RESULTADO FINAL ---");
        System.out.println("Total produzido : " + totalProduzido + " (esperado: " + (2 * ITENS_POR_PRODUTOR) + ")");
        System.out.println("Total consumido : " + totalConsumido + " (esperado: " + (2 * ITENS_POR_PRODUTOR) + ")");
        System.out.println("Valor final de itensNoBuffer: " + itensNoBuffer + " (esperado: 0)");
        System.out.println("Tempo total: " + (fim - inicio) + " ms");
        System.out.println("\n>>> Rode este programa 3-4 vezes seguidas: o resultado final deve ser SEMPRE o mesmo. <<<");
    }

    static void produzir() {
        for (int i = 0; i < ITENS_POR_PRODUTOR; i++) {
            try {
                espacos.acquire();      // down(espacos)
                mutex.acquire();        // down(mutex)
                itensNoBuffer++;
                totalProduzido++;
                mutex.release();        // up(mutex)
                itens.release();        // up(itens)
            } catch (InterruptedException e) { Thread.currentThread().interrupt(); }
        }
    }

    static void consumir() {
        for (int i = 0; i < ITENS_POR_PRODUTOR; i++) {
            try {
                itens.acquire();        // down(itens)
                mutex.acquire();        // down(mutex)
                itensNoBuffer--;
                totalConsumido++;
                mutex.release();        // up(mutex)
                espacos.release();      // up(espacos)
            } catch (InterruptedException e) { Thread.currentThread().interrupt(); }
        }
    }
}
