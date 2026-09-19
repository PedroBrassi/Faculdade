package projetoprincipiosdesign;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;

public class PedidoService extends PagamentoCartao {
    public double calcularTotal(Pedido pedido, String tipoCliente) {
        double total = 0.0;

        for (ItemPedido item : pedido.getItens()) {
            total += item.getPreco() * item.getQuantidade();
        }

        if (tipoCliente.equals("ALUNO")) {
            total *= 0.90;
        } else if (tipoCliente.equals("PROFESSOR")) {
            total *= 0.85;
        } else if (tipoCliente.equals("FUNCIONARIO")) {
            total *= 0.80;
        }

        return total;
    }

    public String obterCidadeEntrega(Pedido pedido) {
        return pedido.getCliente().getEndereco().getCidade().getNome();
    }

    public void finalizarPedido(Pedido pedido, String formaPagamento) {
        double total = calcularTotal(pedido, "ALUNO");

        System.out.println("Salvando pedido em arquivo...");
        String linha = pedido.getCliente().getNome() + ";" + total + System.lineSeparator();

        try {
            Files.writeString(
                Path.of("pedidos.txt"),
                linha,
                StandardOpenOption.CREATE,
                StandardOpenOption.APPEND
            );
        } catch (IOException e) {
            throw new RuntimeException("Erro ao salvar o pedido em arquivo.", e);
        }

        System.out.println("Gerando resumo do pedido...");
        System.out.println("Cliente: " + pedido.getCliente().getNome());
        System.out.printf("Total: R$ %.2f%n", total);

        if (formaPagamento.equals("CARTAO")) {
            PagamentoCartao pagamento = new PagamentoCartao();
            pagamento.pagar(total);
        } else if (formaPagamento.equals("PIX")) {
            PagamentoPix pagamento = new PagamentoPix();
            pagamento.pagar(total);
        } else if (formaPagamento.equals("BOLETO")) {
            PagamentoBoleto pagamento = new PagamentoBoleto();
            pagamento.gerarBoleto(total);
        }

        System.out.println(
            "Enviando mensagem para " + pedido.getCliente().getNome() + ": pedido finalizado."
        );
    }
}
