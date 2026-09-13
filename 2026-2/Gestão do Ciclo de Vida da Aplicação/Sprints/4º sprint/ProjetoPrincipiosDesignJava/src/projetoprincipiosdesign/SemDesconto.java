package projetoprincipiosdesign;

// Representa clientes sem categoria de desconto (substitui o antigo "else" implícito
// do if/else original, quando tipoCliente não era ALUNO, PROFESSOR nem FUNCIONARIO).
public class SemDesconto implements CalculadoraDesconto {
    @Override
    public double aplicar(double total) {
        return total;
    }
}
