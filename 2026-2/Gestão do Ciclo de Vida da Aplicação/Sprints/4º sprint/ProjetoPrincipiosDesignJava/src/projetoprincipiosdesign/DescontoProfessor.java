package projetoprincipiosdesign;

public class DescontoProfessor implements CalculadoraDesconto {
    @Override
    public double aplicar(double total) {
        return total * 0.85;
    }
}
