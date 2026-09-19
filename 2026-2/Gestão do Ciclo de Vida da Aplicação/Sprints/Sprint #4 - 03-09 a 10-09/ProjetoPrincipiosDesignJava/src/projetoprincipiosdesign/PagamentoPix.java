package projetoprincipiosdesign;

// ETAPA 2 - ISP: PIX só sabe pagar. Antes era obrigado a implementar parcelar()
// (lançando exceção) e gerarBoleto() (método vazio) só para satisfazer a interface antiga.
public class PagamentoPix implements IPagavel {
    @Override
    public void pagar(double valor) {
        System.out.printf("PIX pago: R$ %.2f%n", valor);
    }
}
