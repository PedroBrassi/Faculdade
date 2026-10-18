using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using RetroGamesMVC.Models;
using RetroGamesMVC.Repositories;
using RetroGamesMVC.Services;

namespace RetroGamesMVC.Controllers;

public class HomeController : Controller
{
    private readonly JogoRepository _jogos;
    private readonly PedidoService _pedidos;

    public HomeController(JogoRepository jogos, PedidoService pedidos)
    {
        _jogos = jogos;
        _pedidos = pedidos;
    }

    [HttpGet]
    public IActionResult Index()
    {
        return View(PrepararPagina());
    }

    [HttpPost]
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

    public IActionResult Privacy()
    {
        return View();
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }

    private PaginaModel PrepararPagina()
    {
        return new PaginaModel
        {
            Jogos = _jogos.Listar(),
            Pedidos = _pedidos.Listar()
        };
    }
}
