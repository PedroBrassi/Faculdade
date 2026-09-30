namespace Pedidos.Models;

// Dados que a página precisa para montar o catálogo e a lista de pedidos.
public class PaginaModel
{
    public List<Jogo> Jogos { get; set; } = new List<Jogo>();
    public List<Pedido> Pedidos { get; set; } = new List<Pedido>();
    public string? Mensagem { get; set; }
}
