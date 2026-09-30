namespace MvcJogos.Modelo;

public class Jogo
{
    // Em C#, dados são expostos por PROPRIEDADES (equivalem aos getters do Java).
    // "{ get; }" = somente leitura: o valor só pode ser definido no construtor.
    public int Id { get; }
    public string Titulo { get; }
    public string Franquia { get; }
    public string Plataforma { get; }
    public int Ano { get; }

    public Jogo(int id, string titulo, string franquia, string plataforma, int ano)
    {
        Id = id;
        Titulo = titulo;
        Franquia = franquia;
        Plataforma = plataforma;
        Ano = ano;
    }
}
