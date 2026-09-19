package projetoprincipiosdesign;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;

// ETAPA 1 - SRP: única responsabilidade desta classe é persistir o pedido em arquivo.
// PedidoService deixa de saber como/onde o pedido é salvo.
public class PedidoRepository {
    private static final Path ARQUIVO_PEDIDOS = Path.of("pedidos.txt");

    public void salvar(Pedido pedido, double total) {
        String linha = pedido.getCliente().getNome() + ";" + total + System.lineSeparator();

        try {
            Files.writeString(
                ARQUIVO_PEDIDOS,
                linha,
                StandardOpenOption.CREATE,
                StandardOpenOption.APPEND
            );
        } catch (IOException e) {
            throw new RuntimeException("Erro ao salvar o pedido em arquivo.", e);
        }
    }
}
