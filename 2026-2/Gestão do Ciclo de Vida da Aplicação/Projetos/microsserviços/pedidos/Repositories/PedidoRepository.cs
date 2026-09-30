using Pedidos.Models;
namespace Pedidos.Repositories;

// Guarda pedidos somente na memória DESTE processo. Reiniciar apaga os dados.
public class PedidoRepository
{
    private readonly List<Pedido> _pedidos = new List<Pedido>();
    private readonly object _controle = new();

    public List<Pedido> Listar()
    {
        lock (_controle) return new List<Pedido>(_pedidos);
    }

    public Pedido Salvar(string cliente, Jogo jogo, int quantidade)
    {
        lock (_controle)
        {
            var pedido = new Pedido(_pedidos.Count + 1, cliente, jogo.Titulo, quantidade);
            _pedidos.Add(pedido);
            return pedido;
        }
    }
}
