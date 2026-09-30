namespace Pedidos.Models;

// Dados necessários à página MVC.
public class PaginaModel
{
    public List<Jogo> Jogos { get; set; } = new List<Jogo>();
    public List<Pedido> Pedidos { get; set; } = new List<Pedido>();
    public string? Mensagem { get; set; }
}
