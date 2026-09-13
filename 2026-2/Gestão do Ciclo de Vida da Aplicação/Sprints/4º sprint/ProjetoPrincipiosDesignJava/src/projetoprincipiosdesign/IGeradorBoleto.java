package projetoprincipiosdesign;

// ETAPA 2 - ISP: só faz sentido para a forma de pagamento por boleto.
public interface IGeradorBoleto {
    void gerarBoleto(double valor);
}
