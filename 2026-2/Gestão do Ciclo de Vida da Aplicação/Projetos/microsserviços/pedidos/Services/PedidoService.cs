using Pedidos.ApiClients;
using Pedidos.Repositories;
using Pedidos.Models;
namespace Pedidos.Services;

// Regra simples: validar o pedido e consultar o Catálogo por HTTP.
public class PedidoService
{
    private readonly CatalogoClient _catalogo;
    private readonly PedidoRepository _repositorio;

    public PedidoService(CatalogoClient catalogo, PedidoRepository repositorio)
    {
        _catalogo = catalogo;
        _repositorio = repositorio;
    }

    public List<Pedido> Listar()
    {
        return _repositorio.Listar();
    }

    public async Task<Pedido> CriarAsync(string cliente, int jogoId, int quantidade)
    {
        if (string.IsNullOrWhiteSpace(cliente) || quantidade < 1)
            throw new ArgumentException("Informe o cliente e uma quantidade maior que zero.");

        Jogo? jogo = await _catalogo.BuscarAsync(jogoId);
        if (jogo == null)
            throw new ArgumentException("Jogo não encontrado no Catálogo.");

        return _repositorio.Salvar(cliente, jogo, quantidade);
    }
}
