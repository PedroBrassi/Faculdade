package projetoprincipiosdesign;

public class PagamentoBoleto implements IPagamento {
    @Override
    public void pagar(double valor) {
        System.out.printf("Boleto registrado: R$ %.2f%n", valor);
    }

    @Override
    public void parcelar(double valor, int parcelas) {
        System.out.println("Operação não utilizada para boleto.");
    }

    @Override
    public void gerarBoleto(double valor) {
        System.out.printf("Linha digitável gerada para R$ %.2f%n", valor);
    }
}
