
using Pedidos.Repositories;
using Pedidos.Models;
namespace Pedidos.Services;

// Valida o pedido e consulta o catálogo local.
public class PedidoService
{
    private readonly JogoRepository _jogos;
    private readonly PedidoRepository _repositorio;

    public PedidoService(JogoRepository jogos, PedidoRepository repositorio)
    {
        _jogos = jogos;
        _repositorio = repositorio;
    }

    public List<Pedido> Listar()
    {
        return _repositorio.Listar();
    }

    public Pedido Criar(string cliente, int jogoId, int quantidade)
    {
        if (string.IsNullOrWhiteSpace(cliente) || quantidade < 1)
            throw new ArgumentException("Informe o cliente e uma quantidade maior que zero.");

        Jogo? jogo = _jogos.Buscar(jogoId);
        if (jogo == null)
            throw new ArgumentException("Jogo não encontrado no Catálogo.");

        return _repositorio.Salvar(cliente, jogo, quantidade);
    }
}
