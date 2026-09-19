package projetoprincipiosdesign;

public class Cliente {
    private String nome;
    private Endereco endereco;

    public Cliente(String nome, Endereco endereco) {
        this.nome = nome;
        this.endereco = endereco;
    }

    public String getNome() {
        return nome;
    }

    public void setNome(String nome) {
        this.nome = nome;
    }

    public Endereco getEndereco() {
        return endereco;
    }

    public void setEndereco(Endereco endereco) {
        this.endereco = endereco;
    }

    // ETAPA 4 - Lei de Demeter:
    // Cliente conversa apenas com seu "amigo direto" (Endereco), que por sua vez
    // resolve o restante do caminho. Quem usa Cliente não precisa mais saber que
    // existe um Endereco e uma Cidade por trás disso.
    public String getCidadeEntrega() {
        return endereco.getNomeCidade();
    }
}
