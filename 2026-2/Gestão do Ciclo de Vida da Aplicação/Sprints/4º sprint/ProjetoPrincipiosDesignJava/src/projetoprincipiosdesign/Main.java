package projetoprincipiosdesign;

import java.util.List;

public class Main {
    public static void main(String[] args) {
        System.out.println("=== LOJA ACADÊMICA ===");

        Cliente cliente = new Cliente(
            "Ana",
            new Endereco(
                "Rua das Flores",
                new Cidade("Belo Horizonte")
            )
        );

        Pedido pedido = new Pedido(
            cliente,
            List.of(
                new ItemPedido("Livro de Engenharia de Software", 120.0, 1),
                new ItemPedido("Caderno", 20.0, 2)
            )
        );

        PedidoService servico = new PedidoService();

        System.out.println();
        System.out.println("Cidade de entrega:");
        System.out.println(servico.obterCidadeEntrega(pedido));

        // ETAPA 5/6 - quem decide qual desconto e qual forma de pagamento usar
        // é o código cliente (aqui, Main), não o PedidoService.
        CalculadoraDesconto desconto = new DescontoAluno();
        IPagavel formaPagamento = new PagamentoCartao();

        System.out.println();
        System.out.println("Total com desconto:");
        System.out.printf("R$ %.2f%n", servico.calcularTotal(pedido, desconto));

        System.out.println();
        System.out.println("Pagamento:");
        servico.finalizarPedido(pedido, desconto, formaPagamento);

        System.out.println();
        System.out.println("Programa executado com sucesso.");
    }
}
