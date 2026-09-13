package projetoprincipiosdesign;

public class DescontoAluno implements CalculadoraDesconto {
    @Override
    public double aplicar(double total) {
        return total * 0.90;
    }
}
