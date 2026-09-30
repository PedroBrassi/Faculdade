namespace Pedidos.Models;

public class Pedido
{
    public int Id { get; set; }
    public string Cliente { get; set; }
    public string Jogo { get; set; }
    public int Quantidade { get; set; }

    public Pedido(int id, string cliente, string jogo, int quantidade)
    {
        Id = id;
        Cliente = cliente;
        Jogo = jogo;
        Quantidade = quantidade;
    }
}
