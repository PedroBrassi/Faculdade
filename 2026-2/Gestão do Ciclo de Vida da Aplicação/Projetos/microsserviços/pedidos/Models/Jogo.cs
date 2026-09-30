namespace Pedidos.Models;

// Dados recebidos do Catálogo por HTTP.
public class Jogo
{
    public int Id { get; set; }
    public string Titulo { get; set; } = "";
    public decimal Preco { get; set; }
}
