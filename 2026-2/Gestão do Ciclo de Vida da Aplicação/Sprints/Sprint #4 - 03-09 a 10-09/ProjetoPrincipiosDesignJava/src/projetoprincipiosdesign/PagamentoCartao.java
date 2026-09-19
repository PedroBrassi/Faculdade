package projetoprincipiosdesign;

// ETAPA 2 - ISP: cartão realmente paga e realmente parcela, então implementa as duas interfaces.
public class PagamentoCartao implements IPagavel, IParcelavel {
    @Override
    public void pagar(double valor) {
        System.out.printf("Pagamento no cartão: R$ %.2f%n", valor);
    }

    @Override
    public void parcelar(double valor, int parcelas) {
        System.out.printf(
            "Cartão parcelado em %dx de R$ %.2f%n",
            parcelas,
            valor / parcelas
        );
    }
}
