package projetoprincipiosdesign;

public class EntregaRetiradaLoja extends Entrega {
    @Override
    public double calcularFrete(double total) {
        if (total < 50.0) {
            throw new IllegalStateException(
                "Retirada na loja disponível apenas para pedidos a partir de R$ 50,00."
            );
        }

        return 0.0;
    }
}
