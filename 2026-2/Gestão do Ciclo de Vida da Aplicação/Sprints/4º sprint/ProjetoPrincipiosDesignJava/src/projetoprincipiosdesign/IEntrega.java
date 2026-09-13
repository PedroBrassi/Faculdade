package projetoprincipiosdesign;

// ETAPA 7 - LSP: o contrato agora deixa explícito que uma forma de entrega pode
// não estar disponível para um determinado total. calcularFrete() nunca lança exceção -
// qualquer implementação pode substituir outra sem quebrar quem a utiliza.
public interface IEntrega {
    boolean estaDisponivel(double total);
    double calcularFrete(double total);
}
