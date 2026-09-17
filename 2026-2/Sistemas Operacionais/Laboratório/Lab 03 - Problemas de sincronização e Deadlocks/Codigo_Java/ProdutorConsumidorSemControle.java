/**
 * EXPERIMENTO 1a - Produtor-Consumidor SEM sincronização correta.
 *
 * O "buffer" é representado por um contador inteiro (quantidade de itens
 * presentes). Produtores e consumidores fazem uma operação clássica de
 * "ler valor atual, decidir, escrever novo valor" SEM exclusão mútua.
 * Isso é exatamente uma condição de corrida (race condition): threads
 * podem ler o mesmo valor antes de qualquer uma escrever o resultado,
 * fazendo incrementos/decrementos se perderem.
 *
 * Repita a execução várias vezes: os resultados finais VÃO VARIAR a cada
 * rodada (não determinismo) e frequentemente serão diferentes do valor
 * teoricamente esperado.
 */
public class ProdutorConsumidorSemControle {

    static final int CAPACIDADE = 5;
    static final int ITENS_POR_PRODUTOR = 5000;

    static int itensNoBuffer = 0;     // "buffer" = apenas um contador, sem proteção
    static int totalProduzido = 0;
    static int totalConsumido = 0;
    static int producoesRejeitadas = 0; // buffer estava cheio (na visão da thread)
    static int consumosRejeitados = 0;  // buffer estava vazio (na visão da thread)
    static int violacoesCapacidade = 0; // itensNoBuffer ficou fora da faixa [0, CAPACIDADE]

    public static void main(String[] args) throws InterruptedException {
        System.out.println("=== EXPERIMENTO 1a: Produtor-Consumidor SEM controle ===");
        System.out.println("(O buffer limitado é representado, de forma simplificada, por um contador inteiro: 'itensNoBuffer'.)");
        System.out.println("Capacidade do buffer: " + CAPACIDADE);

        Thread p1 = new Thread(ProdutorConsumidorSemControle::produzir, "Produtor-1");
        Thread p2 = new Thread(ProdutorConsumidorSemControle::produzir, "Produtor-2");
        Thread c1 = new Thread(ProdutorConsumidorSemControle::consumir, "Consumidor-1");
        Thread c2 = new Thread(ProdutorConsumidorSemControle::consumir, "Consumidor-2");

        long inicio = System.currentTimeMillis();
        p1.start(); p2.start(); c1.start(); c2.start();
        p1.join(); p2.join(); c1.join(); c2.join();
        long fim = System.currentTimeMillis();

        System.out.println("\n--- RESULTADO FINAL ---");
        System.out.println("Total produzido (aceito)  : " + totalProduzido + " (tentativas: " + (2 * ITENS_POR_PRODUTOR) + ")");
        System.out.println("Total consumido (aceito)  : " + totalConsumido + " (tentativas: " + (2 * ITENS_POR_PRODUTOR) + ")");
        System.out.println("Valor final de itensNoBuffer: " + itensNoBuffer + "  (esperado teoricamente: " + (totalProduzido - totalConsumido) + ")");
        System.out.println("Violações de faixa [0, " + CAPACIDADE + "] detectadas: " + violacoesCapacidade);
        System.out.println("Tempo total: " + (fim - inicio) + " ms");
        System.out.println("\n>>> Rode este programa 3-4 vezes seguidas e compare os números. <<<");
    }

    static void produzir() {
        for (int i = 0; i < ITENS_POR_PRODUTOR; i++) {
            int atual = itensNoBuffer;        // LEITURA sem proteção
            Thread.yield();                   // amplia a janela da condição de corrida
            if (atual < CAPACIDADE) {
                // outra thread pode alterar itensNoBuffer bem aqui, entre a leitura e a escrita
                itensNoBuffer = atual + 1;     // ESCRITA sem proteção
                totalProduzido++;
                if (itensNoBuffer < 0 || itensNoBuffer > CAPACIDADE) violacoesCapacidade++;
            } else {
                producoesRejeitadas++;
            }
        }
    }

    static void consumir() {
        for (int i = 0; i < ITENS_POR_PRODUTOR; i++) {
            int atual = itensNoBuffer;        // LEITURA sem proteção
            Thread.yield();                   // amplia a janela da condição de corrida
            if (atual > 0) {
                itensNoBuffer = atual - 1;     // ESCRITA sem proteção
                totalConsumido++;
                if (itensNoBuffer < 0 || itensNoBuffer > CAPACIDADE) violacoesCapacidade++;
            } else {
                consumosRejeitados++;
            }
        }
    }
}
