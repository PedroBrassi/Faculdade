package projetoprincipiosdesign;

// ETAPA 3 - Composição sobre Herança / LSP:
// PedidoService NÃO herda mais de PagamentoCartao. Um serviço de pedidos não é um
// tipo de pagamento; ele apenas UTILIZA uma forma de pagamento. Essa relação agora
// é resolvida por composição, via parâmetro do tipo IPagavel (injeção de dependência).
//
// ETAPA 6 - OCP/DIP: finalizarPedido não decide mais qual classe de pagamento
// instanciar a partir de uma String (removidos os if/else CARTAO/PIX/BOLETO).
// Quem chama o serviço é quem fornece a implementação de IPagavel já pronta.
//
// ETAPA 5 - OCP/DIP: o mesmo vale para o desconto - calcularTotal recebe uma
// CalculadoraDesconto em vez de um texto com o tipo do cliente.
//
// ETAPA 1 - SRP: a responsabilidade de salvar em arquivo foi extraída para
// PedidoRepository. PedidoService continua orquestrando o fluxo do pedido
// (calcular, registrar, cobrar e apresentar o resumo), mas não sabe mais COMO
// persistir os dados.
public class PedidoService {
    private final PedidoRepository pedidoRepository;

    public PedidoService() {
        this(new PedidoRepository());
    }

    public PedidoService(PedidoRepository pedidoRepository) {
        this.pedidoRepository = pedidoRepository;
    }

    public double calcularTotal(Pedido pedido, CalculadoraDesconto desconto) {
        double total = 0.0;

        for (ItemPedido item : pedido.getItens()) {
            total += item.getPreco() * item.getQuantidade();
        }

        return desconto.aplicar(total);
    }

    // ETAPA 4 - Lei de Demeter: em vez de percorrer
    // pedido -> cliente -> endereco -> cidade -> nome, PedidoService fala apenas
    // com seu "amigo direto" (Cliente), que sabe informar sua própria cidade de entrega.
    public String obterCidadeEntrega(Pedido pedido) {
        return pedido.getCliente().getCidadeEntrega();
    }

    public void finalizarPedido(Pedido pedido, CalculadoraDesconto desconto, IPagavel formaPagamento) {
        double total = calcularTotal(pedido, desconto);

        System.out.println("Salvando pedido em arquivo...");
        pedidoRepository.salvar(pedido, total);

        System.out.println("Gerando resumo do pedido...");
        System.out.println("Cliente: " + pedido.getCliente().getNome());
        System.out.printf("Total: R$ %.2f%n", total);

        formaPagamento.pagar(total);

        System.out.println(
            "Enviando mensagem para " + pedido.getCliente().getNome() + ": pedido finalizado."
        );
    }
}
