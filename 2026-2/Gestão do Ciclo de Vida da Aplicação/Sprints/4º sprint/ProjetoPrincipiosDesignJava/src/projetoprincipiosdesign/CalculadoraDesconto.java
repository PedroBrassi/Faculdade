package projetoprincipiosdesign;

// ETAPA 5 - OCP/DIP: abstração para o cálculo de desconto.
// PedidoService passa a depender apenas desta interface, nunca de um tipo de cliente
// específico. Novos descontos = novas classes, sem tocar em PedidoService (Aberto/Fechado).
public interface CalculadoraDesconto {
    double aplicar(double total);
}
