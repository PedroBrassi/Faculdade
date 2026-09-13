package projetoprincipiosdesign;

// ETAPA 2 - ISP: só faz sentido para formas de pagamento que aceitam parcelamento
// (no projeto original, PIX e Boleto eram forçados a ter esse método mesmo sem usá-lo).
public interface IParcelavel {
    void parcelar(double valor, int parcelas);
}
