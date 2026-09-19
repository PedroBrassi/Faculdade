package projetoprincipiosdesign;

// ETAPA 7 - LSP + "Composição sobre Herança":
// Antes, EntregaRetiradaLoja herdava de Entrega e quebrava o contrato lançando uma
// exceção quando o pedido não atingia R$ 50,00 - um cliente que só conhecesse a
// abstração Entrega seria surpreendido por um comportamento inesperado.
//
// Agora a classe não herda de Entrega, apenas implementa o mesmo contrato (IEntrega)
// e USA uma Entrega padrão por composição como alternativa quando a retirada não está
// disponível. calcularFrete() sempre devolve um valor válido, nunca lança exceção -
// qualquer código que dependa de IEntrega pode usar EntregaRetiradaLoja sem surpresas.
// Quem precisar saber previamente se a retirada gratuita está disponível pode consultar
// estaDisponivel(total) antes de decidir.
public class EntregaRetiradaLoja implements IEntrega {
    private final IEntrega entregaAlternativa;

    public EntregaRetiradaLoja() {
        this(new Entrega());
    }

    public EntregaRetiradaLoja(IEntrega entregaAlternativa) {
        this.entregaAlternativa = entregaAlternativa;
    }

    @Override
    public boolean estaDisponivel(double total) {
        return total >= 50.0;
    }

    @Override
    public double calcularFrete(double total) {
        if (estaDisponivel(total)) {
            return 0.0;
        }

        return entregaAlternativa.calcularFrete(total);
    }
}
