namespace Pedidos.Models;

public class Jogo
{
    public int Id { get; set; }
    public string Titulo { get; set; }
    public decimal Preco { get; set; }

    public Jogo(int id, string titulo, decimal preco)
    {
        Id = id;
        Titulo = titulo;
        Preco = preco;
    }
}
