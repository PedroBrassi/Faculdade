using Pedidos.Models;
namespace Pedidos.Repositories;

// Guarda pedidos na memória. Encerrar o aplicativo apaga os dados.
public class PedidoRepository
{
    private readonly List<Pedido> _pedidos = new List<Pedido>();

    public List<Pedido> Listar()
    {
        return new List<Pedido>(_pedidos);
    }

    public Pedido Salvar(string cliente, Jogo jogo, int quantidade)
    {
        var pedido = new Pedido(_pedidos.Count + 1, cliente, jogo.Titulo, quantidade);
        _pedidos.Add(pedido);
        return pedido;
    }
}
