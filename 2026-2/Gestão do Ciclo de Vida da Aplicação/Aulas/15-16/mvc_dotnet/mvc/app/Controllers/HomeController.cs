using Pedidos.Repositories;
using Pedidos.Services;
using Pedidos.Models;
using Microsoft.AspNetCore.Mvc;

namespace Pedidos.Controllers;

// Controller: recebe a ação do navegador e escolhe a View e seus dados.
public class HomeController : Controller
{
    private readonly JogoRepository _jogos;
    private readonly PedidoService _pedidos;

    public HomeController(JogoRepository jogos, PedidoService pedidos)
    {
        _jogos = jogos;
        _pedidos = pedidos;
    }

    [HttpGet("/")]
    public IActionResult Index()
    {
        var pagina = PrepararPagina();
        return View(pagina);
    }

    [HttpPost("/pedidos")]
    [ValidateAntiForgeryToken]
    public IActionResult Criar(string cliente, int jogoId, int quantidade)
    {
        string mensagem;
        try
        {
            Pedido pedido = _pedidos.Criar(cliente, jogoId, quantidade);
            mensagem = $"Pedido #{pedido.Id} criado.";
        }
        catch (ArgumentException erro)
        {
            mensagem = erro.Message;
        }

        var pagina = PrepararPagina();
        pagina.Mensagem = mensagem;
        return View("Index", pagina);
    }

    private PaginaModel PrepararPagina()
    {
        var pagina = new PaginaModel();
        pagina.Jogos = _jogos.Listar();
        pagina.Pedidos = _pedidos.Listar();
        return pagina;
    }
}
