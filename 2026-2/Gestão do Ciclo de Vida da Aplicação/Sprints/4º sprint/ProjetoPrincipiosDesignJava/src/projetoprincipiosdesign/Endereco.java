package projetoprincipiosdesign;

public class Endereco {
    private String logradouro;
    private Cidade cidade;

    public Endereco(String logradouro, Cidade cidade) {
        this.logradouro = logradouro;
        this.cidade = cidade;
    }

    public String getLogradouro() {
        return logradouro;
    }

    public void setLogradouro(String logradouro) {
        this.logradouro = logradouro;
    }

    public Cidade getCidade() {
        return cidade;
    }

    public void setCidade(Cidade cidade) {
        this.cidade = cidade;
    }

    // ETAPA 4 - Lei de Demeter:
    // Endereco esconde o detalhe de que o nome da cidade está dentro do objeto Cidade.
    // Quem precisar apenas do nome da cidade não precisa navegar até Cidade.
    public String getNomeCidade() {
        return cidade.getNome();
    }
}
