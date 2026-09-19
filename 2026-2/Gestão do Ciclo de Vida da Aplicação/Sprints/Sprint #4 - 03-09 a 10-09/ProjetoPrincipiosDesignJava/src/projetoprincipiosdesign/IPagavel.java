package projetoprincipiosdesign;

// ETAPA 2 - ISP: operação comum a QUALQUER forma de pagamento.
public interface IPagavel {
    void pagar(double valor);
}
