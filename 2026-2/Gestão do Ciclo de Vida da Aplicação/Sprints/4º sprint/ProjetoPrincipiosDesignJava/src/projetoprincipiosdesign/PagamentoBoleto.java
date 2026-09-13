package projetoprincipiosdesign;

// ETAPA 2 - ISP: boleto sabe pagar (registrar a quitação) e gerar a linha digitável,
// mas nunca soube parcelar - por isso não depende mais de IParcelavel.
public class PagamentoBoleto implements IPagavel, IGeradorBoleto {
    @Override
    public void pagar(double valor) {
        System.out.printf("Boleto registrado: R$ %.2f%n", valor);
    }

    @Override
    public void gerarBoleto(double valor) {
        System.out.printf("Linha digitável gerada para R$ %.2f%n", valor);
    }
}
