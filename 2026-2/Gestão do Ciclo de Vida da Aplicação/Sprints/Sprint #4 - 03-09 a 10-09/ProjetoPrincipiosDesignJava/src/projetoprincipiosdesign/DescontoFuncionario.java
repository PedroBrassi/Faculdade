package projetoprincipiosdesign;

public class DescontoFuncionario implements CalculadoraDesconto {
    @Override
    public double aplicar(double total) {
        return total * 0.80;
    }
}
